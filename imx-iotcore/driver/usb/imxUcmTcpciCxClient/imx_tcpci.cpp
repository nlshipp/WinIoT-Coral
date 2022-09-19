/*
* This file is derived from Microsoft example available on https://github.com/microsoft/Windows-driver-samples/blob/master/usb/UcmTcpciCxClientSample
* Original file licensing:
* Copyright (c) 2015 Microsoft
* SPDX-License-Identifier: MS-PL
* NXP modifications are licensed under the same license
* Copyright 2020 NXP
*
*/

#include "imx_tcpci.h"
#if (TCPCI_TRACE == TCPCI_TRACE_WPP)
#include "imx_tcpci.tmh"
#endif

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, EvtDriverContextCleanup)
#pragma alloc_text (PAGE, EvtDeviceAdd)
#pragma alloc_text (PAGE, EvtPrepareHardware)
#pragma alloc_text (PAGE, EvtDeviceD0Entry)
#pragma alloc_text (PAGE, EvtReleaseHardware)
#pragma alloc_text (PAGE, OnInterruptPassiveIsr)
#endif

/*++
Routine Description:
    DriverEntry initializes the driver and is the first routine called by the system after the driver is loaded. DriverEntry specifies the other entry
    points in the function driver, such as EvtDevice and DriverUnload.
Parameters Description:
    DriverObject - represents the instance of the function driver that is loaded into memory. DriverEntry must initialize members of DriverObject before it
                   returns to the caller. DriverObject is allocated by the system before the driver is loaded, and it is released by the system after the system unloads
                   the function driver from memory.
    RegistryPath - represents the driver specific path in the Registry. The function driver can use the path to store driver related data between
                   reboots. The path does not store hardware instance specific data.
Return Value:
    NTSTATUS
--*/
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  pDriverObject, _In_ PUNICODE_STRING pRegistryPath) {
    NTSTATUS               ntStatus=STATUS_SUCCESS;
    WDF_DRIVER_CONFIG      Config;
    WDF_OBJECT_ATTRIBUTES  Attributes;

    WDF_OBJECT_ATTRIBUTES_INIT(&Attributes);
    /* Initialize WPP Tracing */
    WPP_INIT_TRACING(pDriverObject, pRegistryPath);
    DBG_DRV_METHOD_BEG_WITH_PARAMS("Driver: 0x%016p, '%S'", pDriverObject, ((PUNICODE_STRING)pRegistryPath)->Buffer);
    DBG_DRV_PRINT_VERBOSE("***********************************************************************************");
#ifdef __DATE__
    DBG_DRV_PRINT_VERBOSE("*** NXP USB TCPC driver, date: %s %s                             ***", __DATE__, __TIME__);
#else
    DBG_DRV_PRINT_VERBOSE("*** NXP USB TCPC driver                                          ***");
#endif
    DBG_DRV_PRINT_VERBOSE("***********************************************************************************");
    Attributes.EvtCleanupCallback = EvtDriverContextCleanup;
    WDF_DRIVER_CONFIG_INIT(&Config, EvtDeviceAdd);
    if (!NT_SUCCESS(ntStatus = WdfDriverCreate(pDriverObject, pRegistryPath, &Attributes, &Config, WDF_NO_HANDLE))) {
        DBG_PRINT_ERROR_WITH_STATUS(ntStatus,"WdfDriverCreate() failed.");
        WPP_CLEANUP(pDriverObject);
    }
    DBG_DRV_METHOD_END_WITH_STATUS(ntStatus);
    return ntStatus;
}

/*++
Routine Description:
    Free all the resources allocated in DriverEntry.
Arguments:
    DriverObject - handle to a WDF Driver object.
--*/
VOID EvtDriverContextCleanup(_In_ WDFOBJECT hDriverObject) {
    UNREFERENCED_PARAMETER(hDriverObject);
    _IRQL_limited_to_(PASSIVE_LEVEL);
    DBG_DRV_METHOD_BEG();
    PAGED_CODE();
    /* Stop WPP Tracing */
    DBG_DRV_METHOD_END();
    WPP_CLEANUP(WdfDriverWdmGetDriverObject((WDFDRIVER)hDriverObject));
}

