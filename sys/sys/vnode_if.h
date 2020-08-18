/*	$NetBSD: vnode_if.h,v 1.105 2020/05/16 18:31:53 christos Exp $	*/

/*
 * Warning: DO NOT EDIT! This file is automatically generated!
 * (Modifications made here may easily be lost!)
 *
 * Created from the file:
 *	NetBSD: vnode_if.src,v 1.78 2019/10/11 08:04:52 hannken Exp
 * by the script:
 *	NetBSD: vnode_if.sh,v 1.69 2020/02/23 22:14:04 ad Exp
 */

/*
 * Copyright (c) 1992, 1993, 1994, 1995
 *	The Regents of the University of California.  All rights reserved.
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
 */

#ifndef _SYS_VNODE_IF_H_
#define _SYS_VNODE_IF_H_

extern const struct vnodeop_desc vop_default_desc;

struct buf;

#ifndef _KERNEL
#include <stdbool.h>
#endif

#define VOP_BWRITE_DESCOFFSET 1
struct vop_bwrite_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct buf *a_bp;
};
extern const struct vnodeop_desc vop_bwrite_desc;
int VOP_BWRITE(struct vnode *, struct buf *);

#define VOP_LOOKUP_DESCOFFSET 2
struct vop_lookup_v2_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
};
extern const struct vnodeop_desc vop_lookup_desc;
int VOP_LOOKUP(struct vnode *, struct vnode **, struct componentname *);

#define VOP_CREATE_DESCOFFSET 3
struct vop_create_v3_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
};
extern const struct vnodeop_desc vop_create_desc;
int VOP_CREATE(struct vnode *, struct vnode **, struct componentname *, 
    struct vattr *);

#define VOP_MKNOD_DESCOFFSET 4
struct vop_mknod_v3_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
};
extern const struct vnodeop_desc vop_mknod_desc;
int VOP_MKNOD(struct vnode *, struct vnode **, struct componentname *, 
    struct vattr *);

#define VOP_OPEN_DESCOFFSET 5
struct vop_open_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_mode;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_open_desc;
int VOP_OPEN(struct vnode *, int, kauth_cred_t);

#define VOP_CLOSE_DESCOFFSET 6
struct vop_close_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_fflag;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_close_desc;
int VOP_CLOSE(struct vnode *, int, kauth_cred_t);

#define VOP_ACCESS_DESCOFFSET 7
struct vop_access_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	accmode_t a_accmode;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_access_desc;
int VOP_ACCESS(struct vnode *, accmode_t, kauth_cred_t);

#define VOP_ACCESSX_DESCOFFSET 8
struct vop_accessx_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	accmode_t a_accmode;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_accessx_desc;
int VOP_ACCESSX(struct vnode *, accmode_t, kauth_cred_t);

#define VOP_GETATTR_DESCOFFSET 9
struct vop_getattr_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct vattr *a_vap;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_getattr_desc;
int VOP_GETATTR(struct vnode *, struct vattr *, kauth_cred_t);

#define VOP_SETATTR_DESCOFFSET 10
struct vop_setattr_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct vattr *a_vap;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_setattr_desc;
int VOP_SETATTR(struct vnode *, struct vattr *, kauth_cred_t);

#define VOP_READ_DESCOFFSET 11
struct vop_read_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct uio *a_uio;
	int a_ioflag;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_read_desc;
int VOP_READ(struct vnode *, struct uio *, int, kauth_cred_t);

#define VOP_WRITE_DESCOFFSET 12
struct vop_write_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct uio *a_uio;
	int a_ioflag;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_write_desc;
int VOP_WRITE(struct vnode *, struct uio *, int, kauth_cred_t);

#define VOP_FALLOCATE_DESCOFFSET 13
struct vop_fallocate_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	off_t a_pos;
	off_t a_len;
};
extern const struct vnodeop_desc vop_fallocate_desc;
int VOP_FALLOCATE(struct vnode *, off_t, off_t);

