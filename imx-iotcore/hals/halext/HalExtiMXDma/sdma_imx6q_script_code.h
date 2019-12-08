//-----------------------------------------------------------------------------
//
//  Copyright (C) 2007-2011, Freescale Semiconductor, Inc. All Rights Reserved.
//  THIS SOURCE CODE, AND ITS USE AND DISTRIBUTION, IS SUBJECT TO THE TERMS
//  AND CONDITIONS OF THE APPLICABLE LICENSE AGREEMENT
//
//-----------------------------------------------------------------------------
//
// file: sdma_imx6q_script_code.h
// This file contains functions of SDMA scripts code initialization
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
//           SDMA RELEASE LABEL:     "DTCP.00.01.02"
//
//-----------------------------------------------------------------------------

#ifndef SDMA_IMX6Q_SCRIPT_CODE_H
#define SDMA_IMX6Q_SCRIPT_CODE_H

/*!
* SDMA ROM scripts start addresses and sizes
*/

#define imx6_start_ADDR   0
#define imx6_start_SIZE   20

#define imx6_core_ADDR   80
#define imx6_core_SIZE   232

#define imx6_common_ADDR   312
#define imx6_common_SIZE   330

#define imx6_ap_2_ap_ADDR   642
#define imx6_ap_2_ap_SIZE   41

#define imx6_app_2_mcu_ADDR   683
#define imx6_app_2_mcu_SIZE   64

#define imx6_mcu_2_app_ADDR   747
#define imx6_mcu_2_app_SIZE   70

#define imx6_uart_2_mcu_ADDR   817
#define imx6_uart_2_mcu_SIZE   74

#define imx6_shp_2_mcu_ADDR   891
#define imx6_shp_2_mcu_SIZE   69

#define imx6_mcu_2_shp_ADDR   960
#define imx6_mcu_2_shp_SIZE   72

#define imx6_uartsh_2_mcu_ADDR   1032
#define imx6_uartsh_2_mcu_SIZE   68

#define imx6_spdif_2_mcu_ADDR   1100
#define imx6_spdif_2_mcu_SIZE   34

#define imx6_mcu_2_spdif_ADDR   1134
#define imx6_mcu_2_spdif_SIZE   59

#define imx6_loop_DMAs_routines_ADDR   1193
#define imx6_loop_DMAs_routines_SIZE   227

#define imx6_test_ADDR   1420
#define imx6_test_SIZE   63

#define imx6_signature_ADDR   1023
#define imx6_signature_SIZE   1

/*!
* SDMA RAM scripts start addresses and sizes
*/

#define imx6_dtcp__mcu_ADDR   6144
#define imx6_dtcp__mcu_SIZE   143

#define imx6_mcu_2_ssiapp_ADDR   6287
#define imx6_mcu_2_ssiapp_SIZE   98

#define imx6_mcu_2_ssish_ADDR   6385
#define imx6_mcu_2_ssish_SIZE   89

#define imx6_p_2_p_ADDR   6474
#define imx6_p_2_p_SIZE   254

#define imx6_ssiapp_2_mcu_ADDR   6728
#define imx6_ssiapp_2_mcu_SIZE   93

#define imx6_ssish_2_mcu_ADDR   6821
#define imx6_ssish_2_mcu_SIZE   84

/*!
* SDMA RAM image start address and size
*/

#define IMX6_RAM_CODE_START_ADDR     6144
#define IMX6_RAM_CODE_SIZE           761

/*!
* Buffer that holds the SDMA RAM image
*/

