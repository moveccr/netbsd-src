/*	$NetBSD: trap.c,v 1.148 2019/02/18 01:12:23 thorpej Exp $	*/

/*
 * Copyright (c) 1988 University of Utah.
 * Copyright (c) 1982, 1986, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 *
 * from: Utah $Hdr: trap.c 1.37 92/12/20$
 *
 *	@(#)trap.c	8.5 (Berkeley) 1/4/94
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: trap.c,v 1.148 2019/02/18 01:12:23 thorpej Exp $");

#include "opt_ddb.h"
#include "opt_execfmt.h"
#include "opt_fpu_emulate.h"
#include "opt_kgdb.h"
#include "opt_compat_sunos.h"
#include "opt_m68k_arch.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/acct.h>
#include <sys/kernel.h>
#include <sys/signalvar.h>
#include <sys/resourcevar.h>
#include <sys/syscall.h>
#include <sys/syslog.h>
#include <sys/userret.h>
#include <sys/kauth.h>
#ifdef KGDB
#include <sys/kgdb.h>
#endif
#ifdef DEBUG
#include <dev/cons.h>
#endif

#include <m68k/cacheops.h>
#include <machine/db_machdep.h>
#include <machine/pcb.h>
#include <machine/psl.h>
#include <machine/trap.h>
#include <machine/cpu.h>
#include <machine/reg.h>

#include <m68k/fpe/fpu_emulate.h>

#include <uvm/uvm_extern.h>

#include "zsc.h"

#ifdef COMPAT_SUNOS
#include <compat/sunos/sunos_syscall.h>
extern struct emul emul_sunos;
#endif

int	astpending;

const char	*trap_type[] = {
	"Bus error",
	"Address error",
	"Illegal instruction",
	"Zero divide",
	"CHK instruction",
	"TRAPV instruction",
	"Privilege violation",
	"Trace trap",
	"MMU fault",
	"SSIR trap",
	"Format error",
	"68881 exception",
	"Coprocessor violation",
	"Async system trap"
};
int	trap_types = sizeof trap_type / sizeof trap_type[0];

/*
 * Size of various exception stack frames (minus the standard 8 bytes)
 */
short	exframesize[] = {
	FMT0SIZE,	/* type 0 - normal (68020/030/040) */
	FMT1SIZE,	/* type 1 - throwaway (68020/030/040) */
	FMT2SIZE,	/* type 2 - normal 6-word (68020/030/040) */
	FMT3SIZE,	/* type 3 - FP post-instruction (68040) */
	FMT4SIZE,	/* type 4 - LC040 FP exception (68LC040) */
	-1, -1,		/* type 5-6 - undefined */
	FMT7SIZE,	/* type 7 - access error (68040) */
	58,		/* type 8 - bus fault (68010) */
	FMT9SIZE,	/* type 9 - coprocessor mid-instruction (68020/030) */
	FMTASIZE,	/* type A - short bus fault (68020/030) */
	FMTBSIZE,	/* type B - long bus fault (68020/030) */
	-1, -1, -1, -1	/* type C-F - undefined */
};

#ifdef M68040
#define KDFAULT(c)	(mmutype == MMU_68040 ?			\
			  ((c) & SSW4_TMMASK) == SSW4_TMKD : 	\
			  ((c) & (SSW_DF|FC_SUPERD)) == (SSW_DF|FC_SUPERD))
#define WRFAULT(c)	(mmutype == MMU_68040 ?		\
			  ((c) & (SSW4_LK|SSW4_RW)) != SSW4_RW : 	\
			 (((c) & SSW_DF) != 0 && \
			 ((((c) & SSW_RW) == 0) || (((c) & SSW_RM) != 0))))
#else
#define KDFAULT(c)	(((c) & (SSW_DF|SSW_FCMASK)) == (SSW_DF|FC_SUPERD))
#define WRFAULT(c)	(((c) & SSW_DF) != 0 && \
			    ((((c) & SSW_RW) == 0) || (((c) & SSW_RM) != 0)))
#endif