#define VOP_FDISCARD_DESCOFFSET 14
struct vop_fdiscard_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	off_t a_pos;
	off_t a_len;
};
extern const struct vnodeop_desc vop_fdiscard_desc;
int VOP_FDISCARD(struct vnode *, off_t, off_t);

#define VOP_IOCTL_DESCOFFSET 15
struct vop_ioctl_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	u_long a_command;
	void *a_data;
	int a_fflag;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_ioctl_desc;
int VOP_IOCTL(struct vnode *, u_long, void *, int, kauth_cred_t);

#define VOP_FCNTL_DESCOFFSET 16
struct vop_fcntl_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	u_int a_command;
	void *a_data;
	int a_fflag;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_fcntl_desc;
int VOP_FCNTL(struct vnode *, u_int, void *, int, kauth_cred_t);

#define VOP_POLL_DESCOFFSET 17
struct vop_poll_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_events;
};
extern const struct vnodeop_desc vop_poll_desc;
int VOP_POLL(struct vnode *, int);

#define VOP_KQFILTER_DESCOFFSET 18
struct vop_kqfilter_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct knote *a_kn;
};
extern const struct vnodeop_desc vop_kqfilter_desc;
int VOP_KQFILTER(struct vnode *, struct knote *);

#define VOP_REVOKE_DESCOFFSET 19
struct vop_revoke_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_flags;
};
extern const struct vnodeop_desc vop_revoke_desc;
int VOP_REVOKE(struct vnode *, int);

#define VOP_MMAP_DESCOFFSET 20
struct vop_mmap_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	vm_prot_t a_prot;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_mmap_desc;
int VOP_MMAP(struct vnode *, vm_prot_t, kauth_cred_t);

#define VOP_FSYNC_DESCOFFSET 21
struct vop_fsync_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	kauth_cred_t a_cred;
	int a_flags;
	off_t a_offlo;
	off_t a_offhi;
};
extern const struct vnodeop_desc vop_fsync_desc;
int VOP_FSYNC(struct vnode *, kauth_cred_t, int, off_t, off_t);

#define VOP_SEEK_DESCOFFSET 22
struct vop_seek_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	off_t a_oldoff;
	off_t a_newoff;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_seek_desc;
int VOP_SEEK(struct vnode *, off_t, off_t, kauth_cred_t);

#define VOP_REMOVE_DESCOFFSET 23
struct vop_remove_v2_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode *a_vp;
	struct componentname *a_cnp;
};
extern const struct vnodeop_desc vop_remove_desc;
int VOP_REMOVE(struct vnode *, struct vnode *, struct componentname *);

#define VOP_LINK_DESCOFFSET 24
struct vop_link_v2_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode *a_vp;
	struct componentname *a_cnp;
};
extern const struct vnodeop_desc vop_link_desc;
int VOP_LINK(struct vnode *, struct vnode *, struct componentname *);

#define VOP_RENAME_DESCOFFSET 25
struct vop_rename_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_fdvp;
	struct vnode *a_fvp;
	struct componentname *a_fcnp;
	struct vnode *a_tdvp;
	struct vnode *a_tvp;
	struct componentname *a_tcnp;
};
extern const struct vnodeop_desc vop_rename_desc;
int VOP_RENAME(struct vnode *, struct vnode *, struct componentname *, 
    struct vnode *, struct vnode *, struct componentname *);

#define VOP_MKDIR_DESCOFFSET 26
struct vop_mkdir_v3_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
};
extern const struct vnodeop_desc vop_mkdir_desc;
int VOP_MKDIR(struct vnode *, struct vnode **, struct componentname *, 
    struct vattr *);

#define VOP_RMDIR_DESCOFFSET 27
struct vop_rmdir_v2_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode *a_vp;
	struct componentname *a_cnp;
};
extern const struct vnodeop_desc vop_rmdir_desc;
int VOP_RMDIR(struct vnode *, struct vnode *, struct componentname *);

