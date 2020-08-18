/*	$NetBSD: octeon_smireg.h,v 1.3 2020/06/23 05:18:02 simonb Exp $	*/

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
 * SMI Registers
 */

#ifndef _OCTEON_SMIREG_H_
#define _OCTEON_SMIREG_H_

#define	SMI_CMD_OFFSET			0x00ULL
#define	SMI_WR_DAT_OFFSET		0x08ULL
#define	SMI_RD_DAT_OFFSET		0x10ULL
#define	SMI_CLK_OFFSET			0x18ULL
#define	SMI_EN_OFFSET			0x20ULL

#define SMI_BASE			0x0001180000001800ULL
#define SMI_SIZE			0x040ULL
/* XXX - support 1 SMI unit for direct attach; some CPUs have 4 SMIs */
#define SMI_NUNITS			1

/* SMI CMD */
#define SMI_CMD_63_17			UINT64_C(0xfffffffffffe0000)
#define SMI_CMD_PHY_OP			UINT64_C(0x0000000000010000)
#define   SMI_CMD_PHY_OP_READ		  1
#define   SMI_CMD_PHY_OP_WRITE		  0
#define SMI_CMD_15_13			UINT64_C(0x000000000000e000)
#define SMI_CMD_PHY_ADR			UINT64_C(0x0000000000001f00)
#define SMI_CMD_7_5			UINT64_C(0x00000000000000e0)
#define SMI_CMD_REG_ADR			UINT64_C(0x000000000000001f)

/* SMI_WR_DAT */
#define SMI_WR_DAT_63_18		UINT64_C(0xfffffffffffc0000)
#define SMI_WR_DAT_PENDING		UINT64_C(0x0000000000020000)
#define SMI_WR_DAT_VAL			UINT64_C(0x0000000000010000)
#define SMI_WR_DAT_DAT			UINT64_C(0x000000000000ffff)

/* SMI_RD_DAT */
#define SMI_RD_DAT_63_18		UINT64_C(0xfffffffffffc0000)
#define SMI_RD_DAT_PENDING		UINT64_C(0x0000000000020000)
#define SMI_RD_DAT_VAL			UINT64_C(0x0000000000010000)
#define SMI_RD_DAT_DAT			UINT64_C(0x000000000000ffff)

/* SMI_CLK */
#define SMI_CLK_63_21			UINT64_C(0xffffffffffe00000)
#define SMI_CLK_SAMPLE_HI		UINT64_C(0x00000000001f0000)
#define SMI_CLK_15_14			UINT64_C(0x000000000000c000)
#define SMI_CLK_CLK_IDLE		UINT64_C(0x0000000000002000)
#define SMI_CLK_PREAMBLE		UINT64_C(0x0000000000001000)
#define SMI_CLK_SAMPLE			UINT64_C(0x0000000000000f00)
#define SMI_CLK_PHASE			UINT64_C(0x00000000000000ff)

/* SMI_EN */
#define SMI_EN_63_1			UINT64_C(0xfffffffffffffffe)
#define SMI_EN_EN			UINT64_C(0x0000000000000001)

/* XXX */

#endif /* _OCTEON_SMIREG_H_ */
