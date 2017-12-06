/*	$NetBSD: imx6_ccmreg.h,v 1.6 2017/11/09 05:57:23 hkenken Exp $	*/

/*
 * Copyright (c) 2014 Ryo Shimizu <ryo@nerv.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ARM_IMX_IMX6_CCMREG_H
#define _ARM_IMX_IMX6_CCMREG_H

#include <sys/cdefs.h>

/*
 * PERIPHCLK_N is an arm root clock divider for MPcore interupt controller.
 * PERIPHCLK_N is equal to, or greater than two.
 * see "Cortex-A9 MPCore Technical Reference Manual" -
 *     Chapter 5: Clocks, Resets, and Power Management, 5.1: Clocks.
 */
#ifndef IMX6_PERIPHCLK_N
#define IMX6_PERIPHCLK_N	2
#endif

#ifndef IMX6_OSC_FREQ
#define IMX6_OSC_FREQ	(24 * 1000 * 1000)	/* 24MHz */
#endif

#define CCM_CCR					0x00000000
#define  CCM_CCR_RBC_EN				__BIT(27)
#define  CCM_CCR_REG_BYPASS_COUNT		__BITS(26, 21)
#define  CCM_CCR_WB_COUNT			__BITS(18, 16)
#define  CCM_CCR_COSC_EN			__BIT(12)
#define  CCM_CCR_OSCNT				__BITS(7, 0)

#define CCM_CCDR				0x00000004
#define CCM_CSR					0x00000008
#define CCM_CCSR				0x0000000c
#define  CCM_CCSR_PLL3_PFD1_DIS_MASK		__BIT(15)
#define  CCM_CCSR_PLL3_PFD0_DIS_MASK		__BIT(14)
#define  CCM_CCSR_PLL3_PFD3_DIS_MASK		__BIT(13)
#define  CCM_CCSR_PLL3_PFD2_DIS_MASK		__BIT(12)
#define  CCM_CCSR_PLL2_PFD1_594M_DIS_MASK	__BIT(11)
#define  CCM_CCSR_PLL2_PFD0_DIS_MASK		__BIT(10)
#define  CCM_CCSR_PLL2_PFD2_DIS_MASK		__BIT(9)
#define  CCM_CCSR_STEP_SEL			__BIT(8)
#define  CCM_CCSR_PLL1_SW_CLK_SEL		__BIT(2)
#define  CCM_CCSR_PLL3_SW_CLK_SEL		__BIT(0)
#define CCM_CACRR				0x00000010
#define  CCM_CACRR_ARM_PODF			__BITS(2, 0)

#define CCM_CBCDR				0x00000014
#define  CCM_CBCDR_PERIPH_CLK2_PODF		__BITS(29, 27)
/* source of mmdc_ch1_axi_clk_root */
#define  CCM_CBCDR_PERIPH2_CLK_SEL		__BIT(26)
/* source of mmdc_ch0_axi_clk_root */
#define  CCM_CBCDR_PERIPH_CLK_SEL		__BIT(25)
#define  CCM_CBCDR_MMDC_CH0_AXI_PODF		__BITS(21, 19)
#define  CCM_CBCDR_AXI_PODF			__BITS(18, 16)
#define  CCM_CBCDR_AHB_PODF			__BITS(12, 10)
#define  CCM_CBCDR_IPG_PODF			__BITS(9, 8)
#define  CCM_CBCDR_AXI_ALT_SEL			__BIT(7)
#define  CCM_CBCDR_AXI_SEL			__BITS(7, 6)
#define  CCM_CBCDR_MMDC_CH1_AXI_PODF		__BITS(5, 3)
#define  CCM_CBCDR_PERIPH2_CLK2_PODF		__BITS(2, 0)

