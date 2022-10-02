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

    // Device Specific Method takes 4 args:
    //  Arg0 : Buffer containing a UUID [16 bytes]
    //  Arg1 : Integer containing the Revision ID
    //  Arg2 : Integer containing the Function Index
    //  Arg3 : Package that contains function-specific arguments
    Method (_DSM, 0x4, Serialized) {
      // UFX interface identifier
      if (LEqual (Arg0, ToUUID ("732b85d5-b7a7-4a1b-9ba0-4bbd00ffd511"))) {

        // Function selector
        switch (ToInteger (Arg2)) {

          // Function 0: Query support
          //   Bit  Description
          //   ---  -------------------------------
          //     0  Query functions property
          //     1  Get property
          //     2  Get port type
          //     3  Set USB device state
          //     4  Set PMU state PME disable
          //     5  notification enabled state
          //     6  core soft reset
          //     8  Get group id
          //     9  Get power down scale
          case (0) {
            switch (ToInteger (Arg1)) {
              // Revision 1: functions {0,9} supported
              case (1) {
                Return (0x201);
              }
              default {
                Return (Buffer () { 0x01 });
              }
            }
          }

          // Function 1: Return device capabilities bitmap
          //   Bit  Description
          //   ---  -------------------------------
          //     0  Attach detach
          //     1  Software charging
          case (1) {
              Return (0x01);
            }

          // Function 2: Get port type
          //     0x00  Unknown port
          //     0x01  Standard downstream
          //     0x02  Charging downstream
          //     0x03  Dedicated charging
          case (2) {
              Return (0x01);
            }

          // Function 3: Set device state
          case (3) {
              Return (Buffer () { 0x0 });
            }

          // Function 9: Get power down scale
          case (9) {
              Return (0x618);
            }

          // Unknown function
            default {
              Return (Buffer () { 0x0 });
            }
        }
      }
      // UFX interface identifier
      if (LEqual(Arg0,ToUUID("FE56CFEB-49D5-4378-A8A2-2978DBE54AD2"))) {
        // Function selector
        switch (ToInteger(Arg2)) {
        // Function 1: Return number of supported USB PHYSICAL endpoints
        // Up to 8 bidirectional endpoints
        case (1) {
            Return (8);
          }
          default {
            Return (Buffer () { 0x0 });
          }
        }
      }
      Return (0x0);
    } // _DSM

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
