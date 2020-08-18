/*	$NetBSD: octeon_fpareg.h,v 1.5 2020/06/23 05:14:18 simonb Exp $	*/

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
 * FPA Registers
 */

#ifndef _OCTEON_FPAREG_H_
#define _OCTEON_FPAREG_H_

/* ---- register offsets */

#define	FPA_INT_SUM				0x0001180028000040ULL
#define	FPA_INT_ENB				0x0001180028000048ULL
#define	FPA_CTL_STATUS				0x0001180028000050ULL
#define	FPA_QUE0_AVAILABLE			0x0001180028000098ULL
#define	FPA_QUE1_AVAILABLE			0x00011800280000a0ULL
#define	FPA_QUE2_AVAILABLE			0x00011800280000a8ULL
#define	FPA_QUE3_AVAILABLE			0x00011800280000b0ULL
#define	FPA_QUE4_AVAILABLE			0x00011800280000b8ULL
#define	FPA_QUE5_AVAILABLE			0x00011800280000c0ULL
#define	FPA_QUE6_AVAILABLE			0x00011800280000c8ULL
#define	FPA_QUE7_AVAILABLE			0x00011800280000d0ULL
#define	FPA_WART_CTL				0x00011800280000d8ULL
#define	FPA_WART_STATUS				0x00011800280000e0ULL
#define	FPA_BIST_STATUS				0x00011800280000e8ULL
#define	FPA_QUE0_PAGE_INDEX			0x00011800280000f0ULL
#define	FPA_QUE1_PAGE_INDEX			0x00011800280000f8ULL
#define	FPA_QUE2_PAGE_INDEX			0x0001180028000100ULL
#define	FPA_QUE3_PAGE_INDEX			0x0001180028000108ULL
#define	FPA_QUE4_PAGE_INDEX			0x0001180028000110ULL
#define	FPA_QUE5_PAGE_INDEX			0x0001180028000118ULL
#define	FPA_QUE6_PAGE_INDEX			0x0001180028000120ULL
#define	FPA_QUE7_PAGE_INDEX			0x0001180028000128ULL
#define	FPA_QUE_EXP				0x0001180028000130ULL
#define	FPA_QUE_ACT				0x0001180028000138ULL

/* ---- register bit definitions */

#define	FPA_INT_SUM_XXX_63_28			UINT64_C(0xfffffffff0000000)
#define	FPA_INT_SUM_Q7_PERR			UINT64_C(0x0000000008000000)
#define	FPA_INT_SUM_Q7_COFF			UINT64_C(0x0000000004000000)
#define	FPA_INT_SUM_Q7_UND			UINT64_C(0x0000000002000000)
#define	FPA_INT_SUM_Q6_PERR			UINT64_C(0x0000000001000000)
#define	FPA_INT_SUM_Q6_COFF			UINT64_C(0x0000000000800000)
#define	FPA_INT_SUM_Q6_UND			UINT64_C(0x0000000000400000)
#define	FPA_INT_SUM_Q5_PERR			UINT64_C(0x0000000000200000)
#define	FPA_INT_SUM_Q5_COFF			UINT64_C(0x0000000000100000)
#define	FPA_INT_SUM_Q5_UND			UINT64_C(0x0000000000080000)
#define	FPA_INT_SUM_Q4_PERR			UINT64_C(0x0000000000040000)
#define	FPA_INT_SUM_Q4_COFF			UINT64_C(0x0000000000020000)
#define	FPA_INT_SUM_Q4_UND			UINT64_C(0x0000000000010000)
#define	FPA_INT_SUM_Q3_PERR			UINT64_C(0x0000000000008000)
#define	FPA_INT_SUM_Q3_COFF			UINT64_C(0x0000000000004000)
#define	FPA_INT_SUM_Q3_UND			UINT64_C(0x0000000000002000)
#define	FPA_INT_SUM_Q2_PERR			UINT64_C(0x0000000000001000)
#define	FPA_INT_SUM_Q2_COFF			UINT64_C(0x0000000000000800)
#define	FPA_INT_SUM_Q2_UND			UINT64_C(0x0000000000000400)
#define	FPA_INT_SUM_Q1_PERR			UINT64_C(0x0000000000000200)
#define	FPA_INT_SUM_Q1_COFF			UINT64_C(0x0000000000000100)
#define	FPA_INT_SUM_Q1_UND			UINT64_C(0x0000000000000080)
#define	FPA_INT_SUM_Q0_PERR			UINT64_C(0x0000000000000040)
#define	FPA_INT_SUM_Q0_COFF			UINT64_C(0x0000000000000020)
#define	FPA_INT_SUM_Q0_UND			UINT64_C(0x0000000000000010)
#define	FPA_INT_SUM_FED1_DBE			UINT64_C(0x0000000000000008)
#define	FPA_INT_SUM_FED1_SBE			UINT64_C(0x0000000000000004)
#define	FPA_INT_SUM_FED0_DBE			UINT64_C(0x0000000000000002)
#define	FPA_INT_SUM_FED0_SBE			UINT64_C(0x0000000000000001)

