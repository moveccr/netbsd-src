/*	$NetBSD: netbsd32_machdep.c,v 1.118 2019/01/27 02:08:37 pgoyette Exp $	*/

/*
 * Copyright (c) 2001 Wasabi Systems, Inc.
 * All rights reserved.
 *
 * Written by Frank van der Linden for Wasabi Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed for the NetBSD Project by
 *      Wasabi Systems, Inc.
 * 4. The name of Wasabi Systems, Inc. may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY WASABI SYSTEMS, INC. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL WASABI SYSTEMS, INC
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: netbsd32_machdep.c,v 1.118 2019/01/27 02:08:37 pgoyette Exp $");

#ifdef _KERNEL_OPT
#include "opt_compat_netbsd.h"
#include "opt_compat_netbsd32.h"
#include "opt_coredump.h"
#include "opt_execfmt.h"
#include "opt_user_ldt.h"
#include "opt_mtrr.h"
#endif

#include <sys/param.h>
#include <sys/exec.h>
#include <sys/exec_aout.h>
#include <sys/kmem.h>
#include <sys/malloc.h>
#include <sys/proc.h>
#include <sys/signalvar.h>
#include <sys/systm.h>
#include <sys/core.h>
#include <sys/mount.h>
#include <sys/buf.h>
#include <sys/vnode.h>
#include <sys/ras.h>
#include <sys/ptrace.h>
#include <sys/kauth.h>
#include <sys/compat_stub.h>

#include <x86/fpu.h>
#include <x86/dbregs.h>
#include <machine/frame.h>
#include <machine/reg.h>
#include <machine/vmparam.h>
#ifdef MTRR
#include <machine/mtrr.h>
#endif
#include <machine/netbsd32_machdep.h>
#include <machine/sysarch.h>
#include <machine/userret.h>

#include <compat/netbsd32/netbsd32.h>
#include <compat/netbsd32/netbsd32_exec.h>
#include <compat/netbsd32/netbsd32_syscallargs.h>

#include <compat/sys/signal.h>
#include <compat/sys/signalvar.h>

/* Provide a the name of the architecture we're emulating */
const char	machine32[] = "i386";
const char	machine_arch32[] = "i386";	

#ifdef USER_LDT
static int x86_64_get_ldt32(struct lwp *, void *, register_t *);
static int x86_64_set_ldt32(struct lwp *, void *, register_t *);
#else
#define x86_64_get_ldt32(x, y, z)	ENOSYS
#define x86_64_set_ldt32(x, y, z)	ENOSYS
#endif

#ifdef MTRR
static int x86_64_get_mtrr32(struct lwp *, void *, register_t *);
static int x86_64_set_mtrr32(struct lwp *, void *, register_t *);
#else
#define x86_64_get_mtrr32(x, y, z)	ENOSYS
#define x86_64_set_mtrr32(x, y, z)	ENOSYS
#endif

int check_sigcontext32(struct lwp *, const struct netbsd32_sigcontext *);

void  netbsd32_buildcontext(struct lwp *l, struct trapframe *tf, void *fp,
    sig_t catcher, int onstack);

int netbsd32_sendsig_siginfo(const ksiginfo_t *, const sigset_t *);

#ifdef EXEC_AOUT
/*
 * There is no native a.out -- this function is required
 * for i386 a.out emulation (COMPAT_NETBSD32+EXEC_AOUT).
 */
int
cpu_exec_aout_makecmds(struct lwp *p, struct exec_package *e)
{

	return ENOEXEC;
}
#endif

void
netbsd32_setregs(struct lwp *l, struct exec_package *pack, vaddr_t stack)
{
	struct pcb *pcb;
	struct trapframe *tf;
	struct proc *p = l->l_proc;

	pcb = lwp_getpcb(l);

#if defined(USER_LDT)
	pmap_ldt_cleanup(l);
#endif

	netbsd32_adjust_limits(p);

	l->l_md.md_flags = MDL_COMPAT32;	/* Force iret not sysret */
	pcb->pcb_flags = PCB_COMPAT32;

	fpu_save_area_clear(l, pack->ep_osversion >= 699002600
	    ?  __NetBSD_NPXCW__ : __NetBSD_COMPAT_NPXCW__);

	x86_dbregs_clear(l);

	p->p_flag |= PK_32;

	tf = l->l_md.md_regs;
	tf->tf_ds = LSEL(LUDATA32_SEL, SEL_UPL);
	tf->tf_es = LSEL(LUDATA32_SEL, SEL_UPL);
	cpu_segregs32_zero(l);
	cpu_fsgs_reload(l, tf->tf_ds, tf->tf_es);
	tf->tf_rdi = 0;
	tf->tf_rsi = 0;
	tf->tf_rbp = 0;
	tf->tf_rbx = (uint32_t)p->p_psstrp;
	tf->tf_rdx = 0;
	tf->tf_rcx = 0;
	tf->tf_rax = 0;
	tf->tf_rip = pack->ep_entry;
	tf->tf_cs = LSEL(LUCODE32_SEL, SEL_UPL);
	tf->tf_rflags = PSL_USERSET;
	tf->tf_rsp = stack;
	tf->tf_ss = LSEL(LUDATA32_SEL, SEL_UPL);
}

