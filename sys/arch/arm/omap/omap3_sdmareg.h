/*	$NetBSD: omap3_sdmareg.h,v 1.6 2020/04/16 23:29:52 rin Exp $ */

/*
 * Copyright (c) 2012 Michael Lorenz
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

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: omap3_sdmareg.h,v 1.6 2020/04/16 23:29:52 rin Exp $");

#ifndef OMAPDMA_REG_H
#define OMAPDMA_REG_H

/*
 * all register offsets are relative to OMAP3530_SDMA_BASE
 */
 
#define OMAPDMA_REVISION	0x00000000

/* each bit corresponds to a DMA channel, write 1 to clear */
#define OMAPDMA_IRQSTATUS_L0	0x00000008
#define OMAPDMA_IRQSTATUS_L1	0x0000000c
#define OMAPDMA_IRQSTATUS_L2	0x00000010
#define OMAPDMA_IRQSTATUS_L3	0x00000014

#define OMAPDMA_IRQENABLE_L0	0x00000018
#define OMAPDMA_IRQENABLE_L1	0x0000001c
#define OMAPDMA_IRQENABLE_L2	0x00000020
#define OMAPDMA_IRQENABLE_L3	0x00000024

#define OMAPDMA_SYSSTATUS	0x00000028
	#define OMAPDMA_RESET_DONE	0x00000001

#define OMAPDMA_SYSCONFIG	0x0000002c
	#define OMAPDMA_IDLEMODE_MASK		0x00003000
	#define OMAPDMA_IDLEMODE_FORCE_STANDBY	0x00000000
	#define OMAPDMA_IDLEMODE_NO_STANDBY	0x00001000
	#define OMAPDMA_IDLEMODE_SMART_STANDBY	0x00002000
	#define OMAPDMA_CLKACT_MASK		0x00000300
	#define OMAPDMA_EMUFREE			0x00000020
	#define OMAPDMA_SIDLEMODE_MASK		0x00000018
	#define OMAPDMA_FORCE_IDLE		0x00000000
	#define OMAPDMA_NO_IDLE			0x00000008
	#define OMAPDMA_SMART_IDLE		0x00000010
	#define OMAPDMA_SOFTRESET		0x00000002
	#define OMAPDMA_AUTOIDLE		0x00000001

/* capability registers */
#define OMAPDMA_CAPS_0		0x00000064
	#define OMAPDMA_CAP_FILL	0x00080000	/* fill support */
	#define OMAPDMA_CAP_TRANS_BLT	0x00040000	/* transparent blit */
#define OMAPDMA_CAPS_2		0x0000006c
#define OMAPDMA_CAPS_3		0x00000070
#define OMAPDMA_CAPS_4		0x00000074
#define OMAPDMA_GCR		0x00000078
	#define OMAPDMA_GCR_ARB_RATE_MASK	0x00ff0000
	#define OMAPDMA_GCR_ARB_RATE_SHIFT	16
	#define OMAPDMA_GCR_FIFO_BUDGET_NONE	0x00000000
	#define OMAPDMA_GCR_FIFO_BUDGET_75L	0x00004000
	#define OMAPDMA_GCR_FIFO_BUDGET_25L	0x00008000
	#define OMAPDMA_GCR_FIFO_BUDGET_50L	0x0000c000
	#define OMAPDMA_GCR_HI_THREAD_NONE	0x00000000
	#define OMAPDMA_GCR_HI_THREAD_0		0x00001000
	#define OMAPDMA_GCR_HI_THREAD_01	0x00002000
	#define OMAPDMA_GCR_HI_THREAD_012	0x00003000
	#define OMAPDMA_GCR_MAX_FIFO_DEPTH_MASK	0x000000ff


/*
 * there are 32 channels, each occupies a 0x60 register space starting at
 * 0x00000080 + 0x60 * c for c = 0 .. 31
 */
#define OMAPDMA_CHANNEL_BASE	0x00000080

#define OMAPDMAC_CCR		0x00000000
	#define CCR_WRITE_PRIORITY		0x04000000
	#define CCR_BUFFERING_DISABLE		0x02000000
	#define CCR_SEL_SRC_DST_SYNC		0x01000000
	#define CCR_PREFETCH			0x00800000
	#define CCR_SUPERVISOR			0x00400000
	#define CCR_SYNC_CONTROL_UPPER_MASK	0x00180000
	#define CCR_BLOCK_SYNC			0x00040000
	#define CCR_TRANSPARENT_COPY_ENABLE	0x00020000
	#define CCR_CONST_FILL_ENABLE		0x00010000
	#define CCR_DST_AMODE_MASK		0x0000c000
	#define CCR_DST_AMODE_CONST_ADDR	0x00000000
	#define CCR_DST_AMODE_POST_INCR		0x00004000
	#define CCR_DST_AMODE_SINGLE_INDEX	0x00008000
	#define CCR_DST_AMODE_DOUBLE_INDEX	0x0000c000
	#define CCR_SRC_AMODE_MASK		0x00003000
	#define CCR_SRC_AMODE_CONST_ADDR	0x00000000
	#define CCR_SRC_AMODE_POST_INCR		0x00001000
	#define CCR_SRC_AMODE_SINGLE_INDEX	0x00002000
	#define CCR_SRC_AMODE_DOUBLE_INDEX	0x00003000
	#define CCR_WR_ACTIVE			0x00000400
	#define CCR_RD_ACTIVE			0x00000200
	#define CCR_SUSPEND_SENSITIVE		0x00000100
	#define CCR_ENABLE			0x00000080
	#define CCR_READ_PRIORITY		0x00000040
	#define CCR_FRAME_SYNC			0x00000020
	#define CCR_SYNCHRO_CONTROL_MASK	0x0000001f