/*++
Routine Description:
    It is called by the framework in response to AddDevice call from the PnP manager. We create and initialize a device object to represent a new instance of the device.
Arguments:
    Driver     - Handle to a framework driver object created in DriverEntry
    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.
Return Value:
    NTSTATUS
--*/
NTSTATUS EvtDeviceAdd(_In_ WDFDRIVER hDriver, _Inout_ PWDFDEVICE_INIT pDeviceInit) {
    NTSTATUS                     ntStatus;
    WDFDEVICE                    hDevice;
    PDEV_CONTEXT                 pDevContext;
    UCMTCPCI_DEVICE_CONFIG       Config;
    WDF_PNPPOWER_EVENT_CALLBACKS PnpPowerCallbacks;
    WDF_OBJECT_ATTRIBUTES        Attributes;

    DBG_DEV_METHOD_BEG();
    UNREFERENCED_PARAMETER(hDriver);
    PAGED_CODE();
    do {
        WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&Attributes, DEV_CONTEXT);
        WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&PnpPowerCallbacks);
        PnpPowerCallbacks.EvtDevicePrepareHardware = EvtPrepareHardware;
        PnpPowerCallbacks.EvtDeviceReleaseHardware = EvtReleaseHardware;
        PnpPowerCallbacks.EvtDeviceD0Entry         = EvtDeviceD0Entry;
        WdfDeviceInitSetPnpPowerEventCallbacks(pDeviceInit, &PnpPowerCallbacks);
        if (!NT_SUCCESS(ntStatus = UcmTcpciDeviceInitInitialize(pDeviceInit))) {
            DBG_PRINT_ERROR_WITH_STATUS(ntStatus, "[PWDFDEVICE_INIT: 0x%p] UcmTcpciDeviceInitInitialize failed", pDeviceInit);
            break;
        }
        if (!NT_SUCCESS(ntStatus = WdfDeviceCreate(&pDeviceInit, &Attributes, &hDevice))) {
            DBG_PRINT_ERROR_WITH_STATUS(ntStatus, "[PWDFDEVICE_INIT: 0x%p] WdfDeviceCreate failed", pDeviceInit);
            break;
        }
        pDevContext = DeviceGetContext(hDevice);
        pDevContext->Device  = hDevice;                                                /* Save the device handle in the context. */
        UCMTCPCI_DEVICE_CONFIG_INIT(&Config);
        if (!NT_SUCCESS(ntStatus = UcmTcpciDeviceInitialize(hDevice, &Config))) {      /* Register this device with UcmTcpciCx. */
            DBG_PRINT_ERROR_WITH_STATUS(ntStatus, "[WDFDEVICE: 0x%p] UcmTcpciDeviceInitialize failed", hDevice);
            break;
        }
    } while (0);
    DBG_DEV_METHOD_END_WITH_STATUS(ntStatus);
    return ntStatus;
}

/*++
Routine Description:
    A driver's EvtDevicePrepareHardware event callback function performs any operations that are needed to make a device accessible to the driver.
Arguments:
    hDevice             - Handle to a framework device object.
    ResourcesRaw        - Handle to a framework resource-list object that identifies the raw hardware resources that the Plug and Play manager has assigned to the device.
    ResourcesTranslated - Handle to a framework resource-list object that identifies the translated hardware resources that the Plug and Play manager has assigned to the device.
Return Value:
    NTSTATUS
--*/
NTSTATUS EvtPrepareHardware(_In_ WDFDEVICE hDevice, _In_ WDFCMRESLIST hResourcesRaw, _In_ WDFCMRESLIST hResourcesTranslated) {
    NTSTATUS     ntStatus;
    PDEV_CONTEXT pDevContext;

    DBG_DEV_METHOD_BEG();
    PAGED_CODE();
    pDevContext = DeviceGetContext(hDevice);
    KeInitializeEvent(&pDevContext->IoctlAndIsrSyncEvent, SynchronizationEvent, TRUE);
    do {
        if (!NT_SUCCESS(ntStatus = IO_Initialize(pDevContext, hResourcesRaw, hResourcesTranslated))) {  /* Initialize the I2C communication channel to read from/write to the hardware. */
            break;
        }
        if (!NT_SUCCESS(ntStatus = I2C_Open(pDevContext))) {
            break;
        }
        if (!NT_SUCCESS(ntStatus = GPIO_Open(pDevContext))) {
            break;
        }
    } while (0);
    DBG_DEV_METHOD_END_WITH_STATUS(ntStatus);
    return ntStatus;
}

