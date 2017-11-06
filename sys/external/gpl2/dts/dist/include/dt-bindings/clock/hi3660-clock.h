/*	$NetBSD: hi3660-clock.h,v 1.1.1.2 2017/10/28 10:30:32 jmcneill Exp $	*/

/*
 * Copyright (c) 2016-2017 Linaro Ltd.
 * Copyright (c) 2016-2017 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __DTS_HI3660_CLOCK_H
#define __DTS_HI3660_CLOCK_H

/* fixed rate clocks */
#define HI3660_CLKIN_SYS		0
#define HI3660_CLKIN_REF		1
#define HI3660_CLK_FLL_SRC		2
#define HI3660_CLK_PPLL0		3
#define HI3660_CLK_PPLL1		4
#define HI3660_CLK_PPLL2		5
#define HI3660_CLK_PPLL3		6
#define HI3660_CLK_SCPLL		7
#define HI3660_PCLK			8
#define HI3660_CLK_UART0_DBG		9
#define HI3660_CLK_UART6		10
#define HI3660_OSC32K			11
#define HI3660_OSC19M			12
#define HI3660_CLK_480M			13
#define HI3660_CLK_INV			14

/* clk in crgctrl */
#define HI3660_FACTOR_UART3		15
#define HI3660_CLK_FACTOR_MMC		16
#define HI3660_CLK_GATE_I2C0		17
#define HI3660_CLK_GATE_I2C1		18
#define HI3660_CLK_GATE_I2C2		19
#define HI3660_CLK_GATE_I2C6		20
#define HI3660_CLK_DIV_SYSBUS		21
#define HI3660_CLK_DIV_320M		22
#define HI3660_CLK_DIV_A53		23
#define HI3660_CLK_GATE_SPI0		24
#define HI3660_CLK_GATE_SPI2		25
#define HI3660_PCIEPHY_REF		26
#define HI3660_CLK_ABB_USB		27
#define HI3660_HCLK_GATE_SDIO0		28
#define HI3660_HCLK_GATE_SD		29
#define HI3660_CLK_GATE_AOMM		30
#define HI3660_PCLK_GPIO0		31
#define HI3660_PCLK_GPIO1		32
#define HI3660_PCLK_GPIO2		33
#define HI3660_PCLK_GPIO3		34
#define HI3660_PCLK_GPIO4		35
#define HI3660_PCLK_GPIO5		36
#define HI3660_PCLK_GPIO6		37
#define HI3660_PCLK_GPIO7		38
#define HI3660_PCLK_GPIO8		39
#define HI3660_PCLK_GPIO9		40
#define HI3660_PCLK_GPIO10		41
#define HI3660_PCLK_GPIO11		42
#define HI3660_PCLK_GPIO12		43
#define HI3660_PCLK_GPIO13		44
#define HI3660_PCLK_GPIO14		45
#define HI3660_PCLK_GPIO15		46
#define HI3660_PCLK_GPIO16		47
#define HI3660_PCLK_GPIO17		48
#define HI3660_PCLK_GPIO18		49
#define HI3660_PCLK_GPIO19		50
#define HI3660_PCLK_GPIO20		51
#define HI3660_PCLK_GPIO21		52
#define HI3660_CLK_GATE_SPI3		53
#define HI3660_CLK_GATE_I2C7		54
#define HI3660_CLK_GATE_I2C3		55
#define HI3660_CLK_GATE_SPI1		56
#define HI3660_CLK_GATE_UART1		57
#define HI3660_CLK_GATE_UART2		58
#define HI3660_CLK_GATE_UART4		59
#define HI3660_CLK_GATE_UART5		60
#define HI3660_CLK_GATE_I2C4		61
#define HI3660_CLK_GATE_DMAC		62
#define HI3660_PCLK_GATE_DSS		63
#define HI3660_ACLK_GATE_DSS		64
#define HI3660_CLK_GATE_LDI1		65
#define HI3660_CLK_GATE_LDI0		66
#define HI3660_CLK_GATE_VIVOBUS		67
#define HI3660_CLK_GATE_EDC0		68
#define HI3660_CLK_GATE_TXDPHY0_CFG	69
#define HI3660_CLK_GATE_TXDPHY0_REF	70
#define HI3660_CLK_GATE_TXDPHY1_CFG	71
#define HI3660_CLK_GATE_TXDPHY1_REF	72
#define HI3660_ACLK_GATE_USB3OTG	73
#define HI3660_CLK_GATE_SPI4		74
#define HI3660_CLK_GATE_SD		75
#define HI3660_CLK_GATE_SDIO0		76
#define HI3660_CLK_GATE_UFS_SUBSYS	77
#define HI3660_PCLK_GATE_DSI0		78
#define HI3660_PCLK_GATE_DSI1		79
#define HI3660_ACLK_GATE_PCIE		80
#define HI3660_PCLK_GATE_PCIE_SYS       81
#define HI3660_CLK_GATE_PCIEAUX		82
#define HI3660_PCLK_GATE_PCIE_PHY	83
#define HI3660_CLK_ANDGT_LDI0		84
#define HI3660_CLK_ANDGT_LDI1		85
#define HI3660_CLK_ANDGT_EDC0		86
#define HI3660_CLK_GATE_UFSPHY_GT	87
#define HI3660_CLK_ANDGT_MMC		88
#define HI3660_CLK_ANDGT_SD		89
#define HI3660_CLK_A53HPM_ANDGT		90
#define HI3660_CLK_ANDGT_SDIO		91
#define HI3660_CLK_ANDGT_UART0		92
#define HI3660_CLK_ANDGT_UART1		93
#define HI3660_CLK_ANDGT_UARTH		94
#define HI3660_CLK_ANDGT_SPI		95
#define HI3660_CLK_VIVOBUS_ANDGT	96
#define HI3660_CLK_AOMM_ANDGT		97
#define HI3660_CLK_320M_PLL_GT		98
#define HI3660_AUTODIV_EMMC0BUS		99
#define HI3660_AUTODIV_SYSBUS		100
#define HI3660_CLK_GATE_UFSPHY_CFG	101
#define HI3660_CLK_GATE_UFSIO_REF	102
#define HI3660_CLK_MUX_SYSBUS		103
#define HI3660_CLK_MUX_UART0		104
#define HI3660_CLK_MUX_UART1		105
#define HI3660_CLK_MUX_UARTH		106
#define HI3660_CLK_MUX_SPI		107
#define HI3660_CLK_MUX_I2C		108
#define HI3660_CLK_MUX_MMC_PLL		109
#define HI3660_CLK_MUX_LDI1		110
#define HI3660_CLK_MUX_LDI0		111
#define HI3660_CLK_MUX_SD_PLL		112
#define HI3660_CLK_MUX_SD_SYS		113
#define HI3660_CLK_MUX_EDC0		114
#define HI3660_CLK_MUX_SDIO_SYS		115
#define HI3660_CLK_MUX_SDIO_PLL		116
#define HI3660_CLK_MUX_VIVOBUS		117
#define HI3660_CLK_MUX_A53HPM		118
#define HI3660_CLK_MUX_320M		119
#define HI3660_CLK_MUX_IOPERI		120
#define HI3660_CLK_DIV_UART0		121
#define HI3660_CLK_DIV_UART1		122
#define HI3660_CLK_DIV_UARTH		123
#define HI3660_CLK_DIV_MMC		124
#define HI3660_CLK_DIV_SD		125
#define HI3660_CLK_DIV_EDC0		126
#define HI3660_CLK_DIV_LDI0		127
#define HI3660_CLK_DIV_SDIO		128
#define HI3660_CLK_DIV_LDI1		129
#define HI3660_CLK_DIV_SPI		130
#define HI3660_CLK_DIV_VIVOBUS		131
#define HI3660_CLK_DIV_I2C		132
#define HI3660_CLK_DIV_UFSPHY		133
#define HI3660_CLK_DIV_CFGBUS		134
#define HI3660_CLK_DIV_MMC0BUS		135
#define HI3660_CLK_DIV_MMC1BUS		136
#define HI3660_CLK_DIV_UFSPERI		137
#define HI3660_CLK_DIV_AOMM		138
#define HI3660_CLK_DIV_IOPERI		139
#define HI3660_VENC_VOLT_HOLD		140
#define HI3660_PERI_VOLT_HOLD		141
#define HI3660_CLK_GATE_VENC		142
#define HI3660_CLK_GATE_VDEC		143
#define HI3660_CLK_ANDGT_VENC		144
#define HI3660_CLK_ANDGT_VDEC		145
#define HI3660_CLK_MUX_VENC		146
#define HI3660_CLK_MUX_VDEC		147
#define HI3660_CLK_DIV_VENC		148
#define HI3660_CLK_DIV_VDEC		149
#define HI3660_CLK_FAC_ISP_SNCLK	150
#define HI3660_CLK_GATE_ISP_SNCLK0	151
#define HI3660_CLK_GATE_ISP_SNCLK1	152
#define HI3660_CLK_GATE_ISP_SNCLK2	153
#define HI3660_CLK_ANGT_ISP_SNCLK	154
#define HI3660_CLK_MUX_ISP_SNCLK	155
#define HI3660_CLK_DIV_ISP_SNCLK	156