void
netbsd32_buildcontext(struct lwp *l, struct trapframe *tf, void *fp,
    sig_t catcher, int onstack)
{
	/*
	 * Build context to run handler in.
	 */
	tf->tf_ds = GSEL(GUDATA32_SEL, SEL_UPL);
	tf->tf_es = GSEL(GUDATA32_SEL, SEL_UPL);
#if 0
	tf->tf_fs = GSEL(GUDATA32_SEL, SEL_UPL);
	tf->tf_gs = GSEL(GUDATA32_SEL, SEL_UPL);
#endif

	/* Ensure FP state is sane. */
	fpu_save_area_reset(l);

	tf->tf_rip = (uint64_t)catcher;
	tf->tf_cs = GSEL(GUCODE32_SEL, SEL_UPL);
	tf->tf_rflags &= ~PSL_CLEARSIG;
	tf->tf_rsp = (uint64_t)fp;
	tf->tf_ss = GSEL(GUDATA32_SEL, SEL_UPL);

	/* Remember that we're now on the signal stack. */
	if (onstack)
		l->l_sigstk.ss_flags |= SS_ONSTACK;
	if ((vaddr_t)catcher >= VM_MAXUSER_ADDRESS32) {
		/*
		 * process has given an invalid address for the
		 * handler. Stop it, but do not do it before so
		 * we can return the right info to userland (or in core dump)
		 */
		sigexit(l, SIGILL);
		/* NOTREACHED */
	}
}

int
netbsd32_sendsig_siginfo(const ksiginfo_t *ksi, const sigset_t *mask)
{
	struct lwp *l = curlwp;
	struct proc *p = l->l_proc;
	struct sigacts *ps = p->p_sigacts;
	int onstack, error;
	int sig = ksi->ksi_signo;
	struct netbsd32_sigframe_siginfo *fp, frame;
	sig_t catcher = SIGACTION(p, sig).sa_handler;
	struct trapframe *tf = l->l_md.md_regs;

	/* Do we need to jump onto the signal stack? */
	onstack =
	    (l->l_sigstk.ss_flags & (SS_DISABLE | SS_ONSTACK)) == 0 &&
	    (SIGACTION(p, sig).sa_flags & SA_ONSTACK) != 0;

	/* Allocate space for the signal handler context. */
	if (onstack)
		fp = (struct netbsd32_sigframe_siginfo *)
		    ((char *)l->l_sigstk.ss_sp + l->l_sigstk.ss_size);
	else
		fp = (struct netbsd32_sigframe_siginfo *)tf->tf_rsp;

	fp--;

	/* Build stack frame for signal trampoline. */
	switch (ps->sa_sigdesc[sig].sd_vers) {
	case 0:		/* handled by sendsig_sigcontext */
	case 1:		/* handled by sendsig_sigcontext */
	default:	/* unknown version */
		printf("nsendsig: bad version %d\n",
		    ps->sa_sigdesc[sig].sd_vers);
		sigexit(l, SIGILL);
	case 2:
		break;
	}

	memset(&frame, 0, sizeof(frame));
	frame.sf_ra = (uint32_t)(uintptr_t)ps->sa_sigdesc[sig].sd_tramp;
	frame.sf_signum = sig;
	frame.sf_sip = (uint32_t)(uintptr_t)&fp->sf_si;
	frame.sf_ucp = (uint32_t)(uintptr_t)&fp->sf_uc;
	netbsd32_si_to_si32(&frame.sf_si, (const siginfo_t *)&ksi->ksi_info);
	frame.sf_uc.uc_flags = _UC_SIGMASK;
	frame.sf_uc.uc_sigmask = *mask;
	frame.sf_uc.uc_link = (uint32_t)(uintptr_t)l->l_ctxlink;
	frame.sf_uc.uc_flags |= (l->l_sigstk.ss_flags & SS_ONSTACK)
	    ? _UC_SETSTACK : _UC_CLRSTACK;
	sendsig_reset(l, sig);

	mutex_exit(p->p_lock);
	cpu_getmcontext32(l, &frame.sf_uc.uc_mcontext, &frame.sf_uc.uc_flags);
	error = copyout(&frame, fp, sizeof(frame));
	mutex_enter(p->p_lock);

	if (error != 0) {
		/*
		 * Process has trashed its stack; give it an illegal
		 * instruction to halt it in its tracks.
		 */
		sigexit(l, SIGILL);
		/* NOTREACHED */
	}

	netbsd32_buildcontext(l, tf, fp, catcher, onstack);

	return 0;
}

