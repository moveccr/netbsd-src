/*	$NetBSD: uvm_fault.c,v 1.228 2020/07/09 05:57:15 skrll Exp $	*/

/*
 * Copyright (c) 1997 Charles D. Cranor and Washington University.
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
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * from: Id: uvm_fault.c,v 1.1.2.23 1998/02/06 05:29:05 chs Exp
 */

/*
 * uvm_fault.c: fault handler
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: uvm_fault.c,v 1.228 2020/07/09 05:57:15 skrll Exp $");

#include "opt_uvmhist.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/atomic.h>
#include <sys/kernel.h>
#include <sys/mman.h>

#include <uvm/uvm.h>
#include <uvm/uvm_pdpolicy.h>

/*
 *
 * a word on page faults:
 *
 * types of page faults we handle:
 *
 * CASE 1: upper layer faults                   CASE 2: lower layer faults
 *
 *    CASE 1A         CASE 1B                  CASE 2A        CASE 2B
 *    read/write1     write>1                  read/write   +-cow_write/zero
 *         |             |                         |        |
 *      +--|--+       +--|--+     +-----+       +  |  +     | +-----+
 * amap |  V  |       |  ---------> new |          |        | |  ^  |
 *      +-----+       +-----+     +-----+       +  |  +     | +--|--+
 *                                                 |        |    |
 *      +-----+       +-----+                   +--|--+     | +--|--+
 * uobj | d/c |       | d/c |                   |  V  |     +----+  |
 *      +-----+       +-----+                   +-----+       +-----+
 *
 * d/c = don't care
 *
 *   case [0]: layerless fault
 *	no amap or uobj is present.   this is an error.
 *
 *   case [1]: upper layer fault [anon active]
 *     1A: [read] or [write with anon->an_ref == 1]
 *		I/O takes place in upper level anon and uobj is not touched.
 *     1B: [write with anon->an_ref > 1]
 *		new anon is alloc'd and data is copied off ["COW"]
 *
 *   case [2]: lower layer fault [uobj]
 *     2A: [read on non-NULL uobj] or [write to non-copy_on_write area]
 *		I/O takes place directly in object.
 *     2B: [write to copy_on_write] or [read on NULL uobj]
 *		data is "promoted" from uobj to a new anon.
 *		if uobj is null, then we zero fill.
 *
 * we follow the standard UVM locking protocol ordering:
 *
 * MAPS => AMAP => UOBJ => ANON => PAGE QUEUES (PQ)
 * we hold a PG_BUSY page if we unlock for I/O
 *
 *
 * the code is structured as follows:
 *
 *     - init the "IN" params in the ufi structure
 *   ReFault: (ERESTART returned to the loop in uvm_fault_internal)
 *     - do lookups [locks maps], check protection, handle needs_copy
 *     - check for case 0 fault (error)
 *     - establish "range" of fault
 *     - if we have an amap lock it and extract the anons
 *     - if sequential advice deactivate pages behind us
 *     - at the same time check pmap for unmapped areas and anon for pages
 *	 that we could map in (and do map it if found)
 *     - check object for resident pages that we could map in
 *     - if (case 2) goto Case2
 *     - >>> handle case 1
 *           - ensure source anon is resident in RAM
 *           - if case 1B alloc new anon and copy from source
 *           - map the correct page in
 *   Case2:
 *     - >>> handle case 2
 *           - ensure source page is resident (if uobj)
 *           - if case 2B alloc new anon and copy from source (could be zero
 *		fill if uobj == NULL)
 *           - map the correct page in
 *     - done!
 *
 * note on paging:
 *   if we have to do I/O we place a PG_BUSY page in the correct object,
 * unlock everything, and do the I/O.   when I/O is done we must reverify
 * the state of the world before assuming that our data structures are
 * valid.   [because mappings could change while the map is unlocked]
 *
 *  alternative 1: unbusy the page in question and restart the page fault
 *    from the top (ReFault).   this is easy but does not take advantage
 *    of the information that we already have from our previous lookup,
 *    although it is possible that the "hints" in the vm_map will help here.
 *
 * alternative 2: the system already keeps track of a "version" number of
 *    a map.   [i.e. every time you write-lock a map (e.g. to change a
 *    mapping) you bump the version number up by one...]   so, we can save
 *    the version number of the map before we release the lock and start I/O.
 *    then when I/O is done we can relock and check the version numbers
 *    to see if anything changed.    this might save us some over 1 because
 *    we don't have to unbusy the page and may be less compares(?).
 *
 * alternative 3: put in backpointers or a way to "hold" part of a map
 *    in place while I/O is in progress.   this could be complex to
 *    implement (especially with structures like amap that can be referenced
 *    by multiple map entries, and figuring out what should wait could be
 *    complex as well...).
 *
 * we use alternative 2.  given that we are multi-threaded now we may want
 * to reconsider the choice.
 */

/*
 * local data structures
 */

struct uvm_advice {
	int advice;
	int nback;
	int nforw;
};

/*
 * page range array:
 * note: index in array must match "advice" value
 * XXX: borrowed numbers from freebsd.   do they work well for us?
 */

static const struct uvm_advice uvmadvice[] = {
	{ UVM_ADV_NORMAL, 3, 4 },
	{ UVM_ADV_RANDOM, 0, 0 },
	{ UVM_ADV_SEQUENTIAL, 8, 7},
};

#define UVM_MAXRANGE 16	/* must be MAX() of nback+nforw+1 */

/*
 * private prototypes
 */

/*
 * externs from other modules
 */

extern int start_init_exec;	/* Is init_main() done / init running? */

/*
 * inline functions
 */

/*
 * uvmfault_anonflush: try and deactivate pages in specified anons
 *
 * => does not have to deactivate page if it is busy
 */

static inline void
uvmfault_anonflush(struct vm_anon **anons, int n)
{
	int lcv;
	struct vm_page *pg;

	for (lcv = 0; lcv < n; lcv++) {
		if (anons[lcv] == NULL)
			continue;
		KASSERT(rw_lock_held(anons[lcv]->an_lock));
		pg = anons[lcv]->an_page;
		if (pg && (pg->flags & PG_BUSY) == 0) {
			uvm_pagelock(pg);
			uvm_pagedeactivate(pg);
			uvm_pageunlock(pg);
		}
	}
}

/*
 * normal functions
 */

/*
 * uvmfault_amapcopy: clear "needs_copy" in a map.
 *
 * => called with VM data structures unlocked (usually, see below)
 * => we get a write lock on the maps and clear needs_copy for a VA
 * => if we are out of RAM we sleep (waiting for more)
 */

static void
uvmfault_amapcopy(struct uvm_faultinfo *ufi)
{
	for (;;) {

		/*
		 * no mapping?  give up.
		 */

		if (uvmfault_lookup(ufi, true) == false)
			return;

		/*
		 * copy if needed.
		 */

		if (UVM_ET_ISNEEDSCOPY(ufi->entry))
			amap_copy(ufi->map, ufi->entry, AMAP_COPY_NOWAIT,
				ufi->orig_rvaddr, ufi->orig_rvaddr + 1);

		/*
		 * didn't work?  must be out of RAM.   unlock and sleep.
		 */

		if (UVM_ET_ISNEEDSCOPY(ufi->entry)) {
			uvmfault_unlockmaps(ufi, true);
			uvm_wait("fltamapcopy");
			continue;
		}

		/*
		 * got it!   unlock and return.
		 */

		uvmfault_unlockmaps(ufi, true);
		return;
	}
	/*NOTREACHED*/
}

/*
 * uvmfault_anonget: get data in an anon into a non-busy, non-released
 * page in that anon.
 *
 * => Map, amap and thus anon should be locked by caller.
 * => If we fail, we unlock everything and error is returned.
 * => If we are successful, return with everything still locked.
 * => We do not move the page on the queues [gets moved later].  If we
 *    allocate a new page [we_own], it gets put on the queues.  Either way,
 *    the result is that the page is on the queues at return time
 * => For pages which are on loan from a uvm_object (and thus are not owned
 *    by the anon): if successful, return with the owning object locked.
 *    The caller must unlock this object when it unlocks everything else.
 */