NTSTATUS GetStatusFrom5150(_In_ WDFDEVICE hDevice,
    _In_ UINT32 idxCaller,
    _Out_ PUCMTCPCI_PORT_CONTROLLER_CC_STATUS pCCStatus,
    _Out_ PUCMTCPCI_PORT_CONTROLLER_POWER_STATUS pPowerStatus,
    _Out_ PUCMTCPCI_PORT_CONTROLLER_FAULT_STATUS pFaultStatus)
{
    I2C_IO_CMD_t I2C_Cmd;
    TCPC_PHY_CC_STATUS_t ccStatus;
    NTSTATUS ntStatus;
    PDEV_CONTEXT pDevContext;
    pDevContext = DeviceGetContext(hDevice);

    // get 5150 status
    if (!NT_SUCCESS(ntStatus = RdRegSync(TCPC_PHY_CC_STATUS, &(ccStatus.R), idxCaller))) {
        return ntStatus;
    }

    pCCStatus->AsUInt8 = 0;
    pPowerStatus->AsUInt8 = 0;
    pFaultStatus->AsUInt8 = 0;

    if (ccStatus.B.CC_POLARITY == 0) // no cable
    {
        pCCStatus->Looking4Connection = 1;
        pCCStatus->ConnectResult = 0;  // Rp presented
        pCCStatus->CC2State = 0; // SRC.Open(open, Rp)
        pCCStatus->CC1State = 0; // SRC.Open(open, Rp)

        pPowerStatus->SourcingVbus = 1;
        pPowerStatus->VbusPresentDetectionEnabled = 1;
        pPowerStatus->VbusPresent = ccStatus.B.VBUS_DETECT;
    }
    else if (ccStatus.B.PORT_ATTACH_STATUS == 1)  // DFP attached (set to HOST mode)
    {
        // BUGBUG: ignore CC polarity for now
        pCCStatus->Looking4Connection = 0;
        pCCStatus->ConnectResult = 0;  // Rp presented
        pCCStatus->CC2State = 0; // SRC.Open(open, Rp)
        pCCStatus->CC1State = 2; // SRC.Rd(within the vRd range)

        pPowerStatus->SourcingVbus = 1;
        pPowerStatus->VbusPresentDetectionEnabled = 1;
        pPowerStatus->VbusPresent = ccStatus.B.VBUS_DETECT;
    }
    else // if (ccStatus.B.PORT_ATTACH_STATUS == 0)  // UFP attached (set to DEVICE mode)
    {
        pCCStatus->Looking4Connection = 0;
        pCCStatus->ConnectResult = 1;  // Rp presented
        pCCStatus->CC2State = 0; // SNK.Open(below maximum vRa)
        pCCStatus->CC1State = 1; // SNK.Default(above minimum vRd - Connect)

        pPowerStatus->VbusPresentDetectionEnabled = 1;
        pPowerStatus->VbusPresent = ccStatus.B.VBUS_DETECT;
        pPowerStatus->SinkingVbus = 1;
    }

    return ntStatus;
}

