/*	$NetBSD: octeon_pipreg.h,v 1.5 2020/06/23 05:18:02 simonb Exp $	*/

/*
 * Copyright (c) 2007 Internet Initiative Japan, Inc.
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
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * PIP Registers
 */

#ifndef _OCTEON_PIPREG_H_
#define _OCTEON_PIPREG_H_

#define	PIP_BIST_STATUS				0x00011800a0000000ULL
#define	PIP_INT_REG				0x00011800a0000008ULL
#define	PIP_INT_EN				0x00011800a0000010ULL
#define	PIP_STAT_CTL				0x00011800a0000018ULL
#define	PIP_GBL_CTL				0x00011800a0000020ULL
#define	PIP_GBL_CFG				0x00011800a0000028ULL
#define	PIP_SOFT_RST				0x00011800a0000030ULL
#define	PIP_IP_OFFSET				0x00011800a0000060ULL
#define	PIP_TAG_SECRET				0x00011800a0000068ULL
#define	PIP_TAG_MASK				0x00011800a0000070ULL
#define	PIP_DEC_IPSEC0				0x00011800a0000080ULL
#define	PIP_DEC_IPSEC1				0x00011800a0000088ULL
#define	PIP_DEC_IPSEC2				0x00011800a0000090ULL
#define	PIP_DEC_IPSEC3				0x00011800a0000098ULL
#define	PIP_RAW_WORD				0x00011800a00000b0ULL
#define	PIP_QOS_VLAN0				0x00011800a00000c0ULL
#define	PIP_QOS_VLAN1				0x00011800a00000c8ULL
#define	PIP_QOS_VLAN2				0x00011800a00000d0ULL
#define	PIP_QOS_VLAN3				0x00011800a00000d8ULL
#define	PIP_QOS_VLAN4				0x00011800a00000e0ULL
#define	PIP_QOS_VLAN5				0x00011800a00000e8ULL
#define	PIP_QOS_VLAN6				0x00011800a00000f0ULL
#define	PIP_QOS_VLAN7				0x00011800a00000f8ULL
#define	PIP_QOS_WATCH0				0x00011800a0000100ULL
#define	PIP_QOS_WATCH1				0x00011800a0000108ULL
#define	PIP_QOS_WATCH2				0x00011800a0000110ULL
#define	PIP_QOS_WATCH3				0x00011800a0000118ULL
#define	PIP_QOS_WATCH4				0x00011800a0000120ULL
#define	PIP_QOS_WATCH5				0x00011800a0000128ULL
#define	PIP_QOS_WATCH6				0x00011800a0000130ULL
#define	PIP_QOS_WATCH7				0x00011800a0000138ULL
#define	PIP_FRM_LEN_CHK0			0x00011800a0000180ULL
#define	PIP_FRM_LEN_CHK1			0x00011800a0000188ULL
#define	PIP_DSA_SRC_GRP				0x00011800a0000190ULL
#define	PIP_DSA_VID_GRP				0x00011800a0000198ULL
#define	PIP_HG_PRI_QOS				0x00011800a00001a0ULL
#define	PIP_PRT_CFG0				0x00011800a0000200ULL
#define	PIP_PRT_CFG1				0x00011800a0000208ULL
#define	PIP_PRT_CFG2				0x00011800a0000210ULL
#define	PIP_PRT_CFG32				0x00011800a0000300ULL
#define	PIP_PRT_TAG0				0x00011800a0000400ULL
#define	PIP_PRT_TAG1				0x00011800a0000408ULL
#define	PIP_PRT_TAG2				0x00011800a0000410ULL
#define	PIP_PRT_TAG32				0x00011800a0000500ULL
#define	PIP_QOS_DIFF0				0x00011800a0000600ULL
/* PIP_QOS_DIFF[1-63] */
/* PIP_STAT[0-9]_PRT{0,1,2,32} */
#define	PIP_STAT0_PRT(i)			(0x00011800a0000800ULL + (i) * 0x50)
#define	PIP_STAT0_PRT0				0x00011800a0000800ULL
#define	PIP_STAT0_PRT1				0x00011800a0000850ULL
#define	PIP_STAT0_PRT2				0x00011800a00008a0ULL
#define	PIP_STAT0_PRT32				0x00011800a0001200ULL
#define	PIP_TAG_INC0				0x00011800a0001800ULL
/* PIP_TAG_INC[1-63] */
#define	PIP_STAT_INB_PKTS0			0x00011800a0001a00ULL
#define	PIP_STAT_INB_PKTS1			0x00011800a0001a20ULL
#define	PIP_STAT_INB_PKTS2			0x00011800a0001a40ULL
#define	PIP_STAT_INB_PKTS32			0x00011800a0001e00ULL
#define	PIP_STAT_INB_OCTS0			0x00011800a0001a08ULL
#define	PIP_STAT_INB_OCTS1			0x00011800a0001a28ULL
#define	PIP_STAT_INB_OCTS2			0x00011800a0001a48ULL
#define	PIP_STAT_INB_OCTS32			0x00011800a0001e08ULL
#define	PIP_STAT_INB_ERRS0			0x00011800a0001a10ULL
#define	PIP_STAT_INB_ERRS1			0x00011800a0001a30ULL
#define	PIP_STAT_INB_ERRS2			0x00011800a0001a50ULL
#define	PIP_STAT_INB_ERRS32			0x00011800a0001e10ULL

