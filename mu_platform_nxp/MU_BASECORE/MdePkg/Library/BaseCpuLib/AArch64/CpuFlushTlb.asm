;------------------------------------------------------------------------------
;
; CpuFlushTlb() for ARM
;
; Copyright (c) 2006 - 2009, Intel Corporation. All rights reserved.<BR>
; Portions copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
; Portions copyright (c) 2011 - 2013, ARM Ltd. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php.
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
;------------------------------------------------------------------------------

  EXPORT CpuFlushTlb
  AREA BaseCpuLib_LowLevel, CODE, READONLY

;/**
;  Flushes all the Translation Lookaside Buffers(TLB) entries in a CPU.
;
;  Flushes all the Translation Lookaside Buffers(TLB) entries in a CPU.
;
;**/
;VOID
;EFIAPI
;CpuFlushTlb (
;  VOID
;  );
;
CpuFlushTlb
  tlbi  vmalle1                 // Invalidate Inst TLB and Data TLB
  dsb   sy
  isb
  ret

  END