#define CCM_CBCMR				0x00000018
#define  CCM_CBCMR_GPU3D_SHADER_PODF		__BITS(31, 29)
#define  CCM_CBCMR_GPU3D_CORE_PODF		__BITS(28, 26)
#define  CCM_CBCMR_GPU2D_CORE_CLK_PODF		__BITS(25, 23)
#define  CCM_CBCMR_PRE_PERIPH2_CLK_SEL		__BITS(22, 21)
#define  CCM_CBCMR_PERIPH2_CLK2_SEL		__BIT(20)
#define  CCM_CBCMR_PRE_PERIPH_CLK_SEL		__BITS(19, 18)
#define  CCM_CBCMR_GPU2D_CLK_SEL		__BITS(17, 16)
#define  CCM_CBCMR_VPU_AXI_CLK_SEL		__BITS(15, 14)
#define  CCM_CBCMR_PERIPH_CLK2_SEL		__BITS(13, 12)
#define  CCM_CBCMR_VDOAXI_CLK_SEL		__BIT(11)
#define  CCM_CBCMR_PCIE_AXI_CLK_SEL		__BIT(10)
#define  CCM_CBCMR_GPU3D_SHADER_CLK_SEL		__BITS(9, 8)
#define  CCM_CBCMR_GPU3D_CORE_CLK_SEL		__BITS(5, 4)
#define  CCM_CBCMR_GPU3D_AXI_CLK_SEL		__BIT(1)
#define  CCM_CBCMR_GPU2D_AXI_CLK_SEL		__BIT(0)

#define CCM_CSCMR1				0x0000001c
#define  CCM_CSCMR1_ACLK_EIM_SLOW_SEL		__BITS(30, 29)
#define  CCM_CSCMR1_ACLK_SEL			__BITS(28, 27)
#define  CCM_CSCMR1_ACLK_EIM_SLOW_PODF		__BITS(25, 23)
#define  CCM_CSCMR1_ACLK_PODF			__BITS(22, 20)
#define  CCM_CSCMR1_USDHC4_CLK_SEL		__BIT(19)
#define  CCM_CSCMR1_USDHC3_CLK_SEL		__BIT(18)
#define  CCM_CSCMR1_USDHC2_CLK_SEL		__BIT(17)
#define  CCM_CSCMR1_USDHC1_CLK_SEL		__BIT(16)
#define  CCM_CSCMR1_SSI3_CLK_SEL		__BITS(15, 14)
#define  CCM_CSCMR1_SSI2_CLK_SEL		__BITS(13, 12)
#define  CCM_CSCMR1_SSI1_CLK_SEL		__BITS(11, 10)
#define  CCM_CSCMR1_PERCLK_PODF			__BITS(5, 0)

#define CCM_CSCMR2				0x00000020
#define  CCM_CSCMR2_ESAI_CLK_SEL		__BITS(20, 19)
#define  CCM_CSCMR2_LDB_DI1_IPU_DIV		__BIT(11)
#define  CCM_CSCMR2_LDB_DI0_IPU_DIV		__BIT(10)
#define  CCM_CSCMR2_CAN_CLK_PODF		__BITS(7, 2)

#define CCM_CSCDR1				0x00000024
#define  CCM_CSCDR1_VPU_AXI_PODF		__BITS(27, 25)
#define  CCM_CSCDR1_USDHC4_PODF			__BITS(24, 22)
#define  CCM_CSCDR1_USDHC3_PODF			__BITS(21, 19)
#define  CCM_CSCDR1_USDHC2_PODF			__BITS(18, 16)
#define  CCM_CSCDR1_USDHC1_PODF			__BITS(13, 11)
#define  CCM_CSCDR1_UART_CLK_PODF		__BITS(5, 0)

#define CCM_CS1CDR				0x00000028
#define  CCM_CS1CDR_ESAI_CLK_PODF		__BITS(27, 25)
#define  CCM_CS1CDR_SSI3_CLK_PRED		__BITS(24, 22)
#define  CCM_CS1CDR_SSI3_CLK_PODF		__BITS(21, 16)
#define  CCM_CS1CDR_ESAI_CLK_PRED		__BITS(11, 9)
#define  CCM_CS1CDR_SSI1_CLK_PRED		__BITS(8, 6)
#define  CCM_CS1CDR_SSI1_CLK_PODF		__BITS(5, 0)