#define PIP_BASE 0x00011800a0000000ULL
#define PIP_SIZE 0x1e50ULL

#define	PIP_BIST_STATUS_OFFSET				0x0ULL
#define	PIP_INT_REG_OFFSET				0x8ULL
#define	PIP_INT_EN_OFFSET				0x10ULL
#define	PIP_STAT_CTL_OFFSET				0x18ULL
#define	PIP_GBL_CTL_OFFSET				0x20ULL
#define	PIP_GBL_CFG_OFFSET				0x28ULL
#define	PIP_SOFT_RST_OFFSET				0x30ULL
#define	PIP_IP_OFFSET_OFFSET				0x60ULL
#define	PIP_TAG_SECRET_OFFSET				0x68ULL
#define	PIP_TAG_MASK_OFFSET				0x70ULL
#define	PIP_DEC_IPSEC0_OFFSET				0x80ULL
#define	PIP_DEC_IPSEC1_OFFSET				0x88ULL
#define	PIP_DEC_IPSEC2_OFFSET				0x90ULL
#define	PIP_DEC_IPSEC3_OFFSET				0x98ULL
#define	PIP_RAW_WORD_OFFSET				0xb0ULL
#define	PIP_QOS_VLAN0_OFFSET				0xc0ULL
#define	PIP_QOS_VLAN1_OFFSET				0xc8ULL
#define	PIP_QOS_VLAN2_OFFSET				0xd0ULL
#define	PIP_QOS_VLAN3_OFFSET				0xd8ULL
#define	PIP_QOS_VLAN4_OFFSET				0xe0ULL
#define	PIP_QOS_VLAN5_OFFSET				0xe8ULL
#define	PIP_QOS_VLAN6_OFFSET				0xf0ULL
#define	PIP_QOS_VLAN7_OFFSET				0xf8ULL
#define	PIP_QOS_WATCH0_OFFSET				0x100ULL
#define	PIP_QOS_WATCH1_OFFSET				0x108ULL
#define	PIP_QOS_WATCH2_OFFSET				0x110ULL
#define	PIP_QOS_WATCH3_OFFSET				0x118ULL
#define	PIP_PRT_CFG0_OFFSET				0x200ULL
#define	PIP_PRT_CFG1_OFFSET				0x208ULL
#define	PIP_PRT_CFG2_OFFSET				0x210ULL
#define	PIP_PRT_CFG32_OFFSET				0x300ULL
#define	PIP_PRT_TAG0_OFFSET				0x400ULL
#define	PIP_PRT_TAG1_OFFSET				0x408ULL
#define	PIP_PRT_TAG2_OFFSET				0x410ULL
#define	PIP_PRT_TAG32_OFFSET				0x500ULL
#define	PIP_QOS_DIFF0_OFFSET				0x600ULL
/* PIP_QOS_DIFF[1-63] */
#define	PIP_STAT0_PRT_OFFSET(i)				(0x800ULL + (i) * 0x50)
#define	PIP_STAT0_PRT0_OFFSET				0x800ULL
#define	PIP_STAT0_PRT1_OFFSET				0x850ULL
#define	PIP_STAT0_PRT2_OFFSET				0x8a0ULL
#define	PIP_STAT0_PRT32_OFFSET				0x1200ULL
#define	PIP_STAT0_PRT33_OFFSET				0x1250ULL
#define	PIP_STAT1_PRT0_OFFSET				0x800ULL
#define	PIP_STAT1_PRT1_OFFSET				0x850ULL
#define	PIP_STAT1_PRT2_OFFSET				0x8a0ULL
#define	PIP_STAT1_PRT32_OFFSET				0x1200ULL
#define	PIP_STAT1_PRT33_OFFSET				0x1250ULL
#define	PIP_STAT2_PRT0_OFFSET				0x810ULL
#define	PIP_STAT2_PRT1_OFFSET				0x860ULL
#define	PIP_STAT2_PRT2_OFFSET				0x8b0ULL
#define	PIP_STAT2_PRT32_OFFSET				0x1210ULL
#define	PIP_STAT2_PRT33_OFFSET				0x1260ULL
#define	PIP_STAT3_PRT0_OFFSET				0x818ULL
#define	PIP_STAT3_PRT1_OFFSET				0x868ULL
#define	PIP_STAT3_PRT2_OFFSET				0x8b8ULL
#define	PIP_STAT3_PRT32_OFFSET				0x1218ULL
#define	PIP_STAT3_PRT33_OFFSET				0x1268ULL
#define	PIP_STAT4_PRT0_OFFSET				0x820ULL
#define	PIP_STAT4_PRT1_OFFSET				0x870ULL
#define	PIP_STAT4_PRT2_OFFSET				0x8c0ULL
#define	PIP_STAT4_PRT32_OFFSET				0x1220ULL
#define	PIP_STAT4_PRT33_OFFSET				0x1270ULL
#define	PIP_STAT5_PRT0_OFFSET				0x828ULL
#define	PIP_STAT5_PRT1_OFFSET				0x878ULL
#define	PIP_STAT5_PRT2_OFFSET				0x8c8ULL
#define	PIP_STAT5_PRT32_OFFSET				0x1228ULL
#define	PIP_STAT5_PRT33_OFFSET				0x1278ULL
#define	PIP_STAT6_PRT0_OFFSET				0x830ULL
#define	PIP_STAT6_PRT1_OFFSET				0x880ULL
#define	PIP_STAT6_PRT2_OFFSET				0x8d0ULL
#define	PIP_STAT6_PRT32_OFFSET				0x1238ULL
#define	PIP_STAT6_PRT33_OFFSET				0x1288ULL
#define	PIP_STAT7_PRT0_OFFSET				0x838ULL
#define	PIP_STAT7_PRT1_OFFSET				0x888ULL
#define	PIP_STAT7_PRT2_OFFSET				0x8d8ULL
#define	PIP_STAT7_PRT32_OFFSET				0x1238ULL
#define	PIP_STAT7_PRT33_OFFSET				0x1288ULL
#define	PIP_STAT8_PRT0_OFFSET				0x840ULL
#define	PIP_STAT8_PRT1_OFFSET				0x890ULL
#define	PIP_STAT8_PRT2_OFFSET				0x8e0ULL
#define	PIP_STAT8_PRT32_OFFSET				0x1240ULL
#define	PIP_STAT8_PRT33_OFFSET				0x1290ULL
#define	PIP_STAT9_PRT0_OFFSET				0x848ULL
#define	PIP_STAT9_PRT1_OFFSET				0x898ULL
#define	PIP_STAT9_PRT2_OFFSET				0x8e8ULL
#define	PIP_STAT9_PRT32_OFFSET				0x1248ULL
#define	PIP_STAT9_PRT33_OFFSET				0x1298ULL
#define	PIP_TAG_INC0_OFFSET				0x1800ULL
/* PIP_TAG_INC[1-63] */
#define	PIP_STAT_INB_PKTS0_OFFSET			0x1a00ULL
#define	PIP_STAT_INB_PKTS1_OFFSET			0x1a20ULL
#define	PIP_STAT_INB_PKTS2_OFFSET			0x1a40ULL
#define	PIP_STAT_INB_PKTS32_OFFSET			0x1e00ULL
#define	PIP_STAT_INB_OCTS0_OFFSET			0x1a08ULL
#define	PIP_STAT_INB_OCTS1_OFFSET			0x1a28ULL
#define	PIP_STAT_INB_OCTS2_OFFSET			0x1a48ULL
#define	PIP_STAT_INB_OCTS32_OFFSET			0x1e08ULL
#define	PIP_STAT_INB_ERRS0_OFFSET			0x1a10ULL
#define	PIP_STAT_INB_ERRS1_OFFSET			0x1a30ULL
#define	PIP_STAT_INB_ERRS2_OFFSET			0x1a50ULL
#define	PIP_STAT_INB_ERRS32_OFFSET			0x1e10ULL
#define	PIP_STAT_INB_ERRS33_OFFSET			0x1e30ULL

