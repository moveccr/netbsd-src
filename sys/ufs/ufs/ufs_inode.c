/*	$NetBSD: ufs_inode.c,v 1.112 2020/09/05 16:30:13 riastradh Exp $	*/

/*
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)ufs_inode.c	8.9 (Berkeley) 5/14/95
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: ufs_inode.c,v 1.112 2020/09/05 16:30:13 riastradh Exp $");

#if defined(_KERNEL_OPT)
#include "opt_ffs.h"
#include "opt_quota.h"
#include "opt_wapbl.h"
#include "opt_uvmhist.h"
#endif

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/mount.h>
#include <sys/kernel.h>
#include <sys/namei.h>
#include <sys/kauth.h>
#include <sys/wapbl.h>
#include <sys/kmem.h>

#include <ufs/ufs/inode.h>
#include <ufs/ufs/ufsmount.h>
#include <ufs/ufs/ufs_extern.h>
#include <ufs/ufs/ufs_wapbl.h>
#ifdef UFS_DIRHASH
#include <ufs/ufs/dirhash.h>
#endif
#ifdef UFS_EXTATTR
#include <ufs/ufs/extattr.h>
#endif

#ifdef UVMHIST
#include <uvm/uvm.h>
#endif
#include <uvm/uvm_page.h>
#include <uvm/uvm_stat.h>

/*
 * Last reference to an inode.  If necessary, write or delete it.
 */
int
ufs_inactive(void *v)
{
	struct vop_inactive_v2_args /* {
		struct vnode *a_vp;
		struct bool *a_recycle;
	} */ *ap = v;
	struct vnode *vp = ap->a_vp;
	struct inode *ip = VTOI(vp);
	struct mount *mp = vp->v_mount;
	mode_t mode;
	int allerror = 0, error;
	bool wapbl_locked = false;

	UFS_WAPBL_JUNLOCK_ASSERT(mp);

	/*
	 * Ignore inodes related to stale file handles.
	 */
	if (ip->i_mode == 0)
		goto out;

	if (ip->i_nlink <= 0 && (mp->mnt_flag & MNT_RDONLY) == 0) {
#ifdef UFS_EXTATTR
		ufs_extattr_vnode_inactive(vp, curlwp);
#endif
		/*
		 * All file blocks must be freed before we can let the vnode
		 * be reclaimed, so can't postpone full truncating any further.
		 */
		ufs_truncate_all(vp);

#if defined(QUOTA) || defined(QUOTA2)
		error = UFS_WAPBL_BEGIN(mp);
		if (error) {
			allerror = error;
		} else {
			wapbl_locked = true;
			(void)chkiq(ip, -1, NOCRED, 0);
		}
#endif
		DIP_ASSIGN(ip, rdev, 0);
		mode = ip->i_mode;
		ip->i_mode = 0;
		ip->i_omode = mode;
		DIP_ASSIGN(ip, mode, 0);
		ip->i_flag |= IN_CHANGE | IN_UPDATE;
		/*
		 * Defer final inode free and update to ufs_reclaim().
		 */
	}

	if (ip->i_flag & (IN_CHANGE | IN_UPDATE | IN_MODIFIED)) {
		if (! wapbl_locked) {
			error = UFS_WAPBL_BEGIN(mp);
			if (error) {
				allerror = error;
				goto out;
			}
			wapbl_locked = true;
		}
		UFS_UPDATE(vp, NULL, NULL, 0);
	}
out:
	if (wapbl_locked)
		UFS_WAPBL_END(mp);
	/*
	 * If we are done with the inode, reclaim it
	 * so that it can be reused immediately.
	 */
	*ap->a_recycle = (ip->i_mode == 0);

	if (ip->i_mode == 0 && (DIP(ip, size) != 0 || DIP(ip, blocks) != 0)) {
		printf("%s: unlinked ino %" PRId64 " on \"%s\" has"
		    " non zero size %" PRIx64 " or blocks %" PRIx64
		    " with allerror %d\n",
		    __func__, ip->i_number, mp->mnt_stat.f_mntonname,
		    DIP(ip, size), DIP(ip, blocks), allerror);
		panic("%s: dirty filesystem?", __func__);
	}

	return (allerror);
}

/*
 * Reclaim an inode so that it can be used for other purposes.
 */
int
ufs_reclaim(struct vnode *vp)
{
	struct inode *ip = VTOI(vp);

	if (!UFS_WAPBL_BEGIN(vp->v_mount)) {
		UFS_UPDATE(vp, NULL, NULL, UPDATE_CLOSE);
		UFS_WAPBL_END(vp->v_mount);
	}
	UFS_UPDATE(vp, NULL, NULL, UPDATE_CLOSE);

	if (ip->i_devvp) {
		vrele(ip->i_devvp);
		ip->i_devvp = 0;
	}
#if defined(QUOTA) || defined(QUOTA2)
	ufsquota_free(ip);
#endif
#ifdef UFS_DIRHASH
	if (ip->i_dirhash != NULL)
		ufsdirhash_free(ip);
#endif
	return (0);
}

