/* $NetBSD: tegra_reg.h,v 1.24 2017/07/21 01:01:22 jmcneill Exp $ */

/*-
 * Copyright (c) 2015 Jared D. McNeill <jmcneill@invisible.ca>
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

#ifndef _ARM_TEGRA_REG_H
#define _ARM_TEGRA_REG_H

#define TEGRA_PCIE_OFFSET	0x01000000
#define TEGRA_PCIE_SIZE		0x3f000000
#define TEGRA_PCIE_RPCONF_BASE	0x01000000
#define TEGRA_PCIE_RPCONF_SIZE	0x00002000
#define TEGRA_PCIE_PADS_BASE	0x01003000
#define TEGRA_PCIE_PADS_SIZE	0x00000800
#define TEGRA_PCIE_AFI_BASE	0x01003800
#define TEGRA_PCIE_AFI_SIZE	0x00000800
#define TEGRA_PCIE_A1_BASE	0x01000000
#define TEGRA_PCIE_A1_SIZE	0x01000000
#define TEGRA_PCIE_A2_BASE	0x02000000
#define TEGRA_PCIE_A2_SIZE	0x0e000000
#define TEGRA_PCIE_A3_BASE	0x10000000
#define TEGRA_PCIE_A3_SIZE	0x30000000

#define TEGRA_PCIE_CONF_BASE	0x02000000
#define TEGRA_PCIE_CONF_SIZE	0x01000000
#define TEGRA_PCIE_IO_BASE	0x01800000	/* comment in tegra_pcie.c */
#define TEGRA_PCIE_IO_SIZE	0x00800000
#define TEGRA_PCIE_MEM_BASE	0x03000000
#define TEGRA_PCIE_MEM_SIZE	0x0d000000
#define TEGRA_PCIE_EXTC_BASE	0x10000000
#define TEGRA_PCIE_EXTC_SIZE	0x10000000
#define TEGRA_PCIE_PMEM_BASE	0x20000000
#define TEGRA_PCIE_PMEM_SIZE	0x20000000

#define TEGRA_HOST1X_BASE	0x50000000
#define TEGRA_HOST1X_SIZE	0x00034000
#define TEGRA_GHOST_BASE	0x54000000
#define TEGRA_GHOST_SIZE	0x01000000
#define TEGRA_GPU_BASE		0x57000000
#define TEGRA_GPU_SIZE		0x02000000
#define TEGRA_PPSB_BASE		0x60000000
#define TEGRA_PPSB_SIZE		0x01000000
#define TEGRA_APB_BASE		0x70000000
#define TEGRA_APB_SIZE		0x01000000
#define TEGRA_AHB_A2_BASE	0x7c000000
#define TEGRA_AHB_A2_SIZE	0x02000000

#define TEGRA_HOST1X_VBASE	0xfaf00000
#define TEGRA_PPSB_VBASE	0xfb000000
#define TEGRA_APB_VBASE		0xfc000000
#define TEGRA_AHB_A2_VBASE	0xfd000000