#define VOP_SYMLINK_DESCOFFSET 28
struct vop_symlink_v3_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct vnode **a_vpp;
	struct componentname *a_cnp;
	struct vattr *a_vap;
	char *a_target;
};
extern const struct vnodeop_desc vop_symlink_desc;
int VOP_SYMLINK(struct vnode *, struct vnode **, struct componentname *, 
    struct vattr *, char *);

#define VOP_READDIR_DESCOFFSET 29
struct vop_readdir_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct uio *a_uio;
	kauth_cred_t a_cred;
	int *a_eofflag;
	off_t **a_cookies;
	int *a_ncookies;
};
extern const struct vnodeop_desc vop_readdir_desc;
int VOP_READDIR(struct vnode *, struct uio *, kauth_cred_t, int *, off_t **, 
    int *);

#define VOP_READLINK_DESCOFFSET 30
struct vop_readlink_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct uio *a_uio;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_readlink_desc;
int VOP_READLINK(struct vnode *, struct uio *, kauth_cred_t);

#define VOP_ABORTOP_DESCOFFSET 31
struct vop_abortop_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct componentname *a_cnp;
};
extern const struct vnodeop_desc vop_abortop_desc;
int VOP_ABORTOP(struct vnode *, struct componentname *);

#define VOP_INACTIVE_DESCOFFSET 32
struct vop_inactive_v2_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	bool *a_recycle;
};
extern const struct vnodeop_desc vop_inactive_desc;
int VOP_INACTIVE(struct vnode *, bool *);

#define VOP_RECLAIM_DESCOFFSET 33
struct vop_reclaim_v2_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
};
extern const struct vnodeop_desc vop_reclaim_desc;
int VOP_RECLAIM(struct vnode *);

#define VOP_LOCK_DESCOFFSET 34
struct vop_lock_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_flags;
};
extern const struct vnodeop_desc vop_lock_desc;
int VOP_LOCK(struct vnode *, int);

#define VOP_UNLOCK_DESCOFFSET 35
struct vop_unlock_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
};
extern const struct vnodeop_desc vop_unlock_desc;
int VOP_UNLOCK(struct vnode *);

#define VOP_BMAP_DESCOFFSET 36
struct vop_bmap_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	daddr_t a_bn;
	struct vnode **a_vpp;
	daddr_t *a_bnp;
	int *a_runp;
};
extern const struct vnodeop_desc vop_bmap_desc;
int VOP_BMAP(struct vnode *, daddr_t, struct vnode **, daddr_t *, int *);

#define VOP_STRATEGY_DESCOFFSET 37
struct vop_strategy_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	struct buf *a_bp;
};
extern const struct vnodeop_desc vop_strategy_desc;
int VOP_STRATEGY(struct vnode *, struct buf *);

#define VOP_PRINT_DESCOFFSET 38
struct vop_print_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
};
extern const struct vnodeop_desc vop_print_desc;
int VOP_PRINT(struct vnode *);

#define VOP_ISLOCKED_DESCOFFSET 39
struct vop_islocked_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
};
extern const struct vnodeop_desc vop_islocked_desc;
int VOP_ISLOCKED(struct vnode *);

#define VOP_PATHCONF_DESCOFFSET 40
struct vop_pathconf_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_name;
	register_t *a_retval;
};
extern const struct vnodeop_desc vop_pathconf_desc;
int VOP_PATHCONF(struct vnode *, int, register_t *);

#define VOP_ADVLOCK_DESCOFFSET 41
struct vop_advlock_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	void *a_id;
	int a_op;
	struct flock *a_fl;
	int a_flags;
};
extern const struct vnodeop_desc vop_advlock_desc;
int VOP_ADVLOCK(struct vnode *, void *, int, struct flock *, int);

