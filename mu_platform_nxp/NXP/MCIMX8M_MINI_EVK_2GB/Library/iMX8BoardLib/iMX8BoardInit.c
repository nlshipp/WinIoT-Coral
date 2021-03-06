/** @file
*
*  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
*  Copyright 2019 NXP
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>
#include <Library/ArmSmcLib.h>
#include <Ppi/ArmMpCoreInfo.h>

#include "iMX8.h"

ARM_CORE_INFO iMX8Ppi[] =
{
  {
    // Cluster 0, Core 0
    0x0, 0x0,
    // MP Core MailBox Set/Get/Clear Addresses and Clear Value. Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },
#if FixedPcdGet32(PcdCoreCount) > 1
  {
    // Cluster 0, Core 1
    0x0, 0x1,
    // MP Core MailBox Set/Get/Clear Addresses and Clear Value. Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },
#endif // FixedPcdGet32(PcdCoreCount) > 1
#if FixedPcdGet32(PcdCoreCount) > 2
  {
    // Cluster 0, Core 2
    0x0, 0x2,
    // MP Core MailBox Set/Get/Clear Addresses and Clear Value. Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },
  {
    // Cluster 0, Core 3
    0x0, 0x3,
    // MP Core MailBox Set/Get/Clear Addresses and Clear Value. Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  }
#endif // FixedPcdGet32(PcdCoreCount) > 2
};

EFI_STATUS PrePeiCoreGetMpCoreInfo (OUT UINTN *CoreCount, OUT ARM_CORE_INFO **ArmCoreTable)
{
  // Only support one cluster
  *CoreCount = sizeof(iMX8Ppi) / sizeof(ARM_CORE_INFO);
  ASSERT (*CoreCount == FixedPcdGet32 (PcdCoreCount));
  *ArmCoreTable = iMX8Ppi;
  return EFI_SUCCESS;
}

ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };

EFI_PEI_PPI_DESCRIPTOR      gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID ArmPlatformGetPlatformPpiList (OUT UINTN *PpiListSize, OUT EFI_PEI_PPI_DESCRIPTOR **PpiList)
{
  *PpiListSize = sizeof(gPlatformPpiTable);
  *PpiList     = gPlatformPpiTable;
}

VOID UngateClocks ()
{
  // Enable SDMA
  CCM_CCGR_SDMA1 = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
  CCM_CCGR_SDMA2 = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
  // Enable UARTs
  CCM_CCGR_UART1 = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
  CCM_CCGR_UART2 = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
  CCM_CCGR_UART3 = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
  CCM_CCGR_UART4 = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
}

#if FixedPcdGet32(PcdPcie1Enable)
/**
  Initialize PCI Express module on the SOC and perform required pin-muxing
**/
VOID PcieInit ()
{
  ARM_SMC_ARGS smc_args;

  // Enable HSIOMIX power domain
  smc_args.Arg0 = 0xC2000000;
  smc_args.Arg1 = 0x03;
  smc_args.Arg2 = 0x00;
  smc_args.Arg3 = 0x01;
  smc_args.Arg4 = 0x00;
  ArmCallSmc(&smc_args);
  // Enable PCIe power domain
  smc_args.Arg0 = 0xC2000000;
  smc_args.Arg1 = 0x03;
  smc_args.Arg2 = 0x01;
  smc_args.Arg3 = 0x01;
  smc_args.Arg4 = 0x00;
  ArmCallSmc(&smc_args);
  // Disable PCIE_CTRL clock root
  CCM_CCGR_PCIE = 0x00;
  // PCIE_CTRL_CLK_ROOT ClkSrc = SYSTEM_PLL2_DIV4  (250 MHz), PreDiv = 1, PostDiv = 1, Fout = 250 MHz
  CCM_TARGET_ROOT_PCIE_CTRL = CCM_TARGET_ROOT_MUX(1) | CCM_TARGET_ROOT_PRE_PODF(0) | CCM_TARGET_ROOT_POST_PODF(0) | CCM_TARGET_ROOT_ENABLE_MASK;
  // PCIE_PHY_CLK_ROOT  ClkSrc = SYSTEM_PLL2_DIV10 (100 MHz), PreDiv = 1, PostDiv = 1, Fout = 100 MHz
  CCM_TARGET_ROOT_PCIE_PHY  = CCM_TARGET_ROOT_MUX(1) | CCM_TARGET_ROOT_PRE_PODF(0) | CCM_TARGET_ROOT_POST_PODF(0) | CCM_TARGET_ROOT_ENABLE_MASK;
  // PCIE_AUX_CLK_ROOT  ClkSrc = SYSTEM_PLL2_DIV20 ( 50 MHz), PreDiv = 1, PostDiv = 5, Fout =  10 MHz
  CCM_TARGET_ROOT_PCIE_AUX  = CCM_TARGET_ROOT_MUX(2) | CCM_TARGET_ROOT_PRE_PODF(0) | CCM_TARGET_ROOT_POST_PODF(4) | CCM_TARGET_ROOT_ENABLE_MASK;
  // Enable PCIE_CTRL clock root
  CCM_CCGR_PCIE     = 0x03;  // Domain 0 clocks needed when in RUN and WAIT
  CCM_CCGR_SIM_HSIO = 0x03;  // Domain 0 clocks needed when in RUN and WAIT
  // Configure SD2_WP as GPIO to control PCIe nWAKE PAD
  IOMUXC_SW_MUX_CTL_PAD_SD2_WP = IOMUXC_MUX_ALT5;
  GPIO2_GDIR &= ~(0x01 << 20);                  // Set input direction
  // Configure SAI2_RXFS as GPIO to control PCIe nPERST PAD
  IOMUXC_SW_MUX_CTL_PAD_SAI2_RXFS = IOMUXC_MUX_ALT5;
  GPIO4_DR &= ~(0x01 << 21);                    // Set the pad to the low level
  GPIO4_GDIR |= (0x01 << 21);                   // Set output direction
  // Configure GPIO1_IO05 to control PCIe nDISABLE PAD
  IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO05 = IOMUXC_MUX_ALT5;
  GPIO1_DR |= (0x01 << 5);                      // Set the pad to the high level
  GPIO1_GDIR |= (0x01 << 5);                    // Set output direction
}
#endif

