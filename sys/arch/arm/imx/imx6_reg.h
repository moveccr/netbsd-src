/*	$NetBSD: imx6_reg.h,v 1.8 2018/06/20 07:05:37 hkenken Exp $	*/
/*-
 * Copyright (c) 2012 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Matt Thomas of 3am Software Foundry.
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ARM_IMX_IMX6_REG_H_
#define _ARM_IMX_IMX6_REG_H_

#define	IMX6_IOREG_PBASE	IMX6_AIPS1_BASE
#define	IMX6_IOREG_SIZE		(IMX6_AIPS1_SIZE + IMX6_AIPS2_SIZE)

#define	IMX6_ARMCORE_PBASE	IMX6_MPCORE_BASE
#define	IMX6_ARMCORE_SIZE	IMX6_MPCORE_SIZE

#define	IMX6_IO_SIZE		(IMX6_IOREG_SIZE + IMX6_ARMCORE_SIZE)

#define	ARMCORE_SCU_BASE	0x00000000
#define	ARMCORE_L2C_BASE	0x00002000

#define	IMX6_MEM_BASE		0x10000000
#define	IMX6_MEM_SIZE		0xF0000000

#define	IMX6_CS0_BASE		0x08000000
#define	IMX6_CS0_SIZE		0x08000000

#define	IMX6_IPU2_BASE		0x02a00000
#define	IMX6_IPU1_BASE		0x02600000
#define	IMX6_IPU_SIZE		0x00400000

#define	IMX6_HSI_BASE		0x02208000
#define	IMX6_HSI_SIZE		0x00004000

#define	IMX6_OPENVG_BASE	0x02204000
#define	IMX6_OPENVG_SIZE	0x00004000

#define	IMX6_SATA_BASE		0x02200000
#define	IMX6_SATA_SIZE		0x00004000

#define	IMX6_AIPS2_BASE		0x02100000
#define	IMX6_AIPS2_SIZE		0x00100000

#define	IMX6_AIPS1_BASE		0x02000000
#define	IMX6_AIPS1_SIZE		0x00100000

#define	IMX6_PCIE_BASE		0x01ffc000
#define	IMX6_PCIE_SIZE		0x00004000

#define	IMX6_PCIEMEM_BASE	0x01000000
#define	IMX6_PCIEMEM_SIZE	0x00ffc000

#define	IMX6_GPV1_BASE		0x00c00000
#define	IMX6_GPV1_SIZE		0x00100000

#define	IMX6_GPV0_BASE		0x00b00000
#define	IMX6_GPV0_SIZE		0x00100000

#define	IMX6_L2CC_BASE		0x00a02000
#define	IMX6_L2CC_SIZE		0x00001000

#define	IMX6_MPCORE_BASE	0x00a00000
#define	IMX6_MPCORE_SIZE	0x00100000

#define	IMX6_OCRAM1_BASE	0x00940000
#define	IMX6_OCRAM1_SIZE	0x000c0000

#define	IMX6_OCRAM0_BASE	0x00900000
#define	IMX6_OCRAM0_SIZE	0x00040000

#define	IMX6_GPV4_BASE		0x00800000
#define	IMX6_GPV4_SIZE		0x00100000

#define	IMX6_GPV3_BASE		0x00300000
#define	IMX6_GPV3_SIZE		0x00100000

#define	IMX6_GPV2_BASE		0x00200000
#define	IMX6_GPV2_SIZE		0x00100000

#define	IMX6_DTPC_BASE		0x00138000
#define	IMX6_DTPC_SIZE		0x00004000

#define	IMX6_GPU2D_BASE		0x00134000
#define	IMX6_GPU2D_SIZE		0x00004000

#define	IMX6_GPU3D_BASE		0x00130000
#define	IMX6_GPU3D_SIZE		0x00004000

#define	IMX6_HDMI_BASE		0x00120000
#define	IMX6_HDMI_SIZE		0x00009000

#define	IMX6_APBHDMA_BASE	0x00110000
#define	IMX6_APBHDMA_SIZE	0x00002000

#define	IMX6_GPMI_BASE		0x00112000
#define	IMX6_GPMI_SIZE		0x00002000

#define	IMX6_BCH_BASE		0x00114000
#define	IMX6_BCH_SIZE		0x00004000

#define	IMX6_CAAM_BASE		0x00100000
#define	IMX6_CAAM_SIZE		0x00004000

#define	IMX6_ROMCP_BASE		0x00000000
#define	IMX6_ROMCP_SIZE		0x00018000

#define	AIPS1_SDMA_BASE		0x000ec000
#define	AIPS1_DCIC2_BASE	0x000e8000
#define	AIPS1_DCIC1_BASE	0x000e4000
#define	AIPS1_IOMUXC_BASE	0x000e0000
#define	AIPS1_IOMUXC_SIZE	0x00004000
#define	AIPS1_GPC_BASE		0x000dc000
#define	AIPS1_SRC_BASE		0x000d8000
#define	AIPS1_SRC_SIZE		0x00004000
#define	AIPS1_EPIT2_BASE	0x000d4000
#define	AIPS1_EPIT1_BASE	0x000d0000
#define	AIPS1_EPIT_SIZE		0x00000020
#define	AIPS1_SNVS_BASE		0x000cc000
#define	AIPS1_SNVS_SIZE		0x00000c00
#define	AIPS1_USBPHY2_BASE	0x000ca000
#define	AIPS1_USBPHY1_BASE	0x000c9000
#define	AIPS1_USBPHY_SIZE	0x00001000

#define	AIPS1_CCM_ANALOG_BASE	0x000c8000
#define	AIPS1_CCM_ANALOG_SIZE	0x00001000

#define	AIPS1_CCM_BASE		0x000c4000
#define	AIPS1_CCM_SIZE		0x00004000

#define	AIPS1_WDOG2_BASE	0x000c0000
#define	AIPS1_WDOG1_BASE	0x000bc000
#define	AIPS1_WDOG_SIZE		0x00000010
#define	AIPS1_KPP_BASE		0x000b8000
#define	AIPS1_ENET2_BASE	0x000b4000	/* iMX6UL */
#define	AIPS1_GPIO7_BASE	0x000b4000
#define	AIPS1_GPIO6_BASE	0x000b0000
#define	AIPS1_GPIO5_BASE	0x000ac000
#define	AIPS1_GPIO4_BASE	0x000a8000
#define	AIPS1_GPIO3_BASE	0x000a4000
#define	AIPS1_GPIO2_BASE	0x000a0000
#define	AIPS1_GPIO1_BASE	0x0009c000
#define	GPIO_NGROUPS		7
#define	AIPS1_GPT_BASE		0x00098000
#define	AIPS1_CAN2_BASE		0x00094000
#define	AIPS1_CAN1_BASE		0x00090000
#define	AIPS1_PWM4_BASE		0x0008c000
#define	AIPS1_PWM3_BASE		0x00088000
#define	AIPS1_PWM2_BASE		0x00084000
#define	AIPS1_PWM1_BASE		0x00080000
#define	AIPS1_CONFIG_BASE	0x0007c000
#define	AIPS1_VPU_BASE		0x00040000
#define	AIPS1_SPBA_BASE		0x0003c000
#define	AIPS1_ASRC_BASE		0x00034000
#define	AIPS1_SSI3_BASE		0x00030000
#define	AIPS1_SSI2_BASE		0x0002c000
#define	AIPS1_SSI1_BASE		0x00028000
#define	AIPS1_ESAI_BASE		0x00024000
#define	AIPS1_UART1_BASE	0x00020000
#define	AIPS1_UART7_BASE	0x00018000	/* iMX6UL */
#define	AIPS1_ECSPI5_BASE	0x00018000
#define	AIPS1_ECSPI4_BASE	0x00014000
#define	AIPS1_ECSPI3_BASE	0x00010000
#define	AIPS1_ECSPI2_BASE	0x0000c000
#define	AIPS1_ECSPI1_BASE	0x00008000
#define	AIPS1_SPDIF_BASE	0x00004000

