//-----------------------------------------------------------------------------
//
// Copyright 2017, NXP
// All rights reserved.
//
// Redistribution. Reproduction and redistribution in binary form, without
// modification, for use solely in conjunction with a NXP
// chipset, is permitted provided that the following conditions are met:
//
//   . Redistributions must reproduce the above copyright notice and the following
//     disclaimer in the documentation and/or other materials provided with the
//     distribution.
//
//   . Neither the name of NXP nor the names of its suppliers
//     may be used to endorse or promote products derived from this Software
//     without specific prior written permission.
//
//   . No reverse engineering, decompilation, or disassembly of this Software is
//     permitted.
//
// Limited patent license. NXP (.Licensor.) grants you
// (.Licensee.) a limited, worldwide, royalty-free, non-exclusive license under
// the Patents to make, have made, use, import, offer to sell and sell the
// Software. No hardware per se is licensed hereunder.
// The term .Patents. as used in this agreement means only those patents or patent
// applications owned solely and exclusively by Licensor as of the date of
// Licensor.s submission of the Software and any patents deriving priority (i.e.,
// having a first effective filing date) therefrom. The term .Software. as used in
// this agreement means the firmware image submitted by Licensor, under the terms
// of this license, to git://git.kernel.org/pub/scm/linux/kernel/git/firmware/
// linux-firmware.git.
// Notwithstanding anything to the contrary herein, Licensor does not grant and
// Licensee does not receive, by virtue of this agreement or the Licensor's
// submission of any Software, any license or other rights under any patent or
// patent application owned by any affiliate of Licensor or any other entity
// (other than Licensor), whether expressly, impliedly, by virtue of estoppel or
// exhaustion, or otherwise.
//
// DISCLAIMER. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//-----------------------------------------------------------------------------
//
// file: sdma_imx7d_script_code.h
// SDMA release 4.2
// This file contains functions of SDMA scripts code initialization
//
//-----------------------------------------------------------------------------

#ifndef SDMA_IMX7D_SCRIPT_CODE_H
#define SDMA_IMX7D_SCRIPT_CODE_H

/*!
* SDMA scripts start addresses
* Addresses less than 6144 are ROM based
* Addresses 6144 or larger are RAM based
*/

// Script# 0
#define imx7_ap_2_ap_ADDR 644
// Script# 11
#define imx7_uart_2_mcu_ADDR 6956
// Script# 13
#define imx7_mcu_2_app_ADDR 749
// Script# 14
#define imx7_per_2_per_ADDR 6473
// Script# 16
#define imx7_uartsh_2_mcu_ADDR 7048
// Script# 18
#define imx7_mcu_2_shp_ADDR 962
// Script# 22
#define imx7_app_2_mcu_ADDR 685
// Script# 24
#define imx7_shp_2_mcu_ADDR 893
// Script# 27
#define imx7_spdif_2_mcu_ADDR 1102
// Script# 28
#define imx7_mcu_2_spdif_ADDR 1136
// Script# 34
#define imx7_ram_code_start_ADDR 6144
// Script# 35
#define imx7_mcu_2_ssish_ADDR 6384
// Script# 36
#define imx7_ssish_2_mcu_ADDR 6872
// Script# 37
#define imx7_hdmi_dma_ADDR 7223
// Script# 38
#define imx7_zcanfd_2_mcu_ADDR 7122
// Script# 39
#define imx7_zqspi_2_mcu_ADDR 7278
// Script# 40
#define imx7_mcu_2_ecspi_ADDR 6144
// Script# 42
#define imx7_mcu_2_sai_ADDR 6235
// Script# 43
#define imx7_sai_2_mcu_ADDR 6729

/*!
* SDMA RAM image start address and size
*/

#define IMX7_RAM_CODE_START_ADDR     6144
#define IMX7_RAM_CODE_SIZE           1236

/*!
* Buffer that holds the SDMA RAM image
* Contents extracted from http://git.freescale.com/git/cgit.cgi/imx/linux-imx.git/tree/firmware/imx/sdma?h=imx_4.9.11_1.0.0_ga
*/