/**
  Initialize GPIO modules on the SOC and perform required pin-muxing
**/
VOID GpioInit ()
{
  CCM_CCGR_GPIO1 = 0x03;       // Enable GPIO1 clock root
  CCM_CCGR_GPIO2 = 0x03;       // Enable GPIO2 clock root
  CCM_CCGR_GPIO3 = 0x03;       // Enable GPIO3 clock root
  CCM_CCGR_GPIO4 = 0x03;       // Enable GPIO4 clock root
  CCM_CCGR_GPIO5 = 0x03;       // Enable GPIO5 clock root
}

/**
  Initalize the Audio system
**/
#define SAI_PAD_CFG_OUT (IOMUXC_PAD_PUE_ENABLE | IOMUXC_PAD_DSE_R0_DIV_3 | IOMUXC_PAD_SRE_FAST)
VOID AudioInit(VOID)
{
  INT32 count;
  UINT32 preDivSelect = 8;
  UINT32 postDivSelect = 4;
  // Mux the SAI3 pins to wm8524 codec
  IOMUXC_SW_MUX_CTL_PAD_SAI3_TXFS = IOMUXC_MUX_ALT0;
  IOMUXC_SW_MUX_CTL_PAD_SAI3_TXC  = IOMUXC_MUX_ALT0;
  IOMUXC_SW_MUX_CTL_PAD_SAI3_TXD  = IOMUXC_MUX_ALT0;
  IOMUXC_SW_MUX_CTL_PAD_SAI3_MCLK = IOMUXC_MUX_ALT0;

  IOMUXC_SW_PAD_CTL_PAD_SAI3_TXFS = SAI_PAD_CFG_OUT;
  IOMUXC_SW_PAD_CTL_PAD_SAI3_TXC  = SAI_PAD_CFG_OUT;
  IOMUXC_SW_PAD_CTL_PAD_SAI3_TXD  = SAI_PAD_CFG_OUT;
  IOMUXC_SW_PAD_CTL_PAD_SAI3_MCLK = SAI_PAD_CFG_OUT;
  // Unmute audio
  IOMUXC_SW_MUX_CTL_PAD_I2C4_SDA = IOMUXC_MUX_ALT5;
  GPIO5_DR   |= GPIO_DR_DR(1 << 21);
  GPIO5_GDIR |= GPIO_DR_DR(1 << 21);
  // Gate the SAI3 AUDIO device
  CCM_CCGR_SAI3 = 0x00;

  CCM_PLL_CTRL35 = 0x3333;
  CCM_ANALOG_AUDIO_PLL1_GEN_CTRL = CCM_ANALOG_AUDIO_PLL1_GEN_CTRL_PLL_BYPASS_MASK |
                                   CCM_ANALOG_AUDIO_PLL1_GEN_CTRL_PLL_CLKE_MASK;
  CCM_ANALOG_AUDIO_PLL1_FDIV_CTL0 = CCM_ANALOG_AUDIO_PLL1_FDIV_CTL0_PLL_MAIN_DIV(361) |
                                    CCM_ANALOG_AUDIO_PLL1_FDIV_CTL0_PLL_PRE_DIV(3) |
                                    CCM_ANALOG_AUDIO_PLL1_FDIV_CTL0_PLL_POST_DIV(3);
  CCM_ANALOG_AUDIO_PLL1_FDIV_CTL1 = CCM_ANALOG_AUDIO_PLL1_FDIV_CTL1_PLL_DSM(17511);
  MicroSecondDelay (4);
  CCM_ANALOG_AUDIO_PLL1_GEN_CTRL |=  CCM_ANALOG_AUDIO_PLL1_GEN_CTRL_PLL_RST_MASK;
  for (count = 0 ; count < 100; ++count) {
    if (CCM_ANALOG_AUDIO_PLL1_GEN_CTRL & CCM_ANALOG_AUDIO_PLL1_GEN_CTRL_PLL_LOCK_MASK) {
      break;
    }
    MicroSecondDelay (10);
  }
  if (!(CCM_ANALOG_AUDIO_PLL1_GEN_CTRL & CCM_ANALOG_AUDIO_PLL1_GEN_CTRL_PLL_LOCK_MASK)) {
    DebugPrint (0xFFFFFFFF, "Time out waiting for PLL to lock\n");
  }
  CCM_ANALOG_AUDIO_PLL1_GEN_CTRL &= ~(CCM_ANALOG_AUDIO_PLL1_GEN_CTRL_PLL_BYPASS_MASK);

  // Configure SAI3_CLK_ROOT to Audio PLL clock source and divide it down to 11.2896 MHz
  CCM_TARGET_ROOT(77) = CCM_TARGET_ROOT_MUX(0x01) | CCM_TARGET_ROOT_POST_PODF(postDivSelect - 1) |
                            CCM_TARGET_ROOT_PRE_PODF(preDivSelect - 1);

  // Set SAI3 MSEL2 , MSEL3 clock source to SAI3_CLK_ROOT
  IOMUXC_GPR_GPR7 = IOMUXC_GPR_GPR7_GPR_SAI3_SEL2(2) | IOMUXC_GPR_GPR7_GPR_SAI3_SEL3(2);

  CCM_TARGET_ROOT_SET(77) = CCM_TARGET_ROOT_ENABLE_MASK;
  CCM_TARGET_ROOT_SET(34) = CCM_TARGET_ROOT_ENABLE_MASK;
  CCM_CCGR_SAI3 = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
}