#define OMAPDMAC_CLNK_CTRL	0x00000004
	#define CLNK_ENABLE_LNK			0x00008000
	#define CLNK_NEXT_CHANNEL_MASK		0x0000001f
#define OMAPDMAC_CICRI		0x00000008
	#define CICRI_DRAIN_IE			0x00001000
	#define CICRI_MISALIGNED_ERR_IE		0x00000800
	#define CICRI_SUPERVISOR_ERR_IE		0x00000400
	#define CICRI_TRANSFER_ERR_IE		0x00000100
	#define CICRI_PKT_IE			0x00000080
	#define CICRI_BLOCK_IE			0x00000020
	#define CICRI_LAST_IE			0x00000010
	#define CICRI_FRAME_IE			0x00000008
	#define CICRI_HALF_IE			0x00000004
	#define CICRI_DROP_IE			0x00000002
#define OMAPDMAC_CSR		0x0000000c
	#define CSR_DRAIN_END			0x00001000
	#define CSR_MISALIGNED_ADRS_ERR		0x00000800
	#define CSR_SUPERVISOR_ERR		0x00000400
	#define CSR_TRANS_ERR			0x00000100
	#define CSR_PKT				0x00000080
	#define CSR_SYNC			0x00000040
	#define CSR_BLOCK			0x00000020
	#define CSR_LAST			0x00000010
	#define CSR_FRAME			0x00000008
	#define CSR_HALF			0x00000004
	#define CSR_DROP			0x00000002
#define OMAPDMAC_CSDPI		0x00000010
	#define CSDPI_SRC_ENDIAN		0x00100000
	#define CSDPI_SRC_ENDIAN_LOCK		0x00080000
	#define CSDPI_DST_ENDIAN		0x00040000
	#define CSDPI_DST_ENDIAN_LOCK		0x00020000
	#define CSDPI_WRITE_MODE_MASK		0x00018000
	#define CSDPI_WRITE_NONPOSTED		0x00000000
	#define CSDPI_WRITE_POSTED		0x00008000
	#define CSDPI_WRITE_POSTED_EXCEPT_LAST	0x00010000
	#define CSDPI_DST_BURST_MASK		0x0000c000
	#define CSDPI_DST_BURST_SINGLE		0x00000000
	#define CSDPI_DST_BURST_16		0x00004000
	#define CSDPI_DST_BURST_32		0x00008000
	#define CSDPI_DST_BURST_64		0x0000c000
	#define CSDPI_DST_PACKED		0x00002000
	#define CSDPI_SRC_BURST_MASK		0x00000180
	#define CSDPI_SRC_BURST_SINGLE		0x00000000
	#define CSDPI_SRC_BURST_16		0x00000080
	#define CSDPI_SRC_BURST_32		0x00000100
	#define CSDPI_SRC_BURST_64		0x00000180
	#define CSDPI_SRC_PACKED		0x00000040
	#define CSDPI_DATA_TYPE_MASK		0x00000003
	#define CSDPI_DATA_TYPE_8		0x00000000
	#define CSDPI_DATA_TYPE_16		0x00000001
	#define CSDPI_DATA_TYPE_32		0x00000002
#define OMAPDMAC_CEN		0x00000014
#define OMAPDMAC_CFN		0x00000018
#define OMAPDMAC_CSSA		0x0000001c
#define OMAPDMAC_CDSA		0x00000020
#define OMAPDMAC_CSEI		0x00000024
#define OMAPDMAC_CSFI		0x00000028
#define OMAPDMAC_CDEI		0x0000002c
#define OMAPDMAC_CDFI		0x00000030
#define OMAPDMAC_CSAC		0x00000034
#define OMAPDMAC_CDAC		0x00000038
#define OMAPDMAC_CCEN		0x0000003c
#define OMAPDMAC_CCFN		0x00000040
#define OMAPDMAC_COLOR		0x00000044

#endif /* OMAPDMA_REG_H */