int
uvmfault_anonget(struct uvm_faultinfo *ufi, struct vm_amap *amap,
    struct vm_anon *anon)
{
	struct vm_page *pg;
	krw_t lock_type;
	int error;

	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);
	KASSERT(rw_lock_held(anon->an_lock));
	KASSERT(anon->an_lock == amap->am_lock);

	/* Increment the counters.*/
	cpu_count(CPU_COUNT_FLTANGET, 1);
	if (anon->an_page) {
		curlwp->l_ru.ru_minflt++;
	} else {
		curlwp->l_ru.ru_majflt++;
	}
	error = 0;

	/*
	 * Loop until we get the anon data, or fail.
	 */

	for (;;) {
		bool we_own, locked;
		/*
		 * Note: 'we_own' will become true if we set PG_BUSY on a page.
		 */
		we_own = false;
		pg = anon->an_page;

		/*
		 * If there is a resident page and it is loaned, then anon
		 * may not own it.  Call out to uvm_anon_lockloanpg() to
		 * identify and lock the real owner of the page.
		 */

		if (pg && pg->loan_count)
			pg = uvm_anon_lockloanpg(anon);

		/*
		 * Is page resident?  Make sure it is not busy/released.
		 */

		lock_type = rw_lock_op(anon->an_lock);
		if (pg) {

			/*
			 * at this point, if the page has a uobject [meaning
			 * we have it on loan], then that uobject is locked
			 * by us!   if the page is busy, we drop all the
			 * locks (including uobject) and try again.
			 */

			if ((pg->flags & PG_BUSY) == 0) {
				UVMHIST_LOG(maphist, "<- OK",0,0,0,0);
				return 0;
			}
			cpu_count(CPU_COUNT_FLTPGWAIT, 1);

			/*
			 * The last unlock must be an atomic unlock and wait
			 * on the owner of page.
			 */

			if (pg->uobject) {
				/* Owner of page is UVM object. */
				uvmfault_unlockall(ufi, amap, NULL);
				UVMHIST_LOG(maphist, " unlock+wait on uobj",0,
				    0,0,0);
				uvm_pagewait(pg, pg->uobject->vmobjlock, "anonget1");
			} else {
				/* Owner of page is anon. */
				uvmfault_unlockall(ufi, NULL, NULL);
				UVMHIST_LOG(maphist, " unlock+wait on anon",0,
				    0,0,0);
				uvm_pagewait(pg, anon->an_lock, "anonget2");
			}
		} else {
#if defined(VMSWAP)
			/*
			 * No page, therefore allocate one.  A write lock is
			 * required for this.  If the caller didn't supply
			 * one, fail now and have them retry.
			 */

			if (lock_type == RW_READER) {
				return ENOLCK;
			}
			pg = uvm_pagealloc(NULL,
			    ufi != NULL ? ufi->orig_rvaddr : 0,
			    anon, ufi != NULL ? UVM_FLAG_COLORMATCH : 0);
			if (pg == NULL) {
				/* Out of memory.  Wait a little. */
				uvmfault_unlockall(ufi, amap, NULL);
				cpu_count(CPU_COUNT_FLTNORAM, 1);
				UVMHIST_LOG(maphist, "  noram -- UVM_WAIT",0,
				    0,0,0);
				if (!uvm_reclaimable()) {
					return ENOMEM;
				}
				uvm_wait("flt_noram1");
			} else {
				/* PG_BUSY bit is set. */
				we_own = true;
				uvmfault_unlockall(ufi, amap, NULL);

				/*
				 * Pass a PG_BUSY+PG_FAKE clean page into
				 * the uvm_swap_get() function with all data
				 * structures unlocked.  Note that it is OK
				 * to read an_swslot here, because we hold
				 * PG_BUSY on the page.
				 */
				cpu_count(CPU_COUNT_PAGEINS, 1);
				error = uvm_swap_get(pg, anon->an_swslot,
				    PGO_SYNCIO);

				/*
				 * We clean up after the I/O below in the
				 * 'we_own' case.
				 */
			}
#else
			panic("%s: no page", __func__);
#endif /* defined(VMSWAP) */
		}

		/*
		 * Re-lock the map and anon.
		 */

		locked = uvmfault_relock(ufi);
		if (locked || we_own) {
			rw_enter(anon->an_lock, lock_type);
		}

		/*
		 * If we own the page (i.e. we set PG_BUSY), then we need
		 * to clean up after the I/O.  There are three cases to
		 * consider:
		 *
		 * 1) Page was released during I/O: free anon and ReFault.
		 * 2) I/O not OK.  Free the page and cause the fault to fail.
		 * 3) I/O OK!  Activate the page and sync with the non-we_own
		 *    case (i.e. drop anon lock if not locked).
		 */

		if (we_own) {
			KASSERT(lock_type == RW_WRITER);
#if defined(VMSWAP)
			if (error) {

				/*
				 * Remove the swap slot from the anon and
				 * mark the anon as having no real slot.
				 * Do not free the swap slot, thus preventing
				 * it from being used again.
				 */

				if (anon->an_swslot > 0) {
					uvm_swap_markbad(anon->an_swslot, 1);
				}
				anon->an_swslot = SWSLOT_BAD;

				if ((pg->flags & PG_RELEASED) != 0) {
					goto released;
				}

				/*
				 * Note: page was never !PG_BUSY, so it
				 * cannot be mapped and thus no need to
				 * pmap_page_protect() it.
				 */

				uvm_pagefree(pg);

				if (locked) {
					uvmfault_unlockall(ufi, NULL, NULL);
				}
				rw_exit(anon->an_lock);
				UVMHIST_LOG(maphist, "<- ERROR", 0,0,0,0);
				return error;
			}

			if ((pg->flags & PG_RELEASED) != 0) {
released:
				KASSERT(anon->an_ref == 0);

				/*
				 * Released while we had unlocked amap.
				 */

				if (locked) {
					uvmfault_unlockall(ufi, NULL, NULL);
				}
				uvm_anon_release(anon);

				if (error) {
					UVMHIST_LOG(maphist,
					    "<- ERROR/RELEASED", 0,0,0,0);
					return error;
				}

				UVMHIST_LOG(maphist, "<- RELEASED", 0,0,0,0);
				return ERESTART;
			}

			/*
			 * We have successfully read the page, activate it.
			 */

			uvm_pagelock(pg);
			uvm_pageactivate(pg);
			uvm_pagewakeup(pg);
			uvm_pageunlock(pg);
			pg->flags &= ~(PG_BUSY|PG_FAKE);
			uvm_pagemarkdirty(pg, UVM_PAGE_STATUS_UNKNOWN);
			UVM_PAGE_OWN(pg, NULL);
#else
			panic("%s: we_own", __func__);
#endif /* defined(VMSWAP) */
		}

		/*
		 * We were not able to re-lock the map - restart the fault.
		 */

		if (!locked) {
			if (we_own) {
				rw_exit(anon->an_lock);
			}
			UVMHIST_LOG(maphist, "<- REFAULT", 0,0,0,0);
			return ERESTART;
		}

		/*
		 * Verify that no one has touched the amap and moved
		 * the anon on us.
		 */

		if (ufi != NULL && amap_lookup(&ufi->entry->aref,
		    ufi->orig_rvaddr - ufi->entry->start) != anon) {

			uvmfault_unlockall(ufi, amap, NULL);
			UVMHIST_LOG(maphist, "<- REFAULT", 0,0,0,0);
			return ERESTART;
		}

		/*
		 * Retry..
		 */

		cpu_count(CPU_COUNT_FLTANRETRY, 1);
		continue;
	}
	/*NOTREACHED*/
}

/*
 * uvmfault_promote: promote data to a new anon.  used for 1B and 2B.
 *
 *	1. allocate an anon and a page.
 *	2. fill its contents.
 *	3. put it into amap.
 *
 * => if we fail (result != 0) we unlock everything.
 * => on success, return a new locked anon via 'nanon'.
 *    (*nanon)->an_page will be a resident, locked, dirty page.
 * => it's caller's responsibility to put the promoted nanon->an_page to the
 *    page queue.
 */

static int
uvmfault_promote(struct uvm_faultinfo *ufi,
    struct vm_anon *oanon,
    struct vm_page *uobjpage,
    struct vm_anon **nanon, /* OUT: allocated anon */
    struct vm_anon **spare)
{
	struct vm_amap *amap = ufi->entry->aref.ar_amap;
	struct uvm_object *uobj;
	struct vm_anon *anon;
	struct vm_page *pg;
	struct vm_page *opg;
	int error;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	if (oanon) {
		/* anon COW */
		opg = oanon->an_page;
		KASSERT(opg != NULL);
		KASSERT(opg->uobject == NULL || opg->loan_count > 0);
	} else if (uobjpage != PGO_DONTCARE) {
		/* object-backed COW */
		opg = uobjpage;
		KASSERT(rw_lock_held(opg->uobject->vmobjlock));
	} else {
		/* ZFOD */
		opg = NULL;
	}
	if (opg != NULL) {
		uobj = opg->uobject;
	} else {
		uobj = NULL;
	}

	KASSERT(amap != NULL);
	KASSERT(uobjpage != NULL);
	KASSERT(rw_write_held(amap->am_lock));
	KASSERT(oanon == NULL || amap->am_lock == oanon->an_lock);
	KASSERT(uobj == NULL || rw_lock_held(uobj->vmobjlock));

	if (*spare != NULL) {
		anon = *spare;
		*spare = NULL;
	} else {
		anon = uvm_analloc();
	}
	if (anon) {

		/*
		 * The new anon is locked.
		 *
		 * if opg == NULL, we want a zero'd, dirty page,
		 * so have uvm_pagealloc() do that for us.
		 */

		KASSERT(anon->an_lock == NULL);
		anon->an_lock = amap->am_lock;
		pg = uvm_pagealloc(NULL, ufi->orig_rvaddr, anon,
		    UVM_FLAG_COLORMATCH | (opg == NULL ? UVM_PGA_ZERO : 0));
		if (pg == NULL) {
			anon->an_lock = NULL;
		}
	} else {
		pg = NULL;
	}

	/*
	 * out of memory resources?
	 */

	if (pg == NULL) {
		/* save anon for the next try. */
		if (anon != NULL) {
			*spare = anon;
		}

		/* unlock and fail ... */
		uvmfault_unlockall(ufi, amap, uobj);
		if (!uvm_reclaimable()) {
			UVMHIST_LOG(maphist, "out of VM", 0,0,0,0);
			cpu_count(CPU_COUNT_FLTNOANON, 1);
			error = ENOMEM;
			goto done;
		}

		UVMHIST_LOG(maphist, "out of RAM, waiting for more", 0,0,0,0);
		cpu_count(CPU_COUNT_FLTNORAM, 1);
		uvm_wait("flt_noram5");
		error = ERESTART;
		goto done;
	}

	/* copy page [pg now dirty] */
	if (opg) {
		uvm_pagecopy(opg, pg);
	}
	KASSERT(uvm_pagegetdirty(pg) == UVM_PAGE_STATUS_DIRTY);

	amap_add(&ufi->entry->aref, ufi->orig_rvaddr - ufi->entry->start, anon,
	    oanon != NULL);

	/*
	 * from this point on am_lock won't be dropped until the page is
	 * entered, so it's safe to unbusy the page up front.
	 *
	 * uvm_fault_{upper,lower}_done will activate or enqueue the page.
	 */

	pg = anon->an_page;
	pg->flags &= ~(PG_BUSY|PG_FAKE);
	UVM_PAGE_OWN(pg, NULL);

	*nanon = anon;
	error = 0;
done:
	return error;
}

/*
 * Update statistics after fault resolution.
 * - maxrss
 */
void
uvmfault_update_stats(struct uvm_faultinfo *ufi)
{
	struct vm_map		*map;
	struct vmspace 		*vm;
	struct proc		*p;
	vsize_t			 res;

	map = ufi->orig_map;

	p = curproc;
	KASSERT(p != NULL);
	vm = p->p_vmspace;

	if (&vm->vm_map != map)
		return;

	res = pmap_resident_count(map->pmap);
	if (vm->vm_rssmax < res)
		vm->vm_rssmax = res;
}

/*
 *   F A U L T   -   m a i n   e n t r y   p o i n t
 */

/*
 * uvm_fault: page fault handler
 *
 * => called from MD code to resolve a page fault
 * => VM data structures usually should be unlocked.   however, it is
 *	possible to call here with the main map locked if the caller
 *	gets a write lock, sets it recusive, and then calls us (c.f.
 *	uvm_map_pageable).   this should be avoided because it keeps
 *	the map locked off during I/O.
 * => MUST NEVER BE CALLED IN INTERRUPT CONTEXT
 */

#define MASK(entry)     (UVM_ET_ISCOPYONWRITE(entry) ? \
			 ~VM_PROT_WRITE : VM_PROT_ALL)

/* fault_flag values passed from uvm_fault_wire to uvm_fault_internal */
#define UVM_FAULT_WIRE		(1 << 0)
#define UVM_FAULT_MAXPROT	(1 << 1)

struct uvm_faultctx {

	/*
	 * the following members are set up by uvm_fault_check() and
	 * read-only after that.
	 *
	 * note that narrow is used by uvm_fault_check() to change
	 * the behaviour after ERESTART.
	 *
	 * most of them might change after RESTART if the underlying
	 * map entry has been changed behind us.  an exception is
	 * wire_paging, which does never change.
	 */
	vm_prot_t access_type;
	vaddr_t startva;
	int npages;
	int centeridx;
	bool narrow;		/* work on a single requested page only */
	bool wire_mapping;	/* request a PMAP_WIRED mapping
				   (UVM_FAULT_WIRE or VM_MAPENT_ISWIRED) */
	bool wire_paging;	/* request uvm_pagewire
				   (true for UVM_FAULT_WIRE) */
	bool cow_now;		/* VM_PROT_WRITE is actually requested
				   (ie. should break COW and page loaning) */

