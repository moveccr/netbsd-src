/* $NetBSD: cpuvar.h,v 1.6 2020/10/15 01:00:01 thorpej Exp $ */

/*-
 * Copyright (c) 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe.
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

#include "opt_multiprocessor.h"

struct cpu_softc {
	device_t sc_dev;		/* base device */
	struct cpu_info *sc_ci;		/* our cpu_info structure */

	/* Info exported to user-space. */
	uint32_t sc_major_type;		/* CPU major type */
	uint32_t sc_minor_type;		/* CPU minor type */
	bool sc_vax_fp;			/* supports VAX FP */
	bool sc_ieee_fp;		/* supports IEEE FP */
	bool sc_primary_eligible;	/* CPU is primary eligible */

	/* These are only valid if the CPU runs kernel code. */
	u_long sc_amask;		/* AMASK bits (inverted) */
	u_long sc_implver;		/* IMPLVER */

	struct sysctllog *sc_sysctllog;

	struct evcnt sc_evcnt_clock;	/* clock interrupts */
	struct evcnt sc_evcnt_device;	/* device interrupts */
#if defined(MULTIPROCESSOR)
	struct evcnt sc_evcnt_ipi;	/* interprocessor interrupts */
	struct evcnt sc_evcnt_which_ipi[ALPHA_NIPIS];
#endif
};