struct netbsd32_sendsig_hook_t netbsd32_sendsig_hook;

void
netbsd32_sendsig(const ksiginfo_t *ksi, const sigset_t *mask)
{

	MODULE_CALL_VOID_HOOK(netbsd32_sendsig_hook, (ksi, mask),
	    netbsd32_sendsig_siginfo(ksi, mask));
}

#ifdef COREDUMP
/*
 * Dump the machine specific segment at the start of a core dump.
 */     
struct md_core32 {
	struct reg32 intreg;
	struct fpreg32 freg;
};

int
cpu_coredump32(struct lwp *l, struct coredump_iostate *iocookie,
    struct core32 *chdr)
{
	struct md_core32 md_core;
	struct coreseg cseg;
	int error;

	if (iocookie == NULL) {
		CORE_SETMAGIC(*chdr, COREMAGIC, MID_I386, 0);
		chdr->c_hdrsize = ALIGN32(sizeof(*chdr));
		chdr->c_seghdrsize = ALIGN32(sizeof(cseg));
		chdr->c_cpusize = sizeof(md_core);
		chdr->c_nseg++;
		return 0;
	}

	/* Save integer registers. */
	error = netbsd32_process_read_regs(l, &md_core.intreg);
	if (error)
		return error;

	/* Save floating point registers. */
	error = netbsd32_process_read_fpregs(l, &md_core.freg, NULL);
	if (error)
		return error;

	CORE_SETMAGIC(cseg, CORESEGMAGIC, MID_I386, CORE_CPU);
	cseg.c_addr = 0;
	cseg.c_size = chdr->c_cpusize;

	error = coredump_write(iocookie, UIO_SYSSPACE, &cseg,
	    chdr->c_seghdrsize);
	if (error)
		return error;

	return coredump_write(iocookie, UIO_SYSSPACE, &md_core,
	    sizeof(md_core));
}
#endif

int
netbsd32_process_read_regs(struct lwp *l, struct reg32 *regs)
{
	struct trapframe *tf = l->l_md.md_regs;

	/* XXX avoid sign extension problems with unknown upper bits? */
	regs->r_gs = tf->tf_gs & 0xffff;
	regs->r_fs = tf->tf_fs & 0xffff;
	regs->r_es = tf->tf_es & 0xffff;
	regs->r_ds = tf->tf_ds & 0xffff;
	regs->r_eflags = tf->tf_rflags;
	regs->r_edi = tf->tf_rdi & 0xffffffff;
	regs->r_esi = tf->tf_rsi & 0xffffffff;
	regs->r_ebp = tf->tf_rbp & 0xffffffff;
	regs->r_ebx = tf->tf_rbx & 0xffffffff;
	regs->r_edx = tf->tf_rdx & 0xffffffff;
	regs->r_ecx = tf->tf_rcx & 0xffffffff;
	regs->r_eax = tf->tf_rax & 0xffffffff;
	regs->r_eip = tf->tf_rip & 0xffffffff;
	regs->r_cs = tf->tf_cs & 0xffff;
	regs->r_esp = tf->tf_rsp & 0xffffffff;
	regs->r_ss = tf->tf_ss & 0xffff;

	return (0);
}

int
netbsd32_process_read_fpregs(struct lwp *l, struct fpreg32 *regs, size_t *sz)
{

	__CTASSERT(sizeof *regs == sizeof (struct save87));
	process_read_fpregs_s87(l, (struct save87 *)regs);
	return 0;
}

int
netbsd32_process_read_dbregs(struct lwp *l, struct dbreg32 *regs, size_t *sz)
{
#if notyet
	struct pcb *pcb;

	pcb = lwp_getpcb(l);

	regs->dr[0] = pcb->pcb_dbregs->dr[0] & 0xffffffff;
	regs->dr[1] = pcb->pcb_dbregs->dr[1] & 0xffffffff;
	regs->dr[2] = pcb->pcb_dbregs->dr[2] & 0xffffffff;
	regs->dr[3] = pcb->pcb_dbregs->dr[3] & 0xffffffff;

	regs->dr[6] = pcb->pcb_dbregs->dr[6] & 0xffffffff;
	regs->dr[7] = pcb->pcb_dbregs->dr[7] & 0xffffffff;

	return 0;
#else
	return ENOTSUP;
#endif
}

