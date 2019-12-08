// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2001-2007, Tom St Denis
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@gmail.com, http://libtom.org
 */
#include "tomcrypt.h"

/**
   @file dsa_verify_key.c
   DSA implementation, verify a key, Tom St Denis
*/

#ifdef LTC_MDSA

/**
   Verify a DSA key for validity
   @param key   The key to verify
   @param stat  [out]  Result of test, 1==valid, 0==invalid
   @return CRYPT_OK if successful
*/
int dsa_verify_key(dsa_key *key, int *stat)
{
   void   *tmp, *tmp2;
   int    res, err;

   LTC_ARGCHK(key  != NULL);
   LTC_ARGCHK(stat != NULL);

   /* default to an invalid key */
   *stat = 0;

   /* first make sure key->q and key->p are prime */
   if ((err = mp_prime_is_prime(key->q, 8, &res)) != CRYPT_OK) {
      return err;
   }
   if (res == 0) {
      return CRYPT_OK;
   }

   if ((err = mp_prime_is_prime(key->p, 8, &res)) != CRYPT_OK) {
      return err;
   }
   if (res == 0) {
      return CRYPT_OK;
   }

   /* now make sure that g is not -1, 0 or 1 and <p */
   if (mp_cmp_d(key->g, 0) == LTC_MP_EQ || mp_cmp_d(key->g, 1) == LTC_MP_EQ) {
      return CRYPT_OK;
   }
   if ((err = mp_init_multi(&tmp, &tmp2, NULL)) != CRYPT_OK)               { return err; }
   if ((err = mp_sub_d(key->p, 1, tmp)) != CRYPT_OK)                       { goto error; }
   if (mp_cmp(tmp, key->g) == LTC_MP_EQ || mp_cmp(key->g, key->p) != LTC_MP_LT) {
      err = CRYPT_OK;
      goto error;
   }

   /* 1 < y < p-1 */
   if (!(mp_cmp_d(key->y, 1) == LTC_MP_GT && mp_cmp(key->y, tmp) == LTC_MP_LT)) {
      err = CRYPT_OK;
      goto error;
   }

   /* now we have to make sure that g^q = 1, and that p-1/q gives 0 remainder */
   if ((err = mp_div(tmp, key->q, tmp, tmp2)) != CRYPT_OK)             { goto error; }
   if (mp_iszero(tmp2) != LTC_MP_YES) {
      err = CRYPT_OK;
      goto error;
   }

   if ((err = mp_exptmod(key->g, key->q, key->p, tmp)) != CRYPT_OK)    { goto error; }
   if (mp_cmp_d(tmp, 1) != LTC_MP_EQ) {
      err = CRYPT_OK;
      goto error;
   }

   /* now we have to make sure that y^q = 1, this makes sure y \in g^x mod p */
   if ((err = mp_exptmod(key->y, key->q, key->p, tmp)) != CRYPT_OK)       { goto error; }
   if (mp_cmp_d(tmp, 1) != LTC_MP_EQ) {
      err = CRYPT_OK;
      goto error;
   }

   /* at this point we are out of tests ;-( */
   err   = CRYPT_OK;
   *stat = 1;
error:
   mp_clear_multi(tmp, tmp2, NULL);
   return err;
}
#endif

/* $Source: /cvs/libtom/libtomcrypt/src/pk/dsa/dsa_verify_key.c,v $ */
/* $Revision: 1.8 $ */
/* $Date: 2007/05/12 14:32:35 $ */