/*++
Routine Description:
    This routine reads basic information from TCCI controller and starts TCPC stack.
Arguments:
    hDevice       - Handle to a framework device object.
    PreviousState - Previous state (not used).
Return Value:
    NTSTATUS
--*/
#ifdef PTN5150
NTSTATUS EvtDeviceD0Entry(_In_ WDFDEVICE hDevice, _In_ WDF_POWER_DEVICE_STATE PreviousState) {
    NTSTATUS                                ntStatus;
    PDEV_CONTEXT                            pDevContext;
    UCMTCPCIPORTCONTROLLER                  hPortController = WDF_NO_HANDLE;
    WDFQUEUE                                hQueue;
    WDF_IO_QUEUE_CONFIG                     QueueConfig;
    UCMTCPCI_PORT_CONTROLLER_CONFIG         Config;
    UCMTCPCI_PORT_CONTROLLER_IDENTIFICATION Ident;
    UCMTCPCI_PORT_CONTROLLER_CAPABILITIES   Capabilities;
    TCPC_PHY_VERSION_VENDOR_ID_t            VendorVersionId;
    I2C_IO_CMD_t                            I2C_Cmd;

    TCPC_PHY_INT_MASK_t intMask;
    TCPC_PHY_CONTROL_t control;
//    TCPC_PHY_RESET_t reset;


    DBG_DEV_METHOD_BEG();
    UNREFERENCED_PARAMETER(PreviousState);
    PAGED_CODE();
    pDevContext = DeviceGetContext(hDevice);
    UCMTCPCI_PORT_CONTROLLER_CONFIG_INIT(&Config, &Ident, &Capabilities);
    UCMTCPCI_PORT_CONTROLLER_IDENTIFICATION_INIT(&Ident);
    UCMTCPCI_PORT_CONTROLLER_CAPABILITIES_INIT(&Capabilities);

    I2C_IO_CMD_t  DevCapabilities_I2C_IO_Cmds[] = {
        {&intMask,          IMX_EvtDeviceD0Entry_DevCapabilities, I2C_IO_CMD_RD_REG_SYNC(TCPC_PHY_INT_MASK) },
        {&control,          IMX_EvtDeviceD0Entry_DevCapabilities, I2C_IO_CMD_RD_REG_SYNC(TCPC_PHY_CONTROL) },
        {&VendorVersionId,  IMX_EvtDeviceD0Entry_DevCapabilities, I2C_IO_CMD_RD_REG_SYNC(TCPC_PHY_VERSION_VENDOR_ID) | I2C_IO_CMD_LAST_CMD}
    };
    do {
        TCPC_PHY_ReadAllRegs(pDevContext,__FUNCTION__);
        if (!NT_SUCCESS(ntStatus = I2C_RegsIo(pDevContext, DevCapabilities_I2C_IO_Cmds))) {
            DBG_DEV_PRINT_ERROR_WITH_STATUS(ntStatus, "Get TCPC controller DevCapabilities() failed.");
        }
        if (VendorVersionId.B.VENDOR != 3 || VendorVersionId.B.VERSION != 1)
        {
            ntStatus = STATUS_NO_SUCH_DEVICE;
            DBG_DEV_PRINT_ERROR_WITH_STATUS(ntStatus, "[WDFDEVICE: 0x%p] UcmTcpciPortControllerCreate failed.", hDevice);
            break;

        }

        if (!NT_SUCCESS(ntStatus = RdRegSync(TCPC_PHY_INT_MASK, &intMask.R, IMX_EvtDeviceD0Entry))) {
            break;
        }
        if (!NT_SUCCESS(ntStatus = RdRegSync(TCPC_PHY_CONTROL, &control.R, IMX_EvtDeviceD0Entry))) {
            break;
        }

        // PTN5150 is not PD compliant. Fill out the identy and capability structures
        Ident.VendorId = 0x1fc9; // NXP
        Ident.ProductId = 0x5150; // PTN5150
        Ident.DeviceId = 0x0001; // V1
        Ident.TypeCRevisionInBcd = 0x011; // USB Type-C rev 1.1
        Ident.PDInterfaceRevisionAndVersionInBcd = 0x00;
        Ident.PDRevisionAndVersionInBcd = 0x00;

        Capabilities.IsPowerDeliveryCapable = 0;
        Capabilities.DeviceCapabilities1.SourceVbus = 1;
        Capabilities.DeviceCapabilities1.SourceHighVoltageVbus = 0;
        Capabilities.DeviceCapabilities1.SinkVbus = 1;
        Capabilities.DeviceCapabilities1.SourceVconn = 0;
        Capabilities.DeviceCapabilities1.SOPDbgSupport = 0;
        Capabilities.DeviceCapabilities1.RolesSupported = 0x06; // Source, Sink, DRP
        Capabilities.DeviceCapabilities1.SourceResistorSupported = 0x02;  // Rp 3.0A 1.5A, default
        Capabilities.DeviceCapabilities1.VbusMeasurementAndAlarmCapable = 0;
        Capabilities.DeviceCapabilities1.ForceDischarge = 0;
        Capabilities.DeviceCapabilities1.BleedDischarge = 0;
        Capabilities.DeviceCapabilities1.VbusOVPReporting = 0;
        Capabilities.DeviceCapabilities1.VbusOCPReporting = 0;

        Capabilities.DeviceCapabilities2.VconnOvercurrentFaultCapable = 0;
        Capabilities.DeviceCapabilities2.VconnPowerSupported = 0; // 1.0W
        Capabilities.DeviceCapabilities2.VbusVoltageAlarmLSB = 0;
        Capabilities.DeviceCapabilities2.StopDischargeThreshold = 0;
        Capabilities.DeviceCapabilities2.SinkDisconnectDetection = 0;

        /*
        Capabilities.StandardInputCapabilities.ForceOffVbus = 0;
        Capabilities.StandardInputCapabilities.VbusExternalOverCurrentFault = 0;
        Capabilities.StandardInputCapabilities.VbusExternalOverVoltageFault = 0;
        */

        Capabilities.StandardOutputCapabilities.ConnectorOrientation = 1;
        Capabilities.StandardOutputCapabilities.ConnectionPresent = 1;
        Capabilities.StandardOutputCapabilities.AudioAdapterAccessoryIndicator = 1;
        Capabilities.StandardOutputCapabilities.VbusPresentMonitor = 1;

        /* Create a UCMTCPCIPORTCONTROLLER framework object. */
        if (!NT_SUCCESS(ntStatus = UcmTcpciPortControllerCreate(hDevice, &Config, WDF_NO_OBJECT_ATTRIBUTES, &hPortController))) {
            DBG_DEV_PRINT_ERROR_WITH_STATUS(ntStatus, "[WDFDEVICE: 0x%p] UcmTcpciPortControllerCreate failed.", hDevice);
            break;
        }
        pDevContext = DeviceGetContext(hDevice);                            /* Save the UCMTCPCIPORTCONTROLLER in our device context. */
        pDevContext->PortController = hPortController;
        /* Set the hardware request queue for our device. A single I/O Queue is configured for sequential request processing, and a driver context memory allocation is created to hold our structure QUEUE_CONTEXT. */
        WDF_IO_QUEUE_CONFIG_INIT(&QueueConfig, WdfIoQueueDispatchSequential);
        QueueConfig.EvtIoDeviceControl = EvtIoDeviceControl;
        QueueConfig.EvtIoStop          = EvtIoStop;
        if (!NT_SUCCESS(ntStatus = WdfIoQueueCreate(hDevice, &QueueConfig, WDF_NO_OBJECT_ATTRIBUTES, &hQueue))) {  /* Create the hardware request queue. */
            DBG_DEV_PRINT_ERROR_WITH_STATUS(ntStatus, "WdfIoQueueCreate failed.");
            break;
        }
        UcmTcpciPortControllerSetHardwareRequestQueue(pDevContext->PortController, hQueue);                        /* Set this queue as the one to which UcmTcpciCx will forward its hardware requests. */
        /* Direct UcmTcpciCx to start the port controller. At this point, UcmTcpciCx will assume control of USB Type-C and Power Delivery. */
        /* After the port controller is started, UcmTcpciCx may start putting requests into the hardware request queue. */
        if (!NT_SUCCESS(ntStatus = UcmTcpciPortControllerStart(hPortController))) {
            DBG_DEV_PRINT_ERROR_WITH_STATUS(ntStatus, "[UCMTCPCIPORTCONTROLLER: 0x%p] UcmTcpciPortControllerStart failed.", hPortController);
            break;
        }

        // unmask interrupts and set Dual Role Port mode
        intMask.B.ROLE_CHANGE = 0;
        control.B.CABLE_INT_MASK = 0;
        control.B.MODE_SELECT = 0x02;  // DRP

        if (!NT_SUCCESS(ntStatus = WrRegSync(TCPC_PHY_INT_MASK, intMask.R, IMX_EvtDeviceD0Entry))) {
            break;
        }

        if (!NT_SUCCESS(ntStatus = WrRegSync(TCPC_PHY_CONTROL, control.R, IMX_EvtDeviceD0Entry))) {
            break;
        }
    } while (0);
    if (!NT_SUCCESS(ntStatus) && (hPortController != WDF_NO_HANDLE)) {
        WdfObjectDelete(hPortController);
        pDevContext->PortController = WDF_NO_HANDLE;
    }
    DBG_DEV_METHOD_END_WITH_STATUS(ntStatus);
    return ntStatus;
}

