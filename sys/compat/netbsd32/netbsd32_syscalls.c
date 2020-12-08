/* $NetBSD: netbsd32_syscalls.c,v 1.154 2020/10/10 00:03:53 rin Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.139 2020/10/10 00:00:54 rin Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: netbsd32_syscalls.c,v 1.154 2020/10/10 00:03:53 rin Exp $");

#if defined(_KERNEL_OPT)
#if defined(_KERNEL_OPT)
#include "opt_compat_netbsd.h"
#include "opt_ntp.h"
#include "opt_compat_43.h"
#include "opt_quota.h"
#endif
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/syscallargs.h>
#include <compat/netbsd32/netbsd32.h>
#include <compat/netbsd32/netbsd32_syscall.h>
#include <compat/netbsd32/netbsd32_syscallargs.h>
#else /* _KERNEL_OPT */
#include <sys/null.h>
#endif /* _KERNEL_OPT */

const char *const netbsd32_syscallnames[] = {
	/*   0 */	"netbsd32_syscall",
	/*   1 */	"netbsd32_exit",
	/*   2 */	"fork",
	/*   3 */	"netbsd32_read",
	/*   4 */	"netbsd32_write",
	/*   5 */	"netbsd32_open",
	/*   6 */	"netbsd32_close",
	/*   7 */	"compat_50_netbsd32_wait4",
	/*   8 */	"compat_43_netbsd32_ocreat",
	/*   9 */	"netbsd32_link",
	/*  10 */	"netbsd32_unlink",
	/*  11 */	"#11 (obsolete execv)",
	/*  12 */	"netbsd32_chdir",
	/*  13 */	"netbsd32_fchdir",
	/*  14 */	"compat_50_netbsd32_mknod",
	/*  15 */	"netbsd32_chmod",
	/*  16 */	"netbsd32_chown",
	/*  17 */	"netbsd32_break",
	/*  18 */	"compat_20_netbsd32_getfsstat",
	/*  19 */	"compat_43_netbsd32_olseek",
	/*  20 */	"getpid",
	/*  21 */	"compat_40_netbsd32_mount",
	/*  22 */	"netbsd32_unmount",
	/*  23 */	"netbsd32_setuid",
	/*  24 */	"getuid",
	/*  25 */	"geteuid",
	/*  26 */	"netbsd32_ptrace",
	/*  27 */	"netbsd32_recvmsg",
	/*  28 */	"netbsd32_sendmsg",
	/*  29 */	"netbsd32_recvfrom",
	/*  30 */	"netbsd32_accept",
	/*  31 */	"netbsd32_getpeername",
	/*  32 */	"netbsd32_getsockname",
	/*  33 */	"netbsd32_access",
	/*  34 */	"netbsd32_chflags",
	/*  35 */	"netbsd32_fchflags",
	/*  36 */	"sync",
	/*  37 */	"netbsd32_kill",
	/*  38 */	"compat_43_netbsd32_stat43",
	/*  39 */	"getppid",
	/*  40 */	"compat_43_netbsd32_lstat43",
	/*  41 */	"netbsd32_dup",
	/*  42 */	"pipe",
	/*  43 */	"getegid",
	/*  44 */	"netbsd32_profil",
	/*  45 */	"netbsd32_ktrace",
	/*  46 */	"netbsd32_sigaction",
	/*  47 */	"getgid",
	/*  48 */	"compat_13_sigprocmask13",
	/*  49 */	"netbsd32___getlogin",
	/*  50 */	"netbsd32_setlogin",
	/*  51 */	"netbsd32_acct",
	/*  52 */	"compat_13_sigpending13",
	/*  53 */	"compat_13_netbsd32_sigaltstack13",
	/*  54 */	"netbsd32_ioctl",
	/*  55 */	"compat_12_netbsd32_reboot",
	/*  56 */	"netbsd32_revoke",
	/*  57 */	"netbsd32_symlink",
	/*  58 */	"netbsd32_readlink",
	/*  59 */	"netbsd32_execve",
	/*  60 */	"netbsd32_umask",
	/*  61 */	"netbsd32_chroot",
	/*  62 */	"compat_43_netbsd32_fstat43",
	/*  63 */	"compat_43_netbsd32_ogetkerninfo",
	/*  64 */	"compat_43_ogetpagesize",
	/*  65 */	"compat_12_netbsd32_msync",
	/*  66 */	"vfork",
	/*  67 */	"#67 (obsolete vread)",
	/*  68 */	"#68 (obsolete vwrite)",
	/*  69 */	"#69 (obsolete sbrk)",
	/*  70 */	"#70 (obsolete sstk)",
	/*  71 */	"compat_43_netbsd32_ommap",
	/*  72 */	"vadvise",
	/*  73 */	"netbsd32_munmap",
	/*  74 */	"netbsd32_mprotect",
	/*  75 */	"netbsd32_madvise",
	/*  76 */	"#76 (obsolete vhangup)",
	/*  77 */	"#77 (obsolete vlimit)",
	/*  78 */	"netbsd32_mincore",
	/*  79 */	"netbsd32_getgroups",
	/*  80 */	"netbsd32_setgroups",
	/*  81 */	"getpgrp",
	/*  82 */	"netbsd32_setpgid",
	/*  83 */	"compat_50_netbsd32_setitimer",
	/*  84 */	"compat_43_owait",
	/*  85 */	"compat_12_netbsd32_oswapon",
	/*  86 */	"compat_50_netbsd32_getitimer",
	/*  87 */	"compat_43_netbsd32_ogethostname",
	/*  88 */	"compat_43_netbsd32_osethostname",
	/*  89 */	"compat_43_ogetdtablesize",
	/*  90 */	"netbsd32_dup2",
	/*  91 */	"netbsd32_getrandom",
	/*  92 */	"netbsd32_fcntl",
	/*  93 */	"compat_50_netbsd32_select",
	/*  94 */	"#94 (unimplemented setdopt)",
	/*  95 */	"netbsd32_fsync",
	/*  96 */	"netbsd32_setpriority",
	/*  97 */	"compat_30_netbsd32_socket",
	/*  98 */	"netbsd32_connect",
	/*  99 */	"compat_43_netbsd32_oaccept",
	/* 100 */	"netbsd32_getpriority",
	/* 101 */	"compat_43_netbsd32_osend",
	/* 102 */	"compat_43_netbsd32_orecv",
	/* 103 */	"compat_13_sigreturn13",
	/* 104 */	"netbsd32_bind",
	/* 105 */	"netbsd32_setsockopt",
	/* 106 */	"netbsd32_listen",
	/* 107 */	"#107 (obsolete vtimes)",
	/* 108 */	"compat_43_netbsd32_osigvec",
	/* 109 */	"compat_43_netbsd32_sigblock",
	/* 110 */	"compat_43_netbsd32_sigsetmask",
	/* 111 */	"compat_13_sigsuspend13",
	/* 112 */	"compat_43_netbsd32_osigstack",
	/* 113 */	"compat_43_netbsd32_orecvmsg",
	/* 114 */	"compat_43_netbsd32_osendmsg",
	/* 115 */	"#115 (obsolete vtrace)",
	/* 116 */	"compat_50_netbsd32_gettimeofday",
	/* 117 */	"compat_50_netbsd32_getrusage",
	/* 118 */	"netbsd32_getsockopt",
	/* 119 */	"#119 (obsolete resuba)",
	/* 120 */	"netbsd32_readv",
	/* 121 */	"netbsd32_writev",
	/* 122 */	"compat_50_netbsd32_settimeofday",
	/* 123 */	"netbsd32_fchown",
	/* 124 */	"netbsd32_fchmod",
	/* 125 */	"compat_43_netbsd32_orecvfrom",
	/* 126 */	"netbsd32_setreuid",
	/* 127 */	"netbsd32_setregid",
	/* 128 */	"netbsd32_rename",
	/* 129 */	"compat_43_netbsd32_otruncate",
	/* 130 */	"compat_43_netbsd32_oftruncate",
	/* 131 */	"netbsd32_flock",
	/* 132 */	"netbsd32_mkfifo",
	/* 133 */	"netbsd32_sendto",
	/* 134 */	"netbsd32_shutdown",
	/* 135 */	"netbsd32_socketpair",
	/* 136 */	"netbsd32_mkdir",
	/* 137 */	"netbsd32_rmdir",
	/* 138 */	"compat_50_netbsd32_utimes",
	/* 139 */	"#139 (obsolete 4.2 sigreturn)",
	/* 140 */	"compat_50_netbsd32_adjtime",
	/* 141 */	"compat_43_netbsd32_ogetpeername",
	/* 142 */	"compat_43_ogethostid",
	/* 143 */	"compat_43_netbsd32_sethostid",
	/* 144 */	"compat_43_netbsd32_ogetrlimit",
	/* 145 */	"compat_43_netbsd32_osetrlimit",
	/* 146 */	"compat_43_netbsd32_killpg",
	/* 147 */	"setsid",
	/* 148 */	"compat_50_netbsd32_quotactl",
	/* 149 */	"#149 (excluded compat_netbsd32_quota)",
	/* 150 */	"compat_43_netbsd32_ogetsockname",
	/* 151 */	"#151 (unimplemented)",
	/* 152 */	"#152 (unimplemented)",
	/* 153 */	"#153 (unimplemented)",
	/* 154 */	"#154 (unimplemented)",
	/* 155 */	"netbsd32_nfssvc",
	/* 156 */	"compat_43_netbsd32_ogetdirentries",
	/* 157 */	"compat_20_netbsd32_statfs",
	/* 158 */	"compat_20_netbsd32_fstatfs",
	/* 159 */	"#159 (unimplemented)",
	/* 160 */	"#160 (unimplemented)",
	/* 161 */	"compat_30_netbsd32_getfh",
	/* 162 */	"compat_09_netbsd32_ogetdomainname",
	/* 163 */	"compat_09_netbsd32_osetdomainname",
	/* 164 */	"compat_09_netbsd32_uname",
	/* 165 */	"netbsd32_sysarch",
	/* 166 */	"netbsd32___futex",
	/* 167 */	"netbsd32___futex_set_robust_list",
	/* 168 */	"netbsd32___futex_get_robust_list",
	/* 169 */	"compat_10_osemsys",
	/* 170 */	"compat_10_omsgsys",
	/* 171 */	"compat_10_oshmsys",
	/* 172 */	"#172 (unimplemented)",
	/* 173 */	"netbsd32_pread",
	/* 174 */	"netbsd32_pwrite",
	/* 175 */	"compat_30_netbsd32_ntp_gettime",
#if defined(NTP) || !defined(_KERNEL_OPT)
	/* 176 */	"netbsd32_ntp_adjtime",
#else
	/* 176 */	"#176 (excluded ntp_adjtime)",
#endif
	/* 177 */	"#177 (unimplemented)",
	/* 178 */	"#178 (unimplemented)",
	/* 179 */	"#179 (unimplemented)",
	/* 180 */	"#180 (unimplemented)",
	/* 181 */	"netbsd32_setgid",
	/* 182 */	"netbsd32_setegid",
	/* 183 */	"netbsd32_seteuid",
	/* 184 */	"#184 (excluded netbsd32_lfs_bmapv)",
	/* 185 */	"#185 (excluded netbsd32_lfs_markv)",
	/* 186 */	"#186 (excluded netbsd32_lfs_segclean)",
	/* 187 */	"#187 (excluded netbsd32_lfs_segwait)",
	/* 188 */	"compat_12_netbsd32_stat12",
	/* 189 */	"compat_12_netbsd32_fstat12",
	/* 190 */	"compat_12_netbsd32_lstat12",
	/* 191 */	"netbsd32_pathconf",
	/* 192 */	"netbsd32_fpathconf",
	/* 193 */	"netbsd32_getsockopt2",
	/* 194 */	"netbsd32_getrlimit",
	/* 195 */	"netbsd32_setrlimit",
	/* 196 */	"compat_12_netbsd32_getdirentries",
	/* 197 */	"netbsd32_mmap",
	/* 198 */	"netbsd32____syscall",
	/* 199 */	"netbsd32_lseek",
	/* 200 */	"netbsd32_truncate",
	/* 201 */	"netbsd32_ftruncate",
	/* 202 */	"netbsd32___sysctl",
	/* 203 */	"netbsd32_mlock",
	/* 204 */	"netbsd32_munlock",
	/* 205 */	"netbsd32_undelete",
	/* 206 */	"compat_50_netbsd32_futimes",
	/* 207 */	"netbsd32_getpgid",
	/* 208 */	"netbsd32_reboot",
	/* 209 */	"netbsd32_poll",
	/* 210 */	"#210 (unimplemented { int | netbsd32 | | afssys ( long id , long a1 , long a2 , long a3 , long a4 , long a5 , long a6 ) ; })",
	/* 211 */	"#211 (unimplemented)",
	/* 212 */	"#212 (unimplemented)",
	/* 213 */	"#213 (unimplemented)",
	/* 214 */	"#214 (unimplemented)",
	/* 215 */	"#215 (unimplemented)",
	/* 216 */	"#216 (unimplemented)",
	/* 217 */	"#217 (unimplemented)",
	/* 218 */	"#218 (unimplemented)",
	/* 219 */	"#219 (unimplemented)",
	/* 220 */	"compat_14_netbsd32___semctl",
	/* 221 */	"netbsd32_semget",
	/* 222 */	"netbsd32_semop",
	/* 223 */	"netbsd32_semconfig",
	/* 224 */	"compat_14_netbsd32_msgctl",
	/* 225 */	"netbsd32_msgget",
	/* 226 */	"netbsd32_msgsnd",
	/* 227 */	"netbsd32_msgrcv",
	/* 228 */	"netbsd32_shmat",
	/* 229 */	"compat_14_netbsd32_shmctl",
	/* 230 */	"netbsd32_shmdt",
	/* 231 */	"netbsd32_shmget",
	/* 232 */	"compat_50_netbsd32_clock_gettime",
	/* 233 */	"compat_50_netbsd32_clock_settime",
	/* 234 */	"compat_50_netbsd32_clock_getres",
	/* 235 */	"netbsd32_timer_create",
	/* 236 */	"netbsd32_timer_delete",
	/* 237 */	"compat_50_netbsd32_timer_settime",
	/* 238 */	"compat_50_netbsd32_timer_gettime",
	/* 239 */	"netbsd32_timer_getoverrun",
	/* 240 */	"compat_50_netbsd32_nanosleep",
	/* 241 */	"netbsd32_fdatasync",
	/* 242 */	"netbsd32_mlockall",
	/* 243 */	"munlockall",
	/* 244 */	"compat_50_netbsd32___sigtimedwait",
	/* 245 */	"netbsd32_sigqueueinfo",
	/* 246 */	"netbsd32_modctl",
	/* 247 */	"netbsd32__ksem_init",
	/* 248 */	"netbsd32__ksem_open",
	/* 249 */	"netbsd32__ksem_unlink",
	/* 250 */	"netbsd32__ksem_close",
	/* 251 */	"netbsd32__ksem_post",
	/* 252 */	"netbsd32__ksem_wait",
	/* 253 */	"netbsd32__ksem_trywait",
	/* 254 */	"netbsd32__ksem_getvalue",
	/* 255 */	"netbsd32__ksem_destroy",
	/* 256 */	"netbsd32__ksem_timedwait",
	/* 257 */	"netbsd32_mq_open",
	/* 258 */	"netbsd32_mq_close",
	/* 259 */	"netbsd32_mq_unlink",
	/* 260 */	"netbsd32_mq_getattr",
	/* 261 */	"netbsd32_mq_setattr",
	/* 262 */	"netbsd32_mq_notify",
	/* 263 */	"netbsd32_mq_send",
	/* 264 */	"netbsd32_mq_receive",
	/* 265 */	"compat_50_netbsd32_mq_timedsend",
	/* 266 */	"compat_50_netbsd32_mq_timedreceive",
	/* 267 */	"#267 (unimplemented)",
	/* 268 */	"#268 (unimplemented)",
	/* 269 */	"#269 (unimplemented)",
	/* 270 */	"netbsd32___posix_rename",
	/* 271 */	"netbsd32_swapctl",
	/* 272 */	"compat_30_netbsd32_getdents",
	/* 273 */	"netbsd32_minherit",
	/* 274 */	"netbsd32_lchmod",
	/* 275 */	"netbsd32_lchown",
	/* 276 */	"compat_50_netbsd32_lutimes",
	/* 277 */	"netbsd32___msync13",
	/* 278 */	"compat_30_netbsd32___stat13",
	/* 279 */	"compat_30_netbsd32___fstat13",
	/* 280 */	"compat_30_netbsd32___lstat13",
	/* 281 */	"netbsd32___sigaltstack14",
	/* 282 */	"__vfork14",
	/* 283 */	"netbsd32___posix_chown",
	/* 284 */	"netbsd32___posix_fchown",
	/* 285 */	"netbsd32___posix_lchown",
	/* 286 */	"netbsd32_getsid",
	/* 287 */	"netbsd32___clone",
	/* 288 */	"netbsd32_fktrace",
	/* 289 */	"netbsd32_preadv",
	/* 290 */	"netbsd32_pwritev",
	/* 291 */	"netbsd32___sigaction14",
	/* 292 */	"netbsd32___sigpending14",
	/* 293 */	"netbsd32___sigprocmask14",
	/* 294 */	"netbsd32___sigsuspend14",
	/* 295 */	"compat_16_netbsd32___sigreturn14",
	/* 296 */	"netbsd32___getcwd",
	/* 297 */	"netbsd32_fchroot",
	/* 298 */	"compat_30_netbsd32_fhopen",
	/* 299 */	"compat_30_netbsd32_fhstat",
	/* 300 */	"compat_20_netbsd32_fhstatfs",
	/* 301 */	"compat_50_netbsd32___semctl14",
	/* 302 */	"compat_50_netbsd32___msgctl13",
	/* 303 */	"compat_50_netbsd32___shmctl13",
	/* 304 */	"netbsd32_lchflags",
	/* 305 */	"issetugid",
	/* 306 */	"netbsd32_utrace",
	/* 307 */	"netbsd32_getcontext",
	/* 308 */	"netbsd32_setcontext",
	/* 309 */	"netbsd32__lwp_create",
	/* 310 */	"_lwp_exit",
	/* 311 */	"_lwp_self",
	/* 312 */	"netbsd32__lwp_wait",
	/* 313 */	"netbsd32__lwp_suspend",
	/* 314 */	"netbsd32__lwp_continue",
	/* 315 */	"netbsd32__lwp_wakeup",
	/* 316 */	"_lwp_getprivate",
	/* 317 */	"netbsd32__lwp_setprivate",
	/* 318 */	"netbsd32__lwp_kill",
	/* 319 */	"netbsd32__lwp_detach",
	/* 320 */	"compat_50_netbsd32__lwp_park",
	/* 321 */	"netbsd32__lwp_unpark",
	/* 322 */	"netbsd32__lwp_unpark_all",
	/* 323 */	"netbsd32__lwp_setname",
	/* 324 */	"netbsd32__lwp_getname",
	/* 325 */	"netbsd32__lwp_ctl",
	/* 326 */	"#326 (unimplemented)",
	/* 327 */	"#327 (unimplemented)",
	/* 328 */	"#328 (unimplemented)",
	/* 329 */	"#329 (unimplemented)",
	/* 330 */	"#330 (obsolete netbsd32_sa_register)",
	/* 331 */	"#331 (obsolete netbsd32_sa_stacks)",
	/* 332 */	"#332 (obsolete sa_enable)",
	/* 333 */	"#333 (obsolete netbsd32_sa_setconcurrency)",
	/* 334 */	"#334 (obsolete sa_yield)",
	/* 335 */	"#335 (obsolete netbsd32_sa_preempt)",
	/* 336 */	"#336 (obsolete sys_sa_unblockyield)",
	/* 337 */	"#337 (unimplemented)",
	/* 338 */	"#338 (unimplemented)",
	/* 339 */	"#339 (unimplemented)",
	/* 340 */	"netbsd32___sigaction_sigtramp",
	/* 341 */	"#341 (obsolete netbsd32_pmc_get_info)",
	/* 342 */	"#342 (obsolete netbsd32_pmc_control)",
	/* 343 */	"netbsd32_rasctl",
	/* 344 */	"kqueue",
	/* 345 */	"compat_50_netbsd32_kevent",
	/* 346 */	"netbsd32__sched_setparam",
	/* 347 */	"netbsd32__sched_getparam",
	/* 348 */	"netbsd32__sched_setaffinity",
	/* 349 */	"netbsd32__sched_getaffinity",
	/* 350 */	"sched_yield",
	/* 351 */	"netbsd32__sched_protect",
	/* 352 */	"#352 (unimplemented)",
	/* 353 */	"#353 (unimplemented)",
	/* 354 */	"netbsd32_fsync_range",
	/* 355 */	"netbsd32_uuidgen",
	/* 356 */	"compat_90_netbsd32_getvfsstat",
	/* 357 */	"compat_90_netbsd32_statvfs1",
	/* 358 */	"compat_90_netbsd32_fstatvfs1",
	/* 359 */	"compat_30_netbsd32_fhstatvfs1",
	/* 360 */	"netbsd32_extattrctl",
	/* 361 */	"netbsd32_extattr_set_file",
	/* 362 */	"netbsd32_extattr_get_file",
	/* 363 */	"netbsd32_extattr_delete_file",
	/* 364 */	"netbsd32_extattr_set_fd",
	/* 365 */	"netbsd32_extattr_get_fd",
	/* 366 */	"netbsd32_extattr_delete_fd",
	/* 367 */	"netbsd32_extattr_set_link",
	/* 368 */	"netbsd32_extattr_get_link",
	/* 369 */	"netbsd32_extattr_delete_link",
	/* 370 */	"netbsd32_extattr_list_fd",
	/* 371 */	"netbsd32_extattr_list_file",
	/* 372 */	"netbsd32_extattr_list_link",
	/* 373 */	"compat_50_netbsd32_pselect",
	/* 374 */	"compat_50_netbsd32_pollts",
	/* 375 */	"netbsd32_setxattr",
	/* 376 */	"netbsd32_lsetxattr",
	/* 377 */	"netbsd32_fsetxattr",
	/* 378 */	"netbsd32_getxattr",
	/* 379 */	"netbsd32_lgetxattr",
	/* 380 */	"netbsd32_fgetxattr",
	/* 381 */	"netbsd32_listxattr",
	/* 382 */	"netbsd32_llistxattr",
	/* 383 */	"netbsd32_flistxattr",
	/* 384 */	"netbsd32_removexattr",
	/* 385 */	"netbsd32_lremovexattr",
	/* 386 */	"netbsd32_fremovexattr",
	/* 387 */	"compat_50_netbsd32___stat30",
	/* 388 */	"compat_50_netbsd32___fstat30",
	/* 389 */	"compat_50_netbsd32___lstat30",
	/* 390 */	"netbsd32___getdents30",
	/* 391 */	"#391 (ignored old posix fadvise)",
	/* 392 */	"compat_30_netbsd32___fhstat30",
	/* 393 */	"compat_50_netbsd32_ntp_gettime",
	/* 394 */	"netbsd32___socket30",
	/* 395 */	"netbsd32___getfh30",
	/* 396 */	"netbsd32___fhopen40",
	/* 397 */	"compat_90_netbsd32_fhstatvfs1",
	/* 398 */	"compat_50_netbsd32___fhstat40",
	/* 399 */	"#399 (unimplemented sys_aio_cancel)",
	/* 400 */	"#400 (unimplemented sys_aio_error)",
	/* 401 */	"#401 (unimplemented sys_aio_fsync)",
	/* 402 */	"#402 (unimplemented sys_aio_read)",
	/* 403 */	"#403 (unimplemented sys_aio_return)",
	/* 404 */	"#404 (unimplemented sys_aio_suspend)",
	/* 405 */	"#405 (unimplemented sys_aio_write)",
	/* 406 */	"#406 (unimplemented sys_lio_listio)",
	/* 407 */	"#407 (unimplemented)",
	/* 408 */	"#408 (unimplemented)",
	/* 409 */	"#409 (unimplemented)",
	/* 410 */	"netbsd32___mount50",
	/* 411 */	"netbsd32_mremap",
	/* 412 */	"netbsd32_pset_create",
	/* 413 */	"netbsd32_pset_destroy",
	/* 414 */	"netbsd32_pset_assign",
	/* 415 */	"netbsd32__pset_bind",
	/* 416 */	"netbsd32___posix_fadvise50",
	/* 417 */	"netbsd32___select50",
	/* 418 */	"netbsd32___gettimeofday50",
	/* 419 */	"netbsd32___settimeofday50",
	/* 420 */	"netbsd32___utimes50",
	/* 421 */	"netbsd32___adjtime50",
	/* 422 */	"#422 (excluded __lfs_segwait50)",
	/* 423 */	"netbsd32___futimes50",
	/* 424 */	"netbsd32___lutimes50",
	/* 425 */	"netbsd32___setitimer50",
	/* 426 */	"netbsd32___getitimer50",
	/* 427 */	"netbsd32___clock_gettime50",
	/* 428 */	"netbsd32___clock_settime50",
	/* 429 */	"netbsd32___clock_getres50",
	/* 430 */	"netbsd32___nanosleep50",
	/* 431 */	"netbsd32_____sigtimedwait50",
	/* 432 */	"netbsd32___mq_timedsend50",
	/* 433 */	"netbsd32___mq_timedreceive50",
	/* 434 */	"compat_60_netbsd32__lwp_park",
	/* 435 */	"netbsd32___kevent50",
	/* 436 */	"netbsd32___pselect50",
	/* 437 */	"netbsd32___pollts50",
	/* 438 */	"#438 (unimplemented netbsd32___aio_suspend50)",
	/* 439 */	"netbsd32___stat50",
	/* 440 */	"netbsd32___fstat50",
	/* 441 */	"netbsd32___lstat50",
	/* 442 */	"netbsd32_____semctl50",
	/* 443 */	"netbsd32___shmctl50",
	/* 444 */	"netbsd32___msgctl50",
	/* 445 */	"netbsd32___getrusage50",
	/* 446 */	"netbsd32___timer_settime50",
	/* 447 */	"netbsd32___timer_gettime50",
#if defined(NTP) || !defined(_KERNEL_OPT)
	/* 448 */	"netbsd32___ntp_gettime50",
#else
	/* 448 */	"#448 (excluded ___ntp_gettime50)",
#endif
	/* 449 */	"netbsd32___wait450",
	/* 450 */	"netbsd32___mknod50",
	/* 451 */	"netbsd32___fhstat50",
	/* 452 */	"#452 (obsolete 5.99 quotactl)",
	/* 453 */	"netbsd32_pipe2",
	/* 454 */	"netbsd32_dup3",
	/* 455 */	"netbsd32_kqueue1",
	/* 456 */	"netbsd32_paccept",
	/* 457 */	"netbsd32_linkat",
	/* 458 */	"netbsd32_renameat",
	/* 459 */	"netbsd32_mkfifoat",
	/* 460 */	"netbsd32_mknodat",
	/* 461 */	"netbsd32_mkdirat",
	/* 462 */	"netbsd32_faccessat",
	/* 463 */	"netbsd32_fchmodat",
	/* 464 */	"netbsd32_fchownat",
	/* 465 */	"netbsd32_fexecve",
	/* 466 */	"netbsd32_fstatat",
	/* 467 */	"netbsd32_utimensat",
	/* 468 */	"netbsd32_openat",
	/* 469 */	"netbsd32_readlinkat",
	/* 470 */	"netbsd32_symlinkat",
	/* 471 */	"netbsd32_unlinkat",
	/* 472 */	"netbsd32_futimens",
	/* 473 */	"netbsd32___quotactl",
	/* 474 */	"netbsd32_posix_spawn",
	/* 475 */	"netbsd32_recvmmsg",
	/* 476 */	"netbsd32_sendmmsg",
	/* 477 */	"netbsd32_clock_nanosleep",
	/* 478 */	"netbsd32____lwp_park60",
	/* 479 */	"netbsd32_posix_fallocate",
	/* 480 */	"netbsd32_fdiscard",
	/* 481 */	"netbsd32_wait6",
	/* 482 */	"netbsd32_clock_getcpuclockid2",
	/* 483 */	"netbsd32___getvfsstat90",
	/* 484 */	"netbsd32___statvfs190",
	/* 485 */	"netbsd32___fstatvfs190",
	/* 486 */	"netbsd32___fhstatvfs190",
	/* 487 */	"netbsd32___acl_get_link",
	/* 488 */	"netbsd32___acl_set_link",
	/* 489 */	"netbsd32___acl_delete_link",
	/* 490 */	"netbsd32___acl_aclcheck_link",
	/* 491 */	"netbsd32___acl_get_file",
	/* 492 */	"netbsd32___acl_set_file",
	/* 493 */	"netbsd32___acl_get_fd",
	/* 494 */	"netbsd32___acl_set_fd",
	/* 495 */	"netbsd32___acl_delete_file",
	/* 496 */	"netbsd32___acl_delete_fd",
	/* 497 */	"netbsd32___acl_aclcheck_file",
	/* 498 */	"netbsd32___acl_aclcheck_fd",
	/* 499 */	"netbsd32_lpathconf",
	/* 500 */	"# filler",
	/* 501 */	"# filler",
	/* 502 */	"# filler",
	/* 503 */	"# filler",
	/* 504 */	"# filler",
	/* 505 */	"# filler",
	/* 506 */	"# filler",
	/* 507 */	"# filler",
	/* 508 */	"# filler",
	/* 509 */	"# filler",
	/* 510 */	"# filler",
	/* 511 */	"# filler",
};