int
netbsd32_process_write_regs(struct lwp *l, const struct reg32 *regs)
{
	struct trapframe *tf;
	struct pcb *pcb;

	tf = l->l_md.md_regs;
	pcb = lwp_getpcb(l);

	/*
	 * Check for security violations.
	 */
	if (((regs->r_eflags ^ tf->tf_rflags) & PSL_USERSTATIC) != 0)
		return EINVAL;
	if (!VALID_USER_CSEL32(regs->r_cs))
		return EINVAL;
	if (regs->r_fs != 0 && !VALID_USER_DSEL32(regs->r_fs) &&
	    !(VALID_USER_FSEL32(regs->r_fs) && pcb->pcb_fs != 0))
		return EINVAL;
	if (regs->r_gs != 0 && !VALID_USER_DSEL32(regs->r_gs) &&
	    !(VALID_USER_GSEL32(regs->r_gs) && pcb->pcb_gs != 0))
		return EINVAL;
	if (regs->r_es != 0 && !VALID_USER_DSEL32(regs->r_es))
		return EINVAL;
	if (!VALID_USER_DSEL32(regs->r_ds) ||
	    !VALID_USER_DSEL32(regs->r_ss))
		return EINVAL;
	if (regs->r_eip >= VM_MAXUSER_ADDRESS32)
		return EINVAL;

	tf->tf_rax = regs->r_eax;
	tf->tf_rcx = regs->r_ecx;
	tf->tf_rdx = regs->r_edx;
	tf->tf_rbx = regs->r_ebx;
	tf->tf_rsp = regs->r_esp;
	tf->tf_rbp = regs->r_ebp;
	tf->tf_rsi = regs->r_esi;
	tf->tf_rdi = regs->r_edi;
	tf->tf_rip = regs->r_eip;
	tf->tf_rflags = regs->r_eflags;
	tf->tf_cs = regs->r_cs & 0xFFFF;
	tf->tf_ss = regs->r_ss & 0xFFFF;
	tf->tf_ds = regs->r_ds & 0xFFFF;
	tf->tf_es = regs->r_es & 0xFFFF;
	tf->tf_fs = regs->r_fs & 0xFFFF;
	tf->tf_gs = regs->r_gs & 0xFFFF;

	return 0;
}

int
netbsd32_process_write_fpregs(struct lwp *l, const struct fpreg32 *regs,
    size_t sz)
{

	__CTASSERT(sizeof *regs == sizeof (struct save87));
	process_write_fpregs_s87(l, (const struct save87 *)regs);
	return 0;
}

int
netbsd32_process_write_dbregs(struct lwp *l, const struct dbreg32 *regs,
    size_t sz)
{
#if notyet
	struct pcb *pcb;

	pcb = lwp_getpcb(l);

	pcb->pcb_dbregs->dr[0] = regs->dr[0];
	pcb->pcb_dbregs->dr[1] = regs->dr[1];
	pcb->pcb_dbregs->dr[2] = regs->dr[2];
	pcb->pcb_dbregs->dr[3] = regs->dr[3];

	pcb->pcb_dbregs->dr[6] = regs->dr[6];
	pcb->pcb_dbregs->dr[7] = regs->dr[7];

	return 0;
#else
	return ENOTSUP;
#endif
}

int
netbsd32_sysarch(struct lwp *l, const struct netbsd32_sysarch_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) op;
		syscallarg(netbsd32_voidp) parms;
	} */
	int error;

	switch (SCARG(uap, op)) {
	case X86_IOPL:
		error = x86_iopl(l,
		    NETBSD32PTR64(SCARG(uap, parms)), retval);
		break;
	case X86_GET_LDT: 
		error = x86_64_get_ldt32(l,
		    NETBSD32PTR64(SCARG(uap, parms)), retval);
		break;
	case X86_SET_LDT: 
		error = x86_64_set_ldt32(l,
		    NETBSD32PTR64(SCARG(uap, parms)), retval);
		break;
	case X86_GET_MTRR:
		error = x86_64_get_mtrr32(l,
		    NETBSD32PTR64(SCARG(uap, parms)), retval);
		break;
	case X86_SET_MTRR:
		error = x86_64_set_mtrr32(l,
		    NETBSD32PTR64(SCARG(uap, parms)), retval);
		break;
	default:
		error = EINVAL;
		break;
	}
	return error;
}

#ifdef USER_LDT
static int
x86_64_set_ldt32(struct lwp *l, void *args, register_t *retval)
{
	struct x86_set_ldt_args32 ua32;
	struct x86_set_ldt_args ua;
	union descriptor *descv;
	int error;

	if ((error = copyin(args, &ua32, sizeof(ua32))) != 0)
		return (error);

	ua.start = ua32.start;
	ua.num = ua32.num;

	if (ua.num < 0 || ua.num > 8192)
		return EINVAL;

	descv = malloc(sizeof(*descv) * ua.num, M_TEMP, M_NOWAIT);
	if (descv == NULL)
		return ENOMEM;

	error = copyin((void *)(uintptr_t)ua32.desc, descv,
	    sizeof(*descv) * ua.num);
	if (error == 0)
		error = x86_set_ldt1(l, &ua, descv);
	*retval = ua.start;

	free(descv, M_TEMP);
	return error;
}

