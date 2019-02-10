/* $NetBSD: tegra210_carreg.h,v 1.9 2018/12/14 12:29:22 skrll Exp $ */

/*-
 * Copyright (c) 2017 Jared McNeill <jmcneill@invisible.ca>
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
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _ARM_TEGRA210_CARREG_H
#define _ARM_TEGRA210_CARREG_H

#define	TEGRA210_REF_FREQ	38400000

#define	CAR_RST_SOURCE_REG	0x00
#define	CAR_RST_SOURCE_WDT_EN		__BIT(5)
#define	CAR_RST_SOURCE_WDT_SEL		__BIT(4)
#define	CAR_RST_SOURCE_WDT_SYS_RST_EN	__BIT(2)
#define	CAR_RST_SOURCE_WDT_COP_RST_EN	__BIT(1)
#define	CAR_RST_SOURCE_WDT_CPU_RST_EN	__BIT(0)

#define	CAR_CLK_OUT_ENB_L_REG	0x10
#define	CAR_CLK_OUT_ENB_H_REG	0x14
#define	CAR_CLK_OUT_ENB_U_REG	0x18

#define	CAR_PLLE_SS_CNTL_REG	0x68
#define	CAR_PLLE_SS_CNTL_INTEGOFFSET	__BITS(31,30)
#define	CAR_PLLE_SS_CNTL_SSCINCINTRV	__BITS(29,24)
#define	CAR_PLLE_SS_CNTL_SSCINC		__BITS(23,16)
#define	CAR_PLLE_SS_CNTL_SSCINVERT	__BIT(15)
#define	CAR_PLLE_SS_CNTL_SSCCENTER	__BIT(14)
#define	CAR_PLLE_SS_CNTL_SSCPDMBYP	__BIT(13)
#define	CAR_PLLE_SS_CNTL_SSCBYP		__BIT(12)
#define	CAR_PLLE_SS_CNTL_INTERP_RESET	__BIT(11)
#define	CAR_PLLE_SS_CNTL_BYPASS_SS	__BIT(10)
#define	CAR_PLLE_SS_CNTL_SSCMAX		__BITS(8,0)

#define	CAR_PLLP_BASE_REG	0xa0
#define	CAR_PLLP_BASE_BYPASS		__BIT(31)
#define	CAR_PLLP_BASE_ENABLE		__BIT(30)
#define	CAR_PLLP_BASE_REF_DIS		__BIT(29)
#define	CAR_PLLP_BASE_OVERRIDE		__BIT(28)
#define	CAR_PLLP_BASE_LOCK		__BIT(27)
#define	CAR_PLLP_BASE_DIVP		__BITS(24,20)
#define	CAR_PLLP_BASE_DIVN		__BITS(17,10)
#define	CAR_PLLP_BASE_DIVM		__BITS(7,0)

#define	CAR_PLLP_OUTA_REG	0xa4
#define	CAR_PLLP_OUTA_OUT1_RATIO	__BITS(15,8)
#define	CAR_PLLP_OUTA_OUT1_OVRRIDE	__BIT(2)
#define	CAR_PLLP_OUTA_OUT1_CLKEN	__BIT(1)
#define	CAR_PLLP_OUTA_OUT1_RSTN		__BIT(0)
#define	CAR_PLLP_OUTB_REG	0xa8
#define	CAR_PLLP_OUTB_OUT4_RATIO	__BITS(31,24)
#define	CAR_PLLP_OUTB_OUT4_OVRRIDE	__BIT(18)
#define	CAR_PLLP_OUTB_OUT4_CLKEN	__BIT(17)
#define	CAR_PLLP_OUTB_OUT4_RSTN		__BIT(16)
#define	CAR_PLLP_OUTB_OUT3_RATIO	__BITS(15,8)
#define	CAR_PLLP_OUTB_OUT3_OVRRIDE	__BIT(2)
#define	CAR_PLLP_OUTB_OUT3_CLKEN	__BIT(1)
#define	CAR_PLLP_OUTB_OUT3_RSTN		__BIT(0)
#define	CAR_PLLP_OUTC_REG	0x67c
#define	CAR_PLLP_OUTC_OUT5_RATIO	__BITS(31,24)
#define	CAR_PLLP_OUTC_OUT5_OVERRIDE	__BIT(18)
#define	CAR_PLLP_OUTC_OUT5_CLKEN	__BIT(17)
#define	CAR_PLLP_OUTC_OUT5_RSTN		__BIT(16)
#define	CAR_PLLP_MISC_REG	0xac

#define	CAR_PLLC_BASE_REG	0x80
#define	CAR_PLLC_BASE_BYPASS		__BIT(31)
#define	CAR_PLLC_BASE_ENABLE		__BIT(30)
#define	CAR_PLLC_BASE_REF_DIS		__BIT(29)
#define	CAR_PLLC_BASE_LOCK_OVERRIDE	__BIT(27)
#define	CAR_PLLC_BASE_LOCK		__BIT(26)
#define	CAR_PLLC_BASE_DIVP		__BITS(24,20)
#define	CAR_PLLC_BASE_DIVN		__BITS(17,10)
#define	CAR_PLLC_BASE_DIVM		__BITS(7,0)

#define	CAR_PLLU_BASE_REG	0xc0
#define	CAR_PLLU_BASE_BYPASS		__BIT(31)
#define	CAR_PLLU_BASE_ENABLE		__BIT(30)
#define	CAR_PLLU_BASE_REF_DIS		__BIT(29)
#define	CAR_PLLU_BASE_LOCK		__BIT(27)
#define	CAR_PLLU_BASE_CLKENABLE_48M	__BIT(25)
#define	CAR_PLLU_BASE_OVERRIDE		__BIT(24)
#define	CAR_PLLU_BASE_CLKENABLE_ICUSB	__BIT(23)
#define	CAR_PLLU_BASE_CLKENABLE_HSIC	__BIT(22)
#define	CAR_PLLU_BASE_CLKENABLE_USB	__BIT(21)
#define	CAR_PLLU_BASE_DIVP		__BITS(20,16)
#define	CAR_PLLU_BASE_DIVN		__BITS(15,8)
#define	CAR_PLLU_BASE_DIVM		__BITS(4,0)

#define	CAR_PLLU_OUTA_REG	0xc4
#define	CAR_PLLU_OUTA_OUT2_RATIO	__BITS(31,24)
#define	CAR_PLLU_OUTA_OUT2_OVRRIDE	__BIT(18)
#define	CAR_PLLU_OUTA_OUT2_CLKEN	__BIT(17)
#define	CAR_PLLU_OUTA_OUT2_RSTN		__BIT(16)
#define	CAR_PLLU_OUTA_OUT1_RATIO	__BITS(15,8)
#define	CAR_PLLU_OUTA_OUT1_OVRRIDE	__BIT(2)
#define	CAR_PLLU_OUTA_OUT1_CLKEN	__BIT(1)
#define	CAR_PLLU_OUTA_OUT1_RSTN		__BIT(0)

#define	CAR_PLLU_MISC_REG	0xcc
#define	CAR_PLLU_MISC_IDDQ		__BIT(31)
#define	CAR_PLLU_MISC_FREQLOCK		__BIT(30)
#define	CAR_PLLU_MISC_EN_LCKDET		__BIT(29)
#define	CAR_PLLU_MISC_PTS		__BITS(28,27)
#define	CAR_PLLU_MISC_KCP		__BITS(26,25)
#define	CAR_PLLU_MISC_KVCO		__BIT(24)
#define	CAR_PLLU_MISC_SETUP		__BITS(23,0)

#define	CAR_PLLD_BASE_REG	0xd0
#define	CAR_PLLD_BASE_BYPASS		__BIT(31)
#define	CAR_PLLD_BASE_ENABLE		__BIT(30)
#define	CAR_PLLD_BASE_REF_DIS		__BIT(29)
#define	CAR_PLLD_BASE_LOCK		__BIT(27)
#define	CAR_PLLD_BASE_DSIA_CLK_SRC	__BIT(25)
#define	CAR_PLLD_BASE_CSI_CLK_SRC	__BIT(23)
#define	CAR_PLLD_BASE_DIVP		__BITS(22,20)
#define	CAR_PLLD_BASE_DIVN		__BITS(18,11)
#define	CAR_PLLD_BASE_DIVM		__BITS(7,0)

#define	CAR_PLLD_MISC_REG	0xdc

#define	CAR_PLLX_BASE_REG	0xe0
#define	CAR_PLLX_BASE_BYPASS		__BIT(31)
#define	CAR_PLLX_BASE_ENABLE		__BIT(30)
#define	CAR_PLLX_BASE_REF_DIS		__BIT(29)
#define	CAR_PLLX_BASE_LOCK		__BIT(27)
#define	CAR_PLLX_BASE_DIVP		__BITS(24,20)
#define	CAR_PLLX_BASE_DIVN		__BITS(15,8)
#define	CAR_PLLX_BASE_DIVM		__BITS(7,0)

#define	CAR_PLLX_MISC_REG	0xe4
#define	CAR_PLLX_MISC_FO_G_DISABLE	__BIT(28)
#define	CAR_PLLX_MISC_PTS		__BITS(23,22)
#define	CAR_PLLX_MISC_LOCK_ENABLE	__BIT(18)

#define	CAR_PLLE_BASE_REG	0xe8
#define	CAR_PLLE_BASE_ENABLE		__BIT(31)
#define	CAR_PLLE_BASE_LOCK_OVERRIDE	__BIT(30)
#define	CAR_PLLE_BASE_FDIV4B		__BIT(29)
#define	CAR_PLLE_BASE_DIVP_CML		__BITS(28,24)
#define	CAR_PLLE_BASE_EXT_SETUP_23_16	__BITS(23,16)
#define	CAR_PLLE_BASE_DIVN		__BITS(15,8)
#define	CAR_PLLE_BASE_DIVM		__BITS(7,0)

#define	CAR_PLLE_MISC_REG	0xec
#define	CAR_PLLE_MISC_SETUP		__BITS(31,16)
#define	CAR_PLLE_MISC_ENABLE		__BIT(15)
#define	CAR_PLLE_MISC_IDDQ_SWCTL	__BIT(14)
#define	CAR_PLLE_MISC_IDDQ_OVERRIDE	__BIT(13)
#define	CAR_PLLE_MISC_LOCK		__BIT(11)
#define	CAR_PLLE_MISC_LOCK_ENABLE	__BIT(9)
#define	CAR_PLLE_MISC_PTS		__BIT(8)
#define	CAR_PLLE_MISC_KCP		__BITS(7,6)
#define	CAR_PLLE_MISC_VREG_BG_CTRL	__BITS(5,4)
#define	CAR_PLLE_MISC_VREG_CTRL		__BITS(3,2)
#define	CAR_PLLE_MISC_KVCO		__BIT(0)

#define	CAR_PLLD2_BASE_REG	0x4b8
#define	CAR_PLLD2_BASE_BYPASS		__BIT(31)
#define	CAR_PLLD2_BASE_ENABLE		__BIT(30)
#define	CAR_PLLD2_BASE_REF_DIS		__BIT(29)
#define	CAR_PLLD2_BASE_FREQLOCK		__BIT(28)
#define	CAR_PLLD2_BASE_LOCK		__BIT(27)
#define	CAR_PLLD2_BASE_REF_SRC_SEL	__BITS(26,25)
#define	CAR_PLLD2_BASE_REF_SRC_SEL_PLL_D	0
#define	CAR_PLLD2_BASE_REF_SRC_SEL_PLL_D2	1
#define	CAR_PLLD2_BASE_LOCK_OVERRIDE	__BIT(24)
#define	CAR_PLLD2_BASE_DIVP		__BITS(23,19)
#define	CAR_PLLD2_BASE_IDDQ		__BIT(18)
#define	CAR_PLLD2_BASE_PTS		__BIT(16)
#define	CAR_PLLD2_BASE_DIVN		__BITS(15,8)
#define	CAR_PLLD2_BASE_DIVM		__BITS(7,0)

#define	CAR_PLLD2_MISC_REG	0x4bc
#define	CAR_PLLD2_MISC_LOCK_ENABLE	__BIT(30)
#define	CAR_PLLD2_MISC_KCP		__BITS(26,25)
#define	CAR_PLLD2_MISC_KVCO		__BIT(24)
#define	CAR_PLLD2_MISC_SETUP		__BITS(23,0)

#define	CAR_CLKSRC_I2C1_REG		0x124
#define	CAR_CLKSRC_I2C2_REG		0x198
#define	CAR_CLKSRC_I2C3_REG		0x1b8
#define	CAR_CLKSRC_I2C4_REG		0x3c4
#define	CAR_CLKSRC_I2C5_REG		0x128
#define	CAR_CLKSRC_I2C6_REG		0x65c

#define	CAR_CLKSRC_I2C_SRC		__BITS(31,29)
#define	CAR_CLKSRC_I2C_SRC_PLLP_OUT0	0
#define	CAR_CLKSRC_I2C_SRC_PLLC2_OUT0	1
#define	CAR_CLKSRC_I2C_SRC_PLLC_OUT0	2
#define	CAR_CLKSRC_I2C_SRC_PLLC4_OUT0	3
#define	CAR_CLKSRC_I2C_SRC_PLLC4_OUT1	5
#define	CAR_CLKSRC_I2C_SRC_CLK_M	6
#define	CAR_CLKSRC_I2C_SRC_PLLC4_OUT2	7
#define	CAR_CLKSRC_I2C_DIV		__BITS(15,0)

#define	CAR_CLKSRC_SPI1_REG		0x134
#define	CAR_CLKSRC_SPI2_REG		0x118
#define	CAR_CLKSRC_SPI3_REG		0x11c
#define	CAR_CLKSRC_SPI4_REG		0x1b4
#define	CAR_CLKSRC_SPI5_REG		0x3c8
#define	CAR_CLKSRC_SPI6_REG		0x3cc

#define	CAR_CLKSRC_SPI_SRC		__BITS(31,29)
#define	CAR_CLKSRC_SPI_SRC_PLLP_OUT0	0
#define	CAR_CLKSRC_SPI_SRC_PLLC2_OUT0	1
#define	CAR_CLKSRC_SPI_SRC_PLLC_OUT0	2
#define	CAR_CLKSRC_SPI_SRC_PLLC4_OUT0	3
#define	CAR_CLKSRC_SPI_SRC_PLLC4_OUT1	5
#define	CAR_CLKSRC_SPI_SRC_CLK_M	6
#define	CAR_CLKSRC_SPI_SRC_PLLC4_OUT2	7
#define	CAR_CLKSRC_SPI_DIV		__BITS(7,0)

#define	CAR_CLKSRC_UARTA_REG		0x178
#define	CAR_CLKSRC_UARTB_REG		0x17c
#define	CAR_CLKSRC_UARTC_REG		0x1a0
#define	CAR_CLKSRC_UARTD_REG		0x1c0

#define	CAR_CLKSRC_UART_SRC		__BITS(31,29)
#define	CAR_CLKSRC_UART_SRC_PLLP_OUT0	0
#define	CAR_CLKSRC_UART_SRC_PLLC2_OUT0	1
#define	CAR_CLKSRC_UART_SRC_PLLC_OUT0	2
#define	CAR_CLKSRC_UART_SRC_PLLC4_OUT0	3
#define	CAR_CLKSRC_UART_SRC_PLLC4_OUT1	5
#define	CAR_CLKSRC_UART_SRC_CLK_M	6
#define	CAR_CLKSRC_UART_SRC_PLLC4_OUT2	7
#define	CAR_CLKSRC_UART_DIV_ENB		__BIT(24)
#define	CAR_CLKSRC_UART_DIV		__BITS(15,0)

#define	CAR_CLKSRC_SDMMC1_REG		0x150
#define	CAR_CLKSRC_SDMMC2_REG		0x154
#define	CAR_CLKSRC_SDMMC4_REG		0x164
#define	CAR_CLKSRC_SDMMC3_REG		0x1bc

#define	CAR_CLKSRC_SDMMC_SRC		__BITS(31,29)
/* CAR_CLKSRC_SDMMC_SRC_* differs for each instance */
#define	CAR_CLKSRC_SDMMC_DIV		__BITS(7,0)