/*
 * PIP_BIST_STATUS
 */
#define	PIP_BIST_STATUS_63_13			UINT64_C(0xfffffffffffc0000)
#define	PIP_BIST_STATUS_BIST			UINT64_C(0x000000000003ffff)

/*
 * PIP_INT_REG
 */
#define	PIP_INT_REG_63_9			UINT64_C(0xfffffffffffffe00)
#define	PIP_INT_REG_BEPERR			UINT64_C(0x0000000000000100)
#define	PIP_INT_REG_FEPERR			UINT64_C(0x0000000000000080)
#define	PIP_INT_REG_6				UINT64_C(0x0000000000000040)
#define	PIP_INT_REG_SKPRUNT			UINT64_C(0x0000000000000020)
#define	PIP_INT_REG_BADTAG			UINT64_C(0x0000000000000010)
#define	PIP_INT_REG_PRTNXA			UINT64_C(0x0000000000000008)
#define	PIP_INT_REG_2_1				0x00000006
#define	PIP_INT_REG_PKTDRP			UINT32_C(0x00000001)

/*
 * PIP_INT_EN
 */
#define	PIP_INT_EN_63_9				UINT64_C(0xfffffffffffffe00)
#define	PIP_INT_EN_BEPERR			UINT64_C(0x0000000000000100)
#define	PIP_INT_EN_FEPERR			UINT64_C(0x0000000000000080)
#define	PIP_INT_EN_6				UINT64_C(0x0000000000000040)
#define	PIP_INT_EN_SKPRUNT			UINT64_C(0x0000000000000020)
#define	PIP_INT_EN_BADTAG			UINT64_C(0x0000000000000010)
#define	PIP_INT_EN_PRTNXA			UINT64_C(0x0000000000000008)
#define	PIP_INT_EN_2_1				0x00000006
#define	PIP_INT_EN_PKTDRP			UINT32_C(0x00000001)