static int
x86_64_get_ldt32(struct lwp *l, void *args, register_t *retval)
{
	struct x86_get_ldt_args32 ua32;
	struct x86_get_ldt_args ua;
	union descriptor *cp;
	int error;

	if ((error = copyin(args, &ua32, sizeof(ua32))) != 0)
		return error;

	ua.start = ua32.start;
	ua.num = ua32.num;

	if (ua.num < 0 || ua.num > 8192)
		return EINVAL;

	cp = malloc(ua.num * sizeof(union descriptor), M_TEMP, M_WAITOK);
	if (cp == NULL)
		return ENOMEM;

	error = x86_get_ldt1(l, &ua, cp);
	*retval = ua.num;
	if (error == 0)
		error = copyout(cp, (void *)(uintptr_t)ua32.desc,
		    ua.num * sizeof(*cp));

	free(cp, M_TEMP);
	return error;
}
#endif

#ifdef MTRR
static int
x86_64_get_mtrr32(struct lwp *l, void *args, register_t *retval)
{
	struct x86_64_get_mtrr_args32 args32;
	int error, i;
	int32_t n;
	struct mtrr32 *m32p, m32;
	struct mtrr *m64p, *mp;
	size_t size;

	m64p = NULL;

	if (mtrr_funcs == NULL)
		return ENOSYS;

	error = kauth_authorize_machdep(l->l_cred, KAUTH_MACHDEP_MTRR_GET,
	    NULL, NULL, NULL, NULL);
	if (error)
		return (error);

	error = copyin(args, &args32, sizeof args32);
	if (error != 0)
		return error;

	if (args32.mtrrp == 0) {
		n = (MTRR_I686_NFIXED_SOFT + MTRR_I686_NVAR_MAX);
		return copyout(&n, (void *)(uintptr_t)args32.n, sizeof n);
	}

	error = copyin((void *)(uintptr_t)args32.n, &n, sizeof n);
	if (error != 0)
		return error;

	if (n <= 0 || n > (MTRR_I686_NFIXED_SOFT + MTRR_I686_NVAR_MAX))
		return EINVAL;

	size = n * sizeof(struct mtrr);
	m64p = kmem_zalloc(size, KM_SLEEP);
	error = mtrr_get(m64p, &n, l->l_proc, 0);
	if (error != 0)
		goto fail;
	m32p = (struct mtrr32 *)(uintptr_t)args32.mtrrp;
	mp = m64p;
	for (i = 0; i < n; i++) {
		m32.base = mp->base;
		m32.len = mp->len;
		m32.type = mp->type;
		m32.flags = mp->flags;
		m32.owner = mp->owner;
		error = copyout(&m32, m32p, sizeof m32);
		if (error != 0)
			break;
		mp++;
		m32p++;
	}
fail:
	if (m64p != NULL)
		kmem_free(m64p, size);
	if (error != 0)
		n = 0;
	copyout(&n, (void *)(uintptr_t)args32.n, sizeof n);
	return error;
}

static int
x86_64_set_mtrr32(struct lwp *l, void *args, register_t *retval)
{
	struct x86_64_set_mtrr_args32 args32;
	struct mtrr32 *m32p, m32;
	struct mtrr *m64p, *mp;
	int error, i;
	int32_t n;
	size_t size;

	m64p = NULL;

	if (mtrr_funcs == NULL)
		return ENOSYS;

	error = kauth_authorize_machdep(l->l_cred, KAUTH_MACHDEP_MTRR_SET,
	    NULL, NULL, NULL, NULL);
	if (error)
		return (error);

	error = copyin(args, &args32, sizeof args32);
	if (error != 0)
		return error;

	error = copyin((void *)(uintptr_t)args32.n, &n, sizeof n);
	if (error != 0)
		return error;

	if (n <= 0 || n > (MTRR_I686_NFIXED_SOFT + MTRR_I686_NVAR_MAX)) {
		error = EINVAL;
		goto fail;
	}

	size = n * sizeof(struct mtrr);
	m64p = kmem_zalloc(size, KM_SLEEP);
	m32p = (struct mtrr32 *)(uintptr_t)args32.mtrrp;
	mp = m64p;
	for (i = 0; i < n; i++) {
		error = copyin(m32p, &m32, sizeof m32);
		if (error != 0)
			goto fail;
		mp->base = m32.base;
		mp->len = m32.len;
		mp->type = m32.type;
		mp->flags = m32.flags;
		mp->owner = m32.owner;
		m32p++;
		mp++;
	}

	error = mtrr_set(m64p, &n, l->l_proc, 0);
fail:
	if (m64p != NULL)
		kmem_free(m64p, size);
	if (error != 0)
		n = 0;
	copyout(&n, (void *)(uintptr_t)args32.n, sizeof n);
	return error;
}
#endif