/* APB */
#define TEGRA_MPIO_OFFSET	0x00000000
#define TEGRA_MPIO_SIZE		0x4000
#define TEGRA_UARTA_OFFSET	0x00006000
#define TEGRA_UARTA_SIZE	0x40
#define TEGRA_UARTB_OFFSET	0x00006040
#define TEGRA_UARTB_SIZE	0x40
#define TEGRA_UARTC_OFFSET	0x00006200
#define TEGRA_UARTC_SIZE	0x100
#define TEGRA_UARTD_OFFSET	0x00006300
#define TEGRA_UARTD_SIZE	0x100
#define TEGRA_I2C1_OFFSET	0x0000c000
#define TEGRA_I2C1_SIZE		0x100
#define TEGRA_I2C2_OFFSET	0x0000c400
#define TEGRA_I2C2_SIZE		0x100
#define TEGRA_I2C3_OFFSET	0x0000c500
#define TEGRA_I2C3_SIZE		0x100
#define TEGRA_I2C4_OFFSET	0x0000c700
#define TEGRA_I2C4_SIZE		0x100
#define TEGRA_I2C5_OFFSET	0x0000d000
#define TEGRA_I2C5_SIZE		0x100
#define TEGRA_I2C6_OFFSET	0x0000d100
#define TEGRA_I2C6_SIZE		0x100
#define TEGRA_RTC_OFFSET	0x0000e000
#define TEGRA_RTC_SIZE		0x100
#define TEGRA_KBC_OFFSET	0x0000e200
#define TEGRA_KBC_SIZE		0x100
#define TEGRA_PMC_OFFSET	0x0000e400
#define TEGRA_PMC_SIZE		0x800
#define TEGRA_FUSE_OFFSET	0x0000f800
#define TEGRA_FUSE_SIZE		0x00000400
#define TEGRA_CEC_OFFSET	0x00015000
#define TEGRA_CEC_SIZE		0x1000
#define TEGRA_MC_OFFSET		0x00019000
#define TEGRA_MC_SIZE		0x1000
#define TEGRA_SATA_OFFSET	0x00020000
#define TEGRA_SATA_SIZE		0x10000
#define TEGRA_HDA_OFFSET	0x00030000
#define TEGRA_HDA_SIZE		0x10000
#define TEGRA_XUSB_PADCTL_OFFSET 0x0009f000
#define TEGRA_XUSB_PADCTL_SIZE	0x1000
#define TEGRA_XUSB_HOST_OFFSET	0x00090000
#define TEGRA_XUSB_HOST_SIZE	0xa000
#define TEGRA_SDMMC1_OFFSET	0x000b0000
#define TEGRA_SDMMC1_SIZE	0x200
#define TEGRA_SDMMC2_OFFSET	0x000b0200
#define TEGRA_SDMMC2_SIZE	0x200
#define TEGRA_SDMMC3_OFFSET	0x000b0400
#define TEGRA_SDMMC3_SIZE	0x200
#define TEGRA_SDMMC4_OFFSET	0x000b0600
#define TEGRA_SDMMC4_SIZE	0x200
#define TEGRA_XUSB_DEV_OFFSET	0x000d0000
#define TEGRA_XUSB_DEV_SIZE	0xa000
#define TEGRA_SOC_THERM_OFFSET	0x000e2000
#define TEGRA_SOC_THERM_SIZE	0x1000

/* PPSB */
#define TEGRA_TIMER_OFFSET	0x00005000
#define TEGRA_TIMER_SIZE	0x400
#define TEGRA_CAR_OFFSET	0x00006000
#define TEGRA_CAR_SIZE		0x1000
#define TEGRA_GPIO_OFFSET	0x0000d000
#define TEGRA_GPIO_SIZE		0x00000800
#define TEGRA_EVP_OFFSET	0x0000f000
#define TEGRA_EVP_SIZE		0x1000

/* AHB_A2 */
#define TEGRA_USB1_OFFSET	0x01000000
#define TEGRA_USB1_SIZE		0x1800
#define TEGRA_USB2_OFFSET	0x01004000
#define TEGRA_USB2_SIZE		0x1800
#define TEGRA_USB3_OFFSET	0x01008000
#define TEGRA_USB3_SIZE		0x1800

/* Graphics Host (GHOST) */
#define TEGRA_DISPLAYA_OFFSET	0x00200000
#define TEGRA_DISPLAYA_SIZE	0x00040000
#define TEGRA_DISPLAYB_OFFSET	0x00240000
#define TEGRA_DISPLAYB_SIZE	0x00040000
#define TEGRA_HDMI_OFFSET	0x00280000
#define TEGRA_HDMI_SIZE		0x00040000
#define TEGRA_SOR_OFFSET	0x00540000
#define TEGRA_SOR_SIZE		0x00040000
#define TEGRA_DPAUX_OFFSET	0x005c0000
#define TEGRA_DPAUX_SIZE	0x00040000

#endif /* _ARM_TEGRA_REG_H */