#define	CAR_CLKSRC_DISP1_REG		0x138
#define	CAR_CLKSRC_DISP2_REG		0x13c
#define	CAR_CLKSRC_DISP_SRC		__BITS(31,29)
#define	CAR_CLKSRC_DISP_SRC_PLLP_OUT0	0
#define	CAR_CLKSRC_DISP_SRC_PLLD_OUT	1
#define	CAR_CLKSRC_DISP_SRC_PLLD_OUT0	2	/* DISP1 only */
#define	CAR_CLKSRC_DISP_SRC_PLLD2_OUT0	5
#define	CAR_CLKSRC_DISP_SRC_CLK_M	6

#define	CAR_CLKSRC_HOST1X_REG		0x180
#define	CAR_CLKSRC_HOST1X_SRC		__BITS(31,29)
#define	CAR_CLKSRC_HOST1X_IDLE_DIVISOR	__BITS(15,8)
#define	CAR_CLKSRC_HOST1X_CLK_DIVISOR	__BITS(7,0)

#define	CAR_RST_DEV_L_SET_REG		0x300
#define	CAR_RST_DEV_L_CLR_REG		0x304
#define	CAR_RST_DEV_H_SET_REG		0x308
#define	CAR_RST_DEV_H_CLR_REG		0x30c
#define	CAR_RST_DEV_U_SET_REG		0x310
#define	CAR_RST_DEV_U_CLR_REG		0x314
#define	CAR_RST_DEV_V_SET_REG		0x430
#define	CAR_RST_DEV_V_CLR_REG		0x434
#define	CAR_RST_DEV_W_SET_REG		0x438
#define	CAR_RST_DEV_W_CLR_REG		0x43c
#define	CAR_RST_DEV_X_SET_REG		0x290
#define	CAR_RST_DEV_X_CLR_REG		0x294
#define	CAR_RST_DEV_Y_SET_REG		0x2a8
#define	CAR_RST_DEV_Y_CLR_REG		0x2ac

