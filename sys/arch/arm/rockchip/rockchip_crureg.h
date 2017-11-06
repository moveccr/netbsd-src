/* $NetBSD: rockchip_crureg.h,v 1.8 2015/01/17 15:05:24 jmcneill Exp $ */

/*-
 * Copyright (c) 2014 Jared D. McNeill <jmcneill@invisible.ca>
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

#ifndef _ROCKCHIP_CRUREG_H
#define _ROCKCHIP_CRUREG_H

#define CRU_APLL_CON0_REG	0x0000
#define CRU_APLL_CON1_REG	0x0004
#define CRU_APLL_CON2_REG	0x0008
#define CRU_APLL_CON3_REG	0x000c
#define CRU_DPLL_CON0_REG	0x0010
#define CRU_DPLL_CON1_REG	0x0014
#define CRU_DPLL_CON2_REG	0x0018
#define CRU_DPLL_CON3_REG	0x001c
#define CRU_CPLL_CON0_REG	0x0020
#define CRU_CPLL_CON1_REG	0x0024
#define CRU_CPLL_CON2_REG	0x0028
#define CRU_CPLL_CON3_REG	0x002c
#define CRU_GPLL_CON0_REG	0x0030
#define CRU_GPLL_CON1_REG	0x0034
#define CRU_GPLL_CON2_REG	0x0038
#define CRU_GPLL_CON3_REG	0x003c
#define CRU_MODE_CON_REG	0x0040
#define CRU_CLKSEL_CON_REG(n)	(0x0044 + (n) * 4)
#define CRU_CLKGATE_CON_REG(n)	(0x00d0 + (n) * 4)
#define CRU_GLB_SRST_FST_REG	0x0100
#define CRU_GLB_SRST_SND_REG	0x0104
#define CRU_SOFTRST_REG(n)	(0x0110 + (n) * 4)
#define CRU_MISC_CON_REG	0x0134
#define CRU_GLB_CNT_TH_REG	0x0140

#define CRU_PLL_CON0_CLKR_MASK		__BITS(29,24)
#define CRU_PLL_CON0_CLKOD_MASK		__BITS(18,16)
#define CRU_PLL_CON0_CLKR		__BITS(13,8)
#define CRU_PLL_CON0_CLKOD		__BITS(3,0)
#define RK3188_CRU_PLL_CON0_CLKOD_MASK	__BITS(21,16)
#define RK3188_CRU_PLL_CON0_CLKOD	__BITS(5,0)

#define CRU_PLL_CON1_CLKF_MASK		__BITS(28,16)
#define CRU_PLL_CON1_CLKF		__BITS(12,0)
#define RK3188_CRU_PLL_CON1_CLKF_MASK	__BITS(31,16)
#define RK3188_CRU_PLL_CON1_CLKF	__BITS(15,0)

#define CRU_PLL_CON2_BWADJ_MASK		__BITS(27,16)
#define CRU_PLL_CON2_BWADJ		__BITS(11,0)

#define CRU_PLL_CON3_RESET_MASK		__BIT(21)
#define CRU_PLL_CON3_POWER_DOWN_MASK	__BIT(17)
#define CRU_PLL_CON3_RESET		__BIT(5)
#define CRU_PLL_CON3_POWER_DOWN		__BIT(1)

#define CRU_MODE_CON_APLL_WORK_MODE_MASK	__BITS(17,16)
#define CRU_MODE_CON_APLL_WORK_MODE		__BITS(1,0)
#define CRU_MODE_CON_APLL_WORK_MODE_SLOW	0
#define CRU_MODE_CON_APLL_WORK_MODE_NORMAL	1
#define CRU_MODE_CON_APLL_WORK_MODE_DEEP_SLOW	2

#define CRU_CLKSEL_CON0_CPU_CLK_PLL_SEL_MASK	__BIT(24)
#define CRU_CLKSEL_CON0_CORE_PERI_DIV_CON_MASK	__BITS(23,22)
#define CRU_CLKSEL_CON0_A9_CORE_DIV_CON_MASK	__BITS(20,16)
#define CRU_CLKSEL_CON0_CPU_CLK_PLL_SEL		__BIT(8)
#define CRU_CLKSEL_CON0_CORE_PERI_DIV_CON	__BITS(7,6)
#define CRU_CLKSEL_CON0_A9_CORE_DIV_CON		__BITS(4,0)
#define RK3188_CRU_CLKSEL_CON0_A9_CORE_DIV_CON_MASK __BITS(29,25)
#define RK3188_CRU_CLKSEL_CON0_A9_CORE_DIV_CON	__BITS(13,9)

#define CRU_CLKSEL_CON1_AHB2APB_PCLKEN_DIV_CON_MASK __BITS(31,30)
#define CRU_CLKSEL_CON1_CPU_PCLK_DIV_CON_MASK	__BITS(29,28)
#define CRU_CLKSEL_CON1_CPU_HCLK_DIV_CON_MASK	__BITS(25,24)
#define CRU_CLKSEL_CON1_CPU_ACLK_DIV_CON_MASK	__BITS(18,16)
#define CRU_CLKSEL_CON1_AHB2APB_PCLKEN_DIV_CON	__BITS(15,14)
#define CRU_CLKSEL_CON1_CPU_PCLK_DIV_CON	__BITS(13,12)
#define CRU_CLKSEL_CON1_CPU_HCLK_DIV_CON	__BITS(9,8)
#define CRU_CLKSEL_CON1_CPU_ACLK_DIV_CON	__BITS(2,0)
#define RK3188_CRU_CLKSEL_CON1_CPU_ACLK_DIV_CON_MASK __BITS(21,19)
#define RK3188_CRU_CLKSEL_CON1_CPU_ACLK_DIV_CON	__BITS(5,3)

#define CRU_CLKSEL_CON10_PERI_PLL_SEL_MASK	__BIT(31)
#define CRU_CLKSEL_CON10_PERI_PCLK_DIV_CON_MASK	__BITS(29,28)
#define CRU_CLKSEL_CON10_PERI_HCLK_DIV_CON_MASK	__BITS(25,24)
#define CRU_CLKSEL_CON10_PERI_ACLK_DIV_CON_MASK	__BITS(20,16)
#define CRU_CLKSEL_CON10_PERI_PLL_SEL		__BIT(15)
#define CRU_CLKSEL_CON10_PERI_PCLK_DIV_CON	__BITS(13,12)
#define CRU_CLKSEL_CON10_PERI_HCLK_DIV_CON	__BITS(9,8)
#define CRU_CLKSEL_CON10_PERI_ACLK_DIV_CON	__BITS(4,0)

#define CRU_CLKSEL_CON11_MMC0_DIV_CON_MASK	__BITS(21,16)
#define CRU_CLKSEL_CON11_MMC0_DIV_CON		__BITS(5,0)

#define CRU_CLKSEL_CON13_UART0_CLK_SEL_MASK	__BITS(25,24)
#define CRU_CLKSEL_CON13_UART0_DIV_CON_MASK	__BITS(22,16)
#define CRU_CLKSEL_CON13_UART0_CLK_SEL		__BITS(9,8)
#define CRU_CLKSEL_CON13_UART0_DIV_CON		__BITS(6,0)

#define CRU_CLKSEL_CON21_MAC_DIV_CON_MASK	__BITS(28,24)
#define CRU_CLKSEL_CON21_RMII_EXTCLK_SEL_MASK	__BIT(20)
#define CRU_CLKSEL_CON21_MAC_PLL_SEL_MASK	__BIT(16)
#define CRU_CLKSEL_CON21_MAC_DIV_CON		__BITS(12,8)
#define CRU_CLKSEL_CON21_RMII_EXTCLK_SEL	__BIT(4)
#define CRU_CLKSEL_CON21_MAC_PLL_SEL		__BIT(0)

#define CRU_GLB_SRST_FST_MAGIC	0xfdb9

#endif /* !_ROCKCHIP_CRUREG_H */
