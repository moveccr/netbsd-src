/* $NetBSD: sunos_syscalls.c,v 1.95 2018/01/06 16:41:23 kamil Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.77 2017/12/19 08:25:37 kamil Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: sunos_syscalls.c,v 1.95 2018/01/06 16:41:23 kamil Exp $");

#if defined(_KERNEL_OPT)
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
#else /* _KERNEL_OPT */
#include <sys/null.h>
#endif /* _KERNEL_OPT */

const char *const sunos_syscallnames[] = {
	/*   0 */	"syscall",
	/*   1 */	"exit",
	/*   2 */	"fork",
	/*   3 */	"read",
	/*   4 */	"write",
	/*   5 */	"open",
	/*   6 */	"close",
	/*   7 */	"wait4",
	/*   8 */	"creat",
	/*   9 */	"link",
	/*  10 */	"unlink",
	/*  11 */	"execv",
	/*  12 */	"chdir",
	/*  13 */	"#13 (obsolete old_time)",
	/*  14 */	"mknod",
	/*  15 */	"chmod",
	/*  16 */	"chown",
	/*  17 */	"break",
	/*  18 */	"#18 (obsolete old_stat)",
	/*  19 */	"lseek",
	/*  20 */	"getpid_with_ppid",
	/*  21 */	"#21 (obsolete sunos_old_mount)",
	/*  22 */	"#22 (unimplemented System V umount)",
	/*  23 */	"setuid",
	/*  24 */	"getuid_with_euid",
	/*  25 */	"stime",
	/*  26 */	"ptrace",
	/*  27 */	"#27 (unimplemented old_sunos_alarm)",
	/*  28 */	"#28 (unimplemented old_sunos_fstat)",
	/*  29 */	"#29 (unimplemented old_sunos_pause)",
	/*  30 */	"#30 (unimplemented old_sunos_utime)",
	/*  31 */	"#31 (unimplemented old_sunos_stty)",
	/*  32 */	"#32 (unimplemented old_sunos_gtty)",
	/*  33 */	"access",
	/*  34 */	"#34 (unimplemented old_sunos_nice)",
	/*  35 */	"#35 (unimplemented old_sunos_ftime)",
	/*  36 */	"sync",
	/*  37 */	"kill",
	/*  38 */	"stat",
	/*  39 */	"#39 (unimplemented sunos_setpgrp)",
	/*  40 */	"lstat",
	/*  41 */	"dup",
	/*  42 */	"pipe",
	/*  43 */	"#43 (unimplemented sunos_times)",
	/*  44 */	"profil",
	/*  45 */	"#45 (unimplemented)",
	/*  46 */	"setgid",
	/*  47 */	"getgid_with_egid",
	/*  48 */	"#48 (unimplemented sunos_ssig)",
	/*  49 */	"#49 (unimplemented reserved for USG)",
	/*  50 */	"#50 (unimplemented reserved for USG)",
	/*  51 */	"acct",
	/*  52 */	"#52 (unimplemented)",
	/*  53 */	"mctl",
	/*  54 */	"ioctl",
	/*  55 */	"reboot",
	/*  56 */	"#56 (obsolete sunos_owait3)",
	/*  57 */	"symlink",
	/*  58 */	"readlink",
	/*  59 */	"execve",
	/*  60 */	"umask",
	/*  61 */	"chroot",
	/*  62 */	"fstat",
	/*  63 */	"#63 (unimplemented)",
	/*  64 */	"getpagesize",
	/*  65 */	"omsync",
	/*  66 */	"vfork",
	/*  67 */	"#67 (obsolete vread)",
	/*  68 */	"#68 (obsolete vwrite)",
	/*  69 */	"#69 (obsolete sbrk)",
	/*  70 */	"#70 (obsolete sstk)",
	/*  71 */	"mmap",
	/*  72 */	"vadvise",
	/*  73 */	"munmap",
	/*  74 */	"mprotect",
	/*  75 */	"madvise",
	/*  76 */	"vhangup",
	/*  77 */	"#77 (unimplemented vlimit)",
	/*  78 */	"mincore",
	/*  79 */	"getgroups",
	/*  80 */	"setgroups",
	/*  81 */	"getpgrp",
	/*  82 */	"setpgrp",
	/*  83 */	"setitimer",
	/*  84 */	"#84 (unimplemented sunos_sys_wait)",
	/*  85 */	"swapon",
	/*  86 */	"getitimer",
	/*  87 */	"gethostname",
	/*  88 */	"sethostname",
	/*  89 */	"getdtablesize",
	/*  90 */	"dup2",
	/*  91 */	"#91 (unimplemented getdopt)",
	/*  92 */	"fcntl",
	/*  93 */	"select",
	/*  94 */	"#94 (unimplemented setdopt)",
	/*  95 */	"fsync",
	/*  96 */	"setpriority",
	/*  97 */	"socket",
	/*  98 */	"connect",
	/*  99 */	"accept",
	/* 100 */	"getpriority",
	/* 101 */	"send",
	/* 102 */	"recv",
	/* 103 */	"#103 (unimplemented old socketaddr)",
	/* 104 */	"bind",
	/* 105 */	"setsockopt",
	/* 106 */	"listen",
	/* 107 */	"#107 (unimplemented vtimes)",
	/* 108 */	"sigvec",
	/* 109 */	"sigblock",
	/* 110 */	"sigsetmask",
	/* 111 */	"sigsuspend",
	/* 112 */	"sigstack",
	/* 113 */	"recvmsg",
	/* 114 */	"sendmsg",
	/* 115 */	"#115 (obsolete vtrace)",
	/* 116 */	"gettimeofday",
	/* 117 */	"getrusage",
	/* 118 */	"getsockopt",
	/* 119 */	"#119 (unimplemented)",
	/* 120 */	"readv",
	/* 121 */	"writev",
	/* 122 */	"settimeofday",
	/* 123 */	"fchown",
	/* 124 */	"fchmod",
	/* 125 */	"recvfrom",
	/* 126 */	"setreuid",
	/* 127 */	"setregid",
	/* 128 */	"rename",
	/* 129 */	"truncate",
	/* 130 */	"ftruncate",
	/* 131 */	"flock",
	/* 132 */	"#132 (unimplemented)",
	/* 133 */	"sendto",
	/* 134 */	"shutdown",
	/* 135 */	"socketpair",
	/* 136 */	"mkdir",
	/* 137 */	"rmdir",
	/* 138 */	"utimes",
	/* 139 */	"sigreturn",
	/* 140 */	"adjtime",
	/* 141 */	"getpeername",
	/* 142 */	"gethostid",
	/* 143 */	"#143 (unimplemented old sethostid)",
	/* 144 */	"getrlimit",
	/* 145 */	"setrlimit",
	/* 146 */	"killpg",
	/* 147 */	"#147 (unimplemented)",
	/* 148 */	"#148 (unimplemented)",
	/* 149 */	"#149 (unimplemented)",
	/* 150 */	"getsockname",
	/* 151 */	"#151 (unimplemented getmsg)",
	/* 152 */	"#152 (unimplemented putmsg)",
	/* 153 */	"poll",
	/* 154 */	"#154 (unimplemented)",
	/* 155 */	"#155 (unimplemented nfssvc)",
	/* 156 */	"getdirentries",
	/* 157 */	"statfs",
	/* 158 */	"fstatfs",
	/* 159 */	"unmount",
	/* 160 */	"async_daemon",
	/* 161 */	"getfh",
	/* 162 */	"getdomainname",
	/* 163 */	"setdomainname",
	/* 164 */	"#164 (unimplemented rtschedule)",
	/* 165 */	"quotactl",
	/* 166 */	"exportfs",
	/* 167 */	"mount",
	/* 168 */	"ustat",
#ifdef SYSVSEM
	/* 169 */	"semsys",
#else
	/* 169 */	"#169 (unimplemented semsys)",
#endif
#ifdef SYSVMSG
	/* 170 */	"msgsys",
#else
	/* 170 */	"#170 (unimplemented msgsys)",
#endif
#ifdef SYSVSHM
	/* 171 */	"shmsys",
#else
	/* 171 */	"#171 (unimplemented shmsys)",
#endif
	/* 172 */	"auditsys",
	/* 173 */	"#173 (unimplemented rfssys)",
	/* 174 */	"getdents",
	/* 175 */	"setsid",
	/* 176 */	"fchdir",
	/* 177 */	"fchroot",
	/* 178 */	"#178 (unimplemented vpixsys)",
	/* 179 */	"#179 (unimplemented aioread)",
	/* 180 */	"#180 (unimplemented aiowrite)",
	/* 181 */	"#181 (unimplemented aiowait)",
	/* 182 */	"#182 (unimplemented aiocancel)",
	/* 183 */	"sigpending",
	/* 184 */	"#184 (unimplemented)",
	/* 185 */	"setpgid",
	/* 186 */	"pathconf",
	/* 187 */	"fpathconf",
	/* 188 */	"sysconf",
	/* 189 */	"uname",
	/* 190 */	"# filler",
	/* 191 */	"# filler",
	/* 192 */	"# filler",
	/* 193 */	"# filler",
	/* 194 */	"# filler",
	/* 195 */	"# filler",
	/* 196 */	"# filler",
	/* 197 */	"# filler",
	/* 198 */	"# filler",
	/* 199 */	"# filler",
	/* 200 */	"# filler",
	/* 201 */	"# filler",
	/* 202 */	"# filler",
	/* 203 */	"# filler",
	/* 204 */	"# filler",
	/* 205 */	"# filler",
	/* 206 */	"# filler",
	/* 207 */	"# filler",
	/* 208 */	"# filler",
	/* 209 */	"# filler",
	/* 210 */	"# filler",
	/* 211 */	"# filler",
	/* 212 */	"# filler",
	/* 213 */	"# filler",
	/* 214 */	"# filler",
	/* 215 */	"# filler",
	/* 216 */	"# filler",
	/* 217 */	"# filler",
	/* 218 */	"# filler",
	/* 219 */	"# filler",
	/* 220 */	"# filler",
	/* 221 */	"# filler",
	/* 222 */	"# filler",
	/* 223 */	"# filler",
	/* 224 */	"# filler",
	/* 225 */	"# filler",
	/* 226 */	"# filler",
	/* 227 */	"# filler",
	/* 228 */	"# filler",
	/* 229 */	"# filler",
	/* 230 */	"# filler",
	/* 231 */	"# filler",
	/* 232 */	"# filler",
	/* 233 */	"# filler",
	/* 234 */	"# filler",
	/* 235 */	"# filler",
	/* 236 */	"# filler",
	/* 237 */	"# filler",
	/* 238 */	"# filler",
	/* 239 */	"# filler",
	/* 240 */	"# filler",
	/* 241 */	"# filler",
	/* 242 */	"# filler",
	/* 243 */	"# filler",
	/* 244 */	"# filler",
	/* 245 */	"# filler",
	/* 246 */	"# filler",
	/* 247 */	"# filler",
	/* 248 */	"# filler",
	/* 249 */	"# filler",
	/* 250 */	"# filler",
	/* 251 */	"# filler",
	/* 252 */	"# filler",
	/* 253 */	"# filler",
	/* 254 */	"# filler",
	/* 255 */	"# filler",
};