	/*
	 * enter_prot is set up by uvm_fault_check() and clamped
	 * (ie. drop the VM_PROT_WRITE bit) in various places in case
	 * of !cow_now.
	 */
	vm_prot_t enter_prot;	/* prot at which we want to enter pages in */

	/*
	 * the following member is for uvmfault_promote() and ERESTART.
	 */
	struct vm_anon *anon_spare;

	/*
	 * the folloing is actually a uvm_fault_lower() internal.
	 * it's here merely for debugging.
	 * (or due to the mechanical separation of the function?)
	 */
	bool promote;

	/*
	 * type of lock to acquire on objects in both layers.
	 */
	krw_t lower_lock_type;
	krw_t upper_lock_type;
};

static inline int	uvm_fault_check(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct vm_anon ***, bool);

static int		uvm_fault_upper(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct vm_anon **);
static inline int	uvm_fault_upper_lookup(
			    struct uvm_faultinfo *, const struct uvm_faultctx *,
			    struct vm_anon **, struct vm_page **);
static inline void	uvm_fault_upper_neighbor(
			    struct uvm_faultinfo *, const struct uvm_faultctx *,
			    vaddr_t, struct vm_page *, bool);
static inline int	uvm_fault_upper_loan(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct vm_anon *, struct uvm_object **);
static inline int	uvm_fault_upper_promote(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct uvm_object *, struct vm_anon *);
static inline int	uvm_fault_upper_direct(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct uvm_object *, struct vm_anon *);
static int		uvm_fault_upper_enter(
			    struct uvm_faultinfo *, const struct uvm_faultctx *,
			    struct uvm_object *, struct vm_anon *,
			    struct vm_page *, struct vm_anon *);
static inline void	uvm_fault_upper_done(
			    struct uvm_faultinfo *, const struct uvm_faultctx *,
			    struct vm_anon *, struct vm_page *);

static int		uvm_fault_lower(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct vm_page **);
static inline void	uvm_fault_lower_lookup(
			    struct uvm_faultinfo *, const struct uvm_faultctx *,
			    struct vm_page **);
static inline void	uvm_fault_lower_neighbor(
			    struct uvm_faultinfo *, const struct uvm_faultctx *,
			    vaddr_t, struct vm_page *);
static inline int	uvm_fault_lower_io(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct uvm_object **, struct vm_page **);
static inline int	uvm_fault_lower_direct(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct uvm_object *, struct vm_page *);
static inline int	uvm_fault_lower_direct_loan(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct uvm_object *, struct vm_page **,
			    struct vm_page **);
static inline int	uvm_fault_lower_promote(
			    struct uvm_faultinfo *, struct uvm_faultctx *,
			    struct uvm_object *, struct vm_page *);
static int		uvm_fault_lower_enter(
			    struct uvm_faultinfo *, const struct uvm_faultctx *,
			    struct uvm_object *,
			    struct vm_anon *, struct vm_page *);
static inline void	uvm_fault_lower_done(
			    struct uvm_faultinfo *, const struct uvm_faultctx *,
			    struct uvm_object *, struct vm_page *);

int
uvm_fault_internal(struct vm_map *orig_map, vaddr_t vaddr,
    vm_prot_t access_type, int fault_flag)
{
	struct uvm_faultinfo ufi;
	struct uvm_faultctx flt = {
		.access_type = access_type,

		/* don't look for neighborhood * pages on "wire" fault */
		.narrow = (fault_flag & UVM_FAULT_WIRE) != 0,

		/* "wire" fault causes wiring of both mapping and paging */
		.wire_mapping = (fault_flag & UVM_FAULT_WIRE) != 0,
		.wire_paging = (fault_flag & UVM_FAULT_WIRE) != 0,

		/*
		 * default lock type to acquire on upper & lower layer
		 * objects: reader.  this can be upgraded at any point
		 * during the fault from read -> write and uvm_faultctx
		 * changed to match, but is never downgraded write -> read.
		 */
#ifdef __HAVE_UNLOCKED_PMAP /* XXX temporary */
		.upper_lock_type = RW_WRITER,
		.lower_lock_type = RW_WRITER,
#else
		.upper_lock_type = RW_READER,
		.lower_lock_type = RW_READER,
#endif
	};
	const bool maxprot = (fault_flag & UVM_FAULT_MAXPROT) != 0;
	struct vm_anon *anons_store[UVM_MAXRANGE], **anons;
	struct vm_page *pages_store[UVM_MAXRANGE], **pages;
	int error;

	UVMHIST_FUNC(__func__);
	UVMHIST_CALLARGS(maphist, "(map=%#jx, vaddr=%#jx, at=%jd, ff=%jd)",
	      (uintptr_t)orig_map, vaddr, access_type, fault_flag);

	/* Don't count anything until user interaction is possible */
	kpreempt_disable();
	if (__predict_true(start_init_exec)) {
		struct cpu_info *ci = curcpu();
		CPU_COUNT(CPU_COUNT_NFAULT, 1);
		/* Don't flood RNG subsystem with samples. */
		if (++(ci->ci_faultrng) == 503) {
			ci->ci_faultrng = 0;
			rnd_add_uint32(&curcpu()->ci_data.cpu_uvm->rs,
			    sizeof(vaddr_t) == sizeof(uint32_t) ?
			    (uint32_t)vaddr : sizeof(vaddr_t) ==
			    sizeof(uint64_t) ?
			    (uint32_t)vaddr :
			    (uint32_t)ci->ci_counts[CPU_COUNT_NFAULT]);
		}
	}
	kpreempt_enable();

	/*
	 * init the IN parameters in the ufi
	 */

	ufi.orig_map = orig_map;
	ufi.orig_rvaddr = trunc_page(vaddr);
	ufi.orig_size = PAGE_SIZE;	/* can't get any smaller than this */

	error = ERESTART;
	while (error == ERESTART) { /* ReFault: */
		anons = anons_store;
		pages = pages_store;

		error = uvm_fault_check(&ufi, &flt, &anons, maxprot);
		if (error != 0)
			continue;

		error = uvm_fault_upper_lookup(&ufi, &flt, anons, pages);
		if (error != 0)
			continue;

		if (pages[flt.centeridx] == PGO_DONTCARE)
			error = uvm_fault_upper(&ufi, &flt, anons);
		else {
			struct uvm_object * const uobj =
			    ufi.entry->object.uvm_obj;

			if (uobj && uobj->pgops->pgo_fault != NULL) {
				/*
				 * invoke "special" fault routine.
				 */
				rw_enter(uobj->vmobjlock, RW_WRITER);
				/* locked: maps(read), amap(if there), uobj */
				error = uobj->pgops->pgo_fault(&ufi,
				    flt.startva, pages, flt.npages,
				    flt.centeridx, flt.access_type,
				    PGO_LOCKED|PGO_SYNCIO);

				/*
				 * locked: nothing, pgo_fault has unlocked
				 * everything
				 */

				/*
				 * object fault routine responsible for
				 * pmap_update().
				 */

				/*
				 * Wake up the pagedaemon if the fault method
				 * failed for lack of memory but some can be
				 * reclaimed.
				 */
				if (error == ENOMEM && uvm_reclaimable()) {
					uvm_wait("pgo_fault");
					error = ERESTART;
				}
			} else {
				error = uvm_fault_lower(&ufi, &flt, pages);
			}
		}
	}

	if (flt.anon_spare != NULL) {
		flt.anon_spare->an_ref--;
		KASSERT(flt.anon_spare->an_ref == 0);
		KASSERT(flt.anon_spare->an_lock == NULL);
		uvm_anfree(flt.anon_spare);
	}
	return error;
}

/*
 * uvm_fault_check: check prot, handle needs-copy, etc.
 *
 *	1. lookup entry.
 *	2. check protection.
 *	3. adjust fault condition (mainly for simulated fault).
 *	4. handle needs-copy (lazy amap copy).
 *	5. establish range of interest for neighbor fault (aka pre-fault).
 *	6. look up anons (if amap exists).
 *	7. flush pages (if MADV_SEQUENTIAL)
 *
 * => called with nothing locked.
 * => if we fail (result != 0) we unlock everything.
 * => initialize/adjust many members of flt.
 */