/**
  Initialize USB modules on the SOC and perform required pin-muxing
**/
VOID USBInit (VOID)
{
  CCM_CCGR_USB                 = 0x00;
  // USB_BUS_CLK_ROOT ClkSrc     = SYSTEM_PLL2_DIV2  (500 MHz), PreDiv = 1, PostDiv = 1, Fout = 500 MHz
  CCM_TARGET_ROOT_USB_BUS      = CCM_TARGET_ROOT_MUX(1) | CCM_TARGET_ROOT_PRE_PODF(0) | CCM_TARGET_ROOT_POST_PODF(0) | CCM_TARGET_ROOT_ENABLE_MASK;
  // USB_BUS_CLK_ROOT ClkSrc     = SYSTEM_PLL1_DIV8  (100 MHz), PreDiv = 1, PostDiv = 1, Fout = 100 MHz
  CCM_TARGET_ROOT_USB_CORE_REF = CCM_TARGET_ROOT_MUX(1) | CCM_TARGET_ROOT_PRE_PODF(0) | CCM_TARGET_ROOT_POST_PODF(0) | CCM_TARGET_ROOT_ENABLE_MASK;
  // USB_PHY_REF_CLK_ROOT ClkSrc = SYSTEM_PLL1_DIV8  (100 MHz), PreDiv = 1, PostDiv = 1, Fout = 100 MHz
  CCM_TARGET_ROOT_USB_PHY_REF  = CCM_TARGET_ROOT_MUX(1) | CCM_TARGET_ROOT_PRE_PODF(0) | CCM_TARGET_ROOT_POST_PODF(0) | CCM_TARGET_ROOT_ENABLE_MASK;
  CCM_CCGR_USB                 = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
}

