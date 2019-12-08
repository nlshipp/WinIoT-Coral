/** @file
*
*  RPMB IO protocol is an interface implementation for the Replay Protected Memory
*  Block (RPMB) as defined by JEDEC Standard for MultiMediaCard specs 4.41.
*
*  This protocol abstracts the RPMB operations to allow EFI boot services environment
*  to perform eMMC RPMB operations without specific knowledge about the card type or
*  the host controller.
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#ifndef __RPMB_IO_H__
#define __RPMB_IO_H__

// Global ID for the RPMB IO Protocol {FBAEE5B2-08B0-41B8-B0B0-86B72EED1BB6}
#define EFI_RPMB_IO_PROTOCOL_GUID \
  { 0xfbaee5b2, 0x8b0, 0x41b8, { 0xb0, 0xb0, 0x86, 0xb7, 0x2e, 0xed, 0x1b, 0xb6 } };

#define EFI_RPMB_IO_PROTOCOL_REVISION  0x00010000

// RPMB Request Message Types

#define EFI_RPMB_REQUEST_PROGRAM_KEY     0x0001  // Authentication key programming request
#define EFI_RPMB_REQUEST_COUNTER_VALUE   0x0002  // Reading of the Write Counter value -request
#define EFI_RPMB_REQUEST_AUTH_WRITE      0x0003  // Authenticated data write request
#define EFI_RPMB_REQUEST_AUTH_READ       0x0004  // Authenticated data read request
#define EFI_RPMB_REQUEST_RESULT_REQUEST  0x0005  // Result read request

// RPMB Response Message Types

#define EFI_RPMB_RESPONSE_PROGRAM_KEY    0x0100  // Authentication key programming response
#define EFI_RPMB_RESPONSE_COUNTER_VALUE  0x0200  // Reading of the Write Counter value -response
#define EFI_RPMB_RESPONSE_AUTH_WRITE     0x0300  // Authenticated data write response
#define EFI_RPMB_RESPONSE_AUTH_READ      0x0400  // Authenticated data read response

// RPMB Operation Results

#define EFI_RPMB_OK                     0   // Operation OK
#define EFI_RPMB_ERROR_GENERAL          1   // General failure
#define EFI_RPMB_ERROR_AUTH             2   // Authentication failure (MAC comparison
                                            //   not matching, MAC calculation failure)
#define EFI_RPMB_ERROR_COUNTER          3   // Counter failure (counters not matching
                                            //   in comparison, counter incrementing failure)
#define EFI_RPMB_ERROR_ADDRESS          4   // Address failure (address out of range,
                                            //   wrong address alignment)
#define EFI_RPMB_ERROR_WRITE            5   // Write failure (data/counter/result write failure)
#define EFI_RPMB_ERROR_READ             6   // Read failure (data/counter/result read failure)
#define EFI_RPMB_ERROR_KEY              7   // Authentication Key not yet programmed

#define EFI_RPMB_ERROR_MASK             0x7
#define EFI_RPMB_ERROR_CNT_EXPIRED_BIT  0x80

// RPMB Data Frame fields size in bytes.

#define EFI_RPMB_PACKET_STUFF_SIZE       196
#define EFI_RPMB_PACKET_KEY_MAC_SIZE     32
#define EFI_RPMB_PACKET_DATA_SIZE        256
#define EFI_RPMB_PACKET_NONCE_SIZE       16
#define EFI_RPMB_PACKET_WCOUNTER_SIZE    4
#define EFI_RPMB_PACKET_ADDRESS_SIZE     2
#define EFI_RPMB_PACKET_BLOCKCOUNT_SIZE  2
#define EFI_RPMB_PACKET_RESULT_SIZE      2
#define EFI_RPMB_PACKET_TYPE_SIZE        2

// Everything in the RPMB Data Frame is hashed except the Stuff and the MAC itself.
#define EFI_RPMB_PACKET_DATA_HASH_SIZE ( \
  sizeof(EFI_RPMB_DATA_PACKET) - \
  offsetof(EFI_RPMB_DATA_PACKET, PacketData) \
  )

// CID register is 16 byte
#define EFI_RPMB_CID_SIZE 16

/** The data frame to access the replay protected memory area.

  NOTE: Byte order of the RPMB data frame is MSB first, e.g. Write Counter MSB
  [11] is storing the upmost byte of the counter value.
**/
#pragma pack(1)
typedef struct {
  UINT8  Stuff[EFI_RPMB_PACKET_STUFF_SIZE];
  UINT8  KeyOrMAC[EFI_RPMB_PACKET_KEY_MAC_SIZE];  // The authentication key or
                                                  // the message authentication code (MAC)
                                                  // depending on the request/response
                                                  // type. The MAC will be delivered
                                                  // in the last (or the only) block of data
  UINT8  PacketData[EFI_RPMB_PACKET_DATA_SIZE];   // Data to be written or read by signed access.
  UINT8  Nonce[EFI_RPMB_PACKET_NONCE_SIZE];       // Random number generated by the host
                                                  // for the Requests and copied to the
                                                  // Response by the eMMC Replay Protected
                                                  // Memory Block engine
  UINT8  WriteCounter[EFI_RPMB_PACKET_WCOUNTER_SIZE];   // Counter value for the total amount
                                                        // of the successful authenticated data
                                                        // write requests made by the host
  UINT8  Address[EFI_RPMB_PACKET_ADDRESS_SIZE];   // Address of the data to be programmed to
                                                  // or read from the Replay Protected
                                                  // Memory Block. Address is the serial number
                                                  // of the accessed half sector (256B).
                                                  // Address argument in CMD 18 and CMD 25
                                                  // will be ignored
  UINT8  BlockCount[EFI_RPMB_PACKET_BLOCKCOUNT_SIZE];   // Number of blocks (half sectors, 256B)
                                                        // requested to be read/programmed. This
                                                        // value is equal to the count value in
                                                        // CMD23 argument
  UINT8  OperationResult[EFI_RPMB_PACKET_RESULT_SIZE];  // Includes information about the status
                                                        // of the write counter (valid, expired)
                                                        // and successfulness of the access made
                                                        // to the Replay Protected Memory Block
  UINT8  RequestOrResponseType[EFI_RPMB_PACKET_TYPE_SIZE];  // Defines the type of request and
                                                            // response to/from the memory.
} EFI_RPMB_DATA_PACKET;
#pragma pack()

