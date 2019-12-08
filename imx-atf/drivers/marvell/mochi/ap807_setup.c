/*
 * Copyright (C) 2018 Marvell International Ltd.
 *
 * SPDX-License-Identifier:	BSD-3-Clause
 * https://spdx.org/licenses
 */

/* AP807 Marvell SoC driver */

#include <ap_setup.h>
#include <cache_llc.h>
#include <ccu.h>
#include <debug.h>
#include <io_win.h>
#include <mci.h>
#include <mmio.h>
#include <mvebu_def.h>

#define SMMU_sACR				(MVEBU_SMMU_BASE + 0x10)
#define SMMU_sACR_PG_64K			(1 << 16)

#define CCU_GSPMU_CR				(MVEBU_CCU_BASE(MVEBU_AP0) \
								+ 0x3F0)
#define GSPMU_CPU_CONTROL			(0x1 << 0)

#define CCU_HTC_CR				(MVEBU_CCU_BASE(MVEBU_AP0) \
								+ 0x200)
#define CCU_SET_POC_OFFSET			5

#define DSS_CR0					(MVEBU_RFU_BASE + 0x100)
#define DVM_48BIT_VA_ENABLE			(1 << 21)

/* Secure MoChi incoming access */
#define SEC_MOCHI_IN_ACC_REG			(MVEBU_RFU_BASE + 0x4738)
#define SEC_MOCHI_IN_ACC_IHB0_EN		(1)
#define SEC_MOCHI_IN_ACC_IHB1_EN		(1 << 3)
#define SEC_MOCHI_IN_ACC_IHB2_EN		(1 << 6)
#define SEC_MOCHI_IN_ACC_PIDI_EN		(1 << 9)
#define SEC_IN_ACCESS_ENA_ALL_MASTERS		(SEC_MOCHI_IN_ACC_IHB0_EN | \
						 SEC_MOCHI_IN_ACC_IHB1_EN | \
						 SEC_MOCHI_IN_ACC_IHB2_EN | \
						 SEC_MOCHI_IN_ACC_PIDI_EN)

/* SYSRST_OUTn Config definitions */
#define MVEBU_SYSRST_OUT_CONFIG_REG		(MVEBU_MISC_SOC_BASE + 0x4)
#define WD_MASK_SYS_RST_OUT			(1 << 2)

/* DSS PHY for DRAM */
#define DSS_SCR_REG				(MVEBU_RFU_BASE + 0x208)
#define DSS_PPROT_OFFS				4
#define DSS_PPROT_MASK				0x7
#define DSS_PPROT_PRIV_SECURE_DATA		0x1

/* Used for Units of AP-807 (e.g. SDIO and etc) */
#define MVEBU_AXI_ATTR_BASE			(MVEBU_REGS_BASE + 0x6F4580)
#define MVEBU_AXI_ATTR_REG(index)		(MVEBU_AXI_ATTR_BASE + \
							0x4 * index)

enum axi_attr {
	AXI_SDIO_ATTR = 0,
	AXI_DFX_ATTR,
	AXI_MAX_ATTR,
};

static void ap_sec_masters_access_en(uint32_t enable)
{
	uint32_t reg;

	/* Open/Close incoming access for all masters.
	 * The access is disabled in trusted boot mode
	 * Could only be done in EL3
	 */
	reg = mmio_read_32(SEC_MOCHI_IN_ACC_REG);
	if (enable)
		mmio_write_32(SEC_MOCHI_IN_ACC_REG, reg |
			      SEC_IN_ACCESS_ENA_ALL_MASTERS);
	else
		mmio_write_32(SEC_MOCHI_IN_ACC_REG,
			      reg & ~SEC_IN_ACCESS_ENA_ALL_MASTERS);
}

static void setup_smmu(void)
{
	uint32_t reg;

	/* Set the SMMU page size to 64 KB */
	reg = mmio_read_32(SMMU_sACR);
	reg |= SMMU_sACR_PG_64K;
	mmio_write_32(SMMU_sACR, reg);
}

static void init_aurora2(void)
{
	uint32_t reg;

	/* Enable GSPMU control by CPU */
	reg = mmio_read_32(CCU_GSPMU_CR);
	reg |= GSPMU_CPU_CONTROL;
	mmio_write_32(CCU_GSPMU_CR, reg);

#if LLC_ENABLE
	/* Enable LLC for AP807 in exclusive mode */
	llc_enable(0, 1);

	/* Set point of coherency to DDR.
	 * This is required by units which have
	 * SW cache coherency
	 */
	reg = mmio_read_32(CCU_HTC_CR);
	reg |= (0x1 << CCU_SET_POC_OFFSET);
	mmio_write_32(CCU_HTC_CR, reg);
#endif /* LLC_ENABLE */
}