#define	AIPS2_UART6_BASE	0x000fc000	/* iMX6UL */
#define	AIPS2_UART5_BASE	0x000f4000
#define	AIPS2_UART4_BASE	0x000f0000
#define	AIPS2_UART3_BASE	0x000ec000
#define	AIPS2_UART2_BASE	0x000e8000
#define	AIPS2_WDOG3_BASE	0x000e4000	/* iMX6UL */
#define	AIPS2_VDOA_BASE		0x000e3000
#define	AIPS2_MIPIDSI_BASE	0x000e0000
#define	AIPS2_MIPICSI_BASE	0x000dc000
#define	AIPS2_AUDMUX_BASE	0x000d8000
#define	AIPS2_TZASC2_BASE	0x000d4000
#define	AIPS2_TZASC1_BASE	0x000d0000
#define	AIPS2_CSU_BASE		0x000c0000
#define	AIPS2_OCOTP_CTRL_BASE	0x000bc000
#define	AIPS2_OCOTP_CTRL_SIZE	0x00000700
#define	AIPS2_WEIM_BASE		0x000b8000
#define	AIPS2_MMDC2_BASE	0x000b4000
#define	AIPS2_MMDC1_BASE	0x000b0000
#define	AIPS2_ROMCP_BASE	0x000ac000

#define	AIPS2_I2C3_BASE		0x000a8000
#define	AIPS2_I2C2_BASE		0x000a4000
#define	AIPS2_I2C1_BASE		0x000a0000
#define	I2C_SIZE		0x4000

#define	AIPS2_USDHC4_BASE	0x0009c000
#define	AIPS2_USDHC3_BASE	0x00098000
#define	AIPS2_USDHC2_BASE	0x00094000
#define	AIPS2_USDHC1_BASE	0x00090000
#define	AIPS2_USDHC_SIZE	0x000000d0
#define	AIPS2_MLB150_BASE	0x0008c000
#define	AIPS2_ENET_BASE		0x00088000
#define	AIPS2_USBOH_BASE	0x00084000
#define	AIPS2_USBOH_SIZE	0x00000820
#define	AIPS2_CONFIG_BASE	0x0007c000
#define	AIPS2_DAP_BASE		0x00040000
#define	AIPS2_CAAM_BASE		0x00000000

#endif /* _ARM_IMX_IMX6_REG_H_ */