/* libc style syscall names */
const char *const altsunos_syscallnames[] = {
	/*   0 */	"nosys",
	/*   1 */	NULL, /* exit */
	/*   2 */	NULL, /* fork */
	/*   3 */	NULL, /* read */
	/*   4 */	NULL, /* write */
	/*   5 */	NULL, /* open */
	/*   6 */	NULL, /* close */
	/*   7 */	NULL, /* wait4 */
	/*   8 */	NULL, /* creat */
	/*   9 */	NULL, /* link */
	/*  10 */	NULL, /* unlink */
	/*  11 */	NULL, /* execv */
	/*  12 */	NULL, /* chdir */
	/*  13 */	NULL, /* obsolete old_time */
	/*  14 */	NULL, /* mknod */
	/*  15 */	NULL, /* chmod */
	/*  16 */	NULL, /* chown */
	/*  17 */	"obreak",
	/*  18 */	NULL, /* obsolete old_stat */
	/*  19 */	NULL, /* lseek */
	/*  20 */	NULL, /* getpid_with_ppid */
	/*  21 */	NULL, /* obsolete sunos_old_mount */
	/*  22 */	NULL, /* unimplemented System V umount */
	/*  23 */	NULL, /* setuid */
	/*  24 */	NULL, /* getuid_with_euid */
	/*  25 */	NULL, /* stime */
	/*  26 */	NULL, /* ptrace */
	/*  27 */	NULL, /* unimplemented old_sunos_alarm */
	/*  28 */	NULL, /* unimplemented old_sunos_fstat */
	/*  29 */	NULL, /* unimplemented old_sunos_pause */
	/*  30 */	NULL, /* unimplemented old_sunos_utime */
	/*  31 */	NULL, /* unimplemented old_sunos_stty */
	/*  32 */	NULL, /* unimplemented old_sunos_gtty */
	/*  33 */	NULL, /* access */
	/*  34 */	NULL, /* unimplemented old_sunos_nice */
	/*  35 */	NULL, /* unimplemented old_sunos_ftime */
	/*  36 */	NULL, /* sync */
	/*  37 */	NULL, /* kill */
	/*  38 */	NULL, /* stat */
	/*  39 */	NULL, /* unimplemented sunos_setpgrp */
	/*  40 */	NULL, /* lstat */
	/*  41 */	NULL, /* dup */
	/*  42 */	NULL, /* pipe */
	/*  43 */	NULL, /* unimplemented sunos_times */
	/*  44 */	NULL, /* profil */
	/*  45 */	NULL, /* unimplemented */
	/*  46 */	NULL, /* setgid */
	/*  47 */	NULL, /* getgid_with_egid */
	/*  48 */	NULL, /* unimplemented sunos_ssig */
	/*  49 */	NULL, /* unimplemented reserved for USG */
	/*  50 */	NULL, /* unimplemented reserved for USG */
	/*  51 */	NULL, /* acct */
	/*  52 */	NULL, /* unimplemented */
	/*  53 */	NULL, /* mctl */
	/*  54 */	NULL, /* ioctl */
	/*  55 */	NULL, /* reboot */
	/*  56 */	NULL, /* obsolete sunos_owait3 */
	/*  57 */	NULL, /* symlink */
	/*  58 */	NULL, /* readlink */
	/*  59 */	NULL, /* execve */
	/*  60 */	NULL, /* umask */
	/*  61 */	NULL, /* chroot */
	/*  62 */	NULL, /* fstat */
	/*  63 */	NULL, /* unimplemented */
	/*  64 */	NULL, /* getpagesize */
	/*  65 */	NULL, /* omsync */
	/*  66 */	NULL, /* vfork */
	/*  67 */	NULL, /* obsolete vread */
	/*  68 */	NULL, /* obsolete vwrite */
	/*  69 */	NULL, /* obsolete sbrk */
	/*  70 */	NULL, /* obsolete sstk */
	/*  71 */	NULL, /* mmap */
	/*  72 */	"ovadvise",
	/*  73 */	NULL, /* munmap */
	/*  74 */	NULL, /* mprotect */
	/*  75 */	NULL, /* madvise */
	/*  76 */	NULL, /* vhangup */
	/*  77 */	NULL, /* unimplemented vlimit */
	/*  78 */	NULL, /* mincore */
	/*  79 */	NULL, /* getgroups */
	/*  80 */	NULL, /* setgroups */
	/*  81 */	NULL, /* getpgrp */
	/*  82 */	NULL, /* setpgrp */
	/*  83 */	NULL, /* setitimer */
	/*  84 */	NULL, /* unimplemented sunos_sys_wait */
	/*  85 */	NULL, /* swapon */
	/*  86 */	NULL, /* getitimer */
	/*  87 */	NULL, /* gethostname */
	/*  88 */	NULL, /* sethostname */
	/*  89 */	NULL, /* getdtablesize */
	/*  90 */	NULL, /* dup2 */
	/*  91 */	NULL, /* unimplemented getdopt */
	/*  92 */	NULL, /* fcntl */
	/*  93 */	NULL, /* select */
	/*  94 */	NULL, /* unimplemented setdopt */
	/*  95 */	NULL, /* fsync */
	/*  96 */	NULL, /* setpriority */
	/*  97 */	NULL, /* socket */
	/*  98 */	NULL, /* connect */
	/*  99 */	NULL, /* accept */
	/* 100 */	NULL, /* getpriority */
	/* 101 */	NULL, /* send */
	/* 102 */	NULL, /* recv */
	/* 103 */	NULL, /* unimplemented old socketaddr */
	/* 104 */	NULL, /* bind */
	/* 105 */	NULL, /* setsockopt */
	/* 106 */	NULL, /* listen */
	/* 107 */	NULL, /* unimplemented vtimes */
	/* 108 */	NULL, /* sigvec */
	/* 109 */	NULL, /* sigblock */
	/* 110 */	NULL, /* sigsetmask */
	/* 111 */	NULL, /* sigsuspend */
	/* 112 */	NULL, /* sigstack */
	/* 113 */	NULL, /* recvmsg */
	/* 114 */	NULL, /* sendmsg */
	/* 115 */	NULL, /* obsolete vtrace */
	/* 116 */	NULL, /* gettimeofday */
	/* 117 */	NULL, /* getrusage */
	/* 118 */	NULL, /* getsockopt */
	/* 119 */	NULL, /* unimplemented */
	/* 120 */	NULL, /* readv */
	/* 121 */	NULL, /* writev */
	/* 122 */	NULL, /* settimeofday */
	/* 123 */	NULL, /* fchown */
	/* 124 */	NULL, /* fchmod */
	/* 125 */	NULL, /* recvfrom */
	/* 126 */	NULL, /* setreuid */
	/* 127 */	NULL, /* setregid */
	/* 128 */	NULL, /* rename */
	/* 129 */	NULL, /* truncate */
	/* 130 */	NULL, /* ftruncate */
	/* 131 */	NULL, /* flock */
	/* 132 */	NULL, /* unimplemented */
	/* 133 */	NULL, /* sendto */
	/* 134 */	NULL, /* shutdown */
	/* 135 */	NULL, /* socketpair */
	/* 136 */	NULL, /* mkdir */
	/* 137 */	NULL, /* rmdir */
	/* 138 */	NULL, /* utimes */
	/* 139 */	NULL, /* sigreturn */
	/* 140 */	NULL, /* adjtime */
	/* 141 */	NULL, /* getpeername */
	/* 142 */	NULL, /* gethostid */
	/* 143 */	NULL, /* unimplemented old sethostid */
	/* 144 */	NULL, /* getrlimit */
	/* 145 */	NULL, /* setrlimit */
	/* 146 */	NULL, /* killpg */
	/* 147 */	NULL, /* unimplemented */
	/* 148 */	NULL, /* unimplemented */
	/* 149 */	NULL, /* unimplemented */
	/* 150 */	NULL, /* getsockname */
	/* 151 */	NULL, /* unimplemented getmsg */
	/* 152 */	NULL, /* unimplemented putmsg */
	/* 153 */	NULL, /* poll */
	/* 154 */	NULL, /* unimplemented */
	/* 155 */	NULL, /* unimplemented nfssvc */
	/* 156 */	NULL, /* getdirentries */
	/* 157 */	NULL, /* statfs */
	/* 158 */	NULL, /* fstatfs */
	/* 159 */	NULL, /* unmount */
	/* 160 */	"daemon",
	/* 161 */	NULL, /* getfh */
	/* 162 */	NULL, /* getdomainname */
	/* 163 */	NULL, /* setdomainname */
	/* 164 */	NULL, /* unimplemented rtschedule */
	/* 165 */	NULL, /* quotactl */
	/* 166 */	NULL, /* exportfs */
	/* 167 */	NULL, /* mount */
	/* 168 */	NULL, /* ustat */
#ifdef SYSVSEM
	/* 169 */	NULL, /* semsys */
#else
	/* 169 */	NULL, /* unimplemented semsys */
#endif
#ifdef SYSVMSG
	/* 170 */	NULL, /* msgsys */
#else
	/* 170 */	NULL, /* unimplemented msgsys */
#endif
#ifdef SYSVSHM
	/* 171 */	NULL, /* shmsys */
#else
	/* 171 */	NULL, /* unimplemented shmsys */
#endif
	/* 172 */	NULL, /* auditsys */
	/* 173 */	NULL, /* unimplemented rfssys */
	/* 174 */	NULL, /* getdents */
	/* 175 */	NULL, /* setsid */
	/* 176 */	NULL, /* fchdir */
	/* 177 */	NULL, /* fchroot */
	/* 178 */	NULL, /* unimplemented vpixsys */
	/* 179 */	NULL, /* unimplemented aioread */
	/* 180 */	NULL, /* unimplemented aiowrite */
	/* 181 */	NULL, /* unimplemented aiowait */
	/* 182 */	NULL, /* unimplemented aiocancel */
	/* 183 */	NULL, /* sigpending */
	/* 184 */	NULL, /* unimplemented */
	/* 185 */	NULL, /* setpgid */
	/* 186 */	NULL, /* pathconf */
	/* 187 */	NULL, /* fpathconf */
	/* 188 */	NULL, /* sysconf */
	/* 189 */	NULL, /* uname */
	/* 190 */	NULL, /* filler */
	/* 191 */	NULL, /* filler */
	/* 192 */	NULL, /* filler */
	/* 193 */	NULL, /* filler */
	/* 194 */	NULL, /* filler */
	/* 195 */	NULL, /* filler */
	/* 196 */	NULL, /* filler */
	/* 197 */	NULL, /* filler */
	/* 198 */	NULL, /* filler */
	/* 199 */	NULL, /* filler */
	/* 200 */	NULL, /* filler */
	/* 201 */	NULL, /* filler */
	/* 202 */	NULL, /* filler */
	/* 203 */	NULL, /* filler */
	/* 204 */	NULL, /* filler */
	/* 205 */	NULL, /* filler */
	/* 206 */	NULL, /* filler */
	/* 207 */	NULL, /* filler */
	/* 208 */	NULL, /* filler */
	/* 209 */	NULL, /* filler */
	/* 210 */	NULL, /* filler */
	/* 211 */	NULL, /* filler */
	/* 212 */	NULL, /* filler */
	/* 213 */	NULL, /* filler */
	/* 214 */	NULL, /* filler */
	/* 215 */	NULL, /* filler */
	/* 216 */	NULL, /* filler */
	/* 217 */	NULL, /* filler */
	/* 218 */	NULL, /* filler */
	/* 219 */	NULL, /* filler */
	/* 220 */	NULL, /* filler */
	/* 221 */	NULL, /* filler */
	/* 222 */	NULL, /* filler */
	/* 223 */	NULL, /* filler */
	/* 224 */	NULL, /* filler */
	/* 225 */	NULL, /* filler */
	/* 226 */	NULL, /* filler */
	/* 227 */	NULL, /* filler */
	/* 228 */	NULL, /* filler */
	/* 229 */	NULL, /* filler */
	/* 230 */	NULL, /* filler */
	/* 231 */	NULL, /* filler */
	/* 232 */	NULL, /* filler */
	/* 233 */	NULL, /* filler */
	/* 234 */	NULL, /* filler */
	/* 235 */	NULL, /* filler */
	/* 236 */	NULL, /* filler */
	/* 237 */	NULL, /* filler */
	/* 238 */	NULL, /* filler */
	/* 239 */	NULL, /* filler */
	/* 240 */	NULL, /* filler */
	/* 241 */	NULL, /* filler */
	/* 242 */	NULL, /* filler */
	/* 243 */	NULL, /* filler */
	/* 244 */	NULL, /* filler */
	/* 245 */	NULL, /* filler */
	/* 246 */	NULL, /* filler */
	/* 247 */	NULL, /* filler */
	/* 248 */	NULL, /* filler */
	/* 249 */	NULL, /* filler */
	/* 250 */	NULL, /* filler */
	/* 251 */	NULL, /* filler */
	/* 252 */	NULL, /* filler */
	/* 253 */	NULL, /* filler */
	/* 254 */	NULL, /* filler */
	/* 255 */	NULL, /* filler */
};
