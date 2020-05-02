/* $NetBSD: proc.h,v 1.6 2020/04/12 07:49:58 maxv Exp $ */

/*-
 * Copyright (c) 2014 The NetBSD Foundation, Inc.
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

#ifndef _AARCH64_PROC_H_
#define _AARCH64_PROC_H_

#ifdef __aarch64__

#ifdef _KERNEL_OPT
#include "opt_compat_netbsd32.h"
#endif

struct mdlwp {
	void *md_onfault;
	struct trapframe *md_utf;
	uint64_t md_cpacr;
	uint32_t md_flags;

	uint64_t md_ia_kern_lo; /* APIAKeyLo_EL1 */
	uint64_t md_ia_kern_hi; /* APIAKeyHi_EL1 */

	uint64_t md_ia_user_lo; /* APIAKeyLo_EL1 */
	uint64_t md_ia_user_hi; /* APIAKeyHi_EL1 */
};

struct mdproc {
	void (*md_syscall)(struct trapframe *);
	char md_march32[12];	/* machine arch of executable */
};

#ifdef COMPAT_NETBSD32
#define PROC0_MD_INITIALIZERS	.p_md = { .md_march32 = MACHINE32_ARCH },
#endif

#elif defined(__arm__)

#include <arm/proc.h>

#endif /* __aarch64__/__arm__ */

#endif /* _AARCH64_PROC_H_ */
