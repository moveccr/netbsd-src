/* $NetBSD: ultrix_sysent.c,v 1.70 2017/05/10 06:19:49 riastradh Exp $ */

/*
 * System call switch table.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.52 2013/11/07 19:37:19 njoly Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: ultrix_sysent.c,v 1.70 2017/05/10 06:19:49 riastradh Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/sched.h>
#include <sys/syscallargs.h>
#include <compat/ultrix/ultrix_syscallargs.h>

#define	s(type)	sizeof(type)
#define	n(type)	(sizeof(type)/sizeof (register_t))
#define	ns(type)	.sy_narg = n(type), .sy_argsize = s(type)

struct sysent ultrix_sysent[] = {
	{
		.sy_call = (sy_call_t *)sys_nosys
	},		/* 0 = syscall */
	{
		ns(struct sys_exit_args),
		.sy_call = (sy_call_t *)sys_exit
	},		/* 1 = exit */
	{
		.sy_call = (sy_call_t *)sys_fork
	},		/* 2 = fork */
	{
		ns(struct sys_read_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_read
	},		/* 3 = read */
	{
		ns(struct sys_write_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_write
	},		/* 4 = write */
	{
		ns(struct ultrix_sys_open_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_open
	},		/* 5 = open */
	{
		ns(struct sys_close_args),
		.sy_call = (sy_call_t *)sys_close
	},		/* 6 = close */
	{
		.sy_call = (sy_call_t *)compat_43_sys_wait
	},		/* 7 = owait */
	{
		ns(struct ultrix_sys_creat_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_creat
	},		/* 8 = creat */
	{
		ns(struct sys_link_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_link
	},		/* 9 = link */
	{
		ns(struct sys_unlink_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_unlink
	},		/* 10 = unlink */
	{
		ns(struct ultrix_sys_execv_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_execv
	},		/* 11 = execv */
	{
		ns(struct sys_chdir_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_chdir
	},		/* 12 = chdir */
	{
		.sy_call = sys_nosys,
	},		/* 13 = filler */
	{
		ns(struct ultrix_sys_mknod_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_mknod
	},		/* 14 = mknod */
	{
		ns(struct sys_chmod_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_chmod
	},		/* 15 = chmod */
	{
		ns(struct sys___posix_chown_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys___posix_chown
	},		/* 16 = __posix_chown */
	{
		ns(struct sys_obreak_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_obreak
	},		/* 17 = break */
	{
		.sy_call = sys_nosys,
	},		/* 18 = filler */
	{
		ns(struct compat_43_sys_lseek_args),
		.sy_call = (sy_call_t *)compat_43_sys_lseek
	},		/* 19 = lseek */
	{
		.sy_call = (sy_call_t *)sys_getpid
	},		/* 20 = getpid */
	{
		ns(struct ultrix_sys_mount_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_mount
	},		/* 21 = mount */
	{
		.sy_call = sys_nosys,
	},		/* 22 = filler */
	{
		ns(struct sys_setuid_args),
		.sy_call = (sy_call_t *)sys_setuid
	},		/* 23 = setuid */
	{
		.sy_call = (sy_call_t *)sys_getuid
	},		/* 24 = getuid */
	{
		.sy_call = sys_nosys,
	},		/* 25 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 26 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 27 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 28 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 29 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 30 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 31 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 32 = filler */
	{
		ns(struct ultrix_sys_access_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_access
	},		/* 33 = access */
	{
		.sy_call = sys_nosys,
	},		/* 34 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 35 = filler */
	{
		.sy_call = (sy_call_t *)sys_sync
	},		/* 36 = sync */
	{
		ns(struct sys_kill_args),
		.sy_call = (sy_call_t *)sys_kill
	},		/* 37 = kill */
	{
		ns(struct ultrix_sys_stat_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_stat
	},		/* 38 = stat43 */
	{
		.sy_call = sys_nosys,
	},		/* 39 = filler */
	{
		ns(struct ultrix_sys_lstat_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_lstat
	},		/* 40 = lstat43 */
	{
		ns(struct sys_dup_args),
		.sy_call = (sy_call_t *)sys_dup
	},		/* 41 = dup */
	{
		.sy_call = (sy_call_t *)sys_pipe
	},		/* 42 = pipe */
	{
		.sy_call = sys_nosys,
	},		/* 43 = filler */
	{
		ns(struct sys_profil_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_profil
	},		/* 44 = profil */
	{
		.sy_call = sys_nosys,
	},		/* 45 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 46 = filler */
	{
		.sy_call = (sy_call_t *)sys_getgid
	},		/* 47 = getgid */
	{
		.sy_call = sys_nosys,
	},		/* 48 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 49 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 50 = filler */
	{
		ns(struct sys_acct_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_acct
	},		/* 51 = acct */
	{
		.sy_call = sys_nosys,
	},		/* 52 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 53 = filler */
	{
		ns(struct ultrix_sys_ioctl_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_ioctl
	},		/* 54 = ioctl */
	{
		ns(struct sys_reboot_args),
		.sy_call = (sy_call_t *)sys_reboot
	},		/* 55 = reboot */
	{
		.sy_call = sys_nosys,
	},		/* 56 = filler */
	{
		ns(struct sys_symlink_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_symlink
	},		/* 57 = symlink */
	{
		ns(struct sys_readlink_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_readlink
	},		/* 58 = readlink */
	{
		ns(struct ultrix_sys_execve_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_execve
	},		/* 59 = execve */
	{
		ns(struct sys_umask_args),
		.sy_call = (sy_call_t *)sys_umask
	},		/* 60 = umask */
	{
		ns(struct sys_chroot_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_chroot
	},		/* 61 = chroot */
	{
		ns(struct compat_43_sys_fstat_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_fstat
	},		/* 62 = fstat */
	{
		.sy_call = sys_nosys,
	},		/* 63 = filler */
	{
		.sy_call = (sy_call_t *)compat_43_sys_getpagesize
	},		/* 64 = getpagesize */
	{
		.sy_call = sys_nosys,
	},		/* 65 = filler */
	{
		.sy_call = (sy_call_t *)sys_vfork
	},		/* 66 = vfork */
	{
		.sy_call = sys_nosys,
	},		/* 67 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 68 = filler */
	{
		ns(struct sys_sbrk_args),
		.sy_call = (sy_call_t *)sys_sbrk
	},		/* 69 = sbrk */
	{
		ns(struct sys_sstk_args),
		.sy_call = (sy_call_t *)sys_sstk
	},		/* 70 = sstk */
	{
		ns(struct ultrix_sys_mmap_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_mmap
	},		/* 71 = mmap */
	{
		ns(struct sys_ovadvise_args),
		.sy_call = (sy_call_t *)sys_ovadvise
	},		/* 72 = vadvise */
	{
		ns(struct sys_munmap_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_munmap
	},		/* 73 = munmap */
	{
		ns(struct sys_mprotect_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_mprotect
	},		/* 74 = mprotect */
	{
		ns(struct sys_madvise_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_madvise
	},		/* 75 = madvise */
	{
		.sy_call = (sy_call_t *)ultrix_sys_vhangup
	},		/* 76 = vhangup */
	{
		.sy_call = sys_nosys,
	},		/* 77 = filler */
	{
		ns(struct sys_mincore_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_mincore
	},		/* 78 = mincore */
	{
		ns(struct sys_getgroups_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_getgroups
	},		/* 79 = getgroups */
	{
		ns(struct sys_setgroups_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_setgroups
	},		/* 80 = setgroups */
	{
		.sy_call = (sy_call_t *)sys_getpgrp
	},		/* 81 = getpgrp */
	{
		ns(struct ultrix_sys_setpgrp_args),
		.sy_call = (sy_call_t *)ultrix_sys_setpgrp
	},		/* 82 = setpgrp */
	{
		ns(struct sys___setitimer50_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys___setitimer50
	},		/* 83 = __setitimer50 */
	{
		ns(struct ultrix_sys_wait3_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_wait3
	},		/* 84 = wait3 */
	{
		ns(struct compat_12_sys_swapon_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_12_sys_swapon
	},		/* 85 = swapon */
	{
		ns(struct sys___getitimer50_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys___getitimer50
	},		/* 86 = __getitimer50 */
	{
		ns(struct compat_43_sys_gethostname_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_gethostname
	},		/* 87 = gethostname */
	{
		ns(struct compat_43_sys_sethostname_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_sethostname
	},		/* 88 = sethostname */
	{
		.sy_call = (sy_call_t *)compat_43_sys_getdtablesize
	},		/* 89 = getdtablesize */
	{
		ns(struct sys_dup2_args),
		.sy_call = (sy_call_t *)sys_dup2
	},		/* 90 = dup2 */
	{
		.sy_call = sys_nosys,
	},		/* 91 = filler */
	{
		ns(struct ultrix_sys_fcntl_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_fcntl
	},		/* 92 = fcntl */
	{
		ns(struct ultrix_sys_select_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_select
	},		/* 93 = select */
	{
		.sy_call = sys_nosys,
	},		/* 94 = filler */
	{
		ns(struct sys_fsync_args),
		.sy_call = (sy_call_t *)sys_fsync
	},		/* 95 = fsync */
	{
		ns(struct sys_setpriority_args),
		.sy_call = (sy_call_t *)sys_setpriority
	},		/* 96 = setpriority */
	{
		ns(struct compat_30_sys_socket_args),
		.sy_call = (sy_call_t *)compat_30_sys_socket
	},		/* 97 = socket */
	{
		ns(struct sys_connect_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_connect
	},		/* 98 = connect */
	{
		ns(struct compat_43_sys_accept_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_accept
	},		/* 99 = accept */
	{
		ns(struct sys_getpriority_args),
		.sy_call = (sy_call_t *)sys_getpriority
	},		/* 100 = getpriority */
	{
		ns(struct compat_43_sys_send_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_send
	},		/* 101 = send */
	{
		ns(struct compat_43_sys_recv_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_recv
	},		/* 102 = recv */
	{
		ns(struct ultrix_sys_sigreturn_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_sigreturn
	},		/* 103 = sigreturn */
	{
		ns(struct sys_bind_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_bind
	},		/* 104 = bind */
	{
		ns(struct ultrix_sys_setsockopt_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_setsockopt
	},		/* 105 = setsockopt */
	{
		ns(struct sys_listen_args),
		.sy_call = (sy_call_t *)sys_listen
	},		/* 106 = listen */
	{
		.sy_call = sys_nosys,
	},		/* 107 = filler */
	{
		ns(struct ultrix_sys_sigvec_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_sigvec
	},		/* 108 = sigvec */
	{
		ns(struct compat_43_sys_sigblock_args),
		.sy_call = (sy_call_t *)compat_43_sys_sigblock
	},		/* 109 = sigblock */
	{
		ns(struct compat_43_sys_sigsetmask_args),
		.sy_call = (sy_call_t *)compat_43_sys_sigsetmask
	},		/* 110 = sigsetmask */
	{
		ns(struct ultrix_sys_sigsuspend_args),
		.sy_call = (sy_call_t *)ultrix_sys_sigsuspend
	},		/* 111 = sigsuspend */
	{
		ns(struct compat_43_sys_sigstack_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_sigstack
	},		/* 112 = sigstack */
	{
		ns(struct compat_43_sys_recvmsg_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_recvmsg
	},		/* 113 = recvmsg */
	{
		ns(struct compat_43_sys_sendmsg_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_sendmsg
	},		/* 114 = sendmsg */
	{
		.sy_call = sys_nosys,
	},		/* 115 = filler */
	{
		ns(struct sys___gettimeofday50_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys___gettimeofday50
	},		/* 116 = __gettimeofday50 */
	{
		ns(struct sys___getrusage50_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys___getrusage50
	},		/* 117 = __getrusage50 */
	{
		ns(struct sys_getsockopt_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_getsockopt
	},		/* 118 = getsockopt */
	{
		.sy_call = sys_nosys,
	},		/* 119 = filler */
	{
		ns(struct sys_readv_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_readv
	},		/* 120 = readv */
	{
		ns(struct sys_writev_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_writev
	},		/* 121 = writev */
	{
		ns(struct sys___settimeofday50_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys___settimeofday50
	},		/* 122 = __settimeofday50 */
	{
		ns(struct sys___posix_fchown_args),
		.sy_call = (sy_call_t *)sys___posix_fchown
	},		/* 123 = __posix_fchown */
	{
		ns(struct sys_fchmod_args),
		.sy_call = (sy_call_t *)sys_fchmod
	},		/* 124 = fchmod */
	{
		ns(struct compat_43_sys_recvfrom_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_recvfrom
	},		/* 125 = recvfrom */
	{
		ns(struct sys_setreuid_args),
		.sy_call = (sy_call_t *)sys_setreuid
	},		/* 126 = setreuid */
	{
		ns(struct sys_setregid_args),
		.sy_call = (sy_call_t *)sys_setregid
	},		/* 127 = setregid */
	{
		ns(struct sys_rename_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_rename
	},		/* 128 = rename */
	{
		ns(struct compat_43_sys_truncate_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_truncate
	},		/* 129 = truncate */
	{
		ns(struct compat_43_sys_ftruncate_args),
		.sy_call = (sy_call_t *)compat_43_sys_ftruncate
	},		/* 130 = ftruncate */
	{
		ns(struct sys_flock_args),
		.sy_call = (sy_call_t *)sys_flock
	},		/* 131 = flock */
	{
		.sy_call = sys_nosys,
	},		/* 132 = filler */
	{
		ns(struct sys_sendto_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_sendto
	},		/* 133 = sendto */
	{
		ns(struct sys_shutdown_args),
		.sy_call = (sy_call_t *)sys_shutdown
	},		/* 134 = shutdown */
	{
		ns(struct sys_socketpair_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_socketpair
	},		/* 135 = socketpair */
	{
		ns(struct sys_mkdir_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_mkdir
	},		/* 136 = mkdir */
	{
		ns(struct sys_rmdir_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_rmdir
	},		/* 137 = rmdir */
	{
		ns(struct sys___utimes50_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys___utimes50
	},		/* 138 = __utimes50 */
	{
		ns(struct ultrix_sys_sigcleanup_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_sigcleanup
	},		/* 139 = sigcleanup */
	{
		ns(struct sys___adjtime50_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys___adjtime50
	},		/* 140 = __adjtime50 */
	{
		ns(struct compat_43_sys_getpeername_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_getpeername
	},		/* 141 = getpeername */
	{
		.sy_call = (sy_call_t *)compat_43_sys_gethostid
	},		/* 142 = gethostid */
	{
		.sy_call = sys_nosys,
	},		/* 143 = filler */
	{
		ns(struct compat_43_sys_getrlimit_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_getrlimit
	},		/* 144 = getrlimit */
	{
		ns(struct compat_43_sys_setrlimit_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_setrlimit
	},		/* 145 = setrlimit */
	{
		ns(struct compat_43_sys_killpg_args),
		.sy_call = (sy_call_t *)compat_43_sys_killpg
	},		/* 146 = killpg */
	{
		.sy_call = sys_nosys,
	},		/* 147 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 148 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 149 = filler */
	{
		ns(struct compat_43_sys_getsockname_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_getsockname
	},		/* 150 = getsockname */
	{
		.sy_call = sys_nosys,
	},		/* 151 = filler */
#ifdef __mips
	{
		ns(struct ultrix_sys_cacheflush_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_cacheflush
	},		/* 152 = cacheflush */
	{
		ns(struct ultrix_sys_cachectl_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_cachectl
	},		/* 153 = cachectl */
#else	/* !mips */
	{
		.sy_call = sys_nosys,
	},		/* 152 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 153 = filler */
#endif	/* !mips */
	{
		.sy_call = sys_nosys,
	},		/* 154 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 155 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 156 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 157 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 158 = filler */
	{
		ns(struct compat_43_sys_getdirentries_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_getdirentries
	},		/* 159 = getdirentries */
	{
		ns(struct ultrix_sys_statfs_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_statfs
	},		/* 160 = statfs */
	{
		ns(struct ultrix_sys_fstatfs_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_fstatfs
	},		/* 161 = fstatfs */
	{
		.sy_call = sys_nosys,
	},		/* 162 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 163 = filler */
	{
		ns(struct compat_30_sys_getfh_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_30_sys_getfh
	},		/* 164 = getfh */
	{
		ns(struct compat_09_sys_getdomainname_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_09_sys_getdomainname
	},		/* 165 = getdomainname */
	{
		ns(struct compat_09_sys_setdomainname_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_09_sys_setdomainname
	},		/* 166 = setdomainname */
	{
		.sy_call = sys_nosys,
	},		/* 167 = filler */
	{
		ns(struct ultrix_sys_quotactl_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_quotactl
	},		/* 168 = quotactl */
	{
		ns(struct ultrix_sys_exportfs_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_exportfs
	},		/* 169 = exportfs */
	{
		.sy_call = sys_nosys,
	},		/* 170 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 171 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 172 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 173 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 174 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 175 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 176 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 177 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 178 = filler */
	{
		ns(struct ultrix_sys_uname_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_uname
	},		/* 179 = uname */
	{
		ns(struct ultrix_sys_shmsys_args),
		.sy_call = (sy_call_t *)ultrix_sys_shmsys
	},		/* 180 = shmsys */
	{
		.sy_call = sys_nosys,
	},		/* 181 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 182 = filler */
	{
		ns(struct ultrix_sys_ustat_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_ustat
	},		/* 183 = ustat */
	{
		ns(struct ultrix_sys_getmnt_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_getmnt
	},		/* 184 = getmnt */
	{
		.sy_call = sys_nosys,
	},		/* 185 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 186 = filler */
	{
		ns(struct ultrix_sys_sigpending_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_sigpending
	},		/* 187 = sigpending */
	{
		.sy_call = (sy_call_t *)sys_setsid
	},		/* 188 = setsid */
	{
		ns(struct ultrix_sys_waitpid_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_waitpid
	},		/* 189 = waitpid */
	{
		.sy_call = sys_nosys,
	},		/* 190 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 191 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 192 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 193 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 194 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 195 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 196 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 197 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 198 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 199 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 200 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 201 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 202 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 203 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 204 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 205 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 206 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 207 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 208 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 209 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 210 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 211 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 212 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 213 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 214 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 215 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 216 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 217 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 218 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 219 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 220 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 221 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 222 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 223 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 224 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 225 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 226 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 227 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 228 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 229 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 230 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 231 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 232 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 233 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 234 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 235 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 236 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 237 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 238 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 239 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 240 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 241 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 242 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 243 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 244 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 245 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 246 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 247 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 248 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 249 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 250 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 251 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 252 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 253 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 254 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 255 = filler */
	{
		ns(struct ultrix_sys_getsysinfo_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_getsysinfo
	},		/* 256 = getsysinfo */
	{
		ns(struct ultrix_sys_setsysinfo_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)ultrix_sys_setsysinfo
	},		/* 257 = setsysinfo */
	{
		.sy_call = sys_nosys,
	},		/* 258 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 259 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 260 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 261 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 262 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 263 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 264 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 265 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 266 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 267 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 268 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 269 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 270 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 271 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 272 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 273 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 274 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 275 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 276 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 277 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 278 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 279 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 280 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 281 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 282 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 283 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 284 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 285 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 286 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 287 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 288 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 289 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 290 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 291 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 292 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 293 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 294 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 295 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 296 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 297 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 298 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 299 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 300 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 301 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 302 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 303 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 304 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 305 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 306 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 307 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 308 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 309 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 310 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 311 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 312 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 313 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 314 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 315 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 316 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 317 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 318 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 319 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 320 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 321 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 322 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 323 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 324 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 325 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 326 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 327 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 328 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 329 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 330 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 331 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 332 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 333 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 334 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 335 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 336 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 337 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 338 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 339 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 340 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 341 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 342 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 343 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 344 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 345 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 346 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 347 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 348 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 349 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 350 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 351 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 352 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 353 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 354 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 355 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 356 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 357 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 358 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 359 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 360 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 361 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 362 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 363 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 364 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 365 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 366 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 367 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 368 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 369 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 370 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 371 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 372 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 373 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 374 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 375 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 376 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 377 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 378 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 379 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 380 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 381 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 382 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 383 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 384 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 385 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 386 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 387 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 388 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 389 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 390 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 391 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 392 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 393 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 394 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 395 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 396 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 397 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 398 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 399 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 400 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 401 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 402 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 403 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 404 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 405 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 406 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 407 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 408 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 409 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 410 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 411 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 412 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 413 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 414 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 415 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 416 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 417 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 418 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 419 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 420 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 421 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 422 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 423 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 424 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 425 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 426 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 427 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 428 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 429 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 430 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 431 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 432 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 433 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 434 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 435 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 436 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 437 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 438 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 439 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 440 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 441 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 442 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 443 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 444 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 445 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 446 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 447 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 448 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 449 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 450 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 451 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 452 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 453 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 454 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 455 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 456 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 457 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 458 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 459 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 460 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 461 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 462 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 463 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 464 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 465 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 466 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 467 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 468 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 469 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 470 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 471 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 472 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 473 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 474 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 475 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 476 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 477 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 478 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 479 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 480 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 481 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 482 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 483 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 484 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 485 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 486 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 487 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 488 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 489 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 490 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 491 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 492 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 493 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 494 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 495 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 496 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 497 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 498 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 499 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 500 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 501 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 502 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 503 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 504 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 505 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 506 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 507 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 508 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 509 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 510 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 511 = filler */
};
