;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php.
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
; Module Name:
;
;   SetMem32.nasm
;
; Abstract:
;
;   SetMem32 function
;
; Notes:
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
;  VOID *
;  InternalMemSetMem32 (
;    IN VOID   *Buffer,
;    IN UINTN  Count,
;    IN UINT32 Value
;    )
;------------------------------------------------------------------------------
global ASM_PFX(InternalMemSetMem32)
ASM_PFX(InternalMemSetMem32):
    DB      0x49, 0xf, 0x6e, 0xc0         ; movd mm0, r8 (Value)
    mov     rax, rcx                    ; rax <- Buffer
    xchg    rcx, rdx                    ; rcx <- Count  rdx <- Buffer
    shr     rcx, 1                      ; rcx <- # of qwords to set
    jz      @SetDwords
    DB      0xf, 0x70, 0xC0, 0x44         ; pshufw mm0, mm0, 44h
.0:
    DB      0xf, 0xe7, 0x2              ; movntq [rdx], mm0
    lea     rdx, [rdx + 8]              ; use "lea" to avoid flag changes
    loop    .0
    mfence
@SetDwords:
    jnc     .1
    DB      0xf, 0x7e, 0x2               ; movd [rdx], mm0
.1:
    ret