/*
 * PIP_STAT_CTL
 */
#define	PIP_STAT_CTL_63_1			UINT64_C(0xfffffffffffffffe)
#define	PIP_STAT_CTL_RDCLR			UINT64_C(0x0000000000000001)

/*
 * PIP_GBL_CTL
 */
#define	PIP_GBL_CTL_63_17			UINT64_C(0xfffffffffffe0000)
#define	PIP_GBL_CTL_IGNRS			UINT64_C(0x0000000000010000)
#define	PIP_GBL_CTL_VS_WQE			UINT64_C(0x0000000000008000)
#define	PIP_GBL_CTL_VS_QOS			UINT64_C(0x0000000000004000)
#define	PIP_GBL_CTL_L2MAL			UINT64_C(0x0000000000002000)
#define	PIP_GBL_CTL_TCP_FLAG			UINT64_C(0x0000000000001000)
#define	PIP_GBL_CTL_L4_LEN			UINT64_C(0x0000000000000800)
#define	PIP_GBL_CTL_L4_CHK			UINT64_C(0x0000000000000400)
#define	PIP_GBL_CTL_L4_PRT			UINT64_C(0x0000000000000200)
#define	PIP_GBL_CTL_L4_MAL			UINT64_C(0x0000000000000100)
#define	PIP_GBL_CTL_7_6				UINT64_C(0x00000000000000c0)
#define	PIP_GBL_CTL_IP6_EEXT			UINT64_C(0x0000000000000030)
#define	PIP_GBL_CTL_IP4_OPTS			UINT64_C(0x0000000000000008)
#define	PIP_GBL_CTL_IP_HOP			UINT64_C(0x0000000000000004)
#define	PIP_GBL_CTL_IP_MAL			UINT64_C(0x0000000000000002)
#define	PIP_GBL_CTL_IP_CHK			UINT64_C(0x0000000000000001)

/*
 * PIP_GBL_CFG
 */
/* XXX 63_17 is reserved? */
#define	PIP_GBL_CFG_63_19			UINT64_C(0xfffffffffff80000)
#define	PIP_GBL_CFG_TAG_SYN			UINT64_C(0x0000000000040000)
#define	PIP_GBL_CFG_IP6_UDP			UINT64_C(0x0000000000020000)
#define	PIP_GBL_CFG_MAX_L2			UINT64_C(0x0000000000010000)
#define	PIP_GBL_CFG_15_11			UINT64_C(0x000000000000f800)
#define	PIP_GBL_CFG_RAW_SHF			UINT64_C(0x0000000000000700)
#define	PIP_GBL_CFG_7_3				UINT64_C(0x00000000000000f8)
#define	PIP_GBL_CFG_NIP_SHF_MASK		UINT64_C(0x0000000000000007)

/*
 * PIP_SFT_RST
 */
#define	PIP_SFT_RST_63_17			UINT64_C(0xfffffffffffffffe)
#define	PIP_SFT_RST_RST				UINT64_C(0x0000000000000001)

/*
 * PIP_IP_OFFSET
 */
#define	PIP_IP_OFFSET_63_3			UINT64_C(0xfffffffffffffff8)
/* PIP_IP_OFFSET_OFFSET is defined above - conflict! */
#define	PIP_IP_OFFSET_MASK_OFFSET		UINT64_C(0x0000000000000007)

/*
 * PIP_TAG_SECRET
 */
#define	PIP_TAG_SECRET_63_3			UINT64_C(0xffffffff00000000)
#define	PIP_TAG_SECRET_DST			UINT64_C(0x00000000ffff0000)
#define	PIP_TAG_SECRET_SRC			UINT64_C(0x000000000000ffff)

/*
 * PIP_TAG_MASK
 */
#define	PIP_TAG_MASK_63_16			UINT64_C(0xffffffffffff0000)
#define	PIP_TAG_MASK_MASK			UINT64_C(0x000000000000ffff)

/*
 * PIP_DEC_IPSECN
 */
#define	PIP_DEC_IPSECN_63_18			UINT64_C(0xfffffffffffc0000)
#define	PIP_DEC_IPSECN_TCP			UINT64_C(0x0000000000020000)
#define	PIP_DEC_IPSECN_UDP			UINT64_C(0x0000000000010000)
#define	PIP_DEC_IPSECN_DPRT			UINT64_C(0x000000000000ffff)

/*
 * PIP_RAW_WORD
 */
#define	PIP_RAW_WORD_63_56			UINT64_C(0xff00000000000000)
#define	PIP_RAW_WORD_WORD			UINT64_C(0x00ffffffffffffff)

/*
 * PIP_QOS_VLAN
 */
