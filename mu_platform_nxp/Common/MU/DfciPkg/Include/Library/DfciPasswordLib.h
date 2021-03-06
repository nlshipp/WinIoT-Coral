/** @file
DfciPasswordLib.h

Contains base logic for querying, setting, and verifying user passwords.

Copyright (c) 2018, Microsoft Corporation

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/

#ifndef _DFCI_PASSWORD_LIB_H_
#define _DFCI_PASSWORD_LIB_H_

//=============================================================================
//   DATA STORE STRUCTURES AND DEFINTIONS
//=============================================================================
#pragma pack(1)

// This structure is required to be at the top of all password entries.
typedef struct _DFCI_PASSWORD_STORE_HEADER {
    UINT16          Version;
} DFCI_PASSWORD_STORE_HEADER;

//
// Version 1 Definitions
//
#define PASSWORD_STORE_VER_1_VERSION          1
#define PASSWORD_STORE_VER_1_HASH_DIGEST_SIZE SHA256_DIGEST_SIZE
#define PASSWORD_STORE_VER_1_SALT_SIZE        32
#define PASSWORD_STORE_VER_1_ITERATION_COUNT  60000   // Tuned for ~0.5 sec computation.
#define PASSWORD_STORE_VER_1_KEY_SIZE         40
typedef struct _DFCI_PASSWORD_STORE_VER_1 {
    DFCI_PASSWORD_STORE_HEADER  Header;
    UINT8                     Salt[PASSWORD_STORE_VER_1_SALT_SIZE];
    UINT8                     Key[PASSWORD_STORE_VER_1_KEY_SIZE];
} DFCI_PASSWORD_STORE_VER_1;

//
// Data Store Union - A structure large enough to hold any password store.
//
typedef union {
    DFCI_PASSWORD_STORE_VER_1   Ver1;
} DFCI_PASSWORD_STORE;
#define PASSWORD_STORE_CURRENT_VERSION        PASSWORD_STORE_VER_1_VERSION

#pragma pack()

//
// Standard definitions for password handles.
//
// NOTE: These handles MAY be used to track revisions of hashing algorithms...
typedef UINT32    PW_HANDLE;
enum {
  ADMIN_PW_HANDLE = 0,
  MAX_PW_HANDLE   // All new password handles must be added above this. (eg. UserPw, TokenPw, etc.)
  // NOTE: Not that we *could*, but don't exceed a UINT32.
};

//
// Standard defintions for working with password buffers.
//
#define PW_MIN_LENGTH     6
#define PW_MAX_LENGTH     128

//
// Definitions for working with password tests.
//
typedef UINT32            PW_TEST_BITMAP;
#define PW_TEST_STRING_NULL         (1 << 0)
#define PW_TEST_STRING_TOO_SHORT    (1 << 1)
#define PW_TEST_STRING_TOO_LONG     (1 << 2)
#define PW_TEST_STRING_INVALID_CHAR (1 << 3)

//
// Right now, authentication supports a simple cache.
// TODO: Update this to a more robust "session" system if it gets used heavily.
//
#define PW_AUTH_USE_CACHED_VALUE    NULL


/**
  Performs any initialization that is necessary for the functions in this
  library to behave as expected.

  Only necessary to run once per boot.
  Published as a public function so that it can be invoked in a useful driver.

  @retval     EFI_SUCCESS   Initialization is complete.
  @retval     Others        Something went wrong. Investigate further.

**/
EFI_STATUS
InitializePasswordLib (
  VOID
  );


/**
  Deletes all passwords and resets password infrastructure to factory condition.
  Published as a public function so that it can be invoked in a useful driver.

  @retval     EFI_SUCCESS   Reset is complete.
  @retval     Others        Something went wrong. Investigate further.

**/
EFI_STATUS
ResetPasswordLib (
  VOID
  );


/**
  Public interface for validating password strings.

  Will evaluate all current password strength/validity requirements and
  return a BOOLEAN for whether the password is valid. Also uses an optional
  pointer to return a bitmap of which tests failed.

  NOTE: Returns FALSE on NULL strings.

  @param[in]  String    CHAR16 pointer to the string that's being evaluated.
  @param[out] Failures  [Optional] Pointer to a UINT32 that will have bits (defined
                        in DfciPasswordLib.h) set according to which tests may have failed.
                        If NULL, will not return a test bitmap and will fail ASAP.

  @retval     TRUE      Password is valid. "Failures" should be 0.
  @retval     FALSE     Password is invalid. "Failures" will have bits set for which tests failed.

**/
BOOLEAN
EFIAPI
IsPwStringValid (
  IN  CHAR16          *String,
  OUT PW_TEST_BITMAP  *Failures OPTIONAL
  );


/**
  Public interface for determining whether a given password is set.

  NOTE: Will initialize the Password Store for the given handle if it doesn't exist

  @param[in]  Handle  PW_HANDLE for the password being queried.

  @retval     TRUE    Password is set.
  @retval     FALSE   Password is not set, or an error occurred preventing
                      the check from completing successfully.

**/
BOOLEAN
EFIAPI
IsPasswordSet (
  IN  PW_HANDLE    Handle
  );


/**
  Public interface for setting a new password.

  Will run internal checks on the password before setting it. Returns an
  error if the password cannot be set.

  If CurPassword is NULL and a password is currently set, will pass only if Authentication
  has previously been cached.
  If NewPassword is NULL and a password is currently set, will clear password.

  @param[in]  AuthToken     Authorization Token generated by the Auth Manager
  @param[in]  NewPassword   [Optional] Pointer to a buffer containing the new password.
                            If NULL, will delete the current password, if set.

  @retval     EFI_SUCCESS             Requested operation has been successfully performed.
  @retval     EFI_SECURITY_VIOLATION  There is something wrong with the formatting or
                                      authentication of the CurPassword.
  @retval     EFI_INVALID_PARAMETER   There is something wrong with the formatting of
                                      the NewPassword.
  @retval     EFI_ABORTED             Something else went wrong with the internal logic.

**/
EFI_STATUS
EFIAPI
SetPassword (
  IN  UINTN          AuthToken,
  IN  CHAR16         *NewPassword
  );


/**
  Public interface for validating a password against the current password.

  If no password is currently set, will return FALSE.

  NOTE: This function does NOT perform string validation on the password
        being authenticated. This is to accomodate changing valid charsets.
        Will still make sure that string does not exceed max buffer size.

  @param[in]  Handle    PW_HANDLE for the password being verified.
  @param[in]  Password  String being evaluated.

  @retval     TRUE      Password matches the stored password for Handle.
  @retval     TRUE      No password is currently set.
  @retval     TRUE      Password is NULL and Handle has previously authenticated successfully.
  @retval     FALSE     No Password is set for Handle.
  @retval     FALSE     Supplied Password does not match stored password for Handle.

**/
BOOLEAN
EFIAPI
AuthenticatePassword (
  IN  PW_HANDLE   Handle,
  IN  CONST CHAR16      *Password
  );

/**
Public interface to set password variable.

@param[in]  Buffer    Buffer filled with current password. if NULL, then a NULL password code will be set

@retval     EFI_SUCCESS      If set variable is successful
@retval     EFI_ABORTED      If set variable is not successful

**/
EFI_STATUS
SetPasswordVariable(
UINT8 *Buffer
);
#endif // _DFCI_PASSWORD_LIB_H_
