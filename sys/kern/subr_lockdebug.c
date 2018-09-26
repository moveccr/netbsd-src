/*	$NetBSD: subr_lockdebug.c,v 1.67 2018/09/13 01:55:16 mrg Exp $	*/

/*-
 * Copyright (c) 2006, 2007, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Andrew Doran.
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

/*
 * Basic lock debugging code shared among lock primitives.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: subr_lockdebug.c,v 1.67 2018/09/13 01:55:16 mrg Exp $");

#ifdef _KERNEL_OPT
#include "opt_ddb.h"
#endif

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/kmem.h>
#include <sys/lockdebug.h>
#include <sys/sleepq.h>
#include <sys/cpu.h>
#include <sys/atomic.h>
#include <sys/lock.h>
#include <sys/rbtree.h>
#include <sys/ksyms.h>

#include <machine/lock.h>

unsigned int		ld_panic;

#ifdef LOCKDEBUG

#define	LD_BATCH_SHIFT	9
#define	LD_BATCH	(1 << LD_BATCH_SHIFT)
#define	LD_BATCH_MASK	(LD_BATCH - 1)
#define	LD_MAX_LOCKS	1048576
#define	LD_SLOP		16

#define	LD_LOCKED	0x01
#define	LD_SLEEPER	0x02

#define	LD_WRITE_LOCK	0x80000000

typedef struct lockdebug {
	struct rb_node	ld_rb_node;
	__cpu_simple_lock_t ld_spinlock;
	_TAILQ_ENTRY(struct lockdebug, volatile) ld_chain;
	_TAILQ_ENTRY(struct lockdebug, volatile) ld_achain;
	volatile void	*ld_lock;
	lockops_t	*ld_lockops;
	struct lwp	*ld_lwp;
	uintptr_t	ld_locked;
	uintptr_t	ld_unlocked;
	uintptr_t	ld_initaddr;
	uint16_t	ld_shares;
	uint16_t	ld_cpu;
	uint8_t		ld_flags;
	uint8_t		ld_shwant;	/* advisory */
	uint8_t		ld_exwant;	/* advisory */
	uint8_t		ld_unused;
} volatile lockdebug_t;

typedef _TAILQ_HEAD(lockdebuglist, struct lockdebug, volatile) lockdebuglist_t;

__cpu_simple_lock_t	ld_mod_lk;
lockdebuglist_t		ld_free = TAILQ_HEAD_INITIALIZER(ld_free);
lockdebuglist_t		ld_all = TAILQ_HEAD_INITIALIZER(ld_all);
int			ld_nfree;
int			ld_freeptr;
int			ld_recurse;
bool			ld_nomore;
lockdebug_t		ld_prime[LD_BATCH];

static void	lockdebug_abort1(const char *, size_t, lockdebug_t *, int,
    const char *, bool);
static int	lockdebug_more(int);
static void	lockdebug_init(void);
static void	lockdebug_dump(lockdebug_t *, void (*)(const char *, ...)
    __printflike(1, 2));

static signed int
ld_rbto_compare_nodes(void *ctx, const void *n1, const void *n2)
{
	const lockdebug_t *ld1 = n1;
	const lockdebug_t *ld2 = n2;
	const uintptr_t a = (uintptr_t)ld1->ld_lock;
	const uintptr_t b = (uintptr_t)ld2->ld_lock;

	if (a < b)
		return -1;
	if (a > b)
		return 1;
	return 0;
}

static signed int
ld_rbto_compare_key(void *ctx, const void *n, const void *key)
{
	const lockdebug_t *ld = n;
	const uintptr_t a = (uintptr_t)ld->ld_lock;
	const uintptr_t b = (uintptr_t)key;

	if (a < b)
		return -1;
	if (a > b)
		return 1;
	return 0;
}

static rb_tree_t ld_rb_tree;

static const rb_tree_ops_t ld_rb_tree_ops = {
	.rbto_compare_nodes = ld_rbto_compare_nodes,
	.rbto_compare_key = ld_rbto_compare_key,
	.rbto_node_offset = offsetof(lockdebug_t, ld_rb_node),
	.rbto_context = NULL
};