#define	FPA_INT_ENB_XXX_63_28			UINT64_C(0xfffffffff0000000)
#define	FPA_INT_ENB_Q7_PERR			UINT64_C(0x0000000008000000)
#define	FPA_INT_ENB_Q7_COFF			UINT64_C(0x0000000004000000)
#define	FPA_INT_ENB_Q7_UND			UINT64_C(0x0000000002000000)
#define	FPA_INT_ENB_Q6_PERR			UINT64_C(0x0000000001000000)
#define	FPA_INT_ENB_Q6_COFF			UINT64_C(0x0000000000800000)
#define	FPA_INT_ENB_Q6_UND			UINT64_C(0x0000000000400000)
#define	FPA_INT_ENB_Q5_PERR			UINT64_C(0x0000000000200000)
#define	FPA_INT_ENB_Q5_COFF			UINT64_C(0x0000000000100000)
#define	FPA_INT_ENB_Q5_UND			UINT64_C(0x0000000000080000)
#define	FPA_INT_ENB_Q4_PERR			UINT64_C(0x0000000000040000)
#define	FPA_INT_ENB_Q4_COFF			UINT64_C(0x0000000000020000)
#define	FPA_INT_ENB_Q4_UND			UINT64_C(0x0000000000010000)
#define	FPA_INT_ENB_Q3_PERR			UINT64_C(0x0000000000008000)
#define	FPA_INT_ENB_Q3_COFF			UINT64_C(0x0000000000004000)
#define	FPA_INT_ENB_Q3_UND			UINT64_C(0x0000000000002000)
#define	FPA_INT_ENB_Q2_PERR			UINT64_C(0x0000000000001000)
#define	FPA_INT_ENB_Q2_COFF			UINT64_C(0x0000000000000800)
#define	FPA_INT_ENB_Q2_UND			UINT64_C(0x0000000000000400)
#define	FPA_INT_ENB_Q1_PERR			UINT64_C(0x0000000000000200)
#define	FPA_INT_ENB_Q1_COFF			UINT64_C(0x0000000000000100)
#define	FPA_INT_ENB_Q1_UND			UINT64_C(0x0000000000000080)
#define	FPA_INT_ENB_Q0_PERR			UINT64_C(0x0000000000000040)
#define	FPA_INT_ENB_Q0_COFF			UINT64_C(0x0000000000000020)
#define	FPA_INT_ENB_Q0_UND			UINT64_C(0x0000000000000010)
#define	FPA_INT_ENB_FED1_DBE			UINT64_C(0x0000000000000008)
#define	FPA_INT_ENB_FED1_SBE			UINT64_C(0x0000000000000004)
#define	FPA_INT_ENB_FED0_DBE			UINT64_C(0x0000000000000002)
#define	FPA_INT_ENB_FED0_SBE			UINT64_C(0x0000000000000001)

#define	FPA_CTL_STATUS_XXX_63_18		UINT64_C(0xfffffffffffc0000)
#define	FPA_CTL_STATUS_RESET			UINT64_C(0x0000000000020000)
#define	FPA_CTL_STATUS_USE_LDT			UINT64_C(0x0000000000010000)
#define	FPA_CTL_STATUS_USE_STT			UINT64_C(0x0000000000008000)
#define	FPA_CTL_STATUS_ENB			UINT64_C(0x0000000000004000)
#define	FPA_CTL_STATUS_MEM1_ERR			UINT64_C(0x0000000000003f80)
#define	FPA_CTL_STATUS_MEM0_ERR			UINT64_C(0x000000000000007f)

#define	FPA_QUEX_AVAILABLE_XXX_63_29		UINT64_C(0xffffffffe0000000)
#define	FPA_QUEX_AVAILABLE_QUE_SIZ		UINT64_C(0x000000001fffffff)