#define CCM_CS2CDR				0x0000002c
#define  CCM_CS2CDR_ENFC_CLK_PODF		__BITS(26, 21)
#define  CCM_CS2CDR_ENFC_CLK_PRED		__BITS(20, 18)
#define  CCM_CS2CDR_ENFC_CLK_SEL		__BITS(17, 16)
#define  CCM_CS2CDR_LDB_DI1_CLK_SEL		__BITS(14, 12)
#define  CCM_CS2CDR_LDB_DI0_CLK_SEL		__BITS(11, 9)
#define  CCM_CS2CDR_SSI2_CLK_PRED		__BITS(8, 6)
#define  CCM_CS2CDR_SSI2_CLK_PODF		__BITS(5, 0)

#define CCM_CDCDR				0x00000030
#define  CCM_CDCDR_HSI_TX_PODF			__BITS(31, 29)
#define  CCM_CDCDR_HSI_TX_CLK_SEL		__BIT(28)
#define  CCM_CDCDR_SPDIF0_CLK_PRED		__BITS(27, 25)
#define  CCM_CDCDR_SPDIF0_CLK_PODF		__BITS(24, 22)
#define  CCM_CDCDR_SPDIF0_CLK_SEL		__BITS(21, 20)
#define  CCM_CDCDR_SPDIF1_CLK_PRED		__BITS(14, 12)
#define  CCM_CDCDR_SPDIF1_CLK_PODF		__BITS(11, 9)
#define  CCM_CDCDR_SPDIF1_CLK_SEL		__BITS(8, 7)

#define CCM_CHSCCDR				0x00000034
#define  CCM_CHSCCDR_IPU1_DI1_PRE_CLK_SEL	__BITS(17, 15)
#define  CCM_CHSCCDR_IPU1_DI1_PODF		__BITS(14, 12)
#define  CCM_CHSCCDR_IPU1_DI1_CLK_SEL		__BITS(11, 9)
#define  CCM_CHSCCDR_IPU1_DI0_PRE_CLK_SEL	__BITS(8, 6)
#define  CCM_CHSCCDR_IPU1_DI0_PODF		__BITS(5, 3)
#define  CCM_CHSCCDR_IPU1_DI0_CLK_SEL		__BITS(2, 0)


#define CCM_CSCDR2				0x00000038
#define  CCM_CSCDR2_ECSPI_CLK_PODF		__BITS(24, 19)
#define  CCM_CSCDR2_IPU2_DI1_PRE_CLK_SEL	__BITS(17, 15)
#define  CCM_CSCDR2_IPU2_DI1_PODF		__BITS(14, 12)
#define  CCM_CSCDR2_IPU2_DI1_CLK_SEL		__BITS(11, 9)
#define  CCM_CSCDR2_IPU2_DI0_PRE_CLK_SEL	__BITS(8, 6)
#define  CCM_CSCDR2_IPU2_DI0_PODF		__BITS(5, 3)
#define  CCM_CSCDR2_IPU2_DI0_CLK_SEL		__BITS(2, 0)

#define CCM_CDHIPR				0x00000048
#define  CCM_CDHIPR_ARM_PODF_BUSY		__BIT(16)
#define  CCM_CDHIPR_PERIPH_CLK_SEL_BUSY		__BIT(5)
#define  CCM_CDHIPR_MMDC_CH0_PODF_BUSY		__BIT(4)
#define  CCM_CDHIPR_PERIPH2_CLK_SEL_BUSY	__BIT(3)
#define  CCM_CDHIPR_MMDC_CH1_PODF_BUSY		__BIT(2)
#define  CCM_CDHIPR_AHB_PODF_BUSY		__BIT(1)
#define  CCM_CDHIPR_AXI_PODF_BUSY		__BIT(0)

#define CCM_CSCDR3				0x0000003c
#define  CCM_CSCDR3_IPU2_HSP_PODF		__BITS(18, 16)
#define  CCM_CSCDR3_IPU2_HSP_CLK_SEL		__BITS(15, 14)
#define  CCM_CSCDR3_IPU1_HSP_PODF		__BITS(13, 11)
#define  CCM_CSCDR3_IPU1_HSP_CLK_SEL		__BITS(10, 9)

#define CCM_CCOSR				0x00000060
#define  CCM_CCOSR_CLKO2_EN	__BIT(24)
#define  CCM_CCOSR_CLKO2_DIV	__BITS(23, 21)
#define  CCM_CCOSR_CLKO2_SEL	__BITS(20, 16)
#define  CCM_CCOSR_CLK_OUT_SEL	__BIT(8)
#define  CCM_CCOSR_CLKO1_EN	__BIT(7)
#define  CCM_CCOSR_CLKO1_DIV	__BITS(6, 4)
#define  CCM_CCOSR_CLKO1_SEL	__BITS(3, 0)