static inline lockdebug_t *
lockdebug_lookup1(const volatile void *lock)
{
	lockdebug_t *ld;
	struct cpu_info *ci;

	ci = curcpu();
	__cpu_simple_lock(&ci->ci_data.cpu_ld_lock);
	ld = rb_tree_find_node(&ld_rb_tree, (void *)(intptr_t)lock);
	__cpu_simple_unlock(&ci->ci_data.cpu_ld_lock);
	if (ld == NULL) {
		return NULL;
	}
	__cpu_simple_lock(&ld->ld_spinlock);

	return ld;
}

static void
lockdebug_lock_cpus(void)
{
	CPU_INFO_ITERATOR cii;
	struct cpu_info *ci;

	for (CPU_INFO_FOREACH(cii, ci)) {
		__cpu_simple_lock(&ci->ci_data.cpu_ld_lock);
	}
}

static void
lockdebug_unlock_cpus(void)
{
	CPU_INFO_ITERATOR cii;
	struct cpu_info *ci;

	for (CPU_INFO_FOREACH(cii, ci)) {
		__cpu_simple_unlock(&ci->ci_data.cpu_ld_lock);
	}
}

/*
 * lockdebug_lookup:
 *
 *	Find a lockdebug structure by a pointer to a lock and return it locked.
 */
static inline lockdebug_t *
lockdebug_lookup(const char *func, size_t line, const volatile void *lock,
    uintptr_t where)
{
	lockdebug_t *ld;

	ld = lockdebug_lookup1(lock);
	if (__predict_false(ld == NULL)) {
		panic("%s,%zu: uninitialized lock (lock=%p, from=%08"
		    PRIxPTR ")", func, line, lock, where);
	}
	return ld;
}

/*
 * lockdebug_init:
 *
 *	Initialize the lockdebug system.  Allocate an initial pool of
 *	lockdebug structures before the VM system is up and running.
 */
static void
lockdebug_init(void)
{
	lockdebug_t *ld;
	int i;

	TAILQ_INIT(&curcpu()->ci_data.cpu_ld_locks);
	TAILQ_INIT(&curlwp->l_ld_locks);
	__cpu_simple_lock_init(&curcpu()->ci_data.cpu_ld_lock);
	__cpu_simple_lock_init(&ld_mod_lk);

	rb_tree_init(&ld_rb_tree, &ld_rb_tree_ops);

	ld = ld_prime;
	for (i = 1, ld++; i < LD_BATCH; i++, ld++) {
		__cpu_simple_lock_init(&ld->ld_spinlock);
		TAILQ_INSERT_TAIL(&ld_free, ld, ld_chain);
		TAILQ_INSERT_TAIL(&ld_all, ld, ld_achain);
	}
	ld_freeptr = 1;
	ld_nfree = LD_BATCH - 1;
}

/*
 * lockdebug_alloc:
 *
 *	A lock is being initialized, so allocate an associated debug
 *	structure.
 */
