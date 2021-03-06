;------------------------------------------------------------------------------
;
; CpuPause() for ARM
;
; Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
; Portions copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php.
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
;------------------------------------------------------------------------------

  EXPORT CpuPause
  AREA cpu_pause, CODE, READONLY

;/**
;  Requests CPU to pause for a short period of time.
;
;  Requests CPU to pause for a short period of time. Typically used in MP
;  systems to prevent memory starvation while waiting for a spin lock.
;
;**/
;VOID
;EFIAPI
;CpuPause (
;  VOID
;  );
;
CpuPause
    NOP
    NOP
    NOP
    NOP
    NOP
    BX LR

  END
