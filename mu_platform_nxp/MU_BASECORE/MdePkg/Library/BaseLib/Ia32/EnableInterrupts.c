/** @file
  EnableInterrupts function

  Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/




/**
  Enables CPU interrupts.

**/
VOID
EFIAPI
EnableInterrupts (
  VOID
  )
{
  _asm {
    sti
  }
}

//MS_CHANGE - START
/**
  Enables CPU interrupts and then waits for an interrupt to arrive.

**/
VOID
EFIAPI
EnableInterruptsAndSleep (
  VOID
  )
{
  _asm {
    sti
    hlt
  }
}
//MS_CHANGE - END