static int
uvm_fault_check(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct vm_anon ***ranons, bool maxprot)
{
	struct vm_amap *amap;
	struct uvm_object *uobj;
	vm_prot_t check_prot;
	int nback, nforw;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/*
	 * lookup and lock the maps
	 */

	if (uvmfault_lookup(ufi, false) == false) {
		UVMHIST_LOG(maphist, "<- no mapping @ %#jx", ufi->orig_rvaddr,
		    0,0,0);
		return EFAULT;
	}
	/* locked: maps(read) */

#ifdef DIAGNOSTIC
	if ((ufi->map->flags & VM_MAP_PAGEABLE) == 0) {
		printf("Page fault on non-pageable map:\n");
		printf("ufi->map = %p\n", ufi->map);
		printf("ufi->orig_map = %p\n", ufi->orig_map);
		printf("ufi->orig_rvaddr = %#lx\n", (u_long) ufi->orig_rvaddr);
		panic("uvm_fault: (ufi->map->flags & VM_MAP_PAGEABLE) == 0");
	}
#endif

	/*
	 * check protection
	 */

	check_prot = maxprot ?
	    ufi->entry->max_protection : ufi->entry->protection;
	if ((check_prot & flt->access_type) != flt->access_type) {
		UVMHIST_LOG(maphist,
		    "<- protection failure (prot=%#jx, access=%#jx)",
		    ufi->entry->protection, flt->access_type, 0, 0);
		uvmfault_unlockmaps(ufi, false);
		return EFAULT;
	}

	/*
	 * "enter_prot" is the protection we want to enter the page in at.
	 * for certain pages (e.g. copy-on-write pages) this protection can
	 * be more strict than ufi->entry->protection.  "wired" means either
	 * the entry is wired or we are fault-wiring the pg.
	 */

	flt->enter_prot = ufi->entry->protection;
	if (VM_MAPENT_ISWIRED(ufi->entry)) {
		flt->wire_mapping = true;
		flt->wire_paging = true;
		flt->narrow = true;
	}

	if (flt->wire_mapping) {
		flt->access_type = flt->enter_prot; /* full access for wired */
		flt->cow_now = (check_prot & VM_PROT_WRITE) != 0;
	} else {
		flt->cow_now = (flt->access_type & VM_PROT_WRITE) != 0;
	}

	if (flt->wire_paging) {
		/* wiring pages requires a write lock. */
		flt->upper_lock_type = RW_WRITER;
		flt->lower_lock_type = RW_WRITER;
	}

	flt->promote = false;

	/*
	 * handle "needs_copy" case.   if we need to copy the amap we will
	 * have to drop our readlock and relock it with a write lock.  (we
	 * need a write lock to change anything in a map entry [e.g.
	 * needs_copy]).
	 */

	if (UVM_ET_ISNEEDSCOPY(ufi->entry)) {
		if (flt->cow_now || (ufi->entry->object.uvm_obj == NULL)) {
			KASSERT(!maxprot);
			/* need to clear */
			UVMHIST_LOG(maphist,
			    "  need to clear needs_copy and refault",0,0,0,0);
			uvmfault_unlockmaps(ufi, false);
			uvmfault_amapcopy(ufi);
			cpu_count(CPU_COUNT_FLTAMCOPY, 1);
			return ERESTART;

		} else {

			/*
			 * ensure that we pmap_enter page R/O since
			 * needs_copy is still true
			 */

			flt->enter_prot &= ~VM_PROT_WRITE;
		}
	}

	/*
	 * identify the players
	 */

	amap = ufi->entry->aref.ar_amap;	/* upper layer */
	uobj = ufi->entry->object.uvm_obj;	/* lower layer */

	/*
	 * check for a case 0 fault.  if nothing backing the entry then
	 * error now.
	 */

	if (amap == NULL && uobj == NULL) {
		uvmfault_unlockmaps(ufi, false);
		UVMHIST_LOG(maphist,"<- no backing store, no overlay",0,0,0,0);
		return EFAULT;
	}

	/*
	 * for a case 2B fault waste no time on adjacent pages because
	 * they are likely already entered.
	 */

	if (uobj != NULL && amap != NULL &&
	    (flt->access_type & VM_PROT_WRITE) != 0) {
		/* wide fault (!narrow) */
		flt->narrow = true;
	}

	/*
	 * establish range of interest based on advice from mapper
	 * and then clip to fit map entry.   note that we only want
	 * to do this the first time through the fault.   if we
	 * ReFault we will disable this by setting "narrow" to true.
	 */

	if (flt->narrow == false) {

		/* wide fault (!narrow) */
		KASSERT(uvmadvice[ufi->entry->advice].advice ==
			 ufi->entry->advice);
		nback = MIN(uvmadvice[ufi->entry->advice].nback,
		    (ufi->orig_rvaddr - ufi->entry->start) >> PAGE_SHIFT);
		flt->startva = ufi->orig_rvaddr - (nback << PAGE_SHIFT);
		/*
		 * note: "-1" because we don't want to count the
		 * faulting page as forw
		 */
		nforw = MIN(uvmadvice[ufi->entry->advice].nforw,
			    ((ufi->entry->end - ufi->orig_rvaddr) >>
			     PAGE_SHIFT) - 1);
		flt->npages = nback + nforw + 1;
		flt->centeridx = nback;

		flt->narrow = true;	/* ensure only once per-fault */

	} else {

		/* narrow fault! */
		nback = nforw = 0;
		flt->startva = ufi->orig_rvaddr;
		flt->npages = 1;
		flt->centeridx = 0;

	}
	/* offset from entry's start to pgs' start */
	const voff_t eoff = flt->startva - ufi->entry->start;

	/* locked: maps(read) */
	UVMHIST_LOG(maphist, "  narrow=%jd, back=%jd, forw=%jd, startva=%#jx",
		    flt->narrow, nback, nforw, flt->startva);
	UVMHIST_LOG(maphist, "  entry=%#jx, amap=%#jx, obj=%#jx",
	    (uintptr_t)ufi->entry, (uintptr_t)amap, (uintptr_t)uobj, 0);

	/*
	 * guess at the most suitable lock types to acquire.
	 * if we've got an amap then lock it and extract current anons.
	 */

	if (amap) {
		if ((amap_flags(amap) & AMAP_SHARED) == 0) {
			/*
			 * the amap isn't shared.  get a writer lock to
			 * avoid the cost of upgrading the lock later if
			 * needed.
			 *
			 * XXX nice for PostgreSQL, but consider threads.
			 */
			flt->upper_lock_type = RW_WRITER;
		} else if ((flt->access_type & VM_PROT_WRITE) != 0) {
			/*
			 * assume we're about to COW.
			 */
			flt->upper_lock_type = RW_WRITER;
		}
		amap_lock(amap, flt->upper_lock_type);
		amap_lookups(&ufi->entry->aref, eoff, *ranons, flt->npages);
	} else {
		if ((flt->access_type & VM_PROT_WRITE) != 0) {
			/*
			 * we are about to dirty the object and that
			 * requires a write lock.
			 */
			flt->lower_lock_type = RW_WRITER;
		}
		*ranons = NULL;	/* to be safe */
	}

	/* locked: maps(read), amap(if there) */
	KASSERT(amap == NULL ||
	    rw_lock_op(amap->am_lock) == flt->upper_lock_type);

	/*
	 * for MADV_SEQUENTIAL mappings we want to deactivate the back pages
	 * now and then forget about them (for the rest of the fault).
	 */

	if (ufi->entry->advice == MADV_SEQUENTIAL && nback != 0) {

		UVMHIST_LOG(maphist, "  MADV_SEQUENTIAL: flushing backpages",
		    0,0,0,0);
		/* flush back-page anons? */
		if (amap)
			uvmfault_anonflush(*ranons, nback);

		/*
		 * flush object?  change lock type to RW_WRITER, to avoid
		 * excessive competition between read/write locks if many
		 * threads doing "sequential access".
		 */
		if (uobj) {
			voff_t uoff;

			flt->lower_lock_type = RW_WRITER;
			uoff = ufi->entry->offset + eoff;
			rw_enter(uobj->vmobjlock, RW_WRITER);
			(void) (uobj->pgops->pgo_put)(uobj, uoff, uoff +
				    (nback << PAGE_SHIFT), PGO_DEACTIVATE);
		}

		/* now forget about the backpages */
		if (amap)
			*ranons += nback;
		flt->startva += (nback << PAGE_SHIFT);
		flt->npages -= nback;
		flt->centeridx = 0;
	}
	/*
	 * => startva is fixed
	 * => npages is fixed
	 */
	KASSERT(flt->startva <= ufi->orig_rvaddr);
	KASSERT(ufi->orig_rvaddr + ufi->orig_size <=
	    flt->startva + (flt->npages << PAGE_SHIFT));
	return 0;
}

/*
 * uvm_fault_upper_upgrade: upgrade upper lock, reader -> writer
 */

static inline int
uvm_fault_upper_upgrade(struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
    struct vm_amap *amap, struct uvm_object *uobj)
{
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	KASSERT(amap != NULL);
	KASSERT(flt->upper_lock_type == rw_lock_op(amap->am_lock));

	/*
	 * fast path.
	 */

	if (__predict_true(flt->upper_lock_type == RW_WRITER)) {
		return 0;
	}

	/*
	 * otherwise try for the upgrade.  if we don't get it, unlock
	 * everything, restart the fault and next time around get a writer
	 * lock.
	 */

	flt->upper_lock_type = RW_WRITER;
	if (__predict_false(!rw_tryupgrade(amap->am_lock))) {
		uvmfault_unlockall(ufi, amap, uobj);
		cpu_count(CPU_COUNT_FLTNOUP, 1);
		UVMHIST_LOG(maphist, "  !upgrade upper", 0, 0,0,0);
		return ERESTART;
	}
	cpu_count(CPU_COUNT_FLTUP, 1);
	KASSERT(flt->upper_lock_type == rw_lock_op(amap->am_lock));
	return 0;
}

/*
 * uvm_fault_upper_lookup: look up existing h/w mapping and amap.
 *
 * iterate range of interest:
 *	1. check if h/w mapping exists.  if yes, we don't care
 *	2. check if anon exists.  if not, page is lower.
 *	3. if anon exists, enter h/w mapping for neighbors.
 *
 * => called with amap locked (if exists).
 */

static int
uvm_fault_upper_lookup(
	struct uvm_faultinfo *ufi, const struct uvm_faultctx *flt,
	struct vm_anon **anons, struct vm_page **pages)
{
	struct vm_amap *amap = ufi->entry->aref.ar_amap;
	int lcv;
	vaddr_t currva;
	bool shadowed __unused;
	bool entered;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/* locked: maps(read), amap(if there) */
	KASSERT(amap == NULL ||
	    rw_lock_op(amap->am_lock) == flt->upper_lock_type);

	/*
	 * map in the backpages and frontpages we found in the amap in hopes
	 * of preventing future faults.    we also init the pages[] array as
	 * we go.
	 */

	currva = flt->startva;
	shadowed = false;
	entered = false;
	for (lcv = 0; lcv < flt->npages; lcv++, currva += PAGE_SIZE) {
		/*
		 * unmapped or center page.   check if any anon at this level.
		 */
		if (amap == NULL || anons[lcv] == NULL) {
			pages[lcv] = NULL;
			continue;
		}

		/*
		 * check for present page and map if possible.
		 */

		pages[lcv] = PGO_DONTCARE;
		if (lcv == flt->centeridx) {	/* save center for later! */
			shadowed = true;
			continue;
		}

		struct vm_anon *anon = anons[lcv];
		struct vm_page *pg = anon->an_page;

		KASSERT(anon->an_lock == amap->am_lock);

		/*
		 * ignore loaned and busy pages.
		 * don't play with VAs that are already mapped.
		 */

		if (pg && pg->loan_count == 0 && (pg->flags & PG_BUSY) == 0 &&
		    !pmap_extract(ufi->orig_map->pmap, currva, NULL)) {
			uvm_fault_upper_neighbor(ufi, flt, currva,
			    pg, anon->an_ref > 1);
			entered = true;
		}
	}
	if (entered) {
		pmap_update(ufi->orig_map->pmap);
	}

	/* locked: maps(read), amap(if there) */
	KASSERT(amap == NULL ||
	    rw_lock_op(amap->am_lock) == flt->upper_lock_type);
	/* (shadowed == true) if there is an anon at the faulting address */
	UVMHIST_LOG(maphist, "  shadowed=%jd, will_get=%jd", shadowed,
	    (ufi->entry->object.uvm_obj && shadowed != false),0,0);

	return 0;
}

/*
 * uvm_fault_upper_neighbor: enter single upper neighbor page.
 *
 * => called with amap and anon locked.
 */

