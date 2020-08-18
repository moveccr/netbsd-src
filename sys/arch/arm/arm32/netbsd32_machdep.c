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

#include <sys/cdefs.h>

__KERNEL_RCSID(1, "$NetBSD: netbsd32_machdep.c,v 1.11 2020/06/20 07:10:36 skrll Exp $");

#ifdef _KERNEL_OPT
#include "opt_compat_netbsd.h"
#include "opt_compat_netbsd32.h"
#endif

#include <sys/param.h>

#include <sys/core.h>
#include <sys/exec.h>
#include <sys/lwp.h>
#include <sys/signalvar.h>
#include <sys/syscallargs.h>

#include <uvm/uvm_extern.h>

#include <compat/netbsd32/netbsd32.h>
#include <compat/netbsd32/netbsd32_exec.h>
#include <compat/netbsd32/netbsd32_syscallargs.h>

const char machine32[] = MACHINE;
const char machine_arch32[] = MACHINE_ARCH;

int
cpu_coredump32(struct lwp *l, struct coredump_iostate *iocookie,
    struct core32 *chdr)
{
	return cpu_coredump(l, iocookie, (struct core *)chdr);
}

void
netbsd32_sendsig (const ksiginfo_t *ksi, const sigset_t *ss)
{
	sendsig(ksi, ss);
}

void
startlwp32(void *arg)
{
	startlwp(arg);
}

int
cpu_mcontext32_validate(struct lwp *l, const mcontext32_t *mcp)
{
	return cpu_mcontext_validate(l, mcp);
}
void
cpu_getmcontext32(struct lwp *l, mcontext32_t *mcp, unsigned int *flagsp)
{
	cpu_getmcontext(l, mcp, flagsp);
}

int
cpu_setmcontext32(struct lwp *l, const mcontext32_t *mcp, unsigned int flags)
{
	return cpu_setmcontext(l, mcp, flags);
}

int
netbsd32_sysarch(struct lwp *l, const struct netbsd32_sysarch_args *uap,
	register_t *retval)
{
	return sys_sysarch(l, (const struct sys_sysarch_args *)uap, retval);
}

vaddr_t
netbsd32_vm_default_addr(struct proc *p, vaddr_t base, vsize_t sz,
    int topdown)
{
	if (topdown)
		return VM_DEFAULT_ADDRESS_TOPDOWN(base, sz);
	else
		return VM_DEFAULT_ADDRESS_BOTTOMUP(base, sz);
}

void
netbsd32_machdep_md_init(void)
{

	/* nothing to do */
}

void
netbsd32_machdep_md_fini(void)
{

	/* nothing to do */
}

