// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 * Copyright 2018 NXP
 *
 * Peng Fan <peng.fan@nxp.com>
 */

#include <arm32.h>
#include <io.h>
#include <kernel/generic_boot.h>
#include <kernel/tz_ssvce_def.h>
#include <kernel/tz_ssvce_pl310.h>
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#include <sm/optee_smc.h>
#include <platform_config.h>
#include <stdint.h>
#include "imx_pl310.h"

#define PL310_AUX_CTRL_FLZW			BIT(0)
#define PL310_DEBUG_CTRL_DISABLE_WRITEBACK	BIT(1)
#define PL310_DEBUG_CTRL_DISABLE_LINEFILL	BIT(0)

register_phys_mem(MEM_AREA_IO_SEC, PL310_BASE, CORE_MMU_DEVICE_SIZE);
register_phys_mem(MEM_AREA_IO_NSEC, PL310_BASE, CORE_MMU_DEVICE_SIZE);

void arm_cl2_config(vaddr_t pl310_base)
{
	uint32_t val, cache_id;

	/* Disable PL310 */
	write32(0, pl310_base + PL310_CTRL);

	write32(PL310_TAG_RAM_CTRL_INIT, pl310_base + PL310_TAG_RAM_CTRL);
	write32(PL310_DATA_RAM_CTRL_INIT, pl310_base + PL310_DATA_RAM_CTRL);
	write32(PL310_AUX_CTRL_INIT, pl310_base + PL310_AUX_CTRL);
	/*
	 * The L2 cache controller(PL310) version on the i.MX6D/Q
	 * is r3p1-50rel0
	 * The L2 cache controller(PL310) version on the
	 * i.MX6DL/SOLO/SL/SX/DQP is r3p2.
	 * But according to ARM PL310 errata: 752271
	 * ID: 752271: Double linefill feature can cause data corruption
	 * Fault Status: Present in: r3p0, r3p1, r3p1-50rel0. Fixed in r3p2
	 * Workaround: The only workaround to this erratum is to disable the
	 * double linefill feature. This is the default behavior.
	 */
	val = PL310_PREFETCH_CTRL_INIT;
	cache_id = read32(pl310_base + PL310_CACHE_ID);
	if (((cache_id & PL310_CACHE_ID_PART_MASK) == PL310_CACHE_ID_PART_L310)
	    && ((cache_id & PL310_CACHE_ID_RTL_MASK) < PL310_CACHE_ID_RTL_R3P2))
		val &= ~(1 << 30);
	write32(val, pl310_base + PL310_PREFETCH_CTRL);

	write32(PL310_POWER_CTRL_INIT, pl310_base + PL310_POWER_CTRL);

	/* invalidate all cache ways */
	arm_cl2_invbyway(pl310_base);
}

bool pl310_enabled(vaddr_t pl310_base)
{
	return read32(pl310_base + PL310_CTRL) & 1;
}

void arm_cl2_enable(vaddr_t pl310_base)
{
	uint32_t val __maybe_unused;

	/* Enable PL310 ctrl -> only set lsb bit */
	write32(1, pl310_base + PL310_CTRL);

#ifndef CFG_PL310_SIP_PROTOCOL
	/* if L2 FLZW enable, enable in L1 */
	val = read32(pl310_base + PL310_AUX_CTRL);
	if (val & PL310_AUX_CTRL_FLZW)
		write_actlr(read_actlr() | ACTLR_CA9_WFLZ);
#endif
}

vaddr_t pl310_base(void)
{
	return core_mmu_get_va(PL310_BASE, MEM_AREA_IO_SEC);
}

vaddr_t pl310_nsbase(void)
{
	return core_mmu_get_va(PL310_BASE, MEM_AREA_IO_NSEC);
}

#ifdef CFG_PL310_SIP_PROTOCOL
uint32_t pl310_enable(void)
{
	vaddr_t base = pl310_base();

	arm_cl2_config(base);
	arm_cl2_enable(base);
	return OPTEE_SMC_RETURN_OK;
}

uint32_t pl310_disable(void)
{
	EMSG("not implemented");
	return OPTEE_SMC_RETURN_ENOTAVAIL;
}

uint32_t pl310_enable_writeback(void)
{
	vaddr_t base = pl310_base();

	write32(0, base + PL310_DEBUG_CTRL);
	return OPTEE_SMC_RETURN_OK;
}

uint32_t pl310_disable_writeback(void)
{
	vaddr_t base = pl310_base();
	uint32_t val = PL310_DEBUG_CTRL_DISABLE_WRITEBACK |
		       PL310_DEBUG_CTRL_DISABLE_LINEFILL;

	write32(val, base + PL310_DEBUG_CTRL);
	return OPTEE_SMC_RETURN_OK;
}

uint32_t pl310_enable_wflz(void)
{
	write_actlr(read_actlr() | ACTLR_CA9_WFLZ);
	return OPTEE_SMC_RETURN_OK;
}
#endif