static void
uvm_fault_upper_neighbor(
	struct uvm_faultinfo *ufi, const struct uvm_faultctx *flt,
	vaddr_t currva, struct vm_page *pg, bool readonly)
{
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/* locked: amap, anon */

	KASSERT(pg->uobject == NULL);
	KASSERT(pg->uanon != NULL);
	KASSERT(rw_lock_op(pg->uanon->an_lock) == flt->upper_lock_type);
	KASSERT(uvm_pagegetdirty(pg) != UVM_PAGE_STATUS_CLEAN);

	/*
	 * there wasn't a direct fault on the page, so avoid the cost of
	 * activating it.
	 */

	if (!uvmpdpol_pageisqueued_p(pg) && pg->wire_count == 0) {
		uvm_pagelock(pg);
		uvm_pageenqueue(pg);
		uvm_pageunlock(pg);
	}

	UVMHIST_LOG(maphist,
	    "  MAPPING: n anon: pm=%#jx, va=%#jx, pg=%#jx",
	    (uintptr_t)ufi->orig_map->pmap, currva, (uintptr_t)pg, 0);
	cpu_count(CPU_COUNT_FLTNAMAP, 1);

	/*
	 * Since this page isn't the page that's actually faulting,
	 * ignore pmap_enter() failures; it's not critical that we
	 * enter these right now.
	 */

	(void) pmap_enter(ufi->orig_map->pmap, currva,
	    VM_PAGE_TO_PHYS(pg),
	    readonly ? (flt->enter_prot & ~VM_PROT_WRITE) :
	    flt->enter_prot,
	    PMAP_CANFAIL | (flt->wire_mapping ? PMAP_WIRED : 0));
}

/*
 * uvm_fault_upper: handle upper fault.
 *
 *	1. acquire anon lock.
 *	2. get anon.  let uvmfault_anonget do the dirty work.
 *	3. handle loan.
 *	4. dispatch direct or promote handlers.
 */

static int
uvm_fault_upper(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct vm_anon **anons)
{
	struct vm_amap * const amap = ufi->entry->aref.ar_amap;
	struct vm_anon * const anon = anons[flt->centeridx];
	struct uvm_object *uobj;
	int error;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/* locked: maps(read), amap, anon */
	KASSERT(rw_lock_op(amap->am_lock) == flt->upper_lock_type);
	KASSERT(anon->an_lock == amap->am_lock);

	/*
	 * handle case 1: fault on an anon in our amap
	 */

	UVMHIST_LOG(maphist, "  case 1 fault: anon=%#jx",
	    (uintptr_t)anon, 0, 0, 0);

	/*
	 * no matter if we have case 1A or case 1B we are going to need to
	 * have the anon's memory resident.   ensure that now.
	 */

	/*
	 * let uvmfault_anonget do the dirty work.
	 * if it fails (!OK) it will unlock everything for us.
	 * if it succeeds, locks are still valid and locked.
	 * also, if it is OK, then the anon's page is on the queues.
	 * if the page is on loan from a uvm_object, then anonget will
	 * lock that object for us if it does not fail.
	 */
 retry:
	error = uvmfault_anonget(ufi, amap, anon);
	switch (error) {
	case 0:
		break;

	case ERESTART:
		return ERESTART;

	case EAGAIN:
		kpause("fltagain1", false, hz/2, NULL);
		return ERESTART;

	case ENOLCK:
		/* it needs a write lock: retry */
		error = uvm_fault_upper_upgrade(ufi, flt, amap, NULL);
		if (error != 0) {
			return error;
		}
		KASSERT(rw_write_held(amap->am_lock));
		goto retry;

	default:
		return error;
	}

	/*
	 * uobj is non null if the page is on loan from an object (i.e. uobj)
	 */

	uobj = anon->an_page->uobject;	/* locked by anonget if !NULL */

	/* locked: maps(read), amap, anon, uobj(if one) */
	KASSERT(rw_lock_op(amap->am_lock) == flt->upper_lock_type);
	KASSERT(anon->an_lock == amap->am_lock);
	KASSERT(uobj == NULL ||
	    rw_lock_op(uobj->vmobjlock) == flt->lower_lock_type);

	/*
	 * special handling for loaned pages
	 */

	if (anon->an_page->loan_count) {
		error = uvm_fault_upper_loan(ufi, flt, anon, &uobj);
		if (error != 0)
			return error;
	}

	/*
	 * if we are case 1B then we will need to allocate a new blank
	 * anon to transfer the data into.   note that we have a lock
	 * on anon, so no one can busy or release the page until we are done.
	 * also note that the ref count can't drop to zero here because
	 * it is > 1 and we are only dropping one ref.
	 *
	 * in the (hopefully very rare) case that we are out of RAM we
	 * will unlock, wait for more RAM, and refault.
	 *
	 * if we are out of anon VM we kill the process (XXX: could wait?).
	 */

	if (flt->cow_now && anon->an_ref > 1) {
		flt->promote = true;
		error = uvm_fault_upper_promote(ufi, flt, uobj, anon);
	} else {
		error = uvm_fault_upper_direct(ufi, flt, uobj, anon);
	}
	return error;
}

/*
 * uvm_fault_upper_loan: handle loaned upper page.
 *
 *	1. if not cow'ing now, simply adjust flt->enter_prot.
 *	2. if cow'ing now, and if ref count is 1, break loan.
 */

static int
uvm_fault_upper_loan(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct vm_anon *anon, struct uvm_object **ruobj)
{
	struct vm_amap * const amap = ufi->entry->aref.ar_amap;
	int error = 0;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	if (!flt->cow_now) {

		/*
		 * for read faults on loaned pages we just cap the
		 * protection at read-only.
		 */

		flt->enter_prot = flt->enter_prot & ~VM_PROT_WRITE;

	} else {
		/*
		 * note that we can't allow writes into a loaned page!
		 *
		 * if we have a write fault on a loaned page in an
		 * anon then we need to look at the anon's ref count.
		 * if it is greater than one then we are going to do
		 * a normal copy-on-write fault into a new anon (this
		 * is not a problem).  however, if the reference count
		 * is one (a case where we would normally allow a
		 * write directly to the page) then we need to kill
		 * the loan before we continue.
		 */

		/* >1 case is already ok */
		if (anon->an_ref == 1) {
			/* breaking loan requires a write lock. */
			error = uvm_fault_upper_upgrade(ufi, flt, amap, NULL);
			if (error != 0) {
				return error;
			}
			KASSERT(rw_write_held(amap->am_lock));

			error = uvm_loanbreak_anon(anon, *ruobj);
			if (error != 0) {
				uvmfault_unlockall(ufi, amap, *ruobj);
				uvm_wait("flt_noram2");
				return ERESTART;
			}
			/* if we were a loan receiver uobj is gone */
			if (*ruobj)
				*ruobj = NULL;
		}
	}
	return error;
}

/*
 * uvm_fault_upper_promote: promote upper page.
 *
 *	1. call uvmfault_promote.
 *	2. enqueue page.
 *	3. deref.
 *	4. pass page to uvm_fault_upper_enter.
 */

static int
uvm_fault_upper_promote(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct uvm_object *uobj, struct vm_anon *anon)
{
	struct vm_amap * const amap = ufi->entry->aref.ar_amap;
	struct vm_anon * const oanon = anon;
	struct vm_page *pg;
	int error;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	UVMHIST_LOG(maphist, "  case 1B: COW fault",0,0,0,0);
	cpu_count(CPU_COUNT_FLT_ACOW, 1);

	/* promoting requires a write lock. */
	error = uvm_fault_upper_upgrade(ufi, flt, amap, NULL);
	if (error != 0) {
		return error;
	}
	KASSERT(rw_write_held(amap->am_lock));

	error = uvmfault_promote(ufi, oanon, PGO_DONTCARE, &anon,
	    &flt->anon_spare);
	switch (error) {
	case 0:
		break;
	case ERESTART:
		return ERESTART;
	default:
		return error;
	}
	pg = anon->an_page;

	KASSERT(anon->an_lock == oanon->an_lock);
	KASSERT((pg->flags & (PG_BUSY | PG_FAKE)) == 0);

	/* deref: can not drop to zero here by defn! */
	KASSERT(oanon->an_ref > 1);
	oanon->an_ref--;

	/*
	 * note: oanon is still locked, as is the new anon.  we
	 * need to check for this later when we unlock oanon; if
	 * oanon != anon, we'll have to unlock anon, too.
	 */

	return uvm_fault_upper_enter(ufi, flt, uobj, anon, pg, oanon);
}

/*
 * uvm_fault_upper_direct: handle direct fault.
 */

static int
uvm_fault_upper_direct(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct uvm_object *uobj, struct vm_anon *anon)
{
	struct vm_anon * const oanon = anon;
	struct vm_page *pg;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	cpu_count(CPU_COUNT_FLT_ANON, 1);
	pg = anon->an_page;
	if (anon->an_ref > 1)     /* disallow writes to ref > 1 anons */
		flt->enter_prot = flt->enter_prot & ~VM_PROT_WRITE;

	return uvm_fault_upper_enter(ufi, flt, uobj, anon, pg, oanon);
}

/*
 * uvm_fault_upper_enter: enter h/w mapping of upper page.
 */

static int
uvm_fault_upper_enter(
	struct uvm_faultinfo *ufi, const struct uvm_faultctx *flt,
	struct uvm_object *uobj, struct vm_anon *anon, struct vm_page *pg,
	struct vm_anon *oanon)
{
	struct pmap *pmap = ufi->orig_map->pmap;
	vaddr_t va = ufi->orig_rvaddr;
	struct vm_amap * const amap = ufi->entry->aref.ar_amap;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/* locked: maps(read), amap, oanon, anon(if different from oanon) */
	KASSERT(rw_lock_op(amap->am_lock) == flt->upper_lock_type);
	KASSERT(anon->an_lock == amap->am_lock);
	KASSERT(oanon->an_lock == amap->am_lock);
	KASSERT(uobj == NULL ||
	    rw_lock_op(uobj->vmobjlock) == flt->lower_lock_type);
	KASSERT(uvm_pagegetdirty(pg) != UVM_PAGE_STATUS_CLEAN);

	/*
	 * now map the page in.
	 */

	UVMHIST_LOG(maphist,
	    "  MAPPING: anon: pm=%#jx, va=%#jx, pg=%#jx, promote=%jd",
	    (uintptr_t)pmap, va, (uintptr_t)pg, flt->promote);
	if (pmap_enter(pmap, va, VM_PAGE_TO_PHYS(pg),
	    flt->enter_prot, flt->access_type | PMAP_CANFAIL |
	    (flt->wire_mapping ? PMAP_WIRED : 0)) != 0) {

		/*
		 * If pmap_enter() fails, it must not leave behind an existing
		 * pmap entry.  In particular, a now-stale entry for a different
		 * page would leave the pmap inconsistent with the vm_map.
		 * This is not to imply that pmap_enter() should remove an
		 * existing mapping in such a situation (since that could create
		 * different problems, eg. if the existing mapping is wired),
		 * but rather that the pmap should be designed such that it
		 * never needs to fail when the new mapping is replacing an
		 * existing mapping and the new page has no existing mappings.
		 *
		 * XXX This can't be asserted safely any more because many
		 * LWPs and/or many processes could simultaneously fault on
		 * the same VA and some might succeed.
		 */

		/* KASSERT(!pmap_extract(pmap, va, NULL)); */

		/*
		 * ensure that the page is queued in the case that
		 * we just promoted.
		 */

		uvm_pagelock(pg);
		uvm_pageenqueue(pg);
		uvm_pageunlock(pg);

		/*
		 * No need to undo what we did; we can simply think of
		 * this as the pmap throwing away the mapping information.
		 *
		 * We do, however, have to go through the ReFault path,
		 * as the map may change while we're asleep.
		 */

		uvmfault_unlockall(ufi, amap, uobj);
		if (!uvm_reclaimable()) {
			UVMHIST_LOG(maphist,
			    "<- failed.  out of VM",0,0,0,0);
			/* XXX instrumentation */
			return ENOMEM;
		}
		/* XXX instrumentation */
		uvm_wait("flt_pmfail1");
		return ERESTART;
	}

