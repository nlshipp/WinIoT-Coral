// SPDX-License-Identifier: BSD-3-Clause

/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3a, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014 The Regents of the University of California.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions, and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions, and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. Neither the name of the University nor the names of its contributors may
    be used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS", AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ARE
DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "platform.h"
#include "internals.h"
#include "softfloat.h"

uint_fast64_t
 f128_to_ui64( float128_t a, uint_fast8_t roundingMode, bool exact )
{
    union ui128_f128 uA;
    uint_fast64_t uiA64, uiA0;
    bool sign;
    int_fast32_t exp, shiftCount;
    uint_fast64_t sig64, sig0;
    struct uint128 sig128;
    struct uint64_extra sigExtra;

    uA.f = a;
    uiA64 = uA.ui.v64;
    uiA0  = uA.ui.v0;
    sign = signF128UI64( uiA64 );
    exp  = expF128UI64( uiA64 );
    shiftCount = 0x402F - exp;
    if ( shiftCount <= 0 ) {
        if ( shiftCount < -15 ) {
            softfloat_raiseFlags( softfloat_flag_invalid );
            return UINT64_C( 0xFFFFFFFFFFFFFFFF );
        }
        sig64 = fracF128UI64( uiA64 ) | UINT64_C( 0x0001000000000000 );
        sig0  = uiA0;
        if ( shiftCount ) {
            sig128 = softfloat_shortShiftLeft128( sig64, sig0, -shiftCount );
            sig64 = sig128.v64;
            sig0  = sig128.v0;
        }
    } else {
        sig64 = fracF128UI64( uiA64 );
        sig0  = uiA0;
        if ( exp ) sig64 |= UINT64_C( 0x0001000000000000 );
        sigExtra = softfloat_shiftRightJam64Extra( sig64, sig0, shiftCount );
        sig64 = sigExtra.v;
        sig0  = sigExtra.extra;
    }
    return softfloat_roundPackToUI64( sign, sig64, sig0, roundingMode, exact );

}