#if 0
void
netbsd32_mcontext_to_mcontext32(mcontext32_t *m32, mcontext_t *m, int flags)
{
	if ((flags & _UC_CPU) != 0) {
		m32->__gregs[_REG32_GS] = m->__gregs[_REG_GS] & 0xffffffff;
		m32->__gregs[_REG32_FS] = m->__gregs[_REG_FS] & 0xffffffff;
		m32->__gregs[_REG32_ES] = m->__gregs[_REG_ES] & 0xffffffff;
		m32->__gregs[_REG32_DS] = m->__gregs[_REG_DS] & 0xffffffff;
		m32->__gregs[_REG32_EDI] = m->__gregs[_REG_RDI] & 0xffffffff;
		m32->__gregs[_REG32_ESI] = m->__gregs[_REG_RSI] & 0xffffffff;
		m32->__gregs[_REG32_EBP] = m->__gregs[_REG_RBP] & 0xffffffff;
		m32->__gregs[_REG32_ESP] = m->__gregs[_REG_URSP] & 0xffffffff;
		m32->__gregs[_REG32_EBX] = m->__gregs[_REG_RBX] & 0xffffffff;
		m32->__gregs[_REG32_EDX] = m->__gregs[_REG_RDX] & 0xffffffff;
		m32->__gregs[_REG32_ECX] = m->__gregs[_REG_RCX] & 0xffffffff;
		m32->__gregs[_REG32_EAX] = m->__gregs[_REG_RAX] & 0xffffffff;
		m32->__gregs[_REG32_TRAPNO] =
		    m->__gregs[_REG_TRAPNO] & 0xffffffff;
		m32->__gregs[_REG32_ERR] = m->__gregs[_REG_ERR] & 0xffffffff;
		m32->__gregs[_REG32_EIP] = m->__gregs[_REG_RIP] & 0xffffffff;
		m32->__gregs[_REG32_CS] = m->__gregs[_REG_CS] & 0xffffffff;
		m32->__gregs[_REG32_EFL] = m->__gregs[_REG_RFL] & 0xffffffff;
		m32->__gregs[_REG32_UESP] = m->__gregs[_REG_URSP] & 0xffffffff;
		m32->__gregs[_REG32_SS] = m->__gregs[_REG_SS] & 0xffffffff;
	}
	if ((flags & _UC_FPU) != 0)
		memcpy(&m32->__fpregs, &m->__fpregs, sizeof (m32->__fpregs));
}

void
netbsd32_mcontext32_to_mcontext(mcontext_t *m, mcontext32_t *m32, int flags)
{
	if ((flags & _UC_CPU) != 0) {
		m->__gregs[_REG_GS] = m32->__gregs[_REG32_GS];
		m->__gregs[_REG_FS] = m32->__gregs[_REG32_FS];
		m->__gregs[_REG_ES] = m32->__gregs[_REG32_ES];
		m->__gregs[_REG_DS] = m32->__gregs[_REG32_DS];
		m->__gregs[_REG_RDI] = m32->__gregs[_REG32_EDI];
		m->__gregs[_REG_RSI] = m32->__gregs[_REG32_ESI];
		m->__gregs[_REG_RBP] = m32->__gregs[_REG32_EBP];
		m->__gregs[_REG_URSP] = m32->__gregs[_REG32_ESP];
		m->__gregs[_REG_RBX] = m32->__gregs[_REG32_EBX];
		m->__gregs[_REG_RDX] = m32->__gregs[_REG32_EDX];
		m->__gregs[_REG_RCX] = m32->__gregs[_REG32_ECX];
		m->__gregs[_REG_RAX] = m32->__gregs[_REG32_EAX];
		m->__gregs[_REG_TRAPNO] = m32->__gregs[_REG32_TRAPNO];
		m->__gregs[_REG_ERR] = m32->__gregs[_REG32_ERR];
		m->__gregs[_REG_RIP] = m32->__gregs[_REG32_EIP];
		m->__gregs[_REG_CS] = m32->__gregs[_REG32_CS];
		m->__gregs[_REG_RFL] = m32->__gregs[_REG32_EFL];
		m->__gregs[_REG_URSP] = m32->__gregs[_REG32_UESP];
		m->__gregs[_REG_SS] = m32->__gregs[_REG32_SS];
	}
	if (flags & _UC_FPU)
		memcpy(&m->__fpregs, &m32->__fpregs, sizeof (m->__fpregs));
}
#endif