#define	PIP_QOS_VLAN_63_3			UINT64_C(0xfffffffffffffff8)
#define	PIP_QOS_VLAN_QOS			UINT64_C(0x0000000000000007)

/*
 * PIP_QOS_WATCHN
 */
#define	PIP_QOS_WATCHN_63_48			UINT64_C(0xffff000000000000)
#define	PIP_QOS_WATCHN_MASK			UINT64_C(0x0000ffff00000000)
#define	PIP_QOS_WATCHN_31_28			UINT64_C(0x00000000f0000000)
#define	PIP_QOS_WATCHN_GRP			UINT64_C(0x000000000f000000)
#define	PIP_QOS_WATCHN_23			UINT64_C(0x0000000000800000)
#define	PIP_QOS_WATCHN_WATCHER			UINT64_C(0x0000000000700000)
#define	PIP_QOS_WATCHN_19_18			UINT64_C(0x00000000000c0000)
#define	PIP_QOS_WATCHN_TYPE			UINT64_C(0x0000000000030000)
#define	PIP_QOS_WATCHN_15_0			UINT64_C(0x000000000000ffff)

/*
 * PIP_PRT_CFGN
 */
#define	PIP_PRT_CFGN_63_53			UINT64_C(0xffe0000000000000)
#define	PIP_PRT_CFGN_PAD_LEN			UINT64_C(0x0010000000000000)
#define	PIP_PRT_CFGN_VLAN_LEN			UINT64_C(0x0008000000000000)
#define	PIP_PRT_CFGN_LENERR_EN			UINT64_C(0x0004000000000000)
#define	PIP_PRT_CFGN_MAXERR_EN			UINT64_C(0x0002000000000000)
#define	PIP_PRT_CFGN_MINERR_EN			UINT64_C(0x0001000000000000)
#define	PIP_PRT_CFGN_GRP_WAT_47			UINT64_C(0x0000f00000000000)
#define	PIP_PRT_CFGN_QOS_WAT_47			UINT64_C(0x00000f0000000000)
#define	PIP_PRT_CFGN_39_37			UINT64_C(0x000000e000000000)
#define	PIP_PRT_CFGN_RAWDRP			UINT64_C(0x0000001000000000)
#define	PIP_PRT_CFGN_TAG_INC			UINT64_C(0x0000000c00000000)
#define	PIP_PRT_CFGN_DYN_RS			UINT64_C(0x0000000200000000)
#define	PIP_PRT_CFGN_INST_HDR			UINT64_C(0x0000000100000000)
#define	PIP_PRT_CFGN_GRP_WAT			UINT64_C(0x00000000f0000000)
#define	PIP_PRT_CFGN_27				UINT64_C(0x0000000008000000)
#define	PIP_PRT_CFGN_QOS			UINT64_C(0x0000000007000000)
#define	PIP_PRT_CFGN_QOS_WAT			UINT64_C(0x0000000000f00000)
#define	PIP_PRT_CFGN_19				UINT64_C(0x0000000000080000)
#define	PIP_PRT_CFGN_SPARE			UINT64_C(0x0000000000040000)
#define	PIP_PRT_CFGN_QOS_DIFF			UINT64_C(0x0000000000020000)
#define	PIP_PRT_CFGN_QOS_VLAN			UINT64_C(0x0000000000010000)
#define	PIP_PRT_CFGN_15_13			UINT64_C(0x000000000000e000)
#define	PIP_PRT_CFGN_CRC_EN			UINT64_C(0x0000000000001000)
#define	PIP_PRT_CFGN_11_10			UINT64_C(0x0000000000000c00)
#define	PIP_PRT_CFGN_MODE			UINT64_C(0x0000000000000300)
#define   PIP_PORT_CFG_MODE_NONE		  0
#define   PIP_PORT_CFG_MODE_L2			  1
#define   PIP_PORT_CFG_MODE_IP			  2
#define   PIP_PORT_CFG_MODE_PCI			  3
#define	PIP_PRT_CFGN_7				UINT64_C(0x0000000000000080)
#define	PIP_PRT_CFGN_SKIP			UINT64_C(0x000000000000007f)

/*
 * PIP_PRT_TAGN
 */