bool
lockdebug_alloc(const char *func, size_t line, volatile void *lock,
    lockops_t *lo, uintptr_t initaddr)
{
	struct cpu_info *ci;
	lockdebug_t *ld;
	int s;

	if (__predict_false(lo == NULL || panicstr != NULL || ld_panic))
		return false;
	if (__predict_false(ld_freeptr == 0))
		lockdebug_init();

	s = splhigh();
	__cpu_simple_lock(&ld_mod_lk);
	if (__predict_false((ld = lockdebug_lookup1(lock)) != NULL)) {
		__cpu_simple_unlock(&ld_mod_lk);
		lockdebug_abort1(func, line, ld, s, "already initialized",
		    true);
		return false;
	}

	/*
	 * Pinch a new debug structure.  We may recurse because we call
	 * kmem_alloc(), which may need to initialize new locks somewhere
	 * down the path.  If not recursing, we try to maintain at least
	 * LD_SLOP structures free, which should hopefully be enough to
	 * satisfy kmem_alloc().  If we can't provide a structure, not to
	 * worry: we'll just mark the lock as not having an ID.
	 */
	ci = curcpu();
	ci->ci_lkdebug_recurse++;
	if (TAILQ_EMPTY(&ld_free)) {
		if (ci->ci_lkdebug_recurse > 1 || ld_nomore) {
			ci->ci_lkdebug_recurse--;
			__cpu_simple_unlock(&ld_mod_lk);
			splx(s);
			return false;
		}
		s = lockdebug_more(s);
	} else if (ci->ci_lkdebug_recurse == 1 && ld_nfree < LD_SLOP) {
		s = lockdebug_more(s);
	}
	if (__predict_false((ld = TAILQ_FIRST(&ld_free)) == NULL)) {
		__cpu_simple_unlock(&ld_mod_lk);
		splx(s);
		return false;
	}
	TAILQ_REMOVE(&ld_free, ld, ld_chain);
	ld_nfree--;
	ci->ci_lkdebug_recurse--;

	if (__predict_false(ld->ld_lock != NULL)) {
		panic("%s,%zu: corrupt table ld %p", func, line, ld);
	}

	/* Initialise the structure. */
	ld->ld_lock = lock;
	ld->ld_lockops = lo;
	ld->ld_locked = 0;
	ld->ld_unlocked = 0;
	ld->ld_lwp = NULL;
	ld->ld_initaddr = initaddr;
	ld->ld_flags = (lo->lo_type == LOCKOPS_SLEEP ? LD_SLEEPER : 0);
	lockdebug_lock_cpus();
	(void)rb_tree_insert_node(&ld_rb_tree, __UNVOLATILE(ld));
	lockdebug_unlock_cpus();
	__cpu_simple_unlock(&ld_mod_lk);

	splx(s);
	return true;
}

/*
 * lockdebug_free:
 *
 *	A lock is being destroyed, so release debugging resources.
 */
void
lockdebug_free(const char *func, size_t line, volatile void *lock)
{
	lockdebug_t *ld;
	int s;

	if (__predict_false(panicstr != NULL || ld_panic))
		return;

	s = splhigh();
	__cpu_simple_lock(&ld_mod_lk);
	ld = lockdebug_lookup(func, line, lock,
	    (uintptr_t) __builtin_return_address(0));
	if (__predict_false(ld == NULL)) {
		__cpu_simple_unlock(&ld_mod_lk);
		panic("%s,%zu: destroying uninitialized object %p"
		    "(ld_lock=%p)", func, line, lock, ld->ld_lock);
		return;
	}
	if (__predict_false((ld->ld_flags & LD_LOCKED) != 0 ||
	    ld->ld_shares != 0)) {
		__cpu_simple_unlock(&ld_mod_lk);
		lockdebug_abort1(func, line, ld, s, "is locked or in use",
		    true);
		return;
	}
	lockdebug_lock_cpus();
	rb_tree_remove_node(&ld_rb_tree, __UNVOLATILE(ld));
	lockdebug_unlock_cpus();
	ld->ld_lock = NULL;
	TAILQ_INSERT_TAIL(&ld_free, ld, ld_chain);
	ld_nfree++;
	__cpu_simple_unlock(&ld->ld_spinlock);
	__cpu_simple_unlock(&ld_mod_lk);
	splx(s);
}

/*
 * lockdebug_more:
 *
 *	Allocate a batch of debug structures and add to the free list.
 *	Must be called with ld_mod_lk held.
 */
static int
lockdebug_more(int s)
{
	lockdebug_t *ld;
	void *block;
	int i, base, m;

	/*
	 * Can't call kmem_alloc() if in interrupt context.  XXX We could
	 * deadlock, because we don't know which locks the caller holds.
	 */
	if (cpu_intr_p() || cpu_softintr_p()) {
		return s;
	}

	while (ld_nfree < LD_SLOP) {
		__cpu_simple_unlock(&ld_mod_lk);
		splx(s);
		block = kmem_zalloc(LD_BATCH * sizeof(lockdebug_t), KM_SLEEP);
		s = splhigh();
		__cpu_simple_lock(&ld_mod_lk);

		if (ld_nfree > LD_SLOP) {
			/* Somebody beat us to it. */
			__cpu_simple_unlock(&ld_mod_lk);
			splx(s);
			kmem_free(block, LD_BATCH * sizeof(lockdebug_t));
			s = splhigh();
			__cpu_simple_lock(&ld_mod_lk);
			continue;
		}

		base = ld_freeptr;
		ld_nfree += LD_BATCH;
		ld = block;
		base <<= LD_BATCH_SHIFT;
		m = uimin(LD_MAX_LOCKS, base + LD_BATCH);

		if (m == LD_MAX_LOCKS)
			ld_nomore = true;

		for (i = base; i < m; i++, ld++) {
			__cpu_simple_lock_init(&ld->ld_spinlock);
			TAILQ_INSERT_TAIL(&ld_free, ld, ld_chain);
			TAILQ_INSERT_TAIL(&ld_all, ld, ld_achain);
		}

		membar_producer();
	}

	return s;
}