	uvm_fault_upper_done(ufi, flt, anon, pg);

	/*
	 * done case 1!  finish up by unlocking everything and returning success
	 */

	pmap_update(pmap);
	uvmfault_unlockall(ufi, amap, uobj);
	return 0;
}

/*
 * uvm_fault_upper_done: queue upper center page.
 */

static void
uvm_fault_upper_done(
	struct uvm_faultinfo *ufi, const struct uvm_faultctx *flt,
	struct vm_anon *anon, struct vm_page *pg)
{
	const bool wire_paging = flt->wire_paging;

	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/*
	 * ... update the page queues.
	 */

	if (wire_paging) {
		uvm_pagelock(pg);
		uvm_pagewire(pg);
		uvm_pageunlock(pg);

		/*
		 * since the now-wired page cannot be paged out,
		 * release its swap resources for others to use.
		 * and since an anon with no swap cannot be clean,
		 * mark it dirty now.
		 */

		uvm_pagemarkdirty(pg, UVM_PAGE_STATUS_DIRTY);
		uvm_anon_dropswap(anon);
	} else if (uvmpdpol_pageactivate_p(pg)) {
		/*
		 * avoid re-activating the page unless needed,
		 * to avoid false sharing on multiprocessor.
		 */

		uvm_pagelock(pg);
		uvm_pageactivate(pg);
		uvm_pageunlock(pg);
	}
}

/*
 * uvm_fault_lower_upgrade: upgrade lower lock, reader -> writer
 */

static inline int
uvm_fault_lower_upgrade(struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
    struct vm_amap *amap, struct uvm_object *uobj, struct vm_page *uobjpage)
{

	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	KASSERT(uobj != NULL);
	KASSERT(flt->lower_lock_type == rw_lock_op(uobj->vmobjlock));

	/*
	 * fast path.
	 */

	if (__predict_true(flt->lower_lock_type == RW_WRITER)) {
		return 0;
	}

	/*
	 * otherwise try for the upgrade.  if we don't get it, unlock
	 * everything, restart the fault and next time around get a writer
	 * lock.
	 */

	flt->lower_lock_type = RW_WRITER;
	if (__predict_false(!rw_tryupgrade(uobj->vmobjlock))) {
		uvmfault_unlockall(ufi, amap, uobj);
		cpu_count(CPU_COUNT_FLTNOUP, 1);
		UVMHIST_LOG(maphist, "  !upgrade lower", 0, 0,0,0);
		return ERESTART;
	}
	cpu_count(CPU_COUNT_FLTUP, 1);
	KASSERT(flt->lower_lock_type == rw_lock_op(uobj->vmobjlock));
	return 0;
}

/*
 * uvm_fault_lower: handle lower fault.
 *
 *	1. check uobj
 *	1.1. if null, ZFOD.
 *	1.2. if not null, look up unnmapped neighbor pages.
 *	2. for center page, check if promote.
 *	2.1. ZFOD always needs promotion.
 *	2.2. other uobjs, when entry is marked COW (usually MAP_PRIVATE vnode).
 *	3. if uobj is not ZFOD and page is not found, do i/o.
 *	4. dispatch either direct / promote fault.
 */

static int
uvm_fault_lower(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct vm_page **pages)
{
	struct vm_amap *amap __diagused = ufi->entry->aref.ar_amap;
	struct uvm_object *uobj = ufi->entry->object.uvm_obj;
	struct vm_page *uobjpage;
	int error;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/*
	 * now, if the desired page is not shadowed by the amap and we have
	 * a backing object that does not have a special fault routine, then
	 * we ask (with pgo_get) the object for resident pages that we care
	 * about and attempt to map them in.  we do not let pgo_get block
	 * (PGO_LOCKED).
	 */

	if (uobj == NULL) {
		/* zero fill; don't care neighbor pages */
		uobjpage = NULL;
	} else {
		uvm_fault_lower_lookup(ufi, flt, pages);
		uobjpage = pages[flt->centeridx];
	}

	/*
	 * note that at this point we are done with any front or back pages.
	 * we are now going to focus on the center page (i.e. the one we've
	 * faulted on).  if we have faulted on the upper (anon) layer
	 * [i.e. case 1], then the anon we want is anons[centeridx] (we have
	 * not touched it yet).  if we have faulted on the bottom (uobj)
	 * layer [i.e. case 2] and the page was both present and available,
	 * then we've got a pointer to it as "uobjpage" and we've already
	 * made it BUSY.
	 */

	/*
	 * locked:
	 * maps(read), amap(if there), uobj(if !null), uobjpage(if !null)
	 */
	KASSERT(amap == NULL ||
	    rw_lock_op(amap->am_lock) == flt->upper_lock_type);
	KASSERT(uobj == NULL ||
	    rw_lock_op(uobj->vmobjlock) == flt->lower_lock_type);

	/*
	 * note that uobjpage can not be PGO_DONTCARE at this point.  we now
	 * set uobjpage to PGO_DONTCARE if we are doing a zero fill.  if we
	 * have a backing object, check and see if we are going to promote
	 * the data up to an anon during the fault.
	 */

	if (uobj == NULL) {
		uobjpage = PGO_DONTCARE;
		flt->promote = true;		/* always need anon here */
	} else {
		KASSERT(uobjpage != PGO_DONTCARE);
		flt->promote = flt->cow_now && UVM_ET_ISCOPYONWRITE(ufi->entry);
	}
	UVMHIST_LOG(maphist, "  case 2 fault: promote=%jd, zfill=%jd",
	    flt->promote, (uobj == NULL), 0,0);

	/*
	 * if uobjpage is not null then we do not need to do I/O to get the
	 * uobjpage.
	 *
	 * if uobjpage is null, then we need to unlock and ask the pager to
	 * get the data for us.   once we have the data, we need to reverify
	 * the state the world.   we are currently not holding any resources.
	 */

	if (uobjpage) {
		/* update rusage counters */
		curlwp->l_ru.ru_minflt++;
	} else {
		error = uvm_fault_lower_io(ufi, flt, &uobj, &uobjpage);
		if (error != 0)
			return error;
	}

	/*
	 * locked:
	 * maps(read), amap(if !null), uobj(if !null), uobjpage(if uobj)
	 */
	KASSERT(amap == NULL ||
	    rw_lock_op(amap->am_lock) == flt->upper_lock_type);
	KASSERT(uobj == NULL ||
	    rw_lock_op(uobj->vmobjlock) == flt->lower_lock_type);

	/*
	 * notes:
	 *  - at this point uobjpage can not be NULL
	 *  - at this point uobjpage can not be PG_RELEASED (since we checked
	 *  for it above)
	 *  - at this point uobjpage could be waited on (handle later)
	 *  - uobjpage can be from a different object if tmpfs (vnode vs UAO)
	 */

	KASSERT(uobjpage != NULL);
	KASSERT(uobj == NULL ||
	    uobjpage->uobject->vmobjlock == uobj->vmobjlock);
	KASSERT(uobj == NULL || !UVM_OBJ_IS_CLEAN(uobjpage->uobject) ||
	    uvm_pagegetdirty(uobjpage) == UVM_PAGE_STATUS_CLEAN);

	if (!flt->promote) {
		error = uvm_fault_lower_direct(ufi, flt, uobj, uobjpage);
	} else {
		error = uvm_fault_lower_promote(ufi, flt, uobj, uobjpage);
	}
	return error;
}

/*
 * uvm_fault_lower_lookup: look up on-memory uobj pages.
 *
 *	1. get on-memory pages.
 *	2. if failed, give up (get only center page later).
 *	3. if succeeded, enter h/w mapping of neighbor pages.
 */

static void
uvm_fault_lower_lookup(
	struct uvm_faultinfo *ufi, const struct uvm_faultctx *flt,
	struct vm_page **pages)
{
	struct uvm_object *uobj = ufi->entry->object.uvm_obj;
	int lcv, gotpages;
	vaddr_t currva;
	bool entered;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	rw_enter(uobj->vmobjlock, flt->lower_lock_type);

	/*
	 * Locked: maps(read), amap(if there), uobj
	 */

	cpu_count(CPU_COUNT_FLTLGET, 1);
	gotpages = flt->npages;
	(void) uobj->pgops->pgo_get(uobj,
	    ufi->entry->offset + flt->startva - ufi->entry->start,
	    pages, &gotpages, flt->centeridx,
	    flt->access_type & MASK(ufi->entry), ufi->entry->advice,
	    PGO_LOCKED);

	KASSERT(rw_lock_op(uobj->vmobjlock) == flt->lower_lock_type);

	/*
	 * check for pages to map, if we got any
	 */

	if (gotpages == 0) {
		pages[flt->centeridx] = NULL;
		return;
	}

	entered = false;
	currva = flt->startva;
	for (lcv = 0; lcv < flt->npages; lcv++, currva += PAGE_SIZE) {
		struct vm_page *curpg;

		curpg = pages[lcv];
		if (curpg == NULL || curpg == PGO_DONTCARE) {
			continue;
		}

		/*
		 * in the case of tmpfs, the pages might be from a different
		 * uvm_object.  just make sure that they have the same lock.
		 */

		KASSERT(curpg->uobject->vmobjlock == uobj->vmobjlock);
		KASSERT((curpg->flags & PG_BUSY) == 0);

		/*
		 * leave the centre page for later.  don't screw with
		 * existing mappings (needless & expensive).
		 */

		if (lcv == flt->centeridx) {
			UVMHIST_LOG(maphist, "  got uobjpage (%#jx) "
			    "with locked get", (uintptr_t)curpg, 0, 0, 0);
		} else if (!pmap_extract(ufi->orig_map->pmap, currva, NULL)) {
			uvm_fault_lower_neighbor(ufi, flt, currva, curpg);
			entered = true;
		}
	}
	if (entered) {
		pmap_update(ufi->orig_map->pmap);
	}
}

/*
 * uvm_fault_lower_neighbor: enter h/w mapping of lower neighbor page.
 */