#define	CAR_CLK_ENB_L_SET_REG		0x320
#define	CAR_CLK_ENB_L_CLR_REG		0x324
#define	CAR_CLK_ENB_H_SET_REG		0x328
#define	CAR_CLK_ENB_H_CLR_REG		0x32c
#define	CAR_CLK_ENB_U_SET_REG		0x330
#define	CAR_CLK_ENB_U_CLR_REG		0x334
#define	CAR_CLK_ENB_V_SET_REG		0x440
#define	CAR_CLK_ENB_V_CLR_REG		0x444
#define	CAR_CLK_ENB_W_SET_REG		0x448
#define	CAR_CLK_ENB_W_CLR_REG		0x44c
#define	CAR_CLK_ENB_X_SET_REG		0x284
#define	CAR_CLK_ENB_X_CLR_REG		0x288
#define	CAR_CLK_ENB_Y_SET_REG		0x29c
#define	CAR_CLK_ENB_Y_CLR_REG		0x2a0

#define	CAR_DEV_L_CACHE2		__BIT(31)
#define	CAR_DEV_L_I2S1			__BIT(30)
#define	CAR_DEV_L_HOST1X		__BIT(28)
#define	CAR_DEV_L_DISP1			__BIT(27)
#define	CAR_DEV_L_DISP2			__BIT(26)
#define	CAR_DEV_L_ISP			__BIT(23)
#define	CAR_DEV_L_USBD			__BIT(22)
#define	CAR_DEV_L_VI			__BIT(20)
#define	CAR_DEV_L_I2S3			__BIT(18)
#define	CAR_DEV_L_PWM			__BIT(17)
#define	CAR_DEV_L_SDMMC4		__BIT(15)
#define	CAR_DEV_L_SDMMC1		__BIT(14)
#define	CAR_DEV_L_I2C1			__BIT(12)
#define	CAR_DEV_L_I2S2			__BIT(11)
#define	CAR_DEV_L_SPDIF			__BIT(10)
#define	CAR_DEV_L_SDMMC2		__BIT(9)
#define	CAR_DEV_L_GPIO			__BIT(8)
#define	CAR_DEV_L_UARTB			__BIT(7)
#define	CAR_DEV_L_UARTA			__BIT(6)
#define	CAR_DEV_L_TMR			__BIT(5)
#define	CAR_DEV_L_RTC			__BIT(4)
#define	CAR_DEV_L_ISPB			__BIT(3)
#define	CAR_DEV_L_TRIG_SYS		__BIT(2)
#define	CAR_DEV_L_COP			__BIT(1)
#define	CAR_DEV_L_CPU			__BIT(0)