#define CCM_CCGR2						0x00000070
#define  CCM_CCGR2_IPSYNC_VDOA_IPG_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(27, 26))
#define  CCM_CCGR2_IPSYNC_IP2APB_TZASC2_IPG_CLK_ENABLE(n)       __SHIFTIN(n, __BITS(25, 24))
#define  CCM_CCGR2_IPSYNC_IP2APB_TZASC1_IPG_CLK_ENABLE(n)       __SHIFTIN(n, __BITS(23, 22))
#define  CCM_CCGR2_IPMUX3_CLK_ENABLE(n)				__SHIFTIN(n, __BITS(21, 20))
#define  CCM_CCGR2_IPMUX2_CLK_ENABLE(n)				__SHIFTIN(n, __BITS(19, 18))
#define  CCM_CCGR2_IPMUX1_CLK_ENABLE(n)				__SHIFTIN(n, __BITS(17, 16))
#define  CCM_CCGR2_IOMUX_IPT_CLK_IO_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(15, 14))
#define  CCM_CCGR2_IIM_CLK_ENABLE(n)				__SHIFTIN(n, __BITS(13, 12))
#define  CCM_CCGR2_I2C3_SERIAL_CLK_ENABLE(n)			__SHIFTIN(n, __BITS(11, 10))
#define  CCM_CCGR2_I2C2_SERIAL_CLK_ENABLE(n)			__SHIFTIN(n, __BITS(9, 8))
#define  CCM_CCGR2_I2C1_SERIAL_CLK_ENABLE(n)			__SHIFTIN(n, __BITS(7, 6))
#define  CCM_CCGR2_HDMI_TX_ISFRCLK_ENABLE(n)			__SHIFTIN(n, __BITS(5, 4))
#define  CCM_CCGR2_HDMI_TX_IAHBCLK_ENABLE(n)			__SHIFTIN(n, __BITS(1, 0))
#define CCM_CCGR4	0x00000078
#define  CCM_CCGR4_RAWNAND_U_GPMI_INPUT_APB_CLK_ENABLE(N)		__SHIFTIN(n, __BITS(31, 30))
#define  CCM_CCGR4_RAWNAND_U_GPMI_BCH_INPUT_GPMI_IO_CLK_ENABLE(n) 	__SHIFTIN(n, __BITS(29, 28))
#define  CCM_CCGR4_RAWNAND_U_GPMI_BCH_INPUT_BCH_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(27, 26))
#define  CCM_CCGR4_RAWNAND_U_BCH_INPUT_APB_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(25, 24))
#define  CCM_CCGR4_PWM4_CLK_ENABLE(n)					__SHIFTIN(n, __BITS(23, 22))
#define  CCM_CCGR4_PWM3_CLK_ENABLE(n)					__SHIFTIN(n, __BITS(21, 20))
#define  CCM_CCGR4_PWM2_CLK_ENABLE(n)					__SHIFTIN(n, __BITS(19, 18))
#define  CCM_CCGR4_PWM1_CLK_ENABLE(n)					__SHIFTIN(n, __BITS(17, 16))
#define  CCM_CCGR4_PL301_MX6QPER2_MAINCLK_ENABLE(n)			__SHIFTIN(n, __BITS(15, 14))
#define  CCM_CCGR4_PL301_MX6QPER1_BCHCLK_ENABLE(n)			__SHIFTIN(n, __BITS(13, 12))
#define  CCM_CCGR4_CG5_ENABLE(n)					__SHIFTIN(n, __BITS(11, 10))
#define  CCM_CCGR4_PL301_MX6QFAST1_S133CLK_ENABLE(n)			__SHIFTIN(n, __BITS(9, 8))
#define  CCM_CCGR4_CG3_ENABLE(n)					__SHIFTIN(n, __BITS(7, 6))
#define  CCM_CCGR4_CG2_ENABLE(n)					__SHIFTIN(n, __BITS(5, 4))
#define  CCM_CCGR4_CG1_ENABLE(n)					__SHIFTIN(n, __BITS(3, 2))
#define  CCM_CCGR4_125M_ROOT_ENABLE(n)					__SHIFTIN(n, __BITS(1, 0))
#define CCM_CCGR5				0x0000007c
#define  CCM_CCGR5_UART_SERIAL_CLK_ENABLE(n)	__SHIFTIN(n, __BITS(27, 26))
#define  CCM_CCGR5_UART_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(25, 24))
#define  CCM_CCGR5_SSI3_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(23, 22))
#define  CCM_CCGR5_SSI2_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(21, 20))
#define  CCM_CCGR5_SSI1_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(19, 18))
#define  CCM_CCGR5_SPDIF_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(15, 14))
#define  CCM_CCGR5_SPBA_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(13, 12))
#define  CCM_CCGR5_SDMA_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(7, 6))
#define  CCM_CCGR5_100M_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(5, 4))
#define  CCM_CCGR5_ROM_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(1, 0))
#define CCM_CCGR6				0x00000080
#define  CCM_CCGR6_VPU_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(15, 14))
#define  CCM_CCGR6_VDOAXICLK_CLK_ENABLE(n)	__SHIFTIN(n, __BITS(13, 12))
#define  CCM_CCGR6_EIM_SLOW_CLK_ENABLE(n)	__SHIFTIN(n, __BITS(11, 10))
#define  CCM_CCGR6_USDHC4_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(9, 8))
#define  CCM_CCGR6_USDHC3_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(7, 6))
#define  CCM_CCGR6_USDHC2_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(5, 4))
#define  CCM_CCGR6_USDHC1_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(3, 2))
#define  CCM_CCGR6_USBOH3_CLK_ENABLE(n)		__SHIFTIN(n, __BITS(1, 0))