/* libc style syscall names */
const char *const altnetbsd32_syscallnames[] = {
	/*   0 */	"syscall",
	/*   1 */	"exit",
	/*   2 */	NULL, /* fork */
	/*   3 */	"read",
	/*   4 */	"write",
	/*   5 */	"open",
	/*   6 */	"close",
	/*   7 */	NULL, /* compat_50_netbsd32_wait4 */
	/*   8 */	NULL, /* compat_43_netbsd32_ocreat */
	/*   9 */	"link",
	/*  10 */	"unlink",
	/*  11 */	NULL, /* obsolete execv */
	/*  12 */	"chdir",
	/*  13 */	"fchdir",
	/*  14 */	NULL, /* compat_50_netbsd32_mknod */
	/*  15 */	"chmod",
	/*  16 */	"chown",
	/*  17 */	"break",
	/*  18 */	NULL, /* compat_20_netbsd32_getfsstat */
	/*  19 */	NULL, /* compat_43_netbsd32_olseek */
	/*  20 */	"getpid_with_ppid",
	/*  21 */	NULL, /* compat_40_netbsd32_mount */
	/*  22 */	"unmount",
	/*  23 */	"setuid",
	/*  24 */	"getuid_with_euid",
	/*  25 */	NULL, /* geteuid */
	/*  26 */	"ptrace",
	/*  27 */	"recvmsg",
	/*  28 */	"sendmsg",
	/*  29 */	"recvfrom",
	/*  30 */	"accept",
	/*  31 */	"getpeername",
	/*  32 */	"getsockname",
	/*  33 */	"access",
	/*  34 */	"chflags",
	/*  35 */	"fchflags",
	/*  36 */	NULL, /* sync */
	/*  37 */	"kill",
	/*  38 */	NULL, /* compat_43_netbsd32_stat43 */
	/*  39 */	NULL, /* getppid */
	/*  40 */	NULL, /* compat_43_netbsd32_lstat43 */
	/*  41 */	"dup",
	/*  42 */	NULL, /* pipe */
	/*  43 */	NULL, /* getegid */
	/*  44 */	"profil",
	/*  45 */	"ktrace",
	/*  46 */	"sigaction",
	/*  47 */	"getgid_with_egid",
	/*  48 */	NULL, /* compat_13_sigprocmask13 */
	/*  49 */	"__getlogin",
	/*  50 */	"setlogin",
	/*  51 */	"acct",
	/*  52 */	NULL, /* compat_13_sigpending13 */
	/*  53 */	NULL, /* compat_13_netbsd32_sigaltstack13 */
	/*  54 */	"ioctl",
	/*  55 */	NULL, /* compat_12_netbsd32_reboot */
	/*  56 */	"revoke",
	/*  57 */	"symlink",
	/*  58 */	"readlink",
	/*  59 */	"execve",
	/*  60 */	"umask",
	/*  61 */	"chroot",
	/*  62 */	NULL, /* compat_43_netbsd32_fstat43 */
	/*  63 */	NULL, /* compat_43_netbsd32_ogetkerninfo */
	/*  64 */	NULL, /* compat_43_ogetpagesize */
	/*  65 */	NULL, /* compat_12_netbsd32_msync */
	/*  66 */	NULL, /* vfork */
	/*  67 */	NULL, /* obsolete vread */
	/*  68 */	NULL, /* obsolete vwrite */
	/*  69 */	NULL, /* obsolete sbrk */
	/*  70 */	NULL, /* obsolete sstk */
	/*  71 */	NULL, /* compat_43_netbsd32_ommap */
	/*  72 */	"ovadvise",
	/*  73 */	"munmap",
	/*  74 */	"mprotect",
	/*  75 */	"madvise",
	/*  76 */	NULL, /* obsolete vhangup */
	/*  77 */	NULL, /* obsolete vlimit */
	/*  78 */	"mincore",
	/*  79 */	"getgroups",
	/*  80 */	"setgroups",
	/*  81 */	NULL, /* getpgrp */
	/*  82 */	"setpgid",
	/*  83 */	NULL, /* compat_50_netbsd32_setitimer */
	/*  84 */	NULL, /* compat_43_owait */
	/*  85 */	NULL, /* compat_12_netbsd32_oswapon */
	/*  86 */	NULL, /* compat_50_netbsd32_getitimer */
	/*  87 */	NULL, /* compat_43_netbsd32_ogethostname */
	/*  88 */	NULL, /* compat_43_netbsd32_osethostname */
	/*  89 */	NULL, /* compat_43_ogetdtablesize */
	/*  90 */	"dup2",
	/*  91 */	"getrandom",
	/*  92 */	"fcntl",
	/*  93 */	NULL, /* compat_50_netbsd32_select */
	/*  94 */	NULL, /* unimplemented setdopt */
	/*  95 */	"fsync",
	/*  96 */	"setpriority",
	/*  97 */	NULL, /* compat_30_netbsd32_socket */
	/*  98 */	"connect",
	/*  99 */	NULL, /* compat_43_netbsd32_oaccept */
	/* 100 */	"getpriority",
	/* 101 */	NULL, /* compat_43_netbsd32_osend */
	/* 102 */	NULL, /* compat_43_netbsd32_orecv */
	/* 103 */	NULL, /* compat_13_sigreturn13 */
	/* 104 */	"bind",
	/* 105 */	"setsockopt",
	/* 106 */	"listen",
	/* 107 */	NULL, /* obsolete vtimes */
	/* 108 */	NULL, /* compat_43_netbsd32_osigvec */
	/* 109 */	NULL, /* compat_43_netbsd32_sigblock */
	/* 110 */	NULL, /* compat_43_netbsd32_sigsetmask */
	/* 111 */	NULL, /* compat_13_sigsuspend13 */
	/* 112 */	NULL, /* compat_43_netbsd32_osigstack */
	/* 113 */	NULL, /* compat_43_netbsd32_orecvmsg */
	/* 114 */	NULL, /* compat_43_netbsd32_osendmsg */
	/* 115 */	NULL, /* obsolete vtrace */
	/* 116 */	NULL, /* compat_50_netbsd32_gettimeofday */
	/* 117 */	NULL, /* compat_50_netbsd32_getrusage */
	/* 118 */	"getsockopt",
	/* 119 */	NULL, /* obsolete resuba */
	/* 120 */	"readv",
	/* 121 */	"writev",
	/* 122 */	NULL, /* compat_50_netbsd32_settimeofday */
	/* 123 */	"fchown",
	/* 124 */	"fchmod",
	/* 125 */	NULL, /* compat_43_netbsd32_orecvfrom */
	/* 126 */	"setreuid",
	/* 127 */	"setregid",
	/* 128 */	"rename",
	/* 129 */	NULL, /* compat_43_netbsd32_otruncate */
	/* 130 */	NULL, /* compat_43_netbsd32_oftruncate */
	/* 131 */	"flock",
	/* 132 */	"mkfifo",
	/* 133 */	"sendto",
	/* 134 */	"shutdown",
	/* 135 */	"socketpair",
	/* 136 */	"mkdir",
	/* 137 */	"rmdir",
	/* 138 */	NULL, /* compat_50_netbsd32_utimes */
	/* 139 */	NULL, /* obsolete 4.2 sigreturn */
	/* 140 */	NULL, /* compat_50_netbsd32_adjtime */
	/* 141 */	NULL, /* compat_43_netbsd32_ogetpeername */
	/* 142 */	NULL, /* compat_43_ogethostid */
	/* 143 */	NULL, /* compat_43_netbsd32_sethostid */
	/* 144 */	NULL, /* compat_43_netbsd32_ogetrlimit */
	/* 145 */	NULL, /* compat_43_netbsd32_osetrlimit */
	/* 146 */	NULL, /* compat_43_netbsd32_killpg */
	/* 147 */	NULL, /* setsid */
	/* 148 */	NULL, /* compat_50_netbsd32_quotactl */
	/* 149 */	NULL, /* excluded compat_netbsd32_quota */
	/* 150 */	NULL, /* compat_43_netbsd32_ogetsockname */
	/* 151 */	NULL, /* unimplemented */
	/* 152 */	NULL, /* unimplemented */
	/* 153 */	NULL, /* unimplemented */
	/* 154 */	NULL, /* unimplemented */
	/* 155 */	"nfssvc",
	/* 156 */	NULL, /* compat_43_netbsd32_ogetdirentries */
	/* 157 */	NULL, /* compat_20_netbsd32_statfs */
	/* 158 */	NULL, /* compat_20_netbsd32_fstatfs */
	/* 159 */	NULL, /* unimplemented */
	/* 160 */	NULL, /* unimplemented */
	/* 161 */	NULL, /* compat_30_netbsd32_getfh */
	/* 162 */	NULL, /* compat_09_netbsd32_ogetdomainname */
	/* 163 */	NULL, /* compat_09_netbsd32_osetdomainname */
	/* 164 */	NULL, /* compat_09_netbsd32_uname */
	/* 165 */	"sysarch",
	/* 166 */	"__futex",
	/* 167 */	"__futex_set_robust_list",
	/* 168 */	"__futex_get_robust_list",
	/* 169 */	NULL, /* compat_10_osemsys */
	/* 170 */	NULL, /* compat_10_omsgsys */
	/* 171 */	NULL, /* compat_10_oshmsys */
	/* 172 */	NULL, /* unimplemented */
	/* 173 */	"pread",
	/* 174 */	"pwrite",
	/* 175 */	NULL, /* compat_30_netbsd32_ntp_gettime */
#if defined(NTP) || !defined(_KERNEL_OPT)
	/* 176 */	"ntp_adjtime",
#else
	/* 176 */	NULL, /* excluded ntp_adjtime */
#endif
	/* 177 */	NULL, /* unimplemented */
	/* 178 */	NULL, /* unimplemented */
	/* 179 */	NULL, /* unimplemented */
	/* 180 */	NULL, /* unimplemented */
	/* 181 */	"setgid",
	/* 182 */	"setegid",
	/* 183 */	"seteuid",
	/* 184 */	NULL, /* excluded netbsd32_lfs_bmapv */
	/* 185 */	NULL, /* excluded netbsd32_lfs_markv */
	/* 186 */	NULL, /* excluded netbsd32_lfs_segclean */
	/* 187 */	NULL, /* excluded netbsd32_lfs_segwait */
	/* 188 */	NULL, /* compat_12_netbsd32_stat12 */
	/* 189 */	NULL, /* compat_12_netbsd32_fstat12 */
	/* 190 */	NULL, /* compat_12_netbsd32_lstat12 */
	/* 191 */	"pathconf",
	/* 192 */	"fpathconf",
	/* 193 */	"getsockopt2",
	/* 194 */	"getrlimit",
	/* 195 */	"setrlimit",
	/* 196 */	NULL, /* compat_12_netbsd32_getdirentries */
	/* 197 */	"mmap",
	/* 198 */	"___syscall",
	/* 199 */	"lseek",
	/* 200 */	"truncate",
	/* 201 */	"ftruncate",
	/* 202 */	"__sysctl",
	/* 203 */	"mlock",
	/* 204 */	"munlock",
	/* 205 */	"undelete",
	/* 206 */	NULL, /* compat_50_netbsd32_futimes */
	/* 207 */	"getpgid",
	/* 208 */	"reboot",
	/* 209 */	"poll",
	/* 210 */	NULL, /* unimplemented { int | netbsd32 | | afssys ( long id , long a1 , long a2 , long a3 , long a4 , long a5 , long a6 ) ; } */
	/* 211 */	NULL, /* unimplemented */
	/* 212 */	NULL, /* unimplemented */
	/* 213 */	NULL, /* unimplemented */
	/* 214 */	NULL, /* unimplemented */
	/* 215 */	NULL, /* unimplemented */
	/* 216 */	NULL, /* unimplemented */
	/* 217 */	NULL, /* unimplemented */
	/* 218 */	NULL, /* unimplemented */
	/* 219 */	NULL, /* unimplemented */
	/* 220 */	NULL, /* compat_14_netbsd32___semctl */
	/* 221 */	"semget",
	/* 222 */	"semop",
	/* 223 */	"semconfig",
	/* 224 */	NULL, /* compat_14_netbsd32_msgctl */
	/* 225 */	"msgget",
	/* 226 */	"msgsnd",
	/* 227 */	"msgrcv",
	/* 228 */	"shmat",
	/* 229 */	NULL, /* compat_14_netbsd32_shmctl */
	/* 230 */	"shmdt",
	/* 231 */	"shmget",
	/* 232 */	NULL, /* compat_50_netbsd32_clock_gettime */
	/* 233 */	NULL, /* compat_50_netbsd32_clock_settime */
	/* 234 */	NULL, /* compat_50_netbsd32_clock_getres */
	/* 235 */	"timer_create",
	/* 236 */	"timer_delete",
	/* 237 */	NULL, /* compat_50_netbsd32_timer_settime */
	/* 238 */	NULL, /* compat_50_netbsd32_timer_gettime */
	/* 239 */	"timer_getoverrun",
	/* 240 */	NULL, /* compat_50_netbsd32_nanosleep */
	/* 241 */	"fdatasync",
	/* 242 */	"mlockall",
	/* 243 */	NULL, /* munlockall */
	/* 244 */	NULL, /* compat_50_netbsd32___sigtimedwait */
	/* 245 */	"sigqueueinfo",
	/* 246 */	"modctl",
	/* 247 */	"_ksem_init",
	/* 248 */	"_ksem_open",
	/* 249 */	"_ksem_unlink",
	/* 250 */	"_ksem_close",
	/* 251 */	"_ksem_post",
	/* 252 */	"_ksem_wait",
	/* 253 */	"_ksem_trywait",
	/* 254 */	"_ksem_getvalue",
	/* 255 */	"_ksem_destroy",
	/* 256 */	"_ksem_timedwait",
	/* 257 */	"mq_open",
	/* 258 */	"mq_close",
	/* 259 */	"mq_unlink",
	/* 260 */	"mq_getattr",
	/* 261 */	"mq_setattr",
	/* 262 */	"mq_notify",
	/* 263 */	"mq_send",
	/* 264 */	"mq_receive",
	/* 265 */	NULL, /* compat_50_netbsd32_mq_timedsend */
	/* 266 */	NULL, /* compat_50_netbsd32_mq_timedreceive */
	/* 267 */	NULL, /* unimplemented */
	/* 268 */	NULL, /* unimplemented */
	/* 269 */	NULL, /* unimplemented */
	/* 270 */	"__posix_rename",
	/* 271 */	"swapctl",
	/* 272 */	NULL, /* compat_30_netbsd32_getdents */
	/* 273 */	"minherit",
	/* 274 */	"lchmod",
	/* 275 */	"lchown",
	/* 276 */	NULL, /* compat_50_netbsd32_lutimes */
	/* 277 */	"msync",
	/* 278 */	NULL, /* compat_30_netbsd32___stat13 */
	/* 279 */	NULL, /* compat_30_netbsd32___fstat13 */
	/* 280 */	NULL, /* compat_30_netbsd32___lstat13 */
	/* 281 */	"sigaltstack",
	/* 282 */	"vfork",
	/* 283 */	"__posix_chown",
	/* 284 */	"__posix_fchown",
	/* 285 */	"__posix_lchown",
	/* 286 */	"getsid",
	/* 287 */	"__clone",
	/* 288 */	"fktrace",
	/* 289 */	"preadv",
	/* 290 */	"pwritev",
	/* 291 */	"sigaction",
	/* 292 */	"sigpending",
	/* 293 */	"sigprocmask",
	/* 294 */	"sigsuspend",
	/* 295 */	NULL, /* compat_16_netbsd32___sigreturn14 */
	/* 296 */	"__getcwd",
	/* 297 */	"fchroot",
	/* 298 */	NULL, /* compat_30_netbsd32_fhopen */
	/* 299 */	NULL, /* compat_30_netbsd32_fhstat */
	/* 300 */	NULL, /* compat_20_netbsd32_fhstatfs */
	/* 301 */	NULL, /* compat_50_netbsd32___semctl14 */
	/* 302 */	NULL, /* compat_50_netbsd32___msgctl13 */
	/* 303 */	NULL, /* compat_50_netbsd32___shmctl13 */
	/* 304 */	"lchflags",
	/* 305 */	NULL, /* issetugid */
	/* 306 */	"utrace",
	/* 307 */	"getcontext",
	/* 308 */	"setcontext",
	/* 309 */	"_lwp_create",
	/* 310 */	NULL, /* _lwp_exit */
	/* 311 */	NULL, /* _lwp_self */
	/* 312 */	"_lwp_wait",
	/* 313 */	"_lwp_suspend",
	/* 314 */	"_lwp_continue",
	/* 315 */	"_lwp_wakeup",
	/* 316 */	NULL, /* _lwp_getprivate */
	/* 317 */	"_lwp_setprivate",
	/* 318 */	"_lwp_kill",
	/* 319 */	"_lwp_detach",
	/* 320 */	NULL, /* compat_50_netbsd32__lwp_park */
	/* 321 */	"_lwp_unpark",
	/* 322 */	"_lwp_unpark_all",
	/* 323 */	"_lwp_setname",
	/* 324 */	"_lwp_getname",
	/* 325 */	"_lwp_ctl",
	/* 326 */	NULL, /* unimplemented */
	/* 327 */	NULL, /* unimplemented */
	/* 328 */	NULL, /* unimplemented */
	/* 329 */	NULL, /* unimplemented */
	/* 330 */	NULL, /* obsolete netbsd32_sa_register */
	/* 331 */	NULL, /* obsolete netbsd32_sa_stacks */
	/* 332 */	NULL, /* obsolete sa_enable */
	/* 333 */	NULL, /* obsolete netbsd32_sa_setconcurrency */
	/* 334 */	NULL, /* obsolete sa_yield */
	/* 335 */	NULL, /* obsolete netbsd32_sa_preempt */
	/* 336 */	NULL, /* obsolete sys_sa_unblockyield */
	/* 337 */	NULL, /* unimplemented */
	/* 338 */	NULL, /* unimplemented */
	/* 339 */	NULL, /* unimplemented */
	/* 340 */	"__sigaction_sigtramp",
	/* 341 */	NULL, /* obsolete netbsd32_pmc_get_info */
	/* 342 */	NULL, /* obsolete netbsd32_pmc_control */
	/* 343 */	"rasctl",
	/* 344 */	NULL, /* kqueue */
	/* 345 */	NULL, /* compat_50_netbsd32_kevent */
	/* 346 */	"_sched_setparam",
	/* 347 */	"_sched_getparam",
	/* 348 */	"_sched_setaffinity",
	/* 349 */	"_sched_getaffinity",
	/* 350 */	NULL, /* sched_yield */
	/* 351 */	"_sched_protect",
	/* 352 */	NULL, /* unimplemented */
	/* 353 */	NULL, /* unimplemented */
	/* 354 */	"fsync_range",
	/* 355 */	"uuidgen",
	/* 356 */	NULL, /* compat_90_netbsd32_getvfsstat */
	/* 357 */	NULL, /* compat_90_netbsd32_statvfs1 */
	/* 358 */	NULL, /* compat_90_netbsd32_fstatvfs1 */
	/* 359 */	NULL, /* compat_30_netbsd32_fhstatvfs1 */
	/* 360 */	"extattrctl",
	/* 361 */	"extattr_set_file",
	/* 362 */	"extattr_get_file",
	/* 363 */	"extattr_delete_file",
	/* 364 */	"extattr_set_fd",
	/* 365 */	"extattr_get_fd",
	/* 366 */	"extattr_delete_fd",
	/* 367 */	"extattr_set_link",
	/* 368 */	"extattr_get_link",
	/* 369 */	"extattr_delete_link",
	/* 370 */	"extattr_list_fd",
	/* 371 */	"extattr_list_file",
	/* 372 */	"extattr_list_link",
	/* 373 */	NULL, /* compat_50_netbsd32_pselect */
	/* 374 */	NULL, /* compat_50_netbsd32_pollts */
	/* 375 */	"setxattr",
	/* 376 */	"lsetxattr",
	/* 377 */	"fsetxattr",
	/* 378 */	"getxattr",
	/* 379 */	"lgetxattr",
	/* 380 */	"fgetxattr",
	/* 381 */	"listxattr",
	/* 382 */	"llistxattr",
	/* 383 */	"flistxattr",
	/* 384 */	"removexattr",
	/* 385 */	"lremovexattr",
	/* 386 */	"fremovexattr",
	/* 387 */	NULL, /* compat_50_netbsd32___stat30 */
	/* 388 */	NULL, /* compat_50_netbsd32___fstat30 */
	/* 389 */	NULL, /* compat_50_netbsd32___lstat30 */
	/* 390 */	"getdents",
	/* 391 */	NULL, /* ignored old posix fadvise */
	/* 392 */	NULL, /* compat_30_netbsd32___fhstat30 */
	/* 393 */	NULL, /* compat_50_netbsd32_ntp_gettime */
	/* 394 */	"socket",
	/* 395 */	"getfh",
	/* 396 */	"fhopen",
	/* 397 */	NULL, /* compat_90_netbsd32_fhstatvfs1 */
	/* 398 */	NULL, /* compat_50_netbsd32___fhstat40 */
	/* 399 */	NULL, /* unimplemented sys_aio_cancel */
	/* 400 */	NULL, /* unimplemented sys_aio_error */
	/* 401 */	NULL, /* unimplemented sys_aio_fsync */
	/* 402 */	NULL, /* unimplemented sys_aio_read */
	/* 403 */	NULL, /* unimplemented sys_aio_return */
	/* 404 */	NULL, /* unimplemented sys_aio_suspend */
	/* 405 */	NULL, /* unimplemented sys_aio_write */
	/* 406 */	NULL, /* unimplemented sys_lio_listio */
	/* 407 */	NULL, /* unimplemented */
	/* 408 */	NULL, /* unimplemented */
	/* 409 */	NULL, /* unimplemented */
	/* 410 */	"__mount50",
	/* 411 */	"mremap",
	/* 412 */	"pset_create",
	/* 413 */	"pset_destroy",
	/* 414 */	"pset_assign",
	/* 415 */	"_pset_bind",
	/* 416 */	"posix_fadvise",
	/* 417 */	"select",
	/* 418 */	"gettimeofday",
	/* 419 */	"settimeofday",
	/* 420 */	"utimes",
	/* 421 */	"adjtime",
	/* 422 */	NULL, /* excluded __lfs_segwait50 */
	/* 423 */	"futimes",
	/* 424 */	"lutimes",
	/* 425 */	"setitimer",
	/* 426 */	"getitimer",
	/* 427 */	"clock_gettime",
	/* 428 */	"clock_settime",
	/* 429 */	"clock_getres",
	/* 430 */	"nanosleep",
	/* 431 */	"__sigtimedwait",
	/* 432 */	"mq_timedsend",
	/* 433 */	"mq_timedreceive",
	/* 434 */	NULL, /* compat_60_netbsd32__lwp_park */
	/* 435 */	"kevent",
	/* 436 */	"pselect",
	/* 437 */	"pollts",
	/* 438 */	NULL, /* unimplemented netbsd32___aio_suspend50 */
	/* 439 */	"stat",
	/* 440 */	"fstat",
	/* 441 */	"lstat",
	/* 442 */	"__semctl",
	/* 443 */	"shmctl",
	/* 444 */	"msgctl",
	/* 445 */	"getrusage",
	/* 446 */	"timer_settime",
	/* 447 */	"timer_gettime",
#if defined(NTP) || !defined(_KERNEL_OPT)
	/* 448 */	"ntp_gettime",
#else
	/* 448 */	NULL, /* excluded ___ntp_gettime50 */
#endif
	/* 449 */	"wait4",
	/* 450 */	"mknod",
	/* 451 */	"fhstat",
	/* 452 */	NULL, /* obsolete 5.99 quotactl */
	/* 453 */	"pipe2",
	/* 454 */	"dup3",
	/* 455 */	"kqueue1",
	/* 456 */	"paccept",
	/* 457 */	"linkat",
	/* 458 */	"renameat",
	/* 459 */	"mkfifoat",
	/* 460 */	"mknodat",
	/* 461 */	"mkdirat",
	/* 462 */	"faccessat",
	/* 463 */	"fchmodat",
	/* 464 */	"fchownat",
	/* 465 */	"fexecve",
	/* 466 */	"fstatat",
	/* 467 */	"utimensat",
	/* 468 */	"openat",
	/* 469 */	"readlinkat",
	/* 470 */	"symlinkat",
	/* 471 */	"unlinkat",
	/* 472 */	"futimens",
	/* 473 */	"__quotactl",
	/* 474 */	"posix_spawn",
	/* 475 */	"recvmmsg",
	/* 476 */	"sendmmsg",
	/* 477 */	"clock_nanosleep",
	/* 478 */	"_lwp_park",
	/* 479 */	"posix_fallocate",
	/* 480 */	"fdiscard",
	/* 481 */	"wait6",
	/* 482 */	"clock_getcpuclockid2",
	/* 483 */	"getvfsstat",
	/* 484 */	"statvfs1",
	/* 485 */	"fstatvfs1",
	/* 486 */	"fhstatvfs1",
	/* 487 */	"__acl_get_link",
	/* 488 */	"__acl_set_link",
	/* 489 */	"__acl_delete_link",
	/* 490 */	"__acl_aclcheck_link",
	/* 491 */	"__acl_get_file",
	/* 492 */	"__acl_set_file",
	/* 493 */	"__acl_get_fd",
	/* 494 */	"__acl_set_fd",
	/* 495 */	"__acl_delete_file",
	/* 496 */	"__acl_delete_fd",
	/* 497 */	"__acl_aclcheck_file",
	/* 498 */	"__acl_aclcheck_fd",
	/* 499 */	"lpathconf",
	/* 500 */	NULL, /* filler */
	/* 501 */	NULL, /* filler */
	/* 502 */	NULL, /* filler */
	/* 503 */	NULL, /* filler */
	/* 504 */	NULL, /* filler */
	/* 505 */	NULL, /* filler */
	/* 506 */	NULL, /* filler */
	/* 507 */	NULL, /* filler */
	/* 508 */	NULL, /* filler */
	/* 509 */	NULL, /* filler */
	/* 510 */	NULL, /* filler */
	/* 511 */	NULL, /* filler */
};