#endif /* PTN5150 */

/*++
Routine Description:
    This event is invoked for a power-managed queue before the hDevice leaves the working state (D0).
Arguments:
    Queue       - Handle to the framework queue object that is associated with the I/O request.
    Request     - Handle to a framework request object.
    ActionFlags - A bitwise OR of one or more WDF_REQUEST_STOP_ACTION_FLAGS-typed flags that identify the reason that the callback function is being called and whether the request is cancelable.
--*/
VOID EvtIoStop(_In_ WDFQUEUE hQueue, _In_ WDFREQUEST hRequest, _In_ ULONG ActionFlags) {
    WDFDEVICE    hDevice;
    PDEV_CONTEXT pDevContext;
    I2C_IO_CMD_t I2C_Cmd;

    DBG_IOCTL_METHOD_BEG();
    UNREFERENCED_PARAMETER(ActionFlags);
    UNREFERENCED_PARAMETER(hRequest);
    hDevice = WdfIoQueueGetDevice(hQueue);
    pDevContext = DeviceGetContext(hDevice);
    if (pDevContext->IOCTL_hRequest != WDF_NO_HANDLE) {          /* Try to cancel the I2C WDFREQUESTs. */
        DBG_PRINT_ERROR("[WDFREQUEST: 0x%p] Attempting to cancel.", pDevContext->IOCTL_hRequest);
        WdfRequestCancelSentRequest(pDevContext->IOCTL_hRequest);
    }

    // reset the TCPC PHY
    TCPC_PHY_RESET_t reset;
    reset.R = 0;
    reset.B.RESET = 1;
    WrRegSync(TCPC_PHY_RESET, reset.R, IMX_EvtDeviceD0Entry);

    DBG_IOCTL_METHOD_END();
}

/*++
Routine Description:
    A driver's EvtDeviceReleaseHardware event callback function performs operations that are needed when a device is no longer accessible.
Arguments:
    hDevice             - Handle to a framework device object.
    ResourcesTranslated - Handle to a resource list object that identifies the translated hardware resources that the Plug and Play manager has assigned to the device.
Return Value:
    NTSTATUS
--*/
NTSTATUS EvtReleaseHardware(_In_ WDFDEVICE hDevice, _In_ WDFCMRESLIST hResourcesTranslated) {
    NTSTATUS     ntStatus = STATUS_SUCCESS;
    PDEV_CONTEXT pDevContext;

    DBG_DEV_METHOD_BEG();
    UNREFERENCED_PARAMETER(hResourcesTranslated);
    PAGED_CODE();
    pDevContext = DeviceGetContext(hDevice);
    if (pDevContext->PortController != WDF_NO_HANDLE)  {
        UcmTcpciPortControllerStop(pDevContext->PortController);   /* Direct UcmTcpciCx to stop the port controller and then delete the backing object. */
        WdfObjectDelete(pDevContext->PortController);
        pDevContext->PortController = WDF_NO_HANDLE;
    }
    I2C_Close(pDevContext);                                         /* Close the I2C controller. */
    GPIO_Close(pDevContext);                                        /* Close the GPIO controller. */
    DBG_DEV_METHOD_END_WITH_STATUS(ntStatus);
    return ntStatus;
}