#define	CAR_DEV_U_XUSB_DEV		__BIT(31)
#define	CAR_DEV_U_DEV1_OUT		__BIT(30)
#define	CAR_DEV_U_DEV2_OUT		__BIT(29)
#define	CAR_DEV_U_SUS_OUT		__BIT(28)
#define	CAR_DEV_U_MSENC			__BIT(27)
#define	CAR_DEV_U_XUSB_HOST		__BIT(25)
#define	CAR_DEV_U_CRAM2			__BIT(24)
#define	CAR_DEV_U_IRAMD			__BIT(23)
#define	CAR_DEV_U_IRAMC			__BIT(22)
#define	CAR_DEV_U_IRAMB			__BIT(21)
#define	CAR_DEV_U_IRAMA			__BIT(20)
#define	CAR_DEV_U_TSEC			__BIT(19)
#define	CAR_DEV_U_DSIB			__BIT(18)
#define	CAR_DEV_U_I2C_SLOW		__BIT(17)
#define	CAR_DEV_U_DTV			__BIT(15)
#define	CAR_DEV_U_SOC_THERM		__BIT(14)
#define	CAR_DEV_U_PCIEXCLK		__BIT(10)
#define	CAR_DEV_U_CSITE			__BIT(9)
#define	CAR_DEV_U_AFI			__BIT(8)
#define	CAR_DEV_U_PCIE			__BIT(6)
#define	CAR_DEV_U_SDMMC3		__BIT(5)
#define	CAR_DEV_U_SPI4			__BIT(4)
#define	CAR_DEV_U_I2C3			__BIT(3)
#define	CAR_DEV_U_UARTD			__BIT(1)