static const short imx7_sdma_code[] =
{
0xc1e5, 0x57db, 0x52f3, 0x6a01, 0x52fb, 0x6ad3, 0x52fb, 0x1a1c,
0x6ac3, 0x62e8, 0x0211, 0x3aff, 0x0830, 0x02d0, 0x7c3f, 0x008f,
0x003f, 0x00d5, 0x7d01, 0x008d, 0x05a0, 0x5deb, 0x0478, 0x7d03,
0x0479, 0x7d1c, 0x7c20, 0x0479, 0x7c15, 0x56ee, 0x0660, 0x7d05,
0x6509, 0x7e33, 0x620a, 0x7e31, 0x9829, 0x620a, 0x7e2e, 0x6509,
0x7e2c, 0x0512, 0x0512, 0x02ad, 0x6ac8, 0x7f27, 0x2003, 0x4800,
0x7ced, 0x9841, 0x7802, 0x6209, 0x6ac8, 0x9840, 0x0015, 0x7802,
0x620a, 0x6ac8, 0x9840, 0x0015, 0x0015, 0x7802, 0x620b, 0x6ac8,
0x7c14, 0x6ddf, 0x7f12, 0x077f, 0x7d09, 0x52fb, 0x1a04, 0x6ac3,
0x6ad3, 0x62c8, 0x2a04, 0x6ac8, 0x52fb, 0x6ad3, 0x0000, 0x55eb,
0x4d00, 0x7d07, 0xc1fc, 0x57db, 0x9806, 0x0007, 0x68cc, 0x680c,
0xc215, 0xc20c, 0x9801, 0xc1e5, 0x57db, 0x52f3, 0x6a01, 0x52fb,
0x6ad3, 0x008f, 0x38ff, 0x5ce3, 0x00d5, 0x7d01, 0x008d, 0x05a0,
0x5deb, 0x0478, 0x7d03, 0x0479, 0x7d30, 0x7c48, 0x0479, 0x7c15,
0x56ee, 0x0660, 0x7d05, 0x6509, 0x7e75, 0x620a, 0x7e73, 0x987c,
0x620a, 0x7e70, 0x6509, 0x7e6e, 0x0512, 0x0512, 0x02ad, 0x6ac8,
0x7f69, 0x2003, 0x4800, 0x7ced, 0x98d0, 0x52fb, 0x6ad3, 0x048a,
0x57db, 0x068f, 0x0711, 0x3f0f, 0x0610, 0x0611, 0x3e0f, 0x1e01,
0x0617, 0x0617, 0x6209, 0x6ac8, 0x049e, 0x6cd3, 0x2701, 0x2001,
0x4f00, 0x7cf8, 0x4800, 0x7ce9, 0x98cf, 0x0015, 0x52fb, 0x6ad3,
0x048a, 0x57db, 0x068f, 0x0711, 0x3f0f, 0x0610, 0x0611, 0x3e0f,
0x1e01, 0x0617, 0x0617, 0x620a, 0x6ac8, 0x049e, 0x6cd3, 0x2701,
0x2001, 0x4f00, 0x7cf8, 0x4800, 0x7ce9, 0x98cf, 0x0015, 0x0015,
0x52fb, 0x6ad3, 0x048a, 0x57db, 0x068f, 0x0711, 0x3f0f, 0x0610,
0x0611, 0x3e0f, 0x1e01, 0x0617, 0x0617, 0x620b, 0x6ac8, 0x049e,
0x6cd3, 0x2701, 0x2001, 0x4f00, 0x7cf8, 0x4800, 0x7ce9, 0x7c1a,
0x6ddf, 0x7f18, 0x57db, 0x0777, 0x7c0c, 0x0710, 0x3f0f, 0x0717,
0x0717, 0x0e70, 0x0611, 0x0c70, 0x0411, 0x069c, 0x069f, 0x0c01,
0x5c06, 0x0000, 0x54e3, 0x57db, 0x55eb, 0x4d00, 0x7d07, 0xc1fc,
0x57db, 0x9861, 0x0007, 0x68cc, 0x680c, 0xc215, 0xc20c, 0x985c,
0xc1d9, 0xc1e5, 0x57db, 0x5fe3, 0x57e3, 0x52f3, 0x6a21, 0x008f,
0x00d5, 0x7d01, 0x008d, 0x05a0, 0x5deb, 0x56fb, 0x0478, 0x7d03,
0x0479, 0x7d2a, 0x7c31, 0x0479, 0x7c20, 0x0b70, 0x0311, 0x53eb,
0x0f00, 0x0360, 0x7d05, 0x6509, 0x7e37, 0x620a, 0x7e35, 0x9914,
0x620a, 0x7e32, 0x6509, 0x7e30, 0x0512, 0x0512, 0x02ad, 0x0760,
0x7c02, 0x5a06, 0x991c, 0x5a26, 0x7f27, 0x1f01, 0x2003, 0x4800,
0x7ce8, 0x0b70, 0x0311, 0x5313, 0x993d, 0x0015, 0x7804, 0x6209,
0x5a06, 0x6209, 0x5a26, 0x993c, 0x0015, 0x0015, 0x7804, 0x620a,
0x5a06, 0x620a, 0x5a26, 0x993c, 0x0015, 0x0015, 0x0015, 0x7804,
0x620b, 0x5a06, 0x620b, 0x5a26, 0x7c07, 0x0000, 0x55eb, 0x4d00,
0x7d06, 0xc1fc, 0x57e3, 0x98f7, 0x0007, 0x680c, 0xc215, 0xc20c,
0x98f4, 0x0b70, 0x0311, 0x5313, 0x076c, 0x7c01, 0xc1d9, 0x5efb,
0x068a, 0x076b, 0x7c01, 0xc1d9, 0x5ef3, 0x59db, 0x58d3, 0x018f,
0x0110, 0x390f, 0x008b, 0xc13c, 0x7d2b, 0x5ac0, 0x5bc8, 0xc14e,
0x7c27, 0x0388, 0x0689, 0x5ce3, 0x0dff, 0x0511, 0x1dff, 0x05bc,
0x073e, 0x4d00, 0x7d18, 0x0870, 0x0011, 0x077e, 0x7d09, 0x077d,
0x7d02, 0x5228, 0x9974, 0x52f8, 0x54db, 0x02bc, 0x02cc, 0x7c09,
0x077c, 0x7d02, 0x5228, 0x997d, 0x52f8, 0x54d3, 0x02bc, 0x02cc,
0x7d09, 0x0400, 0x996b, 0x008b, 0x52c0, 0x53c8, 0xc159, 0x7dd6,
0x0200, 0x995b, 0x08ff, 0x00bf, 0x077f, 0x7d15, 0x0488, 0x00d5,
0x7d01, 0x008d, 0x05a0, 0x5deb, 0x028f, 0x0212, 0x0212, 0x3aff,
0x05da, 0x7c02, 0x073e, 0x99a8, 0x02a4, 0x02dd, 0x7d02, 0x073e,
0x99a8, 0x075e, 0x99a8, 0x55eb, 0x4d00, 0x7c02, 0x0598, 0x5deb,
0x52f3, 0x54fb, 0x076a, 0x7d26, 0x076c, 0x7d01, 0x99e5, 0x076b,
0x7c57, 0x0769, 0x7d04, 0x0768, 0x7d02, 0x0e01, 0x99bf, 0x5893,
0x00d6, 0x7d01, 0x008e, 0x5593, 0x05a0, 0x5d93, 0x06a0, 0x7802,
0x5502, 0x5d04, 0x7c1d, 0x4e00, 0x7c08, 0x0769, 0x7d03, 0x5502,
0x7e17, 0x99cc, 0x5d04, 0x7f14, 0x0689, 0x5093, 0x4800, 0x7d01,
0x99b7, 0x9a30, 0x0015, 0x7806, 0x5502, 0x5d04, 0x074f, 0x5502,
0x5d24, 0x072f, 0x7c01, 0x9a30, 0x0017, 0x076f, 0x7c01, 0x2001,
0x5593, 0x009d, 0x0007, 0xda37, 0x9983, 0x6cd3, 0x0769, 0x7d04,
0x0768, 0x7d02, 0x0e01, 0x99f4, 0x5893, 0x00d6, 0x7d01, 0x008e,
0x5593, 0x05a0, 0x5d93, 0x06a0, 0x7802, 0x5502, 0x6dc8, 0x7c0f,
0x4e00, 0x7c08, 0x0769, 0x7d03, 0x5502, 0x7e09, 0x9a01, 0x6dc8,
0x7f06, 0x0689, 0x5093, 0x4800, 0x7d01, 0x99ec, 0x9a30, 0x9a2a,
0x6ac3, 0x0769, 0x7d04, 0x0768, 0x7d02, 0x0e01, 0x9a17, 0x5893,
0x00d6, 0x7d01, 0x008e, 0x5593, 0x05a0, 0x5d93, 0x06a0, 0x7802,
0x65c8, 0x5d04, 0x7c0f, 0x4e00, 0x7c08, 0x0769, 0x7d03, 0x65c8,
0x7e09, 0x9a24, 0x5d04, 0x7f06, 0x0689, 0x5093, 0x4800, 0x7d01,
0x9a0f, 0x9a30, 0x5593, 0x009d, 0x0007, 0x6cff, 0xda37, 0x9983,
0x0000, 0x54e3, 0x55eb, 0x4d00, 0x7c01, 0x9983, 0x996b, 0x54e3,
0x55eb, 0x0aff, 0x0211, 0x1aff, 0x077f, 0x7c02, 0x05a0, 0x9a44,
0x009d, 0x058c, 0x05ba, 0x05a0, 0x0210, 0x04ba, 0x04ad, 0x0454,
0x0006, 0xc1e5, 0x57db, 0x52fb, 0x6ac3, 0x52f3, 0x6a05, 0x5ce3,
0x008f, 0x38ff, 0x00d5, 0x7d01, 0x008d, 0x05a0, 0x0478, 0x7d03,
0x0479, 0x7d30, 0x7c15, 0x0479, 0x7c46, 0x56ee, 0x62c8, 0x7e74,
0x0660, 0x7d02, 0x0210, 0x0212, 0x6a09, 0x7f6e, 0x0212, 0x6a09,
0x7f6b, 0x0212, 0x6a09, 0x7f68, 0x2003, 0x4800, 0x7cef, 0x9abb,
0x0015, 0x0015, 0x52fb, 0x6ac3, 0x048a, 0x57db, 0x068f, 0x0711,
0x3f0f, 0x0610, 0x0611, 0x3e0f, 0x1e01, 0x0617, 0x0617, 0x62c8,
0x6a0b, 0x049e, 0x6cc3, 0x2001, 0x2701, 0x4f00, 0x7cf8, 0x4800,
0x7ce9, 0x9aba, 0x0015, 0x52fb, 0x6ac3, 0x048a, 0x57db, 0x068f,
0x0711, 0x3f0f, 0x0610, 0x0611, 0x3e0f, 0x1e01, 0x0617, 0x0617,
0x62c8, 0x6a0a, 0x049e, 0x6cc3, 0x2001, 0x2701, 0x4f00, 0x7cf8,
0x4800, 0x7ce9, 0x9aba, 0x52fb, 0x6ac3, 0x048a, 0x57db, 0x068f,
0x0711, 0x3f0f, 0x0610, 0x0611, 0x3e0f, 0x1e01, 0x0617, 0x0617,
0x62c8, 0x6a09, 0x049e, 0x6cc3, 0x2001, 0x2701, 0x4f00, 0x7cf8,
0x4800, 0x7ce9, 0x7c19, 0x6a28, 0x7f17, 0x57db, 0x0777, 0x7c0c,
0x0710, 0x3f0f, 0x0717, 0x0717, 0x0e70, 0x0611, 0x0c70, 0x0411,
0x069c, 0x069f, 0x0c01, 0x5c06, 0x0000, 0x54e3, 0x57db, 0x4d00,
0x7d05, 0xc1fc, 0x57db, 0x9a4f, 0xc279, 0x0454, 0xc20c, 0x9a4a,
0xc1d9, 0xc1e5, 0x57db, 0x52f3, 0x6a05, 0x008f, 0x00d5, 0x7d01,
0x008d, 0x05a0, 0x56fb, 0x0478, 0x7d03, 0x0479, 0x7d29, 0x7c1f,
0x0479, 0x7c2e, 0x5de3, 0x0d70, 0x0511, 0x55ed, 0x0f00, 0x0760,
0x7d02, 0x5206, 0x9af4, 0x5226, 0x7e33, 0x0560, 0x7d02, 0x0210,
0x0212, 0x6a09, 0x7f2d, 0x0212, 0x6a09, 0x7f2a, 0x0212, 0x6a09,
0x7f27, 0x1f01, 0x2003, 0x4800, 0x7cea, 0x55e3, 0x9b1f, 0x0015,
0x0015, 0x0015, 0x7804, 0x5206, 0x6a0b, 0x5226, 0x6a0b, 0x9b1e,
0x0015, 0x0015, 0x7804, 0x5206, 0x6a0a, 0x5226, 0x6a0a, 0x9b1e,
0x0015, 0x7804, 0x5206, 0x6a09, 0x5226, 0x6a09, 0x7c09, 0x6a28,
0x7f07, 0x0000, 0x57db, 0x4d00, 0x7d05, 0xc1fc, 0x57db, 0x9add,
0xc279, 0x0454, 0xc20c, 0x9ada, 0xc1e5, 0x57db, 0x52f3, 0x6a05,
0x56fb, 0x028e, 0x1a94, 0x6ac3, 0x62c8, 0x0269, 0x7d26, 0x1e94,
0x6ec3, 0x6ed3, 0x62c8, 0x0a01, 0x0211, 0x6ac8, 0x2694, 0x1e98,
0x6ec3, 0x62c8, 0x2698, 0x6ec3, 0x0260, 0x7d0f, 0x1e98, 0x6ec3,
0x62c8, 0x026c, 0x7d03, 0x2698, 0x6ec3, 0x9b6b, 0x0a10, 0x0211,
0x6ed3, 0x6ac8, 0x2698, 0x6ec3, 0x9b7b, 0x62c8, 0x9b57, 0x6a09,
0x7f18, 0x2501, 0x4d00, 0x7d1f, 0x9b3f, 0x6ee3, 0x008f, 0x05d8,
0x7d01, 0x008d, 0x62c8, 0x6a09, 0x7f0c, 0x2501, 0x2001, 0x7cfa,
0x57db, 0x4d00, 0x7d10, 0x6a28, 0x7f04, 0x0000, 0xc1fc, 0x57db,
0x9b30, 0x0007, 0x6204, 0x6a0c, 0x9b78, 0x6a28, 0x7ffa, 0x6204,
0xc27c, 0x0458, 0x0454, 0x08ff, 0x0011, 0x18ff, 0x00bc, 0x00cd,
0x7c01, 0x9b6b, 0x6a28, 0x7fed, 0x6204, 0xc27c, 0xc20c, 0x9b2d,
0xc1d9, 0xc1e5, 0x57db, 0x52f3, 0x6a05, 0x56fb, 0x028e, 0x1a94,
0x5202, 0x0269, 0x7d1d, 0x1e94, 0x5206, 0x0248, 0x5a06, 0x2694,
0x1e98, 0x5206, 0x024c, 0x5a06, 0x2698, 0x1e98, 0x5206, 0x0260,
0x7c0a, 0x2698, 0x5206, 0x026e, 0x7d23, 0x6a09, 0x7f1d, 0x2501,
0x4d00, 0x7d24, 0x9b9d, 0x6a28, 0x7f17, 0x6204, 0xc27c, 0x9bce,
0x008f, 0x05d8, 0x7d01, 0x008d, 0x05a0, 0x5206, 0x026e, 0x7d10,
0x6a09, 0x7f0a, 0x2001, 0x7cf9, 0x6a28, 0x7f06, 0x0000, 0x4d00,
0x7d0d, 0xc1fc, 0x57db, 0x9b8d, 0x0007, 0x6204, 0x6a0c, 0x9bcb,
0x6a28, 0x7ffa, 0x6204, 0xc27c, 0x0458, 0x0454, 0x6a28, 0x7ff4,
0xc20c, 0x9b8a, 0xdc16, 0x57db, 0x52f3, 0x6a05, 0x69c7, 0x008f,
0x00d5, 0x7d01, 0x008d, 0x05a0, 0x0478, 0x7d03, 0x0479, 0x7d1c,
0x7c15, 0x0479, 0x7c1e, 0x56ee, 0x62c8, 0x7e28, 0x0660, 0x7d02,
0x0210, 0x0212, 0x6a09, 0x7f22, 0x0212, 0x6a09, 0x7f1f, 0x0212,
0x6a09, 0x7f1c, 0x2003, 0x4800, 0x7cef, 0x9c05, 0x0015, 0x0015,
0x7802, 0x62c8, 0x6a0b, 0x9c04, 0x0015, 0x7802, 0x62c8, 0x6a0a,
0x9c04, 0x7802, 0x62c8, 0x6a09, 0x7c09, 0x6a28, 0x7f07, 0x4d00,
0x7d07, 0x52d3, 0x0801, 0x5802, 0x0400, 0x9bd6, 0xc279, 0x0454,
0x52d3, 0x0801, 0x5802, 0x0401, 0xdc2e, 0x9bd3, 0x0b70, 0x0311,
0x5313, 0x5fdb, 0x070a, 0x58d3, 0x008b, 0x5efb, 0xc13c, 0x7d15,
0x5ac0, 0x5bc8, 0xc14e, 0x7c11, 0x0388, 0x5df0, 0x0dff, 0x0511,
0x1dff, 0x05bc, 0x4d00, 0x7d04, 0x9c2d, 0x0708, 0x5fdb, 0x070a,
0x008b, 0x52c3, 0x53cb, 0xc159, 0x7dec, 0x0200, 0x9c1e, 0x6e01,
0x610b, 0x7e2f, 0x620b, 0x7e2d, 0x630b, 0x7e2b, 0x0d0c, 0x0417,
0x0417, 0x0417, 0x049d, 0x1d08, 0x05cc, 0x7c01, 0x0d0c, 0x6ad1,
0x0f00, 0x0742, 0x6fc8, 0x6fdd, 0x7f1c, 0x008e, 0x009d, 0x6801,
0x670b, 0x7e17, 0x6bd5, 0x0804, 0x7802, 0x6fc8, 0x0712, 0x7c11,
0x670b, 0x7e0f, 0x0804, 0x7802, 0x6fc8, 0x0712, 0x7c0a, 0x6fdd,
0x7f08, 0x69d1, 0x0f01, 0x6fc8, 0x6fdd, 0x7f03, 0x0101, 0x0400,
0x9c43, 0x0007, 0x68ff, 0x680c, 0x0200, 0x9c43, 0xdcb3, 0x57db,
0x52f3, 0x6a05, 0x52fb, 0x6ac7, 0x008f, 0x00d5, 0x7d01, 0x008d,
0x05a0, 0x0478, 0x7d03, 0x0479, 0x7d1c, 0x7c15, 0x0479, 0x7c1e,
0x56ee, 0x62c8, 0x7e28, 0x0660, 0x7d02, 0x0210, 0x0212, 0x6a09,
0x7f22, 0x0212, 0x6a09, 0x7f1f, 0x0212, 0x6a09, 0x7f1c, 0x2003,
0x4800, 0x7cef, 0x9ca2, 0x0015, 0x0015, 0x7802, 0x62c8, 0x6a0b,
0x9ca1, 0x0015, 0x7802, 0x62c8, 0x6a0a, 0x9ca1, 0x7802, 0x62c8,
0x6a09, 0x7c09, 0x6a28, 0x7f07, 0x4d00, 0x7d07, 0x52d3, 0x0801,
0x5802, 0x0400, 0x9c72, 0xc279, 0x0454, 0x52d3, 0x0801, 0x5802,
0x0401, 0xdccb, 0x9c6f, 0x0b70, 0x0311, 0x5313, 0x5fdb, 0x070a,
0x58d3, 0x008b, 0x5efb, 0xc13c, 0x7d15, 0x5ac0, 0x5bc8, 0xc14e,
0x7c11, 0x0388, 0x5df0, 0x0dff, 0x0511, 0x1dff, 0x05bc, 0x4d00,
0x7d04, 0x9cca, 0x0708, 0x5fdb, 0x070a, 0x008b, 0x52c3, 0x53cb,
0xc159, 0x7dec, 0x0200, 0x9cbb
};
#endif
