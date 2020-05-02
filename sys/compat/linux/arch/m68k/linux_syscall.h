/* $NetBSD: linux_syscall.h,v 1.108 2020/04/26 19:20:19 thorpej Exp $ */

/*
 * System call numbers.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	NetBSD: syscalls.master,v 1.95 2020/04/26 18:53:32 thorpej Exp
 */

#ifndef _LINUX_SYS_SYSCALL_H_
#define	_LINUX_SYS_SYSCALL_H_

#define	LINUX_SYS_MAXSYSARGS	8

/* syscall: "syscall" ret: "int" args: */
#define	LINUX_SYS_syscall	0

/* syscall: "exit" ret: "int" args: "int" */
#define	LINUX_SYS_exit	1

/* syscall: "fork" ret: "int" args: */
#define	LINUX_SYS_fork	2

/* syscall: "read" ret: "ssize_t" args: "int" "void *" "size_t" */
#define	LINUX_SYS_read	3

/* syscall: "write" ret: "ssize_t" args: "int" "const void *" "size_t" */
#define	LINUX_SYS_write	4

/* syscall: "open" ret: "int" args: "const char *" "int" "linux_umode_t" */
#define	LINUX_SYS_open	5

/* syscall: "close" ret: "int" args: "int" */
#define	LINUX_SYS_close	6

/* syscall: "waitpid" ret: "int" args: "int" "int *" "int" */
#define	LINUX_SYS_waitpid	7

/* syscall: "creat" ret: "int" args: "const char *" "linux_umode_t" */
#define	LINUX_SYS_creat	8

/* syscall: "link" ret: "int" args: "const char *" "const char *" */
#define	LINUX_SYS_link	9

/* syscall: "unlink" ret: "int" args: "const char *" */
#define	LINUX_SYS_unlink	10

/* syscall: "execve" ret: "int" args: "const char *" "char **" "char **" */
#define	LINUX_SYS_execve	11

/* syscall: "chdir" ret: "int" args: "const char *" */
#define	LINUX_SYS_chdir	12

/* syscall: "time" ret: "int" args: "linux_time_t *" */
#define	LINUX_SYS_time	13

/* syscall: "mknod" ret: "int" args: "const char *" "linux_umode_t" "unsigned" */
#define	LINUX_SYS_mknod	14

/* syscall: "chmod" ret: "int" args: "const char *" "int" */
#define	LINUX_SYS_chmod	15

/* syscall: "chown16" ret: "int" args: "const char *" "linux_uid16_t" "linux_gid16_t" */
#define	LINUX_SYS_chown16	16

				/* 17 is obsolete break */
				/* 18 is obsolete ostat */
#if !defined(_KERNEL) || defined(COMPAT_43)
/* syscall: "lseek" ret: "long" args: "int" "long" "int" */
#define	LINUX_SYS_lseek	19

#else
#endif
/* syscall: "getpid" ret: "pid_t" args: */
#define	LINUX_SYS_getpid	20

				/* 22 is obsolete umount */
/* syscall: "linux_setuid16" ret: "int" args: "uid_t" */
#define	LINUX_SYS_linux_setuid16	23

/* syscall: "linux_getuid16" ret: "uid_t" args: */
#define	LINUX_SYS_linux_getuid16	24

/* syscall: "stime" ret: "int" args: "linux_time_t *" */
#define	LINUX_SYS_stime	25

/* syscall: "ptrace" ret: "int" args: "int" "int" "int" "int" */
#define	LINUX_SYS_ptrace	26

/* syscall: "alarm" ret: "int" args: "unsigned int" */
#define	LINUX_SYS_alarm	27

				/* 28 is obsolete ofstat */
/* syscall: "pause" ret: "int" args: */
#define	LINUX_SYS_pause	29

/* syscall: "utime" ret: "int" args: "const char *" "struct linux_utimbuf *" */
#define	LINUX_SYS_utime	30

				/* 31 is obsolete stty */
				/* 32 is obsolete gtty */
/* syscall: "access" ret: "int" args: "const char *" "int" */
#define	LINUX_SYS_access	33

/* syscall: "nice" ret: "int" args: "int" */
#define	LINUX_SYS_nice	34

				/* 35 is obsolete ftime */