#define	PIP_PRT_TAGN_63_40			UINT64_C(0xffffff0000000000)
#define	PIP_PRT_TAGN_GRPTAGBASE			UINT64_C(0x000000f000000000)
#define	PIP_PRT_TAGN_GRPTAGMASK			UINT64_C(0x0000000f00000000)
#define	PIP_PRT_TAGN_GRPTAG			UINT64_C(0x0000000080000000)
#define	PIP_PRT_TAGN_SPARE			UINT64_C(0x0000000040000000)
#define	PIP_PRT_TAGN_TAG_MODE			UINT64_C(0x0000000030000000)
#define	PIP_PRT_TAGN_INC_VS			UINT64_C(0x000000000c000000)
#define	PIP_PRT_TAGN_INC_VLAN			UINT64_C(0x0000000002000000)
#define	PIP_PRT_TAGN_INC_PRT			UINT64_C(0x0000000001000000)
#define	PIP_PRT_TAGN_IP6_DPRT			UINT64_C(0x0000000000800000)
#define	PIP_PRT_TAGN_IP4_DPRT			UINT64_C(0x0000000000400000)
#define	PIP_PRT_TAGN_IP6_SPRT			UINT64_C(0x0000000000200000)
#define	PIP_PRT_TAGN_IP4_SPRT			UINT64_C(0x0000000000100000)
#define	PIP_PRT_TAGN_IP6_NXTH			UINT64_C(0x0000000000080000)
#define	PIP_PRT_TAGN_IP4_PCTL			UINT64_C(0x0000000000040000)
#define	PIP_PRT_TAGN_IP6_DST			UINT64_C(0x0000000000020000)
#define	PIP_PRT_TAGN_IP4_SRC			UINT64_C(0x0000000000010000)
#define	PIP_PRT_TAGN_IP6_SRC			UINT64_C(0x0000000000008000)
#define	PIP_PRT_TAGN_IP4_DST			UINT64_C(0x0000000000004000)
#define	PIP_PRT_TAGN_TCP6_TAG			UINT64_C(0x0000000000003000)
#define	   PIP_PRT_TAGN_TCP6_TAG_ORDERED	  0
#define	   PIP_PRT_TAGN_TCP6_TAG_ATOMIC		  1
#define	   PIP_PRT_TAGN_TCP6_TAG_NULL		  2
#define	   PIP_PRT_TAGN_TCP6_TAG_XXX_3		  3
#define	PIP_PRT_TAGN_TCP4_TAG			UINT64_C(0x0000000000000c00)
#define	   PIP_PRT_TAGN_TCP4_TAG_ORDERED	  0
#define	   PIP_PRT_TAGN_TCP4_TAG_ATOMIC		  1
#define	   PIP_PRT_TAGN_TCP4_TAG_NULL		  2
#define	   PIP_PRT_TAGN_TCP4_TAG_XXX_3		  3
#define	PIP_PRT_TAGN_IP6_TAG			UINT64_C(0x0000000000000300)
#define	   PIP_PRT_TAGN_IP6_TAG_ORDERED		  0
#define	   PIP_PRT_TAGN_IP6_TAG_ATOMIC		  1
#define	   PIP_PRT_TAGN_IP6_TAG_NULL		  2
#define	   PIP_PRT_TAGN_IP6_TAG_XXX_3		  3
#define	PIP_PRT_TAGN_IP4_TAG			UINT64_C(0x00000000000000c0)
#define	   PIP_PRT_TAGN_IP4_TAG_ORDERED		  0
#define	   PIP_PRT_TAGN_IP4_TAG_ATOMIC		  1
#define	   PIP_PRT_TAGN_IP4_TAG_NULL		  2
#define	   PIP_PRT_TAGN_IP4_TAG_XXX_3		  3
#define	PIP_PRT_TAGN_NON_TAG			UINT64_C(0x0000000000000030)
#define	   PIP_PRT_TAGN_NON_TAG_ORDERED		  0
#define	   PIP_PRT_TAGN_NON_TAG_ATOMIC		  1
#define	   PIP_PRT_TAGN_NON_TAG_NULL		  2
#define	   PIP_PRT_TAGN_NON_TAG_XXX_3		  3
#define	PIP_PRT_TAGN_GRP			UINT64_C(0x000000000000000f)

/*
 * PIP_QOS_DIFFN
 */
#define	PIP_QOS_DIFF_63_3			UINT64_C(0xfffffffffffffff8)
#define	PIP_QOS_DIFF_QOS			UINT64_C(0x0000000000000007)

/*
 * PIP_TAG_INCN
 */
#define	PIP_TAG_INCN_63_8			UINT64_C(0xffffffffffffff00)
#define	PIP_TAG_INCN_EN				UINT64_C(0x00000000000000ff)

/*
 * PIP_STAT0_PRTN
 */
#define	PIP_STAT0_PRTN_DRP_PKTS			UINT64_C(0xffffffff00000000)
#define	PIP_STAT0_PRTN_DRP_OCTS			UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT1_PRTN
 */
#define	PIP_STAT1_PRTN_63_48			UINT64_C(0xffff000000000000)
#define	PIP_STAT1_PRTN_OCTS			UINT64_C(0x0000ffffffffffff)

/*
 * PIP_STAT2_PRTN
 */
#define	PIP_STAT2_PRTN_PKTS			UINT64_C(0xffffffff00000000)
#define	PIP_STAT2_PRTN_RAW			UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT3_PRTN
 */
#define	PIP_STAT3_PRTN_BCST			UINT64_C(0xffffffff00000000)
#define	PIP_STAT3_PRTN_MCST			UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT4_PRTN
 */
#define	PIP_STAT4_PRTN_H65TO127			UINT64_C(0xffffffff00000000)
#define	PIP_STAT4_PRTN_H64			UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT5_PRTN
 */
#define	PIP_STAT5_PRTN_H256TO511		UINT64_C(0xffffffff00000000)
#define	PIP_STAT5_PRTN_H128TO255		UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT6_PRTN
 */
