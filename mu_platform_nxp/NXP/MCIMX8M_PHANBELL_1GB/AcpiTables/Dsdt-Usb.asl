/*
* USB XHCI Controllers
*
*  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*/

//
// iMX8M USB port 1 is a USB 3.0 xHCI controller - OTG USB type C port
//
// iMX8M USB port 2 is a USB 3.0 xHCI controller - Host only USB type A port
//
// TODO add OTG support once PEP driver is available which is a requirement
// of the UFX/URS framework
//

Device (URS1)
{
  Name (_HID, "NXPI011C") // NXPI: Windows requires 4 character vendor IDs
  Name (_CID, "PNP0CA1")  // Synopsis dual-role device
  Name (_UID, 0x0)
  Name (_CCA, 0x0)    // XHCI is not coherent

  Name (_CRS, ResourceTemplate() {
    Memory32Fixed(ReadWrite, 0x38100000, 0x10000)
  })

  Device (USB1)
  {
    Name (_ADR, 0)
    Name (_CRS, ResourceTemplate() {
      // Controller interrupt
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {72}
    })
  }

  Device (UFN1)
  {
    Name (_ADR, 1)
    Name (_CRS, ResourceTemplate() {
      // interrupt
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {72}
    })
  }
}

#if 0 == 1

Device (USB1)
{
  Name (_HID, "NXPI010C") // NXPI: Windows requires 4 character vendor IDs
  Name (_CID, "PNP0D10")
  Name (_UID, 0x0)
  Name (_CCA, 0x0)    // XHCI is not coherent

  OperationRegion (USBH, SystemMemory, 0x38100000, 0x10000)
  Field (USBH, DWordAcc, NoLock, Preserve)
  {
    Offset(0x0000C110),
    GCTL, 32, // USB1_GCTL
  }

  Method(_CRS, 0x0, Serialized) {
    Name(RBUF, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x38100000, 0x10000)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {72}
      })
    Return(RBUF)
  }
  Method(_PS0, 0x0, Serialized) {
    Store(0x30c11004, GCTL)
  }
  Method(_PS3, 0x0, Serialized) {
  }
}
#endif 

Device (USB2)
{
  Name (_HID, "NXPI010C")
  Name (_CID, "PNP0D10")
  Name (_UID, 0x1)
  Name (_CCA, 0x0)    // XHCI is not coherent

  OperationRegion (USBH, SystemMemory, 0x38200000, 0x10000)
  Field (USBH, DWordAcc, NoLock, Preserve)
  {
    Offset(0x0000C110),
    GCTL, 32, // USB1_GCTL
  }

  Method(_CRS, 0x0, Serialized) {
    Name(RBUF, ResourceTemplate() {
      Memory32Fixed(ReadWrite, 0x38200000, 0x10000)
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {73}
      })
    Return(RBUF)
  }
  Method(_PS0, 0x0, Serialized) {
    Store(0x30c11004, GCTL)
  }
  Method(_PS3, 0x0, Serialized) {
  }
}