#define	CAR_DEV_H_BSEV			__BIT(31)
#define	CAR_DEV_H_USB2			__BIT(26)
#define	CAR_DEV_H_EMC			__BIT(25)
#define	CAR_DEV_H_MIPI_CAL		__BIT(24)
#define	CAR_DEV_H_UARTC			__BIT(23)
#define	CAR_DEV_H_I2C2			__BIT(22)
#define	CAR_DEV_H_CSI			__BIT(20)
#define	CAR_DEV_H_DSI			__BIT(16)
#define	CAR_DEV_H_I2C5			__BIT(15)
#define	CAR_DEV_H_SPI3			__BIT(14)
#define	CAR_DEV_H_SPI2			__BIT(12)
#define	CAR_DEV_H_SPI1			__BIT(9)
#define	CAR_DEV_H_KFUSE			__BIT(8)
#define	CAR_DEV_H_FUSE			__BIT(7)
#define	CAR_DEV_H_PMC			__BIT(6)
#define	CAR_DEV_H_STAT_MON		__BIT(5)
#define	CAR_DEV_H_APBDMA		__BIT(2)
#define	CAR_DEV_H_AHBDMA		__BIT(1)
#define	CAR_DEV_H_MEM			__BIT(0)

#define	CAR_DEV_V_HDA			__BIT(29)
#define	CAR_DEV_V_SATA			__BIT(28)
#define	CAR_DEV_V_SATA_OOB		__BIT(27)
#define	CAR_DEV_V_EXTPERIPH3		__BIT(26)
#define	CAR_DEV_V_EXTPERIPH2		__BIT(25)
#define	CAR_DEV_V_EXTPERIPH1		__BIT(24)
#define	CAR_DEV_V_ACTMON		__BIT(23)
#define	CAR_DEV_V_SPDIF_DOUBLER		__BIT(22)
#define	CAR_DEV_V_ATOMICS		__BIT(16)
#define	CAR_DEV_V_HDA2CODEC_2X		__BIT(15)
#define	CAR_DEV_V_APB2APE		__BIT(11)
#define	CAR_DEV_V_AHUB			__BIT(10)
#define	CAR_DEV_V_I2C4			__BIT(7)
#define	CAR_DEV_V_I2S5			__BIT(6)
#define	CAR_DEV_V_I2S4			__BIT(5)
#define	CAR_DEV_V_TSENSOR		__BIT(4)
#define	CAR_DEV_V_MSELECT		__BIT(3)
#define	CAR_DEV_V_CPULP			__BIT(1)
#define	CAR_DEV_V_CPUG			__BIT(0)

#define	CAR_DEV_W_MC1			__BIT(30)
#define	CAR_DEV_W_EMC_DLL		__BIT(29)
#define	CAR_DEV_W_XUSB_SS		__BIT(28)
#define	CAR_DEV_W_DVFS			__BIT(27)
#define	CAR_DEV_W_ENTROPY		__BIT(21)
#define	CAR_DEV_W_DSIB_LP		__BIT(20)
#define	CAR_DEV_W_DSIA_LP		__BIT(19)
#define	CAR_DEV_W_CILEF			__BIT(18)
#define	CAR_DEV_W_CILCD			__BIT(17)
#define	CAR_DEV_W_CILAB			__BIT(16)
#define	CAR_DEV_W_XUSB			__BIT(15)
#define	CAR_DEV_W_XUSB_PADCTL		__BIT(14)
#define	CAR_DEV_W_MIPI_IOBIST		__BIT(13)
#define	CAR_DEV_W_SATA_IOBIST		__BIT(12)
#define	CAR_DEV_W_EMC_IOBIST		__BIT(10)
#define	CAR_DEV_W_PCIE2_IOBIST		__BIT(9)
#define	CAR_DEV_W_CEC			__BIT(8)
#define	CAR_DEV_W_PCIERX5		__BIT(7)
#define	CAR_DEV_W_PCIERX4		__BIT(6)
#define	CAR_DEV_W_PCIERX3		__BIT(5)
#define	CAR_DEV_W_PCIERX2		__BIT(4)
#define	CAR_DEV_W_PCIERX1		__BIT(3)
#define	CAR_DEV_W_PCIERX0		__BIT(2)
#define	CAR_DEV_W_SATACOLD		__BIT(1)
#define	CAR_DEV_W_HDA2HDMICODEC		__BIT(0)

#define	CAR_DEV_X_PLLG_REF		__BIT(29)
#define	CAR_DEV_X_PLLA_ADSP		__BIT(28)
#define	CAR_DEV_X_PLLP_ADSP		__BIT(27)
#define	CAR_DEV_X_HPLL_ADSP		__BIT(26)
#define	CAR_DEV_X_DBGAPB		__BIT(25)
#define	CAR_DEV_X_GPU			__BIT(24)
#define	CAR_DEV_X_SOR1			__BIT(23)
#define	CAR_DEV_X_SOR0			__BIT(22)
#define	CAR_DEV_X_DPAUX			__BIT(21)
#define	CAR_DEV_X_VIC			__BIT(18)
#define	CAR_DEV_X_UART_FST_MIPI_CAL	__BIT(17)
#define	CAR_DEV_X_EMC_DLL		__BIT(14)
#define	CAR_DEV_X_VIM2_CLK		__BIT(11)
#define	CAR_DEV_X_MC_BBC		__BIT(10)
#define	CAR_DEV_X_MC_CPU		__BIT(9)
#define	CAR_DEV_X_MC_CBPA		__BIT(8)
#define	CAR_DEV_X_MC_CAPA		__BIT(7)
#define	CAR_DEV_X_I2C6			__BIT(6)
#define	CAR_DEV_X_CAM_MCLK2		__BIT(5)
#define	CAR_DEV_X_CAM_MCLK		__BIT(4)
#define	CAR_DEV_X_ETR			__BIT(3)
#define	CAR_DEV_X_SPARE			__BIT(0)