/*
 * lockdebug_wantlock:
 *
 *	Process the preamble to a lock acquire.  The "shared"
 *	parameter controls which ld_{ex,sh}want counter is
 *	updated; a negative value of shared updates neither.
 */
void
lockdebug_wantlock(const char *func, size_t line,
    const volatile void *lock, uintptr_t where, int shared)
{
	struct lwp *l = curlwp;
	lockdebug_t *ld;
	bool recurse;
	int s;

	(void)shared;
	recurse = false;

	if (__predict_false(panicstr != NULL || ld_panic))
		return;

	s = splhigh();
	if ((ld = lockdebug_lookup(func, line, lock, where)) == NULL) {
		splx(s);
		return;
	}
	if ((ld->ld_flags & LD_LOCKED) != 0 || ld->ld_shares != 0) {
		if ((ld->ld_flags & LD_SLEEPER) != 0) {
			if (ld->ld_lwp == l)
				recurse = true;
		} else if (ld->ld_cpu == (uint16_t)cpu_index(curcpu()))
			recurse = true;
	}
	if (cpu_intr_p()) {
		if (__predict_false((ld->ld_flags & LD_SLEEPER) != 0)) {
			lockdebug_abort1(func, line, ld, s,
			    "acquiring sleep lock from interrupt context",
			    true);
			return;
		}
	}
	if (shared > 0)
		ld->ld_shwant++;
	else if (shared == 0)
		ld->ld_exwant++;
	if (__predict_false(recurse)) {
		lockdebug_abort1(func, line, ld, s, "locking against myself",
		    true);
		return;
	}
	__cpu_simple_unlock(&ld->ld_spinlock);
	splx(s);
}

/*
 * lockdebug_locked:
 *
 *	Process a lock acquire operation.
 */
void
lockdebug_locked(const char *func, size_t line,
    volatile void *lock, void *cvlock, uintptr_t where, int shared)
{
	struct lwp *l = curlwp;
	lockdebug_t *ld;
	int s;

	if (__predict_false(panicstr != NULL || ld_panic))
		return;

	s = splhigh();
	if ((ld = lockdebug_lookup(func, line, lock, where)) == NULL) {
		splx(s);
		return;
	}
	if (cvlock) {
		KASSERT(ld->ld_lockops->lo_type == LOCKOPS_CV);
		if (lock == (void *)&lbolt) {
			/* nothing */
		} else if (ld->ld_shares++ == 0) {
			ld->ld_locked = (uintptr_t)cvlock;
		} else if (__predict_false(cvlock != (void *)ld->ld_locked)) {
			lockdebug_abort1(func, line, ld, s,
			    "multiple locks used with condition variable",
			    true);
			return;
		}
	} else if (shared) {
		l->l_shlocks++;
		ld->ld_locked = where;
		ld->ld_shares++;
		ld->ld_shwant--;
	} else {
		if (__predict_false((ld->ld_flags & LD_LOCKED) != 0)) {
			lockdebug_abort1(func, line, ld, s, "already locked",
			    true);
			return;
		}
		ld->ld_flags |= LD_LOCKED;
		ld->ld_locked = where;
		ld->ld_exwant--;
		if ((ld->ld_flags & LD_SLEEPER) != 0) {
			TAILQ_INSERT_TAIL(&l->l_ld_locks, ld, ld_chain);
		} else {
			TAILQ_INSERT_TAIL(&curcpu()->ci_data.cpu_ld_locks,
			    ld, ld_chain);
		}
	}
	ld->ld_cpu = (uint16_t)cpu_index(curcpu());
	ld->ld_lwp = l;
	__cpu_simple_unlock(&ld->ld_spinlock);
	splx(s);
}