#define CCM_ANALOG_PLL_ARM			0x00000000	/* = 020c8000 */
#define  CCM_ANALOG_PLL_ARM_LOCK		__BIT(31)
#define  CCM_ANALOG_PLL_ARM_PLL_SEL		__BIT(19)
#define  CCM_ANALOG_PLL_ARM_LVDS_24MHZ_SEL	__BIT(18)
#define  CCM_ANALOG_PLL_ARM_LVDS_SEL		__BIT(17)
#define  CCM_ANALOG_PLL_ARM_BYPASS		__BIT(16)
#define  CCM_ANALOG_PLL_ARM_BYPASS_CLK_SRC	__BITS(15, 14)
#define  CCM_ANALOG_PLL_ARM_ENABLE		__BIT(13)
#define  CCM_ANALOG_PLL_ARM_POWERDOWN		__BIT(12)
#define  CCM_ANALOG_PLL_ARM_DIV_SELECT		__BITS(6, 0)

#define CCM_ANALOG_PLL_ARM_SET			0x00000004
#define CCM_ANALOG_PLL_ARM_CLR			0x00000008
#define CCM_ANALOG_PLL_ARM_TOG			0x0000000c
#define  CCM_ANALOG_PLL_ARM_DIV_SELECT		__BITS(6, 0)

#define CCM_ANALOG_PLL_USB1			0x00000010
#define CCM_ANALOG_PLL_USB1_SET			0x00000014
#define CCM_ANALOG_PLL_USB1_CLR			0x00000018
#define CCM_ANALOG_PLL_USB1_TOG			0x0000001c
#define  CCM_ANALOG_PLL_USB1_LOCK		__BIT(31)
#define  CCM_ANALOG_PLL_USB1_BYPASS		__BIT(16)
#define  CCM_ANALOG_PLL_USB1_BYPASS_CLK_SRC	__BITS(15, 14)
#define  CCM_ANALOG_PLL_USB1_ENABLE		__BIT(13)
#define  CCM_ANALOG_PLL_USB1_POWER		__BIT(12)
#define  CCM_ANALOG_PLL_USB1_EN_USB_CLK		__BIT(6)
#define  CCM_ANALOG_PLL_USB1_DIV_SELECT		__BITS(1, 0)