/* syscall: "sync" ret: "int" args: */
#define	LINUX_SYS_sync	36

/* syscall: "kill" ret: "int" args: "int" "int" */
#define	LINUX_SYS_kill	37

/* syscall: "__posix_rename" ret: "int" args: "const char *" "const char *" */
#define	LINUX_SYS___posix_rename	38

/* syscall: "mkdir" ret: "int" args: "const char *" "linux_umode_t" */
#define	LINUX_SYS_mkdir	39

/* syscall: "rmdir" ret: "int" args: "const char *" */
#define	LINUX_SYS_rmdir	40

/* syscall: "dup" ret: "int" args: "int" */
#define	LINUX_SYS_dup	41

/* syscall: "pipe" ret: "int" args: "int *" */
#define	LINUX_SYS_pipe	42

/* syscall: "times" ret: "int" args: "struct times *" */
#define	LINUX_SYS_times	43

				/* 44 is obsolete prof */
/* syscall: "brk" ret: "int" args: "char *" */
#define	LINUX_SYS_brk	45

/* syscall: "linux_setgid16" ret: "int" args: "gid_t" */
#define	LINUX_SYS_linux_setgid16	46

/* syscall: "linux_getgid16" ret: "gid_t" args: */
#define	LINUX_SYS_linux_getgid16	47

/* syscall: "signal" ret: "int" args: "int" "linux_handler_t" */
#define	LINUX_SYS_signal	48

/* syscall: "linux_geteuid16" ret: "uid_t" args: */
#define	LINUX_SYS_linux_geteuid16	49

/* syscall: "linux_getegid16" ret: "gid_t" args: */
#define	LINUX_SYS_linux_getegid16	50

/* syscall: "acct" ret: "int" args: "char *" */
#define	LINUX_SYS_acct	51

				/* 53 is obsolete lock */
/* syscall: "ioctl" ret: "int" args: "int" "u_long" "void *" */
#define	LINUX_SYS_ioctl	54

/* syscall: "fcntl" ret: "int" args: "int" "int" "void *" */
#define	LINUX_SYS_fcntl	55

				/* 56 is obsolete mpx */
/* syscall: "setpgid" ret: "int" args: "int" "int" */
#define	LINUX_SYS_setpgid	57

				/* 58 is obsolete ulimit */
/* syscall: "umask" ret: "int" args: "int" */
#define	LINUX_SYS_umask	60

/* syscall: "chroot" ret: "int" args: "char *" */
#define	LINUX_SYS_chroot	61

/* syscall: "dup2" ret: "int" args: "int" "int" */
#define	LINUX_SYS_dup2	63

/* syscall: "getppid" ret: "pid_t" args: */
#define	LINUX_SYS_getppid	64

/* syscall: "getpgrp" ret: "int" args: */
#define	LINUX_SYS_getpgrp	65

/* syscall: "setsid" ret: "int" args: */
#define	LINUX_SYS_setsid	66

/* syscall: "sigaction" ret: "int" args: "int" "const struct linux_old_sigaction *" "struct linux_old_sigaction *" */
#define	LINUX_SYS_sigaction	67

/* syscall: "siggetmask" ret: "int" args: */
#define	LINUX_SYS_siggetmask	68

/* syscall: "sigsetmask" ret: "int" args: "linux_old_sigset_t" */
#define	LINUX_SYS_sigsetmask	69

/* syscall: "setreuid16" ret: "int" args: "linux_uid16_t" "linux_uid16_t" */
#define	LINUX_SYS_setreuid16	70

/* syscall: "setregid16" ret: "int" args: "linux_gid16_t" "linux_gid16_t" */
#define	LINUX_SYS_setregid16	71

/* syscall: "sigsuspend" ret: "int" args: "void *" "int" "int" */
#define	LINUX_SYS_sigsuspend	72

/* syscall: "sigpending" ret: "int" args: "linux_old_sigset_t *" */
#define	LINUX_SYS_sigpending	73

#if !defined(_KERNEL) || defined(COMPAT_43)
/* syscall: "sethostname" ret: "int" args: "char *" "u_int" */
#define	LINUX_SYS_sethostname	74

