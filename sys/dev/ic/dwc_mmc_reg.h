/* $NetBSD: dwc_mmc_reg.h,v 1.9 2020/01/22 23:19:12 jmcneill Exp $ */

/*-
 * Copyright (c) 2014-2017 Jared McNeill <jmcneill@invisible.ca>
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

#ifndef _DWC_MMC_REG_H
#define _DWC_MMC_REG_H

#define DWC_MMC_GCTRL			0x0000
#define DWC_MMC_PWREN			0x0004
#define	DWC_MMC_CLKDIV			0x0008
#define	DWC_MMC_CLKSRC			0x000c
#define	DWC_MMC_CLKENA			0x0010
#define DWC_MMC_TIMEOUT			0x0014
#define DWC_MMC_WIDTH			0x0018
#define DWC_MMC_BLKSZ			0x001c
#define DWC_MMC_BYTECNT			0x0020
#define DWC_MMC_IMASK			0x0024
#define DWC_MMC_ARG			0x0028
#define DWC_MMC_CMD			0x002c
#define DWC_MMC_RESP0			0x0030
#define DWC_MMC_RESP1			0x0034
#define DWC_MMC_RESP2			0x0038
#define DWC_MMC_RESP3			0x003c
#define DWC_MMC_MINT			0x0040
#define DWC_MMC_RINT			0x0044
#define DWC_MMC_STATUS			0x0048
#define DWC_MMC_FIFOTH			0x004c
#define	DWC_MMC_CDETECT			0x0050
#define	DWC_MMC_WRITEPROT		0x0054
#define	DWC_MMC_GPIO			0x0058
#define DWC_MMC_CBCR			0x005c
#define DWC_MMC_BBCR			0x0060
#define	DWC_MMC_DEBNCE			0x0064
#define	DWC_MMC_USRID			0x0068
#define	DWC_MMC_VERID			0x006c
#define	DWC_MMC_HCON			0x0070
#define	DWC_MMC_UHS			0x0074
#define	DWC_MMC_RST			0x0078
#define DWC_MMC_DMAC			0x0080
#define	DWC_MMC_PLDMND			0x0084
#define DWC_MMC_DLBA			0x0088
#define DWC_MMC_IDST			0x008c
#define DWC_MMC_IDIE			0x0090
#define DWC_MMC_DSCADDR			0x0094
#define DWC_MMC_BUFADDR			0x0098
#define DWC_MMC_CARDTHRCTL		0x0100

#define DWC_MMC_GCTRL_USE_INTERNAL_DMAC	__BIT(25)
#define DWC_MMC_GCTRL_SEND_AUTO_STOP_CCSD __BIT(10)
#define DWC_MMC_GCTRL_DMAEN		__BIT(5)
#define DWC_MMC_GCTRL_INTEN		__BIT(4)
#define DWC_MMC_GCTRL_DMARESET		__BIT(2)
#define DWC_MMC_GCTRL_FIFORESET		__BIT(1)
#define DWC_MMC_GCTRL_SOFTRESET		__BIT(0)
#define DWC_MMC_GCTRL_RESET \
	(DWC_MMC_GCTRL_SOFTRESET | DWC_MMC_GCTRL_FIFORESET | \
	 DWC_MMC_GCTRL_DMARESET)

#define DWC_MMC_CLKENA_LOWPOWERON	__BIT(16)
#define DWC_MMC_CLKENA_CARDCLKON	__BIT(0)

#define DWC_MMC_WIDTH_1		0x00000000
#define DWC_MMC_WIDTH_4		0x00000001
#define DWC_MMC_WIDTH_8		0x00010000

#define DWC_MMC_CMD_START		__BIT(31)
#define DWC_MMC_CMD_USE_HOLD_REG	__BIT(29)
#define DWC_MMC_CMD_VOL_SWITCH		__BIT(28)
#define DWC_MMC_CMD_BOOT_MODE		__BIT(27)
#define DWC_MMC_CMD_DISABLE_BOOT	__BIT(26)
#define DWC_MMC_CMD_EXPECT_BOOT_ACT	__BIT(25)
#define DWC_MMC_CMD_ENABLE_BOOT		__BIT(24)
#define DWC_MMC_CMD_UPCLK_ONLY		__BIT(21)
#define DWC_MMC_CMD_SEND_INIT_SEQ	__BIT(15)
#define DWC_MMC_CMD_STOP_ABORT_CMD	__BIT(14)
#define DWC_MMC_CMD_WAIT_PRE_OVER	__BIT(13)
#define DWC_MMC_CMD_SEND_AUTO_STOP	__BIT(12)
#define DWC_MMC_CMD_SEQMOD		__BIT(11)
#define DWC_MMC_CMD_WRITE		__BIT(10)
#define DWC_MMC_CMD_DATA_EXP		__BIT(9)
#define DWC_MMC_CMD_CHECK_RSP_CRC	__BIT(8)
#define DWC_MMC_CMD_LONG_RSP		__BIT(7)
#define DWC_MMC_CMD_RSP_EXP		__BIT(6)

#define DWC_MMC_INT_CARD_REMOVE		__BIT(31)
#define DWC_MMC_INT_CARD_INSERT		__BIT(30)
#define DWC_MMC_INT_SDIO_INT(n)		__BIT(16 + (n))
#define DWC_MMC_INT_END_BIT_ERR		__BIT(15)
#define DWC_MMC_INT_AUTO_CMD_DONE	__BIT(14)
#define DWC_MMC_INT_START_BIT_ERR	__BIT(13)
#define DWC_MMC_INT_HW_LOCKED		__BIT(12)
#define DWC_MMC_INT_FIFO_RUN_ERR	__BIT(11)
#define DWC_MMC_INT_VOL_CHG_DONE	__BIT(10)
#define DWC_MMC_INT_DATA_STARVE		__BIT(10)
#define DWC_MMC_INT_BOOT_START		__BIT(9)
#define DWC_MMC_INT_DATA_TIMEOUT	__BIT(9)
#define DWC_MMC_INT_ACK_RCV		__BIT(8)
#define DWC_MMC_INT_RESP_TIMEOUT	__BIT(8)
#define DWC_MMC_INT_DATA_CRC_ERR	__BIT(7)
#define DWC_MMC_INT_RESP_CRC_ERR	__BIT(6)
#define DWC_MMC_INT_RX_DATA_REQ		__BIT(5)
#define DWC_MMC_INT_TX_DATA_REQ		__BIT(4)
#define DWC_MMC_INT_DATA_OVER		__BIT(3)
#define DWC_MMC_INT_CMD_DONE		__BIT(2)
#define DWC_MMC_INT_RESP_ERR		__BIT(1)
#define DWC_MMC_INT_ERROR \
	(DWC_MMC_INT_RESP_ERR | DWC_MMC_INT_RESP_CRC_ERR | \
	 DWC_MMC_INT_DATA_CRC_ERR | DWC_MMC_INT_RESP_TIMEOUT | \
	 DWC_MMC_INT_FIFO_RUN_ERR | DWC_MMC_INT_HW_LOCKED | \
	 DWC_MMC_INT_START_BIT_ERR  | DWC_MMC_INT_END_BIT_ERR)

#define DWC_MMC_STATUS_DMAREQ		__BIT(31)
#define DWC_MMC_STATUS_DATA_FSM_BUSY	__BIT(10)
#define DWC_MMC_STATUS_CARD_DATA_BUSY	__BIT(9)
#define DWC_MMC_STATUS_CARD_PRESENT	__BIT(8)
#define DWC_MMC_STATUS_FIFO_FULL	__BIT(3)
#define DWC_MMC_STATUS_FIFO_EMPTY	__BIT(2)
#define DWC_MMC_STATUS_TXWL_FLAG	__BIT(1)
#define DWC_MMC_STATUS_RXWL_FLAG	__BIT(0)

#define	DWC_MMC_FIFOTH_DMA_MULTIPLE_TXN_SIZE __BITS(30,28)
#define	DWC_MMC_FIFOTH_DMA_MULTIPLE_TXN_SIZE_16	3
#define	DWC_MMC_FIFOTH_RX_WMARK		__BITS(27,16)
#define	DWC_MMC_FIFOTH_TX_WMARK		__BITS(11,0)

#define DWC_MMC_DMAC_IDMA_ON		__BIT(7)
#define DWC_MMC_DMAC_FIX_BURST		__BIT(1)
#define DWC_MMC_DMAC_SOFTRESET		__BIT(0)

#define DWC_MMC_VERID_ID		__BITS(15,0)
#define DWC_MMC_VERID_240A		0x240a

#define DWC_MMC_IDST_HOST_ABT		__BIT(10)
#define DWC_MMC_IDST_ABNORMAL_INT_SUM	__BIT(9)
#define DWC_MMC_IDST_NORMAL_INT_SUM	__BIT(8)
#define DWC_MMC_IDST_CARD_ERR_SUM	__BIT(5)
#define DWC_MMC_IDST_DES_INVALID	__BIT(4)
#define DWC_MMC_IDST_FATAL_BUS_ERR	__BIT(2)
#define DWC_MMC_IDST_RECEIVE_INT	__BIT(1)
#define DWC_MMC_IDST_TRANSMIT_INT	__BIT(0)
#define DWC_MMC_IDST_ERROR \
	(DWC_MMC_IDST_ABNORMAL_INT_SUM | DWC_MMC_IDST_CARD_ERR_SUM | \
	 DWC_MMC_IDST_DES_INVALID | DWC_MMC_IDST_FATAL_BUS_ERR)
#define DWC_MMC_IDST_COMPLETE \
	(DWC_MMC_IDST_RECEIVE_INT | DWC_MMC_IDST_TRANSMIT_INT)

#define DWC_MMC_CARDTHRCTL_RDTHR	__BITS(27,16)
#define DWC_MMC_CARDTHRCTL_RDTHREN	__BIT(0)

struct dwc_mmc_idma_desc {
	uint32_t		dma_config;
#define DWC_MMC_IDMA_CONFIG_DIC		__BIT(1)
#define DWC_MMC_IDMA_CONFIG_LD		__BIT(2)
#define DWC_MMC_IDMA_CONFIG_FD		__BIT(3)
#define DWC_MMC_IDMA_CONFIG_CH		__BIT(4)
#define DWC_MMC_IDMA_CONFIG_ER		__BIT(5)
#define DWC_MMC_IDMA_CONFIG_CES		__BIT(30)
#define DWC_MMC_IDMA_CONFIG_OWN		__BIT(31)
	uint32_t		dma_buf_size;
	uint32_t		dma_buf_addr;
	uint32_t		dma_next;
} __packed;

#endif /* !_DWC_MMC_REG_H */