#define CCM_ANALOG_PLL_USB2			0x00000020
#define CCM_ANALOG_PLL_USB2_SET			0x00000024
#define CCM_ANALOG_PLL_USB2_CLR			0x00000028
#define CCM_ANALOG_PLL_USB2_TOG			0x0000002c
#define  CCM_ANALOG_PLL_USB2_LOCK		__BIT(31)
#define  CCM_ANALOG_PLL_USB2_BYPASS		__BIT(16)
#define  CCM_ANALOG_PLL_USB2_BYPASS_CLK_SRC	__BITS(15, 14)
#define  CCM_ANALOG_PLL_USB2_ENABLE		__BIT(13)
#define  CCM_ANALOG_PLL_USB2_POWER		__BIT(12)
#define  CCM_ANALOG_PLL_USB2_EN_USB_CLK		__BIT(6)
#define  CCM_ANALOG_PLL_USB2_DIV_SELECT		__BITS(1, 0)

#define  CCM_ANALOG_PLL_USBn_LOCK		__BIT(31)
#define  CCM_ANALOG_PLL_USBn_BYPASS		__BIT(16)
#define  CCM_ANALOG_PLL_USBn_BYPASS_CLK_SRC	__BITS(15, 14)
#define  CCM_ANALOG_PLL_USBn_ENABLE		__BIT(13)
#define  CCM_ANALOG_PLL_USBn_POWER		__BIT(12)
#define  CCM_ANALOG_PLL_USBn_EN_USB_CLK		__BIT(6)
#define  CCM_ANALOG_PLL_USBn_DIV_SELECT		__BITS(1, 0)