#else
#endif
/* syscall: "setrlimit" ret: "int" args: "u_int" "struct orlimit *" */
#define	LINUX_SYS_setrlimit	75

/* syscall: "getrlimit" ret: "int" args: "u_int" "struct orlimit *" */
#define	LINUX_SYS_getrlimit	76

/* syscall: "getrusage" ret: "int" args: "int" "struct rusage50 *" */
#define	LINUX_SYS_getrusage	77

/* syscall: "gettimeofday" ret: "int" args: "struct timeval50 *" "struct timezone *" */
#define	LINUX_SYS_gettimeofday	78

/* syscall: "settimeofday" ret: "int" args: "struct timeval50 *" "struct timezone *" */
#define	LINUX_SYS_settimeofday	79

/* syscall: "getgroups16" ret: "int" args: "int" "linux_gid16_t *" */
#define	LINUX_SYS_getgroups16	80

/* syscall: "setgroups16" ret: "int" args: "int" "linux_gid16_t *" */
#define	LINUX_SYS_setgroups16	81

/* syscall: "oldselect" ret: "int" args: "struct linux_oldselect *" */
#define	LINUX_SYS_oldselect	82

/* syscall: "symlink" ret: "int" args: "const char *" "const char *" */
#define	LINUX_SYS_symlink	83

#if !defined(_KERNEL) || defined(COMPAT_43)
/* syscall: "oolstat" ret: "int" args: "const char *" "struct stat43 *" */
#define	LINUX_SYS_oolstat	84

#else
#endif
/* syscall: "readlink" ret: "ssize_t" args: "const char *" "char *" "int" */
#define	LINUX_SYS_readlink	85

#ifdef EXEC_AOUT
/* syscall: "uselib" ret: "int" args: "const char *" */
#define	LINUX_SYS_uselib	86

#else
#endif
/* syscall: "swapon" ret: "int" args: "char *" */
#define	LINUX_SYS_swapon	87

/* syscall: "reboot" ret: "int" args: "int" "int" "int" "void *" */
#define	LINUX_SYS_reboot	88

/* syscall: "readdir" ret: "int" args: "int" "void *" "unsigned int" */
#define	LINUX_SYS_readdir	89

/* syscall: "old_mmap" ret: "int" args: "struct linux_oldmmap *" */
#define	LINUX_SYS_old_mmap	90

/* syscall: "munmap" ret: "int" args: "void *" "size_t" */
#define	LINUX_SYS_munmap	91

/* syscall: "truncate" ret: "int" args: "const char *" "long" */
#define	LINUX_SYS_truncate	92

#if !defined(_KERNEL) || defined(COMPAT_43)
/* syscall: "ftruncate" ret: "int" args: "int" "long" */
#define	LINUX_SYS_ftruncate	93

#else
#endif
/* syscall: "fchmod" ret: "int" args: "int" "linux_umode_t" */
#define	LINUX_SYS_fchmod	94

/* syscall: "fchown16" ret: "int" args: "int" "linux_uid16_t" "linux_gid16_t" */
#define	LINUX_SYS_fchown16	95

/* syscall: "getpriority" ret: "int" args: "int" "int" */
#define	LINUX_SYS_getpriority	96

/* syscall: "setpriority" ret: "int" args: "int" "int" "int" */
#define	LINUX_SYS_setpriority	97

/* syscall: "profil" ret: "int" args: "void *" "u_int" "u_int" "u_int" */
#define	LINUX_SYS_profil	98

/* syscall: "statfs" ret: "int" args: "const char *" "struct linux_statfs *" */
#define	LINUX_SYS_statfs	99

/* syscall: "fstatfs" ret: "int" args: "int" "struct linux_statfs *" */
#define	LINUX_SYS_fstatfs	100

/* syscall: "socketcall" ret: "int" args: "int" "void *" */
#define	LINUX_SYS_socketcall	102

/* syscall: "setitimer" ret: "int" args: "int" "struct itimerval50 *" "struct itimerval50 *" */
#define	LINUX_SYS_setitimer	104

/* syscall: "getitimer" ret: "int" args: "int" "struct itimerval50 *" */
#define	LINUX_SYS_getitimer	105

