/* $NetBSD: linux32_syscalls.c,v 1.81 2020/04/26 19:20:58 thorpej Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.71 2020/04/26 18:53:33 thorpej Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: linux32_syscalls.c,v 1.81 2020/04/26 19:20:58 thorpej Exp $");

#if defined(_KERNEL_OPT)
#include <sys/param.h>
#include <sys/poll.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/syscallargs.h>
#include <machine/netbsd32_machdep.h>
#include <compat/netbsd32/netbsd32.h>
#include <compat/netbsd32/netbsd32_syscallargs.h>
#include <compat/linux/common/linux_types.h>
#include <compat/linux32/common/linux32_types.h>
#include <compat/linux32/common/linux32_signal.h>
#include <compat/linux32/arch/amd64/linux32_missing.h>
#include <compat/linux32/linux32_syscallargs.h>
#include <compat/linux/common/linux_mmap.h>
#include <compat/linux/common/linux_signal.h>
#include <compat/linux/common/linux_siginfo.h>
#include <compat/linux/common/linux_machdep.h>
#include <compat/linux/common/linux_ipc.h>
#include <compat/linux/common/linux_sem.h>
#include <compat/linux/linux_syscallargs.h>
#else /* _KERNEL_OPT */
#include <sys/null.h>
#endif /* _KERNEL_OPT */

