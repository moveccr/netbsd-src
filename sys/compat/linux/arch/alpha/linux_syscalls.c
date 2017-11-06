/* $NetBSD: linux_syscalls.c,v 1.107 2017/05/10 06:19:48 riastradh Exp $ */

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.94 2017/02/03 16:17:08 christos Exp
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: linux_syscalls.c,v 1.107 2017/05/10 06:19:48 riastradh Exp $");

#if defined(_KERNEL_OPT)
#if defined(_KERNEL_OPT)
#include "opt_sysv.h"
#endif
#include <sys/param.h>
#include <sys/poll.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/syscallargs.h>
#include <compat/linux/common/linux_types.h>
#include <compat/linux/common/linux_signal.h>
#include <compat/linux/common/linux_siginfo.h>
#include <compat/linux/common/linux_ipc.h>
#include <compat/linux/common/linux_msg.h>
#include <compat/linux/common/linux_sem.h>
#include <compat/linux/common/linux_shm.h>
#include <compat/linux/common/linux_mmap.h>
#include <compat/linux/linux_syscallargs.h>
#else /* _KERNEL_OPT */
#include <sys/null.h>
#endif /* _KERNEL_OPT */

const char *const linux_syscallnames[] = {
	/*   0 */	"syscall",
	/*   1 */	"exit",
	/*   2 */	"fork",
	/*   3 */	"read",
	/*   4 */	"write",
	/*   5 */	"#5 (unimplemented)",
	/*   6 */	"close",
	/*   7 */	"wait4",
	/*   8 */	"creat",
	/*   9 */	"link",
	/*  10 */	"unlink",
	/*  11 */	"#11 (unimplemented)",
	/*  12 */	"chdir",
	/*  13 */	"fchdir",
	/*  14 */	"mknod",
	/*  15 */	"chmod",
	/*  16 */	"__posix_chown",
	/*  17 */	"brk",
	/*  18 */	"#18 (unimplemented)",
	/*  19 */	"lseek",
	/*  20 */	"getpid_with_ppid",
	/*  21 */	"mount",
	/*  22 */	"#22 (unimplemented umount)",
	/*  23 */	"setuid",
	/*  24 */	"getuid_with_euid",
	/*  25 */	"#25 (unimplemented)",
	/*  26 */	"ptrace",
	/*  27 */	"#27 (unimplemented)",
	/*  28 */	"#28 (unimplemented)",
	/*  29 */	"#29 (unimplemented)",
	/*  30 */	"#30 (unimplemented)",
	/*  31 */	"#31 (unimplemented)",
	/*  32 */	"#32 (unimplemented)",
	/*  33 */	"access",
	/*  34 */	"#34 (unimplemented)",
	/*  35 */	"#35 (unimplemented)",
	/*  36 */	"sync",
	/*  37 */	"kill",
	/*  38 */	"#38 (unimplemented)",
	/*  39 */	"setpgid",
	/*  40 */	"#40 (unimplemented)",
	/*  41 */	"dup",
	/*  42 */	"pipe",
	/*  43 */	"set_program_attributes",
	/*  44 */	"#44 (unimplemented)",
	/*  45 */	"open",
	/*  46 */	"#46 (unimplemented)",
	/*  47 */	"getgid_with_egid",
	/*  48 */	"sigprocmask",
	/*  49 */	"#49 (unimplemented)",
	/*  50 */	"#50 (unimplemented)",
	/*  51 */	"acct",
	/*  52 */	"sigpending",
	/*  53 */	"#53 (unimplemented)",
	/*  54 */	"ioctl",
	/*  55 */	"#55 (unimplemented)",
	/*  56 */	"#56 (unimplemented)",
	/*  57 */	"symlink",
	/*  58 */	"readlink",
	/*  59 */	"execve",
	/*  60 */	"umask",
	/*  61 */	"chroot",
	/*  62 */	"#62 (unimplemented)",
	/*  63 */	"getpgrp",
	/*  64 */	"getpagesize",
	/*  65 */	"#65 (unimplemented)",
	/*  66 */	"__vfork14",
	/*  67 */	"stat",
	/*  68 */	"lstat",
	/*  69 */	"#69 (unimplemented)",
	/*  70 */	"#70 (unimplemented)",
	/*  71 */	"mmap",
	/*  72 */	"#72 (unimplemented)",
	/*  73 */	"munmap",
	/*  74 */	"mprotect",
	/*  75 */	"madvise",
	/*  76 */	"#76 (unimplemented vhangup)",
	/*  77 */	"#77 (unimplemented)",
	/*  78 */	"#78 (unimplemented)",
	/*  79 */	"getgroups",
	/*  80 */	"setgroups",
	/*  81 */	"#81 (unimplemented)",
	/*  82 */	"#82 (unimplemented setpgrp)",
	/*  83 */	"setitimer",
	/*  84 */	"#84 (unimplemented)",
	/*  85 */	"#85 (unimplemented)",
	/*  86 */	"#86 (unimplemented osf1_sys_getitimer)",
	/*  87 */	"gethostname",
	/*  88 */	"sethostname",
	/*  89 */	"#89 (unimplemented getdtablesize)",
	/*  90 */	"dup2",
	/*  91 */	"fstat",
	/*  92 */	"fcntl",
	/*  93 */	"select",
	/*  94 */	"poll",
	/*  95 */	"fsync",
	/*  96 */	"setpriority",
	/*  97 */	"socket",
	/*  98 */	"connect",
	/*  99 */	"accept",
	/* 100 */	"getpriority",
	/* 101 */	"send",
	/* 102 */	"recv",
	/* 103 */	"sigreturn",
	/* 104 */	"bind",
	/* 105 */	"setsockopt",
	/* 106 */	"listen",
	/* 107 */	"#107 (unimplemented)",
	/* 108 */	"#108 (unimplemented)",
	/* 109 */	"#109 (unimplemented)",
	/* 110 */	"#110 (unimplemented)",
	/* 111 */	"sigsuspend",
	/* 112 */	"sigstack",
	/* 113 */	"recvmsg",
	/* 114 */	"sendmsg",
	/* 115 */	"#115 (unimplemented)",
	/* 116 */	"gettimeofday",
	/* 117 */	"getrusage",
	/* 118 */	"getsockopt",
	/* 119 */	"#119 (unimplemented)",
	/* 120 */	"readv",
	/* 121 */	"writev",
	/* 122 */	"settimeofday",
	/* 123 */	"__posix_fchown",
	/* 124 */	"fchmod",
	/* 125 */	"recvfrom",
	/* 126 */	"setreuid",
	/* 127 */	"setregid",
	/* 128 */	"__posix_rename",
	/* 129 */	"truncate",
	/* 130 */	"ftruncate",
	/* 131 */	"flock",
	/* 132 */	"setgid",
	/* 133 */	"sendto",
	/* 134 */	"shutdown",
	/* 135 */	"socketpair",
	/* 136 */	"mkdir",
	/* 137 */	"rmdir",
	/* 138 */	"utimes",
	/* 139 */	"#139 (unimplemented)",
	/* 140 */	"#140 (unimplemented)",
	/* 141 */	"getpeername",
	/* 142 */	"#142 (unimplemented)",
	/* 143 */	"#143 (unimplemented)",
	/* 144 */	"getrlimit",
	/* 145 */	"setrlimit",
	/* 146 */	"#146 (unimplemented)",
	/* 147 */	"setsid",
	/* 148 */	"#148 (unimplemented quotactl)",
	/* 149 */	"#149 (unimplemented)",
	/* 150 */	"getsockname",
	/* 151 */	"#151 (unimplemented)",
	/* 152 */	"#152 (unimplemented)",
	/* 153 */	"#153 (unimplemented)",
	/* 154 */	"#154 (unimplemented)",
	/* 155 */	"#155 (unimplemented)",
	/* 156 */	"sigaction",
	/* 157 */	"#157 (unimplemented)",
	/* 158 */	"#158 (unimplemented)",
	/* 159 */	"getdirentries",
	/* 160 */	"statfs",
	/* 161 */	"fstatfs",
	/* 162 */	"#162 (unimplemented)",
	/* 163 */	"#163 (unimplemented)",
	/* 164 */	"#164 (unimplemented)",
	/* 165 */	"getdomainname",
	/* 166 */	"setdomainname",
	/* 167 */	"#167 (unimplemented)",
	/* 168 */	"#168 (unimplemented)",
	/* 169 */	"#169 (unimplemented)",
	/* 170 */	"#170 (unimplemented)",
	/* 171 */	"#171 (unimplemented)",
	/* 172 */	"#172 (unimplemented)",
	/* 173 */	"#173 (unimplemented)",
	/* 174 */	"#174 (unimplemented)",
	/* 175 */	"#175 (unimplemented)",
	/* 176 */	"#176 (unimplemented)",
	/* 177 */	"#177 (unimplemented)",
	/* 178 */	"#178 (unimplemented)",
	/* 179 */	"#179 (unimplemented)",
	/* 180 */	"#180 (unimplemented)",
	/* 181 */	"#181 (unimplemented)",
	/* 182 */	"#182 (unimplemented)",
	/* 183 */	"#183 (unimplemented)",
	/* 184 */	"#184 (unimplemented)",
	/* 185 */	"#185 (unimplemented)",
	/* 186 */	"#186 (unimplemented)",
	/* 187 */	"#187 (unimplemented)",
	/* 188 */	"#188 (unimplemented)",
	/* 189 */	"#189 (unimplemented)",
	/* 190 */	"#190 (unimplemented)",
	/* 191 */	"ugetrlimit",
	/* 192 */	"#192 (unimplemented mmap2)",
	/* 193 */	"#193 (unimplemented)",
	/* 194 */	"#194 (unimplemented)",
	/* 195 */	"#195 (unimplemented)",
	/* 196 */	"#196 (unimplemented)",
	/* 197 */	"#197 (unimplemented)",
	/* 198 */	"#198 (unimplemented)",
	/* 199 */	"#199 (unimplemented osf1_sys_swapon)",
#ifdef SYSVMSG
	/* 200 */	"msgctl",
	/* 201 */	"msgget",
	/* 202 */	"msgrcv",
	/* 203 */	"msgsnd",
#else
	/* 200 */	"#200 (unimplemented msgctl)",
	/* 201 */	"#201 (unimplemented msgget)",
	/* 202 */	"#202 (unimplemented msgrcv)",
	/* 203 */	"#203 (unimplemented msgsnd)",
#endif
#ifdef SYSVSEM
	/* 204 */	"semctl",
	/* 205 */	"semget",
	/* 206 */	"semop",
#else
	/* 204 */	"#204 (unimplemented semctl)",
	/* 205 */	"#205 (unimplemented semget)",
	/* 206 */	"#206 (unimplemented semop)",
#endif
	/* 207 */	"olduname",
	/* 208 */	"__posix_lchown",
#ifdef SYSVSHM
	/* 209 */	"shmat",
	/* 210 */	"shmctl",
	/* 211 */	"shmdt",
	/* 212 */	"shmget",
#else
	/* 209 */	"#209 (unimplemented shmat)",
	/* 210 */	"#210 (unimplemented shmctl)",
	/* 211 */	"#211 (unimplemented shmdt)",
	/* 212 */	"#212 (unimplemented shmget)",
#endif
	/* 213 */	"#213 (unimplemented)",
	/* 214 */	"#214 (unimplemented)",
	/* 215 */	"#215 (unimplemented)",
	/* 216 */	"#216 (unimplemented)",
	/* 217 */	"__msync13",
	/* 218 */	"#218 (unimplemented)",
	/* 219 */	"#219 (unimplemented)",
	/* 220 */	"#220 (unimplemented)",
	/* 221 */	"#221 (unimplemented)",
	/* 222 */	"#222 (unimplemented)",
	/* 223 */	"#223 (unimplemented)",
	/* 224 */	"#224 (unimplemented)",
	/* 225 */	"#225 (unimplemented)",
	/* 226 */	"#226 (unimplemented)",
	/* 227 */	"#227 (unimplemented)",
	/* 228 */	"#228 (unimplemented)",
	/* 229 */	"#229 (unimplemented)",
	/* 230 */	"#230 (unimplemented)",
	/* 231 */	"#231 (unimplemented)",
	/* 232 */	"#232 (unimplemented)",
	/* 233 */	"getpgid",
	/* 234 */	"getsid",
	/* 235 */	"#235 (unimplemented sigaltstack)",
	/* 236 */	"#236 (unimplemented)",
	/* 237 */	"#237 (unimplemented)",
	/* 238 */	"#238 (unimplemented)",
	/* 239 */	"#239 (unimplemented)",
	/* 240 */	"#240 (unimplemented)",
	/* 241 */	"sysinfo",
	/* 242 */	"#242 (unimplemented)",
	/* 243 */	"#243 (unimplemented)",
	/* 244 */	"#244 (unimplemented osf1_sys_proplist_syscall)",
	/* 245 */	"#245 (unimplemented)",
	/* 246 */	"#246 (unimplemented)",
	/* 247 */	"#247 (unimplemented)",
	/* 248 */	"#248 (unimplemented)",
	/* 249 */	"#249 (unimplemented)",
	/* 250 */	"#250 (unimplemented)",
	/* 251 */	"usleep_thread",
	/* 252 */	"#252 (unimplemented)",
	/* 253 */	"#253 (unimplemented)",
	/* 254 */	"#254 (unimplemented)",
	/* 255 */	"#255 (unimplemented sysfs)",
	/* 256 */	"getsysinfo",
	/* 257 */	"setsysinfo",
	/* 258 */	"#258 (unimplemented)",
	/* 259 */	"#259 (unimplemented)",
	/* 260 */	"#260 (unimplemented)",
	/* 261 */	"fdatasync",
	/* 262 */	"#262 (unimplemented)",
	/* 263 */	"#263 (unimplemented)",
	/* 264 */	"#264 (unimplemented)",
	/* 265 */	"#265 (unimplemented)",
	/* 266 */	"#266 (unimplemented)",
	/* 267 */	"#267 (unimplemented)",
	/* 268 */	"#268 (unimplemented)",
	/* 269 */	"#269 (unimplemented)",
	/* 270 */	"#270 (unimplemented)",
	/* 271 */	"#271 (unimplemented)",
	/* 272 */	"#272 (unimplemented)",
	/* 273 */	"#273 (unimplemented)",
	/* 274 */	"#274 (unimplemented)",
	/* 275 */	"#275 (unimplemented)",
	/* 276 */	"#276 (unimplemented)",
	/* 277 */	"#277 (unimplemented)",
	/* 278 */	"#278 (unimplemented)",
	/* 279 */	"#279 (unimplemented)",
	/* 280 */	"#280 (unimplemented)",
	/* 281 */	"#281 (unimplemented)",
	/* 282 */	"#282 (unimplemented)",
	/* 283 */	"#283 (unimplemented)",
	/* 284 */	"#284 (unimplemented)",
	/* 285 */	"#285 (unimplemented)",
	/* 286 */	"#286 (unimplemented)",
	/* 287 */	"#287 (unimplemented)",
	/* 288 */	"#288 (unimplemented)",
	/* 289 */	"#289 (unimplemented)",
	/* 290 */	"#290 (unimplemented)",
	/* 291 */	"#291 (unimplemented)",
	/* 292 */	"#292 (unimplemented)",
	/* 293 */	"#293 (unimplemented)",
	/* 294 */	"#294 (unimplemented)",
	/* 295 */	"#295 (unimplemented)",
	/* 296 */	"#296 (unimplemented)",
	/* 297 */	"#297 (unimplemented)",
	/* 298 */	"#298 (unimplemented)",
	/* 299 */	"#299 (unimplemented)",
	/* 300 */	"#300 (unimplemented bdflush)",
	/* 301 */	"#301 (unimplemented sethae)",
	/* 302 */	"#302 (unimplemented mount)",
	/* 303 */	"#303 (unimplemented old_adjtimex)",
	/* 304 */	"swapoff",
	/* 305 */	"getdents",
	/* 306 */	"#306 (unimplemented alpha_create_module)",
	/* 307 */	"#307 (unimplemented init_module)",
	/* 308 */	"#308 (unimplemented delete_module)",
	/* 309 */	"#309 (unimplemented get_kernel_syms)",
	/* 310 */	"#310 (unimplemented syslog)",
	/* 311 */	"reboot",
	/* 312 */	"clone",
#ifdef EXEC_AOUT
	/* 313 */	"uselib",
#else
	/* 313 */	"#313 (unimplemented sys_uselib)",
#endif
	/* 314 */	"mlock",
	/* 315 */	"munlock",
	/* 316 */	"mlockall",
	/* 317 */	"munlockall",
	/* 318 */	"sysinfo",
	/* 319 */	"__sysctl",
	/* 320 */	"#320 (unimplemented idle)",
	/* 321 */	"#321 (unimplemented oldumount)",
	/* 322 */	"swapon",
	/* 323 */	"times",
	/* 324 */	"personality",
	/* 325 */	"setfsuid",
	/* 326 */	"setfsgid",
	/* 327 */	"#327 (unimplemented ustat)",
	/* 328 */	"statfs",
	/* 329 */	"fstatfs",
	/* 330 */	"sched_setparam",
	/* 331 */	"sched_getparam",
	/* 332 */	"sched_setscheduler",
	/* 333 */	"sched_getscheduler",
	/* 334 */	"sched_yield",
	/* 335 */	"sched_get_priority_max",
	/* 336 */	"sched_get_priority_min",
	/* 337 */	"#337 (unimplemented sched_rr_get_interval)",
	/* 338 */	"#338 (unimplemented afs_syscall)",
	/* 339 */	"uname",
	/* 340 */	"nanosleep",
	/* 341 */	"mremap",
	/* 342 */	"#342 (unimplemented nfsservctl)",
	/* 343 */	"setresuid",
	/* 344 */	"getresuid",
	/* 345 */	"#345 (unimplemented pciconfig_read)",
	/* 346 */	"#346 (unimplemented pciconfig_write)",
	/* 347 */	"#347 (unimplemented query_module)",
	/* 348 */	"#348 (unimplemented prctl)",
	/* 349 */	"pread",
	/* 350 */	"pwrite",
	/* 351 */	"rt_sigreturn",
	/* 352 */	"rt_sigaction",
	/* 353 */	"rt_sigprocmask",
	/* 354 */	"rt_sigpending",
	/* 355 */	"rt_sigtimedwait",
	/* 356 */	"rt_queueinfo",
	/* 357 */	"rt_sigsuspend",
	/* 358 */	"select",
	/* 359 */	"gettimeofday",
	/* 360 */	"settimeofday",
	/* 361 */	"getitimer",
	/* 362 */	"setitimer",
	/* 363 */	"utimes",
	/* 364 */	"getrusage",
	/* 365 */	"wait4",
	/* 366 */	"#366 (unimplemented adjtimex)",
	/* 367 */	"__getcwd",
	/* 368 */	"#368 (unimplemented capget)",
	/* 369 */	"#369 (unimplemented capset)",
	/* 370 */	"#370 (unimplemented sendfile)",
	/* 371 */	"#371 (unimplemented setresgid)",
	/* 372 */	"#372 (unimplemented getresgid)",
	/* 373 */	"#373 (unimplemented sys_dipc)",
	/* 374 */	"#374 (unimplemented pivot_root)",
	/* 375 */	"mincore",
	/* 376 */	"#376 (unimplemented pciconfig_iobase)",
	/* 377 */	"getdents64",
	/* 378 */	"gettid",
	/* 379 */	"#379 (unimplemented readahead)",
	/* 380 */	"#380 (unimplemented / * unused * /)",
	/* 381 */	"tkill",
	/* 382 */	"setxattr",
	/* 383 */	"lsetxattr",
	/* 384 */	"fsetxattr",
	/* 385 */	"getxattr",
	/* 386 */	"lgetxattr",
	/* 387 */	"fgetxattr",
	/* 388 */	"listxattr",
	/* 389 */	"llistxattr",
	/* 390 */	"flistxattr",
	/* 391 */	"removexattr",
	/* 392 */	"lremovexattr",
	/* 393 */	"fremovexattr",
	/* 394 */	"futex",
	/* 395 */	"sched_setaffinity",
	/* 396 */	"sched_getaffinity",
	/* 397 */	"#397 (unimplemented tuxcall)",
	/* 398 */	"#398 (unimplemented io_setup)",
	/* 399 */	"#399 (unimplemented io_destroy)",
	/* 400 */	"#400 (unimplemented io_getevents)",
	/* 401 */	"#401 (unimplemented io_submit)",
	/* 402 */	"#402 (unimplemented io_cancel)",
	/* 403 */	"#403 (unimplemented / * unused * /)",
	/* 404 */	"#404 (unimplemented / * unused * /)",
	/* 405 */	"exit_group",
	/* 406 */	"#406 (unimplemented lookup_dcookie)",
	/* 407 */	"#407 (unimplemented sys_epoll_create)",
	/* 408 */	"#408 (unimplemented sys_epoll_ctl)",
	/* 409 */	"#409 (unimplemented sys_epoll_wait)",
	/* 410 */	"#410 (unimplemented remap_file_pages)",
	/* 411 */	"set_tid_address",
	/* 412 */	"#412 (unimplemented restart_syscall)",
	/* 413 */	"fadvise64",
	/* 414 */	"#414 (unimplemented timer_create)",
	/* 415 */	"#415 (unimplemented timer_settime)",
	/* 416 */	"#416 (unimplemented timer_gettime)",
	/* 417 */	"#417 (unimplemented timer_getoverrun)",
	/* 418 */	"#418 (unimplemented timer_delete)",
	/* 419 */	"clock_settime",
	/* 420 */	"clock_gettime",
	/* 421 */	"clock_getres",
	/* 422 */	"clock_nanosleep",
	/* 423 */	"#423 (unimplemented semtimedop)",
	/* 424 */	"tgkill",
	/* 425 */	"stat64",
	/* 426 */	"lstat64",
	/* 427 */	"fstat64",
	/* 428 */	"#428 (unimplemented vserver)",
	/* 429 */	"#429 (unimplemented mbind)",
	/* 430 */	"#430 (unimplemented get_mempolicy)",
	/* 431 */	"#431 (unimplemented set_mempolicy)",
	/* 432 */	"#432 (unimplemented mq_open)",
	/* 433 */	"#433 (unimplemented mq_unlink)",
	/* 434 */	"#434 (unimplemented mq_timedsend)",
	/* 435 */	"#435 (unimplemented mq_timedreceive)",
	/* 436 */	"#436 (unimplemented mq_notify)",
	/* 437 */	"#437 (unimplemented mq_getsetattr)",
	/* 438 */	"#438 (unimplemented waitid)",
	/* 439 */	"#439 (unimplemented add_key)",
	/* 440 */	"#440 (unimplemented request_key)",
	/* 441 */	"#441 (unimplemented keyctl)",
	/* 442 */	"#442 (unimplemented ioprio_set)",
	/* 443 */	"#443 (unimplemented ioprio_get)",
	/* 444 */	"#444 (unimplemented inotify_init)",
	/* 445 */	"#445 (unimplemented inotify_add_watch)",
	/* 446 */	"#446 (unimplemented inotify_rm_watch)",
	/* 447 */	"#447 (unimplemented fdatasync)",
	/* 448 */	"#448 (unimplemented kexec_load)",
	/* 449 */	"#449 (unimplemented migrate_pages)",
	/* 450 */	"openat",
	/* 451 */	"mkdirat",
	/* 452 */	"mknodat",
	/* 453 */	"fchownat",
	/* 454 */	"#454 (unimplemented futimesat)",
	/* 455 */	"fstatat64",
	/* 456 */	"unlinkat",
	/* 457 */	"renameat",
	/* 458 */	"linkat",
	/* 459 */	"symlinkat",
	/* 460 */	"readlinkat",
	/* 461 */	"fchmodat",
	/* 462 */	"faccessat",
	/* 463 */	"pselect6",
	/* 464 */	"ppoll",
	/* 465 */	"#465 (unimplemented unshare)",
	/* 466 */	"set_robust_list",
	/* 467 */	"get_robust_list",
	/* 468 */	"#468 (unimplemented splice)",
	/* 469 */	"#469 (unimplemented sync_file_range)",
	/* 470 */	"#470 (unimplemented tee)",
	/* 471 */	"#471 (unimplemented vmsplice)",
	/* 472 */	"#472 (unimplemented move_pages)",
	/* 473 */	"#473 (unimplemented getcpu)",
	/* 474 */	"#474 (unimplemented epoll_wait)",
	/* 475 */	"utimensat",
	/* 476 */	"#476 (unimplemented signalfd)",
	/* 477 */	"#477 (unimplemented timerfd)",
	/* 478 */	"#478 (unimplemented eventfd)",
	/* 479 */	"recvmmsg",
	/* 480 */	"#480 (unimplemented fallocate)",
	/* 481 */	"#481 (unimplemented timerfd_create)",
	/* 482 */	"#482 (unimplemented timerfd_settime)",
	/* 483 */	"#483 (unimplemented timerfd_gettime)",
	/* 484 */	"#484 (unimplemented signalfd4)",
	/* 485 */	"#485 (unimplemented eventfd2)",
	/* 486 */	"#486 (unimplemented epoll_create1)",
	/* 487 */	"dup3",
	/* 488 */	"pipe2",
	/* 489 */	"#489 (unimplemented inotify_init1)",
	/* 490 */	"#490 (unimplemented preadv)",
	/* 491 */	"#491 (unimplemented pwritev)",
	/* 492 */	"#492 (unimplemented rt_tgsigqueueinfo)",
	/* 493 */	"#493 (unimplemented perf_counter_open)",
	/* 494 */	"#494 (unimplemented fanotify_init)",
	/* 495 */	"#495 (unimplemented fanotify_mark)",
	/* 496 */	"#496 (unimplemented prlimit64)",
	/* 497 */	"#497 (unimplemented name_to_handle_at)",
	/* 498 */	"#498 (unimplemented open_by_handle_at)",
	/* 499 */	"#499 (unimplemented clock_adjtime)",
	/* 500 */	"#500 (unimplemented syncfs)",
	/* 501 */	"#501 (unimplemented setns)",
	/* 502 */	"accept4",
	/* 503 */	"sendmmsg",
	/* 504 */	"#504 (unimplemented process_vm_readv)",
	/* 505 */	"#505 (unimplemented process_vm_writev)",
	/* 506 */	"#506 (unimplemented kcmp)",
	/* 507 */	"#507 (unimplemented finit_module)",
	/* 508 */	"#508 (unimplemented sched_setattr)",
	/* 509 */	"#509 (unimplemented sched_getattr)",
	/* 510 */	"#510 (unimplemented renameat2)",
	/* 511 */	"# filler",
};