/* syscall: "stat" ret: "int" args: "const char *" "struct linux_stat *" */
#define	LINUX_SYS_stat	106

/* syscall: "lstat" ret: "int" args: "const char *" "struct linux_stat *" */
#define	LINUX_SYS_lstat	107

/* syscall: "fstat" ret: "int" args: "int" "struct linux_stat *" */
#define	LINUX_SYS_fstat	108

/* syscall: "wait4" ret: "int" args: "int" "int *" "int" "struct rusage50 *" */
#define	LINUX_SYS_wait4	114

/* syscall: "swapoff" ret: "int" args: "const char *" */
#define	LINUX_SYS_swapoff	115

/* syscall: "sysinfo" ret: "int" args: "struct linux_sysinfo *" */
#define	LINUX_SYS_sysinfo	116

/* syscall: "ipc" ret: "int" args: "int" "int" "int" "int" "void *" */
#define	LINUX_SYS_ipc	117

/* syscall: "fsync" ret: "int" args: "int" */
#define	LINUX_SYS_fsync	118

/* syscall: "sigreturn" ret: "int" args: */
#define	LINUX_SYS_sigreturn	119

/* syscall: "clone" ret: "int" args: "int" "void *" "void *" "void *" "void *" */
#define	LINUX_SYS_clone	120

/* syscall: "setdomainname" ret: "int" args: "char *" "int" */
#define	LINUX_SYS_setdomainname	121

/* syscall: "uname" ret: "int" args: "struct linux_utsname *" */
#define	LINUX_SYS_uname	122

/* syscall: "cacheflush" ret: "int" args: "unsigned long" "int" "int" "unsigned long" */
#define	LINUX_SYS_cacheflush	123

/* syscall: "mprotect" ret: "int" args: "const void *" "unsigned long" "int" */
#define	LINUX_SYS_mprotect	125

/* syscall: "sigprocmask" ret: "int" args: "int" "const linux_old_sigset_t *" "linux_old_sigset_t *" */
#define	LINUX_SYS_sigprocmask	126

/* syscall: "getpgid" ret: "pid_t" args: "pid_t" */
#define	LINUX_SYS_getpgid	132

/* syscall: "fchdir" ret: "int" args: "int" */
#define	LINUX_SYS_fchdir	133

/* syscall: "personality" ret: "int" args: "unsigned long" */
#define	LINUX_SYS_personality	136

/* syscall: "linux_setfsuid16" ret: "int" args: "uid_t" */
#define	LINUX_SYS_linux_setfsuid16	138

/* syscall: "linux_setfsgid16" ret: "int" args: "gid_t" */
#define	LINUX_SYS_linux_setfsgid16	139

/* syscall: "llseek" ret: "int" args: "int" "u_int32_t" "u_int32_t" "void *" "int" */
#define	LINUX_SYS_llseek	140

/* syscall: "getdents" ret: "int" args: "int" "struct linux_dirent *" "unsigned int" */
#define	LINUX_SYS_getdents	141

/* syscall: "select" ret: "int" args: "int" "fd_set *" "fd_set *" "fd_set *" "struct timeval50 *" */
#define	LINUX_SYS_select	142

/* syscall: "flock" ret: "int" args: "int" "int" */
#define	LINUX_SYS_flock	143

/* syscall: "__msync13" ret: "int" args: "void *" "size_t" "int" */
#define	LINUX_SYS___msync13	144

/* syscall: "readv" ret: "ssize_t" args: "int" "const struct iovec *" "int" */
#define	LINUX_SYS_readv	145

/* syscall: "writev" ret: "ssize_t" args: "int" "const struct iovec *" "int" */
#define	LINUX_SYS_writev	146

/* syscall: "getsid" ret: "pid_t" args: "pid_t" */
#define	LINUX_SYS_getsid	147

/* syscall: "fdatasync" ret: "int" args: "int" */
#define	LINUX_SYS_fdatasync	148

/* syscall: "__sysctl" ret: "int" args: "struct linux___sysctl *" */
#define	LINUX_SYS___sysctl	149

/* syscall: "mlock" ret: "int" args: "void *" "size_t" */
#define	LINUX_SYS_mlock	150

/* syscall: "munlock" ret: "int" args: "void *" "size_t" */
#define	LINUX_SYS_munlock	151