#define VOP_WHITEOUT_DESCOFFSET 42
struct vop_whiteout_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_dvp;
	struct componentname *a_cnp;
	int a_flags;
};
extern const struct vnodeop_desc vop_whiteout_desc;
int VOP_WHITEOUT(struct vnode *, struct componentname *, int);

#define VOP_GETPAGES_DESCOFFSET 43
struct vop_getpages_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	voff_t a_offset;
	struct vm_page **a_m;
	int *a_count;
	int a_centeridx;
	vm_prot_t a_access_type;
	int a_advice;
	int a_flags;
};
extern const struct vnodeop_desc vop_getpages_desc;
int VOP_GETPAGES(struct vnode *, voff_t, struct vm_page **, int *, int, 
    vm_prot_t, int, int);

#define VOP_PUTPAGES_DESCOFFSET 44
struct vop_putpages_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	voff_t a_offlo;
	voff_t a_offhi;
	int a_flags;
};
extern const struct vnodeop_desc vop_putpages_desc;
int VOP_PUTPAGES(struct vnode *, voff_t, voff_t, int);

#define VOP_GETACL_DESCOFFSET 45
struct vop_getacl_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	acl_type_t a_type;
	struct acl *a_aclp;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_getacl_desc;
int VOP_GETACL(struct vnode *, acl_type_t, struct acl *, kauth_cred_t);

#define VOP_SETACL_DESCOFFSET 46
struct vop_setacl_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	acl_type_t a_type;
	struct acl *a_aclp;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_setacl_desc;
int VOP_SETACL(struct vnode *, acl_type_t, struct acl *, kauth_cred_t);

#define VOP_ACLCHECK_DESCOFFSET 47
struct vop_aclcheck_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	acl_type_t a_type;
	struct acl *a_aclp;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_aclcheck_desc;
int VOP_ACLCHECK(struct vnode *, acl_type_t, struct acl *, kauth_cred_t);

#define VOP_CLOSEEXTATTR_DESCOFFSET 48
struct vop_closeextattr_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_commit;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_closeextattr_desc;
int VOP_CLOSEEXTATTR(struct vnode *, int, kauth_cred_t);

#define VOP_GETEXTATTR_DESCOFFSET 49
struct vop_getextattr_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_attrnamespace;
	const char *a_name;
	struct uio *a_uio;
	size_t *a_size;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_getextattr_desc;
int VOP_GETEXTATTR(struct vnode *, int, const char *, struct uio *, 
    size_t *, kauth_cred_t);

#define VOP_LISTEXTATTR_DESCOFFSET 50
struct vop_listextattr_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_attrnamespace;
	struct uio *a_uio;
	size_t *a_size;
	int a_flag;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_listextattr_desc;
int VOP_LISTEXTATTR(struct vnode *, int, struct uio *, size_t *, int, 
    kauth_cred_t);

#define VOP_OPENEXTATTR_DESCOFFSET 51
struct vop_openextattr_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_openextattr_desc;
int VOP_OPENEXTATTR(struct vnode *, kauth_cred_t);

#define VOP_DELETEEXTATTR_DESCOFFSET 52
struct vop_deleteextattr_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_attrnamespace;
	const char *a_name;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_deleteextattr_desc;
int VOP_DELETEEXTATTR(struct vnode *, int, const char *, kauth_cred_t);

#define VOP_SETEXTATTR_DESCOFFSET 53
struct vop_setextattr_args {
	const struct vnodeop_desc *a_desc;
	struct vnode *a_vp;
	int a_attrnamespace;
	const char *a_name;
	struct uio *a_uio;
	kauth_cred_t a_cred;
};
extern const struct vnodeop_desc vop_setextattr_desc;
int VOP_SETEXTATTR(struct vnode *, int, const char *, struct uio *, 
    kauth_cred_t);

#define VNODE_OPS_COUNT	54

#endif /* !_SYS_VNODE_IF_H_ */
