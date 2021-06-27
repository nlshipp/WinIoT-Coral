/*
* This file is derived from Microsoft example available on https://github.com/microsoft/Windows-driver-samples/blob/master/usb/UcmTcpciCxClientSample
* Original file licensing:
* Copyright (c) 2015 Microsoft
* SPDX-License-Identifier: MS-PL
* NXP modifications are licensed under the same license
* Copyright 2020 NXP
*
*/

// UCM-TCPCI device. Can be named anything.
Device(USBC)
{
    // This device needs to be enumerated by ACPI, so it needs a HWID.
    // Your INF should match on it.
    Name(_HID, "USBC0001")
    Method(_CRS, 0x0, NotSerialized)
    {
        Name (RBUF, ResourceTemplate ()
        {
            //
            // Sample I2C and GPIO resources.
            // platform's underlying controllers and connections.
            // \_SB.I2C and \_SB.GPIO are paths to predefined I2C and GPIO controller instances.
            //  GPIO3_IO03 NAND_CE2_B
            I2CSerialBus(0x50, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.I2C1")
            GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 67 } // 4 * 32 + 16
            GpioInt(Level, ActiveLow, Exclusive, PullUp, 0, "\\_SB.GPIO",) { 67 }
            // GPIO3_IO15
            GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 79 } // 4 * 32 + 17
        })
        Return(RBUF)
    }
    // Inside the scope of the UCM-TCPCI device, you need to define one "connector" device.
    // It can be named anything.
    Device(CON0)
    {
        // This device is not meant to be enumerated by ACPI, hence you should not assign a
        // HWID to it. Instead, use _ADR to assign address 0 to it.
        Name(_ADR, 0x00000000)

        // _PLD as defined in the ACPI spec. The GroupToken and GroupPosition are used to
        // derive a unique "Connector ID". This PLD should correlate with the PLD associated
        // with the XHCI device for the same port.
        Name(_PLD, Package()
        {
            Buffer()
            {
                0x82,                   // Revision 2, ignore color.
                0x00,0x00,0x00,         // Color (ignored).
                0x00,0x00,0x00,0x00,    // Width and height.
                0x69,                   // User visible; Back panel; VerticalPos:Center.
                0x0c,                   // HorizontalPos:0; Shape:Vertical Rectangle; GroupOrientation:0.
                0x80,0x00,              // Group Token:0; Group Position:1; So Connector ID is 1.
                0x00,0x00,0x00,0x00,    // Not ejectable.
                0xFF,0xFF,0xFF,0xFF     // Vert. and horiz. offsets not supplied.
            }
        })

        // _UPC as defined in the ACPI spec.
        Name(_UPC, Package()
        {
            0x01,                       // Port is connectable.
            0x09,                       // Connector type: Type C connector - USB2 and SS with switch.
            0x00000000,                 // Reserved0 must be zero.
            0x00000000                  // Reserved1 must be zero.
        })

        Name(_DSD, Package()
        {
            // The UUID for Type-C connector capabilities.
            ToUUID("6b856e62-40f4-4688-bd46-5e888a2260de"),

            // The data structure which contains the connector capabilities. Each package
            // element contains two elements: the capability type ID, and the capability data
            // (which depends on the capability type). Note that any information defined here
            // will override similar information described by the driver itself. For example, if
            // the driver claims the port controller is DRP-capable, but ACPI says it is UFP-only
            // ACPI will take precedence.

            Package()
            {
                Package() {1, 4},      // Supported operating modes (DRP).
                Package() {2, 1},      // Supported Type-C sourcing capabilities (DefaultUSB).
                Package() {3, 0},      // Audio accessory capable (False).
                Package() {4, 1},      // Is PD supported (True).
                Package() {5, 3},      // Supported power roles (Sink and Source).
                Package()
                {
                    6,                  // Capability type ID of PD Source Capabilities.
                    Package()
                    {
                        0x0001905A      // Source PDO #0: Fixed:5V, 900mA. No need to describe fixed bits.
                    }
                },
                Package()
                {
                    7,                  // Capability type ID of PD Sink Capabilities.
                    Package ()
                    {
                        0x00019096      // Sink PDO #0: Fixed:5V, 1.5A. No need to describe fixed bits.
                    }
                },
            }
        })
    } // Device(CON0)
} // Device(USBC)