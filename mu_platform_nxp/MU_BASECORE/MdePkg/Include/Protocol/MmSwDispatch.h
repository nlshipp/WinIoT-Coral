/** @file
  MM Software Dispatch Protocol introduced from PI 1.5 Specification
  Volume 4 Management Mode Core Interface.

  This protocol provides the parent dispatch service for a given MMI source generator.

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _MM_SW_DISPATCH_H_
#define _MM_SW_DISPATCH_H_

#include <Pi/PiMmCis.h>

#define EFI_MM_SW_DISPATCH_PROTOCOL_GUID \
  { \
    0x18a3c6dc, 0x5eea, 0x48c8, {0xa1, 0xc1, 0xb5, 0x33, 0x89, 0xf9, 0x89, 0x99 } \
  }

///
/// A particular chipset may not support all possible software MMI input values.
/// For example, the ICH supports only values 00h to 0FFh.  The parent only allows a single
/// child registration for each SwMmiInputValue.
///
typedef struct {
  UINTN SwMmiInputValue;
} EFI_MM_SW_REGISTER_CONTEXT;

///
/// The DispatchFunction will be called with Context set to the same value as was passed into
/// this function in RegisterContext and with CommBuffer (and CommBufferSize) pointing
/// to an instance of EFI_MM_SW_CONTEXT indicating the index of the CPU which generated the
/// software MMI.
///
typedef struct {
  ///
  /// The 0-based index of the CPU which generated the software MMI.
  ///
  UINTN SwMmiCpuIndex;
  ///
  /// This value corresponds directly to the CommandPort parameter used in the call to Trigger().
  ///
  UINT8 CommandPort;
  ///
  /// This value corresponds directly to the DataPort parameter used in the call to Trigger().
  ///
  UINT8 DataPort;
} EFI_MM_SW_CONTEXT;

typedef struct _EFI_MM_SW_DISPATCH_PROTOCOL  EFI_MM_SW_DISPATCH_PROTOCOL;

/**
  Register a child MMI source dispatch function for the specified software MMI.

  This service registers a function (DispatchFunction) which will be called when the software
  MMI source specified by RegisterContext->SwMmiCpuIndex is detected. On return,
  DispatchHandle contains a unique handle which may be used later to unregister the function
  using UnRegister().

  @param[in]  This                 Pointer to the EFI_MM_SW_DISPATCH_PROTOCOL instance.
  @param[in]  DispatchFunction     Function to register for handler when the specified software
                                   MMI is generated.
  @param[in, out] RegisterContext  Pointer to the dispatch function's context.
                                   The caller fills this context in before calling
                                   the register function to indicate to the register
                                   function which Software MMI input value the
                                   dispatch function should be invoked for.
  @param[out] DispatchHandle       Handle generated by the dispatcher to track the
                                   function instance.

  @retval EFI_SUCCESS            The dispatch function has been successfully
                                 registered and the MMI source has been enabled.
  @retval EFI_DEVICE_ERROR       The SW driver was unable to enable the MMI source.
  @retval EFI_INVALID_PARAMETER  RegisterContext is invalid. The SW MMI input value
                                 is not within a valid range or is already in use.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory (system or MM) to manage this
                                 child.
  @retval EFI_OUT_OF_RESOURCES   A unique software MMI value could not be assigned
                                 for this dispatch.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_MM_SW_REGISTER)(
  IN  CONST EFI_MM_SW_DISPATCH_PROTOCOL  *This,
  IN        EFI_MM_HANDLER_ENTRY_POINT   DispatchFunction,
  IN  OUT   EFI_MM_SW_REGISTER_CONTEXT   *RegisterContext,
  OUT       EFI_HANDLE                   *DispatchHandle
  );

/**
  Unregister a child MMI source dispatch function for the specified software MMI.

  This service removes the handler associated with DispatchHandle so that it will no longer be
  called in response to a software MMI.

  @param[in] This                Pointer to the EFI_MM_SW_DISPATCH_PROTOCOL instance.
  @param[in] DispatchHandle      Handle of dispatch function to deregister.

  @retval EFI_SUCCESS            The dispatch function has been successfully unregistered.
  @retval EFI_INVALID_PARAMETER  The DispatchHandle was not valid.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_MM_SW_UNREGISTER)(
  IN CONST EFI_MM_SW_DISPATCH_PROTOCOL  *This,
  IN       EFI_HANDLE                   DispatchHandle
);

///
/// Interface structure for the MM Software MMI Dispatch Protocol.
///
/// The EFI_MM_SW_DISPATCH_PROTOCOL provides the ability to install child handlers for the
/// given software.  These handlers will respond to software interrupts, and the maximum software
/// interrupt in the EFI_MM_SW_REGISTER_CONTEXT is denoted by MaximumSwiValue.
///
struct _EFI_MM_SW_DISPATCH_PROTOCOL {
  EFI_MM_SW_REGISTER    Register;
  EFI_MM_SW_UNREGISTER  UnRegister;
  ///
  /// A read-only field that describes the maximum value that can be used in the
  /// EFI_MM_SW_DISPATCH_PROTOCOL.Register() service.
  ///
  UINTN                 MaximumSwiValue;
};

extern EFI_GUID gEfiMmSwDispatchProtocolGuid;

#endif
