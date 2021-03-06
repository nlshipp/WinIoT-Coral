// SPDX-License-Identifier: BSD-2-Clause
/**
 * @copyright 2018 NXP
 *
 * @file    modulus.c
 *
 * @brief   Cryptographic library using the i.MX CAAM driver.\n
 *          Mathematical Modulus operation implementation.
 */
/* Global includes */
#include <string.h>
#include <utee_defines.h>
#include <util.h>

/* Library i.MX includes */
#include <libimxcrypt.h>
#include <libimxcrypt_math.h>

/**
 * @brief   operation (A xor B) mod N
 *
 * @param[in/out] data   input/output data operation
 *
 * @retval TEE_SUCCESS               Operation success
 * @retval TEE_ERROR_BAD_PARAMETERS  Bad parameters
 * @retval TEE_ERROR_GENERIC         Operation failed
 */
TEE_Result libimxcrypt_xor_mod_n(struct imxcrypt_mod_op *data)
{
	TEE_Result ret = TEE_ERROR_GENERIC;

	struct imxcrypt_math *math = NULL;

	/* Check input parameters */
	if (((!data->A.data) || (!data->A.length)) ||
		((!data->B.data) || (!data->B.length)) ||
		((!data->result.data) || (!data->result.length)) ||
		(!data->N.length))
		return TEE_ERROR_BAD_PARAMETERS;

	if (data->result.length < data->N.length)
		return TEE_ERROR_BAD_PARAMETERS;

	math = imxcrypt_getmod(CRYPTO_MATH_HW);
	if (math) {
		/*
		 * Operation done by Math driver
		 */
		ret = math->xor_mod_n(data);
	} else {
		/*
		 * Operation done by Software
		 */
		size_t min, idx;

		/* Calculate the minimum size to do A xor B */
		min = MIN(data->A.length, data->B.length);
		min = MIN(min, data->N.length);

		for (idx = 0; idx < min; idx++)
			data->result.data[idx] = data->A.data[idx] ^
			    data->B.data[idx];

		if (min < data->N.length) {
			/* Complete result to make a N modulus number */
			if (data->A.length > min) {
				memcpy(&data->result.data[idx],
						&data->A.data[idx],
						data->N.length - min);
			} else if (data->B.length > min) {
				memcpy(&data->result.data[idx],
						&data->B.data[idx],
						data->N.length - min);
			} else {
				memset(&data->result.data[idx], 0,
						data->N.length - min);
			}
		}

		ret = TEE_SUCCESS;
	}

	return ret;
}