#define	FPA_WART_CTL_XXX_63_16			UINT64_C(0xffffffffffff0000)
#define	FPA_WART_CTL_CTL			UINT64_C(0x000000000000ffff)

#define	FPA_WART_STATUS_XXX_63_32		UINT64_C(0xffffffff00000000)
#define	FPA_WART_STATUS_STATUS			UINT64_C(0x00000000ffffffff)

#define	FPA_BIST_STATUS_XXX_63_5		UINT64_C(0xffffffffffffffe0)
#define	FPA_BIST_STATUS_FRD			UINT64_C(0x0000000000000010)
#define	FPA_BIST_STATUS_FPF0			UINT64_C(0x0000000000000008)
#define	FPA_BIST_STATUS_FPF1			UINT64_C(0x0000000000000004)
#define	FPA_BIST_STATUS_FFR			UINT64_C(0x0000000000000002)
#define	FPA_BIST_STATUS_FDR			UINT64_C(0x0000000000000001)

#define	FPA_QUEX_PAGE_INDEX_XXX_63_25		UINT64_C(0xfffffffffe000000)
#define	FPA_QUEX_PAGE_INDEX_PG_NUM		UINT64_C(0x0000000001ffffff)

#define	FPA_QUE_EXP_XXX_63_32			UINT64_C(0xffffffff00000000)
#define	FPA_QUE_EXP_XXX_31_29			UINT64_C(0x00000000e0000000)
#define	FPA_QUE_EXP_EXP_QUE			UINT64_C(0x000000001c000000)
#define	FPA_QUE_EXP_EXP_INDX			UINT64_C(0x0000000003ffffff)

#define	FPA_QUE_ACT_XXX_63_32			UINT64_C(0xffffffff00000000)
#define	FPA_QUE_ACT_XXX_31_29			UINT64_C(0x00000000e0000000)
#define	FPA_QUE_ACT_ACT_QUE			UINT64_C(0x000000001c000000)
#define	FPA_QUE_ACT_ACT_INDX			UINT64_C(0x0000000003ffffff)

/* ---- operations */

/*
 * Free Pool Unit Operations
 */

#define	FPA_MAJOR_DID				0x5

/* Store Operations */
#define	FPA_OPS_STORE_ADDR			UINT64_C(0x000000ffffffffff)

#define	FPA_OPS_STORE_DATA_DWBCOUNT		UINT64_C(0x00000000000001ff)

/* ---- bus_space(9) */

#define	FPA_BASE				0x0001180028000000ULL
#define	FPA_SIZE				0x0200

#define	FPA_NPOOLS				8

#define	FPA_INT_SUM_OFFSET			0x0040
#define	FPA_INT_ENB_OFFSET			0x0048
#define	FPA_CTL_STATUS_OFFSET			0x0050
#define	FPA_QUE0_AVAILABLE_OFFSET		0x0098
#define	FPA_QUE1_AVAILABLE_OFFSET		0x00a0
#define	FPA_QUE2_AVAILABLE_OFFSET		0x00a8
#define	FPA_QUE3_AVAILABLE_OFFSET		0x00b0
#define	FPA_QUE4_AVAILABLE_OFFSET		0x00b8
#define	FPA_QUE5_AVAILABLE_OFFSET		0x00c0
#define	FPA_QUE6_AVAILABLE_OFFSET		0x00c8
#define	FPA_QUE7_AVAILABLE_OFFSET		0x00d0
#define	FPA_WART_CTL_OFFSET			0x00d8
#define	FPA_WART_STATUS_OFFSET			0x00e0
#define	FPA_BIST_STATUS_OFFSET			0x00e8
#define	FPA_QUE0_PAGE_INDEX_OFFSET		0x00f0
#define	FPA_QUE1_PAGE_INDEX_OFFSET		0x00f8
#define	FPA_QUE2_PAGE_INDEX_OFFSET		0x0100
#define	FPA_QUE3_PAGE_INDEX_OFFSET		0x0108
#define	FPA_QUE4_PAGE_INDEX_OFFSET		0x0110
#define	FPA_QUE5_PAGE_INDEX_OFFSET		0x0118
#define	FPA_QUE6_PAGE_INDEX_OFFSET		0x0120
#define	FPA_QUE7_PAGE_INDEX_OFFSET		0x0128
#define	FPA_QUE_EXP_OFFSET			0x0130
#define	FPA_QUE_ACT_OFFSET			0x0138

#endif /* _OCTEON_FPAREG_H_ */