const char *const linux32_syscallnames[] = {
	/*   0 */	"syscall",
	/*   1 */	"exit",
	/*   2 */	"fork",
	/*   3 */	"netbsd32_read",
	/*   4 */	"netbsd32_write",
	/*   5 */	"open",
	/*   6 */	"netbsd32_close",
	/*   7 */	"waitpid",
	/*   8 */	"creat",
	/*   9 */	"netbsd32_link",
	/*  10 */	"unlink",
	/*  11 */	"netbsd32_execve",
	/*  12 */	"netbsd32_chdir",
	/*  13 */	"time",
	/*  14 */	"mknod",
	/*  15 */	"netbsd32_chmod",
	/*  16 */	"lchown16",
	/*  17 */	"break",
	/*  18 */	"#18 (obsolete ostat)",
	/*  19 */	"compat_43_netbsd32_olseek",
	/*  20 */	"getpid",
	/*  21 */	"#21 (unimplemented mount)",
	/*  22 */	"#22 (unimplemented umount)",
	/*  23 */	"linux_setuid16",
	/*  24 */	"linux_getuid16",
	/*  25 */	"stime",
	/*  26 */	"ptrace",
	/*  27 */	"alarm",
	/*  28 */	"#28 (obsolete ofstat)",
	/*  29 */	"pause",
	/*  30 */	"utime",
	/*  31 */	"#31 (obsolete stty)",
	/*  32 */	"#32 (obsolete gtty)",
	/*  33 */	"netbsd32_access",
	/*  34 */	"nice",
	/*  35 */	"#35 (obsolete ftime)",
	/*  36 */	"sync",
	/*  37 */	"kill",
	/*  38 */	"netbsd32___posix_rename",
	/*  39 */	"netbsd32_mkdir",
	/*  40 */	"netbsd32_rmdir",
	/*  41 */	"netbsd32_dup",
	/*  42 */	"pipe",
	/*  43 */	"times",
	/*  44 */	"#44 (obsolete prof)",
	/*  45 */	"brk",
	/*  46 */	"linux_setgid16",
	/*  47 */	"linux_getgid16",
	/*  48 */	"signal",
	/*  49 */	"linux_geteuid16",
	/*  50 */	"linux_getegid16",
	/*  51 */	"netbsd32_acct",
	/*  52 */	"#52 (obsolete phys)",
	/*  53 */	"#53 (obsolete lock)",
	/*  54 */	"ioctl",
	/*  55 */	"fcntl",
	/*  56 */	"#56 (obsolete mpx)",
	/*  57 */	"netbsd32_setpgid",
	/*  58 */	"#58 (obsolete ulimit)",
	/*  59 */	"oldolduname",
	/*  60 */	"netbsd32_umask",
	/*  61 */	"netbsd32_chroot",
	/*  62 */	"#62 (unimplemented ustat)",
	/*  63 */	"netbsd32_dup2",
	/*  64 */	"getppid",
	/*  65 */	"getpgrp",
	/*  66 */	"setsid",
	/*  67 */	"#67 (unimplemented sigaction)",
	/*  68 */	"siggetmask",
	/*  69 */	"sigsetmask",
	/*  70 */	"setreuid16",
	/*  71 */	"setregid16",
	/*  72 */	"#72 (unimplemented sigsuspend)",
	/*  73 */	"#73 (unimplemented sigpending)",
	/*  74 */	"compat_43_netbsd32_osethostname",
	/*  75 */	"setrlimit",
	/*  76 */	"getrlimit",
	/*  77 */	"compat_50_netbsd32_getrusage",
	/*  78 */	"gettimeofday",
	/*  79 */	"settimeofday",
	/*  80 */	"getgroups16",
	/*  81 */	"setgroups16",
	/*  82 */	"oldselect",
	/*  83 */	"netbsd32_symlink",
	/*  84 */	"compat_43_netbsd32_lstat43",
	/*  85 */	"netbsd32_readlink",
	/*  86 */	"#86 (unimplemented uselib)",
	/*  87 */	"swapon",
	/*  88 */	"reboot",
	/*  89 */	"readdir",
	/*  90 */	"old_mmap",
	/*  91 */	"netbsd32_munmap",
	/*  92 */	"compat_43_netbsd32_otruncate",
	/*  93 */	"compat_43_netbsd32_oftruncate",
	/*  94 */	"netbsd32_fchmod",
	/*  95 */	"fchown16",
	/*  96 */	"getpriority",
	/*  97 */	"netbsd32_setpriority",
	/*  98 */	"netbsd32_profil",
	/*  99 */	"statfs",
	/* 100 */	"fstatfs",
	/* 101 */	"ioperm",
	/* 102 */	"socketcall",
	/* 103 */	"#103 (unimplemented syslog)",
	/* 104 */	"compat_50_netbsd32_setitimer",
	/* 105 */	"compat_50_netbsd32_getitimer",
	/* 106 */	"stat",
	/* 107 */	"lstat",
	/* 108 */	"fstat",
	/* 109 */	"olduname",
	/* 110 */	"iopl",
	/* 111 */	"#111 (unimplemented vhangup)",
	/* 112 */	"#112 (unimplemented idle)",
	/* 113 */	"#113 (unimplemented vm86old)",
	/* 114 */	"wait4",
	/* 115 */	"swapoff",
	/* 116 */	"sysinfo",
	/* 117 */	"ipc",
	/* 118 */	"netbsd32_fsync",
	/* 119 */	"sigreturn",
	/* 120 */	"clone",
	/* 121 */	"setdomainname",
	/* 122 */	"uname",
	/* 123 */	"modify_ldt",
	/* 124 */	"#124 (unimplemented adjtimex)",
	/* 125 */	"mprotect",
	/* 126 */	"#126 (unimplemented sigprocmask)",
	/* 127 */	"#127 (unimplemented create_module)",
	/* 128 */	"#128 (unimplemented init_module)",
	/* 129 */	"#129 (unimplemented delete_module)",
	/* 130 */	"#130 (unimplemented get_kernel_syms)",
	/* 131 */	"#131 (unimplemented quotactl)",
	/* 132 */	"netbsd32_getpgid",
	/* 133 */	"netbsd32_fchdir",
	/* 134 */	"#134 (unimplemented bdflush)",
	/* 135 */	"#135 (unimplemented sysfs)",
	/* 136 */	"personality",
	/* 137 */	"#137 (unimplemented afs_syscall)",
	/* 138 */	"setfsuid16",
	/* 139 */	"setfsgid16",
	/* 140 */	"llseek",
	/* 141 */	"getdents",
	/* 142 */	"select",
	/* 143 */	"netbsd32_flock",
	/* 144 */	"netbsd32___msync13",
	/* 145 */	"netbsd32_readv",
	/* 146 */	"netbsd32_writev",
	/* 147 */	"netbsd32_getsid",
	/* 148 */	"fdatasync",
	/* 149 */	"__sysctl",
	/* 150 */	"netbsd32_mlock",
	/* 151 */	"netbsd32_munlock",
	/* 152 */	"netbsd32_mlockall",
	/* 153 */	"munlockall",
	/* 154 */	"sched_setparam",
	/* 155 */	"sched_getparam",
	/* 156 */	"sched_setscheduler",
	/* 157 */	"sched_getscheduler",
	/* 158 */	"sched_yield",
	/* 159 */	"sched_get_priority_max",
	/* 160 */	"sched_get_priority_min",
	/* 161 */	"#161 (unimplemented sched_rr_get_interval)",
	/* 162 */	"nanosleep",
	/* 163 */	"mremap",
	/* 164 */	"setresuid16",
	/* 165 */	"getresuid16",
	/* 166 */	"#166 (unimplemented vm86)",
	/* 167 */	"#167 (unimplemented query_module)",
	/* 168 */	"netbsd32_poll",
	/* 169 */	"#169 (unimplemented nfsservctl)",
	/* 170 */	"setresgid16",
	/* 171 */	"getresgid16",
	/* 172 */	"#172 (unimplemented prctl)",
	/* 173 */	"rt_sigreturn",
	/* 174 */	"rt_sigaction",
	/* 175 */	"rt_sigprocmask",
	/* 176 */	"rt_sigpending",
	/* 177 */	"rt_sigtimedwait",
	/* 178 */	"rt_queueinfo",
	/* 179 */	"rt_sigsuspend",
	/* 180 */	"pread",
	/* 181 */	"pwrite",
	/* 182 */	"chown16",
	/* 183 */	"netbsd32___getcwd",
	/* 184 */	"#184 (unimplemented capget)",
	/* 185 */	"#185 (unimplemented capset)",
	/* 186 */	"#186 (unimplemented sigaltstack)",
	/* 187 */	"#187 (unimplemented sendfile)",
	/* 188 */	"#188 (unimplemented getpmsg)",
	/* 189 */	"#189 (unimplemented putpmsg)",
	/* 190 */	"__vfork14",
	/* 191 */	"ugetrlimit",
	/* 192 */	"mmap2",
	/* 193 */	"truncate64",
	/* 194 */	"ftruncate64",
	/* 195 */	"stat64",
	/* 196 */	"lstat64",
	/* 197 */	"fstat64",
	/* 198 */	"netbsd32___posix_lchown",
	/* 199 */	"getuid",
	/* 200 */	"getgid",
	/* 201 */	"geteuid",
	/* 202 */	"getegid",
	/* 203 */	"netbsd32_setreuid",
	/* 204 */	"netbsd32_setregid",
	/* 205 */	"netbsd32_getgroups",
	/* 206 */	"netbsd32_setgroups",
	/* 207 */	"netbsd32___posix_fchown",
	/* 208 */	"setresuid",
	/* 209 */	"getresuid",
	/* 210 */	"setresgid",
	/* 211 */	"getresgid",
	/* 212 */	"netbsd32___posix_chown",
	/* 213 */	"netbsd32_setuid",
	/* 214 */	"netbsd32_setgid",
	/* 215 */	"setfsuid",
	/* 216 */	"setfsgid",
	/* 217 */	"#217 (unimplemented pivot_root)",
	/* 218 */	"netbsd32_mincore",
	/* 219 */	"netbsd32_madvise",
	/* 220 */	"getdents64",
#define linux32_sys_fcntl64 linux32_sys_fcntl
#define linux32_sys_fcntl64_args linux32_sys_fcntl_args
	/* 221 */	"fcntl64",
	/* 222 */	"#222 (unimplemented / * unused * /)",
	/* 223 */	"#223 (unimplemented / * unused * /)",
	/* 224 */	"gettid",
	/* 225 */	"#225 (unimplemented readahead)",
	/* 226 */	"netbsd32_setxattr",
	/* 227 */	"netbsd32_lsetxattr",
	/* 228 */	"netbsd32_fsetxattr",
	/* 229 */	"netbsd32_getxattr",
	/* 230 */	"netbsd32_lgetxattr",
	/* 231 */	"netbsd32_fgetxattr",
	/* 232 */	"netbsd32_listxattr",
	/* 233 */	"netbsd32_llistxattr",
	/* 234 */	"netbsd32_flistxattr",
	/* 235 */	"netbsd32_removexattr",
	/* 236 */	"netbsd32_lremovexattr",
	/* 237 */	"netbsd32_fremovexattr",
	/* 238 */	"tkill",
	/* 239 */	"#239 (unimplemented sendfile64)",
	/* 240 */	"futex",
	/* 241 */	"sched_setaffinity",
	/* 242 */	"sched_getaffinity",
	/* 243 */	"set_thread_area",
	/* 244 */	"get_thread_area",
	/* 245 */	"#245 (unimplemented io_setup)",
	/* 246 */	"#246 (unimplemented io_destroy)",
	/* 247 */	"#247 (unimplemented io_getevents)",
	/* 248 */	"#248 (unimplemented io_submit)",
	/* 249 */	"#249 (unimplemented io_cancel)",
	/* 250 */	"fadvise64",
	/* 251 */	"#251 (unimplemented / * unused * /)",
	/* 252 */	"exit_group",
	/* 253 */	"#253 (unimplemented lookup_dcookie)",
	/* 254 */	"#254 (unimplemented epoll_create)",
	/* 255 */	"#255 (unimplemented epoll_ctl)",
	/* 256 */	"#256 (unimplemented epoll_wait)",
	/* 257 */	"#257 (unimplemented remap_file_pages)",
	/* 258 */	"set_tid_address",
	/* 259 */	"#259 (unimplemented timer_create)",
	/* 260 */	"#260 (unimplemented timer_settime)",
	/* 261 */	"#261 (unimplemented timer_gettime)",
	/* 262 */	"#262 (unimplemented timer_getoverrun)",
	/* 263 */	"#263 (unimplemented timer_delete)",
	/* 264 */	"clock_settime",
	/* 265 */	"clock_gettime",
	/* 266 */	"clock_getres",
	/* 267 */	"clock_nanosleep",
	/* 268 */	"statfs64",
	/* 269 */	"fstatfs64",
	/* 270 */	"tgkill",
	/* 271 */	"compat_50_netbsd32_utimes",
	/* 272 */	"fadvise64_64",
	/* 273 */	"#273 (unimplemented vserver)",
	/* 274 */	"#274 (unimplemented mbind)",
	/* 275 */	"#275 (unimplemented get_mempolicy)",
	/* 276 */	"#276 (unimplemented set_mempolicy)",
	/* 277 */	"#277 (unimplemented mq_open)",
	/* 278 */	"#278 (unimplemented mq_unlink)",
	/* 279 */	"#279 (unimplemented mq_timedsend)",
	/* 280 */	"#280 (unimplemented mq_timedreceive)",
	/* 281 */	"#281 (unimplemented mq_notify)",
	/* 282 */	"#282 (unimplemented mq_getsetattr)",
	/* 283 */	"#283 (unimplemented kexec_load)",
	/* 284 */	"#284 (unimplemented waitid)",
	/* 285 */	"#285 (unimplemented / * unused * /)",
	/* 286 */	"#286 (unimplemented add_key)",
	/* 287 */	"#287 (unimplemented request_key)",
	/* 288 */	"#288 (unimplemented keyctl)",
	/* 289 */	"#289 (unimplemented ioprio_set)",
	/* 290 */	"#290 (unimplemented ioprio_get)",
	/* 291 */	"#291 (unimplemented inotify_init)",
	/* 292 */	"#292 (unimplemented inotify_add_watch)",
	/* 293 */	"#293 (unimplemented inotify_rm_watch)",
	/* 294 */	"#294 (unimplemented migrate_pages)",
	/* 295 */	"openat",
	/* 296 */	"netbsd32_mkdirat",
	/* 297 */	"mknodat",
	/* 298 */	"fchownat",
	/* 299 */	"#299 (unimplemented futimesat)",
	/* 300 */	"fstatat64",
	/* 301 */	"unlinkat",
	/* 302 */	"netbsd32_renameat",
	/* 303 */	"linkat",
	/* 304 */	"netbsd32_symlinkat",
	/* 305 */	"netbsd32_readlinkat",
	/* 306 */	"fchmodat",
	/* 307 */	"faccessat",
	/* 308 */	"#308 (unimplemented pselect6)",
	/* 309 */	"ppoll",
	/* 310 */	"#310 (unimplemented unshare)",
	/* 311 */	"netbsd32___futex_set_robust_list",
	/* 312 */	"netbsd32___futex_get_robust_list",
	/* 313 */	"#313 (unimplemented splice)",
	/* 314 */	"#314 (unimplemented sync_file_range)",
	/* 315 */	"#315 (unimplemented tee)",
	/* 316 */	"#316 (unimplemented vmsplice)",
	/* 317 */	"#317 (unimplemented move_pages)",
	/* 318 */	"#318 (unimplemented getcpu)",
	/* 319 */	"#319 (unimplemented epoll_wait)",
	/* 320 */	"utimensat",
	/* 321 */	"#321 (unimplemented signalfd)",
	/* 322 */	"#322 (unimplemented timerfd_create)",
	/* 323 */	"#323 (unimplemented eventfd)",
	/* 324 */	"fallocate",
	/* 325 */	"#325 (unimplemented timerfd_settime)",
	/* 326 */	"#326 (unimplemented timerfd_gettime)",
	/* 327 */	"#327 (unimplemented signalfd4)",
	/* 328 */	"#328 (unimplemented eventfd2)",
	/* 329 */	"#329 (unimplemented epoll_create1)",
	/* 330 */	"dup3",
	/* 331 */	"pipe2",
	/* 332 */	"#332 (unimplemented inotify_init1)",
	/* 333 */	"#333 (unimplemented preadv)",
	/* 334 */	"#334 (unimplemented pwritev)",
	/* 335 */	"#335 (unimplemented rt_tgsigqueueinfo)",
	/* 336 */	"#336 (unimplemented perf_counter_open)",
	/* 337 */	"#337 (unimplemented recvmmsg)",
	/* 338 */	"#338 (unimplemented fanotify_init)",
	/* 339 */	"#339 (unimplemented fanotify_mark)",
	/* 340 */	"#340 (unimplemented prlimit64)",
	/* 341 */	"#341 (unimplemented name_to_handle_at)",
	/* 342 */	"#342 (unimplemented open_by_handle_at)",
	/* 343 */	"#343 (unimplemented clock_adjtime)",
	/* 344 */	"#344 (unimplemented syncfs)",
	/* 345 */	"#345 (unimplemented sendmmsg)",
	/* 346 */	"#346 (unimplemented setns)",
	/* 347 */	"#347 (unimplemented process_vm_readv)",
	/* 348 */	"#348 (unimplemented process_vm_writev)",
	/* 349 */	"#349 (unimplemented kcmp)",
	/* 350 */	"#350 (unimplemented finit_module)",
	/* 351 */	"# filler",
	/* 352 */	"# filler",
	/* 353 */	"# filler",
	/* 354 */	"# filler",
	/* 355 */	"# filler",
	/* 356 */	"# filler",
	/* 357 */	"# filler",
	/* 358 */	"# filler",
	/* 359 */	"# filler",
	/* 360 */	"# filler",
	/* 361 */	"# filler",
	/* 362 */	"# filler",
	/* 363 */	"# filler",
	/* 364 */	"# filler",
	/* 365 */	"# filler",
	/* 366 */	"# filler",
	/* 367 */	"# filler",
	/* 368 */	"# filler",
	/* 369 */	"# filler",
	/* 370 */	"# filler",
	/* 371 */	"# filler",
	/* 372 */	"# filler",
	/* 373 */	"# filler",
	/* 374 */	"# filler",
	/* 375 */	"# filler",
	/* 376 */	"# filler",
	/* 377 */	"# filler",
	/* 378 */	"# filler",
	/* 379 */	"# filler",
	/* 380 */	"# filler",
	/* 381 */	"# filler",
	/* 382 */	"# filler",
	/* 383 */	"# filler",
	/* 384 */	"# filler",
	/* 385 */	"# filler",
	/* 386 */	"# filler",
	/* 387 */	"# filler",
	/* 388 */	"# filler",
	/* 389 */	"# filler",
	/* 390 */	"# filler",
	/* 391 */	"# filler",
	/* 392 */	"# filler",
	/* 393 */	"# filler",
	/* 394 */	"# filler",
	/* 395 */	"# filler",
	/* 396 */	"# filler",
	/* 397 */	"# filler",
	/* 398 */	"# filler",
	/* 399 */	"# filler",
	/* 400 */	"# filler",
	/* 401 */	"# filler",
	/* 402 */	"# filler",
	/* 403 */	"# filler",
	/* 404 */	"# filler",
	/* 405 */	"# filler",
	/* 406 */	"# filler",
	/* 407 */	"# filler",
	/* 408 */	"# filler",
	/* 409 */	"# filler",
	/* 410 */	"# filler",
	/* 411 */	"# filler",
	/* 412 */	"# filler",
	/* 413 */	"# filler",
	/* 414 */	"# filler",
	/* 415 */	"# filler",
	/* 416 */	"# filler",
	/* 417 */	"# filler",
	/* 418 */	"# filler",
	/* 419 */	"# filler",
	/* 420 */	"# filler",
	/* 421 */	"# filler",
	/* 422 */	"# filler",
	/* 423 */	"# filler",
	/* 424 */	"# filler",
	/* 425 */	"# filler",
	/* 426 */	"# filler",
	/* 427 */	"# filler",
	/* 428 */	"# filler",
	/* 429 */	"# filler",
	/* 430 */	"# filler",
	/* 431 */	"# filler",
	/* 432 */	"# filler",
	/* 433 */	"# filler",
	/* 434 */	"# filler",
	/* 435 */	"# filler",
	/* 436 */	"# filler",
	/* 437 */	"# filler",
	/* 438 */	"# filler",
	/* 439 */	"# filler",
	/* 440 */	"# filler",
	/* 441 */	"# filler",
	/* 442 */	"# filler",
	/* 443 */	"# filler",
	/* 444 */	"# filler",
	/* 445 */	"# filler",
	/* 446 */	"# filler",
	/* 447 */	"# filler",
	/* 448 */	"# filler",
	/* 449 */	"# filler",
	/* 450 */	"# filler",
	/* 451 */	"# filler",
	/* 452 */	"# filler",
	/* 453 */	"# filler",
	/* 454 */	"# filler",
	/* 455 */	"# filler",
	/* 456 */	"# filler",
	/* 457 */	"# filler",
	/* 458 */	"# filler",
	/* 459 */	"# filler",
	/* 460 */	"# filler",
	/* 461 */	"# filler",
	/* 462 */	"# filler",
	/* 463 */	"# filler",
	/* 464 */	"# filler",
	/* 465 */	"# filler",
	/* 466 */	"# filler",
	/* 467 */	"# filler",
	/* 468 */	"# filler",
	/* 469 */	"# filler",
	/* 470 */	"# filler",
	/* 471 */	"# filler",
	/* 472 */	"# filler",
	/* 473 */	"# filler",
	/* 474 */	"# filler",
	/* 475 */	"# filler",
	/* 476 */	"# filler",
	/* 477 */	"# filler",
	/* 478 */	"# filler",
	/* 479 */	"# filler",
	/* 480 */	"# filler",
	/* 481 */	"# filler",
	/* 482 */	"# filler",
	/* 483 */	"# filler",
	/* 484 */	"# filler",
	/* 485 */	"# filler",
	/* 486 */	"# filler",
	/* 487 */	"# filler",
	/* 488 */	"# filler",
	/* 489 */	"# filler",
	/* 490 */	"# filler",
	/* 491 */	"# filler",
	/* 492 */	"# filler",
	/* 493 */	"# filler",
	/* 494 */	"# filler",
	/* 495 */	"# filler",
	/* 496 */	"# filler",
	/* 497 */	"# filler",
	/* 498 */	"# filler",
	/* 499 */	"# filler",
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
const char *const altlinux32_syscallnames[] = {
	/*   0 */	"nosys",
	/*   1 */	NULL, /* exit */
	/*   2 */	NULL, /* fork */
	/*   3 */	"read",
	/*   4 */	"write",
	/*   5 */	NULL, /* open */
	/*   6 */	"close",
	/*   7 */	NULL, /* waitpid */
	/*   8 */	NULL, /* creat */
	/*   9 */	"link",
	/*  10 */	NULL, /* unlink */
	/*  11 */	"execve",
	/*  12 */	"chdir",
	/*  13 */	NULL, /* time */
	/*  14 */	NULL, /* mknod */
	/*  15 */	"chmod",
	/*  16 */	NULL, /* lchown16 */
	/*  17 */	NULL, /* break */
	/*  18 */	NULL, /* obsolete ostat */
	/*  19 */	"olseek",
	/*  20 */	NULL, /* getpid */
	/*  21 */	NULL, /* unimplemented mount */
	/*  22 */	NULL, /* unimplemented umount */
	/*  23 */	"setuid",
	/*  24 */	"getuid",
	/*  25 */	NULL, /* stime */
	/*  26 */	NULL, /* ptrace */
	/*  27 */	NULL, /* alarm */
	/*  28 */	NULL, /* obsolete ofstat */
	/*  29 */	NULL, /* pause */
	/*  30 */	NULL, /* utime */
	/*  31 */	NULL, /* obsolete stty */
	/*  32 */	NULL, /* obsolete gtty */
	/*  33 */	"access",
	/*  34 */	NULL, /* nice */
	/*  35 */	NULL, /* obsolete ftime */
	/*  36 */	NULL, /* sync */
	/*  37 */	NULL, /* kill */
	/*  38 */	"__posix_rename",
	/*  39 */	"mkdir",
	/*  40 */	"rmdir",
	/*  41 */	"dup",
	/*  42 */	NULL, /* pipe */
	/*  43 */	NULL, /* times */
	/*  44 */	NULL, /* obsolete prof */
	/*  45 */	NULL, /* brk */
	/*  46 */	"setgid",
	/*  47 */	"getgid",
	/*  48 */	NULL, /* signal */
	/*  49 */	"geteuid",
	/*  50 */	"getegid",
	/*  51 */	"acct",
	/*  52 */	NULL, /* obsolete phys */
	/*  53 */	NULL, /* obsolete lock */
	/*  54 */	NULL, /* ioctl */
	/*  55 */	NULL, /* fcntl */
	/*  56 */	NULL, /* obsolete mpx */
	/*  57 */	"setpgid",
	/*  58 */	NULL, /* obsolete ulimit */
	/*  59 */	NULL, /* oldolduname */
	/*  60 */	"umask",
	/*  61 */	"chroot",
	/*  62 */	NULL, /* unimplemented ustat */
	/*  63 */	"dup2",
	/*  64 */	NULL, /* getppid */
	/*  65 */	NULL, /* getpgrp */
	/*  66 */	NULL, /* setsid */
	/*  67 */	NULL, /* unimplemented sigaction */
	/*  68 */	NULL, /* siggetmask */
	/*  69 */	NULL, /* sigsetmask */
	/*  70 */	NULL, /* setreuid16 */
	/*  71 */	NULL, /* setregid16 */
	/*  72 */	NULL, /* unimplemented sigsuspend */
	/*  73 */	NULL, /* unimplemented sigpending */
	/*  74 */	"osethostname",
	/*  75 */	NULL, /* setrlimit */
	/*  76 */	NULL, /* getrlimit */
	/*  77 */	"getrusage",
	/*  78 */	NULL, /* gettimeofday */
	/*  79 */	NULL, /* settimeofday */
	/*  80 */	NULL, /* getgroups16 */
	/*  81 */	NULL, /* setgroups16 */
	/*  82 */	NULL, /* oldselect */
	/*  83 */	"symlink",
	/*  84 */	"lstat43",
	/*  85 */	"readlink",
	/*  86 */	NULL, /* unimplemented uselib */
	/*  87 */	NULL, /* swapon */
	/*  88 */	NULL, /* reboot */
	/*  89 */	NULL, /* readdir */
	/*  90 */	NULL, /* old_mmap */
	/*  91 */	"munmap",
	/*  92 */	"otruncate",
	/*  93 */	"oftruncate",
	/*  94 */	"fchmod",
	/*  95 */	NULL, /* fchown16 */
	/*  96 */	NULL, /* getpriority */
	/*  97 */	"setpriority",
	/*  98 */	"profil",
	/*  99 */	NULL, /* statfs */
	/* 100 */	NULL, /* fstatfs */
	/* 101 */	NULL, /* ioperm */
	/* 102 */	NULL, /* socketcall */
	/* 103 */	NULL, /* unimplemented syslog */
	/* 104 */	"setitimer",
	/* 105 */	"getitimer",
	/* 106 */	NULL, /* stat */
	/* 107 */	NULL, /* lstat */
	/* 108 */	NULL, /* fstat */
	/* 109 */	NULL, /* olduname */
	/* 110 */	NULL, /* iopl */
	/* 111 */	NULL, /* unimplemented vhangup */
	/* 112 */	NULL, /* unimplemented idle */
	/* 113 */	NULL, /* unimplemented vm86old */
	/* 114 */	NULL, /* wait4 */
	/* 115 */	NULL, /* swapoff */
	/* 116 */	NULL, /* sysinfo */
	/* 117 */	NULL, /* ipc */
	/* 118 */	"fsync",
	/* 119 */	NULL, /* sigreturn */
	/* 120 */	NULL, /* clone */
	/* 121 */	NULL, /* setdomainname */
	/* 122 */	NULL, /* uname */
	/* 123 */	NULL, /* modify_ldt */
	/* 124 */	NULL, /* unimplemented adjtimex */
	/* 125 */	NULL, /* mprotect */
	/* 126 */	NULL, /* unimplemented sigprocmask */
	/* 127 */	NULL, /* unimplemented create_module */
	/* 128 */	NULL, /* unimplemented init_module */
	/* 129 */	NULL, /* unimplemented delete_module */
	/* 130 */	NULL, /* unimplemented get_kernel_syms */
	/* 131 */	NULL, /* unimplemented quotactl */
	/* 132 */	"getpgid",
	/* 133 */	"fchdir",
	/* 134 */	NULL, /* unimplemented bdflush */
	/* 135 */	NULL, /* unimplemented sysfs */
	/* 136 */	NULL, /* personality */
	/* 137 */	NULL, /* unimplemented afs_syscall */
	/* 138 */	"setfsuid",
	/* 139 */	"setfsgid",
	/* 140 */	NULL, /* llseek */
	/* 141 */	NULL, /* getdents */
	/* 142 */	NULL, /* select */
	/* 143 */	"flock",
	/* 144 */	"msync",
	/* 145 */	"readv",
	/* 146 */	"writev",
	/* 147 */	"getsid",
	/* 148 */	NULL, /* fdatasync */
	/* 149 */	NULL, /* __sysctl */
	/* 150 */	"mlock",
	/* 151 */	"munlock",
	/* 152 */	"mlockall",
	/* 153 */	NULL, /* munlockall */
	/* 154 */	NULL, /* sched_setparam */
	/* 155 */	NULL, /* sched_getparam */
	/* 156 */	NULL, /* sched_setscheduler */
	/* 157 */	NULL, /* sched_getscheduler */
	/* 158 */	NULL, /* sched_yield */
	/* 159 */	NULL, /* sched_get_priority_max */
	/* 160 */	NULL, /* sched_get_priority_min */
	/* 161 */	NULL, /* unimplemented sched_rr_get_interval */
	/* 162 */	NULL, /* nanosleep */
	/* 163 */	NULL, /* mremap */
	/* 164 */	NULL, /* setresuid16 */
	/* 165 */	NULL, /* getresuid16 */
	/* 166 */	NULL, /* unimplemented vm86 */
	/* 167 */	NULL, /* unimplemented query_module */
	/* 168 */	"poll",
	/* 169 */	NULL, /* unimplemented nfsservctl */
	/* 170 */	NULL, /* setresgid16 */
	/* 171 */	NULL, /* getresgid16 */
	/* 172 */	NULL, /* unimplemented prctl */
	/* 173 */	NULL, /* rt_sigreturn */
	/* 174 */	NULL, /* rt_sigaction */
	/* 175 */	NULL, /* rt_sigprocmask */
	/* 176 */	NULL, /* rt_sigpending */
	/* 177 */	NULL, /* rt_sigtimedwait */
	/* 178 */	NULL, /* rt_queueinfo */
	/* 179 */	NULL, /* rt_sigsuspend */
	/* 180 */	NULL, /* pread */
	/* 181 */	NULL, /* pwrite */
	/* 182 */	NULL, /* chown16 */
	/* 183 */	"__getcwd",
	/* 184 */	NULL, /* unimplemented capget */
	/* 185 */	NULL, /* unimplemented capset */
	/* 186 */	NULL, /* unimplemented sigaltstack */
	/* 187 */	NULL, /* unimplemented sendfile */
	/* 188 */	NULL, /* unimplemented getpmsg */
	/* 189 */	NULL, /* unimplemented putpmsg */
	/* 190 */	"vfork",
	/* 191 */	NULL, /* ugetrlimit */
	/* 192 */	NULL, /* mmap2 */
	/* 193 */	NULL, /* truncate64 */
	/* 194 */	NULL, /* ftruncate64 */
	/* 195 */	NULL, /* stat64 */
	/* 196 */	NULL, /* lstat64 */
	/* 197 */	NULL, /* fstat64 */
	/* 198 */	"__posix_lchown",
	/* 199 */	NULL, /* getuid */
	/* 200 */	NULL, /* getgid */
	/* 201 */	NULL, /* geteuid */
	/* 202 */	NULL, /* getegid */
	/* 203 */	"setreuid",
	/* 204 */	"setregid",
	/* 205 */	"getgroups",
	/* 206 */	"setgroups",
	/* 207 */	"__posix_fchown",
	/* 208 */	NULL, /* setresuid */
	/* 209 */	NULL, /* getresuid */
	/* 210 */	NULL, /* setresgid */
	/* 211 */	NULL, /* getresgid */
	/* 212 */	"__posix_chown",
	/* 213 */	"setuid",
	/* 214 */	"setgid",
	/* 215 */	NULL, /* setfsuid */
	/* 216 */	NULL, /* setfsgid */
	/* 217 */	NULL, /* unimplemented pivot_root */
	/* 218 */	"mincore",
	/* 219 */	"madvise",
	/* 220 */	NULL, /* getdents64 */
#define linux32_sys_fcntl64 linux32_sys_fcntl
#define linux32_sys_fcntl64_args linux32_sys_fcntl_args
	/* 221 */	NULL, /* fcntl64 */
	/* 222 */	NULL, /* unimplemented / * unused * / */
	/* 223 */	NULL, /* unimplemented / * unused * / */
	/* 224 */	NULL, /* gettid */
	/* 225 */	NULL, /* unimplemented readahead */
	/* 226 */	"setxattr",
	/* 227 */	"lsetxattr",
	/* 228 */	"fsetxattr",
	/* 229 */	"getxattr",
	/* 230 */	"lgetxattr",
	/* 231 */	"fgetxattr",
	/* 232 */	"listxattr",
	/* 233 */	"llistxattr",
	/* 234 */	"flistxattr",
	/* 235 */	"removexattr",
	/* 236 */	"lremovexattr",
	/* 237 */	"fremovexattr",
	/* 238 */	NULL, /* tkill */
	/* 239 */	NULL, /* unimplemented sendfile64 */
	/* 240 */	NULL, /* futex */
	/* 241 */	NULL, /* sched_setaffinity */
	/* 242 */	NULL, /* sched_getaffinity */
	/* 243 */	NULL, /* set_thread_area */
	/* 244 */	NULL, /* get_thread_area */
	/* 245 */	NULL, /* unimplemented io_setup */
	/* 246 */	NULL, /* unimplemented io_destroy */
	/* 247 */	NULL, /* unimplemented io_getevents */
	/* 248 */	NULL, /* unimplemented io_submit */
	/* 249 */	NULL, /* unimplemented io_cancel */
	/* 250 */	NULL, /* fadvise64 */
	/* 251 */	NULL, /* unimplemented / * unused * / */
	/* 252 */	NULL, /* exit_group */
	/* 253 */	NULL, /* unimplemented lookup_dcookie */
	/* 254 */	NULL, /* unimplemented epoll_create */
	/* 255 */	NULL, /* unimplemented epoll_ctl */
	/* 256 */	NULL, /* unimplemented epoll_wait */
	/* 257 */	NULL, /* unimplemented remap_file_pages */
	/* 258 */	NULL, /* set_tid_address */
	/* 259 */	NULL, /* unimplemented timer_create */
	/* 260 */	NULL, /* unimplemented timer_settime */
	/* 261 */	NULL, /* unimplemented timer_gettime */
	/* 262 */	NULL, /* unimplemented timer_getoverrun */
	/* 263 */	NULL, /* unimplemented timer_delete */
	/* 264 */	NULL, /* clock_settime */
	/* 265 */	NULL, /* clock_gettime */
	/* 266 */	NULL, /* clock_getres */
	/* 267 */	NULL, /* clock_nanosleep */
	/* 268 */	NULL, /* statfs64 */
	/* 269 */	NULL, /* fstatfs64 */
	/* 270 */	NULL, /* tgkill */
	/* 271 */	"utimes",
	/* 272 */	NULL, /* fadvise64_64 */
	/* 273 */	NULL, /* unimplemented vserver */
	/* 274 */	NULL, /* unimplemented mbind */
	/* 275 */	NULL, /* unimplemented get_mempolicy */
	/* 276 */	NULL, /* unimplemented set_mempolicy */
	/* 277 */	NULL, /* unimplemented mq_open */
	/* 278 */	NULL, /* unimplemented mq_unlink */
	/* 279 */	NULL, /* unimplemented mq_timedsend */
	/* 280 */	NULL, /* unimplemented mq_timedreceive */
	/* 281 */	NULL, /* unimplemented mq_notify */
	/* 282 */	NULL, /* unimplemented mq_getsetattr */
	/* 283 */	NULL, /* unimplemented kexec_load */
	/* 284 */	NULL, /* unimplemented waitid */
	/* 285 */	NULL, /* unimplemented / * unused * / */
	/* 286 */	NULL, /* unimplemented add_key */
	/* 287 */	NULL, /* unimplemented request_key */
	/* 288 */	NULL, /* unimplemented keyctl */
	/* 289 */	NULL, /* unimplemented ioprio_set */
	/* 290 */	NULL, /* unimplemented ioprio_get */
	/* 291 */	NULL, /* unimplemented inotify_init */
	/* 292 */	NULL, /* unimplemented inotify_add_watch */
	/* 293 */	NULL, /* unimplemented inotify_rm_watch */
	/* 294 */	NULL, /* unimplemented migrate_pages */
	/* 295 */	NULL, /* openat */
	/* 296 */	"mkdirat",
	/* 297 */	NULL, /* mknodat */
	/* 298 */	NULL, /* fchownat */
	/* 299 */	NULL, /* unimplemented futimesat */
	/* 300 */	NULL, /* fstatat64 */
	/* 301 */	NULL, /* unlinkat */
	/* 302 */	"renameat",
	/* 303 */	NULL, /* linkat */
	/* 304 */	"symlinkat",
	/* 305 */	"readlinkat",
	/* 306 */	NULL, /* fchmodat */
	/* 307 */	NULL, /* faccessat */
	/* 308 */	NULL, /* unimplemented pselect6 */
	/* 309 */	NULL, /* ppoll */
	/* 310 */	NULL, /* unimplemented unshare */
	/* 311 */	"__futex_set_robust_list",
	/* 312 */	"__futex_get_robust_list",
	/* 313 */	NULL, /* unimplemented splice */
	/* 314 */	NULL, /* unimplemented sync_file_range */
	/* 315 */	NULL, /* unimplemented tee */
	/* 316 */	NULL, /* unimplemented vmsplice */
	/* 317 */	NULL, /* unimplemented move_pages */
	/* 318 */	NULL, /* unimplemented getcpu */
	/* 319 */	NULL, /* unimplemented epoll_wait */
	/* 320 */	NULL, /* utimensat */
	/* 321 */	NULL, /* unimplemented signalfd */
	/* 322 */	NULL, /* unimplemented timerfd_create */
	/* 323 */	NULL, /* unimplemented eventfd */
	/* 324 */	NULL, /* fallocate */
	/* 325 */	NULL, /* unimplemented timerfd_settime */
	/* 326 */	NULL, /* unimplemented timerfd_gettime */
	/* 327 */	NULL, /* unimplemented signalfd4 */
	/* 328 */	NULL, /* unimplemented eventfd2 */
	/* 329 */	NULL, /* unimplemented epoll_create1 */
	/* 330 */	NULL, /* dup3 */
	/* 331 */	NULL, /* pipe2 */
	/* 332 */	NULL, /* unimplemented inotify_init1 */
	/* 333 */	NULL, /* unimplemented preadv */
	/* 334 */	NULL, /* unimplemented pwritev */
	/* 335 */	NULL, /* unimplemented rt_tgsigqueueinfo */
	/* 336 */	NULL, /* unimplemented perf_counter_open */
	/* 337 */	NULL, /* unimplemented recvmmsg */
	/* 338 */	NULL, /* unimplemented fanotify_init */
	/* 339 */	NULL, /* unimplemented fanotify_mark */
	/* 340 */	NULL, /* unimplemented prlimit64 */
	/* 341 */	NULL, /* unimplemented name_to_handle_at */
	/* 342 */	NULL, /* unimplemented open_by_handle_at */
	/* 343 */	NULL, /* unimplemented clock_adjtime */
	/* 344 */	NULL, /* unimplemented syncfs */
	/* 345 */	NULL, /* unimplemented sendmmsg */
	/* 346 */	NULL, /* unimplemented setns */
	/* 347 */	NULL, /* unimplemented process_vm_readv */
	/* 348 */	NULL, /* unimplemented process_vm_writev */
	/* 349 */	NULL, /* unimplemented kcmp */
	/* 350 */	NULL, /* unimplemented finit_module */
	/* 351 */	NULL, /* filler */
	/* 352 */	NULL, /* filler */
	/* 353 */	NULL, /* filler */
	/* 354 */	NULL, /* filler */
	/* 355 */	NULL, /* filler */
	/* 356 */	NULL, /* filler */
	/* 357 */	NULL, /* filler */
	/* 358 */	NULL, /* filler */
	/* 359 */	NULL, /* filler */
	/* 360 */	NULL, /* filler */
	/* 361 */	NULL, /* filler */
	/* 362 */	NULL, /* filler */
	/* 363 */	NULL, /* filler */
	/* 364 */	NULL, /* filler */
	/* 365 */	NULL, /* filler */
	/* 366 */	NULL, /* filler */
	/* 367 */	NULL, /* filler */
	/* 368 */	NULL, /* filler */
	/* 369 */	NULL, /* filler */
	/* 370 */	NULL, /* filler */
	/* 371 */	NULL, /* filler */
	/* 372 */	NULL, /* filler */
	/* 373 */	NULL, /* filler */
	/* 374 */	NULL, /* filler */
	/* 375 */	NULL, /* filler */
	/* 376 */	NULL, /* filler */
	/* 377 */	NULL, /* filler */
	/* 378 */	NULL, /* filler */
	/* 379 */	NULL, /* filler */
	/* 380 */	NULL, /* filler */
	/* 381 */	NULL, /* filler */
	/* 382 */	NULL, /* filler */
	/* 383 */	NULL, /* filler */
	/* 384 */	NULL, /* filler */
	/* 385 */	NULL, /* filler */
	/* 386 */	NULL, /* filler */
	/* 387 */	NULL, /* filler */
	/* 388 */	NULL, /* filler */
	/* 389 */	NULL, /* filler */
	/* 390 */	NULL, /* filler */
	/* 391 */	NULL, /* filler */
	/* 392 */	NULL, /* filler */
	/* 393 */	NULL, /* filler */
	/* 394 */	NULL, /* filler */
	/* 395 */	NULL, /* filler */
	/* 396 */	NULL, /* filler */
	/* 397 */	NULL, /* filler */
	/* 398 */	NULL, /* filler */
	/* 399 */	NULL, /* filler */
	/* 400 */	NULL, /* filler */
	/* 401 */	NULL, /* filler */
	/* 402 */	NULL, /* filler */
	/* 403 */	NULL, /* filler */
	/* 404 */	NULL, /* filler */
	/* 405 */	NULL, /* filler */
	/* 406 */	NULL, /* filler */
	/* 407 */	NULL, /* filler */
	/* 408 */	NULL, /* filler */
	/* 409 */	NULL, /* filler */
	/* 410 */	NULL, /* filler */
	/* 411 */	NULL, /* filler */
	/* 412 */	NULL, /* filler */
	/* 413 */	NULL, /* filler */
	/* 414 */	NULL, /* filler */
	/* 415 */	NULL, /* filler */
	/* 416 */	NULL, /* filler */
	/* 417 */	NULL, /* filler */
	/* 418 */	NULL, /* filler */
	/* 419 */	NULL, /* filler */
	/* 420 */	NULL, /* filler */
	/* 421 */	NULL, /* filler */
	/* 422 */	NULL, /* filler */
	/* 423 */	NULL, /* filler */
	/* 424 */	NULL, /* filler */
	/* 425 */	NULL, /* filler */
	/* 426 */	NULL, /* filler */
	/* 427 */	NULL, /* filler */
	/* 428 */	NULL, /* filler */
	/* 429 */	NULL, /* filler */
	/* 430 */	NULL, /* filler */
	/* 431 */	NULL, /* filler */
	/* 432 */	NULL, /* filler */
	/* 433 */	NULL, /* filler */
	/* 434 */	NULL, /* filler */
	/* 435 */	NULL, /* filler */
	/* 436 */	NULL, /* filler */
	/* 437 */	NULL, /* filler */
	/* 438 */	NULL, /* filler */
	/* 439 */	NULL, /* filler */
	/* 440 */	NULL, /* filler */
	/* 441 */	NULL, /* filler */
	/* 442 */	NULL, /* filler */
	/* 443 */	NULL, /* filler */
	/* 444 */	NULL, /* filler */
	/* 445 */	NULL, /* filler */
	/* 446 */	NULL, /* filler */
	/* 447 */	NULL, /* filler */
	/* 448 */	NULL, /* filler */
	/* 449 */	NULL, /* filler */
	/* 450 */	NULL, /* filler */
	/* 451 */	NULL, /* filler */
	/* 452 */	NULL, /* filler */
	/* 453 */	NULL, /* filler */
	/* 454 */	NULL, /* filler */
	/* 455 */	NULL, /* filler */
	/* 456 */	NULL, /* filler */
	/* 457 */	NULL, /* filler */
	/* 458 */	NULL, /* filler */
	/* 459 */	NULL, /* filler */
	/* 460 */	NULL, /* filler */
	/* 461 */	NULL, /* filler */
	/* 462 */	NULL, /* filler */
	/* 463 */	NULL, /* filler */
	/* 464 */	NULL, /* filler */
	/* 465 */	NULL, /* filler */
	/* 466 */	NULL, /* filler */
	/* 467 */	NULL, /* filler */
	/* 468 */	NULL, /* filler */
	/* 469 */	NULL, /* filler */
	/* 470 */	NULL, /* filler */
	/* 471 */	NULL, /* filler */
	/* 472 */	NULL, /* filler */
	/* 473 */	NULL, /* filler */
	/* 474 */	NULL, /* filler */
	/* 475 */	NULL, /* filler */
	/* 476 */	NULL, /* filler */
	/* 477 */	NULL, /* filler */
	/* 478 */	NULL, /* filler */
	/* 479 */	NULL, /* filler */
	/* 480 */	NULL, /* filler */
	/* 481 */	NULL, /* filler */
	/* 482 */	NULL, /* filler */
	/* 483 */	NULL, /* filler */
	/* 484 */	NULL, /* filler */
	/* 485 */	NULL, /* filler */
	/* 486 */	NULL, /* filler */
	/* 487 */	NULL, /* filler */
	/* 488 */	NULL, /* filler */
	/* 489 */	NULL, /* filler */
	/* 490 */	NULL, /* filler */
	/* 491 */	NULL, /* filler */
	/* 492 */	NULL, /* filler */
	/* 493 */	NULL, /* filler */
	/* 494 */	NULL, /* filler */
	/* 495 */	NULL, /* filler */
	/* 496 */	NULL, /* filler */
	/* 497 */	NULL, /* filler */
	/* 498 */	NULL, /* filler */
	/* 499 */	NULL, /* filler */
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