/* MCIx indirect access register are based by default at 0xf4000000/0xf6000000
 * to avoid conflict of internal registers of units connected via MCIx, which
 * can be based on the same address (i.e CP1 base is also 0xf4000000),
 * the following routines remaps the MCIx indirect bases to another domain
 */
static void mci_remap_indirect_access_base(void)
{
	uint32_t mci;

	for (mci = 0; mci < MCI_MAX_UNIT_ID; mci++)
		mmio_write_32(MCIX4_REG_START_ADDRESS_REG(mci),
				  MVEBU_MCI_REG_BASE_REMAP(mci) >>
				  MCI_REMAP_OFF_SHIFT);
}

static void ap807_axi_attr_init(void)
{
	uint32_t index, data;

	/* Initialize AXI attributes for AP807 */
	/* Go over the AXI attributes and set Ax-Cache and Ax-Domain */
	for (index = 0; index < AXI_MAX_ATTR; index++) {
		switch (index) {
		/* DFX works with no coherent only -
		 * there's no option to configure the Ax-Cache and Ax-Domain
		 */
		case AXI_DFX_ATTR:
			continue;
		default:
			/* Set Ax-Cache as cacheable, no allocate, modifiable,
			 * bufferable.
			 * The values are different because Read & Write
			 * definition is different in Ax-Cache
			 */
			data = mmio_read_32(MVEBU_AXI_ATTR_REG(index));
			data &= ~MVEBU_AXI_ATTR_ARCACHE_MASK;
			data |= (CACHE_ATTR_WRITE_ALLOC |
				 CACHE_ATTR_CACHEABLE   |
				 CACHE_ATTR_BUFFERABLE) <<
				 MVEBU_AXI_ATTR_ARCACHE_OFFSET;
			data &= ~MVEBU_AXI_ATTR_AWCACHE_MASK;
			data |= (CACHE_ATTR_READ_ALLOC |
				 CACHE_ATTR_CACHEABLE  |
				 CACHE_ATTR_BUFFERABLE) <<
				 MVEBU_AXI_ATTR_AWCACHE_OFFSET;
			/* Set Ax-Domain as Outer domain */
			data &= ~MVEBU_AXI_ATTR_ARDOMAIN_MASK;
			data |= DOMAIN_OUTER_SHAREABLE <<
				MVEBU_AXI_ATTR_ARDOMAIN_OFFSET;
			data &= ~MVEBU_AXI_ATTR_AWDOMAIN_MASK;
			data |= DOMAIN_OUTER_SHAREABLE <<
				MVEBU_AXI_ATTR_AWDOMAIN_OFFSET;
			mmio_write_32(MVEBU_AXI_ATTR_REG(index), data);
		}
	}
}

static void misc_soc_configurations(void)
{
	uint32_t reg;

	/* Enable 48-bit VA */
	mmio_setbits_32(DSS_CR0, DVM_48BIT_VA_ENABLE);

	/* Un-mask Watchdog reset from influencing the SYSRST_OUTn.
	 * Otherwise, upon WD timeout, the WD reset signal won't trigger reset
	 */
	reg = mmio_read_32(MVEBU_SYSRST_OUT_CONFIG_REG);
	reg &= ~(WD_MASK_SYS_RST_OUT);
	mmio_write_32(MVEBU_SYSRST_OUT_CONFIG_REG, reg);
}

void ap_init(void)
{
	/* Setup Aurora2. */
	init_aurora2();

	/* configure MCI mapping */
	mci_remap_indirect_access_base();

	/* configure IO_WIN windows */
	init_io_win(MVEBU_AP0);

	/* configure CCU windows */
	init_ccu(MVEBU_AP0);

	/* configure the SMMU */
	setup_smmu();

	/* Open AP incoming access for all masters */
	ap_sec_masters_access_en(1);

	/* configure axi for AP */
	ap807_axi_attr_init();

	/* misc configuration of the SoC */
	misc_soc_configurations();
}

static void ap807_dram_phy_access_config(void)
{
	uint32_t reg_val;
	/* Update DSS port access permission to DSS_PHY */
	reg_val = mmio_read_32(DSS_SCR_REG);
	reg_val &= ~(DSS_PPROT_MASK << DSS_PPROT_OFFS);
	reg_val |= ((DSS_PPROT_PRIV_SECURE_DATA & DSS_PPROT_MASK) <<
		    DSS_PPROT_OFFS);
	mmio_write_32(DSS_SCR_REG, reg_val);
}

void ap_ble_init(void)
{
	/* Enable DSS port */
	ap807_dram_phy_access_config();
}

int ap_get_count(void)
{
	return 1;
}