/*
 * allocate a range of blocks in a file.
 * after this function returns, any page entirely contained within the range
 * will map to invalid data and thus must be overwritten before it is made
 * accessible to others.
 */

int
ufs_balloc_range(struct vnode *vp, off_t off, off_t len, kauth_cred_t cred,
    int flags)
{
	off_t neweof;	/* file size after the operation */
	off_t neweob;	/* offset next to the last block after the operation */
	off_t pagestart; /* starting offset of range covered by pgs */
	off_t eob;	/* offset next to allocated blocks */
	struct uvm_object *uobj;
	int i, delta, error, npages;
	int bshift = vp->v_mount->mnt_fs_bshift;
	int bsize = 1 << bshift;
	int ppb = MAX(bsize >> PAGE_SHIFT, 1);
	struct vm_page **pgs;
	size_t pgssize;
	UVMHIST_FUNC("ufs_balloc_range"); UVMHIST_CALLED(ubchist);
	UVMHIST_LOG(ubchist, "vp %#jx off 0x%jx len 0x%jx u_size 0x%jx",
		    (uintptr_t)vp, off, len, vp->v_size);

	neweof = MAX(vp->v_size, off + len);
	GOP_SIZE(vp, neweof, &neweob, 0);

	error = 0;
	uobj = &vp->v_uobj;

	/*
	 * read or create pages covering the range of the allocation and
	 * keep them locked until the new block is allocated, so there
	 * will be no window where the old contents of the new block are
	 * visible to racing threads.
	 */

	pagestart = trunc_page(off) & ~(bsize - 1);
	npages = MIN(ppb, (round_page(neweob) - pagestart) >> PAGE_SHIFT);
	pgssize = npages * sizeof(struct vm_page *);
	pgs = kmem_zalloc(pgssize, KM_SLEEP);

	/*
	 * adjust off to be block-aligned.
	 */

	delta = off & (bsize - 1);
	off -= delta;
	len += delta;

	genfs_node_wrlock(vp);
	rw_enter(uobj->vmobjlock, RW_WRITER);
	error = VOP_GETPAGES(vp, pagestart, pgs, &npages, 0,
	    VM_PROT_WRITE, 0, PGO_SYNCIO | PGO_PASTEOF | PGO_NOBLOCKALLOC |
	    PGO_NOTIMESTAMP | PGO_GLOCKHELD);
	if (error) {
		genfs_node_unlock(vp);
		goto out;
	}

	/*
	 * now allocate the range.
	 */

	error = GOP_ALLOC(vp, off, len, flags, cred);
	genfs_node_unlock(vp);

	/*
	 * if the allocation succeeded, mark all the pages dirty
	 * and clear PG_RDONLY on any pages that are now fully backed
	 * by disk blocks.  if the allocation failed, we do not invalidate
	 * the pages since they might have already existed and been dirty,
	 * in which case we need to keep them around.  if we created the pages,
	 * they will be clean and read-only, and leaving such pages
	 * in the cache won't cause any problems.
	 */

	GOP_SIZE(vp, off + len, &eob, 0);
	rw_enter(uobj->vmobjlock, RW_WRITER);
	for (i = 0; i < npages; i++) {
		KASSERT((pgs[i]->flags & PG_RELEASED) == 0);
		if (!error) {
			if (off <= pagestart + (i << PAGE_SHIFT) &&
			    pagestart + ((i + 1) << PAGE_SHIFT) <= eob) {
				pgs[i]->flags &= ~PG_RDONLY;
			}
			uvm_pagemarkdirty(pgs[i], UVM_PAGE_STATUS_DIRTY);
		}
		uvm_pagelock(pgs[i]);
		uvm_pageactivate(pgs[i]);
		uvm_pageunlock(pgs[i]);
	}
	uvm_page_unbusy(pgs, npages);
	rw_exit(uobj->vmobjlock);

 out:
 	kmem_free(pgs, pgssize);
	return error;
}

int
ufs_truncate_retry(struct vnode *vp, int ioflag, uint64_t newsize,
    kauth_cred_t cred)
{
	struct inode *ip = VTOI(vp);
	struct mount *mp = vp->v_mount;
	int error = 0;

	UFS_WAPBL_JUNLOCK_ASSERT(mp);

	/*
	 * Truncate might temporarily fail, loop until done.
	 */
	do {
		error = UFS_WAPBL_BEGIN(mp);
		if (error)
			goto out;

		error = UFS_TRUNCATE(vp, newsize, ioflag, cred);
		UFS_WAPBL_END(mp);

		if (error != 0 && error != EAGAIN)
			goto out;
	} while (ip->i_size != newsize);

  out:
	return error;
}

/* truncate all the data of the inode including extended attributes */
int
ufs_truncate_all(struct vnode *vp)
{
	struct inode *ip = VTOI(vp);
	off_t isize = ip->i_size;

	if (ip->i_ump->um_fstype == UFS2)
		isize += ip->i_ffs2_extsize;

	if (isize == 0)
		return 0;
	return ufs_truncate_retry(vp, IO_NORMAL | IO_EXT, 0, NOCRED);
}