/* clk in pmuctrl */
#define HI3660_GATE_ABB_192		0

/* clk in pctrl */
#define HI3660_GATE_UFS_TCXO_EN		0
#define HI3660_GATE_USB_TCXO_EN		1

/* clk in sctrl */
#define HI3660_PCLK_AO_GPIO0		0
#define HI3660_PCLK_AO_GPIO1		1
#define HI3660_PCLK_AO_GPIO2		2
#define HI3660_PCLK_AO_GPIO3		3
#define HI3660_PCLK_AO_GPIO4		4
#define HI3660_PCLK_AO_GPIO5		5
#define HI3660_PCLK_AO_GPIO6		6
#define HI3660_PCLK_GATE_MMBUF		7
#define HI3660_CLK_GATE_DSS_AXI_MM	8
#define HI3660_PCLK_MMBUF_ANDGT		9
#define HI3660_CLK_MMBUF_PLL_ANDGT	10
#define HI3660_CLK_FLL_MMBUF_ANDGT	11
#define HI3660_CLK_SYS_MMBUF_ANDGT	12
#define HI3660_CLK_GATE_PCIEPHY_GT	13
#define HI3660_ACLK_MUX_MMBUF		14
#define HI3660_CLK_SW_MMBUF		15
#define HI3660_CLK_DIV_AOBUS		16
#define HI3660_PCLK_DIV_MMBUF		17
#define HI3660_ACLK_DIV_MMBUF		18
#define HI3660_CLK_DIV_PCIEPHY		19

/* clk in iomcu */
#define HI3660_CLK_I2C0_IOMCU		0
#define HI3660_CLK_I2C1_IOMCU		1
#define HI3660_CLK_I2C2_IOMCU		2
#define HI3660_CLK_I2C6_IOMCU		3
#define HI3660_CLK_IOMCU_PERI0		4

#endif	/* __DTS_HI3660_CLOCK_H */