#ifdef DEBUG
int mmudebug = 0;
int mmupid = -1;
#define MDB_FOLLOW	1
#define MDB_WBFOLLOW	2
#define MDB_WBFAILED	4
#define MDB_ISPID(pid)	((pid) == mmupid)
#endif

/* trap() only called from locore */
void	trap(struct frame *, int, u_int, u_int);

static inline void userret(struct lwp *, struct frame *, u_quad_t, u_int, int);

/*
 * Trap and syscall both need the following work done before returning
 * to user mode.
 */
static inline void
userret(struct lwp *l, struct frame *fp, u_quad_t oticks, u_int faultaddr,
    int fromtrap)
{
	struct proc *p = l->l_proc;
#if defined(M68040)
	int sig;
	int beenhere = 0;

again:
#endif
	/* Invoke MI userret code */
	mi_userret(l);

	/*
	 * If profiling, charge recent system time.
	 */
	if (p->p_stflag & PST_PROFIL) {
		extern int psratio;
		
		addupc_task(l, fp->f_pc,
		    (int)(p->p_sticks - oticks) * psratio);
	}
#if defined(M68040)
	/*
	 * Deal with user mode writebacks (from trap, or from sigreturn).
	 * If any writeback fails, go back and attempt signal delivery
	 * unless we have already been here and attempted the writeback
	 * (e.g. bad address with user ignoring SIGSEGV).  In that case,
	 * we just return to the user without successfully completing
	 * the writebacks.  Maybe we should just drop the sucker?
	 */
	if (mmutype == MMU_68040 && fp->f_format == FMT7) {
		if (beenhere) {
#if DEBUG
			if (mmudebug & MDB_WBFAILED)
				printf(fromtrap ?
		"pid %d(%s): writeback aborted, pc=%x, fa=%x\n" :
		"pid %d(%s): writeback aborted in sigreturn, pc=%x\n",
				    p->p_pid, p->p_comm, fp->f_pc, faultaddr);
#endif
		} else if ((sig = m68040_writeback(fp, fromtrap))) {
			ksiginfo_t ksi;
			beenhere = 1;
			oticks = p->p_sticks;
			(void)memset(&ksi, 0, sizeof(ksi));
			ksi.ksi_signo = sig;
			ksi.ksi_addr = (void *)faultaddr;
			ksi.ksi_code = BUS_OBJERR;
			trapsignal(l, &ksi);
			goto again;
		}
	}
#endif
}

/*
 * Used by the common m68k syscall() and child_return() functions.
 * XXX: Temporary until all m68k ports share common trap()/userret() code.
 */
void machine_userret(struct lwp *, struct frame *, u_quad_t);

void
machine_userret(struct lwp *l, struct frame *f, u_quad_t t)
{

	userret(l, f, t, 0, 0);
}

/*
 * Trap is called from locore to handle most types of processor traps,
 * including events such as simulated software interrupts/AST's.
 * System calls are broken out for efficiency.
 */
