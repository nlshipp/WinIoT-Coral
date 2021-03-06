/** @file
  A hook-in library for:
  - MdeModulePkg/Universal/Variable/Pei/VariablePei.inf
  - MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  - MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmm.inf

  Plugging this library instance into one of the above modules makes that
  variable service backend wait for another platform module to dynamically
  initialize or verify EFI_FIRMWARE_VOLUME_HEADER and VARIABLE_STORE_HEADER in
  the non-volatile variable store FVB device. The initialization / verification
  is signaled by installing gEdkiiNvVarStoreFormattedGuid into the
  phase-matching PPI or protocol database, with a NULL interface. (Note that
  installing gEdkiiNvVarStoreFormattedGuid into either the DXE or the MM
  protocol database will unblock VariableSmm -- refer to EFI_SECTION_MM_DEPEX
  in the PI spec.)

  Copyright (C) 2018, Red Hat, Inc.

  This program and the accompanying materials are licensed and made available
  under the terms and conditions of the BSD License which accompanies this
  distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS, WITHOUT
  WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Base.h>

RETURN_STATUS
EFIAPI
NvVarStoreFormattedInitialize (
  VOID
  )
{
  //
  // Do nothing, just imbue VariablePei / VariableRuntimeDxe / VariableSmm with
  // a PPI or protocol dependency on EDKII_NV_VAR_STORE_FORMATTED_GUID.
  //
  return RETURN_SUCCESS;
}