/* libc style syscall names */
const char *const altlinux_syscallnames[] = {
	/*   0 */	"nosys",
	/*   1 */	NULL, /* exit */
	/*   2 */	NULL, /* fork */
	/*   3 */	NULL, /* read */
	/*   4 */	NULL, /* write */
	/*   5 */	NULL, /* unimplemented */
	/*   6 */	NULL, /* close */
	/*   7 */	NULL, /* wait4 */
	/*   8 */	NULL, /* creat */
	/*   9 */	NULL, /* link */
	/*  10 */	NULL, /* unlink */
	/*  11 */	NULL, /* unimplemented */
	/*  12 */	NULL, /* chdir */
	/*  13 */	NULL, /* fchdir */
	/*  14 */	NULL, /* mknod */
	/*  15 */	NULL, /* chmod */
	/*  16 */	NULL, /* __posix_chown */
	/*  17 */	NULL, /* brk */
	/*  18 */	NULL, /* unimplemented */
	/*  19 */	NULL, /* lseek */
	/*  20 */	NULL, /* getpid_with_ppid */
	/*  21 */	NULL, /* mount */
	/*  22 */	NULL, /* unimplemented umount */
	/*  23 */	NULL, /* setuid */
	/*  24 */	NULL, /* getuid_with_euid */
	/*  25 */	NULL, /* unimplemented */
	/*  26 */	NULL, /* ptrace */
	/*  27 */	NULL, /* unimplemented */
	/*  28 */	NULL, /* unimplemented */
	/*  29 */	NULL, /* unimplemented */
	/*  30 */	NULL, /* unimplemented */
	/*  31 */	NULL, /* unimplemented */
	/*  32 */	NULL, /* unimplemented */
	/*  33 */	NULL, /* access */
	/*  34 */	NULL, /* unimplemented */
	/*  35 */	NULL, /* unimplemented */
	/*  36 */	NULL, /* sync */
	/*  37 */	NULL, /* kill */
	/*  38 */	NULL, /* unimplemented */
	/*  39 */	NULL, /* setpgid */
	/*  40 */	NULL, /* unimplemented */
	/*  41 */	NULL, /* dup */
	/*  42 */	NULL, /* pipe */
	/*  43 */	NULL, /* set_program_attributes */
	/*  44 */	NULL, /* unimplemented */
	/*  45 */	NULL, /* open */
	/*  46 */	NULL, /* unimplemented */
	/*  47 */	NULL, /* getgid_with_egid */
	/*  48 */	NULL, /* sigprocmask */
	/*  49 */	NULL, /* unimplemented */
	/*  50 */	NULL, /* unimplemented */
	/*  51 */	NULL, /* acct */
	/*  52 */	NULL, /* sigpending */
	/*  53 */	NULL, /* unimplemented */
	/*  54 */	NULL, /* ioctl */
	/*  55 */	NULL, /* unimplemented */
	/*  56 */	NULL, /* unimplemented */
	/*  57 */	NULL, /* symlink */
	/*  58 */	NULL, /* readlink */
	/*  59 */	NULL, /* execve */
	/*  60 */	NULL, /* umask */
	/*  61 */	NULL, /* chroot */
	/*  62 */	NULL, /* unimplemented */
	/*  63 */	NULL, /* getpgrp */
	/*  64 */	NULL, /* getpagesize */
	/*  65 */	NULL, /* unimplemented */
	/*  66 */	"vfork",
	/*  67 */	NULL, /* stat */
	/*  68 */	NULL, /* lstat */
	/*  69 */	NULL, /* unimplemented */
	/*  70 */	NULL, /* unimplemented */
	/*  71 */	NULL, /* mmap */
	/*  72 */	NULL, /* unimplemented */
	/*  73 */	NULL, /* munmap */
	/*  74 */	NULL, /* mprotect */
	/*  75 */	NULL, /* madvise */
	/*  76 */	NULL, /* unimplemented vhangup */
	/*  77 */	NULL, /* unimplemented */
	/*  78 */	NULL, /* unimplemented */
	/*  79 */	NULL, /* getgroups */
	/*  80 */	NULL, /* setgroups */
	/*  81 */	NULL, /* unimplemented */
	/*  82 */	NULL, /* unimplemented setpgrp */
	/*  83 */	NULL, /* setitimer */
	/*  84 */	NULL, /* unimplemented */
	/*  85 */	NULL, /* unimplemented */
	/*  86 */	NULL, /* unimplemented osf1_sys_getitimer */
	/*  87 */	NULL, /* gethostname */
	/*  88 */	NULL, /* sethostname */
	/*  89 */	NULL, /* unimplemented getdtablesize */
	/*  90 */	NULL, /* dup2 */
	/*  91 */	NULL, /* fstat */
	/*  92 */	NULL, /* fcntl */
	/*  93 */	NULL, /* select */
	/*  94 */	NULL, /* poll */
	/*  95 */	NULL, /* fsync */
	/*  96 */	NULL, /* setpriority */
	/*  97 */	NULL, /* socket */
	/*  98 */	NULL, /* connect */
	/*  99 */	NULL, /* accept */
	/* 100 */	NULL, /* getpriority */
	/* 101 */	NULL, /* send */
	/* 102 */	NULL, /* recv */
	/* 103 */	NULL, /* sigreturn */
	/* 104 */	NULL, /* bind */
	/* 105 */	NULL, /* setsockopt */
	/* 106 */	NULL, /* listen */
	/* 107 */	NULL, /* unimplemented */
	/* 108 */	NULL, /* unimplemented */
	/* 109 */	NULL, /* unimplemented */
	/* 110 */	NULL, /* unimplemented */
	/* 111 */	NULL, /* sigsuspend */
	/* 112 */	NULL, /* sigstack */
	/* 113 */	NULL, /* recvmsg */
	/* 114 */	NULL, /* sendmsg */
	/* 115 */	NULL, /* unimplemented */
	/* 116 */	NULL, /* gettimeofday */
	/* 117 */	NULL, /* getrusage */
	/* 118 */	NULL, /* getsockopt */
	/* 119 */	NULL, /* unimplemented */
	/* 120 */	NULL, /* readv */
	/* 121 */	NULL, /* writev */
	/* 122 */	NULL, /* settimeofday */
	/* 123 */	NULL, /* __posix_fchown */
	/* 124 */	NULL, /* fchmod */
	/* 125 */	NULL, /* recvfrom */
	/* 126 */	NULL, /* setreuid */
	/* 127 */	NULL, /* setregid */
	/* 128 */	NULL, /* __posix_rename */
	/* 129 */	NULL, /* truncate */
	/* 130 */	NULL, /* ftruncate */
	/* 131 */	NULL, /* flock */
	/* 132 */	NULL, /* setgid */
	/* 133 */	NULL, /* sendto */
	/* 134 */	NULL, /* shutdown */
	/* 135 */	NULL, /* socketpair */
	/* 136 */	NULL, /* mkdir */
	/* 137 */	NULL, /* rmdir */
	/* 138 */	NULL, /* utimes */
	/* 139 */	NULL, /* unimplemented */
	/* 140 */	NULL, /* unimplemented */
	/* 141 */	NULL, /* getpeername */
	/* 142 */	NULL, /* unimplemented */
	/* 143 */	NULL, /* unimplemented */
	/* 144 */	NULL, /* getrlimit */
	/* 145 */	NULL, /* setrlimit */
	/* 146 */	NULL, /* unimplemented */
	/* 147 */	NULL, /* setsid */
	/* 148 */	NULL, /* unimplemented quotactl */
	/* 149 */	NULL, /* unimplemented */
	/* 150 */	NULL, /* getsockname */
	/* 151 */	NULL, /* unimplemented */
	/* 152 */	NULL, /* unimplemented */
	/* 153 */	NULL, /* unimplemented */
	/* 154 */	NULL, /* unimplemented */
	/* 155 */	NULL, /* unimplemented */
	/* 156 */	NULL, /* sigaction */
	/* 157 */	NULL, /* unimplemented */
	/* 158 */	NULL, /* unimplemented */
	/* 159 */	NULL, /* getdirentries */
	/* 160 */	NULL, /* statfs */
	/* 161 */	NULL, /* fstatfs */
	/* 162 */	NULL, /* unimplemented */
	/* 163 */	NULL, /* unimplemented */
	/* 164 */	NULL, /* unimplemented */
	/* 165 */	NULL, /* getdomainname */
	/* 166 */	NULL, /* setdomainname */
	/* 167 */	NULL, /* unimplemented */
	/* 168 */	NULL, /* unimplemented */
	/* 169 */	NULL, /* unimplemented */
	/* 170 */	NULL, /* unimplemented */
	/* 171 */	NULL, /* unimplemented */
	/* 172 */	NULL, /* unimplemented */
	/* 173 */	NULL, /* unimplemented */
	/* 174 */	NULL, /* unimplemented */
	/* 175 */	NULL, /* unimplemented */
	/* 176 */	NULL, /* unimplemented */
	/* 177 */	NULL, /* unimplemented */
	/* 178 */	NULL, /* unimplemented */
	/* 179 */	NULL, /* unimplemented */
	/* 180 */	NULL, /* unimplemented */
	/* 181 */	NULL, /* unimplemented */
	/* 182 */	NULL, /* unimplemented */
	/* 183 */	NULL, /* unimplemented */
	/* 184 */	NULL, /* unimplemented */
	/* 185 */	NULL, /* unimplemented */
	/* 186 */	NULL, /* unimplemented */
	/* 187 */	NULL, /* unimplemented */
	/* 188 */	NULL, /* unimplemented */
	/* 189 */	NULL, /* unimplemented */
	/* 190 */	NULL, /* unimplemented */
	/* 191 */	NULL, /* ugetrlimit */
	/* 192 */	NULL, /* unimplemented mmap2 */
	/* 193 */	NULL, /* unimplemented */
	/* 194 */	NULL, /* unimplemented */
	/* 195 */	NULL, /* unimplemented */
	/* 196 */	NULL, /* unimplemented */
	/* 197 */	NULL, /* unimplemented */
	/* 198 */	NULL, /* unimplemented */
	/* 199 */	NULL, /* unimplemented osf1_sys_swapon */
#ifdef SYSVMSG
	/* 200 */	NULL, /* msgctl */
	/* 201 */	NULL, /* msgget */
	/* 202 */	NULL, /* msgrcv */
	/* 203 */	NULL, /* msgsnd */
#else
	/* 200 */	NULL, /* unimplemented msgctl */
	/* 201 */	NULL, /* unimplemented msgget */
	/* 202 */	NULL, /* unimplemented msgrcv */
	/* 203 */	NULL, /* unimplemented msgsnd */
#endif
#ifdef SYSVSEM
	/* 204 */	NULL, /* semctl */
	/* 205 */	NULL, /* semget */
	/* 206 */	NULL, /* semop */
#else
	/* 204 */	NULL, /* unimplemented semctl */
	/* 205 */	NULL, /* unimplemented semget */
	/* 206 */	NULL, /* unimplemented semop */
#endif
	/* 207 */	NULL, /* olduname */
	/* 208 */	NULL, /* __posix_lchown */
#ifdef SYSVSHM
	/* 209 */	NULL, /* shmat */
	/* 210 */	NULL, /* shmctl */
	/* 211 */	NULL, /* shmdt */
	/* 212 */	NULL, /* shmget */
#else
	/* 209 */	NULL, /* unimplemented shmat */
	/* 210 */	NULL, /* unimplemented shmctl */
	/* 211 */	NULL, /* unimplemented shmdt */
	/* 212 */	NULL, /* unimplemented shmget */
#endif
	/* 213 */	NULL, /* unimplemented */
	/* 214 */	NULL, /* unimplemented */
	/* 215 */	NULL, /* unimplemented */
	/* 216 */	NULL, /* unimplemented */
	/* 217 */	"msync",
	/* 218 */	NULL, /* unimplemented */
	/* 219 */	NULL, /* unimplemented */
	/* 220 */	NULL, /* unimplemented */
	/* 221 */	NULL, /* unimplemented */
	/* 222 */	NULL, /* unimplemented */
	/* 223 */	NULL, /* unimplemented */
	/* 224 */	NULL, /* unimplemented */
	/* 225 */	NULL, /* unimplemented */
	/* 226 */	NULL, /* unimplemented */
	/* 227 */	NULL, /* unimplemented */
	/* 228 */	NULL, /* unimplemented */
	/* 229 */	NULL, /* unimplemented */
	/* 230 */	NULL, /* unimplemented */
	/* 231 */	NULL, /* unimplemented */
	/* 232 */	NULL, /* unimplemented */
	/* 233 */	NULL, /* getpgid */
	/* 234 */	NULL, /* getsid */
	/* 235 */	NULL, /* unimplemented sigaltstack */
	/* 236 */	NULL, /* unimplemented */
	/* 237 */	NULL, /* unimplemented */
	/* 238 */	NULL, /* unimplemented */
	/* 239 */	NULL, /* unimplemented */
	/* 240 */	NULL, /* unimplemented */
	/* 241 */	NULL, /* sysinfo */
	/* 242 */	NULL, /* unimplemented */
	/* 243 */	NULL, /* unimplemented */
	/* 244 */	NULL, /* unimplemented osf1_sys_proplist_syscall */
	/* 245 */	NULL, /* unimplemented */
	/* 246 */	NULL, /* unimplemented */
	/* 247 */	NULL, /* unimplemented */
	/* 248 */	NULL, /* unimplemented */
	/* 249 */	NULL, /* unimplemented */
	/* 250 */	NULL, /* unimplemented */
	/* 251 */	NULL, /* usleep_thread */
	/* 252 */	NULL, /* unimplemented */
	/* 253 */	NULL, /* unimplemented */
	/* 254 */	NULL, /* unimplemented */
	/* 255 */	NULL, /* unimplemented sysfs */
	/* 256 */	NULL, /* getsysinfo */
	/* 257 */	NULL, /* setsysinfo */
	/* 258 */	NULL, /* unimplemented */
	/* 259 */	NULL, /* unimplemented */
	/* 260 */	NULL, /* unimplemented */
	/* 261 */	NULL, /* fdatasync */
	/* 262 */	NULL, /* unimplemented */
	/* 263 */	NULL, /* unimplemented */
	/* 264 */	NULL, /* unimplemented */
	/* 265 */	NULL, /* unimplemented */
	/* 266 */	NULL, /* unimplemented */
	/* 267 */	NULL, /* unimplemented */
	/* 268 */	NULL, /* unimplemented */
	/* 269 */	NULL, /* unimplemented */
	/* 270 */	NULL, /* unimplemented */
	/* 271 */	NULL, /* unimplemented */
	/* 272 */	NULL, /* unimplemented */
	/* 273 */	NULL, /* unimplemented */
	/* 274 */	NULL, /* unimplemented */
	/* 275 */	NULL, /* unimplemented */
	/* 276 */	NULL, /* unimplemented */
	/* 277 */	NULL, /* unimplemented */
	/* 278 */	NULL, /* unimplemented */
	/* 279 */	NULL, /* unimplemented */
	/* 280 */	NULL, /* unimplemented */
	/* 281 */	NULL, /* unimplemented */
	/* 282 */	NULL, /* unimplemented */
	/* 283 */	NULL, /* unimplemented */
	/* 284 */	NULL, /* unimplemented */
	/* 285 */	NULL, /* unimplemented */
	/* 286 */	NULL, /* unimplemented */
	/* 287 */	NULL, /* unimplemented */
	/* 288 */	NULL, /* unimplemented */
	/* 289 */	NULL, /* unimplemented */
	/* 290 */	NULL, /* unimplemented */
	/* 291 */	NULL, /* unimplemented */
	/* 292 */	NULL, /* unimplemented */
	/* 293 */	NULL, /* unimplemented */
	/* 294 */	NULL, /* unimplemented */
	/* 295 */	NULL, /* unimplemented */
	/* 296 */	NULL, /* unimplemented */
	/* 297 */	NULL, /* unimplemented */
	/* 298 */	NULL, /* unimplemented */
	/* 299 */	NULL, /* unimplemented */
	/* 300 */	NULL, /* unimplemented bdflush */
	/* 301 */	NULL, /* unimplemented sethae */
	/* 302 */	NULL, /* unimplemented mount */
	/* 303 */	NULL, /* unimplemented old_adjtimex */
	/* 304 */	NULL, /* swapoff */
	/* 305 */	NULL, /* getdents */
	/* 306 */	NULL, /* unimplemented alpha_create_module */
	/* 307 */	NULL, /* unimplemented init_module */
	/* 308 */	NULL, /* unimplemented delete_module */
	/* 309 */	NULL, /* unimplemented get_kernel_syms */
	/* 310 */	NULL, /* unimplemented syslog */
	/* 311 */	NULL, /* reboot */
	/* 312 */	NULL, /* clone */
#ifdef EXEC_AOUT
	/* 313 */	NULL, /* uselib */
#else
	/* 313 */	NULL, /* unimplemented sys_uselib */
#endif
	/* 314 */	NULL, /* mlock */
	/* 315 */	NULL, /* munlock */
	/* 316 */	NULL, /* mlockall */
	/* 317 */	NULL, /* munlockall */
	/* 318 */	NULL, /* sysinfo */
	/* 319 */	NULL, /* __sysctl */
	/* 320 */	NULL, /* unimplemented idle */
	/* 321 */	NULL, /* unimplemented oldumount */
	/* 322 */	NULL, /* swapon */
	/* 323 */	NULL, /* times */
	/* 324 */	NULL, /* personality */
	/* 325 */	NULL, /* setfsuid */
	/* 326 */	NULL, /* setfsgid */
	/* 327 */	NULL, /* unimplemented ustat */
	/* 328 */	NULL, /* statfs */
	/* 329 */	NULL, /* fstatfs */
	/* 330 */	NULL, /* sched_setparam */
	/* 331 */	NULL, /* sched_getparam */
	/* 332 */	NULL, /* sched_setscheduler */
	/* 333 */	NULL, /* sched_getscheduler */
	/* 334 */	NULL, /* sched_yield */
	/* 335 */	NULL, /* sched_get_priority_max */
	/* 336 */	NULL, /* sched_get_priority_min */
	/* 337 */	NULL, /* unimplemented sched_rr_get_interval */
	/* 338 */	NULL, /* unimplemented afs_syscall */
	/* 339 */	NULL, /* uname */
	/* 340 */	NULL, /* nanosleep */
	/* 341 */	NULL, /* mremap */
	/* 342 */	NULL, /* unimplemented nfsservctl */
	/* 343 */	NULL, /* setresuid */
	/* 344 */	NULL, /* getresuid */
	/* 345 */	NULL, /* unimplemented pciconfig_read */
	/* 346 */	NULL, /* unimplemented pciconfig_write */
	/* 347 */	NULL, /* unimplemented query_module */
	/* 348 */	NULL, /* unimplemented prctl */
	/* 349 */	NULL, /* pread */
	/* 350 */	NULL, /* pwrite */
	/* 351 */	NULL, /* rt_sigreturn */
	/* 352 */	NULL, /* rt_sigaction */
	/* 353 */	NULL, /* rt_sigprocmask */
	/* 354 */	NULL, /* rt_sigpending */
	/* 355 */	NULL, /* rt_sigtimedwait */
	/* 356 */	NULL, /* rt_queueinfo */
	/* 357 */	NULL, /* rt_sigsuspend */
	/* 358 */	NULL, /* select */
	/* 359 */	NULL, /* gettimeofday */
	/* 360 */	NULL, /* settimeofday */
	/* 361 */	NULL, /* getitimer */
	/* 362 */	NULL, /* setitimer */
	/* 363 */	NULL, /* utimes */
	/* 364 */	NULL, /* getrusage */
	/* 365 */	NULL, /* wait4 */
	/* 366 */	NULL, /* unimplemented adjtimex */
	/* 367 */	NULL, /* __getcwd */
	/* 368 */	NULL, /* unimplemented capget */
	/* 369 */	NULL, /* unimplemented capset */
	/* 370 */	NULL, /* unimplemented sendfile */
	/* 371 */	NULL, /* unimplemented setresgid */
	/* 372 */	NULL, /* unimplemented getresgid */
	/* 373 */	NULL, /* unimplemented sys_dipc */
	/* 374 */	NULL, /* unimplemented pivot_root */
	/* 375 */	NULL, /* mincore */
	/* 376 */	NULL, /* unimplemented pciconfig_iobase */
	/* 377 */	NULL, /* getdents64 */
	/* 378 */	NULL, /* gettid */
	/* 379 */	NULL, /* unimplemented readahead */
	/* 380 */	NULL, /* unimplemented / * unused * / */
	/* 381 */	NULL, /* tkill */
	/* 382 */	NULL, /* setxattr */
	/* 383 */	NULL, /* lsetxattr */
	/* 384 */	NULL, /* fsetxattr */
	/* 385 */	NULL, /* getxattr */
	/* 386 */	NULL, /* lgetxattr */
	/* 387 */	NULL, /* fgetxattr */
	/* 388 */	NULL, /* listxattr */
	/* 389 */	NULL, /* llistxattr */
	/* 390 */	NULL, /* flistxattr */
	/* 391 */	NULL, /* removexattr */
	/* 392 */	NULL, /* lremovexattr */
	/* 393 */	NULL, /* fremovexattr */
	/* 394 */	NULL, /* futex */
	/* 395 */	NULL, /* sched_setaffinity */
	/* 396 */	NULL, /* sched_getaffinity */
	/* 397 */	NULL, /* unimplemented tuxcall */
	/* 398 */	NULL, /* unimplemented io_setup */
	/* 399 */	NULL, /* unimplemented io_destroy */
	/* 400 */	NULL, /* unimplemented io_getevents */
	/* 401 */	NULL, /* unimplemented io_submit */
	/* 402 */	NULL, /* unimplemented io_cancel */
	/* 403 */	NULL, /* unimplemented / * unused * / */
	/* 404 */	NULL, /* unimplemented / * unused * / */
	/* 405 */	NULL, /* exit_group */
	/* 406 */	NULL, /* unimplemented lookup_dcookie */
	/* 407 */	NULL, /* unimplemented sys_epoll_create */
	/* 408 */	NULL, /* unimplemented sys_epoll_ctl */
	/* 409 */	NULL, /* unimplemented sys_epoll_wait */
	/* 410 */	NULL, /* unimplemented remap_file_pages */
	/* 411 */	NULL, /* set_tid_address */
	/* 412 */	NULL, /* unimplemented restart_syscall */
	/* 413 */	NULL, /* fadvise64 */
	/* 414 */	NULL, /* unimplemented timer_create */
	/* 415 */	NULL, /* unimplemented timer_settime */
	/* 416 */	NULL, /* unimplemented timer_gettime */
	/* 417 */	NULL, /* unimplemented timer_getoverrun */
	/* 418 */	NULL, /* unimplemented timer_delete */
	/* 419 */	NULL, /* clock_settime */
	/* 420 */	NULL, /* clock_gettime */
	/* 421 */	NULL, /* clock_getres */
	/* 422 */	NULL, /* clock_nanosleep */
	/* 423 */	NULL, /* unimplemented semtimedop */
	/* 424 */	NULL, /* tgkill */
	/* 425 */	NULL, /* stat64 */
	/* 426 */	NULL, /* lstat64 */
	/* 427 */	NULL, /* fstat64 */
	/* 428 */	NULL, /* unimplemented vserver */
	/* 429 */	NULL, /* unimplemented mbind */
	/* 430 */	NULL, /* unimplemented get_mempolicy */
	/* 431 */	NULL, /* unimplemented set_mempolicy */
	/* 432 */	NULL, /* unimplemented mq_open */
	/* 433 */	NULL, /* unimplemented mq_unlink */
	/* 434 */	NULL, /* unimplemented mq_timedsend */
	/* 435 */	NULL, /* unimplemented mq_timedreceive */
	/* 436 */	NULL, /* unimplemented mq_notify */
	/* 437 */	NULL, /* unimplemented mq_getsetattr */
	/* 438 */	NULL, /* unimplemented waitid */
	/* 439 */	NULL, /* unimplemented add_key */
	/* 440 */	NULL, /* unimplemented request_key */
	/* 441 */	NULL, /* unimplemented keyctl */
	/* 442 */	NULL, /* unimplemented ioprio_set */
	/* 443 */	NULL, /* unimplemented ioprio_get */
	/* 444 */	NULL, /* unimplemented inotify_init */
	/* 445 */	NULL, /* unimplemented inotify_add_watch */
	/* 446 */	NULL, /* unimplemented inotify_rm_watch */
	/* 447 */	NULL, /* unimplemented fdatasync */
	/* 448 */	NULL, /* unimplemented kexec_load */
	/* 449 */	NULL, /* unimplemented migrate_pages */
	/* 450 */	NULL, /* openat */
	/* 451 */	NULL, /* mkdirat */
	/* 452 */	NULL, /* mknodat */
	/* 453 */	NULL, /* fchownat */
	/* 454 */	NULL, /* unimplemented futimesat */
	/* 455 */	NULL, /* fstatat64 */
	/* 456 */	NULL, /* unlinkat */
	/* 457 */	NULL, /* renameat */
	/* 458 */	NULL, /* linkat */
	/* 459 */	NULL, /* symlinkat */
	/* 460 */	NULL, /* readlinkat */
	/* 461 */	NULL, /* fchmodat */
	/* 462 */	NULL, /* faccessat */
	/* 463 */	NULL, /* pselect6 */
	/* 464 */	NULL, /* ppoll */
	/* 465 */	NULL, /* unimplemented unshare */
	/* 466 */	NULL, /* set_robust_list */
	/* 467 */	NULL, /* get_robust_list */
	/* 468 */	NULL, /* unimplemented splice */
	/* 469 */	NULL, /* unimplemented sync_file_range */
	/* 470 */	NULL, /* unimplemented tee */
	/* 471 */	NULL, /* unimplemented vmsplice */
	/* 472 */	NULL, /* unimplemented move_pages */
	/* 473 */	NULL, /* unimplemented getcpu */
	/* 474 */	NULL, /* unimplemented epoll_wait */
	/* 475 */	NULL, /* utimensat */
	/* 476 */	NULL, /* unimplemented signalfd */
	/* 477 */	NULL, /* unimplemented timerfd */
	/* 478 */	NULL, /* unimplemented eventfd */
	/* 479 */	NULL, /* recvmmsg */
	/* 480 */	NULL, /* unimplemented fallocate */
	/* 481 */	NULL, /* unimplemented timerfd_create */
	/* 482 */	NULL, /* unimplemented timerfd_settime */
	/* 483 */	NULL, /* unimplemented timerfd_gettime */
	/* 484 */	NULL, /* unimplemented signalfd4 */
	/* 485 */	NULL, /* unimplemented eventfd2 */
	/* 486 */	NULL, /* unimplemented epoll_create1 */
	/* 487 */	NULL, /* dup3 */
	/* 488 */	NULL, /* pipe2 */
	/* 489 */	NULL, /* unimplemented inotify_init1 */
	/* 490 */	NULL, /* unimplemented preadv */
	/* 491 */	NULL, /* unimplemented pwritev */
	/* 492 */	NULL, /* unimplemented rt_tgsigqueueinfo */
	/* 493 */	NULL, /* unimplemented perf_counter_open */
	/* 494 */	NULL, /* unimplemented fanotify_init */
	/* 495 */	NULL, /* unimplemented fanotify_mark */
	/* 496 */	NULL, /* unimplemented prlimit64 */
	/* 497 */	NULL, /* unimplemented name_to_handle_at */
	/* 498 */	NULL, /* unimplemented open_by_handle_at */
	/* 499 */	NULL, /* unimplemented clock_adjtime */
	/* 500 */	NULL, /* unimplemented syncfs */
	/* 501 */	NULL, /* unimplemented setns */
	/* 502 */	NULL, /* accept4 */
	/* 503 */	NULL, /* sendmmsg */
	/* 504 */	NULL, /* unimplemented process_vm_readv */
	/* 505 */	NULL, /* unimplemented process_vm_writev */
	/* 506 */	NULL, /* unimplemented kcmp */
	/* 507 */	NULL, /* unimplemented finit_module */
	/* 508 */	NULL, /* unimplemented sched_setattr */
	/* 509 */	NULL, /* unimplemented sched_getattr */
	/* 510 */	NULL, /* unimplemented renameat2 */
	/* 511 */	NULL, /* filler */
};