/*
 * lockdebug_unlocked:
 *
 *	Process a lock release operation.
 */
void
lockdebug_unlocked(const char *func, size_t line,
    volatile void *lock, uintptr_t where, int shared)
{
	struct lwp *l = curlwp;
	lockdebug_t *ld;
	int s;

	if (__predict_false(panicstr != NULL || ld_panic))
		return;

	s = splhigh();
	if ((ld = lockdebug_lookup(func, line, lock, where)) == NULL) {
		splx(s);
		return;
	}
	if (ld->ld_lockops->lo_type == LOCKOPS_CV) {
		if (lock == (void *)&lbolt) {
			/* nothing */
		} else {
			ld->ld_shares--;
		}
	} else if (shared) {
		if (__predict_false(l->l_shlocks == 0)) {
			lockdebug_abort1(func, line, ld, s,
			    "no shared locks held by LWP", true);
			return;
		}
		if (__predict_false(ld->ld_shares == 0)) {
			lockdebug_abort1(func, line, ld, s,
			    "no shared holds on this lock", true);
			return;
		}
		l->l_shlocks--;
		ld->ld_shares--;
		if (ld->ld_lwp == l) {
			ld->ld_unlocked = where;
			ld->ld_lwp = NULL;
		}
		if (ld->ld_cpu == (uint16_t)cpu_index(curcpu()))
			ld->ld_cpu = (uint16_t)-1;
	} else {
		if (__predict_false((ld->ld_flags & LD_LOCKED) == 0)) {
			lockdebug_abort1(func, line, ld, s, "not locked", true);
			return;
		}

		if ((ld->ld_flags & LD_SLEEPER) != 0) {
			if (__predict_false(ld->ld_lwp != curlwp)) {
				lockdebug_abort1(func, line, ld, s,
				    "not held by current LWP", true);
				return;
			}
			TAILQ_REMOVE(&l->l_ld_locks, ld, ld_chain);
		} else {
			uint16_t idx = (uint16_t)cpu_index(curcpu());
			if (__predict_false(ld->ld_cpu != idx)) {
				lockdebug_abort1(func, line, ld, s,
				    "not held by current CPU", true);
				return;
			}
			TAILQ_REMOVE(&curcpu()->ci_data.cpu_ld_locks, ld,
			    ld_chain);
		}
		ld->ld_flags &= ~LD_LOCKED;
		ld->ld_unlocked = where;		
		ld->ld_lwp = NULL;
	}
	__cpu_simple_unlock(&ld->ld_spinlock);
	splx(s);
}

/*
 * lockdebug_wakeup:
 *
 *	Process a wakeup on a condition variable.
 */
void
lockdebug_wakeup(const char *func, size_t line, volatile void *lock,
    uintptr_t where)
{
	lockdebug_t *ld;
	int s;

	if (__predict_false(panicstr != NULL || ld_panic || lock == (void *)&lbolt))
		return;

	s = splhigh();
	/* Find the CV... */
	if ((ld = lockdebug_lookup(func, line, lock, where)) == NULL) {
		splx(s);
		return;
	}
	/*
	 * If it has any waiters, ensure that they are using the
	 * same interlock.
	 */
	if (__predict_false(ld->ld_shares != 0 &&
	    !mutex_owned((kmutex_t *)ld->ld_locked))) {
		lockdebug_abort1(func, line, ld, s, "interlocking mutex not "
		    "held during wakeup", true);
		return;
	}
	__cpu_simple_unlock(&ld->ld_spinlock);
	splx(s);
}

/*
 * lockdebug_barrier:
 *	
 *	Panic if we hold more than one specified spin lock, and optionally,
 *	if we hold sleep locks.
 */