#define	CAR_DEV_Y_PLLP_OUT_CPU		__BIT(31)
#define	CAR_DEV_Y_SOR_SAFE		__BIT(30)
#define	CAR_DEV_Y_IQC1			__BIT(29)
#define	CAR_DEV_Y_IQC2			__BIT(28)
#define	CAR_DEV_Y_NVENC			__BIT(27)
#define	CAR_DEV_Y_ADSPNEON		__BIT(26)
#define	CAR_DEV_Y_ADSPSCU		__BIT(25)
#define	CAR_DEV_Y_ADSPWDT		__BIT(24)
#define	CAR_DEV_Y_ADSPDBG		__BIT(23)
#define	CAR_DEV_Y_ADSPPERIPH		__BIT(22)
#define	CAR_DEV_Y_ADSPINTF		__BIT(21)
#define	CAR_DEV_Y_UARTAPE		__BIT(20)
#define	CAR_DEV_Y_QSPI			__BIT(19)
#define	CAR_DEV_Y_USB2_TRK		__BIT(18)
#define	CAR_DEV_Y_HSIC_TRK		__BIT(17)
#define	CAR_DEV_Y_VI_I2C		__BIT(16)
#define	CAR_DEV_Y_DPAUX			__BIT(15)
#define	CAR_DEV_Y_TSECB			__BIT(14)
#define	CAR_DEV_Y_PEX_USB_UPHY		__BIT(13)
#define	CAR_DEV_Y_SATA_USB_UPHY		__BIT(12)
#define	CAR_DEV_Y_MAUD			__BIT(10)
#define	CAR_DEV_Y_MC_CCPA		__BIT(9)
#define	CAR_DEV_Y_MC_CDPA		__BIT(8)
#define	CAR_DEV_Y_ADSP			__BIT(7)
#define	CAR_DEV_Y_APE			__BIT(6)
#define	CAR_DEV_Y_DMIC3			__BIT(5)
#define	CAR_DEV_Y_AXIAP			__BIT(4)
#define	CAR_DEV_Y_NVJPG			__BIT(3)
#define	CAR_DEV_Y_NVDEC			__BIT(2)
#define	CAR_DEV_Y_SDMMC_LEGACY_TM	__BIT(1)
#define	CAR_DEV_Y_SPARE1		__BIT(0)

#define	CAR_CCLKG_BURST_POLICY_REG	0x368
#define	CAR_CCLKG_BURST_POLICY_CPU_STATE	__BITS(31,28)
#define	CAR_CCLKG_BURST_POLICY_CPU_STATE_IDLE			1
#define	CAR_CCLKG_BURST_POLICY_CPU_STATE_RUN			2
#define	CAR_CCLKG_BURST_POLICY_CWAKEUP_IDLE_SOURCE __BITS(3,0)
#define	CAR_CCLKG_BURST_POLICY_CWAKEUP_SOURCE_CLKM		0
#define	CAR_CCLKG_BURST_POLICY_CWAKEUP_SOURCE_PLLX_OUT0_LJ	8

#define	CAR_CLKSRC_MSELECT_REG		0x3b4
#define	CAR_CLKSRC_MSELECT_SRC		__BITS(31,29)
#define	CAR_CLKSRC_MSELECT_DIV		__BITS(7,0)

#define	CAR_CLKSRC_TSENSOR_REG		0x3b8
#define	CAR_CLKSRC_TSENSOR_SRC		__BITS(31,29)
#define	CAR_CLKSRC_TSENSOR_SRC_CLK_M	4
#define	CAR_CLKSRC_TSENSOR_DIV		__BITS(7,0)

#define	CAR_CLKSRC_HDA2CODEC_2X_REG	0x3e4
#define	CAR_CLKSRC_HDA2CODEC_2X_SRC	__BITS(31,29)
#define	CAR_CLKSRC_HDA2CODEC_2X_DIV	__BITS(7,0)

#define	CAR_CLKSRC_SATA_OOB_REG		0x420
#define	CAR_CLKSRC_SATA_OOB_SRC		__BITS(31,29)
#define	CAR_CLKSRC_SATA_OOB_DIV		__BITS(7,0)

#define	CAR_CLKSRC_SATA_REG		0x424
#define	CAR_CLKSRC_SATA_SRC		__BITS(31,29)
#define	CAR_CLKSRC_SATA_AUX_CLK_ENB	__BIT(24)
#define	CAR_CLKSRC_SATA_DIV		__BITS(7,0)

#define	CAR_CLKSRC_HDA_REG		0x428
#define	CAR_CLKSRC_HDA_SRC		__BITS(31,29)
#define	CAR_CLKSRC_HDA_DIV		__BITS(7,0)

#define	CAR_UTMIP_PLL_CFG0_REG		0x480

#define	CAR_UTMIP_PLL_CFG1_REG		0x484
#define	CAR_UTMIP_PLL_CFG1_ENABLE_DLY_COUNT	__BITS(31,27)
#define	CAR_UTMIP_PLL_CFG1_PLLU_POWERUP		__BIT(17)
#define	CAR_UTMIP_PLL_CFG1_PLLU_POWERDOWN	__BIT(16)
#define	CAR_UTMIP_PLL_CFG1_PLL_ENABLE_POWERUP	__BIT(15)
#define	CAR_UTMIP_PLL_CFG1_PLL_ENABLE_POWERDOWN	__BIT(14)
#define	CAR_UTMIP_PLL_CFG1_XTAL_FREQ_COUNT	__BITS(11,0)