/*ARGSUSED*/
void
trap(struct frame *fp, int type, u_int code, u_int v)
{
	extern char fubail[], subail[];
	struct lwp *l;
	struct proc *p;
	struct pcb *pcb;
	void *onfault;
	ksiginfo_t ksi;
	int s;
	int rv;
	u_quad_t sticks;

	curcpu()->ci_data.cpu_ntrap++;
	l = curlwp;
	p = l->l_proc;
	pcb = lwp_getpcb(l);

	KSI_INIT_TRAP(&ksi);
	ksi.ksi_trap = type & ~T_USER;

	if (USERMODE(fp->f_sr)) {
		type |= T_USER;
		sticks = p->p_sticks;
		l->l_md.md_regs = fp->f_regs;
		LWP_CACHE_CREDS(l, p);
	} else
		sticks = 0;

	switch (type) {
	default:
	dopanic:
		printf("trap type %d, code = 0x%x, v = 0x%x\n", type, code, v);
		printf("%s program counter = 0x%x\n",
		    (type & T_USER) ? "user" : "kernel", fp->f_pc);
		/*
		 * Let the kernel debugger see the trap frame that
		 * caused us to panic.  This is a convenience so
		 * one can see registers at the point of failure.
		 */
		s = splhigh();
#ifdef KGDB
		/* If connected, step or cont returns 1 */
		if (kgdb_trap(type, (db_regs_t *)fp))
			goto kgdb_cont;
#endif
#ifdef DDB
		(void)kdb_trap(type, (db_regs_t *)fp);
#endif
#ifdef KGDB
	kgdb_cont:
#endif
		splx(s);
		if (panicstr) {
			printf("trap during panic!\n");
#ifdef DEBUG
			/* XXX should be a machine-dependent hook */
			printf("(press a key)\n"); (void)cngetc();
#endif
		}
		regdump((struct trapframe *)fp, 128);
		type &= ~T_USER;
		if ((u_int)type < trap_types)
			panic(trap_type[type]);
		panic("trap");

	case T_BUSERR:		/* Kernel bus error */
		onfault = pcb->pcb_onfault;
		if (onfault == NULL)
			goto dopanic;
		rv = EFAULT;
		/*
		 * If we have arranged to catch this fault in any of the
		 * copy to/from user space routines, set PC to return to
		 * indicated location and set flag informing buserror code
		 * that it may need to clean up stack frame.
		 */
copyfault:
		fp->f_stackadj = exframesize[fp->f_format];
		fp->f_format = fp->f_vector = 0;
		fp->f_pc = (int)onfault;
		fp->f_regs[D0] = rv;
		return;

	case T_BUSERR|T_USER:	/* Bus error */
	case T_ADDRERR|T_USER:	/* Address error */
		ksi.ksi_addr = (void *)v;
		ksi.ksi_signo = SIGBUS;
		ksi.ksi_code = (type == (T_BUSERR|T_USER)) ?
			BUS_OBJERR : BUS_ADRERR;
		break;

	case T_ILLINST|T_USER:	/* Illegal instruction fault */
	case T_PRIVINST|T_USER:	/* Privileged instruction fault */
		ksi.ksi_addr = (void *)(int)fp->f_format;
				/* XXX was ILL_PRIVIN_FAULT */
		ksi.ksi_signo = SIGILL;
		ksi.ksi_code = (type == (T_PRIVINST|T_USER)) ?
			ILL_PRVOPC : ILL_ILLOPC;
		break;
	/*
	 * divde by zero, CHK/TRAPV inst 
	 */
	case T_ZERODIV|T_USER:		/* Divide by zero trap */
		ksi.ksi_code = FPE_FLTDIV;
	case T_CHKINST|T_USER:		/* CHK instruction trap */
	case T_TRAPVINST|T_USER:	/* TRAPV instruction trap */
		ksi.ksi_addr = (void *)(int)fp->f_format;
		ksi.ksi_signo = SIGFPE;
		break;

	/* 
	 * User coprocessor violation
	 */
	case T_COPERR|T_USER:
	/* XXX What is a proper response here? */
		ksi.ksi_signo = SIGFPE;
		ksi.ksi_code = FPE_FLTINV;
		break;
	/* 
	 * 6888x exceptions 
	 */
	case T_FPERR|T_USER:
		/*
		 * We decode the 68881 status register which locore
		 * stashed in code for us.
		 */
		ksi.ksi_signo = SIGFPE;
		ksi.ksi_code = fpsr2siginfocode(code);
		break;

	/*
	 * FPU faults in supervisor mode.
	 */
	case T_ILLINST:	/* fnop generates this, apparently. */
	case T_FPEMULI:
	case T_FPEMULD: {
		extern label_t *nofault;

		if (nofault)	/* If we're probing. */
			longjmp(nofault);
		if (type == T_ILLINST)
			printf("Kernel Illegal Instruction trap.\n");
		else
			printf("Kernel FPU trap.\n");
		goto dopanic;
	}

	/*
	 * Unimplemented FPU instructions/datatypes.
	 */
	case T_FPEMULI|T_USER:
	case T_FPEMULD|T_USER:
#ifdef FPU_EMULATE
		if (fpu_emulate(fp, &pcb->pcb_fpregs, &ksi) == 0)
			; /* XXX - Deal with tracing? (fp->f_sr & PSL_T) */
#else
		uprintf("pid %d killed: no floating point support.\n",
			p->p_pid);
		ksi.ksi_signo = SIGILL;
		ksi.ksi_code = ILL_ILLOPC;
#endif
		break;

	case T_COPERR:		/* Kernel coprocessor violation */
	case T_FMTERR:		/* Kernel format error */
	case T_FMTERR|T_USER:	/* User format error */
		/*
		 * The user has most likely trashed the RTE or FP state info
		 * in the stack frame of a signal handler.
		 */
		printf("pid %d: kernel %s exception\n", p->p_pid,
		    type==T_COPERR ? "coprocessor" : "format");
		type |= T_USER;
		mutex_enter(p->p_lock);
		SIGACTION(p, SIGILL).sa_handler = SIG_DFL;
		sigdelset(&p->p_sigctx.ps_sigignore, SIGILL);
		sigdelset(&p->p_sigctx.ps_sigcatch, SIGILL);
		sigdelset(&l->l_sigmask, SIGILL);
		mutex_exit(p->p_lock);
		ksi.ksi_signo = SIGILL;
		ksi.ksi_addr = (void *)(int)fp->f_format;
				/* XXX was ILL_RESAD_FAULT */
		ksi.ksi_code = (type == T_COPERR) ?
			ILL_COPROC : ILL_ILLOPC;
		break;

	/*
	 * XXX: Trace traps are a nightmare.
	 *
	 *	HP-UX uses trap #1 for breakpoints,
	 *	NetBSD/m68k uses trap #2,
	 *	SUN 3.x uses trap #15,
	 *	DDB and KGDB uses trap #15 (for kernel breakpoints;
	 *	handled elsewhere).
	 *
	 * NetBSD and HP-UX traps both get mapped by locore.s into T_TRACE.
	 * SUN 3.x traps get passed through as T_TRAP15 and are not really
	 * supported yet.
	 *
	 * XXX: We should never get kernel-mode T_TRAP15 because
	 * XXX: locore.s now gives it special treatment.
	 */
	case T_TRAP15:		/* SUN trace trap */
#ifdef DEBUG
		printf("unexpected kernel trace trap, type = %d\n", type);
		printf("program counter = 0x%x\n", fp->f_pc);
#endif
		fp->f_sr &= ~PSL_T;
		ksi.ksi_signo = SIGTRAP;
		break;

	case T_TRACE|T_USER:	/* user trace trap */
#ifdef COMPAT_SUNOS
		/*
		 * SunOS uses Trap #2 for a "CPU cache flush".
		 * Just flush the on-chip caches and return.
		 */
		if (p->p_emul == &emul_sunos) {
			ICIA();
			DCIU();
			return;
		}
#endif
		/* FALLTHROUGH */
	case T_TRACE:		/* tracing a trap instruction */
	case T_TRAP15|T_USER:	/* SUN user trace trap */
		fp->f_sr &= ~PSL_T;
		ksi.ksi_signo = SIGTRAP;
		break;

	case T_ASTFLT:		/* System async trap, cannot happen */
		goto dopanic;

	case T_ASTFLT|T_USER:	/* User async trap. */
		astpending = 0;
		/*
		 * We check for software interrupts first.  This is because
		 * they are at a higher level than ASTs, and on a VAX would
		 * interrupt the AST.  We assume that if we are processing
		 * an AST that we must be at IPL0 so we don't bother to
		 * check.  Note that we ensure that we are at least at SIR
		 * IPL while processing the SIR.
		 */
		spl1();
		/* fall into... */

	case T_SSIR:		/* Software interrupt */
	case T_SSIR|T_USER:
		/*
		 * If this was not an AST trap, we are all done.
		 */
		if (type != (T_ASTFLT|T_USER)) {
			curcpu()->ci_data.cpu_ntrap--;
			return;
		}
		spl0();
		if (l->l_pflag & LP_OWEUPC) {
			l->l_pflag &= ~LP_OWEUPC;
			ADDUPROF(l);
		}
		if (curcpu()->ci_want_resched)
			preempt();
		goto out;

	case T_MMUFLT:		/* Kernel mode page fault */
		/*
		 * If we were doing profiling ticks or other user mode
		 * stuff from interrupt code, Just Say No.
		 */
		onfault = pcb->pcb_onfault;
		if (onfault == fubail || onfault == subail) {
			rv = EFAULT;
			goto copyfault;
		}
		/* fall into... */

	case T_MMUFLT|T_USER:	/* page fault */
	    {
		vaddr_t va;
		struct vmspace *vm = p->p_vmspace;
		struct vm_map *map;
		vm_prot_t ftype;
		extern struct vm_map *kernel_map;

		onfault = pcb->pcb_onfault;

#ifdef DEBUG
		if ((mmudebug & MDB_WBFOLLOW) || MDB_ISPID(p->p_pid))
		printf("trap: T_MMUFLT pid=%d, code=%x, v=%x, pc=%x, sr=%x\n",
			p->p_pid, code, v, fp->f_pc, fp->f_sr);
#endif
		/*
		 * It is only a kernel address space fault iff:
		 *	1. (type & T_USER) == 0 and
		 *	2. pcb_onfault not set or
		 *	3. pcb_onfault set but supervisor data fault
		 * The last can occur during an exec() copyin where the
		 * argument space is lazy-allocated.
		 */
		if (type == T_MMUFLT && (onfault == NULL || KDFAULT(code)))
			map = kernel_map;
		else {
			map = vm ? &vm->vm_map : kernel_map;
		}
		if (WRFAULT(code))
			ftype = VM_PROT_WRITE;
		else
			ftype = VM_PROT_READ;
		va = trunc_page((vaddr_t)v);
#ifdef DEBUG
		if (map == kernel_map && va == 0) {
			printf("trap: bad kernel access at %x\n", v);
			goto dopanic;
		}
#endif
		pcb->pcb_onfault = NULL;
		rv = uvm_fault(map, va, ftype);
		pcb->pcb_onfault = onfault;
#ifdef DEBUG
		if (rv && MDB_ISPID(p->p_pid))
			printf("uvm_fault(%p, 0x%lx, 0x%x) -> 0x%x\n",
			    map, va, ftype, rv);
#endif
		/*
		 * If this was a stack access, we keep track of the maximum
		 * accessed stack size.  Also, if vm_fault gets a protection
		 * failure, it is due to accessing the stack region outside
		 * the current limit and we need to reflect that as an access
		 * error.
		 */
		if (rv == 0) {
			if (map != kernel_map && (void *)va >= vm->vm_maxsaddr)
				uvm_grow(p, va);

			if (type == T_MMUFLT) {
				if (ucas_ras_check(&fp->F_t)) {
					return;
				}
#if defined(M68040)
				if (mmutype == MMU_68040)
					(void)m68040_writeback(fp, 1);
#endif
				return;
			}
			goto out;
		}
		if (type == T_MMUFLT) {
			if (onfault)
				goto copyfault;
			printf("uvm_fault(%p, 0x%lx, 0x%x) -> 0x%x\n",
			    map, va, ftype, rv);
			printf("  type %x, code [mmu,,ssw]: %x\n",
				type, code);
			goto dopanic;
		}
		ksi.ksi_addr = (void *)v;
		switch (rv) {
		case ENOMEM:
			printf("UVM: pid %d (%s), uid %d killed: out of swap\n",
			       p->p_pid, p->p_comm,
			       l->l_cred ?
			       kauth_cred_geteuid(l->l_cred) : -1);
			ksi.ksi_signo = SIGKILL;
			break;
		case EINVAL:
			ksi.ksi_signo = SIGBUS;
			ksi.ksi_code = BUS_ADRERR;
			break;
		case EACCES:
			ksi.ksi_signo = SIGSEGV;
			ksi.ksi_code = SEGV_ACCERR;
			break;
		default:
			ksi.ksi_signo = SIGSEGV;
			ksi.ksi_code = SEGV_MAPERR;
			break;
		}
		break;
	    }
	}
	if (ksi.ksi_signo)
		trapsignal(l, &ksi);
	if ((type & T_USER) == 0)
		return;
out:
	userret(l, fp, sticks, v, 1); 
}