void
lockdebug_barrier(const char *func, size_t line, volatile void *spinlock,
    int slplocks)
{
	struct lwp *l = curlwp;
	lockdebug_t *ld;
	int s;

	if (__predict_false(panicstr != NULL || ld_panic))
		return;

	s = splhigh();
	if ((l->l_pflag & LP_INTR) == 0) {
		TAILQ_FOREACH(ld, &curcpu()->ci_data.cpu_ld_locks, ld_chain) {
			if (ld->ld_lock == spinlock) {
				continue;
			}
			__cpu_simple_lock(&ld->ld_spinlock);
			lockdebug_abort1(func, line, ld, s,
			    "spin lock held", true);
			return;
		}
	}
	if (slplocks) {
		splx(s);
		return;
	}
	ld = TAILQ_FIRST(&l->l_ld_locks);
	if (__predict_false(ld != NULL)) {
		__cpu_simple_lock(&ld->ld_spinlock);
		lockdebug_abort1(func, line, ld, s, "sleep lock held", true);
		return;
	}
	splx(s);
	if (l->l_shlocks != 0) {
		TAILQ_FOREACH(ld, &ld_all, ld_achain) {
			if (ld->ld_lockops->lo_type == LOCKOPS_CV)
				continue;
			if (ld->ld_lwp == l)
				lockdebug_dump(ld, printf);
		}
		panic("%s,%zu: holding %d shared locks", func, line,
		    l->l_shlocks);
	}
}

/*
 * lockdebug_mem_check:
 *
 *	Check for in-use locks within a memory region that is
 *	being freed.
 */
void
lockdebug_mem_check(const char *func, size_t line, void *base, size_t sz)
{
	lockdebug_t *ld;
	struct cpu_info *ci;
	int s;

	if (__predict_false(panicstr != NULL || ld_panic))
		return;

	s = splhigh();
	ci = curcpu();
	__cpu_simple_lock(&ci->ci_data.cpu_ld_lock);
	ld = (lockdebug_t *)rb_tree_find_node_geq(&ld_rb_tree, base);
	if (ld != NULL) {
		const uintptr_t lock = (uintptr_t)ld->ld_lock;

		if (__predict_false((uintptr_t)base > lock))
			panic("%s,%zu: corrupt tree ld=%p, base=%p, sz=%zu",
			    func, line, ld, base, sz);
		if (lock >= (uintptr_t)base + sz)
			ld = NULL;
	}
	__cpu_simple_unlock(&ci->ci_data.cpu_ld_lock);
	if (__predict_false(ld != NULL)) {
		__cpu_simple_lock(&ld->ld_spinlock);
		lockdebug_abort1(func, line, ld, s,
		    "allocation contains active lock", !cold);
		return;
	}
	splx(s);
}

/*
 * lockdebug_dump:
 *
 *	Dump information about a lock on panic, or for DDB.
 */
static void
lockdebug_dump(lockdebug_t *ld, void (*pr)(const char *, ...)
    __printflike(1, 2))
{
	int sleeper = (ld->ld_flags & LD_SLEEPER);

	(*pr)(
	    "lock address : %#018lx type     : %18s\n"
	    "initialized  : %#018lx",
	    (long)ld->ld_lock, (sleeper ? "sleep/adaptive" : "spin"),
	    (long)ld->ld_initaddr);

	if (ld->ld_lockops->lo_type == LOCKOPS_CV) {
		(*pr)(" interlock: %#018lx\n", (long)ld->ld_locked);
	} else {
		(*pr)("\n"
		    "shared holds : %18u exclusive: %18u\n"
		    "shares wanted: %18u exclusive: %18u\n"
		    "current cpu  : %18u last held: %18u\n"
		    "current lwp  : %#018lx last held: %#018lx\n"
		    "last locked%c : %#018lx unlocked%c: %#018lx\n",
		    (unsigned)ld->ld_shares, ((ld->ld_flags & LD_LOCKED) != 0),
		    (unsigned)ld->ld_shwant, (unsigned)ld->ld_exwant,
		    (unsigned)cpu_index(curcpu()), (unsigned)ld->ld_cpu,
		    (long)curlwp, (long)ld->ld_lwp,
		    ((ld->ld_flags & LD_LOCKED) ? '*' : ' '),
		    (long)ld->ld_locked,
		    ((ld->ld_flags & LD_LOCKED) ? ' ' : '*'),
		    (long)ld->ld_unlocked);
	}

	if (ld->ld_lockops->lo_dump != NULL)
		(*ld->ld_lockops->lo_dump)(ld->ld_lock);

	if (sleeper) {
		(*pr)("\n");
		turnstile_print(ld->ld_lock, pr);
	}
}

/*
 * lockdebug_abort1:
 *
 *	An error has been trapped - dump lock info and panic.
 */