typedef struct {
  EFI_RPMB_DATA_PACKET  *Packets;
  UINTN                 PacketCount;  // The number of RPMB Frames/Packets which is
                                      // also the number of half sectors (256Byte) to
                                      // be read/programmed since each RPBM frame
                                      // holds 256Bytes of data.
} EFI_RPMB_DATA_BUFFER;

typedef struct _EFI_RPMB_IO_PROTOCOL EFI_RPMB_IO_PROTOCOL;

/** Authentication key programming request.

  @param[in] This Indicates a pointer to the calling context.
  @param[in] ProgrammingRequest A data packet describing a key programming request.
  @param[out] ResultResponse A caller allocated data packet which will receive the
  key programming result.

  @retval EFI_SUCCESS RPMB communication sequence with the eMMC succeeded
  according to specs, other values are returned in case of any protocol error.
  Failure during key programming any other eMMC internal failure is reported
  in the Result field of the returned response data packet.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_RPMB_PROGRAM_KEY) (
  IN EFI_RPMB_IO_PROTOCOL   *This,
  IN EFI_RPMB_DATA_PACKET   *ProgrammingRequest,
  OUT EFI_RPMB_DATA_PACKET  *ResultResponse
  );

/** Reading of the write counter value request.

  @param[in] This Indicates a pointer to the calling context.
  @param[in] ReadRequest A data packet describing a read counter value request.
  @param[out] ReadResponse A caller allocated data packet which will receive
  the counter value read response. If counter has expired bit 7 is set to 1 in
  returned Result.

  @retval EFI_SUCCESS RPMB communication sequence with the eMMC succeeded
  according to specs, other values are returned in case of any protocol error.
  Failure during counter read or any other eMMC internal failure is reported
  in the Result field of the returned response data packet.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_RPMB_READ_COUNTER) (
  IN EFI_RPMB_IO_PROTOCOL   *This,
  IN EFI_RPMB_DATA_PACKET   *ReadRequest,
  OUT EFI_RPMB_DATA_PACKET  *ReadResponse
  );

/** Authenticated data write request.

  @param[in] This Indicates a pointer to the calling context.
  @param[in] WriteRequest A sequence of data packets describing data write
  requests and holds the data to be written.
  @param[out] ResultResponse A caller allocated data packet which will receive
  the data write programming result.

  @retval EFI_SUCCESS RPMB communication sequence with the eMMC succeeded
  according to specs, other values are returned in case of any protocol error.
  Failure during data programming or any other eMMC internal failure is reported
  in the Result field of the returned data packet.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_RPMB_AUTHENTICATED_WRITE) (
  IN EFI_RPMB_IO_PROTOCOL   *This,
  IN EFI_RPMB_DATA_BUFFER   *WriteRequest,
  OUT EFI_RPMB_DATA_PACKET  *ResultResponse
  );

/** Authenticated data read request.

  @param[in] This Indicates a pointer to the calling context.
  @param[in] ReadRequest A data packet that describes a data read request.
  @param[out] ReadResponse A caller allocated data packets which will receive
  the data read.

  @retval EFI_SUCCESS RPMB communication sequence with the eMMC succeeded
  according to specs, other values are returned in case of any protocol error.
  Failure during data fetch from the eMMC or any other eMMC internal failure
  is reported in the Result field of the returned data packet.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_RPMB_AUTHENTICATED_READ) (
  IN EFI_RPMB_IO_PROTOCOL   *This,
  IN EFI_RPMB_DATA_PACKET   *ReadRequest,
  OUT EFI_RPMB_DATA_BUFFER  *ReadResponse
  );

struct _EFI_RPMB_IO_PROTOCOL {
  UINT64  Revision;
  UINT8   Cid[EFI_RPMB_CID_SIZE];   // MMC Card IDentification (CID) register.
  UINT8   ReliableSectorCount;      // Reliable write sector count as defined by
                                    // the REL_WR_SEC_C field of the
                                    // MMC EXT_CSD register.
  UINT8   RpmbSizeMult;             // RPMB sector size multiplier as defined by
                                    // the RPMB_SIZE_MULT field of the
                                    // MMC EXT_CSD register.
                                    // The RPMB partition size is calculated from
                                    // the register by using the
                                    // following equation:
                                    // RPMB partition size = 128kB x RPMB_SIZE_MULT

  // Protocol Callbacks
  EFI_RPMB_PROGRAM_KEY          ProgramKey;
  EFI_RPMB_READ_COUNTER         ReadCounter;
  EFI_RPMB_AUTHENTICATED_WRITE  AuthenticatedWrite;
  EFI_RPMB_AUTHENTICATED_READ   AuthenticatedRead;
};

__inline__
static
CONST CHAR16*
RpmbOperationResultToString (
  UINT16 OperationResult
  )
{
  switch (OperationResult) {
    case EFI_RPMB_OK:
      return L"Operation OK";
    case EFI_RPMB_ERROR_GENERAL:
      return L"General failure";
    case EFI_RPMB_ERROR_AUTH:
      return L"Authentication failure";
    case EFI_RPMB_ERROR_COUNTER:
      return L"Counter failure";
    case EFI_RPMB_ERROR_ADDRESS:
      return L"Address failure";
    case EFI_RPMB_ERROR_WRITE:
      return L"Write failure";
    case EFI_RPMB_ERROR_READ:
      return L"Read failure";
    case EFI_RPMB_ERROR_KEY:
      return L"Authentication key not yet programmed";
    default:
      return L"Undefined operation result";
  }
}

extern EFI_GUID gEfiRpmbIoProtocolGuid;

#endif // __RPMB_IO_H__