#define	PIP_STAT6_PRTN_H1024TO1518		UINT64_C(0xffffffff00000000)
#define	PIP_STAT6_PRTN_H512TO1023		UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT7_PRTN
 */
#define	PIP_STAT7_PRTN_FCS			UINT64_C(0xffffffff00000000)
#define	PIP_STAT7_PRTN_H1519			UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT8_PRTN
 */
#define	PIP_STAT8_PRTN_FRAG			UINT64_C(0xffffffff00000000)
#define	PIP_STAT8_PRTN_UNDERSZ			UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT9_PRTN
 */
#define	PIP_STAT9_PRTN_JABBER			UINT64_C(0xffffffff00000000)
#define	PIP_STAT9_PRTN_OVERSZ			UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT_INB_PKTN
 */
#define	PIP_STAT_INB_PKTSN			UINT64_C(0xffffffff00000000)
#define	PIP_STAT_INB_PKTSN_PKTS			UINT64_C(0x00000000ffffffff)

/*
 * PIP_STAT_INB_OCTSN
 */
#define	PIP_STAT_INB_OCTSN			UINT64_C(0xffff000000000000)
#define	PIP_STAT_INB_OCTSN_OCTS			UINT64_C(0x0000ffffffffffff)

/*
 * PIP_STAT_INB_ERRS
 */
#define	PIP_STAT_INB_ERRSN			UINT64_C(0xffffffffffff0000)
#define	PIP_STAT_INB_ERRSN_OCTS			UINT64_C(0x000000000000ffff)

/*
 * Work-Queue Entry Format
 */
/* WORD0 */
#define PIP_WQE_WORD0_HW_CSUM			UINT64_C(0xffff000000000000)
#define PIP_WQE_WORD0_47_40			UINT64_C(0x0000ff0000000000)
#define PIP_WQE_WORD0_POW_NEXT_PTR		UINT64_C(0x000000ffffffffff)

/* WORD 1 */
#define PIP_WQE_WORD1_LEN			UINT64_C(0xffff000000000000)
#define PIP_WQE_WORD1_IPRT			UINT64_C(0x0000fc0000000000)
#define PIP_WQE_WORD1_QOS			UINT64_C(0x0000038000000000)
#define PIP_WQE_WORD1_GRP			UINT64_C(0x0000007800000000)
#define PIP_WQE_WORD1_TT			UINT64_C(0x0000000700000000)
#define PIP_WQE_WORD1_TAG			UINT64_C(0x00000000ffffffff)

/* WORD 2 */
#define PIP_WQE_WORD2_RAWFULL_BUFS		UINT64_C(0xff00000000000000)
#define PIP_WQE_WORD2_RAWFULL_PIP_RAW_WORD	UINT64_C(0x00ffffffffffffff)

#define PIP_WQE_WORD2_IP_BUFS			UINT64_C(0xff00000000000000)
#define PIP_WQE_WORD2_IP_OFFSET			UINT64_C(0x00ff000000000000)
#define PIP_WQE_WORD2_IP_VV			UINT64_C(0x0000800000000000)
#define PIP_WQE_WORD2_IP_VS			UINT64_C(0x0000400000000000)
#define PIP_WQE_WORD2_IP_45			UINT64_C(0x0000200000000000)
#define PIP_WQE_WORD2_IP_VC			UINT64_C(0x0000100000000000)
#define PIP_WQE_WORD2_IP_VLAN_ID		UINT64_C(0x00000fff00000000)
#define PIP_WQE_WORD2_IP_31_20			UINT64_C(0x00000000fff00000)
#define PIP_WQE_WORD2_IP_CO			UINT64_C(0x0000000000080000)
#define PIP_WQE_WORD2_IP_TU			UINT64_C(0x0000000000040000)
#define PIP_WQE_WORD2_IP_SE			UINT64_C(0x0000000000020000)
#define PIP_WQE_WORD2_IP_V6			UINT64_C(0x0000000000010000)
#define PIP_WQE_WORD2_IP_15			UINT64_C(0x0000000000008000)
#define PIP_WQE_WORD2_IP_LE			UINT64_C(0x0000000000004000)
#define PIP_WQE_WORD2_IP_FR			UINT64_C(0x0000000000002000)
#define PIP_WQE_WORD2_IP_IE			UINT64_C(0x0000000000001000)
#define PIP_WQE_WORD2_IP_B			UINT64_C(0x0000000000000800)
#define PIP_WQE_WORD2_IP_M			UINT64_C(0x0000000000000400)
#define PIP_WQE_WORD2_IP_NI			UINT64_C(0x0000000000000200)
#define PIP_WQE_WORD2_IP_RE			UINT64_C(0x0000000000000100)
#define PIP_WQE_WORD2_IP_OPECODE		UINT64_C(0x00000000000000ff)