#define CCM_ANALOG_PLL_SYS			0x00000030
#define CCM_ANALOG_PLL_SYS_SET			0x00000034
#define CCM_ANALOG_PLL_SYS_CLR			0x00000038
#define CCM_ANALOG_PLL_SYS_TOG			0x0000003c
#define  CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC	__BITS(15, 14)
#define  CCM_ANALOG_PLL_SYS_ENABLE		__BIT(13)
#define  CCM_ANALOG_PLL_SYS_DIV_SELECT		__BIT(0)
#define CCM_ANALOG_PLL_SYS_SS			0x00000040
#define CCM_ANALOG_PLL_SYS_NUM			0x00000050
#define CCM_ANALOG_PLL_SYS_DENOM		0x00000060
#define CCM_ANALOG_PLL_AUDIO			0x00000070
#define CCM_ANALOG_PLL_AUDIO_SET		0x00000074
#define CCM_ANALOG_PLL_AUDIO_CLR		0x00000078
#define CCM_ANALOG_PLL_AUDIO_TOG		0x0000007c
#define  CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT	__BITS(20, 19)
#define  CCM_ANALOG_PLL_AUDIO_BYPASS_CLK_SRC	__BITS(15, 14)
#define  CCM_ANALOG_PLL_AUDIO_ENABLE		__BIT(13)
#define  CCM_ANALOG_PLL_AUDIO_DIV_SELECT	__BITS(6, 0)
#define CCM_ANALOG_PLL_AUDIO_NUM		0x00000080
#define CCM_ANALOG_PLL_AUDIO_DENOM		0x00000090
#define CCM_ANALOG_PLL_VIDEO			0x000000a0
#define  CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT	__BITS(20, 19)
#define  CCM_ANALOG_PLL_VIDEO_BYPASS_CLK_SRC	__BITS(15, 14)
#define  CCM_ANALOG_PLL_VIDEO_ENABLE		__BIT(13)
#define  CCM_ANALOG_PLL_VIDEO_DIV_SELECT	__BITS(6, 0)
#define CCM_ANALOG_PLL_VIDEO_SET		0x000000a4
#define CCM_ANALOG_PLL_VIDEO_CLR		0x000000a8
#define CCM_ANALOG_PLL_VIDEO_TOG		0x000000ac
#define CCM_ANALOG_PLL_VIDEO_NUM		0x000000b0
#define CCM_ANALOG_PLL_VIDEO_DENOM		0x000000c0
#define CCM_ANALOG_PLL_MLB			0x000000d0
#define CCM_ANALOG_PLL_MLB_SET			0x000000d4
#define CCM_ANALOG_PLL_MLB_CLR			0x000000d8
#define CCM_ANALOG_PLL_MLB_TOG			0x000000dc
#define CCM_ANALOG_PLL_ENET			0x000000e0
#define CCM_ANALOG_PLL_ENET_SET			0x000000e4
#define CCM_ANALOG_PLL_ENET_CLR			0x000000e8
#define CCM_ANALOG_PLL_ENET_TOG			0x000000ec
#define  CCM_ANALOG_PLL_ENET_LOCK		__BIT(31)
#define  CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN	__BIT(21)	/* iMX6UL */
#define  CCM_ANALOG_PLL_ENET_ENET2_125M_EN	__BIT(20)	/* iMX6UL */
#define  CCM_ANALOG_PLL_ENET_ENABLE_100M	__BIT(20)	/* SATA */
#define  CCM_ANALOG_PLL_ENET_ENABLE_125M	__BIT(19)	/* PCIe */
#define  CCM_ANALOG_PLL_ENET_PFD_OFFSET_EN	__BIT(18)
#define  CCM_ANALOG_PLL_ENET_BYPASS		__BIT(16)
#define  CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC	__BITS(15, 14)
#define  CCM_ANALOG_PLL_ENET_ENET1_125M_EN	__BIT(13)	/* iMX6UL */
#define  CCM_ANALOG_PLL_ENET_ENABLE		__BIT(13)	/* Ether */
#define  CCM_ANALOG_PLL_ENET_POWERDOWN		__BIT(12)
#define  CCM_ANALOG_PLL_ENET1_DIV_SELECT(d)	__SHIFTIN(d, __BITS(2, 1))
#define  CCM_ANALOG_PLL_ENET1_DIV_SELECT_MASK	__BITS(3, 2)
#define  CCM_ANALOG_PLL_ENET_DIV_SELECT(d)	__SHIFTIN(d, __BITS(1, 0))
#define  CCM_ANALOG_PLL_ENET_DIV_SELECT_MASK	__BITS(1, 0)
#define CCM_ANALOG_PFD_480			0x000000f0
#define CCM_ANALOG_PFD_480_SET			0x000000f4
#define CCM_ANALOG_PFD_480_CLR			0x000000f8
#define CCM_ANALOG_PFD_480_TOG			0x000000fc
#define  CCM_ANALOG_PFD_480_PFD3_CLKGATE	__BIT(31)
#define  CCM_ANALOG_PFD_480_PFD3_STABLE		__BIT(30)
#define  CCM_ANALOG_PFD_480_PFD3_FRAC		__BITS(29, 24)
#define  CCM_ANALOG_PFD_480_PFD2_CLKGATE	__BIT(23)
#define  CCM_ANALOG_PFD_480_PFD2_STABLE		__BIT(22)
#define  CCM_ANALOG_PFD_480_PFD2_FRAC		__BITS(21, 16)
#define  CCM_ANALOG_PFD_480_PFD1_CLKGATE	__BIT(15)
#define  CCM_ANALOG_PFD_480_PFD1_STABLE		__BIT(14)
#define  CCM_ANALOG_PFD_480_PFD1_FRAC		__BITS(13, 8)
#define  CCM_ANALOG_PFD_480_PFD0_CLKGATE	__BIT(7)
#define  CCM_ANALOG_PFD_480_PFD0_STABLE		__BIT(6)
#define  CCM_ANALOG_PFD_480_PFD0_FRAC		__BITS(5, 0)
#define CCM_ANALOG_PFD_528			0x00000100
#define CCM_ANALOG_PFD_528_SET			0x00000104
#define CCM_ANALOG_PFD_528_CLR			0x00000108
#define CCM_ANALOG_PFD_528_TOG			0x0000010c
#define  CCM_ANALOG_PFD_528_PFD2_CLKGATE	__BIT(23)
#define  CCM_ANALOG_PFD_528_PFD2_STABLE		__BIT(22)
#define  CCM_ANALOG_PFD_528_PFD2_FRAC		__BITS(21, 16)
#define  CCM_ANALOG_PFD_528_PFD1_CLKGATE	__BIT(15)
#define  CCM_ANALOG_PFD_528_PFD1_STABLE		__BIT(14)
#define  CCM_ANALOG_PFD_528_PFD1_FRAC		__BITS(13, 8)
#define  CCM_ANALOG_PFD_528_PFD0_CLKGATE	__BIT(7)
#define  CCM_ANALOG_PFD_528_PFD0_STABLE		__BIT(6)
#define  CCM_ANALOG_PFD_528_PFD0_FRAC		__BITS(5, 0)
#define CCM_ANALOG_MISC0			0x00000150
#define CCM_ANALOG_MISC0_SET			0x00000154
#define CCM_ANALOG_MISC0_CLR			0x00000158
#define CCM_ANALOG_MISC0_TOG			0x0000015c
#define CCM_ANALOG_MISC1			0x00000160
#define CCM_ANALOG_MISC1_SET			0x00000164
#define CCM_ANALOG_MISC1_CLR			0x00000168
#define CCM_ANALOG_MISC1_TOG			0x0000016c
#define  CCM_ANALOG_MISC1_LVDS_CLK1_SRC		__BITS(4, 0)
#define  CCM_ANALOG_MISC1_LVDS_CLK1_SRC_PCIE	__SHIFTIN(0xa, CCM_ANALOG_MISC1_LVDS_CLK1_SRC)
#define  CCM_ANALOG_MISC1_LVDS_CLK1_SRC_SATA	__SHIFTIN(0xb, CCM_ANALOG_MISC1_LVDS_CLK1_SRC)
#define  CCM_ANALOG_MISC1_LVDS_CLK1_OBEN	__BIT(10)
#define  CCM_ANALOG_MISC1_LVDS_CLK2_OBEN	__BIT(11)
#define  CCM_ANALOG_MISC1_LVDS_CLK1_IBEN	__BIT(12)
#define  CCM_ANALOG_MISC1_LVDS_CLK2_IBEN	__BIT(13)
#define CCM_ANALOG_MISC2			0x00000170
#define CCM_ANALOG_MISC2_SET			0x00000174
#define CCM_ANALOG_MISC2_CLR			0x00000178
#define CCM_ANALOG_MISC2_TOG			0x0000017C