#define	CAR_UTMIP_PLL_CFG2_REG		0x488
#define	CAR_UTMIP_PLL_CFG2_PHY_XTAL_CLOCKEN	__BIT(30)
#define	CAR_UTMIP_PLL_CFG2_PD_SAMP_D_POWERUP	__BIT(25)
#define	CAR_UTMIP_PLL_CFG2_PD_SAMP_D_POWERDOWN	__BIT(24)
#define	CAR_UTMIP_PLL_CFG2_ACTIVE_DLY_COUNT	__BITS(23,18)
#define	CAR_UTMIP_PLL_CFG2_STABLE_COUNT		__BITS(17,6)
#define	CAR_UTMIP_PLL_CFG2_PD_SAMP_C_POWERUP	__BIT(5)
#define	CAR_UTMIP_PLL_CFG2_PD_SAMP_C_POWERDOWN	__BIT(4)
#define	CAR_UTMIP_PLL_CFG2_PD_SAMP_B_POWERUP	__BIT(3)
#define	CAR_UTMIP_PLL_CFG2_PD_SAMP_B_POWERDOWN	__BIT(2)
#define	CAR_UTMIP_PLL_CFG2_PD_SAMP_A_POWERUP	__BIT(1)
#define	CAR_UTMIP_PLL_CFG2_PD_SAMP_A_POWERDOWN	__BIT(0)

#define	CAR_PLLE_AUX_REG		0x48c
#define	CAR_PLLE_AUX_SS_SEQ_INCLUDE		__BIT(31)
#define	CAR_PLLE_AUX_REF_SEL_PLLREFE		__BIT(28)
#define	CAR_PLLE_AUX_SEQ_STATE			__BITS(27,26)
#define	CAR_PLLE_AUX_SEQ_START_STATE		__BIT(25)
#define	CAR_PLLE_AUX_SEQ_ENABLE			__BIT(24)
#define	CAR_PLLE_AUX_SS_DLY			__BITS(23,16)
#define	CAR_PLLE_AUX_LOCK_DLY			__BITS(15,8)
#define	CAR_PLLE_AUX_FAST_PT			__BIT(7)
#define	CAR_PLLE_AUX_SS_SWCTL			__BIT(6)
#define	CAR_PLLE_AUX_CONFIG_SWCTL		__BIT(5)
#define	CAR_PLLE_AUX_ENABLE_SWCTL		__BIT(4)
#define	CAR_PLLE_AUX_USE_LOCKDET		__BIT(3)
#define	CAR_PLLE_AUX_REF_SRC			__BIT(2)
#define	CAR_PLLE_AUX_CML1_OEN			__BIT(1)
#define	CAR_PLLE_AUX_CML0_OEN			__BIT(0)

#define	CAR_SATA_PLL_CFG0_REG		0x490
#define	CAR_SATA_PLL_CFG0_SEQ_STATE		__BITS(27,26)
#define	CAR_SATA_PLL_CFG0_SEQ_START_STATE	__BIT(25)
#define	CAR_SATA_PLL_CFG0_SEQ_ENABLE		__BIT(24)
#define	CAR_SATA_PLL_CFG0_SEQ_PADPLL_SLEEP_IDDQ	__BIT(13)
#define	CAR_SATA_PLL_CFG0_SEQ_PADPLL_PD_INPUT_VALUE __BIT(7)
#define	CAR_SATA_PLL_CFG0_SEQ_LANE_PD_INPUT_VALUE __BIT(6)
#define	CAR_SATA_PLL_CFG0_SEQ_RESET_INPUT_VALUE	__BIT(5)
#define	CAR_SATA_PLL_CFG0_SEQ_IN_SWCTL		__BIT(4)
#define	CAR_SATA_PLL_CFG0_PADPLL_USE_LOCKDET	__BIT(2)
#define	CAR_SATA_PLL_CFG0_PADPLL_RESET_OVERRIDE_VALUE __BIT(1)
#define	CAR_SATA_PLL_CFG0_PADPLL_RESET_SWCTL	__BIT(0)

#define	CAR_SATA_PLL_CFG1_REG		0x494
#define	CAR_SATA_PLL_CFG1_LANE_IDDQ2_PADPLL_RESET_DLY __BITS(31,24)
#define	CAR_SATA_PLL_CFG1_PADPLL_IDDQ2LANE_SLUMBER_DLY __BITS(23,16)
#define	CAR_SATA_PLL_CFG1_PADPLL_PU_POST_DLY	__BITS(15,8)
#define	CAR_SATA_PLL_CFG1_LANE_IDDQ2_PADPLL_IDDQ_DLY __BITS(7,0)

#define	CAR_UTMIP_PLL_CFG3_REG		0x4c0
#define	CAR_UTMIP_PLL_CFG3_REF_SRC_SEL		__BIT(26)
#define	CAR_UTMIP_PLL_CFG3_REF_DIS		__BIT(25)
#define	CAR_UTMIP_PLL_CFG3_PTS			__BIT(24)
#define	CAR_UTMIP_PLL_CFG3_SETUP		__BITS(23,0)

#define	CAR_PLLREFE_BASE_REG		0x4c4
#define	CAR_PLLREFE_BASE_BYPASS			__BIT(31)
#define	CAR_PLLREFE_BASE_ENABLE			__BIT(30)
#define	CAR_PLLREFE_BASE_REF_DIS		__BIT(29)
#define	CAR_PLLREFE_BASE_KCP			__BITS(28,27)
#define	CAR_PLLREFE_BASE_KVCO			__BIT(26)
#define	CAR_PLLREFE_BASE_DIVP			__BITS(20,16)
#define	CAR_PLLREFE_BASE_DIVN			__BITS(15,8)
#define	CAR_PLLREFE_BASE_DIVM			__BITS(7,0)

#define	CAR_PLLREFE_MISC_REG		0x4c8
#define	CAR_PLLREFE_MISC_LOCK_ENABLE		__BIT(30)
#define	CAR_PLLREFE_MISC_LOCK_OVERRIDE		__BIT(29)
#define	CAR_PLLREFE_MISC_FREQLOCK		__BIT(28)
#define	CAR_PLLREFE_MISC_LOCK			__BIT(27)
#define	CAR_PLLREFE_MISC_PTS			__BITS(26,25)
#define	CAR_PLLREFE_MISC_IDDQ			__BIT(24)
#define	CAR_PLLREFE_MISC_SETUP			__BITS(23,0)