/**
  Initialize ENETs modules on the SOC and perform required pin-muxing.
**/
#define ENET_MDC_PAD_CTRL     (IOMUXC_PAD_DSE_R0_DIV_3 | IOMUXC_PAD_SRE_SLOW)
#define ENET_MDIO_PAD_CTRL    (IOMUXC_PAD_DSE_R0_DIV_3 | IOMUXC_PAD_SRE_SLOW)
#define ENET_TX_PAD_CTRL      (IOMUXC_PAD_DSE_R0_DIV_7 | IOMUXC_PAD_SRE_MAX)
#define ENET_RX_PAD_CTRL      (IOMUXC_PAD_DSE_R0_DIV_1 | IOMUXC_PAD_SRE_FAST | IOMUXC_PAD_HYS_ENABLED)
VOID EnetInit(VOID)
{
  // ENET1/2 MDIO bus (both ENETs share one MDIO bus conected to the ENET1 controller)
  IOMUXC_SW_MUX_CTL_PAD_ENET_MDC    = IOMUXC_MUX_ALT0;    // ENET1_MDC  -> PAD_GPIO1_IO11
  IOMUXC_SW_MUX_CTL_PAD_ENET_MDIO   = IOMUXC_MUX_ALT0;    // ENET1_MDIO -> PAD_PAD_ENET_MDIO
  IOMUXC_ENET1_MDIO_SELECT_INPUT    = IOMUXC_MUX_ALT1;    // ENET1_MDIO <- PAD_PAD_ENET_MDIO
  IOMUXC_SW_PAD_CTL_PAD_ENET_MDC    = ENET_MDC_PAD_CTRL;  // ENET1_MDC  electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_MDIO   = ENET_MDIO_PAD_CTRL; // ENET1_MDIO electrical settings
  // ENET1 RGMMI pins routing
  IOMUXC_SW_MUX_CTL_PAD_ENET_TD3    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TD3    -> PAD_ENET1_TD3
  IOMUXC_SW_MUX_CTL_PAD_ENET_TD2    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TD2    -> PAD_ENET1_TD2
  IOMUXC_SW_MUX_CTL_PAD_ENET_TD1    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TD1    -> PAD_ENET1_TD1
  IOMUXC_SW_MUX_CTL_PAD_ENET_TD0    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TD0    -> PAD_ENET1_TD0
  IOMUXC_SW_MUX_CTL_PAD_ENET_TX_CTL = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TX_CTL -> PAD_ENET1_TX_CTL
  IOMUXC_SW_MUX_CTL_PAD_ENET_TXC    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TXC    -> PAD_ENET1_TXC
  IOMUXC_SW_MUX_CTL_PAD_ENET_RD3    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RD3    <- PAD_ENET1_RD3
  IOMUXC_SW_MUX_CTL_PAD_ENET_RD2    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RD2    <- PAD_ENET1_RD2
  IOMUXC_SW_MUX_CTL_PAD_ENET_RD1    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RD1    <- PAD_ENET1_RD1
  IOMUXC_SW_MUX_CTL_PAD_ENET_RD0    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RD0    <- PAD_ENET1_RD0
  IOMUXC_SW_MUX_CTL_PAD_ENET_RX_CTL = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RX_CTL <- PAD_ENET1_RX_CTL
  IOMUXC_SW_MUX_CTL_PAD_ENET_RXC    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RXC    <- PAD_ENET1_RXC
  // ENET1 RGMMI pins electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_RD3    = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RD3    electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_RD2    = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RD2    electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_RD1    = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RD1    electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_RD0    = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RD0    electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_RX_CTL = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RX_CTL electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_RXC    = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RXC    electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_TD3    = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TD3    electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_TD2    = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TD2    electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_TD1    = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TD1    electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_TD0    = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TD0    electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_TX_CTL = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TX_CTL electrical settings
  IOMUXC_SW_PAD_CTL_PAD_ENET_TXC    = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TXC    electrical settings
  // Configure ENET_nRST signal
  IOMUXC_SW_MUX_CTL_PAD_SAI2_RXC    = IOMUXC_MUX_ALT5;   // ENET_nRST -> PAD_SAI2_RXC[ALT5]=GPIO4_22
  IOMUXC_SW_PAD_CTL_PAD_SAI2_RXC    = IOMUXC_PAD_DSE_R0_DIV_1 | IOMUXC_PAD_SRE_SLOW;
  GPIO4_DR &= ~(1 << 22);                                // Set ENET_nRST = 0
  GPIO4_GDIR |= (1 << 22);                               // Set direction to output
  MicroSecondDelay(500);
  GPIO4_DR |= (1 << 22);                                 // Set ENET_nRST = 1
  // Disable ENET1 clock(s)
  CCM_CCGR_ENET1    = 0x00;
  CCM_CCGR_SIM_ENET = 0x00;
  // ENET_AXI_CLK_ROOT ClkSrc = SYSTEM_PLL1_DIV3  (266 MHz), PreDiv = 1, PostDiv = 1, Fout = 266 MHz
  CCM_TARGET_ROOT_ENET_AXI = CCM_TARGET_ROOT_MUX(1) | CCM_TARGET_ROOT_PRE_PODF(0) | CCM_TARGET_ROOT_POST_PODF(0) | CCM_TARGET_ROOT_ENABLE_MASK;
  // ENET_REF ClkSrc: SYSTEM_PLL2_DIV8 (125 MHz), fout = 125 MHz
  CCM_TARGET_ROOT_ENET_REF = CCM_TARGET_ROOT_MUX(1) | CCM_TARGET_ROOT_PRE_PODF(0) | CCM_TARGET_ROOT_POST_PODF(0) | CCM_TARGET_ROOT_ENABLE_MASK;
  // Enable ENET1 clock(s)
  CCM_CCGR_ENET1    = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
  CCM_CCGR_SIM_ENET = 0x02;  // Domain 0 clocks needed when in RUN and WAIT
}

