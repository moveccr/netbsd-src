/* $NetBSD: sunos_sysent.c,v 1.91 2018/01/06 16:41:23 kamil Exp $ */

/*
 * System call switch table.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.77 2017/12/19 08:25:37 kamil Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: sunos_sysent.c,v 1.91 2018/01/06 16:41:23 kamil Exp $");

#if defined(_KERNEL_OPT)
#include "opt_sysv.h"
#endif
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/poll.h>
#include <sys/sched.h>
#include <sys/syscallargs.h>
#include <compat/sunos/sunos.h>
#include <compat/sunos/sunos_syscallargs.h>

#define	s(type)	sizeof(type)
#define	n(type)	(sizeof(type)/sizeof (register_t))
#define	ns(type)	.sy_narg = n(type), .sy_argsize = s(type)

struct sysent sunos_sysent[] = {
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
		ns(struct sunos_sys_open_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_open
	},		/* 5 = open */
	{
		ns(struct sys_close_args),
		.sy_call = (sy_call_t *)sys_close
	},		/* 6 = close */
	{
		ns(struct sunos_sys_wait4_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_wait4
	},		/* 7 = wait4 */
	{
		ns(struct sunos_sys_creat_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_creat
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
		ns(struct sunos_sys_execv_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_execv
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
		ns(struct sunos_sys_mknod_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_mknod
	},		/* 14 = mknod */
	{
		ns(struct sys_chmod_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_chmod
	},		/* 15 = chmod */
	{
		ns(struct sys_chown_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_chown
	},		/* 16 = chown */
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
		.sy_call = (sy_call_t *)sys_getpid_with_ppid
	},		/* 20 = getpid_with_ppid */
	{
		.sy_call = sys_nosys,
	},		/* 21 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 22 = filler */
	{
		ns(struct sys_setuid_args),
		.sy_call = (sy_call_t *)sys_setuid
	},		/* 23 = setuid */
	{
		.sy_call = (sy_call_t *)sys_getuid_with_euid
	},		/* 24 = getuid_with_euid */
	{
		ns(struct sunos_sys_stime_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_stime
	},		/* 25 = stime */
	{
		ns(struct sunos_sys_ptrace_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_ptrace
	},		/* 26 = ptrace */
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
		ns(struct sys_access_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_access
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
		ns(struct compat_43_sys_stat_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_stat
	},		/* 38 = stat */
	{
		.sy_call = sys_nosys,
	},		/* 39 = filler */
	{
		ns(struct compat_43_sys_lstat_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_43_sys_lstat
	},		/* 40 = lstat */
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
		ns(struct sys_setgid_args),
		.sy_call = (sy_call_t *)sys_setgid
	},		/* 46 = setgid */
	{
		.sy_call = (sy_call_t *)sys_getgid_with_egid
	},		/* 47 = getgid_with_egid */
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
		ns(struct sunos_sys_mctl_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_mctl
	},		/* 53 = mctl */
	{
		ns(struct sunos_sys_ioctl_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_ioctl
	},		/* 54 = ioctl */
	{
		ns(struct sunos_sys_reboot_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_reboot
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
		ns(struct sunos_sys_execve_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_execve
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
		ns(struct sunos_sys_omsync_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_omsync
	},		/* 65 = omsync */
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
		.sy_call = sys_nosys,
	},		/* 69 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 70 = filler */
	{
		ns(struct sunos_sys_mmap_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_mmap
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
		.sy_call = (sy_call_t *)sunos_sys_vhangup
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
		ns(struct sunos_sys_setpgrp_args),
		.sy_call = (sy_call_t *)sunos_sys_setpgrp
	},		/* 82 = setpgrp */
	{
		ns(struct compat_50_sys_setitimer_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_50_sys_setitimer
	},		/* 83 = setitimer */
	{
		.sy_call = sys_nosys,
	},		/* 84 = filler */
	{
		ns(struct compat_12_sys_swapon_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_12_sys_swapon
	},		/* 85 = swapon */
	{
		ns(struct compat_50_sys_getitimer_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_50_sys_getitimer
	},		/* 86 = getitimer */
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
		ns(struct sunos_sys_fcntl_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_fcntl
	},		/* 92 = fcntl */
	{
		ns(struct compat_50_sys_select_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_50_sys_select
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
		ns(struct sunos_sys_socket_args),
		.sy_call = (sy_call_t *)sunos_sys_socket
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
		.sy_call = sys_nosys,
	},		/* 103 = filler */
	{
		ns(struct sys_bind_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_bind
	},		/* 104 = bind */
	{
		ns(struct sunos_sys_setsockopt_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_setsockopt
	},		/* 105 = setsockopt */
	{
		ns(struct sys_listen_args),
		.sy_call = (sy_call_t *)sys_listen
	},		/* 106 = listen */
	{
		.sy_call = sys_nosys,
	},		/* 107 = filler */
	{
		ns(struct sunos_sys_sigvec_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_sigvec
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
		ns(struct sunos_sys_sigsuspend_args),
		.sy_call = (sy_call_t *)sunos_sys_sigsuspend
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
		ns(struct compat_50_sys_gettimeofday_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_50_sys_gettimeofday
	},		/* 116 = gettimeofday */
	{
		ns(struct compat_50_sys_getrusage_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_50_sys_getrusage
	},		/* 117 = getrusage */
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
		ns(struct compat_50_sys_settimeofday_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_50_sys_settimeofday
	},		/* 122 = settimeofday */
	{
		ns(struct sys_fchown_args),
		.sy_call = (sy_call_t *)sys_fchown
	},		/* 123 = fchown */
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
		ns(struct sunos_sys_socketpair_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_socketpair
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
		ns(struct compat_50_sys_utimes_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_50_sys_utimes
	},		/* 138 = utimes */
	{
		ns(struct sunos_sys_sigreturn_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_sigreturn
	},		/* 139 = sigreturn */
	{
		ns(struct compat_50_sys_adjtime_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_50_sys_adjtime
	},		/* 140 = adjtime */
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
		ns(struct sunos_sys_getrlimit_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_getrlimit
	},		/* 144 = getrlimit */
	{
		ns(struct sunos_sys_setrlimit_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_setrlimit
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
	{
		.sy_call = sys_nosys,
	},		/* 152 = filler */
	{
		ns(struct sys_poll_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_poll
	},		/* 153 = poll */
	{
		.sy_call = sys_nosys,
	},		/* 154 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 155 = filler */
	{
		ns(struct compat_12_sys_getdirentries_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_12_sys_getdirentries
	},		/* 156 = getdirentries */
	{
		ns(struct sunos_sys_statfs_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_statfs
	},		/* 157 = statfs */
	{
		ns(struct sunos_sys_fstatfs_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_fstatfs
	},		/* 158 = fstatfs */
	{
		ns(struct sunos_sys_unmount_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_unmount
	},		/* 159 = unmount */
	{
		.sy_call = (sy_call_t *)async_daemon
	},		/* 160 = async_daemon */
	{
		ns(struct compat_30_sys_getfh_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_30_sys_getfh
	},		/* 161 = getfh */
	{
		ns(struct compat_09_sys_getdomainname_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_09_sys_getdomainname
	},		/* 162 = getdomainname */
	{
		ns(struct compat_09_sys_setdomainname_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)compat_09_sys_setdomainname
	},		/* 163 = setdomainname */
	{
		.sy_call = sys_nosys,
	},		/* 164 = filler */
	{
		ns(struct sunos_sys_quotactl_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_quotactl
	},		/* 165 = quotactl */
	{
		ns(struct sunos_sys_exportfs_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_exportfs
	},		/* 166 = exportfs */
	{
		ns(struct sunos_sys_mount_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_mount
	},		/* 167 = mount */
	{
		ns(struct sunos_sys_ustat_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_ustat
	},		/* 168 = ustat */
#ifdef SYSVSEM
	{
		ns(struct compat_10_sys_semsys_args),
		.sy_call = (sy_call_t *)compat_10_sys_semsys
	},		/* 169 = semsys */
#else
	{
		.sy_call = sys_nosys,
	},		/* 169 = filler */
#endif
#ifdef SYSVMSG
	{
		ns(struct compat_10_sys_msgsys_args),
		.sy_call = (sy_call_t *)compat_10_sys_msgsys
	},		/* 170 = msgsys */
#else
	{
		.sy_call = sys_nosys,
	},		/* 170 = filler */
#endif
#ifdef SYSVSHM
	{
		ns(struct compat_10_sys_shmsys_args),
		.sy_call = (sy_call_t *)compat_10_sys_shmsys
	},		/* 171 = shmsys */
#else
	{
		.sy_call = sys_nosys,
	},		/* 171 = filler */
#endif
	{
		ns(struct sunos_sys_auditsys_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_auditsys
	},		/* 172 = auditsys */
	{
		.sy_call = sys_nosys,
	},		/* 173 = filler */
	{
		ns(struct sunos_sys_getdents_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_getdents
	},		/* 174 = getdents */
	{
		.sy_call = (sy_call_t *)sys_setsid
	},		/* 175 = setsid */
	{
		ns(struct sys_fchdir_args),
		.sy_call = (sy_call_t *)sys_fchdir
	},		/* 176 = fchdir */
	{
		ns(struct sys_fchroot_args),
		.sy_call = (sy_call_t *)sys_fchroot
	},		/* 177 = fchroot */
	{
		.sy_call = sys_nosys,
	},		/* 178 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 179 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 180 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 181 = filler */
	{
		.sy_call = sys_nosys,
	},		/* 182 = filler */
	{
		ns(struct sunos_sys_sigpending_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_sigpending
	},		/* 183 = sigpending */
	{
		.sy_call = sys_nosys,
	},		/* 184 = filler */
	{
		ns(struct sys_setpgid_args),
		.sy_call = (sy_call_t *)sys_setpgid
	},		/* 185 = setpgid */
	{
		ns(struct sys_pathconf_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sys_pathconf
	},		/* 186 = pathconf */
	{
		ns(struct sys_fpathconf_args),
		.sy_call = (sy_call_t *)sys_fpathconf
	},		/* 187 = fpathconf */
	{
		ns(struct sunos_sys_sysconf_args),
		.sy_call = (sy_call_t *)sunos_sys_sysconf
	},		/* 188 = sysconf */
	{
		ns(struct sunos_sys_uname_args),
		.sy_flags = SYCALL_ARG_PTR,
		.sy_call = (sy_call_t *)sunos_sys_uname
	},		/* 189 = uname */
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
};