static void
uvm_fault_lower_neighbor(
	struct uvm_faultinfo *ufi, const struct uvm_faultctx *flt,
	vaddr_t currva, struct vm_page *pg)
{
	const bool readonly = uvm_pagereadonly_p(pg) || pg->loan_count > 0;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/* locked: maps(read), amap(if there), uobj */

	/*
	 * calling pgo_get with PGO_LOCKED returns us pages which
	 * are neither busy nor released, so we don't need to check
	 * for this.  we can just directly enter the pages.
	 *
	 * there wasn't a direct fault on the page, so avoid the cost of
	 * activating it.
	 */

	if (!uvmpdpol_pageisqueued_p(pg) && pg->wire_count == 0) {
		uvm_pagelock(pg);
		uvm_pageenqueue(pg);
		uvm_pageunlock(pg);
	}

	UVMHIST_LOG(maphist,
	    "  MAPPING: n obj: pm=%#jx, va=%#jx, pg=%#jx",
	    (uintptr_t)ufi->orig_map->pmap, currva, (uintptr_t)pg, 0);
	cpu_count(CPU_COUNT_FLTNOMAP, 1);

	/*
	 * Since this page isn't the page that's actually faulting,
	 * ignore pmap_enter() failures; it's not critical that we
	 * enter these right now.
	 * NOTE: page can't be waited on or PG_RELEASED because we've
	 * held the lock the whole time we've had the handle.
	 */
	KASSERT((pg->flags & PG_PAGEOUT) == 0);
	KASSERT((pg->flags & PG_RELEASED) == 0);
	KASSERT(!UVM_OBJ_IS_CLEAN(pg->uobject) ||
	    uvm_pagegetdirty(pg) == UVM_PAGE_STATUS_CLEAN);
	KASSERT((pg->flags & PG_BUSY) == 0);
	KASSERT(rw_lock_op(pg->uobject->vmobjlock) == flt->lower_lock_type);

	const vm_prot_t mapprot =
	    readonly ? (flt->enter_prot & ~VM_PROT_WRITE) :
	    flt->enter_prot & MASK(ufi->entry);
	const u_int mapflags =
	    PMAP_CANFAIL | (flt->wire_mapping ? (mapprot | PMAP_WIRED) : 0);
	(void) pmap_enter(ufi->orig_map->pmap, currva,
	    VM_PAGE_TO_PHYS(pg), mapprot, mapflags);
}

/*
 * uvm_fault_lower_io: get lower page from backing store.
 *
 *	1. unlock everything, because i/o will block.
 *	2. call pgo_get.
 *	3. if failed, recover.
 *	4. if succeeded, relock everything and verify things.
 */

static int
uvm_fault_lower_io(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct uvm_object **ruobj, struct vm_page **ruobjpage)
{
	struct vm_amap * const amap = ufi->entry->aref.ar_amap;
	struct uvm_object *uobj = *ruobj;
	struct vm_page *pg;
	bool locked;
	int gotpages;
	int error;
	voff_t uoff;
	vm_prot_t access_type;
	int advice;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/* update rusage counters */
	curlwp->l_ru.ru_majflt++;

	/* grab everything we need from the entry before we unlock */
	uoff = (ufi->orig_rvaddr - ufi->entry->start) + ufi->entry->offset;
	access_type = flt->access_type & MASK(ufi->entry);
	advice = ufi->entry->advice;

	/* Locked: maps(read), amap(if there), uobj */
	KASSERT(rw_lock_op(uobj->vmobjlock) == flt->lower_lock_type);

	/* Upgrade to a write lock if needed. */
	error = uvm_fault_lower_upgrade(ufi, flt, amap, uobj, NULL);
	if (error != 0) {
		return error;
	}
	uvmfault_unlockall(ufi, amap, NULL);

	/* Locked: uobj(write) */
	KASSERT(rw_write_held(uobj->vmobjlock));

	cpu_count(CPU_COUNT_FLTGET, 1);
	gotpages = 1;
	pg = NULL;
	error = uobj->pgops->pgo_get(uobj, uoff, &pg, &gotpages,
	    0, access_type, advice, PGO_SYNCIO);
	/* locked: pg(if no error) */

	/*
	 * recover from I/O
	 */

	if (error) {
		if (error == EAGAIN) {
			UVMHIST_LOG(maphist,
			    "  pgo_get says TRY AGAIN!",0,0,0,0);
			kpause("fltagain2", false, hz/2, NULL);
			return ERESTART;
		}

#if 0
		KASSERT(error != ERESTART);
#else
		/* XXXUEBS don't re-fault? */
		if (error == ERESTART)
			error = EIO;
#endif

		UVMHIST_LOG(maphist, "<- pgo_get failed (code %jd)",
		    error, 0,0,0);
		return error;
	}

	/*
	 * re-verify the state of the world by first trying to relock
	 * the maps.  always relock the object.
	 */

	locked = uvmfault_relock(ufi);
	if (locked && amap)
		amap_lock(amap, flt->upper_lock_type);

	/* might be changed */
	uobj = pg->uobject;

	rw_enter(uobj->vmobjlock, flt->lower_lock_type);
	KASSERT((pg->flags & PG_BUSY) != 0);
	KASSERT(flt->lower_lock_type == RW_WRITER);

	uvm_pagelock(pg);
	uvm_pageactivate(pg);
	uvm_pageunlock(pg);

	/* locked(locked): maps(read), amap(if !null), uobj, pg */
	/* locked(!locked): uobj, pg */

	/*
	 * verify that the page has not be released and re-verify
	 * that amap slot is still free.   if there is a problem,
	 * we unlock and clean up.
	 */

	if ((pg->flags & PG_RELEASED) != 0 ||
	    (locked && amap && amap_lookup(&ufi->entry->aref,
	      ufi->orig_rvaddr - ufi->entry->start))) {
		if (locked)
			uvmfault_unlockall(ufi, amap, NULL);
		locked = false;
	}

	/*
	 * unbusy/release the page.
	 */

	if ((pg->flags & PG_RELEASED) == 0) {
		pg->flags &= ~PG_BUSY;
		uvm_pagelock(pg);
		uvm_pagewakeup(pg);
		uvm_pageunlock(pg);
		UVM_PAGE_OWN(pg, NULL);
	} else {
		cpu_count(CPU_COUNT_FLTPGRELE, 1);
		uvm_pagefree(pg);
	}

	/*
	 * didn't get the lock?   retry.
	 */

	if (locked == false) {
		UVMHIST_LOG(maphist,
		    "  wasn't able to relock after fault: retry",
		    0,0,0,0);
		rw_exit(uobj->vmobjlock);
		return ERESTART;
	}

	/*
	 * we have the data in pg.  we are holding object lock (so the page
	 * can't be released on us).
	 */

	/* locked: maps(read), amap(if !null), uobj */

	*ruobj = uobj;
	*ruobjpage = pg;
	return 0;
}

/*
 * uvm_fault_lower_direct: fault lower center page
 *
 *	1. adjust flt->enter_prot.
 *	2. if page is loaned, resolve.
 */

int
uvm_fault_lower_direct(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct uvm_object *uobj, struct vm_page *uobjpage)
{
	struct vm_page *pg;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/*
	 * we are not promoting.   if the mapping is COW ensure that we
	 * don't give more access than we should (e.g. when doing a read
	 * fault on a COPYONWRITE mapping we want to map the COW page in
	 * R/O even though the entry protection could be R/W).
	 *
	 * set "pg" to the page we want to map in (uobjpage, usually)
	 */

	cpu_count(CPU_COUNT_FLT_OBJ, 1);
	if (UVM_ET_ISCOPYONWRITE(ufi->entry) ||
	    UVM_OBJ_NEEDS_WRITEFAULT(uobjpage->uobject))
		flt->enter_prot &= ~VM_PROT_WRITE;
	pg = uobjpage;		/* map in the actual object */

	KASSERT(uobjpage != PGO_DONTCARE);

	/*
	 * we are faulting directly on the page.   be careful
	 * about writing to loaned pages...
	 */

	if (uobjpage->loan_count) {
		uvm_fault_lower_direct_loan(ufi, flt, uobj, &pg, &uobjpage);
	}
	KASSERT(pg == uobjpage);
	KASSERT((pg->flags & PG_BUSY) == 0);
	return uvm_fault_lower_enter(ufi, flt, uobj, NULL, pg);
}

/*
 * uvm_fault_lower_direct_loan: resolve loaned page.
 *
 *	1. if not cow'ing, adjust flt->enter_prot.
 *	2. if cow'ing, break loan.
 */

static int
uvm_fault_lower_direct_loan(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct uvm_object *uobj, struct vm_page **rpg,
	struct vm_page **ruobjpage)
{
	struct vm_amap * const amap = ufi->entry->aref.ar_amap;
	struct vm_page *pg;
	struct vm_page *uobjpage = *ruobjpage;
	int error;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	if (!flt->cow_now) {
		/* read fault: cap the protection at readonly */
		/* cap! */
		flt->enter_prot = flt->enter_prot & ~VM_PROT_WRITE;
	} else {
		/*
		 * write fault: must break the loan here.  to do this
		 * we need a write lock on the object.
		 */

		error = uvm_fault_lower_upgrade(ufi, flt, amap, uobj, uobjpage);
		if (error != 0) {
			return error;
		}
		KASSERT(rw_write_held(uobj->vmobjlock));

		pg = uvm_loanbreak(uobjpage);
		if (pg == NULL) {

			uvmfault_unlockall(ufi, amap, uobj);
			UVMHIST_LOG(maphist,
			  "  out of RAM breaking loan, waiting",
			  0,0,0,0);
			cpu_count(CPU_COUNT_FLTNORAM, 1);
			uvm_wait("flt_noram4");
			return ERESTART;
		}
		*rpg = pg;
		*ruobjpage = pg;

		/*
		 * drop ownership of page while still holding object lock,
		 * which won't be dropped until the page is entered.
		 */

		uvm_pagelock(pg);
		uvm_pagewakeup(pg);
		uvm_pageunlock(pg);
		pg->flags &= ~PG_BUSY;
		UVM_PAGE_OWN(pg, NULL);
	}
	return 0;
}

/*
 * uvm_fault_lower_promote: promote lower page.
 *
 *	1. call uvmfault_promote.
 *	2. fill in data.
 *	3. if not ZFOD, dispose old page.
 */

int
uvm_fault_lower_promote(
	struct uvm_faultinfo *ufi, struct uvm_faultctx *flt,
	struct uvm_object *uobj, struct vm_page *uobjpage)
{
	struct vm_amap * const amap = ufi->entry->aref.ar_amap;
	struct vm_anon *anon;
	struct vm_page *pg;
	int error;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	KASSERT(amap != NULL);