#define PIP_WQE_WORD2_NOIP_BUFS			UINT64_C(0xff00000000000000)
#define PIP_WQE_WORD2_NOIP_55_48		UINT64_C(0x00ff000000000000)
#define PIP_WQE_WORD2_NOIP_VV			UINT64_C(0x0000800000000000)
#define PIP_WQE_WORD2_NOIP_VS			UINT64_C(0x0000400000000000)
#define PIP_WQE_WORD2_NOIP_45			UINT64_C(0x0000200000000000)
#define PIP_WQE_WORD2_NOIP_VC			UINT64_C(0x0000100000000000)
#define PIP_WQE_WORD2_NOIP_VLAN_ID		UINT64_C(0x00000fff00000000)
#define PIP_WQE_WORD2_NOIP_31_14		UINT64_C(0x00000000ffffc000)
#define PIP_WQE_WORD2_NOIP_IR			UINT64_C(0x0000000000002000)
#define PIP_WQE_WORD2_NOIP_IA			UINT64_C(0x0000000000001000)
#define PIP_WQE_WORD2_NOIP_B			UINT64_C(0x0000000000000800)
#define PIP_WQE_WORD2_NOIP_M			UINT64_C(0x0000000000000400)
#define PIP_WQE_WORD2_NOIP_NI			UINT64_C(0x0000000000000200)
#define PIP_WQE_WORD2_NOIP_RE			UINT64_C(0x0000000000000100)
#define PIP_WQE_WORD2_NOIP_OPECODE		UINT64_C(0x00000000000000ff)

/* WORD 3 */
#define PIP_WQE_WORD3_63			UINT64_C(0x8000000000000000)
#define PIP_WQE_WORD3_BACK			UINT64_C(0x7800000000000000)
#define PIP_WQE_WORD3_58_56			UINT64_C(0x0700000000000000)
#define PIP_WQE_WORD3_SIZE			UINT64_C(0x00ffff0000000000)
#define PIP_WQE_WORD3_ADDR			UINT64_C(0x000000ffffffffff)

/* opcode for WORD2[LE] */
#define PIP_WQE_WORD2_LE_OPCODE_MAL		1
#define PIP_WQE_WORD2_LE_OPCODE_CSUM		2
#define PIP_WQE_WORD2_LE_OPCODE_UDPLEN		3
#define PIP_WQE_WORD2_LE_OPCODE_PORT		4
#define PIP_WQE_WORD2_LE_OPCODE_XXX_5		5
#define PIP_WQE_WORD2_LE_OPCODE_XXX_6		6
#define PIP_WQE_WORD2_LE_OPCODE_XXX_7		7
#define PIP_WQE_WORD2_LE_OPCODE_FINO		8
#define PIP_WQE_WORD2_LE_OPCODE_NOFL		9
#define PIP_WQE_WORD2_LE_OPCODE_FINRST		10
#define PIP_WQE_WORD2_LE_OPCODE_SYNURG		11
#define PIP_WQE_WORD2_LE_OPCODE_SYNRST		12
#define PIP_WQE_WORD2_LE_OPCODE_SYNFIN		13

/* opcode for WORD2[IE] */
#define PIP_WQE_WORD2_IE_OPCODE_NOTIP		1
#define PIP_WQE_WORD2_IE_OPCODE_CSUM		2
#define PIP_WQE_WORD2_IE_OPCODE_MALHDR		3
#define PIP_WQE_WORD2_IE_OPCODE_MAL		4
#define PIP_WQE_WORD2_IE_OPCODE_TTL		5
#define PIP_WQE_WORD2_IE_OPCODE_OPT		6

/* opcode for WORD2[RE] */
#define PIP_WQE_WORD2_RE_OPCODE_PARTIAL		1
#define PIP_WQE_WORD2_RE_OPCODE_JABBER		2
#define PIP_WQE_WORD2_RE_OPCODE_OVRRUN		3
#define PIP_WQE_WORD2_RE_OPCODE_OVRSZ		4
#define PIP_WQE_WORD2_RE_OPCODE_ALIGN		5
#define PIP_WQE_WORD2_RE_OPCODE_FRAG		6
#define PIP_WQE_WORD2_RE_OPCODE_GMXFCS		7
#define PIP_WQE_WORD2_RE_OPCODE_UDRSZ		8
#define PIP_WQE_WORD2_RE_OPCODE_EXTEND		9
#define PIP_WQE_WORD2_RE_OPCODE_LENGTH		10
#define PIP_WQE_WORD2_RE_OPCODE_MIIRX		11
#define PIP_WQE_WORD2_RE_OPCODE_MIISKIP		12
#define PIP_WQE_WORD2_RE_OPCODE_MIINBL		13
#define PIP_WQE_WORD2_RE_OPCODE_XXX_14		14
#define PIP_WQE_WORD2_RE_OPCODE_XXX_15		15
#define PIP_WQE_WORD2_RE_OPCODE_XXX_16		16
#define PIP_WQE_WORD2_RE_OPCODE_SKIP		17
#define PIP_WQE_WORD2_RE_OPCODE_L2MAL		18

#endif /* _OCTEON_PIPREG_H_ */
