// SPDX-License-Identifier: BSD-2-Clause
/**
 * @copyright 2018 NXP
 *
 * @file    hash_oid.c
 *
 * @brief   Definition of the Hash's OID
 */

/* Library i.MX includes */
#include <asn1_oid.h>

/**
 * @brief   Hash OID values
 */
const struct imxcrypt_oid imxcrypt_hash_oid[MAX_HASH_SUPPORTED] = {
	/* MD5 */
	OID_DEF(OID_ID_MD5),
	/* SHA1 */
	OID_DEF(OID_ID_SHA1),
	/* SHA224 */
	OID_DEF(OID_ID_SHA224),
	/* SHA256 */
	OID_DEF(OID_ID_SHA256),
	/* SHA386 */
	OID_DEF(OID_ID_SHA384),
	/* SHA512 */
	OID_DEF(OID_ID_SHA512),
};
