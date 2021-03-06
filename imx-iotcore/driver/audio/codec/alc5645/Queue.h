/* Copyright (c) Microsoft Corporation. All rights reserved.
   Licensed under the MIT License.

Module Name:

    queue.h

Abstract:

    This file contains the queue definitions.

Environment:

    User-mode Driver Framework 2

*/

EXTERN_C_START

//
// This is the context that can be placed per queue
// and would contain per queue information.
//
typedef struct _QUEUE_CONTEXT {

    ULONG PrivateDeviceData;  // just a placeholder

} QUEUE_CONTEXT, *PQUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, QueueGetContext)

NTSTATUS
Alc5645CodecQueueInitialize(
    _In_ WDFDEVICE hDevice
    );

//
// Events from the IoQueue object
//
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL Alc5645CodecEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP Alc5645CodecEvtIoStop;

EXTERN_C_END