#define CCM_TEMPMON_TEMPSENSE0			0x00000180
#define  CCM_TEMPMON_TEMPSENSE0_ALARM_VALUE	__BIT(31, 30)
#define  CCM_TEMPMON_TEMPSENSE0_TEMP_CNT	__BITS(19, 8)
#define  CCM_TEMPMON_TEMPSENSE0_FINISHED	__BIT(2)
#define  CCM_TEMPMON_TEMPSENSE0_MEASURE_TEMP	__BIT(1)
#define  CCM_TEMPMON_TEMPSENSE0_POWER_DOWN	__BIT(0)
#define CCM_TEMPMON_TEMPSENSE1			0x00000180
#define  CCM_TEMPMON_TEMPSENSE1_MEASURE_FREQ	__BITS(15, 0)


#define USB_ANALOG_USB1_VBUS_DETECT		0x000001a0
#define USB_ANALOG_USB1_CHRG_DETECT		0x000001b0
#define  USB_ANALOG_USB_CHRG_DETECT_EN_B	__BIT(20)
#define  USB_ANALOG_USB_CHRG_DETECT_CHK_CHRG_B	__BIT(19)
#define  USB_ANALOG_USB_CHRG_DETECT_CHK_CHK_CONTACT __BIT(18)
#define USB_ANALOG_USB1_VBUS_DETECT_STAT	0x000001c0
#define USB_ANALOG_USB1_CHRG_DETECT_STAT	0x000001d0
#define USB_ANALOG_USB1_MISC			0x000001f0
#define USB_ANALOG_USB2_VBUS_DETECT		0x00000200
#define USB_ANALOG_USB2_CHRG_DETECT		0x00000210
#define USB_ANALOG_USB2_VBUS_DETECT_STAT	0x00000220
#define USB_ANALOG_USB2_CHRG_DETECT_STAT	0x00000230
#define USB_ANALOG_USB2_MISC			0x00000250

#define USB_ANALOG_DIGPROG			0x00000260
#define USB_ANALOG_DIGPROG_SOLOLITE		0x00000280
#define  USB_ANALOG_DIGPROG_MAJOR		__BITS(23, 8)
#define  USB_ANALOG_DIGPROG_MINOR		__BITS(7, 0)

#endif /* _ARM_IMX_IMX6_CCMREG_H */
