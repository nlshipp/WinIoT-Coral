/* cryptoki.h include file for PKCS #11. */
/* $Revision: 1.4 $ */

/* License to copy and use this software is granted provided that it is
 * identified as "RSA Security Inc. PKCS #11 Cryptographic Token Interface
 * (Cryptoki)" in all material mentioning or referencing this software.

 * License is also granted to make and use derivative works provided that
 * such works are identified as "derived from the RSA Security Inc. PKCS #11
 * Cryptographic Token Interface (Cryptoki)" in all material mentioning or
 * referencing the derived work.

 * RSA Security Inc. makes no representations concerning either the
 * merchantability of this software or the suitability of this software for
 * any particular purpose. It is provided "as is" without express or implied
 * warranty of any kind.
 */

/* This is a sample file containing the top level include directives
 * for building Win32 Cryptoki libraries and applications.
 */

#ifndef ___CRYPTOKI_H_INC___
#define ___CRYPTOKI_H_INC___

#ifdef WIN32
#pragma pack(push, cryptoki, 1)

/* Specifies that the function is a DLL entry point. */
//#define CK_IMPORT_SPEC __declspec(dllimport)  /* RVE: delete this to compile a static library */
#define CK_IMPORT_SPEC
#else
#define CK_IMPORT_SPEC
#endif

/* Define CRYPTOKI_EXPORTS during the build of cryptoki libraries. Do
 * not define it in applications.
 */
#ifdef CRYPTOKI_EXPORTS
  #ifdef WIN32
    /* Specified that the function is an exported DLL entry point. */
    #define CK_EXPORT_SPEC __declspec(dllexport)
  #else
    #define CK_EXPORT_SPEC CK_IMPORT_SPEC
  #endif
#else
#define CK_EXPORT_SPEC CK_IMPORT_SPEC
#endif

/* Ensures the calling convention for Win32 builds */
#ifdef WIN32
#define CK_CALL_SPEC __cdecl
#else
#define CK_CALL_SPEC
#endif

#define CK_PTR *

#define CK_DEFINE_FUNCTION(returnType, name) \
  returnType CK_EXPORT_SPEC CK_CALL_SPEC name

#define CK_DECLARE_FUNCTION(returnType, name) \
  returnType CK_EXPORT_SPEC CK_CALL_SPEC name

#define CK_DECLARE_FUNCTION_POINTER(returnType, name) \
  returnType CK_IMPORT_SPEC (CK_CALL_SPEC CK_PTR name)

#define CK_CALLBACK_FUNCTION(returnType, name) \
  returnType (CK_CALL_SPEC CK_PTR name)

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#include "pkcs11.h"

#ifdef WIN32
#pragma pack(pop, cryptoki)
#endif

#endif /* ___CRYPTOKI_H_INC___ */
