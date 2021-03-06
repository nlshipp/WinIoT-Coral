;------------------------------------------------------------------------------
;
; MemoryFence() for AArch64
;
; Copyright (c) 2013, ARM Ltd. All rights reserved.
;
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php.
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
;------------------------------------------------------------------------------

  EXPORT MemoryFence
; MS_CHANGE: change area name to |.text| and add an ALIGN directive
  AREA |.text|, ALIGN=2, CODE, READONLY

;/**
;  Used to serialize load and store operations.
;
;  All loads and stores that proceed calls to this function are guaranteed to be
;  globally visible when this function returns.
;
;**/
;VOID
;EFIAPI
;MemoryFence (
;  VOID
;  );
;
MemoryFence
    // System wide Data Memory Barrier.
    dmb sy
    ret

  END