	/* promoting requires a write lock. */
	error = uvm_fault_upper_upgrade(ufi, flt, amap, uobj);
	if (error != 0) {
		return error;
	}
	KASSERT(rw_write_held(amap->am_lock));
	KASSERT(uobj == NULL ||
	    rw_lock_op(uobj->vmobjlock) == flt->lower_lock_type);

	/*
	 * If we are going to promote the data to an anon we
	 * allocate a blank anon here and plug it into our amap.
	 */
	error = uvmfault_promote(ufi, NULL, uobjpage, &anon, &flt->anon_spare);
	switch (error) {
	case 0:
		break;
	case ERESTART:
		return ERESTART;
	default:
		return error;
	}

	pg = anon->an_page;

	/*
	 * Fill in the data.
	 */

	if (uobjpage != PGO_DONTCARE) {
		cpu_count(CPU_COUNT_FLT_PRCOPY, 1);

		/*
		 * promote to shared amap?  make sure all sharing
		 * procs see it
		 */

		if ((amap_flags(amap) & AMAP_SHARED) != 0) {
			pmap_page_protect(uobjpage, VM_PROT_NONE);
			/*
			 * XXX: PAGE MIGHT BE WIRED!
			 */
		}

		UVMHIST_LOG(maphist,
		    "  promote uobjpage %#jx to anon/page %#jx/%#jx",
		    (uintptr_t)uobjpage, (uintptr_t)anon, (uintptr_t)pg, 0);

	} else {
		cpu_count(CPU_COUNT_FLT_PRZERO, 1);

		/*
		 * Page is zero'd and marked dirty by
		 * uvmfault_promote().
		 */

		UVMHIST_LOG(maphist,"  zero fill anon/page %#jx/%#jx",
		    (uintptr_t)anon, (uintptr_t)pg, 0, 0);
	}

	return uvm_fault_lower_enter(ufi, flt, uobj, anon, pg);
}

/*
 * uvm_fault_lower_enter: enter h/w mapping of lower page or anon page promoted
 * from the lower page.
 */

int
uvm_fault_lower_enter(
	struct uvm_faultinfo *ufi, const struct uvm_faultctx *flt,
	struct uvm_object *uobj,
	struct vm_anon *anon, struct vm_page *pg)
{
	struct vm_amap * const amap = ufi->entry->aref.ar_amap;
	const bool readonly = uvm_pagereadonly_p(pg);
	int error;
	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	/*
	 * Locked:
	 *
	 *	maps(read), amap(if !null), uobj(if !null),
	 *	anon(if !null), pg(if anon), unlock_uobj(if !null)
	 *
	 * anon must be write locked (promotion).  uobj can be either.
	 *
	 * Note: pg is either the uobjpage or the new page in the new anon.
	 */

	KASSERT(amap == NULL ||
	    rw_lock_op(amap->am_lock) == flt->upper_lock_type);
	KASSERT(uobj == NULL ||
	    rw_lock_op(uobj->vmobjlock) == flt->lower_lock_type);
	KASSERT(anon == NULL || anon->an_lock == amap->am_lock);

	/*
	 * note that pg can't be PG_RELEASED or PG_BUSY since we did
	 * not drop the object lock since the last time we checked.
	 */

	KASSERT((pg->flags & PG_RELEASED) == 0);
	KASSERT((pg->flags & PG_BUSY) == 0);

	/*
	 * all resources are present.   we can now map it in and free our
	 * resources.
	 */

	UVMHIST_LOG(maphist,
	    "  MAPPING: case2: pm=%#jx, va=%#jx, pg=%#jx, promote=%jd",
	    (uintptr_t)ufi->orig_map->pmap, ufi->orig_rvaddr,
	    (uintptr_t)pg, flt->promote);
	KASSERTMSG((flt->access_type & VM_PROT_WRITE) == 0 || !readonly,
	    "promote=%u cow_now=%u access_type=%x enter_prot=%x cow=%u "
	    "entry=%p map=%p orig_rvaddr=%p pg=%p",
	    flt->promote, flt->cow_now, flt->access_type, flt->enter_prot,
	    UVM_ET_ISCOPYONWRITE(ufi->entry), ufi->entry, ufi->orig_map,
	    (void *)ufi->orig_rvaddr, pg);
	KASSERT((flt->access_type & VM_PROT_WRITE) == 0 || !readonly);
	if (pmap_enter(ufi->orig_map->pmap, ufi->orig_rvaddr,
	    VM_PAGE_TO_PHYS(pg),
	    readonly ? flt->enter_prot & ~VM_PROT_WRITE : flt->enter_prot,
	    flt->access_type | PMAP_CANFAIL |
	    (flt->wire_mapping ? PMAP_WIRED : 0)) != 0) {

		/*
		 * No need to undo what we did; we can simply think of
		 * this as the pmap throwing away the mapping information.
		 *
		 * We do, however, have to go through the ReFault path,
		 * as the map may change while we're asleep.
		 */

		/*
		 * ensure that the page is queued in the case that
		 * we just promoted the page.
		 */

		if (anon != NULL) {
			uvm_pagelock(pg);
			uvm_pageenqueue(pg);
			uvm_pagewakeup(pg);
			uvm_pageunlock(pg);
		}

		uvmfault_unlockall(ufi, amap, uobj);
		if (!uvm_reclaimable()) {
			UVMHIST_LOG(maphist,
			    "<- failed.  out of VM",0,0,0,0);
			/* XXX instrumentation */
			error = ENOMEM;
			return error;
		}
		/* XXX instrumentation */
		uvm_wait("flt_pmfail2");
		return ERESTART;
	}

	uvm_fault_lower_done(ufi, flt, uobj, pg);
	pmap_update(ufi->orig_map->pmap);
	uvmfault_unlockall(ufi, amap, uobj);

	UVMHIST_LOG(maphist, "<- done (SUCCESS!)",0,0,0,0);
	return 0;
}

/*
 * uvm_fault_lower_done: queue lower center page.
 */

void
uvm_fault_lower_done(
	struct uvm_faultinfo *ufi, const struct uvm_faultctx *flt,
	struct uvm_object *uobj, struct vm_page *pg)
{

	UVMHIST_FUNC(__func__); UVMHIST_CALLED(maphist);

	if (flt->wire_paging) {
		uvm_pagelock(pg);
		uvm_pagewire(pg);
		uvm_pageunlock(pg);
		if (pg->flags & PG_AOBJ) {

			/*
			 * since the now-wired page cannot be paged out,
			 * release its swap resources for others to use.
			 * since an aobj page with no swap cannot be clean,
			 * mark it dirty now.
			 *
			 * use pg->uobject here.  if the page is from a
			 * tmpfs vnode, the pages are backed by its UAO and
			 * not the vnode.
			 */

			KASSERT(uobj != NULL);
			KASSERT(uobj->vmobjlock == pg->uobject->vmobjlock);
			uvm_pagemarkdirty(pg, UVM_PAGE_STATUS_DIRTY);
			uao_dropswap(pg->uobject, pg->offset >> PAGE_SHIFT);
		}
	} else if (uvmpdpol_pageactivate_p(pg)) {
		/*
		 * avoid re-activating the page unless needed,
		 * to avoid false sharing on multiprocessor.
		 */

		uvm_pagelock(pg);
		uvm_pageactivate(pg);
		uvm_pageunlock(pg);
	}
}


/*
 * uvm_fault_wire: wire down a range of virtual addresses in a map.
 *
 * => map may be read-locked by caller, but MUST NOT be write-locked.
 * => if map is read-locked, any operations which may cause map to
 *	be write-locked in uvm_fault() must be taken care of by
 *	the caller.  See uvm_map_pageable().
 */

int
uvm_fault_wire(struct vm_map *map, vaddr_t start, vaddr_t end,
    vm_prot_t access_type, int maxprot)
{
	vaddr_t va;
	int error;

	/*
	 * now fault it in a page at a time.   if the fault fails then we have
	 * to undo what we have done.   note that in uvm_fault VM_PROT_NONE
	 * is replaced with the max protection if fault_type is VM_FAULT_WIRE.
	 */

	/*
	 * XXX work around overflowing a vaddr_t.  this prevents us from
	 * wiring the last page in the address space, though.
	 */
	if (start > end) {
		return EFAULT;
	}

	for (va = start; va < end; va += PAGE_SIZE) {
		error = uvm_fault_internal(map, va, access_type,
		    (maxprot ? UVM_FAULT_MAXPROT : 0) | UVM_FAULT_WIRE);
		if (error) {
			if (va != start) {
				uvm_fault_unwire(map, start, va);
			}
			return error;
		}
	}
	return 0;
}

/*
 * uvm_fault_unwire(): unwire range of virtual space.
 */

void
uvm_fault_unwire(struct vm_map *map, vaddr_t start, vaddr_t end)
{
	vm_map_lock_read(map);
	uvm_fault_unwire_locked(map, start, end);
	vm_map_unlock_read(map);
}

/*
 * uvm_fault_unwire_locked(): the guts of uvm_fault_unwire().
 *
 * => map must be at least read-locked.
 */

void
uvm_fault_unwire_locked(struct vm_map *map, vaddr_t start, vaddr_t end)
{
	struct vm_map_entry *entry, *oentry;
	pmap_t pmap = vm_map_pmap(map);
	vaddr_t va;
	paddr_t pa;
	struct vm_page *pg;

	/*
	 * we assume that the area we are unwiring has actually been wired
	 * in the first place.   this means that we should be able to extract
	 * the PAs from the pmap.   we also lock out the page daemon so that
	 * we can call uvm_pageunwire.
	 */

	/*
	 * find the beginning map entry for the region.
	 */

	KASSERT(start >= vm_map_min(map) && end <= vm_map_max(map));
	if (uvm_map_lookup_entry(map, start, &entry) == false)
		panic("uvm_fault_unwire_locked: address not in map");

	oentry = NULL;
	for (va = start; va < end; va += PAGE_SIZE) {

		/*
		 * find the map entry for the current address.
		 */

		KASSERT(va >= entry->start);
		while (va >= entry->end) {
			KASSERT(entry->next != &map->header &&
				entry->next->start <= entry->end);
			entry = entry->next;
		}

		/*
		 * lock it.
		 */

		if (entry != oentry) {
			if (oentry != NULL) {
				uvm_map_unlock_entry(oentry);
			}
			uvm_map_lock_entry(entry, RW_WRITER);
			oentry = entry;
		}

		/*
		 * if the entry is no longer wired, tell the pmap.
		 */

		if (!pmap_extract(pmap, va, &pa))
			continue;

		if (VM_MAPENT_ISWIRED(entry) == 0)
			pmap_unwire(pmap, va);

		pg = PHYS_TO_VM_PAGE(pa);
		if (pg) {
			uvm_pagelock(pg);
			uvm_pageunwire(pg);
			uvm_pageunlock(pg);
		}
	}

	if (oentry != NULL) {
		uvm_map_unlock_entry(entry);
	}
}
