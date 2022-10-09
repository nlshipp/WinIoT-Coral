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
// iMX8M USB port 1 is a USB 3.0 xHCI controller - Dual role USB type C port
//
// iMX8M USB port 2 is a USB 3.0 xHCI controller - Host only USB type A port
//

Device (URS1)
{
  Name (_HID, "NXPI011C") // NXPI: Windows requires 4 character vendor IDs
  Name (_CID, "PNP0CA1")  // Synopsys dual-role device
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

    Method (_STA) {
      return (0xF)
    }

    Device( RHUB) { 
      Name( _ADR, 0x00000000) // Value of 0 is reserved for root HUB

      // Root hub, port 1
      Device( PRT1) {
        // Address object for port 1. This value must be 1 
        Name( _ADR, 0x00000001)

        // USB port capabilities object. This object returns the system 
        // specific USB port configuration information for port number 1
        Name( _UPC, Package(){
          0xFF, // Port is connectable
          0x09, // USB-C with SS
          0x00000000, // Reserved 0 
          0x00000000}) // Reserved 1

        // provide physical port location info
        // Needs to match value in dsdt-tcpc descriptor (specifically group id)
        Name( _PLD, Package(1) {
          Buffer(0x14) { 
          0x82,                 // Revision 2, Ignore color
          0x00,0x00,0x00,       // Color (ignored)
          0x00,0x00,0x00,0x00,  // Width and height
          0x69,                 // User visible, Back panel
          0x0c,                 // Center, shape = vert. rectangle/ Vertical
          0x80,0x00,            // Group Token = 0, Group Pos = 1
          0x03,0x00,0x00,0x00,  // ejectable, requires OPSM eject assistance
          0xFF,0xFF,0xFF,0xFF}}) // Vert. and Horiz. Offsets not supplied
      } // Device( PRT1)
    } // RHUB
  }

  Device (UFN1)
  {
    Name (_ADR, 1)
    Name (_CRS, ResourceTemplate() {
      // interrupt
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {72}
    })

    Device( RHUB) { 
      Name( _ADR, 0x00000000) // Value of 0 is reserved for root HUB

      // Root hub, port 1
      Device( PRT1) {
        // Address object for port 1. This value must be 1 
        Name( _ADR, 0x00000001)

        // USB port capabilities object. This object returns the system 
        // specific USB port configuration information for port number 1
        Name( _UPC, Package(){
          0xFF, // Port is connectable
          0x09, // USB-C with SS
          0x00000000, // Reserved 0 
          0x00000000}) // Reserved 1

        // provide physical port location info
        Name( _PLD, Package(1) {
          Buffer(0x14) { 
          0x82,                 // Revision 2, Ignore color
          0x00,0x00,0x00,       // Color (ignored)
          0x00,0x00,0x00,0x00,  // Width and height
          0x69,                 // User visible, Back panel
          0x0c,                 // Center, shape = vert. rectangle/ Vertical
          0x80,0x00,            // Group Token = 0, Group Pos = 1
          0x03,0x00,0x00,0x00,  // ejectable, requires OPSM eject assistance
          0xFF,0xFF,0xFF,0xFF}}) // Vert. and Horiz. Offsets not supplied
      } // Device( PRT1)
    } // RHUB
  }
}

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
    // Set to host mode
    Store(0x00111004, GCTL)
  }
  Method(_PS3, 0x0, Serialized) {
  }
}