int
cpu_setmcontext32(struct lwp *l, const mcontext32_t *mcp, unsigned int flags)
{
	struct trapframe *tf = l->l_md.md_regs;
	const __greg32_t *gr = mcp->__gregs;
	struct proc *p = l->l_proc;
	int error;

	/* Restore register context, if any. */
	if ((flags & _UC_CPU) != 0) {
		/*
		 * Check for security violations.
		 */
		error = cpu_mcontext32_validate(l, mcp);
		if (error != 0)
			return error;

		cpu_fsgs_reload(l, gr[_REG32_FS], gr[_REG32_GS]);
		tf->tf_es = gr[_REG32_ES] & 0xFFFF;
		tf->tf_ds = gr[_REG32_DS] & 0xFFFF;
		/* Only change the user-alterable part of eflags */
		tf->tf_rflags &= ~PSL_USER;
		tf->tf_rflags |= (gr[_REG32_EFL] & PSL_USER);
		tf->tf_rdi    = gr[_REG32_EDI];
		tf->tf_rsi    = gr[_REG32_ESI];
		tf->tf_rbp    = gr[_REG32_EBP];
		tf->tf_rbx    = gr[_REG32_EBX];
		tf->tf_rdx    = gr[_REG32_EDX];
		tf->tf_rcx    = gr[_REG32_ECX];
		tf->tf_rax    = gr[_REG32_EAX];
		tf->tf_rip    = gr[_REG32_EIP];
		tf->tf_cs     = gr[_REG32_CS] & 0xFFFF;
		tf->tf_rsp    = gr[_REG32_UESP];
		tf->tf_ss     = gr[_REG32_SS] & 0xFFFF;
	}

	if ((flags & _UC_TLSBASE) != 0)
		lwp_setprivate(l, (void *)(uintptr_t)mcp->_mc_tlsbase);

	/* Restore floating point register context, if any. */
	if ((flags & _UC_FPU) != 0) {
		/* Assume fxsave context */
		process_write_fpregs_xmm(l, (const struct fxsave *)
		    &mcp->__fpregs.__fp_reg_set.__fp_xmm_state);
	}

	mutex_enter(p->p_lock);
	if (flags & _UC_SETSTACK)
		l->l_sigstk.ss_flags |= SS_ONSTACK;
	if (flags & _UC_CLRSTACK)
		l->l_sigstk.ss_flags &= ~SS_ONSTACK;
	mutex_exit(p->p_lock);

	return (0);
}

void
cpu_getmcontext32(struct lwp *l, mcontext32_t *mcp, unsigned int *flags)
{
	const struct trapframe *tf = l->l_md.md_regs;
	__greg32_t *gr = mcp->__gregs;
	__greg32_t ras_eip;

	/* Save register context. */
	gr[_REG32_GS]  = tf->tf_gs & 0xFFFF;
	gr[_REG32_FS]  = tf->tf_fs & 0xFFFF;
	gr[_REG32_ES]  = tf->tf_es & 0xFFFF;
	gr[_REG32_DS]  = tf->tf_ds & 0xFFFF;
	gr[_REG32_EFL] = tf->tf_rflags;
	gr[_REG32_EDI]    = tf->tf_rdi;
	gr[_REG32_ESI]    = tf->tf_rsi;
	gr[_REG32_EBP]    = tf->tf_rbp;
	gr[_REG32_EBX]    = tf->tf_rbx;
	gr[_REG32_EDX]    = tf->tf_rdx;
	gr[_REG32_ECX]    = tf->tf_rcx;
	gr[_REG32_EAX]    = tf->tf_rax;
	gr[_REG32_EIP]    = tf->tf_rip;
	gr[_REG32_CS]     = tf->tf_cs & 0xFFFF;
	gr[_REG32_ESP]    = tf->tf_rsp;
	gr[_REG32_UESP]   = tf->tf_rsp;
	gr[_REG32_SS]     = tf->tf_ss & 0xFFFF;
	gr[_REG32_TRAPNO] = tf->tf_trapno;
	gr[_REG32_ERR]    = tf->tf_err;

	if ((ras_eip = (__greg32_t)(uintptr_t)ras_lookup(l->l_proc,
	    (void *) (uintptr_t)gr[_REG32_EIP])) != -1)
		gr[_REG32_EIP] = ras_eip;

	*flags |= _UC_CPU;

	mcp->_mc_tlsbase = (uint32_t)(uintptr_t)l->l_private;
	*flags |= _UC_TLSBASE;

	/* Save floating point register context. */
	process_read_fpregs_xmm(l, (struct fxsave *)
	    &mcp->__fpregs.__fp_reg_set.__fp_xmm_state);
	memset(&mcp->__fpregs.__fp_pad, 0, sizeof mcp->__fpregs.__fp_pad);
	*flags |= _UC_FXSAVE | _UC_FPU;
}