/**
  Initialize PWM block and perform required pin-muxing.
**/
VOID PwmInit()
{
  int pwm_ccgr_offset = 40;
  int pwm_clk_root_offset = 103;
  // Initialize PWM1-PWM4 clocks
  for(int i = 0; i < 4; i++) {
    CCM_CCGR_CLR(pwm_ccgr_offset + i) = 0x03;
    CCM_TARGET_ROOT(pwm_clk_root_offset + i) = (CCM_TARGET_ROOT_MUX(0x00) | CCM_TARGET_ROOT_ENABLE_MASK);   // Set 25M_REF_CLK as PWM input clock, no PRE nor POST divider, Enable PWM clock
    CCM_CCGR_SET(pwm_ccgr_offset + i) = 0x03;
  }
  // Configure GPIO1_IO01 as PWM1_OUT
  IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO01 = IOMUXC_MUX_ALT1;
  // Drive strength 85 Ohm @3.3V, 80 Ohm @2.5V, 75 Ohm @1.8V, 90 Ohm @1.2V, Slow Frequency Slew Rate (50Mhz)
  IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO01 = (IOMUXC_SW_PAD_CTL_PAD_DSE(4) | IOMUXC_SW_PAD_CTL_PAD_FSEL(0));
}

/**
  Initialize controllers that must setup at the early stage
**/
RETURN_STATUS ArmPlatformInitialize(IN UINTN MpId)
{
  if (!ArmPlatformIsPrimaryCore (MpId)) {
    return RETURN_SUCCESS;
  }
  // Initialize debug serial port
  SerialPortInitialize ();
  SerialPortWrite ((UINT8 *)SERIAL_DEBUG_PORT_INIT_MSG, (UINTN)sizeof(SERIAL_DEBUG_PORT_INIT_MSG));
  UngateClocks();
  GpioInit();
#if FixedPcdGet32(PcdPcie1Enable)
  PcieInit();
#endif
  USBInit();
  EnetInit();
  AudioInit();
  PwmInit();
  return RETURN_SUCCESS;
}

/**
  Return the current Boot Mode. This function returns the boot reason on the platform
**/
EFI_BOOT_MODE ArmPlatformGetBootMode (VOID)
{
  return BOOT_WITH_FULL_CONFIGURATION;
}