#define	CAR_XUSBIO_PLL_CFG0_REG		0x51c
#define	CAR_XUSBIO_PLL_CFG0_SEQ_STATE		__BITS(27,26)
#define	CAR_XUSBIO_PLL_CFG0_SEQ_START_STATE	__BIT(25)
#define	CAR_XUSBIO_PLL_CFG0_SEQ_ENABLE		__BIT(24)
#define	CAR_XUSBIO_PLL_CFG0_PADPLL_SLEEP_IDDQ	__BIT(13)
#define	CAR_XUSBIO_PLL_CFG0_PADPLL_USE_LOCKDET	__BIT(6)
#define	CAR_XUSBIO_PLL_CFG0_SEQ_RESET_INPUT_VALUE	__BIT(5)
#define	CAR_XUSBIO_PLL_CFG0_SEQ_IN_SWCTL	__BIT(4)
#define	CAR_XUSBIO_PLL_CFG0_CLK_ENABLE_OVERRIDE	__BIT(3)
#define	CAR_XUSBIO_PLL_CFG0_CLK_ENABLE_SWCTL	__BIT(2)
#define	CAR_XUSBIO_PLL_CFG0_PADPLL_RESET_OVERRIDE_VALUE __BIT(1)
#define	CAR_XUSBIO_PLL_CFG0_PADPLL_RESET_SWCTL	__BIT(0)

#define	CAR_UTMIPLL_HW_PWRDN_CFG0_REG	0x52c
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_LOCK		__BIT(31)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_SEQ_STATE	__BITS(27,26)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_SEQ_START_STATE __BIT(25)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_SEQ_ENABLE	__BIT(24)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_IDDQ_PD_INCLUDE __BIT(7)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_USE_LOCKDET	__BIT(6)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_SEQ_RESET_INPUT_VALUE __BIT(5)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_SEQ_IN_SWCTL	__BIT(4)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_CLK_ENABLE_OVERRIDE __BIT(3)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_CLK_ENABLE_SWCTL __BIT(2)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_IDDQ_OVERRIDE	__BIT(1)
#define	CAR_UTMIPLL_HW_PWRDN_CFG0_IDDQ_SWCTL	__BIT(0)

#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_REG	0x530
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_IDDQ_PD_INCLUDE	__BIT(28)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_SEQ_STATE		__BITS(27,26)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_SEQ_START_STATE	__BIT(25)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_SEQ_ENABLE	__BIT(24)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_USE_SWITCH_DETECT	__BIT(7)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_USE_LOCKDET	__BIT(6)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_SEQ_RESET_INPUT_VALUE __BIT(5)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_SEQ_IN_SWCTL	__BIT(4)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_CLK_ENABLE_OVERRIDE_VALUE __BIT(3)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_CLK_ENABLE_SWCTL	__BIT(2)
#define	CLK_RST_CONTROLLER_PLLU_HW_PWRDN_CFG0_CLK_SWITCH_SWCTL	__BIT(0)

#define	CLK_RST_CONTROLLER_XUSB_PLL_CFG0_REG	0x534
#define	CLK_RST_CONTROLLER_XUSB_PLL_CFG0_PLLU_CLK_SWITCH_DLY	__BITS(31,24)
#define	CLK_RST_CONTROLLER_XUSB_PLL_CFG0_PLLU_LOCK_DLY		__BITS(23,14)
#define	CLK_RST_CONTROLLER_XUSB_PLL_CFG0_UTMIPLL_IDDQ2_ENABLE_DLY __BITS(13,10)
#define	CLK_RST_CONTROLLER_XUSB_PLL_CFG0_UTMIPLL_LOCK_DLY	__BITS(9,0)

#define	CAR_CLKSRC_XUSB_HOST_REG	0x600
#define	CAR_CLKSRC_XUSB_HOST_SRC		__BITS(31,29)
#define	CAR_CLKSRC_XUSB_HOST_DIV		__BITS(7,0)

#define	CAR_CLKSRC_XUSB_FALCON_REG	0x604
#define	CAR_CLKSRC_XUSB_FALCON_SRC		__BITS(31,29)
#define	CAR_CLKSRC_XUSB_FALCON_DIV		__BITS(7,0)

#define	CAR_CLKSRC_XUSB_FS_REG		0x608
#define	CAR_CLKSRC_XUSB_FS_SRC			__BITS(31,29)
#define	CAR_CLKSRC_XUSB_FS_DIV			__BITS(7,0)

#define	CAR_CLKSRC_XUSB_SS_REG		0x610
#define	CAR_CLKSRC_XUSB_SS_SRC			__BITS(31,29)
#define	CAR_CLKSRC_XUSB_SS_HS_CLK_BYPASS	__BIT(25)
#define	CAR_CLKSRC_XUSB_SS_SS_CLK_BYPASS	__BIT(24)
#define	CAR_CLKSRC_XUSB_SS_DIV			__BITS(7,0)

#define	CAR_CLKSRC_SOC_THERM_REG	0x644
#define	CAR_CLKSRC_SOC_THERM_SRC	__BITS(31,29)
#define	CAR_CLKSRC_SOC_THERM_SRC_PLLP_OUT0	2
#define	CAR_CLKSRC_SOC_THERM_DDLL_SEL	__BITS(11,10)
#define	CAR_CLKSRC_SOC_THERM_DIV	__BITS(7,0)

#define	CAR_CLKSRC_USB2_HSIC_TRK_REG	0x6cc
#define	CAR_CLKSRC_USB2_HSIC_TRK_DIV	__BITS(7,0)

#endif /* _ARM_TEGRA210_CARREG_H */