void
startlwp32(void *arg)
{
	ucontext32_t *uc = arg;
	lwp_t *l = curlwp;
	int error __diagused;

	error = cpu_setmcontext32(l, &uc->uc_mcontext, uc->uc_flags);
	KASSERT(error == 0);

	/* Note: we are freeing ucontext_t, not ucontext32_t. */
	kmem_free(uc, sizeof(ucontext_t));
	userret(l);
}

/*
 * For various reasons, the amd64 port can't do what the i386 port does,
 * and relies on catching invalid user contexts on exit from the kernel.
 * These functions perform the needed checks.
 */

int
check_sigcontext32(struct lwp *l, const struct netbsd32_sigcontext *scp)
{
	struct pmap *pmap = l->l_proc->p_vmspace->vm_map.pmap;
	struct trapframe *tf;
	struct pcb *pcb;

	tf = l->l_md.md_regs;
	pcb = lwp_getpcb(curlwp);

	if (((scp->sc_eflags ^ tf->tf_rflags) & PSL_USERSTATIC) != 0)
		return EINVAL;

	if (__predict_false(pmap->pm_ldt != NULL)) {
		/* Only when the LDT is user-set (with USER_LDT) */
		if (!USERMODE(scp->sc_cs))
			return EINVAL;
	} else {
		if (!VALID_USER_CSEL32(scp->sc_cs))
			return EINVAL;
		if (scp->sc_fs != 0 && !VALID_USER_DSEL32(scp->sc_fs) &&
			!(VALID_USER_FSEL32(scp->sc_fs) && pcb->pcb_fs != 0))
			return EINVAL;
		if (scp->sc_gs != 0 && !VALID_USER_DSEL32(scp->sc_gs) &&
			!(VALID_USER_GSEL32(scp->sc_gs) && pcb->pcb_gs != 0))
			return EINVAL;
		if (scp->sc_es != 0 && !VALID_USER_DSEL32(scp->sc_es))
			return EINVAL;
		if (!VALID_USER_DSEL32(scp->sc_ds) || !VALID_USER_DSEL32(scp->sc_ss))
			return EINVAL;
	}

	if (scp->sc_eip >= VM_MAXUSER_ADDRESS32)
		return EINVAL;

	return 0;
}

int
cpu_mcontext32_validate(struct lwp *l, const mcontext32_t *mcp)
{
	const __greg32_t *gr;
	struct trapframe *tf;

	gr = mcp->__gregs;
	tf = l->l_md.md_regs;

	if (((gr[_REG32_EFL] ^ tf->tf_rflags) & PSL_USERSTATIC) != 0)
		return EINVAL;

#ifdef USER_LDT
	/* Userland is allowed to have unfamiliar segment register values */
	if (!USERMODE(gr[_REG32_CS]))
		return EINVAL;
#else
	struct pcb *pcb = lwp_getpcb(l);

	if (!VALID_USER_CSEL32(gr[_REG32_CS]))
		return EINVAL;
	if (gr[_REG32_FS] != 0 && !VALID_USER_DSEL32(gr[_REG32_FS]) &&
	    !(VALID_USER_FSEL32(gr[_REG32_FS]) && pcb->pcb_fs != 0))
		return EINVAL;
	if (gr[_REG32_GS] != 0 && !VALID_USER_DSEL32(gr[_REG32_GS]) &&
	    !(VALID_USER_GSEL32(gr[_REG32_GS]) && pcb->pcb_gs != 0))
		return EINVAL;
	if (gr[_REG32_ES] != 0 && !VALID_USER_DSEL32(gr[_REG32_ES]))
		return EINVAL;
	if (!VALID_USER_DSEL32(gr[_REG32_DS]) ||
	    !VALID_USER_DSEL32(gr[_REG32_SS]))
		return EINVAL;
#endif

	if (gr[_REG32_EIP] >= VM_MAXUSER_ADDRESS32)
		return EINVAL;

	return 0;
}

vaddr_t
netbsd32_vm_default_addr(struct proc *p, vaddr_t base, vsize_t sz,
    int topdown)
{
	if (topdown)
		return VM_DEFAULT_ADDRESS32_TOPDOWN(base, sz);
	else
		return VM_DEFAULT_ADDRESS32_BOTTOMUP(base, sz);
}

static const char *
netbsd32_machine32(void)
{

	return machine32;
}

void
netbsd32_machdep_md_init(void)
{

	MODULE_SET_HOOK(netbsd32_machine32_hook, "mach32", netbsd32_machine32);
}

void
netbsd32_machdep_md_fini(void)
{

	MODULE_UNSET_HOOK(netbsd32_machine32_hook);
}