/* syscall: "mlockall" ret: "int" args: "int" */
#define	LINUX_SYS_mlockall	152

/* syscall: "munlockall" ret: "int" args: */
#define	LINUX_SYS_munlockall	153

/* syscall: "sched_setparam" ret: "int" args: "pid_t" "const struct linux_sched_param *" */
#define	LINUX_SYS_sched_setparam	154

/* syscall: "sched_getparam" ret: "int" args: "pid_t" "struct linux_sched_param *" */
#define	LINUX_SYS_sched_getparam	155

/* syscall: "sched_setscheduler" ret: "int" args: "pid_t" "int" "const struct linux_sched_param *" */
#define	LINUX_SYS_sched_setscheduler	156

/* syscall: "sched_getscheduler" ret: "int" args: "pid_t" */
#define	LINUX_SYS_sched_getscheduler	157

/* syscall: "sched_yield" ret: "int" args: */
#define	LINUX_SYS_sched_yield	158

/* syscall: "sched_get_priority_max" ret: "int" args: "int" */
#define	LINUX_SYS_sched_get_priority_max	159

/* syscall: "sched_get_priority_min" ret: "int" args: "int" */
#define	LINUX_SYS_sched_get_priority_min	160

/* syscall: "nanosleep" ret: "int" args: "const struct linux_timespec *" "struct linux_timespec *" */
#define	LINUX_SYS_nanosleep	162

/* syscall: "mremap" ret: "void *" args: "void *" "size_t" "size_t" "u_long" */
#define	LINUX_SYS_mremap	163

/* syscall: "setresuid16" ret: "int" args: "linux_uid16_t" "linux_uid16_t" "linux_uid16_t" */
#define	LINUX_SYS_setresuid16	164

/* syscall: "getresuid16" ret: "int" args: "linux_uid16_t *" "linux_uid16_t *" "linux_uid16_t *" */
#define	LINUX_SYS_getresuid16	165

/* syscall: "poll" ret: "int" args: "struct pollfd *" "u_int" "int" */
#define	LINUX_SYS_poll	168

/* syscall: "setresgid16" ret: "int" args: "linux_gid16_t" "linux_gid16_t" "linux_gid16_t" */
#define	LINUX_SYS_setresgid16	170

/* syscall: "getresgid16" ret: "int" args: "linux_gid16_t *" "linux_gid16_t *" "linux_gid16_t *" */
#define	LINUX_SYS_getresgid16	171

/* syscall: "rt_sigreturn" ret: "int" args: */
#define	LINUX_SYS_rt_sigreturn	173

/* syscall: "rt_sigaction" ret: "int" args: "int" "const struct linux_sigaction *" "struct linux_sigaction *" "size_t" */
#define	LINUX_SYS_rt_sigaction	174

/* syscall: "rt_sigprocmask" ret: "int" args: "int" "const linux_sigset_t *" "linux_sigset_t *" "size_t" */
#define	LINUX_SYS_rt_sigprocmask	175

/* syscall: "rt_sigpending" ret: "int" args: "linux_sigset_t *" "size_t" */
#define	LINUX_SYS_rt_sigpending	176

/* syscall: "rt_sigtimedwait" ret: "int" args: "const linux_sigset_t *" "linux_siginfo_t *" "const struct linux_timespec *" */
#define	LINUX_SYS_rt_sigtimedwait	177

/* syscall: "rt_queueinfo" ret: "int" args: "int" "int" "linux_siginfo_t *" */
#define	LINUX_SYS_rt_queueinfo	178

/* syscall: "rt_sigsuspend" ret: "int" args: "linux_sigset_t *" "size_t" */
#define	LINUX_SYS_rt_sigsuspend	179

/* syscall: "pread" ret: "int" args: "int" "char *" "size_t" "off_t" */
#define	LINUX_SYS_pread	180

/* syscall: "pwrite" ret: "int" args: "int" "char *" "size_t" "off_t" */
#define	LINUX_SYS_pwrite	181

/* syscall: "lchown16" ret: "int" args: "const char *" "linux_uid16_t" "linux_gid16_t" */
#define	LINUX_SYS_lchown16	182