/*++
Routine Description:
    Per the TCPCI spec, the port controller hardware will drive the Alert pin high when a hardware event occurs. This routine services such a hardware interrupt at PASSIVE_LEVEL.
    The routine determines if an interrupt is an alert from the port controller hardware; if so, it completes processing of the alert.
Arguments:
    Interrupt: A handle to a framework interrupt object.
    MessageID: If the device is using message-signaled interrupts (MSIs), this parameter is the message number that identifies the device's hardware interrupt message. Otherwise, this value is 0.
Return Value:
    TRUE if the function services the hardware interrupt. Otherwise, this function must return FALSE.
--*/

#ifdef PTN5150

BOOLEAN OnInterruptPassiveIsr(_In_ WDFINTERRUPT hPortControllerInterrupt, _In_ ULONG MessageID) {
    UNREFERENCED_PARAMETER(MessageID);
    PAGED_CODE();

    NTSTATUS                                ntStatus;
    PDEV_CONTEXT                            pDevContext;
    BOOLEAN                                 interruptRecognized = FALSE;
    int                                     NumAlertReports = 0;
    size_t                                  NumAlertsInReport;
    UCMTCPCI_PORT_CONTROLLER_ALERT_DATA     alertData;
//    UCMTCPCI_PORT_CONTROLLER_RECEIVE_BUFFER receiveBuffer;
    UCMTCPCI_PORT_CONTROLLER_ALERT_DATA     hardwareAlerts[ISR_MAX_ALERTS_TO_REPORT];  /* UcmTcpciCx expects the information on all of the alerts firing presently. */
    I2C_IO_CMD_t                            I2C_Cmd;

    TCPC_PHY_INT_STATUS_t                   intStatus;
    TCPC_PHY_CABLE_INT_STATUS_t             cableIntStatus;

    pDevContext = DeviceGetContext(WdfInterruptGetDevice(hPortControllerInterrupt));

    DBG_IOCTL_METHOD_BEG_WITH_PARAMS("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    /* Process the alerts as long as there are bits set in the alert register. Set a maximum number of alerts to process in this loop. If the hardware is messed up and we're unable */
    /* to quiesce the interrupt by writing to the alert register, then we don't want to be stuck in an infinite loop. */
    ntStatus = KeWaitForSingleObject(&pDevContext->IoctlAndIsrSyncEvent, Executive, KernelMode, FALSE, NULL);
    if (!NT_SUCCESS(ntStatus))
    {
        DBG_IOCTL_PRINT_ERROR_WITH_STATUS(ntStatus, "KeWaitForSingleObject failed");
        return FALSE;
    }

    while (NumAlertReports <= ISR_MAX_ALERTS_TO_PROCESS) {
        // Reading INT_STATUS and CABLE_INT_STATUS clears interrupt flags
        if (!NT_SUCCESS(ntStatus = RdRegSync(TCPC_PHY_INT_STATUS, &(intStatus.R), IMX_ISR))) {
            break;
        }
        if (!NT_SUCCESS(ntStatus = RdRegSync(TCPC_PHY_CABLE_INT_STATUS, &(cableIntStatus.R), IMX_ISR))) {
            break;
        }

        if ((intStatus.R == 0) && (cableIntStatus.R == 0)) {  /* If there are no bits set in the alert register, we should not service this interrupt. */
            break;
        }
        interruptRecognized = TRUE;         /* Since there are bits set in the alert register, we can safely assume that the interrupt is ours to process. */
        NumAlertsInReport = 0;
        do {
            UCMTCPCI_PORT_CONTROLLER_CC_STATUS CCStatus;
            UCMTCPCI_PORT_CONTROLLER_POWER_STATUS PowerStatus;
            UCMTCPCI_PORT_CONTROLLER_FAULT_STATUS FaultStatus;
            bool fChange = false;

            if (intStatus.B.ROLE_CHANGE == 1) {
                DBG_IOCTL_PRINT_INFO("ROLE_CHANGE == 1");
                fChange = true;
            }
            if (cableIntStatus.B.CABLE_ATTACH_INT == 1) {
                DBG_IOCTL_PRINT_INFO("CABLE_ATTACH_INT == 1");
                fChange = true;
            }
            if (cableIntStatus.B.CABLE_DETACH_INT == 1) {
                DBG_IOCTL_PRINT_INFO("CABLE_DETACH_INT == 1");
                fChange = true;
            }

            if (fChange) {
                if (!NT_SUCCESS(ntStatus = GetStatusFrom5150(WdfInterruptGetDevice(hPortControllerInterrupt), IMX_ISR, &CCStatus, &PowerStatus, &FaultStatus))) {
                    break;
                }

                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertCCStatus;
                alertData.CCStatus = CCStatus;
                hardwareAlerts[NumAlertsInReport++] = alertData;

                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertPowerStatus;
                alertData.PowerStatus = PowerStatus;
                hardwareAlerts[NumAlertsInReport++] = alertData;
            }
        } while (0);
        if (NT_SUCCESS(ntStatus)) {
            if (NumAlertsInReport) {
                DBG_IOCTL_PRINT_INFO("!!!!!!!!! ->> UcmTcpciPortControllerAlert +++ !!!!!!!!! ");
                UcmTcpciPortControllerAlert(pDevContext->PortController, hardwareAlerts, NumAlertsInReport);
                DBG_IOCTL_PRINT_INFO("!!!!!!!!! <<- UcmTcpciPortControllerAlert --- !!!!!!!!! ");
            }
        } else {
            break;
        }
        ++NumAlertReports;
    }

    DBG_IOCTL_CMD_DUMP("ISR_DONE: IOCTL pending, Setting event ...");
    KeSetEvent(&pDevContext->IoctlAndIsrSyncEvent, 0, FALSE);                                      /* Wake up passive ISR */
    DBG_IOCTL_METHOD_END_WITH_PARAMS("interruptRecognized = %d -----------------------------------", interruptRecognized);
    return interruptRecognized;
}

#endif /* PTN5150 */
/*++
Routine Description:
    This event is invoked when the framework receives IRP_MJ_DEVICE_CONTROL request.
Arguments:
    hQueue             - Handle to the framework queue object that is associated with the I/O request.
    hRequest           - Handle to a framework request object.
    OutputBufferLength - Size of the output buffer in bytes
    InputBufferLength  - Size of the input buffer in bytes
    IoControlCode      - I/O control code.
--*/

#ifdef PTN5150

VOID EvtIoDeviceControl(_In_ WDFQUEUE hQueue, _In_ WDFREQUEST hRequest, _In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode) {
    DEV_CONTEXT    *pDevContext;
    NTSTATUS        ntStatus = STATUS_SUCCESS;
    VOID           *pBuffer;
    I2C_IO_CMD_t   *pI2CIOCmd;

    DBG_IOCTL_METHOD_BEG_WITH_PARAMS("%s", Dbg_GetIOCTLName(IoControlCode));
    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(OutputBufferLength);

    pDevContext = DeviceGetContext(WdfIoQueueGetDevice(hQueue));
    ASSERT(pDevContext->IOCTL_hRequest == WDF_NO_HANDLE);          /* Only one IOCTL request is allowed */
    pI2CIOCmd = pDevContext->IOCTL_I2CCmds;
    DBG_IOCTL_CMD_DUMP("+++ WDFREQUEST: 0x%p, Ioctl:%s", hRequest, Dbg_GetIOCTLName(IoControlCode));

    ntStatus = KeWaitForSingleObject(&pDevContext->IoctlAndIsrSyncEvent, Executive, KernelMode, FALSE, NULL);
    if (!NT_SUCCESS(ntStatus))
    {
        DBG_IOCTL_PRINT_ERROR_WITH_STATUS(ntStatus, "KeWaitForSingleObject failed");
        WdfRequestComplete(hRequest, ntStatus);
        return;
    }

    switch (IoControlCode) {
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_GET_STATUS:
            if (NT_SUCCESS(ntStatus = WdfRequestRetrieveOutputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_GET_STATUS_OUT_PARAMS), &pBuffer, NULL))) {
                // get status
                PUCMTCPCI_PORT_CONTROLLER_GET_STATUS_OUT_PARAMS params = (PUCMTCPCI_PORT_CONTROLLER_GET_STATUS_OUT_PARAMS)pBuffer;
                if (!NT_SUCCESS(ntStatus = GetStatusFrom5150(WdfIoQueueGetDevice(hQueue), IMX_IOCTL_GET_STATUS, &params->CCStatus, &params->PowerStatus, &params->FaultStatus))) {
                    break;
                }
                WdfRequestSetInformation(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_GET_STATUS_OUT_PARAMS));
            }

            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_GET_CONTROL:
            if (NT_SUCCESS(ntStatus = WdfRequestRetrieveOutputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_GET_CONTROL_OUT_PARAMS), &pBuffer, NULL))) {
                // get control
                PUCMTCPCI_PORT_CONTROLLER_GET_CONTROL_OUT_PARAMS params = (PUCMTCPCI_PORT_CONTROLLER_GET_CONTROL_OUT_PARAMS)pBuffer;
                params->TCPCControl.AsUInt8 = 0;
                params->RoleControl.AsUInt8 = 0;
                params->FaultControl.AsUInt8 = 0;
                params->PowerControl.AsUInt8 = 0;

                WdfRequestSetInformation(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_GET_CONTROL_OUT_PARAMS));
            }
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_CONTROL:
            if (NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_CONTROL_IN_PARAMS), &pBuffer, NULL))) {
                PUCMTCPCI_PORT_CONTROLLER_SET_CONTROL_IN_PARAMS pParams = (UCMTCPCI_PORT_CONTROLLER_SET_CONTROL_IN_PARAMS *)pBuffer;
                switch (pParams->ControlType) {
                    case UcmTcpciPortControllerTcpcControl:
                        DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_CONTROL TCPC_CONTROL %02x", pParams->TCPCControl.AsUInt8);
                        if (pDevContext->GPIO_hTarget != WDF_NO_HANDLE) {
//                             GPIO_PlugOrientation_Set(pDevContext, *(TCPC_PHY_TCPC_CONTROL_t *)(&((UCMTCPCI_PORT_CONTROLLER_SET_CONTROL_IN_PARAMS *)pBuffer)->TCPCControl));
                        }
                        // set control
                        break;
                    case UcmTcpciPortControllerRoleControl: {
                        DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_CONTROL ROLE_CONTROL %02x", pParams->RoleControl.AsUInt8);
                        break;
                    }
                    case UcmTcpciPortControllerPowerControl: {
                        DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_CONTROL POWER_CONTROL %02x", pParams->PowerControl.AsUInt8);
                        break;
                    }
                    case UcmTcpciPortControllerFaultControl:
                        DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_CONTROL FAULT_CONTROL %02x", pParams->FaultControl.AsUInt8);
                        break;
                    default:
                        DBG_IOCTL_PRINT_ERROR("Invalid control register type.");
                        ntStatus = STATUS_INVALID_DEVICE_REQUEST;
                }
            }
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT:
            if (NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_IN_PARAMS), &pBuffer, NULL))) {
                PUCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_IN_PARAMS pParams = (PUCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_IN_PARAMS)pBuffer;
                DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_TRANSMIT %02x", pParams->Transmit.AsUInt8);
            }
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_BUFFER:
            if (NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_BUFFER_IN_PARAMS), &pBuffer, NULL))) {
                DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_TRANSMIT_BUFFER");
                if ((((UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_BUFFER_IN_PARAMS *)pBuffer)->TransmitBuffer.TransmitByteCount + sizeof(((UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_BUFFER_IN_PARAMS *)pBuffer)->TransmitBuffer.TransmitByteCount)) > TCPCI_I2C_DATA_BUFFER_SIZE) {
                    ntStatus = STATUS_INVALID_BUFFER_SIZE;
                } else {
                    // Set Xmit
                }
            }
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_RECEIVE_DETECT:
            if (NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_RECEIVE_DETECT_IN_PARAMS), &pBuffer, NULL))) {
                DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_RECEIVE_DETECT");
                // set Rx
            }
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_CONFIG_STANDARD_OUTPUT:
            if (NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_CONFIG_STANDARD_OUTPUT_IN_PARAMS), &pBuffer, NULL))) {
                PUCMTCPCI_PORT_CONTROLLER_SET_CONFIG_STANDARD_OUTPUT_IN_PARAMS pParams = (PUCMTCPCI_PORT_CONTROLLER_SET_CONFIG_STANDARD_OUTPUT_IN_PARAMS)pBuffer;
                DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_CONFIG_STANDARD_OUTPUT %02x", pParams->ConfigStandardOutput.AsUInt8);
                // SET config
            }
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_COMMAND:
            if (NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_COMMAND_IN_PARAMS), &pBuffer, NULL))) {
                PUCMTCPCI_PORT_CONTROLLER_SET_COMMAND_IN_PARAMS pParams = (PUCMTCPCI_PORT_CONTROLLER_SET_COMMAND_IN_PARAMS)pBuffer;
                DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_COMMAND %02x", pParams->Command);
                // SET command
            }
            break;
        case IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_MESSAGE_HEADER_INFO:
            if (NT_SUCCESS(ntStatus = WdfRequestRetrieveInputBuffer(hRequest, sizeof(UCMTCPCI_PORT_CONTROLLER_SET_MESSAGE_HEADER_INFO_IN_PARAMS), &pBuffer, NULL))) {
                PUCMTCPCI_PORT_CONTROLLER_SET_MESSAGE_HEADER_INFO_IN_PARAMS pParams = (PUCMTCPCI_PORT_CONTROLLER_SET_MESSAGE_HEADER_INFO_IN_PARAMS)pBuffer;
                DBG_IOCTL_PRINT_INFO("IOCTL: PORT_CONTROLLER_SET_MESSAGE_HEADER_INFO %02x", pParams->MessageHeaderInfo.AsUInt8);
                // SET msg header
            }
            break;
        default:
            DBG_IOCTL_PRINT_ERROR("Received unexpected IoControlCode 0x%08X, %s", IoControlCode, Dbg_GetIOCTLName(IoControlCode));
            ntStatus = STATUS_NOT_SUPPORTED;
    }
    if (NT_SUCCESS(ntStatus)) {
        WdfRequestComplete(hRequest, ntStatus);
    } else {
        DBG_IOCTL_CMD_DUMP("--- WDFREQUEST: 0x%p, Ioctl:%s, ntStatus: 0x%08X", hRequest, Dbg_GetIOCTLName(IoControlCode), ntStatus);
        WdfRequestComplete(hRequest, ntStatus);
    }

    KeSetEvent(&pDevContext->IoctlAndIsrSyncEvent, 0, FALSE);                                      /* Wake up passive ISR */

    DBG_IOCTL_METHOD_END_WITH_PARAMS("%s", Dbg_GetIOCTLName(IoControlCode));
}
#endif  /* PTN5150 */