static void
lockdebug_abort1(const char *func, size_t line, lockdebug_t *ld, int s,
		 const char *msg, bool dopanic)
{

	/*
	 * Don't make the situation worse if the system is already going
	 * down in flames.  Once a panic is triggered, lockdebug state
	 * becomes stale and cannot be trusted.
	 */
	if (atomic_inc_uint_nv(&ld_panic) != 1) {
		__cpu_simple_unlock(&ld->ld_spinlock);
		splx(s);
		return;
	}

	printf_nolog("%s error: %s,%zu: %s\n\n", ld->ld_lockops->lo_name,
	    func, line, msg);
	lockdebug_dump(ld, printf_nolog);
	__cpu_simple_unlock(&ld->ld_spinlock);
	splx(s);
	printf_nolog("\n");
	if (dopanic)
		panic("LOCKDEBUG: %s error: %s,%zu: %s",
		    ld->ld_lockops->lo_name, func, line, msg);
}

#endif	/* LOCKDEBUG */

/*
 * lockdebug_lock_print:
 *
 *	Handle the DDB 'show lock' command.
 */
#ifdef DDB
#include <machine/db_machdep.h>
#include <ddb/db_interface.h>

void
lockdebug_lock_print(void *addr, void (*pr)(const char *, ...))
{
#ifdef LOCKDEBUG
	lockdebug_t *ld;

	TAILQ_FOREACH(ld, &ld_all, ld_achain) {
		if (ld->ld_lock == NULL)
			continue;
		if (addr == NULL || ld->ld_lock == addr) {
			lockdebug_dump(ld, pr);
			if (addr != NULL)
				return;
		}
	}
	if (addr != NULL) {
		(*pr)("Sorry, no record of a lock with address %p found.\n",
		    addr);
	}
#else
	(*pr)("Sorry, kernel not built with the LOCKDEBUG option.\n");
#endif	/* LOCKDEBUG */
}

#ifdef LOCKDEBUG
static void
lockdebug_show_all_locks_lwp(void (*pr)(const char *, ...), bool show_trace)
{
	struct proc *p;

	LIST_FOREACH(p, &allproc, p_list) {
		struct lwp *l;
		LIST_FOREACH(l, &p->p_lwps, l_sibling) {
			lockdebug_t *ld;
			const char *sym;
			int i = 0;
			if (TAILQ_EMPTY(&l->l_ld_locks))
				continue;
			(*pr)("Locks held by an LWP (%s):\n",
			    l->l_name ? l->l_name : p->p_comm);
			TAILQ_FOREACH(ld, &l->l_ld_locks, ld_chain) {
				ksyms_getname(NULL, &sym,
				    (vaddr_t)ld->ld_initaddr,
				    KSYMS_CLOSEST|KSYMS_PROC|KSYMS_ANY);
				(*pr)("Lock %d (initialized at %s)\n", i++, sym);
				lockdebug_dump(ld, pr);
			}
			if (show_trace) {
				db_stack_trace_print((db_expr_t)(intptr_t)l,
				    true,
				    32 /* Limit just in case */,
				    "a", pr);
			}
			(*pr)("\n");
		}
	}
}

static void
lockdebug_show_all_locks_cpu(void (*pr)(const char *, ...), bool show_trace)
{
	lockdebug_t *ld;
	CPU_INFO_ITERATOR cii;
	struct cpu_info *ci;
	const char *sym;

	for (CPU_INFO_FOREACH(cii, ci)) {
		int i = 0;
		if (TAILQ_EMPTY(&ci->ci_data.cpu_ld_locks))
			continue;
		(*pr)("Locks held on CPU %u:\n", ci->ci_index);
		TAILQ_FOREACH(ld, &ci->ci_data.cpu_ld_locks, ld_chain) {
			ksyms_getname(NULL, &sym,
			    (vaddr_t)ld->ld_initaddr,
			    KSYMS_CLOSEST|KSYMS_PROC|KSYMS_ANY);
			(*pr)("Lock %d (initialized at %s)\n", i++, sym);
			lockdebug_dump(ld, pr);
			if (show_trace) {
				db_stack_trace_print(
				    (db_expr_t)(intptr_t)ci->ci_curlwp,
				    true,
				    32 /* Limit just in case */,
				    "a", pr);
			}
			(*pr)("\n");
		}
	}
}
#endif	/* LOCKDEBUG */