/* syscall: "__getcwd" ret: "int" args: "char *" "size_t" */
#define	LINUX_SYS___getcwd	183

/* syscall: "sigaltstack" ret: "int" args: "const struct linux_sigaltstack *" "struct linux_sigaltstack *" */
#define	LINUX_SYS_sigaltstack	186

/* syscall: "__vfork14" ret: "int" args: */
#define	LINUX_SYS___vfork14	190

/* syscall: "ugetrlimit" ret: "int" args: "int" "struct orlimit *" */
#define	LINUX_SYS_ugetrlimit	191

#define linux_sys_mmap2_args linux_sys_mmap_args
/* syscall: "mmap2" ret: "linux_off_t" args: "unsigned long" "size_t" "int" "int" "int" "linux_off_t" */
#define	LINUX_SYS_mmap2	192

/* syscall: "truncate64" ret: "int" args: "const char *" "off_t" */
#define	LINUX_SYS_truncate64	193

/* syscall: "ftruncate64" ret: "int" args: "unsigned int" "off_t" */
#define	LINUX_SYS_ftruncate64	194

/* syscall: "stat64" ret: "int" args: "const char *" "struct linux_stat64 *" */
#define	LINUX_SYS_stat64	195

/* syscall: "lstat64" ret: "int" args: "const char *" "struct linux_stat64 *" */
#define	LINUX_SYS_lstat64	196

/* syscall: "fstat64" ret: "int" args: "int" "struct linux_stat64 *" */
#define	LINUX_SYS_fstat64	197

/* syscall: "__posix_chown" ret: "int" args: "const char *" "uid_t" "gid_t" */
#define	LINUX_SYS___posix_chown	198

/* syscall: "getuid" ret: "uid_t" args: */
#define	LINUX_SYS_getuid	199

/* syscall: "getgid" ret: "gid_t" args: */
#define	LINUX_SYS_getgid	200

/* syscall: "geteuid" ret: "uid_t" args: */
#define	LINUX_SYS_geteuid	201

/* syscall: "getegid" ret: "gid_t" args: */
#define	LINUX_SYS_getegid	202

/* syscall: "setreuid" ret: "int" args: "uid_t" "uid_t" */
#define	LINUX_SYS_setreuid	203

/* syscall: "setregid" ret: "int" args: "gid_t" "gid_t" */
#define	LINUX_SYS_setregid	204

/* syscall: "getgroups" ret: "int" args: "int" "gid_t *" */
#define	LINUX_SYS_getgroups	205

/* syscall: "setgroups" ret: "int" args: "int" "gid_t *" */
#define	LINUX_SYS_setgroups	206

/* syscall: "__posix_fchown" ret: "int" args: "int" "uid_t" "gid_t" */
#define	LINUX_SYS___posix_fchown	207

/* syscall: "setresuid" ret: "int" args: "uid_t" "uid_t" "uid_t" */
#define	LINUX_SYS_setresuid	208

/* syscall: "getresuid" ret: "int" args: "uid_t *" "uid_t *" "uid_t *" */
#define	LINUX_SYS_getresuid	209

/* syscall: "setresgid" ret: "int" args: "gid_t" "gid_t" "gid_t" */
#define	LINUX_SYS_setresgid	210

/* syscall: "getresgid" ret: "int" args: "gid_t *" "gid_t *" "gid_t *" */
#define	LINUX_SYS_getresgid	211

/* syscall: "__posix_lchown" ret: "int" args: "const char *" "uid_t" "gid_t" */
#define	LINUX_SYS___posix_lchown	212

/* syscall: "setuid" ret: "int" args: "uid_t" */
#define	LINUX_SYS_setuid	213

/* syscall: "setgid" ret: "int" args: "gid_t" */
#define	LINUX_SYS_setgid	214

/* syscall: "setfsuid" ret: "int" args: "uid_t" */
#define	LINUX_SYS_setfsuid	215

/* syscall: "setfsgid" ret: "int" args: "gid_t" */
#define	LINUX_SYS_setfsgid	216

/* syscall: "getdents64" ret: "int" args: "int" "struct linux_dirent64 *" "unsigned int" */
#define	LINUX_SYS_getdents64	220

