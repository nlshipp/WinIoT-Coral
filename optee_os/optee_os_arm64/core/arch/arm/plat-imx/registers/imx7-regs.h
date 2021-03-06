/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright 2017-2019 NXP
 */
#ifndef __IMX7_REGS_H__
#define __IMX7_REGS_H__
#include <registers/imx7-crm_regs.h>
#include <registers/imx7-gpc_regs.h>
#include <registers/imx7-src_regs.h>
#include <registers/imx7-iomux_regs.h>
#include <registers/imx7-ddrc_regs.h>

#define GIC_BASE		0x31000000
#define GICC_OFFSET		0x2000
#define GICD_OFFSET		0x1000

#define UART1_BASE		0x30860000
#define AIPS1_BASE		0x30000000
#define AIPS1_SIZE		0x400000
#define AIPS2_BASE		0x30400000
#define AIPS2_SIZE		0x400000
#define AIPS3_BASE		0x30800000
#define AIPS3_SIZE		0x400000
#define LPSR_BASE		0x30270000
#define IOMUXC_BASE		0x30330000
#define IOMUXC_GPR_BASE		0x30340000
#define OCOTP_BASE		0x30350000
#define ANATOP_BASE		0x30360000
#define SNVS_BASE		0x30370000
#define CCM_BASE		0x30380000
#define SRC_BASE		0x30390000
#define GPC_BASE		0x303A0000
#define TZASC_BASE		0x30780000
#define DDRC_PHY_BASE		0x30790000
#define MMDC_P0_BASE		0x307A0000
#define DDRC_BASE		0x307A0000
#define IRAM_S_BASE		0x00180000
#define WDOG_BASE               0x30280000
#define CAAM_BASE               0x30900000

#define CSU_BASE		0x303E0000
#define CSU_CSL_START	0x0
#define CSU_CSL_END		0x100
#define CSU_ACCESS_ALL		0x00FF00FF
#define CSU_SETTING_LOCK	0x01000100

#define TRUSTZONE_OCRAM_START	0x180000

#endif /* __IMX7_REGS_H__ */