static const short imx6_sdma_code[] =
{
0x0d70, 0x0511, 0x5515, 0x0217, 0x0217, 0x0217, 0x5aed, 0x5ef5,
0x5ffd, 0x0b01, 0x0666, 0x7c01, 0x0317, 0x0667, 0x7c02, 0x0317,
0x0317, 0x0668, 0x7c04, 0x0317, 0x0317, 0x0317, 0x0317, 0x5bc5,
0x0f84, 0x0712, 0x2f13, 0x0712, 0x2f00, 0x0712, 0x2f02, 0x5fcd,
0xc13c, 0x7d61, 0xc14e, 0x7c5f, 0x1d08, 0x6d01, 0x6e05, 0x0e70,
0x0611, 0x058e, 0x5616, 0x5ad6, 0x0aff, 0x0211, 0x1aff, 0x02bc,
0x5bde, 0x0388, 0x5ce6, 0x54ee, 0x02d4, 0x7c01, 0x048a, 0x02a4,
0x572d, 0x07b9, 0x4f00, 0x7c05, 0x57fd, 0x07bb, 0x4f00, 0x7c01,
0x9838, 0x57fe, 0x6fc7, 0x0802, 0x7802, 0x67c8, 0x6f0b, 0x7c1d,
0x6f28, 0x7f1b, 0xd889, 0x7f19, 0x0401, 0x2408, 0x7c02, 0x4a00,
0x7d1a, 0x57f6, 0x6fd7, 0x0802, 0x7802, 0x670b, 0x6fc8, 0x7c0d,
0x6fdf, 0x7f0b, 0x4c00, 0x7cdc, 0x0000, 0x9833, 0x50d7, 0x51df,
0x5ad7, 0x5bdf, 0x5ce7, 0x0388, 0x068f, 0x0007, 0x68ff, 0x680c,
0x54e6, 0x0454, 0x5ce6, 0x008b, 0x52d6, 0x53de, 0x54e6, 0x078e,
0xc159, 0x7c11, 0xc14e, 0x7c0f, 0x58d7, 0x59df, 0x51f7, 0x69d7,
0x6d01, 0x0802, 0x7802, 0x610b, 0x69c8, 0x7ce0, 0x69df, 0x7fde,
0x50d7, 0x51df, 0x9824, 0x0e70, 0x0611, 0x5616, 0xd889, 0x0400,
0x9820, 0x57ce, 0x6fd2, 0x57c6, 0x6fc8, 0x6fde, 0x0006, 0xc1e3,
0x57db, 0x5fe3, 0x57e3, 0x52f3, 0x6a01, 0x008f, 0x00d5, 0x7d01,
0x008d, 0x05a0, 0x5deb, 0x0478, 0x7d03, 0x0479, 0x7d2c, 0x7c36,
0x0479, 0x7c1f, 0x56ee, 0x0f00, 0x0660, 0x7d05, 0x6509, 0x7e43,
0x620a, 0x7e41, 0x98af, 0x620a, 0x7e3e, 0x6509, 0x7e3c, 0x0512,
0x0512, 0x02ad, 0x0760, 0x7d03, 0x55fb, 0x6dd3, 0x98ba, 0x55fb,
0x1d04, 0x6dd3, 0x6ac8, 0x7f2f, 0x1f01, 0x2003, 0x4800, 0x7ce4,
0x98e2, 0x55fb, 0x6dd7, 0x0015, 0x7805, 0x6209, 0x6ac8, 0x6209,
0x6ac8, 0x6dd7, 0x98e1, 0x55fb, 0x6dd7, 0x0015, 0x0015, 0x7805,
0x620a, 0x6ac8, 0x620a, 0x6ac8, 0x6dd7, 0x98e1, 0x55fb, 0x6dd7,
0x0015, 0x0015, 0x0015, 0x7805, 0x620b, 0x6ac8, 0x620b, 0x6ac8,
0x6dd7, 0x7c09, 0x6ddf, 0x7f07, 0x0000, 0x55eb, 0x4d00, 0x7d07,
0xc1fa, 0x57e3, 0x9895, 0x0007, 0x68cc, 0x680c, 0xc213, 0xc20a,
0x9892, 0xc1d9, 0xc1e3, 0x57db, 0x5fe3, 0x57e3, 0x52f3, 0x6a21,
0x008f, 0x00d5, 0x7d01, 0x008d, 0x05a0, 0x5deb, 0x56fb, 0x0478,
0x7d03, 0x0479, 0x7d2a, 0x7c31, 0x0479, 0x7c20, 0x0b70, 0x0311,
0x53eb, 0x0f00, 0x0360, 0x7d05, 0x6509, 0x7e37, 0x620a, 0x7e35,
0x9915, 0x620a, 0x7e32, 0x6509, 0x7e30, 0x0512, 0x0512, 0x02ad,
0x0760, 0x7c02, 0x5a06, 0x991d, 0x5a26, 0x7f27, 0x1f01, 0x2003,
0x4800, 0x7ce8, 0x0b70, 0x0311, 0x5313, 0x993e, 0x0015, 0x7804,
0x6209, 0x5a06, 0x6209, 0x5a26, 0x993d, 0x0015, 0x0015, 0x7804,
0x620a, 0x5a06, 0x620a, 0x5a26, 0x993d, 0x0015, 0x0015, 0x0015,
0x7804, 0x620b, 0x5a06, 0x620b, 0x5a26, 0x7c07, 0x0000, 0x55eb,
0x4d00, 0x7d06, 0xc1fa, 0x57e3, 0x98f8, 0x0007, 0x680c, 0xc213,
0xc20a, 0x98f5, 0x0b70, 0x0311, 0x5313, 0x076c, 0x7c01, 0xc1d9,
0x5efb, 0x068a, 0x076b, 0x7c01, 0xc1d9, 0x5ef3, 0x59db, 0x58d3,
0x018f, 0x0110, 0x390f, 0x008b, 0xc13c, 0x7d2b, 0x5ac0, 0x5bc8,
0xc14e, 0x7c27, 0x0388, 0x0689, 0x5ce3, 0x0dff, 0x0511, 0x1dff,
0x05bc, 0x073e, 0x4d00, 0x7d18, 0x0870, 0x0011, 0x077e, 0x7d09,
0x077d, 0x7d02, 0x5228, 0x9975, 0x52f8, 0x54db, 0x02bc, 0x02cc,
0x7c09, 0x077c, 0x7d02, 0x5228, 0x997e, 0x52f8, 0x54d3, 0x02bc,
0x02cc, 0x7d09, 0x0400, 0x996c, 0x008b, 0x52c0, 0x53c8, 0xc159,
0x7dd6, 0x0200, 0x995c, 0x08ff, 0x00bf, 0x077f, 0x7d15, 0x0488,
0x00d5, 0x7d01, 0x008d, 0x05a0, 0x5deb, 0x028f, 0x0212, 0x0212,
0x3aff, 0x05da, 0x7c02, 0x073e, 0x99a7, 0x02a4, 0x02dd, 0x7d02,
0x073e, 0x99a7, 0x075e, 0x99a7, 0x55eb, 0x0598, 0x5deb, 0x52f3,
0x54fb, 0x076a, 0x7d26, 0x076c, 0x7d01, 0x99e4, 0x076b, 0x7c57,
0x0769, 0x7d04, 0x0768, 0x7d02, 0x0e01, 0x99be, 0x5893, 0x00d6,
0x7d01, 0x008e, 0x5593, 0x05a0, 0x5d93, 0x06a0, 0x7802, 0x5502,
0x5d04, 0x7c1d, 0x4e00, 0x7c08, 0x0769, 0x7d03, 0x5502, 0x7e17,
0x99cb, 0x5d04, 0x7f14, 0x0689, 0x5093, 0x4800, 0x7d01, 0x99b6,
0x9a2f, 0x0015, 0x7806, 0x5502, 0x5d04, 0x074f, 0x5502, 0x5d24,
0x072f, 0x7c01, 0x9a2f, 0x0017, 0x076f, 0x7c01, 0x2001, 0x5593,
0x009d, 0x0007, 0xda36, 0x9984, 0x6cd3, 0x0769, 0x7d04, 0x0768,
0x7d02, 0x0e01, 0x99f3, 0x5893, 0x00d6, 0x7d01, 0x008e, 0x5593,
0x05a0, 0x5d93, 0x06a0, 0x7802, 0x5502, 0x6dc8, 0x7c0f, 0x4e00,
0x7c08, 0x0769, 0x7d03, 0x5502, 0x7e09, 0x9a00, 0x6dc8, 0x7f06,
0x0689, 0x5093, 0x4800, 0x7d01, 0x99eb, 0x9a2f, 0x9a29, 0x6ac3,
0x0769, 0x7d04, 0x0768, 0x7d02, 0x0e01, 0x9a16, 0x5893, 0x00d6,
0x7d01, 0x008e, 0x5593, 0x05a0, 0x5d93, 0x06a0, 0x7802, 0x65c8,
0x5d04, 0x7c0f, 0x4e00, 0x7c08, 0x0769, 0x7d03, 0x65c8, 0x7e09,
0x9a23, 0x5d04, 0x7f06, 0x0689, 0x5093, 0x4800, 0x7d01, 0x9a0e,
0x9a2f, 0x5593, 0x009d, 0x0007, 0x6cff, 0xda36, 0x9984, 0x0000,
0x54e3, 0x55eb, 0x4d00, 0x7c01, 0x9984, 0x996c, 0x54e3, 0x55eb,
0x0aff, 0x0211, 0x1aff, 0x077f, 0x7c02, 0x05a0, 0x9a43, 0x009d,
0x058c, 0x05ba, 0x05a0, 0x0210, 0x04ba, 0x04ad, 0x0454, 0x0006,
0xc1e3, 0x57db, 0x52fb, 0x6ac3, 0x52f3, 0x6a05, 0x008f, 0x00d5,
0x7d01, 0x008d, 0x05a0, 0x5deb, 0x0478, 0x7d03, 0x0479, 0x7d2b,
0x7c1e, 0x0479, 0x7c33, 0x56ee, 0x0f00, 0x55fb, 0x0760, 0x7d02,
0x6dc3, 0x9a64, 0x1d04, 0x6dc3, 0x62c8, 0x7e3b, 0x0660, 0x7d02,
0x0210, 0x0212, 0x6a09, 0x7f35, 0x0212, 0x6a09, 0x7f32, 0x0212,
0x6a09, 0x7f2f, 0x1f01, 0x2003, 0x4800, 0x7ce7, 0x9a98, 0x55fb,
0x6dc7, 0x0015, 0x0015, 0x0015, 0x7805, 0x62c8, 0x6a0b, 0x62c8,
0x6a0b, 0x6dc7, 0x9a97, 0x55fb, 0x6dc7, 0x0015, 0x0015, 0x7805,
0x62c8, 0x6a0a, 0x62c8, 0x6a0a, 0x6dc7, 0x9a97, 0x55fb, 0x6dc7,
0x0015, 0x7805, 0x62c8, 0x6a09, 0x62c8, 0x6a09, 0x6dc7, 0x7c09,
0x6a28, 0x7f07, 0x0000, 0x55eb, 0x4d00, 0x7d05, 0xc1fa, 0x57db,
0x9a4e, 0xc277, 0x0454, 0xc20a, 0x9a49, 0xc1d9, 0xc1e3, 0x57db,
0x52f3, 0x6a05, 0x008f, 0x00d5, 0x7d01, 0x008d, 0x05a0, 0x56fb,
0x0478, 0x7d03, 0x0479, 0x7d29, 0x7c1f, 0x0479, 0x7c2e, 0x5de3,
0x0d70, 0x0511, 0x55ed, 0x0f00, 0x0760, 0x7d02, 0x5206, 0x9ac1,
0x5226, 0x7e33, 0x0560, 0x7d02, 0x0210, 0x0212, 0x6a09, 0x7f2d,
0x0212, 0x6a09, 0x7f2a, 0x0212, 0x6a09, 0x7f27, 0x1f01, 0x2003,
0x4800, 0x7cea, 0x55e3, 0x9aec, 0x0015, 0x0015, 0x0015, 0x7804,
0x5206, 0x6a0b, 0x5226, 0x6a0b, 0x9aeb, 0x0015, 0x0015, 0x7804,
0x5206, 0x6a0a, 0x5226, 0x6a0a, 0x9aeb, 0x0015, 0x7804, 0x5206,
0x6a09, 0x5226, 0x6a09, 0x7c09, 0x6a28, 0x7f07, 0x0000, 0x57db,
0x4d00, 0x7d05, 0xc1fa, 0x57db, 0x9aaa, 0xc277, 0x0454, 0xc20a,
0x9aa7
};
#endif