/* syscall: "gettid" ret: "pid_t" args: */
#define	LINUX_SYS_gettid	221

/* syscall: "tkill" ret: "int" args: "int" "int" */
#define	LINUX_SYS_tkill	222

/* syscall: "setxattr" ret: "int" args: "char *" "char *" "void *" "size_t" "int" */
#define	LINUX_SYS_setxattr	223

/* syscall: "lsetxattr" ret: "int" args: "char *" "char *" "void *" "size_t" "int" */
#define	LINUX_SYS_lsetxattr	224

/* syscall: "fsetxattr" ret: "int" args: "int" "char *" "void *" "size_t" "int" */
#define	LINUX_SYS_fsetxattr	225

/* syscall: "getxattr" ret: "ssize_t" args: "char *" "char *" "void *" "size_t" */
#define	LINUX_SYS_getxattr	226

/* syscall: "lgetxattr" ret: "ssize_t" args: "char *" "char *" "void *" "size_t" */
#define	LINUX_SYS_lgetxattr	227

/* syscall: "fgetxattr" ret: "ssize_t" args: "int" "char *" "void *" "size_t" */
#define	LINUX_SYS_fgetxattr	228

/* syscall: "listxattr" ret: "ssize_t" args: "char *" "char *" "size_t" */
#define	LINUX_SYS_listxattr	229

/* syscall: "llistxattr" ret: "ssize_t" args: "char *" "char *" "size_t" */
#define	LINUX_SYS_llistxattr	230

/* syscall: "flistxattr" ret: "ssize_t" args: "int" "char *" "size_t" */
#define	LINUX_SYS_flistxattr	231

/* syscall: "removexattr" ret: "int" args: "char *" "char *" */
#define	LINUX_SYS_removexattr	232

/* syscall: "lremovexattr" ret: "int" args: "char *" "char *" */
#define	LINUX_SYS_lremovexattr	233

/* syscall: "fremovexattr" ret: "int" args: "int" "char *" */
#define	LINUX_SYS_fremovexattr	234

/* syscall: "futex" ret: "int" args: "int *" "int" "int" "const struct linux_timespec *" "int *" "int" */
#define	LINUX_SYS_futex	235

/* syscall: "mincore" ret: "int" args: "void *" "size_t" "char *" */
#define	LINUX_SYS_mincore	237

/* syscall: "madvise" ret: "int" args: "void *" "size_t" "int" */
#define	LINUX_SYS_madvise	238

/* syscall: "fcntl64" ret: "int" args: "int" "int" "void *" */
#define	LINUX_SYS_fcntl64	239

/* syscall: "fadvise64" ret: "int" args: "int" "off_t" "size_t" "int" */
#define	LINUX_SYS_fadvise64	246

/* syscall: "set_tid_address" ret: "int" args: "int *" */
#define	LINUX_SYS_set_tid_address	253

/* syscall: "clock_settime" ret: "int" args: "clockid_t" "struct linux_timespec *" */
#define	LINUX_SYS_clock_settime	259

/* syscall: "clock_gettime" ret: "int" args: "clockid_t" "struct linux_timespec *" */
#define	LINUX_SYS_clock_gettime	260

/* syscall: "clock_getres" ret: "int" args: "clockid_t" "struct linux_timespec *" */
#define	LINUX_SYS_clock_getres	261

/* syscall: "clock_nanosleep" ret: "int" args: "clockid_t" "int" "struct linux_timespec *" "struct linux_timespec *" */
#define	LINUX_SYS_clock_nanosleep	262

/* syscall: "statfs64" ret: "int" args: "const char *" "size_t" "struct linux_statfs64 *" */
#define	LINUX_SYS_statfs64	263

/* syscall: "fstatfs64" ret: "int" args: "int" "size_t" "struct linux_statfs64 *" */
#define	LINUX_SYS_fstatfs64	264

/* syscall: "tgkill" ret: "int" args: "int" "int" "int" */
#define	LINUX_SYS_tgkill	265

/* syscall: "utimes" ret: "int" args: "const char *" "const struct timeval50 *" */
#define	LINUX_SYS_utimes	266

/* syscall: "fadvise64_64" ret: "int" args: "int" "off_t" "off_t" "int" */
#define	LINUX_SYS_fadvise64_64	267

