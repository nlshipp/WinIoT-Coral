/*------------------------------------------------------------------------------
--       Copyright (c) 2015-2017, VeriSilicon Inc. All rights reserved        --
--         Copyright (c) 2011-2014, Google Inc. All rights reserved.          --
--         Copyright (c) 2007-2010, Hantro OY. All rights reserved.           --
--                                                                            --
-- This software is confidential and proprietary and may be used only as      --
--   expressly authorized by VeriSilicon in a written licensing agreement.    --
--                                                                            --
--         This entire notice must be reproduced on all copies                --
--                       and may not be removed.                              --
--                                                                            --
--------------------------------------------------------------------------------
-- Redistribution and use in source and binary forms, with or without         --
-- modification, are permitted provided that the following conditions are met:--
--   * Redistributions of source code must retain the above copyright notice, --
--       this list of conditions and the following disclaimer.                --
--   * Redistributions in binary form must reproduce the above copyright      --
--       notice, this list of conditions and the following disclaimer in the  --
--       documentation and/or other materials provided with the distribution. --
--   * Neither the names of Google nor the names of its contributors may be   --
--       used to endorse or promote products derived from this software       --
--       without specific prior written permission.                           --
--------------------------------------------------------------------------------
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"--
-- AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE  --
-- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE --
-- ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE  --
-- LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR        --
-- CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF       --
-- SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS   --
-- INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN    --
-- CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)    --
-- ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE --
-- POSSIBILITY OF SUCH DAMAGE.                                                --
--------------------------------------------------------------------------------
--                                                                            --
--  Description : Camera stabilization standalone configuration
--
------------------------------------------------------------------------------*/
#ifndef __VIDSTBCFG_H__
#define __VIDSTBCFG_H__

#include "basetype.h"

/* Here is defined the default values for the encoder build-time configuration.
 * You can override these settings by defining the values as compiler flags
 * in the Makefile.
 */

/* The input image's 32-bit swap: 0 or 1
 * This defines the 32-bit endianess of the ASIC input YUV
 * 1 = 64-bit endianess */
#ifndef VSH1_INPUT_SWAP_32_YUV
#define VSH1_INPUT_SWAP_32_YUV                   0
#endif

/* The input image's 16-bit swap: 0 or 1
 * This defines the 16-bit endianess of the ASIC input YUV
 */
#ifndef VSH1_INPUT_SWAP_16_YUV
#define VSH1_INPUT_SWAP_16_YUV                   1
#endif

/* The input image's 8-bit swap: 0 or 1
 * This defines the byte endianess of the ASIC input YUV
 */
#ifndef VSH1_INPUT_SWAP_8_YUV
#define VSH1_INPUT_SWAP_8_YUV                    1
#endif

/* The input image's 32-bit swap: 0 or 1
 * This defines the 32-bit endianess of the ASIC input RGB16
 * 1 = 64-bit endianess */
#ifndef VSH1_INPUT_SWAP_32_RGB16
#define VSH1_INPUT_SWAP_32_RGB16                 0
#endif

/* The input image's 16-bit swap: 0 or 1
 * This defines the 16-bit endianess of the ASIC input RGB16
 */
#ifndef VSH1_INPUT_SWAP_16_RGB16
#define VSH1_INPUT_SWAP_16_RGB16                 1
#endif

/* The input image's byte swap: 0 or 1
 * This defines the byte endianess of the ASIC input RGB16
 */
#ifndef VSH1_INPUT_SWAP_8_RGB16
#define VSH1_INPUT_SWAP_8_RGB16                  0
#endif

/* The input image's 32-bit swap: 0 or 1
 * This defines the 32-bit endianess of the ASIC input RGB32
 * 1 = 64-bit endianess */
#ifndef VSH1_INPUT_SWAP_32_RGB32
#define VSH1_INPUT_SWAP_32_RGB32                 0
#endif

/* The input image's 16-bit swap: 0 or 1
 * This defines the 16-bit endianess of the ASIC input RGB32
 */
#ifndef VSH1_INPUT_SWAP_16_RGB32
#define VSH1_INPUT_SWAP_16_RGB32                 0
#endif

/* The input image's byte swap: 0 or 1
 * This defines the byte endianess of the ASIC input RGB32
 */
#ifndef VSH1_INPUT_SWAP_8_RGB32
#define VSH1_INPUT_SWAP_8_RGB32                  0
#endif

/* ASIC interrupt enable.
 * This enables/disables the ASIC to generate interrupts
 * If this is '1', the EWL must poll the registers to find out
 * when the HW is ready.
 */
#ifndef VSH1_IRQ_DISABLE
#define VSH1_IRQ_DISABLE                         0
#endif

/* ASIC bus interface configuration values                  */
/* DO NOT CHANGE IF NOT FAMILIAR WITH THE CONCEPTS INVOLVED */

/* Burst length. This sets the maximum length of a single ASIC burst in addresses.
 * Allowed values are:
 *          AHB {0, 4, 8, 16} ( 0 means incremental burst type INCR)
 *          OCP [1,63]
 *          AXI [1,16]
 */
#ifndef VSH1_BURST_LENGTH
#define VSH1_BURST_LENGTH                               16
#endif

/* INCR type burst mode                          */
/* 0 allowe INCR type bursts                     */
/* 1 disable INCR type and use SINGLE instead    */
#ifndef VSH1_BURST_INCR_TYPE_ENABLED
#define VSH1_BURST_INCR_TYPE_ENABLED                    0
#endif

/* Data discard mode. When enabled read bursts of length 2 or 3 are converted to */
/* BURST4 and  useless data is discarded. Otherwise use INCR type for that kind  */
/* of read bursts */
/* 0 disable data discard                   */
/* 1 enable data discard                    */
#ifndef VSH1_BURST_DATA_DISCARD_ENABLED
#define VSH1_BURST_DATA_DISCARD_ENABLED                 0
#endif

/* AXI bus read and write ID values used by HW. 0 - 255 */
#ifndef VSH1_AXI_READ_ID
#define VSH1_AXI_READ_ID                                0
#endif

#ifndef VSH1_AXI_WRITE_ID
#define VSH1_AXI_WRITE_ID                               0
#endif

/* End of "ASIC bus interface configuration values" */

/* ASIC internal clock gating control. 0 - disabled, 1 - enabled */
#ifndef VSH1_ASIC_CLOCK_GATING_ENABLED
#define VSH1_ASIC_CLOCK_GATING_ENABLED                  0
#endif

#endif