void
lockdebug_show_all_locks(void (*pr)(const char *, ...), const char *modif)
{
#ifdef LOCKDEBUG
	bool show_trace = false;
	if (modif[0] == 't')
		show_trace = true;

	(*pr)("[Locks tracked through LWPs]\n");
	lockdebug_show_all_locks_lwp(pr, show_trace);
	(*pr)("\n");

	(*pr)("[Locks tracked through CPUs]\n");
	lockdebug_show_all_locks_cpu(pr, show_trace);
	(*pr)("\n");
#else
	(*pr)("Sorry, kernel not built with the LOCKDEBUG option.\n");
#endif	/* LOCKDEBUG */
}

void
lockdebug_show_lockstats(void (*pr)(const char *, ...))
{
#ifdef LOCKDEBUG
	lockdebug_t *ld;
	void *_ld;
	uint32_t n_null = 0;
	uint32_t n_spin_mutex = 0;
	uint32_t n_adaptive_mutex = 0;
	uint32_t n_rwlock = 0;
	uint32_t n_cv = 0;
	uint32_t n_others = 0;

	RB_TREE_FOREACH(_ld, &ld_rb_tree) {
		ld = _ld;
		if (ld->ld_lock == NULL) {
			n_null++;
			continue;
		}
		if (ld->ld_lockops->lo_type == LOCKOPS_CV) {
			n_cv++;
			continue;
		}
		if (ld->ld_lockops->lo_name[0] == 'M') {
			if (ld->ld_lockops->lo_type == LOCKOPS_SLEEP)
				n_adaptive_mutex++;
			else
				n_spin_mutex++;
			continue;
		}
		if (ld->ld_lockops->lo_name[0] == 'R') {
			n_rwlock++;
			continue;
		}
		n_others++;
	}
	(*pr)(
	    "condvar: %u\n"
	    "spin mutex: %u\n"
	    "adaptive mutex: %u\n"
	    "rwlock: %u\n"
	    "null locks: %u\n"
	    "others: %u\n",
	    n_cv,  n_spin_mutex, n_adaptive_mutex, n_rwlock,
	    n_null, n_others);
#else
	(*pr)("Sorry, kernel not built with the LOCKDEBUG option.\n");
#endif	/* LOCKDEBUG */
}
#endif	/* DDB */

/*
 * lockdebug_dismiss:
 *
 *      The system is rebooting, and potentially from an unsafe
 *      place so avoid any future aborts.
 */
void
lockdebug_dismiss(void)
{

	atomic_inc_uint_nv(&ld_panic);
}

/*
 * lockdebug_abort:
 *
 *	An error has been trapped - dump lock info and call panic().
 */
void
lockdebug_abort(const char *func, size_t line, const volatile void *lock,
    lockops_t *ops, const char *msg)
{
#ifdef LOCKDEBUG
	lockdebug_t *ld;
	int s;

	s = splhigh();
	if ((ld = lockdebug_lookup(func, line, lock, 
			(uintptr_t) __builtin_return_address(0))) != NULL) {
		lockdebug_abort1(func, line, ld, s, msg, true);
		return;
	}
	splx(s);
#endif	/* LOCKDEBUG */

	/*
	 * Don't make the situation worse if the system is already going
	 * down in flames.  Once a panic is triggered, lockdebug state
	 * becomes stale and cannot be trusted.
	 */
	if (atomic_inc_uint_nv(&ld_panic) > 1)
		return;

	printf_nolog("%s error: %s,%zu: %s\n\n"
	    "lock address : %#018lx\n"
	    "current cpu  : %18d\n"
	    "current lwp  : %#018lx\n",
	    ops->lo_name, func, line, msg, (long)lock,
	    (int)cpu_index(curcpu()), (long)curlwp);
	(*ops->lo_dump)(lock);
	printf_nolog("\n");

	panic("lock error: %s: %s,%zu: %s: lock %p cpu %d lwp %p",
	    ops->lo_name, func, line, msg, lock, cpu_index(curcpu()), curlwp);
}