/* syscall: "openat" ret: "int" args: "int" "const char *" "int" "..." */
#define	LINUX_SYS_openat	288

/* syscall: "mkdirat" ret: "int" args: "int" "const char *" "linux_umode_t" */
#define	LINUX_SYS_mkdirat	289

/* syscall: "mknodat" ret: "int" args: "int" "const char *" "linux_umode_t" "unsigned" */
#define	LINUX_SYS_mknodat	290

/* syscall: "fchownat" ret: "int" args: "int" "const char *" "uid_t" "gid_t" "int" */
#define	LINUX_SYS_fchownat	291

/* syscall: "fstatat64" ret: "int" args: "int" "const char *" "struct linux_stat64 *" "int" */
#define	LINUX_SYS_fstatat64	293

/* syscall: "unlinkat" ret: "int" args: "int" "const char *" "int" */
#define	LINUX_SYS_unlinkat	294

/* syscall: "renameat" ret: "int" args: "int" "const char *" "int" "const char *" */
#define	LINUX_SYS_renameat	295

/* syscall: "linkat" ret: "int" args: "int" "const char *" "int" "const char *" "int" */
#define	LINUX_SYS_linkat	296

/* syscall: "symlinkat" ret: "int" args: "const char *" "int" "const char *" */
#define	LINUX_SYS_symlinkat	297

/* syscall: "readlinkat" ret: "ssize_t" args: "int" "const char *" "char *" "size_t" */
#define	LINUX_SYS_readlinkat	298

/* syscall: "fchmodat" ret: "int" args: "int" "const char *" "linux_umode_t" */
#define	LINUX_SYS_fchmodat	299

/* syscall: "faccessat" ret: "int" args: "int" "const char *" "int" */
#define	LINUX_SYS_faccessat	300

/* syscall: "pselect6" ret: "int" args: "int" "fd_set *" "fd_set *" "fd_set *" "struct linux_timespec *" "linux_sized_sigset_t *" */
#define	LINUX_SYS_pselect6	301

/* syscall: "ppoll" ret: "int" args: "struct pollfd *" "u_int" "struct linux_timespec *" "linux_sigset_t *" */
#define	LINUX_SYS_ppoll	302

/* syscall: "__futex_set_robust_list" ret: "int" args: "void *" "size_t" */
#define	LINUX_SYS___futex_set_robust_list	304

/* syscall: "__futex_get_robust_list" ret: "int" args: "lwpid_t" "void **" "size_t *" */
#define	LINUX_SYS___futex_get_robust_list	305

/* syscall: "sched_setaffinity" ret: "int" args: "pid_t" "unsigned int" "unsigned long *" */
#define	LINUX_SYS_sched_setaffinity	311

/* syscall: "sched_getaffinity" ret: "int" args: "pid_t" "unsigned int" "unsigned long *" */
#define	LINUX_SYS_sched_getaffinity	312

/* syscall: "utimensat" ret: "int" args: "int" "const char *" "struct linux_timespec *" "int" */
#define	LINUX_SYS_utimensat	316

/* syscall: "fallocate" ret: "int" args: "int" "int" "off_t" "off_t" */
#define	LINUX_SYS_fallocate	320

/* syscall: "dup3" ret: "int" args: "int" "int" "int" */
#define	LINUX_SYS_dup3	326

/* syscall: "pipe2" ret: "int" args: "int *" "int" */
#define	LINUX_SYS_pipe2	327

/* syscall: "accept4" ret: "int" args: "int" "struct osockaddr *" "int *" "int" */
#define	LINUX_SYS_accept4	361

/* syscall: "recvmmsg" ret: "int" args: "int" "struct linux_mmsghdr *" "unsigned int" "unsigned int" "struct timespec *" */
#define	LINUX_SYS_recvmmsg	371

/* syscall: "sendmmsg" ret: "int" args: "int" "struct linux_mmsghdr *" "unsigned int" "unsigned int" */
#define	LINUX_SYS_sendmmsg	372

#define	LINUX_SYS_MAXSYSCALL	379
#define	LINUX_SYS_NSYSENT	512
#endif /* _LINUX_SYS_SYSCALL_H_ */
