/*
 * 実環境での動作テスト用
 */

#include <errno.h>
#include <fcntl.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <util.h>
#include <sys/audioio.h>
#include <sys/cdefs.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/time.h>
#include <poll.h>
#include <sys/event.h>
#include <sys/sysctl.h>
#include <sys/wait.h>

#if !defined(AUDIO_ENCODING_SLINEAR_NE)
#if BYTE_ORDER == LITTLE_ENDIAN
#define AUDIO_ENCODING_SLINEAR_NE AUDIO_ENCODING_SLINEAR_LE
#define AUDIO_ENCODING_ULINEAR_NE AUDIO_ENCODING_ULINEAR_LE
#define AUDIO_ENCODING_SLINEAR_OE AUDIO_ENCODING_SLINEAR_BE
#define AUDIO_ENCODING_ULINEAR_OE AUDIO_ENCODING_ULINEAR_BE
#else
#define AUDIO_ENCODING_SLINEAR_NE AUDIO_ENCODING_SLINEAR_BE
#define AUDIO_ENCODING_ULINEAR_NE AUDIO_ENCODING_ULINEAR_BE
#define AUDIO_ENCODING_SLINEAR_OE AUDIO_ENCODING_SLINEAR_LE
#define AUDIO_ENCODING_ULINEAR_OE AUDIO_ENCODING_ULINEAR_LE
#endif
#endif

struct testtable {
	const char *name;
	void (*func)(void);
};

void init(int);

int debug;
int netbsd;
int props;
int hwfull;
char hwconfig[16];
char hwconfig9[16];	// audio%d
int x68k;
char testname[100];
char descname[100];
int testcount;
int failcount;
int expfcount;
int skipcount;
char devaudio[16];
char devsound[16];
char devaudioctl[16];
char devmixer[16];
extern struct testtable testtable[];

/* from audio.c */
static const char *encoding_names[] = {
	"none",
	AudioEmulaw,
	AudioEalaw,
	"pcm16",
	"pcm8",
	AudioEadpcm,
	AudioEslinear_le,
	AudioEslinear_be,
	AudioEulinear_le,
	AudioEulinear_be,
	AudioEslinear,
	AudioEulinear,
	AudioEmpeg_l1_stream,
	AudioEmpeg_l1_packets,
	AudioEmpeg_l1_system,
	AudioEmpeg_l2_stream,
	AudioEmpeg_l2_packets,
	AudioEmpeg_l2_system,
	AudioEac3,
};

void __attribute__((__noreturn__))
usage()
{
	// test は複数列挙できる。
	printf("usage: %s [<options>] {-a | <testname...>}\n", getprogname());
	printf("  -d: debug\n");
	printf("  -u <unit>: audio/sound device unit number (defualt:0)\n");
	printf(" testname:\n");
	for (int i = 0; testtable[i].name != NULL; i++) {
		printf("\t%s\n", testtable[i].name);
	}
	exit(1);
}

int
main(int ac, char *av[])
{
	int i;
	int c;
	int opt_all;
	int unit;

	testname[0] = '\0';
	descname[0] = '\0';
	props = -1;
	hwfull = 0;
	x68k = 0;
	unit = 0;

	// global option
	opt_all = 0;
	while ((c = getopt(ac, av, "adu:")) != -1) {
		switch (c) {
		 case 'a':
			opt_all = 1;
			break;
		 case 'd':
			debug++;
			break;
		 case 'u':
			unit = atoi(optarg);
			if (unit < 0) {
				printf("invalid device unit: %d\n", unit);
				exit(1);
			}
			break;
		 default:
			usage();
		}
	}
	ac -= optind;
	av += optind;

	init(unit);

	if (opt_all) {
		// -a なら引数なしで、全項目テスト
		if (ac > 0)
			usage();

		for (int j = 0; testtable[j].name != NULL; j++) {
			testtable[j].func();
			testname[0] = '\0';
			descname[0] = '\0';
		}
	} else {
		// -a なしなら test
		if (ac == 0)
			usage();

		// そうでなければ指定されたやつ(前方一致)を順にテスト
		for (i = 0; i < ac; i++) {
			bool found = false;
			for (int j = 0; testtable[j].name != NULL; j++) {
				if (strncmp(av[i], testtable[j].name, strlen(av[i])) == 0) {
					found = true;
					testtable[j].func();
					testname[0] = '\0';
					descname[0] = '\0';
				}
			}
			if (found == false) {
				printf("test not found: %s\n", av[i]);
				exit(1);
			}
		}
	}
	if (testcount > 0) {
		printf("Result: %d tests, %d success",
			testcount,
			testcount - failcount - expfcount - skipcount);
		if (failcount > 0)
			printf(", %d failed", failcount);
		if (expfcount > 0)
			printf(", %d expected failure", expfcount);
		if (skipcount > 0)
			printf(", %d skipped", skipcount);
		printf("\n");
	}
	return 0;
}

// err(3) ぽい自前関数
#define err(code, fmt...)	xp_err(code, __LINE__, fmt)
void xp_err(int, int, const char *, ...) __printflike(3, 4) __dead;
void
xp_err(int code, int line, const char *fmt, ...)
{
	va_list ap;
	int backup_errno;

	backup_errno = errno;
	printf(" ERR %d: ", line);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf(": %s\n", strerror(backup_errno));

	exit(code);
}

#define errx(code, fmt...)	xp_errx(code, __LINE__, fmt)
void xp_errx(int, int, const char *, ...) __printflike(3, 4) __dead;
void
xp_errx(int code, int line, const char *fmt, ...)
{
	va_list ap;

	printf(" ERR %d: ", line);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\n");

	exit(code);
}

void
init(int unit)
{
	audio_device_t dev;
	char name[256];
	size_t len;
	int r;
	int rel;
	int fd;

	snprintf(devaudio, sizeof(devaudio), "/dev/audio%d", unit);
	snprintf(devsound, sizeof(devsound), "/dev/sound%d", unit);
	snprintf(devaudioctl, sizeof(devaudioctl), "/dev/audioctl%d", unit);
	snprintf(devmixer, sizeof(devmixer), "/dev/mixer%d", unit);
	snprintf(hwconfig9, sizeof(hwconfig9), "audio%d", unit);
	if (debug)
		printf("unit = %d\n", unit);

	// バージョンを適当に判断。
	// 7 系なら 7
	// 8 系なら 8
	// 8.99 系で AUDIO2 なら 9
	// 8.99 系でそれ以外なら 8
	len = sizeof(rel);
	r = sysctlbyname("kern.osrevision", &rel, &len, NULL, 0);
	if (r == -1)
		err(1, "sysctl: osrevision");
	netbsd = rel / 100000000;
	if (netbsd == 8) {
		len = sizeof(name);
		r = sysctlbyname("kern.version", name, &len, NULL, 0);
		if (r == -1)
			err(1, "sysctl: version");
		if (strstr(name, "AUDIO2"))
			netbsd++;
	}

	if (debug)
		printf("netbsd = %d\n", netbsd);

	// デバイスのプロパティを取得
	// GETPROPS のための open/ioctl/close が信頼できるかどうかは悩ましいが
	// とりあえずね

	fd = open(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "init: open: %s", devaudio);
	r = ioctl(fd, AUDIO_GETPROPS, &props);
	if (r == -1)
		err(1, "init:AUDIO_GETPROPS");
	hwfull = (props & AUDIO_PROP_FULLDUPLEX) ? 1 : 0;

	// NetBSD8 の sysctl 用に MD 名を取得。
	// GETDEV で得られる値の定義がないので、現物合わせでよしなに頑張るorz
	r = ioctl(fd, AUDIO_GETDEV, &dev);
	if (r == -1)
		err(1, "init:AUDIO_GETDEV");
	if (strcmp(dev.config, "eap") == 0 ||
	    strcmp(dev.config, "vs") == 0)
	{
		// config に MD デバイス名(unit 番号なし)が入っているパターン
		// o eap (VMware Player)
		// o vs (x68k)
		// サウンドカードが2つはないのでへーきへーき…
		snprintf(hwconfig, sizeof(hwconfig), "%s0", dev.config);
	} else {
		// そうでなければ
		// MDデバイス名が入っていないパターン
		// o hdafg (mai)
		// config に MD デバイス名(unit 番号あり)が入ってるパターン
		// o auich (VirtualBox)

		// XXX orz
		if (strcmp(dev.config, "01h") == 0) {
			strlcpy(hwconfig, "hdafg0", sizeof(hwconfig));
		} else {
			strlcpy(hwconfig, dev.config, sizeof(hwconfig));
		}
	}
	if (debug)
		printf("hwconfig = %s\n", hwconfig);

	// ショートカット
	if (strcmp(dev.config, "vs") == 0)
		x68k = 1;
	close(fd);
}

// テスト名
static inline void TEST(const char *, ...) __printflike(1, 2);
static inline void
TEST(const char *name, ...)
{
	va_list ap;

	va_start(ap, name);
	vsnprintf(testname, sizeof(testname), name, ap);
	va_end(ap);
	printf("%s\n", testname);
	fflush(stdout);

	descname[0] = '\0';
}

// テスト詳細
static inline void DESC(const char *, ...) __printflike(1, 2);
static inline void
DESC(const char *name, ...)
{
	va_list ap;

	va_start(ap, name);
	vsnprintf(descname, sizeof(descname), name, ap);
	va_end(ap);

	if (debug)
		printf("%s(%s)\n", testname, descname);
}

// 検査

// XP_FAIL は呼び出し元でテストした上で失敗した時に呼ぶ。
// xp_fail はすでに testcount を追加した後で呼ぶ。
#define XP_FAIL(fmt...)	do {	\
	testcount++;	\
	xp_fail(__LINE__, fmt);	\
} while (0)
void xp_fail(int line, const char *fmt, ...)
{
	va_list ap;

	printf(" FAIL %d: %s", line, testname);
	if (descname[0])
		printf("(%s)", descname);
	printf(": ");
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\n");
	fflush(stdout);
	failcount++;
}

#define XP_EXPFAIL(fmt...)	do { \
	testcount++;	\
	xp_expfail(__LINE__, fmt);	\
} while (0)
void xp_expfail(int line, const char *fmt, ...)
{
	va_list ap;

	printf(" Expected Failure %d: %s", line, testname);
	if (descname[0])
		printf("(%s)", descname);
	printf(": ");
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\n");
	fflush(stdout);
	expfcount++;
}

#define XP_SKIP(fmt...)	do { \
	testcount++;	\
	xp_skip(__LINE__, fmt);	\
} while (0)
void xp_skip(int line, const char *fmt, ...)
{
	va_list ap;

	printf(" SKIP %d: %s", line, testname);
	if (descname[0])
		printf("(%s)", descname);
	printf(": ");
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\n");
	fflush(stdout);
	skipcount++;
}

#define XP_EQ(exp, act)	xp_eq(__LINE__, exp, act, #act)
void xp_eq(int line, int exp, int act, const char *varname)
{
	testcount++;
	if (exp != act)
		xp_fail(line, "%s expects %d but %d", varname, exp, act);
}
void xp_eq(int line, const char *exp, const char *act, const char *varname)
{
	testcount++;
	if (strcmp(exp, act) != 0)
		xp_fail(line, "%s expects \"%s\" but \"%s\"", varname, exp, act);
}

#define XP_NE(exp, act)	xp_ne(__LINE__, exp, act, #act)
void xp_ne(int line, int exp, int act, const char *varname)
{
	testcount++;
	if (exp == act)
		xp_fail(line, "%s expects != %d but %d", varname, exp, act);
}

// システムコールの結果 exp になることを期待
#define XP_SYS_EQ(exp, act)	xp_sys_eq(__LINE__, exp, act, #act)
void xp_sys_eq(int line, int exp, int act, const char *varname)
{
	testcount++;
	if (exp != act) {
		if (act == -1) {
			xp_fail(line, "%s expects %d but -1,err#%d(%s)", varname, exp,
				errno, strerror(errno));
		} else {
			xp_eq(line, exp, act, varname);
		}
	}
}

// システムコールの結果 exp 以外になることを期待
// エラーもテスト成功に含む
#define XP_SYS_NE(exp, act)	xp_sys_ne(__LINE__, exp, act, #act)
void xp_sys_ne(int line, int exp, int act, const char *varname)
{
	testcount++;
	if (act != -1) {
		xp_ne(line, exp, act, varname);
	}
}

// システムコールの結果成功することを期待
// open(2) のように返ってくる成功値が分からない場合用
#define XP_SYS_OK(act)	xp_sys_ok(__LINE__, act, #act)
void xp_sys_ok(int line, int act, const char *varname)
{
	testcount++;
	if (act == -1)
		xp_fail(line, "%s expects success but -1,err#%d(%s)",
			varname, errno, strerror(errno));
}
void xp_sys_ok(int line, void *act, const char *varname)
{
	testcount++;
	if (act == (void *)-1)
		xp_fail(line, "%s expects success but -1,err#%d(%s)",
			varname, errno, strerror(errno));
}

// システムコールがexperrnoで失敗することを期待
#define XP_SYS_NG(experrno, act) xp_sys_ng(__LINE__, experrno, act, #act)
void xp_sys_ng(int line, int experrno, int act, const char *varname)
{
	testcount++;
	if (act != -1) {
		xp_fail(line, "%s expects -1,err#%d but %d",
			varname, experrno, act);
	} else if (experrno != errno) {
		char acterrbuf[100];
		int acterrno = errno;
		strlcpy(acterrbuf, strerror(acterrno), sizeof(acterrbuf));
		xp_fail(line, "%s expects -1,err#%d(%s) but -1,err#%d(%s)",
			varname, experrno, strerror(experrno),
			acterrno, acterrbuf);
	}
}
void xp_sys_ng(int line, int experrno, void *act, const char *varname)
{
	testcount++;
	if (act != (void *)-1) {
		xp_fail(line, "%s expects -1,err#%d but %p",
			varname, experrno, act);
	} else if (experrno != errno) {
		char acterrbuf[100];
		int acterrno = errno;
		strlcpy(acterrbuf, strerror(acterrno), sizeof(acterrbuf));
		xp_fail(line, "%s expects -1,err#%d(%s) but -1,err#%d(%s)",
			varname, experrno, strerror(experrno),
			acterrno, acterrbuf);
	}
}

// ai.*.buffer_size が期待通りか調べる
// bool exp が true なら buffer_size の期待値は非ゼロ、
// exp が false なら buffer_size の期待値はゼロ。
#define XP_BUFFSIZE(exp, act)	xp_buffsize(__LINE__, exp, act, #act)
void xp_buffsize(int line, bool exp, int act, const char *varname)
{
	testcount++;
	if (exp) {
		if (act == 0)
			xp_fail(line, "%s expects non-zero but %d", varname, act);
	} else {
		if (act != 0)
			xp_fail(line, "%s expects zero but %d", varname, act);
	}
}

#define DPRINTF(fmt...)	do {	\
	if (debug)	\
		printf(fmt);	\
} while (0)

#define DPRINTFF(line, fmt...)	do {	\
	if (debug) {	\
		printf("  > %d: ", line);	\
		DPRINTF(fmt);	\
		fflush(stdout);	\
	}	\
} while (0)

#define DRESULT(r)	do {	\
	int backup_errno = errno;	\
	if ((r) == -1) {	\
		DPRINTF(" = %d, err#%d %s\n",	\
			r, backup_errno, strerror(backup_errno));	\
	} else {	\
		DPRINTF(" = %d\n", r);	\
	}	\
	errno = backup_errno;	\
	return r;	\
} while (0)

// ポインタ版 (mmap)
// -1 は -1 と表示してくれたほうが分かりやすい
#define DRESULT_PTR(r)	do {	\
	int backup_errno = errno;	\
	if ((r) == (void *)-1) {	\
		DPRINTF(" = -1, err#%d %s\n",	\
			backup_errno, strerror(backup_errno));	\
	} else {	\
		DPRINTF(" = %p\n", r);	\
	}	\
	errno = backup_errno;	\
	return r;	\
} while (0)

static const char *openmodetable[] = {
	"O_RDONLY",
	"O_WRONLY",
	"O_RDWR",
};
static const char *aumodetable[] __unused = {
	"RECORD",
	"PLAY",
	"PLAY|REC",
	"AUMODE_0",
};

// システムコールはこのマクロを経由して呼ぶ
#define OPEN(name, mode)	debug_open(__LINE__, name, mode)
int debug_open(int line, const char *name, int mode)
{
	char modestr[32];
	int n;

	if ((mode & 3) != 3)
		n = snprintf(modestr, sizeof(modestr), "%s", openmodetable[mode & 3]);
	else
		n = snprintf(modestr, sizeof(modestr), "%d", mode & 3);
	if (mode & O_NONBLOCK)
		n += snprintf(modestr + n, sizeof(modestr) - n, "|O_NONBLOCK");

	DPRINTFF(line, "open(\"%s\", %s)", name, modestr);
	int r = open(name, mode);
	DRESULT(r);
}

#define WRITE(fd, addr, len)	debug_write(__LINE__, fd, addr, len)
int debug_write(int line, int fd, const void *addr, size_t len)
{
	DPRINTFF(line, "write(%d, %p, %zd)", fd, addr, len);
	int r = write(fd, addr, len);
	DRESULT(r);
}

#define READ(fd, addr, len)	debug_read(__LINE__, fd, addr, len)
int debug_read(int line, int fd, void *addr, size_t len)
{
	DPRINTFF(line, "read(%d, %p, %zd)", fd, addr, len);
	int r = read(fd, addr, len);
	DRESULT(r);
}

// addrstr は値についてのコメント。ex.
//	int onoff = 0;
//	ioctl(fd, SWITCH, onoff); -> IOCTL(fd, SWITCH, onoff, "off")
#define IOCTL(fd, name, addr, addrfmt...)	\
	debug_ioctl(__LINE__, fd, name, #name, addr, addrfmt)
int debug_ioctl(int line, int fd, u_long name, const char *namestr,
	void *addr, const char *addrfmt, ...)
{
	char addrbuf[100];
	va_list ap;

	va_start(ap, addrfmt);
	vsnprintf(addrbuf, sizeof(addrbuf), addrfmt, ap);
	va_end(ap);
	DPRINTFF(line, "ioctl(%d, %s, %s)", fd, namestr, addrbuf);
	int r = ioctl(fd, name, addr);
	DRESULT(r);
}

#define FCNTL(fd, name...)	\
	debug_fcntl(__LINE__, fd, name, #name)
int debug_fcntl(int line, int fd, int name, const char *namestr, ...)
{
	int r;

	switch (name) {
	 case F_GETFL:	// 引数なし
		DPRINTFF(line, "fcntl(%d, %s)", fd, namestr);
		r = fcntl(fd, name);
		break;
	}
	DRESULT(r);
}

#define CLOSE(fd)	debug_close(__LINE__, fd)
int debug_close(int line, int fd)
{
	DPRINTFF(line, "close(%d)", fd);
	int r = close(fd);
	DRESULT(r);
}

#define MMAP(ptr, len, prot, flags, fd, offset)	\
	debug_mmap(__LINE__, ptr, len, prot, flags, fd, offset)
void *debug_mmap(int line, void *ptr, int len, int prot, int flags, int fd,
	int offset)
{
	char protbuf[256];
	char flagbuf[256];
	int n;

#define ADDFLAG(buf, var, name)	do {	\
	if (((var) & (name)))	\
		n = strlcat(buf, "|" #name, sizeof(buf));	\
		var &= ~(name);	\
} while (0)

	n = 0;
	protbuf[n] = '\0';
	if (prot == 0) {
		strlcpy(protbuf, "|PROT_NONE", sizeof(protbuf));
	} else {
		ADDFLAG(protbuf, prot, PROT_EXEC);
		ADDFLAG(protbuf, prot, PROT_WRITE);
		ADDFLAG(protbuf, prot, PROT_READ);
		if (prot != 0)
			snprintf(protbuf + n, sizeof(protbuf) - n, "|prot=0x%x", prot);
	}

	n = 0;
	flagbuf[n] = '\0';
	if (flags == 0) {
		strlcpy(flagbuf, "|MAP_FILE", sizeof(flagbuf));
	} else {
		ADDFLAG(flagbuf, flags, MAP_SHARED);
		ADDFLAG(flagbuf, flags, MAP_PRIVATE);
		ADDFLAG(flagbuf, flags, MAP_FIXED);
		ADDFLAG(flagbuf, flags, MAP_INHERIT);
		ADDFLAG(flagbuf, flags, MAP_HASSEMAPHORE);
		ADDFLAG(flagbuf, flags, MAP_TRYFIXED);
		ADDFLAG(flagbuf, flags, MAP_WIRED);
		ADDFLAG(flagbuf, flags, MAP_ANON);
		if ((flags & MAP_ALIGNMENT_MASK)) {
			n += snprintf(flagbuf + n, sizeof(flagbuf) - n, "|MAP_ALIGN(%d)",
				((flags & MAP_ALIGNMENT_MASK) >> MAP_ALIGNMENT_SHIFT));
			flags &= ~MAP_ALIGNMENT_MASK;
		}
		if (flags != 0)
			n += snprintf(flagbuf + n, sizeof(flagbuf) - n, "|flag=0x%x",
				flags);
	}

	DPRINTFF(line, "mmap(%p, %d, %s, %s, %d, %d)",
		ptr, len, protbuf + 1, flagbuf + 1, fd, offset);
	void *r = mmap(ptr, len, prot, flags, fd, offset);
	DRESULT_PTR(r);
}

#define MUNMAP(ptr, len)	debug_munmap(__LINE__, ptr, len)
int debug_munmap(int line, void *ptr, int len)
{
	DPRINTFF(line, "munmap(%p, %d)", ptr, len);
	int r = munmap(ptr, len);
	DRESULT(r);
}

#define POLL(pfd, nfd, timeout)	debug_poll(__LINE__, pfd, nfd, timeout)
int debug_poll(int line, struct pollfd *pfd, int nfd, int timeout)
{
	char buf[256];
	int n = 0;
	buf[n] = '\0';
	for (int i = 0; i < nfd; i++) {
		n += snprintf(buf + n, sizeof(buf) - n, "{fd=%d,events=%d}",
			pfd[i].fd, pfd[i].events);
	}
	DPRINTFF(line, "poll(%s, %d, %d)", buf, nfd, timeout);
	int r = poll(pfd, nfd, timeout);
	DRESULT(r);
}

#define KQUEUE()	debug_kqueue(__LINE__)
int debug_kqueue(int line)
{
	DPRINTFF(line, "kqueue()");
	int r = kqueue();
	DRESULT(r);
}

#define KEVENT_SET(kq, kev, nev)	debug_kevent_set(__LINE__, kq, kev, nev)
int debug_kevent_set(int line, int kq, struct kevent *kev, size_t nev)
{
	DPRINTFF(line, "kevent_set(%d, %p, %zd)", kq, kev, nev);
	int r = kevent(kq, kev, nev, NULL, 0, NULL);
	DRESULT(r);
}

#define KEVENT_POLL(kq, kev, nev, ts) \
	debug_kevent_poll(__LINE__, kq, kev, nev, ts)
int debug_kevent_poll(int line, int kq, struct kevent *kev, size_t nev,
	const struct timespec *ts)
{
	char tsbuf[32];

	if (ts == NULL) {
		snprintf(tsbuf, sizeof(tsbuf), "NULL");
	} else if (ts->tv_sec == 0 && ts->tv_nsec == 0) {
		snprintf(tsbuf, sizeof(tsbuf), "0.0");
	} else {
		snprintf(tsbuf, sizeof(tsbuf), "%d.%09d",
			(int)ts->tv_sec, (int)ts->tv_nsec);
	}
	DPRINTFF(line, "kevent_poll(%d, %p, %zd, %s)", kq, kev, nev, tsbuf);
	int r = kevent(kq, NULL, 0, kev, nev, ts);
	DRESULT(r);
}

#define DEBUG_KEV(name, kev)	debug_kev(__LINE__, name, kev)
void debug_kev(int line, const char *name, const struct kevent *kev)
{
	char flagbuf[256];
	const char *filterbuf;
	uint32_t v;
	int n;

	n = 0;
	flagbuf[n] = '\0';
	if (kev->flags == 0) {
		strcpy(flagbuf, "|0?");
	} else {
		v = kev->flags;
		ADDFLAG(flagbuf, v, EV_ADD);
		if (v != 0)
			snprintf(flagbuf + n, sizeof(flagbuf)-n, "|0x%x", v);
	}

	switch (kev->filter) {
	 case EVFILT_READ:	filterbuf = "EVFILT_READ";	break;
	 case EVFILT_WRITE:	filterbuf = "EVFILT_WRITE";	break;
	 default:			filterbuf = "EVFILT_?";		break;
	}

	DPRINTFF(line,
		"%s={id:%d,%s,%s,fflags:0x%x,data:0x%" PRIx64 ",udata:0x%x}\n",
		name,
		(int)kev->ident,
		flagbuf + 1,
		filterbuf,
		kev->fflags,
		kev->data,
		(int)kev->udata);
}

#define GETUID()	debug_getuid(__LINE__)
uid_t debug_getuid(int line)
{
	DPRINTFF(line, "getuid");
	uid_t r = getuid();
	DRESULT(r);
}

#define SETEUID(id)	debug_seteuid(__LINE__, id)
int debug_seteuid(int line, uid_t id)
{
	DPRINTFF(line, "seteuid(%d)", (int)id);
	int r = seteuid(id);
	DRESULT(r);
}

#define SYSCTLBYNAME(name, oldp, oldlenp, newp, newlen)	\
	debug_sysctlbyname(__LINE__, name, oldp, oldlenp, newp, newlen)
int debug_sysctlbyname(int line, const char *name, void *oldp, size_t *oldlenp,
	const void *newp, size_t newlen)
{
	DPRINTFF(line, "sysctlbyname(\"%s\")", name);
	int r = sysctlbyname(name, oldp, oldlenp, newp, newlen);
	DRESULT(r);
}

#define SYSTEM(cmd)	debug_system(__LINE__, cmd)
int debug_system(int line, const char *cmd)
{
	DPRINTFF(line, "system(\"%s\")", cmd);
	int r = system(cmd);
	DRESULT(r);
}

// popen() して1行目を返す
#define POPEN_GETS(buf, buflen, cmd...) \
	debug_popen_gets(__LINE__, buf, buflen, cmd)
int debug_popen_gets(int line, char *buf, size_t bufsize, const char *cmd, ...)
{
	char cmdbuf[256];
	va_list ap;
	FILE *fp;
	char *p;

	va_start(ap, cmd);
	vsnprintf(cmdbuf, sizeof(cmdbuf), cmd, ap);
	va_end(ap);

	DPRINTFF(line, "popen(\"%s\", \"r\")", cmdbuf);
	fp = popen(cmdbuf, "r");
	if (fp == NULL) {
		DPRINTF(" = NULL, popen failed\n");
		return -1;
	}
	if (fgets(buf, bufsize, fp) == NULL) {
		DPRINTF(" = NULL, fgets failed\n");
		pclose(fp);
		return -1;
	}

	p = strchr(buf, '\n');
	if (p)
		*p = '\0';

	DPRINTF(" = \"%s\"\n", buf);
	return 0;
}

// ---

// sysctl で使う HW デバイス名を返します。
// N8 では MD 名(vs0 とか)、
// N9 では audioN です。
const char *
hwconfigname()
{
	if (netbsd <= 8) {
		return hwconfig;
	} else {
		return hwconfig9;
	}
}

// ---

// O_* を AUMODE_* に変換。HW Full 固定
int mode2aumode_full[] = {
	AUMODE_RECORD,
	AUMODE_PLAY | AUMODE_PLAY_ALL,
	AUMODE_PLAY | AUMODE_PLAY_ALL | AUMODE_RECORD,
};
// O_* を AUMODE_* に変換 (hwfull を考慮)
int mode2aumode(int mode)
{
	int aumode = mode2aumode_full[mode];
	if (hwfull == 0 && mode == O_RDWR)
		aumode &= ~AUMODE_RECORD;
	return aumode;
}

// O_* を PLAY 側がオープンされているかに変換。HW Full 固定
int mode2popen_full[] = {
	0, 1, 1,
};
// O_* を PLAY 側がオープンされてるかに変換 (hwfull を考慮、ただし同じになる)
int mode2popen(int mode)
{
	return mode2popen_full[mode];
}

// O_* を RECORD 側がオープンされてるかに変換。HW Full 固定
int mode2ropen_full[] = {
	1, 0, 1,
};
// O_* を RECORD 側がオープンされてるかに変換 (hwfull を考慮)
int mode2ropen(int mode)
{
	int rec = mode2ropen_full[mode];
	if (hwfull == 0 && mode == O_RDWR)
		rec = 0;
	return rec;
}


// オープンモードによるオープン直後の状態を調べる
void
test_open_1(void)
{
	struct audio_info ai;
	int fd;
	int r;

	// 再生専用デバイスのテストとかはまた
	TEST("open_1");
	for (int mode = 0; mode <= 2; mode++) {
		DESC("%s", openmodetable[mode]);
		fd = OPEN(devaudio, mode);
		XP_SYS_OK(fd);

		memset(&ai, 0, sizeof(ai));
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);
		XP_EQ(0, ai.play.pause);
		XP_EQ(0, ai.record.pause);
		XP_EQ(mode2popen_full[mode], ai.play.open);
		XP_EQ(mode2ropen_full[mode], ai.record.open);
		// ai.mode は open_5 で調べている

		if (netbsd <= 8) {
			// N7、N8 では使わないほうのトラックのバッファも常にある
			XP_NE(0, ai.play.buffer_size);
			XP_NE(0, ai.record.buffer_size);
		} else {
			// AUDIO2 では使わないほうのバッファは確保してない
			XP_BUFFSIZE(mode2popen_full[mode], ai.play.buffer_size);
			XP_BUFFSIZE(mode2ropen_full[mode], ai.record.buffer_size);
		}

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
	}
}

// /dev/audio は何回開いても初期値は同じ。
// /dev/audio の初期値確認、いろいろ変更して close、もう一度開いて初期値確認。
void
test_open_2(void)
{
	struct audio_info ai, ai0;
	int channels;
	int fd;
	int r;
	bool pbuff, rbuff;
	int buff_size;

	TEST("open_2");
	for (int mode = 0; mode <= 2; mode++) {
		DESC("%s", openmodetable[mode]);

		// N7、N8 では常に両方のバッファが存在する
		// AUDIO2 では mode による
		if (netbsd <= 8) {
			pbuff = true;
			rbuff = true;
		} else {
			pbuff = mode2popen_full[mode];
			rbuff = mode2ropen_full[mode];
		}

		// オープンして初期値をチェック
		fd = OPEN(devaudio, mode);
		if (fd == -1)
			err(1, "open");
		memset(&ai, 0, sizeof(ai));
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);

		XP_NE(0, ai.blocksize);
		buff_size = (mode == O_RDONLY)
			? ai.record.buffer_size
			: ai.play.buffer_size;
		if (netbsd == 9 && mode == O_RDONLY) {
			// AUDIO2 では RDONLY なら play track がなく rec track が見える
			// どうしたものか。
			XP_EQ(buff_size / ai.blocksize - 1, ai.hiwat);
			XP_EQ(0, ai.lowat);
		} else {
			// それ以外は play track が見えるべき。
			XP_EQ(buff_size / ai.blocksize, ai.hiwat);
			XP_EQ(buff_size * 3 / 4 / ai.blocksize, ai.lowat);
		}
		XP_EQ(mode2aumode(mode), ai.mode);

		// play
		XP_EQ(8000, ai.play.sample_rate);
		XP_EQ(1, ai.play.channels);
		XP_EQ(8, ai.play.precision);
		XP_EQ(AUDIO_ENCODING_ULAW, ai.play.encoding);
		// gain
		// port
		XP_EQ(0, ai.play.seek);
		// avail_ports
		XP_BUFFSIZE(pbuff, ai.play.buffer_size);
		XP_EQ(0, ai.play.samples);
		XP_EQ(0, ai.play.eof);
		XP_EQ(0, ai.play.pause);
		XP_EQ(0, ai.play.error);
		XP_EQ(0, ai.play.waiting);
		// balance
		XP_EQ(mode2popen_full[mode], ai.play.open);
		XP_EQ(0, ai.play.active);
		// record
		XP_EQ(8000, ai.record.sample_rate);
		XP_EQ(1, ai.record.channels);
		XP_EQ(8, ai.record.precision);
		XP_EQ(AUDIO_ENCODING_ULAW, ai.record.encoding);
		// gain
		// port
		if (netbsd == 8 && ai.record.seek != 0) {
			// N8 では録音オープンしただけで録音が始まる(場合がある?)
			XP_EXPFAIL("recording was started on open");
		} else {
			XP_EQ(0, ai.record.seek);
		}
		// avail_ports
		XP_BUFFSIZE(rbuff, ai.record.buffer_size);
		if (netbsd == 8 && ai.record.samples != 0) {
			// N8 では録音オープンしただけで録音が始まる(場合がある?)
			XP_EXPFAIL("recording was started on open");
		} else {
			XP_EQ(0, ai.record.samples);
		}
		XP_EQ(0, ai.record.eof);
		XP_EQ(0, ai.record.pause);
		XP_EQ(0, ai.record.error);
		XP_EQ(0, ai.record.waiting);
		// balance
		XP_EQ(mode2ropen_full[mode], ai.record.open);
		if (netbsd <= 7) {
			// N7 は録音が有効ならオープン直後から active になるらしい。
			XP_EQ(mode2ropen(mode), ai.record.active);
		} else if (netbsd == 8 && ai.record.active) {
			// N8 は録音オープンしただけで録音が始まる(場合がある?)
			XP_EXPFAIL("recording was started on open");
		} else {
			// AUDIO2 はオープンしただけではまだアクティブにならない。
			XP_EQ(0, ai.record.active);
		}
		// これを保存しておく
		ai0 = ai;

		// できるだけ変更
		channels = (netbsd <= 7 && x68k) ? 1 : 2;
		AUDIO_INITINFO(&ai);
		ai.blocksize = ai0.blocksize * 2;
		if (ai0.hiwat > 0)
			ai.hiwat = ai0.hiwat - 1;
		if (ai0.lowat < ai0.hiwat)
			ai.lowat = ai0.lowat + 1;
		ai.mode = ai.mode & ~AUMODE_PLAY_ALL;
		ai.play.sample_rate = 11025;
		ai.play.channels = channels;
		ai.play.precision = 16;
		ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
		ai.play.pause = 1;
		ai.record.sample_rate = 11025;
		ai.record.channels = channels;
		ai.record.precision = 16;
		ai.record.encoding = AUDIO_ENCODING_SLINEAR_LE;
		ai.record.pause = 1;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "ai");
		if (r == -1)
			err(1, "AUDIO_SETINFO");
		r = CLOSE(fd);
		XP_SYS_EQ(0, r);

		// 再オープンしてチェック
		fd = OPEN(devaudio, mode);
		if (fd == -1)
			err(1, "open");
		memset(&ai, 0, sizeof(ai));
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);

		XP_EQ(ai0.blocksize, ai.blocksize);
		XP_EQ(ai0.hiwat, ai.hiwat);
		XP_EQ(ai0.lowat, ai.lowat);
		XP_EQ(mode2aumode(mode), ai.mode);
		// play
		XP_EQ(8000, ai.play.sample_rate);
		XP_EQ(1, ai.play.channels);
		XP_EQ(8, ai.play.precision);
		XP_EQ(AUDIO_ENCODING_ULAW, ai.play.encoding);
		// gain
		// port
		XP_EQ(0, ai.play.seek);
		// avail_ports
		XP_EQ(ai0.play.buffer_size, ai.play.buffer_size);
		XP_EQ(0, ai.play.samples);
		XP_EQ(0, ai.play.eof);
		XP_EQ(0, ai.play.pause);
		XP_EQ(0, ai.play.error);
		XP_EQ(0, ai.play.waiting);
		// balance
		XP_EQ(mode2popen_full[mode], ai.play.open);
		XP_EQ(0, ai.play.active);
		// record
		XP_EQ(8000, ai.record.sample_rate);
		XP_EQ(1, ai.record.channels);
		XP_EQ(8, ai.record.precision);
		XP_EQ(AUDIO_ENCODING_ULAW, ai.record.encoding);
		// gain
		// port
		if (netbsd == 8 && ai.record.seek != 0) {
			// N8 では録音オープンしただけで録音が始まる(場合がある?)
			XP_EXPFAIL("recording was started on open");
		} else {
			XP_EQ(0, ai.record.seek);
		}
		// avail_ports
		XP_EQ(ai0.record.buffer_size, ai.record.buffer_size);
		if (netbsd == 8 && ai.record.samples != 0) {
			// N8 では録音オープンしただけで録音が始まる(場合がある?)
			XP_EXPFAIL("recording was started on open");
		} else {
			XP_EQ(0, ai.record.samples);
		}
		XP_EQ(0, ai.record.eof);
		XP_EQ(0, ai.record.pause);
		XP_EQ(0, ai.record.error);
		XP_EQ(0, ai.record.waiting);
		// balance
		XP_EQ(mode2ropen_full[mode], ai.record.open);
		if (netbsd <= 7) {
			// N7 は録音が有効ならオープン直後から active になるらしい。
			XP_EQ(mode2ropen(mode), ai.record.active);
		} else if (netbsd == 8 && ai.record.active) {
			// N8 は録音オープンしただけで録音が始まる(場合がある?)
			XP_EXPFAIL("recording was started on open");
		} else {
			// オープンしただけではまだアクティブにならない。
			XP_EQ(0, ai.record.active);
		}

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
	}
}

// /dev/sound は前回の値がみえる
// /dev/audio を一旦開いて初期化しておき、
// /dev/sound の初期値がそれになることを確認、
// いろいろ変更して close、もう一度開いて残っていることを確認。
void
test_open_3(void)
{
	struct audio_info ai, ai0;
	int channels;
	int fd;
	int r;
	int aimode;
	bool pbuff, rbuff;
	int buff_size;

	TEST("open_3");

	// N8 eap だと panic する。
	// ncmd.cpp の cmd_eap_input 参照。
	if (netbsd == 8 && strncmp(hwconfig, "eap", 3) == 0) {
		XP_EXPFAIL("it causes panic on NetBSD8 + eap");
		return;
	}

	for (int mode = 0; mode <= 2; mode++) {
		DESC("%s", openmodetable[mode]);

		// N7、N8 では常に両方のバッファが存在する
		// AUDIO2 では mode による
		if (netbsd <= 8) {
			pbuff = true;
			rbuff = true;
		} else {
			pbuff = mode2popen_full[mode];
			rbuff = mode2ropen_full[mode];
		}

		// まず /dev/audio を RDWR で開いて両方初期化させておく。
		// ただし NetBSD8 は audio と sound が分離されてるので別コード。
		if (netbsd == 8) {
			fd = OPEN(devsound, O_RDWR);
			if (fd == -1)
				err(1, "open");
			AUDIO_INITINFO(&ai);
			ai.play.encoding = AUDIO_ENCODING_ULAW;
			ai.play.precision = 8;
			ai.play.channels = 1;
			ai.play.sample_rate = 8000;
			ai.play.pause = 0;
			ai.record = ai.play;
			r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
			XP_SYS_EQ(0, r);
		} else {
			fd = OPEN(devaudio, O_RDWR);
			if (fd == -1)
				err(1, "open");
		}
		r = CLOSE(fd);
		XP_SYS_EQ(0, r);

		// オープンして初期値をチェック
		fd = OPEN(devsound, mode);
		if (fd == -1)
			err(1, "open");
		memset(&ai, 0, sizeof(ai));
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);

		// audio の初期値と同じものが見えるはず
		XP_NE(0, ai.blocksize);
		buff_size = (mode == O_RDONLY)
			? ai.record.buffer_size
			: ai.play.buffer_size;
		if (netbsd == 9 && mode == O_RDONLY) {
			// AUDIO2 では RDONLY なら play track がなく rec track が見える
			// どうしたものか。
			XP_EQ(buff_size / ai.blocksize - 1, ai.hiwat);
			XP_EQ(0, ai.lowat);
		} else {
			// それ以外は play track が見えるべき。
			XP_EQ(buff_size / ai.blocksize, ai.hiwat);
			XP_EQ(buff_size * 3 / 4 / ai.blocksize, ai.lowat);
		}
		XP_EQ(mode2aumode(mode), ai.mode);
		aimode = ai.mode;
		// play
		XP_EQ(8000, ai.play.sample_rate);
		XP_EQ(1, ai.play.channels);
		XP_EQ(8, ai.play.precision);
		XP_EQ(AUDIO_ENCODING_ULAW, ai.play.encoding);
		// gain
		// port
		XP_EQ(0, ai.play.seek);
		// avail_ports
		XP_BUFFSIZE(pbuff, ai.play.buffer_size);
		XP_EQ(0, ai.play.samples);
		XP_EQ(0, ai.play.eof);
		XP_EQ(0, ai.play.pause);
		XP_EQ(0, ai.play.error);
		XP_EQ(0, ai.play.waiting);
		// balance
		XP_EQ(mode2popen_full[mode], ai.play.open);
		XP_EQ(0, ai.play.active);
		// record
		XP_EQ(8000, ai.record.sample_rate);
		XP_EQ(1, ai.record.channels);
		XP_EQ(8, ai.record.precision);
		XP_EQ(AUDIO_ENCODING_ULAW, ai.record.encoding);
		// gain
		// port
		if (netbsd == 8 && ai.record.seek != 0) {
			// N8 では録音オープンしただけで録音が始まる(場合がある?)
			XP_EXPFAIL("recording was started on open");
		} else {
			XP_EQ(0, ai.record.seek);
		}
		// avail_ports
		XP_BUFFSIZE(rbuff, ai.record.buffer_size);
		if (netbsd == 8 && ai.record.samples != 0) {
			// N8 では録音オープンしただけで録音が始まる(場合がある?)
			XP_EXPFAIL("recording was started on open");
		} else {
			XP_EQ(0, ai.record.samples);
		}
		XP_EQ(0, ai.record.eof);
		XP_EQ(0, ai.record.pause);
		XP_EQ(0, ai.record.error);
		XP_EQ(0, ai.record.waiting);
		// balance
		XP_EQ(mode2ropen_full[mode], ai.record.open);
		if (netbsd == 8 && ai.record.active) {
			// N8 では再生モードによらずオープンしただけで録音アクティブが
			// 立つ(場合がある?)
			XP_EXPFAIL("recording was started on open");
		} else {
			XP_EQ(0, ai.record.active);
		}

		// できるだけ変更
		ai0 = ai;
		AUDIO_INITINFO(&ai);
		channels = (netbsd <= 7 && x68k) ? 1 : 2;
		ai.blocksize = ai0.blocksize * 2;
		ai.mode = aimode & ~AUMODE_PLAY_ALL;
		ai.play.sample_rate = 11025;
		ai.play.channels = channels;
		ai.play.precision = 16;
		ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
		ai.play.pause = 1;
		ai.record.sample_rate = 11025;
		ai.record.channels = channels;
		ai.record.precision = 16;
		ai.record.encoding = AUDIO_ENCODING_SLINEAR_LE;
		ai.record.pause = 1;
		ai.hiwat = ai0.hiwat - 1;
		ai.lowat = ai0.lowat + 1;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "ai");
		if (r == -1)
			err(1, "AUDIO_SETINFO");
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai0, "ai0");
		if (r == -1)
			err(1, "AUDIO_GETBUFINFO");
		r = CLOSE(fd);
		XP_SYS_EQ(0, r);

		// 再オープンしてチェック
		fd = OPEN(devsound, mode);
		if (fd == -1)
			err(1, "open");
		memset(&ai, 0, sizeof(ai));
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);

		if (netbsd == 8) {
			// 期待値未検証。
		} else {
			XP_EQ(ai0.blocksize, ai.blocksize);
		}
		// hiwat, lowat は引き継がない
		if (netbsd == 9 && mode == O_RDONLY) {
			// AUDIO2 では RDONLY なら play track がなく rec track が見える
			// どうしたものか。
			XP_EQ(buff_size / ai.blocksize - 1, ai.hiwat);
			XP_EQ(0, ai.lowat);
		} else {
			// それ以外は play track が見えるべき。
			XP_EQ(buff_size / ai.blocksize, ai.hiwat);
			XP_EQ(buff_size * 3 / 4 / ai.blocksize, ai.lowat);
		}
		// mode は引き継がない
		XP_EQ(mode2aumode(mode), ai.mode);
		// play
		XP_EQ(ai0.play.sample_rate, ai.play.sample_rate);
		XP_EQ(ai0.play.channels, ai.play.channels);
		XP_EQ(ai0.play.precision, ai.play.precision);
		XP_EQ(ai0.play.encoding, ai.play.encoding);
		// gain
		// port
		XP_EQ(0, ai.play.seek);
		// avail_ports
		XP_BUFFSIZE(pbuff, ai.play.buffer_size);
		XP_EQ(0, ai.play.samples);
		XP_EQ(0, ai.play.eof);
		XP_EQ(ai0.play.pause, ai.play.pause);
		XP_EQ(0, ai.play.error);
		XP_EQ(0, ai.play.waiting);
		// balance
		XP_EQ(mode2popen_full[mode], ai.play.open);
		XP_EQ(0, ai.play.active);
		// record
		XP_EQ(ai0.record.sample_rate, ai.record.sample_rate);
		XP_EQ(ai0.record.channels, ai.record.channels);
		XP_EQ(ai0.record.precision, ai.record.precision);
		XP_EQ(ai0.record.encoding, ai.record.encoding);
		// gain
		// port
		XP_EQ(0, ai.record.seek);
		// avail_ports
		XP_BUFFSIZE(rbuff, ai.record.buffer_size);
		XP_EQ(0, ai.record.samples);
		XP_EQ(0, ai.record.eof);
		XP_EQ(ai0.record.pause, ai.record.pause);
		XP_EQ(0, ai.record.error);
		XP_EQ(0, ai.record.waiting);
		// balance
		XP_EQ(mode2ropen_full[mode], ai.record.open);
		XP_EQ(0, ai.record.active);

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
	}
}

// /dev/sound -> /dev/audio -> /dev/sound と開くと2回目の sound は
// audio の設定に影響される。
// /dev/audio と /dev/sound の設定は互いに独立しているわけではなく、
// 内部設定は1つで、
// /dev/sound はそれを継承して使い、/dev/audio はそれを初期化して使う。
// というイメージのようだ。
void
test_open_4(void)
{
	struct audio_info ai;
	int fd;
	int r;

	TEST("open_4");

	// まず /dev/sound を開いて適当に設定。
	// 代表値でエンコーディングだけ変える。
	// このケースでだけ open_2、open_3 とは挙動が違う項目が一つだけ
	// あるとかだと捕捉できないが、さすがにいいだろう…。
	fd = OPEN(devsound, O_WRONLY);
	if (fd == -1)
		err(1, "open");
	AUDIO_INITINFO(&ai);
	ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	if (r == -1)
		err(1, "AUDIO_SETINFO");
	CLOSE(fd);

	// 続いて /dev/audio をオープン。オープンしただけで自身は mulaw になる
	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");
	CLOSE(fd);

	// 再び /dev/sound をオープン。
	fd = OPEN(devsound, O_WRONLY);
	if (fd == -1)
		err(1, "open");
	memset(&ai, 0, sizeof(ai));
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	if (r == -1)
		err(1, "AUDIO_GETBUFINFO");
	if (netbsd == 8 && ai.play.encoding == AUDIO_ENCODING_SLINEAR_LE) {
		// N8 は /dev/sound の引き継ぎは /dev/sound 同士に変えたようだ
		XP_EXPFAIL("/dev/sound inherits only from /dev/sound on N8");
	} else {
		// N7, AUDIO2 は直前の設定を引き継ぐ
		XP_EQ(AUDIO_ENCODING_ULAW, ai.play.encoding);
	}
	CLOSE(fd);
}

// オープン、多重オープン時の mode
// N8 では HW Full/Half によらず常に full duplex かのようにオープン出来るが
// read/write がおかしいので、よくない。
// AUDIO2 では HW Half ならおかしい組み合わせは弾く。
void
test_open_5()
{
	struct audio_info ai;
	int fd0, fd1;
	int r;
	int actmode;
#define AUMODE_BOTH (AUMODE_PLAY | AUMODE_RECORD)
	struct {
		int mode0;
		int mode1;
	} expfulltable[] = {
		// fd0 の期待値		fd1 の期待値(-errnoはエラー)
		{ AUMODE_RECORD,	AUMODE_RECORD },	// REC, REC
		{ AUMODE_RECORD,	AUMODE_PLAY },		// REC, PLAY
		{ AUMODE_RECORD,	AUMODE_BOTH },		// REC, BOTH
		{ AUMODE_PLAY,		AUMODE_RECORD },	// PLAY, REC
		{ AUMODE_PLAY,		AUMODE_PLAY },		// PLAY, PLAY
		{ AUMODE_PLAY,		AUMODE_BOTH },		// PLAY, BOTH
		{ AUMODE_BOTH,		AUMODE_RECORD },	// BOTH, REC
		{ AUMODE_BOTH,		AUMODE_PLAY },		// BOTH, PLAY
		{ AUMODE_BOTH,		AUMODE_BOTH },		// BOTH, BOTH
	},
	exphalftable[] = {
		// fd0 の期待値		fd1 の期待値(-errnoはエラー)
		{ AUMODE_RECORD,	AUMODE_RECORD },	// REC, REC
		{ AUMODE_RECORD,	-ENODEV },			// REC, PLAY
		{ AUMODE_RECORD,	-ENODEV },			// REC, BOTH
		{ AUMODE_PLAY,		-ENODEV },			// PLAY, REC
		{ AUMODE_PLAY,		AUMODE_PLAY },		// PLAY, PLAY
		{ AUMODE_PLAY,		AUMODE_PLAY },		// PLAY, BOTH
		{ AUMODE_PLAY,		-ENODEV },			// BOTH, REC
		{ AUMODE_PLAY,		AUMODE_PLAY },		// BOTH, PLAY
		{ AUMODE_PLAY,		AUMODE_PLAY },		// BOTH, BOTH
	}, *exptable;

	// HW が Full/Half で期待値が違う
	if (hwfull) {
		exptable = expfulltable;
	} else {
		exptable = exphalftable;
	}

	TEST("open_5");
	// 1本目をオープン
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			DESC("%s,%s", openmodetable[i], openmodetable[j]);

			// 1本目
			fd0 = OPEN(devaudio, i);
			if (fd0 == -1)
				err(1, "open");
			r = IOCTL(fd0, AUDIO_GETBUFINFO, &ai, "");
			if (r == -1)
				err(1, "ioctl");
			actmode = ai.mode & (AUMODE_PLAY | AUMODE_RECORD);
			XP_EQ(exptable[i * 3 + j].mode0, actmode);

			// N7 では多重オープンはできない
			if (netbsd >= 8) {
				// 2本目
				fd1 = OPEN(devaudio, j);
				if (exptable[i * 3 + j].mode1 >= 0) {
					// オープンできることを期待するケース
					XP_SYS_OK(fd1);
					r = IOCTL(fd1, AUDIO_GETBUFINFO, &ai, "");
					XP_SYS_EQ(0, r);
					if (r == 0) {
						actmode = ai.mode & (AUMODE_PLAY | AUMODE_RECORD);
						XP_EQ(exptable[i * 3 + j].mode1, actmode);
					}
				} else {
					// オープンできないことを期待するケース
					XP_SYS_NG(ENODEV, fd1);
					if (fd1 == -1) {
						XP_EQ(-exptable[i * 3 + j].mode1, errno);
					} else {
						r = IOCTL(fd1, AUDIO_GETBUFINFO, &ai, "");
						XP_EQ(0, r);
						if (r == 0) {
							actmode = ai.mode & (AUMODE_PLAY | AUMODE_RECORD);
							XP_FAIL("expects error but %d", actmode);
						}
					}
				}
				if (fd1 >= 0)
					CLOSE(fd1);
			}
			CLOSE(fd0);
		}
	}
}

// 別ユーザとのオープン
void
test_open_6()
{
	char name[32];
	char cmd[64];
	int multiuser;
	int fd0;
	int fd1;
	int r;
	uid_t ouid;

	TEST("open_6");
	if (geteuid() != 0) {
		XP_SKIP("This test must be priviledged user");
		return;
	}

	for (int i = 0; i <= 1; i++) {
		// N7 には multiuser の概念がない
		// AUDIO2 は未実装
		if (netbsd != 8) {
			if (i == 1)
				break;
			multiuser = 0;
		} else {
			multiuser = 1 - i;
			DESC("multiuser%d", multiuser);

			snprintf(name, sizeof(name), "hw.%s.multiuser", hwconfigname());
			snprintf(cmd, sizeof(cmd),
				"sysctl -w %s=%d > /dev/null", name, multiuser);
			r = SYSTEM(cmd);
			if (r == -1)
				err(1, "system: %s", cmd);
			if (r != 0)
				errx(1, "system failed: %s", cmd);

			// 確認
			int newval = 0;
			size_t len = sizeof(newval);
			r = SYSCTLBYNAME(name, &newval, &len, NULL, 0);
			if (r == -1)
				err(1, "multiuser");
			if (newval != multiuser)
				errx(1, "set multiuser=%d failed", multiuser);
		}

		fd0 = OPEN(devaudio, O_WRONLY);
		if (fd0 == -1)
			err(1, "open");

		ouid = GETUID();
		r = SETEUID(1);
		if (r == -1)
			err(1, "setuid");

		fd1 = OPEN(devaudio, O_WRONLY);
		if (multiuser) {
			// 別ユーザもオープン可能
			XP_SYS_OK(fd1);
		} else {
			// 別ユーザはオープンできない
			// N7 は EBUSY (Device Busy)
			// N8 は EPERM (Operation not permitted)
			// N7 はデバイス1つなので Device Busy は適切だと思う。
			if (netbsd == 7)
				XP_SYS_NG(EBUSY, fd1);
			else
				XP_SYS_NG(EPERM, fd1);
		}
		if (fd1 != -1) {
			r = CLOSE(fd1);
			XP_SYS_EQ(0, r);
		}

		r = SETEUID(ouid);
		if (r == -1)
			err(1, "setuid");

		r = CLOSE(fd0);
		XP_SYS_EQ(0, r);
	}
}

// SETINFO の受付エンコーディング
// 正しく変換できるかどうかまではここでは調べない。
void
test_encoding_1(void)
{
	int fd;
	int r;

	// リニア、正常系
	int enctable[] = {
		AUDIO_ENCODING_SLINEAR_LE,
		AUDIO_ENCODING_SLINEAR_BE,
		AUDIO_ENCODING_ULINEAR_LE,
		AUDIO_ENCODING_ULINEAR_BE,
		AUDIO_ENCODING_SLINEAR,		// for backward compatibility
		AUDIO_ENCODING_ULINEAR,		// for backward compatibility
	};
	int prectable[] = {
		8, 16, 32,
	};
	int chtable[] = {
		1, 2, 4, 8, 12,
	};
	int freqtable[] = {
		1000, 192000,
	};

	TEST("encoding_1");
	for (int i = 0; i < __arraycount(enctable); i++) {
		int enc = enctable[i];
		for (int j = 0; j < __arraycount(prectable); j++) {
			int prec = prectable[j];

			// 実際には HW 依存だが確認方法がないので
			// とりあえず手元の hdafg(4) で動く組み合わせだけ
			if (prec >= 24 && netbsd <= 7)
				continue;

			for (int k = 0; k < __arraycount(chtable); k++) {
				int ch = chtable[k];

				// 実際には HW 依存だが確認方法がないので
				if (ch > 1 && netbsd <= 7)
					continue;

				for (int m = 0; m < __arraycount(freqtable); m++) {
					int freq = freqtable[m];

					// AUDIO2/x68k ではメモリ足りなくてこける可能性
					if (netbsd >= 9 && x68k) {
						if (ch == 12)
							continue;
						if (freq == 192000)
							continue;
					}

					char buf[100];
					snprintf(buf, sizeof(buf), "enc=%d,prec=%d,ch=%d,freq=%d",
						enc, prec, ch, freq);

					DESC("%s", buf);
					fd = OPEN(devaudio, O_WRONLY);
					if (fd == -1)
						err(1, "open");

					struct audio_info ai;
					AUDIO_INITINFO(&ai);
					ai.play.encoding = enc;
					ai.play.precision = prec;
					ai.play.channels = ch;
					ai.play.sample_rate = freq;
					ai.mode = AUMODE_PLAY_ALL;
					r = IOCTL(fd, AUDIO_SETINFO, &ai, "play");
					if (netbsd <= 7) {
						// N7 は失敗しても気にしないことにする
						if (r == 0) {
							XP_SYS_EQ(0, r);
						} else {
							XP_SKIP("XXX not checked");
						}
					} else if (netbsd == 8 && r != 0) {
						// N8 はこける理由がわからん
						if (ch > 2) {
							XP_EXPFAIL("ch > 2 not supported?");
						}
					} else {
						// AUDIO2 は全部パスする
						XP_SYS_EQ(0, r);
					}

					CLOSE(fd);
				}
			}
		}
	}
}

void
test_encoding_2()
{
	int fd;
	int r;

	// リニア、異常系

	TEST("encoding_2");
	// 本当はサポートしている以外全部なんだが
	int prectable[] = {
		0, 4, 24,
	};
	for (int i = 0; i < __arraycount(prectable); i++) {
		int prec = prectable[i];
		DESC("prec%d", prec);
		fd = OPEN(devaudio, O_WRONLY);
		if (fd == -1)
			err(1, "open");

		struct audio_info ai;
		AUDIO_INITINFO(&ai);
		ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
		ai.play.precision = prec;
		ai.play.channels = 1;
		ai.play.sample_rate = 1000;
		ai.mode = AUMODE_PLAY_ALL;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
		XP_SYS_NG(EINVAL, r);

		CLOSE(fd);
	}

	int chtable[] = {
		0, 13,
	};
	for (int i = 0; i < __arraycount(chtable); i++) {
		int ch = chtable[i];
		DESC("ch%d", ch);
		fd = OPEN(devaudio, O_WRONLY);
		if (fd == -1)
			err(1, "open");

		struct audio_info ai;
		AUDIO_INITINFO(&ai);
		ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
		ai.play.precision = 16;
		ai.play.channels = ch;
		ai.play.sample_rate = 1000;
		ai.mode = AUMODE_PLAY_ALL;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
		XP_SYS_NG(EINVAL, r);

		CLOSE(fd);
	}

	int freqtable[] = {
		0, 999,	192001,
	};
	for (int i = 0; i < __arraycount(freqtable); i++) {
		int freq = freqtable[i];
		DESC("freq%d", freq);

		// XXX freq=0 は NetBSD<=8 ではプロセスが無限ループに入ってしまう
		if (freq == 0 && netbsd <= 8) {
			XP_EXPFAIL("not tested: it causes infinate loop");
			continue;
		}
		// N7 は周波数のチェックも MI レベルではしない (ただ freq 0 はアカン)
		// N8 はチェックすべきだと思うけどシラン
		if (netbsd <= 8)
			continue;

		fd = OPEN(devaudio, O_WRONLY);
		if (fd == -1)
			err(1, "open");

		struct audio_info ai;
		AUDIO_INITINFO(&ai);
		ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
		ai.play.precision = 16;
		ai.play.channels = 2;
		ai.play.sample_rate = freq;
		ai.mode = AUMODE_PLAY_ALL;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
		XP_SYS_NG(EINVAL, r);

		CLOSE(fd);
	}
}

void
test_drain_1(void)
{
	int r;
	int fd;

	TEST("drain_1");

	fd = open(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	struct audio_info ai;
	AUDIO_INITINFO(&ai);
	// 1フレーム複数バイト、PLAY に設定
	ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
	ai.play.precision = 16;
	ai.play.channels = (netbsd <= 7 && x68k) ? 1 : 2;
	ai.play.sample_rate = 11050;
	ai.mode = AUMODE_PLAY;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	if (r == -1)
		err(1, "AUDIO_SETINFO");
	// 1バイト書いて close
	r = WRITE(fd, &r, 1);
	XP_SYS_EQ(1, r);
	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// pause したまま drain
void
test_drain_2(void)
{
	int r;
	int fd;

	TEST("drain_2");

	fd = open(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	struct audio_info ai;
	AUDIO_INITINFO(&ai);
	ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
	ai.play.precision = 16;
	ai.play.channels = (netbsd <= 7 && x68k) ? 1 : 2;
	ai.play.sample_rate = 11050;
	ai.mode = AUMODE_PLAY;
	ai.play.pause = 1;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	if (r == -1)
		err(1, "AUDIO_SETINFO");
	// 4バイト書いて close
	r = WRITE(fd, &r, 4);
	XP_SYS_EQ(4, r);
	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// PLAY_SYNC でブロックサイズずつ書き込む
// 期待通りの音が出るかは分からないので、play.error が0なことだけ確認
void
test_playsync_1(void)
{
	struct audio_info ai;
	char *wav;
	int wavsize;
	int fd;
	int r;

	TEST("playsync_1");

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	AUDIO_INITINFO(&ai);
	ai.mode = AUMODE_PLAY;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "mode");
	XP_SYS_EQ(0, r);

	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);

	wavsize = ai.blocksize;
	wav = (char *)malloc(wavsize);
	if (wav == NULL)
		err(1, "malloc");
	memset(wav, 0, wavsize);

	for (int i = 0; i < 5; i++) {
		r = WRITE(fd, wav, wavsize);
		XP_SYS_EQ(wavsize, r);
	}

	// ブロックサイズで書き込めばエラーにはならないらしいが、
	// ブロックサイズ未満で書き込んでエラーになるかどうかの条件が分からないので
	// ブロックサイズ未満のテストは保留。
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	if (netbsd == 8) {
		// XXX 詳細不明。1 になるようだ
		XP_EXPFAIL("ai.play.error expects 0 but %d", ai.play.error);
	} else {
		XP_EQ(0, ai.play.error);
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);

	free(wav);
}

// HWFull/Half によらず open mode の方の操作(read/write)はできる。
void
test_readwrite_1(void)
{
	struct audio_info ai;
	char buf[10];
	int fd;
	int r;
	struct {
		int openmode;
		bool canwrite;
		bool canread;
	} exp7table[] = {
		{ O_RDONLY,	0, 1 },
		{ O_WRONLY,	1, 0 },
		{ O_RDWR,	1, 1 },
		{ -O_RDWR,	1, 1 },	// Half でも一応両方アクセスは出来る(ダミー)
		{ 99, },			// 仕方ないので番兵
	}, exp9table[] = {
		{ O_RDONLY,	0, 1 },
		{ O_WRONLY,	1, 0 },
		{ O_RDWR,	1, 1 },
		{ -O_RDWR,	1, 0 },	// Half なら Play と同等になる
		{ 99, },			// 仕方ないので番兵
	}, *exptable;

	TEST("readwrite_1");
	if (netbsd <= 8)
		exptable = exp7table;
	else
		exptable = exp9table;

	AUDIO_INITINFO(&ai);
	ai.play.pause = 1;
	ai.record.pause = 1;

	for (int i = 0; exptable[i].openmode != 99 ; i++) {
		int openmode = exptable[i].openmode;
		bool canwrite = exptable[i].canwrite;
		bool canread = exptable[i].canread;
		if (hwfull) {
			// HWFull なら O_RDWR のほう
			if (openmode < 0)
				continue;
		} else {
			// HWHalf なら O_RDWR は負数のほう
			if (openmode == O_RDWR)
				continue;
			if (openmode == -O_RDWR) {
				openmode = O_RDWR;
			}
		}
		DESC("%s", openmodetable[openmode]);

		fd = OPEN(devaudio, openmode);
		if (fd == -1)
			err(1, "open");

		// 書き込みを伴うので音がでないよう pause しとく
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "pause");
		if (r == -1)
			err(1, "ioctl");

		// write は mode2popen[] が期待値
		memset(buf, 0, sizeof(buf));
		r = WRITE(fd, buf, sizeof(buf));
		if (canwrite) {
			XP_SYS_EQ(10, r);
		} else {
			XP_SYS_NG(EBADF, r);
		}

		// read は mode2ropen[] が期待値
		// N7 は 1バイト以上 read しようとするとブロックする?
		r = READ(fd, buf, 0);
		if (canread) {
			XP_SYS_EQ(0, r);
		} else {
			XP_SYS_NG(EBADF, r);
		}

		CLOSE(fd);
	}
}

// N8 は HWFull/Half によらず1本目も2本目も互いに影響なく各自の
// open mode でオープンでき、(open mode によって許されている) read/write は
// 時系列的に衝突しなければできるようだ。pause してるからだけかもしれない。
//
// AUDIO2 では HWHalf なら full duplex 操作になる open を禁止する。
void
test_readwrite_2(void)
{
	struct audio_info ai;
	char buf[10];
	int fd0, fd1;
	int r;
	struct {
		bool canopen;
		bool canwrite;
		bool canread;
	} expfulltable[] = {
		{ 1, 0, 1 },	// REC, REC
		{ 1, 1, 0 },	// REC, PLAY
		{ 1, 1, 1 },	// REC, BOTH
		{ 1, 0, 1 },	// PLAY, REC
		{ 1, 1, 0 },	// PLAY, PLAY
		{ 1, 1, 1 },	// PLAY, BOTH
		{ 1, 0, 1 },	// BOTH, REC
		{ 1, 1, 0 },	// BOTH, PLAY
		{ 1, 1, 1 },	// BOTH, BOTH
	},
	exphalftable[] = {
		{ 1, 0, 1 },	// REC, REC
		{ 0, 0, 0 },	// REC, PLAY
		{ 0, 0, 0 },	// REC, BOTH
		{ 0, 0, 0 },	// PLAY, REC
		{ 1, 1, 0 },	// PLAY, PLAY
		{ 1, 1, 0 },	// PLAY, BOTH
		{ 0, 0, 0 },	// BOTH, REC
		{ 1, 1, 0 },	// BOTH, PLAY
		{ 0, 0, 0 },	// BOTH, BOTH
	}, *exptable;

	// N7 は多重オープンはできない
	if (netbsd <= 7) {
		TEST("readwrite_2");
		XP_SKIP("N7 does not support multi-open");
		return;
	}
	// HW が Full/Half で期待値が違う
	if (hwfull) {
		exptable = expfulltable;
	} else {
		exptable = exphalftable;
	}

	TEST("readwrite_2");
	AUDIO_INITINFO(&ai);
	ai.play.pause = 1;
	ai.record.pause = 1;

	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			DESC("%s,%s", openmodetable[i], openmodetable[j]);
			bool canopen  = exptable[i * 3 + j].canopen;
			bool canwrite = exptable[i * 3 + j].canwrite;
			bool canread  = exptable[i * 3 + j].canread;

			// XXX オープンできない組み合わせはオープンできない検査すべき?
			if (canopen == false) {
				XP_SKIP("XXX");
				continue;
			}

			fd0 = OPEN(devaudio, i);
			if (fd0 == -1)
				err(1, "open");

			fd1 = OPEN(devaudio, j);
			if (fd1 == -1)
				err(1, "open");

			// 書き込みを伴うので音がでないよう pause しとく
			r = IOCTL(fd1, AUDIO_SETINFO, &ai, "pause");
			if (r == -1)
				err(1, "ioctl");

			// write は mode2popen[] が期待値
			memset(buf, 0, sizeof(buf));
			r = WRITE(fd1, buf, sizeof(buf));
			if (canwrite) {
				XP_SYS_EQ(10, r);
			} else {
				XP_SYS_NG(EBADF, r);
			}

			// read は mode2ropen[] が期待値
			r = READ(fd1, buf, 0);
			if (canread) {
				XP_SYS_EQ(0, r);
			} else {
				XP_SYS_NG(EBADF, r);
			}

			CLOSE(fd0);
			CLOSE(fd1);
		}
	}
}

// 別ディスクリプタを同時に読み書き
// HW Half ではこの操作は行えない
void
test_readwrite_3()
{
	char buf[1024];
	int fd0, fd1;
	int r;
	int status;
	pid_t pid;

	TEST("readwrite_3");
	// N7 では多重オープンは出来ないので、このテストは無効
	if (netbsd <= 7) {
		XP_SKIP("N7 does not support multi-open");
		return;
	}
	if (hwfull == 0) {
		// N8 では read がブロックするバグ
		if (netbsd <= 8) {
			XP_EXPFAIL("not tested; it will block");
			return;
		}
		// AUDIO2 では HalfHW に対して R+W の多重オープンはできない
		XP_SKIP("AUDIO2 does not support R+W open on half HW");
		return;
	}

	memset(buf, 0, sizeof(buf));

	fd0 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");

	fd1 = OPEN(devaudio, O_RDONLY);
	if (fd1 == -1)
		err(1, "open");

	// 事前に吐き出しておかないと fork 後に出力が重複する?
	fflush(stdout);
	fflush(stderr);
	pid = fork();
	if (pid == -1)
		err(1, "fork");
	if (pid == 0) {
		// child (read)
		for (int i = 0; i < 10; i++) {
			r = READ(fd1, buf, sizeof(buf));
			if (r == -1)
				err(1, "read(i=%d)", i);
		}
		exit(0);
	} else {
		// parent (write)
		for (int i = 0; i < 10; i++) {
			r = WRITE(fd0, buf, sizeof(buf));
			if (r == -1)
				err(1, "write(i=%d)", i);
		}
		waitpid(pid, &status, 0);
	}

	CLOSE(fd0);
	CLOSE(fd1);
	// ここまで来れば自動的に成功とする
	XP_EQ(0, 0);
}

// mmap できる mode と prot の組み合わせ
// それと mmap 出来たら read/write は出来ないのテスト
void
test_mmap_1()
{
	struct audio_info ai;
	int fd;
	int r;
	int len;
	void *ptr;
	struct {
		int mode;
		int prot;
		int exp;	// AUDIO2 で mmap が成功するか
	} exptable[] = {
		// 現状 VM システムの制約で mmap は再生バッファに対してのみのようだ。
		// prot はぶっちゃけ見ていないようだ。
		// N7 では open mode に関わらず再生バッファは存在するので mmap は
		// 常に成功する。
		// AUDIO2 では再生バッファがあれば成功なので O_RDONLY 以外なら成功
		{ O_RDONLY,	PROT_NONE,				0 },
		{ O_RDONLY,	PROT_READ,				0 },
		{ O_RDONLY,	PROT_WRITE,				0 },
		{ O_RDONLY,	PROT_READ | PROT_WRITE,	0 },

		{ O_WRONLY,	PROT_NONE,				1 },
		{ O_WRONLY,	PROT_READ,				1 },
		{ O_WRONLY,	PROT_WRITE,				1 },
		{ O_WRONLY,	PROT_READ | PROT_WRITE,	1 },

		// HWFull の場合
		{ O_RDWR,	PROT_NONE,				1 },
		{ O_RDWR,	PROT_READ,				1 },
		{ O_RDWR,	PROT_WRITE,				1 },
		{ O_RDWR,	PROT_READ | PROT_WRITE,	1 },

		// HWHalf の場合 (-O_RDWR を取り出した時に加工する)
		{ -O_RDWR,	PROT_NONE,				1 },
		{ -O_RDWR,	PROT_READ,				1 },
		{ -O_RDWR,	PROT_WRITE,				1 },
		{ -O_RDWR,	PROT_READ | PROT_WRITE,	1 },
	};

	TEST("mmap_1");
	if ((props & AUDIO_PROP_MMAP) == 0) {
		return;
	}

	for (int i = 0; i < __arraycount(exptable); i++) {
		int mode = exptable[i].mode;
		int prot = exptable[i].prot;
		int expected = exptable[i].exp;

		if (hwfull) {
			// HWFull なら O_RDWR のほう
			if (mode < 0)
				continue;
		} else {
			// HWHalf なら O_RDWR は負数のほう
			if (mode == O_RDWR)
				continue;
			if (mode == -O_RDWR) {
				mode = O_RDWR;
			}
		}

		// N7、N8 なら mmap 自体は常に成功する
		if (netbsd <= 8)
			expected = 1;

		char protbuf[32];
		if (prot == 0) {
			strlcpy(protbuf, "PROT_NONE", sizeof(protbuf));
		} else {
			snprintb_m(protbuf, sizeof(protbuf),
				"\177\020" "b\1PROT_WRITE\0b\0PROT_READ\0", prot, 0);
		}
		DESC("%s,%s", openmodetable[mode], protbuf);

		fd = OPEN(devaudio, mode);
		if (fd == -1)
			err(1, "open");

		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "get");
		if (r == -1)
			err(1, "AUDIO_GETINFO");

		// 再生側しかサポートしていないのでこれでいい
		len = ai.play.buffer_size;

		// pause にしとく
		AUDIO_INITINFO(&ai);
		ai.play.pause = 1;
		ai.record.pause = 1;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "pause");
		if (r == -1)
			err(1, "AUDIO_SETINFO");

		ptr = MMAP(NULL, len, prot, MAP_FILE, fd, 0);
		if (expected == 0) {
			XP_SYS_NG(EACCES, ptr);
		} else {
			XP_SYS_OK(ptr);

			// read トラックは mmap フラグ立ってないので read は出来てしまう
			if (mode == O_RDONLY || (mode == O_RDWR && hwfull)) {
				r = READ(fd, &ai, 0);
				XP_SYS_EQ(0, r);
			}
			// write は出来なくなること
			if (mode == O_WRONLY || mode == O_RDWR) {
				r = WRITE(fd, &ai, 4);
				if (netbsd <= 8)
					XP_SYS_NG(EINVAL, r);
				else
					XP_SYS_NG(EPERM, r);
			}

			r = MUNMAP(ptr, len);
			XP_SYS_EQ(0, r);
		}

		// 再生側の pause が効いてること
		// 録音側はどっちらけなので調べない
		if (mode != O_RDONLY) {
			r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
			XP_SYS_EQ(0, r);
			XP_EQ(1, ai.play.pause);
		}

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);

		// N7 では mmap したディスクリプタをクローズした直後は
		// オープンが失敗する気がする…。
		// なのでここで一回オープンしてリセット(?)しておく。
		if (netbsd <= 7) {
			fd = OPEN(devaudio, mode);
			if (fd != -1)
				CLOSE(fd);
		}
	}
}

// mmap の len と offset パラメータ
void
test_mmap_2()
{
	struct audio_info ai;
	int fd;
	int r;
	size_t len;
	off_t offset;
	void *ptr;
	int bufsize;
	int pagesize;

	TEST("mmap_2");
	if ((props & AUDIO_PROP_MMAP) == 0) {
		return;
	}

	// とりあえず再生側のことしか考えなくていいか。

	len = sizeof(pagesize);
	r = SYSCTLBYNAME("hw.pagesize", &pagesize, &len, NULL, 0);
	if (r == -1)
		err(1, "sysctl");

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	// buffer_size 取得
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	if (r == -1)
		err(1, "AUDIO_GETINFO");
	bufsize = ai.play.buffer_size;

	// バッファサイズのほうが大きい場合とページサイズのほうが大きい場合が
	// あって、どっちらけ。
	int lsize = roundup2(bufsize, pagesize);

	struct {
		size_t len;
		off_t offset;
		int exp;
	} table[] = {
		// len		offset	expected
		{ 0,		0,		0 },		// len=0 だけど構わないらしい
		{ 1,		0,		0 },		// len が短くても構わないらしい
		{ lsize,	0,		0 },		// 大きい方ぴったり
		{ lsize + 1,0,		EOVERFLOW },// それを超えてはいけない

		{ 0,		-1,		EINVAL },	// 負数
		{ 0,	lsize,		0 },		// これは意味ないはずだが計算上 OK...
		{ 0,	lsize + 1,	EOVERFLOW },// 足して超えるので NG
		{ 1,	lsize,		EOVERFLOW },// 足して超えるので NG
	};

	for (int i = 0; i < __arraycount(table); i++) {
		len = table[i].len;
		offset = table[i].offset;
		int exp = table[i].exp;
		DESC("len=%d,offset=%d", (int)len, (int)offset);

		ptr = MMAP(NULL, len, PROT_WRITE, MAP_FILE, fd, offset);
		if (exp == 0) {
			XP_SYS_OK(ptr);
		} else {
			// N7 時点ではまだ EOVERFLOW のチェックがなかった
			if (netbsd <= 7 && exp == EOVERFLOW)
				exp = EINVAL;
			XP_SYS_NG(exp, ptr);
		}

		if (ptr != MAP_FAILED) {
			r = MUNMAP(ptr, len);
			XP_SYS_EQ(0, r);
		}
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);

	// N7 では mmap したディスクリプタをクローズした直後は
	// オープンが失敗する。
	// なのでここで一回オープンしてリセット(?)しておく。
	if (netbsd <= 7) {
		fd = OPEN(devaudio, O_WRONLY);
		if (fd != -1)
			CLOSE(fd);
	}
}

// mmap するとすぐに動き始める
void
test_mmap_3()
{
	struct audio_info ai;
	int fd;
	int r;
	int len;
	void *ptr;

	// とりあえず再生側のことしか考えなくていいか。
	TEST("mmap_3");
	if ((props & AUDIO_PROP_MMAP) == 0) {
		return;
	}

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "get");
	if (r == -1)
		err(1, "AUDIO_GETINFO");
	len = ai.play.buffer_size;

	ptr = MMAP(NULL, len, PROT_WRITE, MAP_FILE, fd, 0);
	XP_SYS_OK(ptr);

	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "get");
	XP_SYS_EQ(0, r);
	XP_EQ(1, ai.play.active);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);

	// N7 では mmap したディスクリプタをクローズした直後は
	// オープンが失敗する。
	// なのでここで一回オープンしてリセット(?)しておく。
	if (netbsd <= 7) {
		fd = OPEN(devaudio, O_WRONLY);
		if (fd != -1)
			CLOSE(fd);
	}
}

// 同一ディスクリプタで二重 mmap
void
test_mmap_4()
{
	struct audio_info ai;
	int fd;
	int r;
	int len;
	void *ptr;
	void *ptr2;

	TEST("mmap_4");
	if ((props & AUDIO_PROP_MMAP) == 0) {
		return;
	}

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "get");
	if (r == -1)
		err(1, "AUDIO_GETINFO");
	len = ai.play.buffer_size;

	ptr = MMAP(NULL, len, PROT_WRITE, MAP_FILE, fd, 0);
	XP_SYS_OK(ptr);

	// N7 では成功するようだが意図してるのかどうか分からん。
	// N8 も成功するようだが意図してるのかどうか分からん。
	ptr2 = MMAP(NULL, len, PROT_WRITE, MAP_FILE, fd, 0);
	XP_SYS_OK(ptr2);
	if (ptr2 != MAP_FAILED) {
		r = MUNMAP(ptr2, len);
		XP_SYS_EQ(0, r);
	}

	r = MUNMAP(ptr, len);
	XP_SYS_EQ(0, r);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);

	// N7 では mmap したディスクリプタをクローズした直後は
	// オープンが失敗する。
	// なのでここで一回オープンしてリセット(?)しておく。
	if (netbsd <= 7) {
		fd = OPEN(devaudio, O_WRONLY);
		if (fd != -1)
			CLOSE(fd);
	}
}

// 別ディスクリプタで mmap
void
test_mmap_5()
{
	struct audio_info ai;
	int fd0;
	int fd1;
	int r;
	int len;
	void *ptr0;
	void *ptr1;

	TEST("mmap_5");
	// 多重オープンはできない
	if (netbsd <= 7)
		return;
	if ((props & AUDIO_PROP_MMAP) == 0) {
		return;
	}

	fd0 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");

	r = IOCTL(fd0, AUDIO_GETBUFINFO, &ai, "get");
	if (r == -1)
		err(1, "AUDIO_GETINFO");
	len = ai.play.buffer_size;

	fd1 = OPEN(devaudio, O_WRONLY);
	if (fd1 == -1)
		err(1, "open");

	ptr0 = MMAP(NULL, len, PROT_WRITE, MAP_FILE, fd0, 0);
	XP_SYS_OK(ptr0);

	ptr1 = MMAP(NULL, len,  PROT_WRITE, MAP_FILE, fd1, 0);
	XP_SYS_OK(ptr1);

	r = MUNMAP(ptr1, len);
	XP_SYS_EQ(0, r);

	r = CLOSE(fd1);
	XP_SYS_EQ(0, r);

	r = MUNMAP(ptr0, len);
	XP_SYS_EQ(0, r);

	r = CLOSE(fd0);
	XP_SYS_EQ(0, r);
}

// mmap 前に GET[IO]OFFS すると、既に次のポジションがセットされているようだ。
// N7 だと RDONLY でも WRONLY でも同じ値が読めるようだ。
// GETIOFFS は初期オフセット 0 だがどうせこっちは動いてないはず。
void
test_mmap_6()
{
	struct audio_info ai;
	struct audio_offset ao;
	int fd;
	int r;

	TEST("mmap_6");
	for (int mode = 0; mode <= 2; mode++) {
		DESC("%s", openmodetable[mode]);

		fd = OPEN(devaudio, mode);
		if (fd == -1)
			err(1, "open");

		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		if (r == -1)
			err(1, "AUDIO_GETINFO");

		r = IOCTL(fd, AUDIO_GETOOFFS, &ao, "");
		XP_SYS_EQ(0, r);
		XP_EQ(0, ao.samples);			// 転送バイト数 0
		XP_EQ(0, ao.deltablks);			// 前回チェック時の転送ブロック数 0
		if (netbsd == 9 && mode == O_RDONLY) {
			// N7, N8 は常にトラックとかがあるのでいつでも ao.offset が
			// 次のブロックを指せるが、AUDIO2 では O_RDONLY だと再生トラックは
			// 存在しないので、再生ブロックサイズもない。
			// これはやむを得ないか。
			XP_EQ(0, ao.offset);
		} else {
			XP_EQ(ai.blocksize, ao.offset);	// これから転送する位置は次ブロック
		}

		r = IOCTL(fd, AUDIO_GETIOFFS, &ao, "");
		XP_SYS_EQ(0, r);
		XP_EQ(0, ao.samples);			// 転送バイト数 0
		XP_EQ(0, ao.deltablks);			// 前回チェック時の転送ブロック数 0
		XP_EQ(0, ao.offset);			// 0

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
	}
}

// mmap_7, 8 の共通部
void
test_mmap_7_8_common(int type)
{
	struct audio_info ai;
	struct audio_offset ao;
	char *buf;
	int fd;
	int r;
	int blocksize;

	// N7、N8 はなぜかこの PR に書いてあるとおりにならない
	if (netbsd <= 8) {
		XP_SKIP("This test does not work on N7/N8");
		return;
	}
	// A2 の type0 は今のところ仕様
	if (netbsd == 9 && type == 0) {
		XP_SKIP("On AUDIO2 it can not set blocksize");
		return;
	}
	// A2 の type1 も今のところエラーが出るので後で考える
	if (netbsd == 9 && type == 1) {
		XP_SKIP("under construction");
		return;
	}

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	AUDIO_INITINFO(&ai);
	ai.play.pause = 1;
	if (type == 0)
		ai.blocksize = 1024;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	if (r == -1)
		err(1, "AUDIO_SETINFO");

	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	if (r == -1)
		err(1, "AUDIO_GETINFO");
	if (type == 0) {
		// 1024バイトが設定できること
		XP_EQ(1024, ai.blocksize);
		blocksize = 1024;
	} else {
		// カーネルのブロックサイズを取得
		blocksize = ai.blocksize;
	}

	buf = (char *)malloc(blocksize);
	if (buf == NULL)
		err(1, "malloc");
	memset(buf, 0, blocksize);

	// オープン直後の GETOOFFS
	r = IOCTL(fd, AUDIO_GETOOFFS, &ao, "");
	XP_SYS_EQ(0, r);
	XP_EQ(0, ao.samples);			// まだ何も書いてない
	XP_EQ(0, ao.deltablks);
	XP_EQ(blocksize, ao.offset);	// 次ブロックを指している

	// 1ブロック書き込み
	r = WRITE(fd, buf, blocksize);
	XP_SYS_EQ(blocksize, r);

	// 1ブロック書き込み後の GETOOFFS
	r = IOCTL(fd, AUDIO_GETOOFFS, &ao, "");
	XP_SYS_EQ(0, r);
	XP_EQ(blocksize, ao.samples);	// 1ブロック
	XP_EQ(1, ao.deltablks);				// 前回チェック時の転送ブロック数
	XP_EQ(blocksize * 2, ao.offset);	// 次ブロック

	// pause 解除
	AUDIO_INITINFO(&ai);
	ai.play.pause = 0;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// pause 解除後の GETOOFFS
	r = IOCTL(fd, AUDIO_GETOOFFS, &ao, "");
	XP_SYS_EQ(0, r);
	XP_EQ(blocksize, ao.samples);
	XP_EQ(0, ao.deltablks);
	XP_EQ(blocksize * 2, ao.offset);

	// もう1ブロック書き込み
	r = WRITE(fd, buf, blocksize);
	XP_SYS_EQ(blocksize, r);
	r = IOCTL(fd, AUDIO_DRAIN, NULL, "");
	XP_SYS_EQ(0, r);

	// 書き込み完了後の GETOOFFS
	r = IOCTL(fd, AUDIO_GETOOFFS, &ao, "");
	XP_SYS_EQ(0, r);
	XP_EQ(blocksize * 2, ao.samples);
	XP_EQ(1, ao.deltablks);
	XP_EQ(blocksize * 3, ao.offset);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);

	free(buf);
}

// 1ブロック write した後に GETOOFFS する。
// 1ブロックは 1024 バイト。
// PR kern/50613
void
test_mmap_7()
{
	TEST("mmap_7");
	test_mmap_7_8_common(0);
}

// 1ブロック write した後に GETOOFFS する。
// 1ブロックはネイティブブロックサイズ。
void
test_mmap_8()
{
	TEST("mmap_8");
	test_mmap_7_8_common(1);
}

// mmap 開始
void
test_mmap_9()
{
	struct audio_info ai;
	struct audio_offset ao;
	char *ptr;
	int fd;
	int r;

	TEST("mmap_9");
	if (x68k && netbsd <= 7) {
		// HW エンコードにセットするあたりのテストが面倒
		XP_SKIP("not supported yet");
		return;
	}
	// 後で見る
	if (netbsd == 9) {
		XP_SKIP("under construction");
		return;
	}

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	AUDIO_INITINFO(&ai);
	ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
	ai.play.precision = 16;
	ai.play.channels = 2;
	ai.play.sample_rate = 48000;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	if (r == -1)
		err(1, "AUDIO_SETINFO");

	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	if (r == -1)
		err(1, "AUDIO_GETINFO");

	ptr = (char *)MMAP(NULL, ai.play.buffer_size, 0, 0, fd, 0);
	XP_SYS_OK(ptr);

	// 雑なテスト
	// mmap 直後は理想的には samples = 0、offset = blksize になる。

	r = IOCTL(fd, AUDIO_GETOOFFS, &ao, "");
	XP_SYS_EQ(0, r);
	if (netbsd == 8) {
		// N8 ではこの時点でなんか進んでるようだ?
		XP_EXPFAIL("ao.samples expects 0 but %d", ao.samples);
	} else {
		// N7, AUDIO2 ではまだ進んでいないはず
		XP_EQ(0, ao.samples);
	}
	// XXX というか先に一回 GETOOFFS しとかないと delta はわからないのでは?
	if (netbsd == 8) {
		// N8 ではこの時点でなんか進んでるようだ?
		XP_EXPFAIL("ao.deltablks expects 0 but %d", ao.deltablks);
	} else {
		XP_EQ(0, ao.deltablks);
	}
	if (netbsd == 8) {
		// N8 では詳細不明
		XP_EXPFAIL("ao.offset expects %d but %d", ai.blocksize, ao.offset);
	} else {
		// N7, AUDIO2 では次の書き込み位置はブロックサイズ分先の位置のはず
		XP_EQ(ai.blocksize, ao.offset);
	}

	usleep(50 * 1000);
	// 50msec 後には理想的には samples は 1ブロック分、offset ももう
	// 1ブロック分進んでいるはず。

	r = IOCTL(fd, AUDIO_GETOOFFS, &ao, "");
	XP_SYS_EQ(0, r);
	if (netbsd == 8) {
		// N8 では詳細不明
		XP_EXPFAIL("ao.samples expects %d but %d", ai.blocksize, ao.samples);
	} else {
		// 1ブロック書き込み済み
		XP_EQ(ai.blocksize, ao.samples);
	}
	if (netbsd == 8) {
		// N8 では詳細不明
		XP_EXPFAIL("ao.deltablks expects 1 but %d", ao.deltablks);
	} else {
		// 前回チェック時の転送ブロック数
		XP_EQ(1, ao.deltablks);
	}
	if (netbsd == 8) {
		// N8 では詳細不明
		XP_EXPFAIL("ao.offset expects %d but %d", ai.blocksize * 2, ao.offset);
	} else {
		// その次のブロックを指しているはず
		XP_EQ(ai.blocksize * 2, ao.offset);
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);

	// N7 では mmap したディスクリプタをクローズした直後は
	// オープンが失敗する。
	// なのでここで一回オープンしてリセット(?)しておく。
	if (netbsd <= 7) {
		fd = OPEN(devaudio, O_WRONLY);
		if (fd != -1)
			CLOSE(fd);
	}
}

// openmode と poll の関係
void
test_poll_1()
{
	struct pollfd pfd;
	int fd;
	int r;
	struct {
		int openmode;
		int events;
		int exp_revents;
#define IN POLLIN
#define OUT POLLOUT
	} fulltable[] = {
		// openmode	events	exp_revents
		{ O_RDONLY,	IN,		0 },		// 正常だがデータがないはず=※1
		{ O_RDONLY,	OUT,	0 },		// OUTは成立しない=※2
		{ O_RDONLY, IN|OUT,	0 },		// ※1 && ※2

		{ O_WRONLY,	IN,		0 },		// INは成立しない
		{ O_WRONLY,	OUT,	OUT },		// 正常
		{ O_WRONLY,	IN|OUT,	OUT },		// INは成立しない

		{ O_RDWR,	IN,		0 },		// 正常がデータがないはず
		{ O_RDWR,	OUT,	OUT },		// 正常
		{ O_RDWR,	IN|OUT,	OUT },		// ※1
		{ 99 },
	}, halftable[] = {
		{ O_RDONLY,	IN,		0 },		// 正常だがデータがないはず=※1
		{ O_RDONLY,	OUT,	0 },		// OUTは成立しない=※2
		{ O_RDONLY, IN|OUT,	0 },		// ※1 && ※2

		{ O_WRONLY,	IN,		0 },		// INは成立しない
		{ O_WRONLY,	OUT,	OUT },		// 正常
		{ O_WRONLY,	IN|OUT,	OUT },		// INは成立しない

		{ O_RDWR,	IN,		0 },		// 正常がデータがないはず
		{ O_RDWR,	OUT,	OUT },		// 正常
		{ O_RDWR,	IN|OUT,	OUT },		// ※1
		{ 99 },
#undef IN
#undef OUT
	}, *table;

	if (hwfull)
		table = fulltable;
	else
		table = halftable;

	TEST("poll_1");
	for (int i = 0; table[i].openmode != 99; i++) {
		int openmode = table[i].openmode;
		int events = table[i].events;
		int exp_revents = table[i].exp_revents;
		int exp = (exp_revents != 0) ? 1 : 0;

		char evbuf[64];
		snprintb(evbuf, sizeof(evbuf), 
			"\177\020" \
		    "b\10WRBAND\0" \
		    "b\7RDBAND\0" "b\6RDNORM\0" "b\5NVAL\0" "b\4HUP\0" \
		    "b\3ERR\0" "b\2OUT\0" "b\1PRI\0" "b\0IN\0",
			events);
		DESC("%s,%s", openmodetable[openmode], evbuf);

		fd = OPEN(devaudio, openmode);
		if (fd == -1)
			err(1, "open");

		memset(&pfd, 0, sizeof(pfd));
		pfd.fd = fd;
		pfd.events = events;

		r = POLL(&pfd, 1, 100);
		// 複雑なのでちょっと特別扱い
		if (r < 0 || r > 1) {
			// システムコールがエラーになるケースと、pfd 1つのpoll が
			// 2以上を返してくるのは、知らない異常なので先に判定しておく。
			XP_SYS_EQ(exp, r);
		} else {
			// ここは poll が 0 か 1 を返したはず。
			// poll の戻り値と revents は連動しているはずなので、
			// revents を先に調べることにする。
			int checked = 0;
			DPRINTF("  > pfd.revents=%x\n", pfd.revents);

			// RDONLY/RDWR で録音を開始してないのに POLLIN が立つのは
			// バグだと思う。
			if (netbsd <= 8 && openmode != O_WRONLY && (events & POLLIN) != 0
			 && (pfd.revents & POLLIN) != 0) {
				XP_EXPFAIL(
					"recording has not started but POLLIN set (bug?)");
				checked = 1;
			}
			if (netbsd <= 8 && openmode == O_RDONLY && (events & POLLOUT) != 0
			 && (pfd.revents & POLLOUT) != 0) {
				XP_EXPFAIL("RDONLY but POLLOUT set (bug?)");
				checked = 1;
			}
			if (checked == 0) {
				XP_EQ(exp_revents, pfd.revents);
				XP_EQ(exp, r);
			}
		}

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
	}
}

// poll、空で POLLOUT のテスト
void
test_poll_2()
{
	struct pollfd pfd;
	int fd;
	int r;

	TEST("poll_2");

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	memset(&pfd, 0, sizeof(pfd));
	pfd.fd = fd;
	pfd.events = POLLOUT;

	// 空の状態でチェック。タイムアウト0でも成功するはず
	r = POLL(&pfd, 1, 0);
	XP_SYS_EQ(1, r);
	XP_EQ(POLLOUT, pfd.revents);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// poll、バッファフルで POLLOUT のテスト
void
test_poll_3()
{
	struct audio_info ai;
	struct pollfd pfd;
	int fd;
	int r;
	char *buf;
	int buflen;

	TEST("poll_3");

	fd = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
	if (fd == -1)
		err(1, "open");

	// pause セット
	AUDIO_INITINFO(&ai);
	ai.play.pause = 1;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// バッファサイズ取得
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// 書き込み
	buflen = ai.play.buffer_size;
	buf = (char *)malloc(buflen);
	if (buf == NULL)
		err(1, "malloc");
	memset(buf, 0, buflen);
	do {
		r = WRITE(fd, buf, buflen);
	} while (r == buflen);
	if (r == -1) {
		XP_SYS_NG(EAGAIN, r);
	}

	// poll
	memset(&pfd, 0, sizeof(pfd));
	pfd.fd = fd;
	pfd.events = POLLOUT;
	r = POLL(&pfd, 1, 0);
	XP_SYS_EQ(0, r);
	XP_EQ(0, pfd.revents);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
	free(buf);
}

// poll、hiwat 設定してバッファフルで POLLOUT のテスト
void
test_poll_4()
{
	struct audio_info ai;
	struct pollfd pfd;
	int fd;
	int r;
	char *buf;
	int buflen;
	int newhiwat;

	TEST("poll_4");

	fd = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
	if (fd == -1)
		err(1, "open");

	// バッファサイズ、hiwat 取得
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	// なんでもいいので変更する
	newhiwat = ai.lowat;

	// pause、hiwat 設定
	AUDIO_INITINFO(&ai);
	ai.play.pause = 1;
	ai.hiwat = newhiwat;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// hiwat 再取得
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// 書き込み
	buflen = ai.blocksize * ai.hiwat;
	buf = (char *)malloc(buflen);
	if (buf == NULL)
		err(1, "malloc");
	memset(buf, 0, buflen);
	do {
		r = WRITE(fd, buf, buflen);
	} while (r == buflen);
	if (r == -1) {
		XP_SYS_NG(EAGAIN, r);
	}

	// poll
	memset(&pfd, 0, sizeof(pfd));
	pfd.fd = fd;
	pfd.events = POLLOUT;
	r = POLL(&pfd, 1, 0);
	XP_SYS_EQ(0, r);
	XP_EQ(0, pfd.revents);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
	free(buf);
}

// バッファフルから pause を解除した場合。
void
test_poll_5()
{
	struct audio_info ai;
	struct pollfd pfd;
	int fd;
	int r;
	char *buf;
	int buflen;

	TEST("poll_5");

	for (int emul = 0; emul < 2; emul++) {
		DESC("emul=%d", emul);

		// ノンブロッキングオープン
		fd = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
		if (fd == -1)
			err(1, "open");

		// pause を設定
		AUDIO_INITINFO(&ai);
		ai.play.pause = 1;
		// ついでにエンコーディングも設定
		// XXX 本当は GETENC でチェックすべきだが手抜き
		if (emul) {
			ai.play.encoding = AUDIO_ENCODING_SLINEAR_NE;
			ai.play.precision = 16;
			ai.play.channels = 2;
			ai.play.sample_rate = 22050;
		}
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "pause=1");
		XP_SYS_EQ(0, r);

		// いろいろ再取得
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);

		// 書き込み
		buflen = ai.blocksize * ai.hiwat;
		buf = (char *)malloc(buflen);
		if (buf == NULL)
			err(1, "malloc");
		memset(buf, 0, buflen);
		do {
			r = WRITE(fd, buf, buflen);
		} while (r == buflen);
		if (r == -1) {
			XP_SYS_NG(EAGAIN, r);
		}

		// バッファフルなので POLLOUT が立たないこと
		memset(&pfd, 0, sizeof(pfd));
		pfd.fd = fd;
		pfd.events = POLLOUT;
		r = POLL(&pfd, 1, 0);
		XP_SYS_EQ(0, r);
		XP_EQ(0, pfd.revents);

		// pause 解除
		AUDIO_INITINFO(&ai);
		ai.play.pause = 0;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "pause=0");
		XP_SYS_EQ(0, r);

		// pause を解除すると、
		// N7(emul=0) バッファはそのまま
		// N7(emul=1) バッファはクリアされる
		// N8         バッファはそのまま
		// AUDIO2     バッファはそのまま
		// ただしいずれのケースも kevent() に変化はないようだ。
		// XXX たぶんクリアした時に selnotify() を呼べば変化しそうな気も
		// XXX するけど未確認。

		// pause 解除後再生ループが回るかもしれない猶予を与えてみる
		usleep(100 * 1000);

		// poll() の結果は状況による(?)
		pfd.revents = 0;
		r = POLL(&pfd, 1, 0);
		if (netbsd == 7 && emul == 1) {
			// バッファはクリアされているので、書き込める
			XP_SYS_EQ(1, r);
			XP_EQ(POLLOUT, pfd.revents);
		} else {
			// バッファはそのままなので、(全部は)書き込めない
			XP_SYS_OK(r);
		}

		// 書き込めるかどうかは状況による(?)
		r = WRITE(fd, buf, buflen);
		if (netbsd == 7 && emul == 1) {
			// バッファはクリアされているので、書き込める
			XP_SYS_EQ(buflen, r);
		} else {
			// バッファはそのままなので、(全部は)書き込めない
			XP_SYS_NE(buflen, r);
		}

		// ただし再生する必要はないのでフラッシュする
		r = IOCTL(fd, AUDIO_FLUSH, NULL, "");
		XP_SYS_EQ(0, r);

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
		free(buf);
	}
}

// 再生が隣のディスクリプタの影響を受けないこと
void
test_poll_6()
{
	struct audio_info ai;
	struct pollfd pfd[2];
	int fd[2];
	int r;
	char *buf;
	int blocksize;
	int buflen;
	int time;

	TEST("poll_6");

	// 多重化は N7 では出来ない
	if (netbsd < 8) {
		XP_SKIP("NetBSD7 does not support multi-open");
		return;
	}

	for (int i = 0; i < 2; i++) {
		int a = i;
		int b = 1 - i;
		DESC("%d", i);

		fd[0] = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
		if (fd[0] == -1)
			err(1, "open");
		fd[1] = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
		if (fd[1] == -1)
			err(1, "open");

		AUDIO_INITINFO(&ai);
		// 再生時間を縮めるため blocksize を小さくしたいが N8 では効かないっぽい
		ai.blocksize = 320;
		ai.hiwat = 6;
		ai.lowat = 3;
		// fdA は pause を設定
		ai.play.pause = 1;
		r = IOCTL(fd[a], AUDIO_SETINFO, &ai, "pause=1");
		XP_SYS_EQ(0, r);
		// fdB は pause なし
		ai.play.pause = 0;
		r = IOCTL(fd[b], AUDIO_SETINFO, &ai, "pause=0");
		XP_SYS_EQ(0, r);

		// いろいろ再取得
		// 手抜きして使い回す
		r = IOCTL(fd[a], AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);
		XP_EQ(6, ai.hiwat);
		XP_EQ(3, ai.lowat);
		blocksize = ai.blocksize;

		// 8kHz での再生時間 msec に対して十分長くとる
		time = ai.hiwat * blocksize / 8 * 2;

		// fdA に書き込み
		buflen = blocksize * ai.hiwat;
		buf = (char *)malloc(buflen);
		if (buf == NULL)
			err(1, "malloc");
		memset(buf, 0, buflen);
		do {
			r = WRITE(fd[a], buf, buflen);
		} while (r == buflen);
		if (r == -1) {
			XP_SYS_NG(EAGAIN, r);
		}

		// fdA はバッファフルなので POLLOUT が立たないこと
		memset(pfd, 0, sizeof(pfd));
		pfd[0].fd = fd[a];
		pfd[0].events = POLLOUT;
		r = POLL(pfd, 1, 0);
		XP_SYS_EQ(0, r);
		XP_EQ(0, pfd[0].revents);

		// fdB に(lowat以上)書き込む
		r = WRITE(fd[b], buf, buflen);
		XP_SYS_EQ(buflen, r);
		do {
			r = WRITE(fd[b], buf, buflen);
		} while (r == buflen);

		memset(pfd, 0, sizeof(pfd));
		pfd[0].fd = fd[0];
		pfd[0].events = POLLOUT;
		pfd[1].fd = fd[1];
		pfd[1].events = POLLOUT;
		// 待ってれば fdB だけ POLLOUT になるはず
		r = POLL(pfd, 2, time);
		XP_SYS_EQ(1, r);
		if (r != -1) {
			XP_EQ(0, pfd[a].revents);
			XP_EQ(POLLOUT, pfd[b].revents);
		}

		r = CLOSE(fd[0]);
		XP_SYS_EQ(0, r);
		r = CLOSE(fd[1]);
		XP_SYS_EQ(0, r);
		free(buf);
	}
}

// 再生ディスクリプタで POLLIN してても(これ自体が普通ではないが)
// 隣の録音ディスクリプタの影響を受けないこと
void
test_poll_writeIN_1()
{
	struct audio_info ai;
	struct pollfd pfd;
	int fd[2];
	int r;
	char *buf;
	int blocksize;

	TEST("poll_writeIN_1");

	// 多重化は N7 では出来ない
	if (netbsd < 8) {
		XP_SKIP("NetBSD7 does not support multi-open");
		return;
	}
	// Full Duplex でないと R/W 一度にオープンできない
	if (hwfull == 0) {
		XP_SKIP("Half duplex HW cannot open R/W");
		return;
	}

	int play = 0;
	int rec = 1;

	fd[play] = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
	if (fd[play] == -1)
		err(1, "open");
	fd[rec] = OPEN(devaudio, O_RDONLY);
	if (fd[rec] == -1)
		err(1, "open");

	// ブロックサイズ取得
	r = IOCTL(fd[rec], AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	blocksize = ai.blocksize;

	buf = (char *)malloc(blocksize);
	if (buf == NULL)
		err(1, "malloc");

	// まずは再生側で POLLIN が返ってこないことをチェック。
	// この時点で返ってくるようだと、テストの意味がない。
	memset(&pfd, 0, sizeof(pfd));
	pfd.fd = fd[play];
	pfd.events = POLLIN;
	r = POLL(&pfd, 1, 0);
	if (r == 0 && pfd.revents == 0) {
		XP_SYS_EQ(0, r);
		XP_EQ(0, pfd.revents);
	} else {
		XP_SKIP("playfd returns POLLIN");
		goto abort;
	}

	// 録音開始
	r = READ(fd[rec], buf, blocksize);
	XP_SYS_EQ(blocksize, r);

	// 再生側を POLLIN しても何も起きないべき
	r = POLL(&pfd, 1, 1000);
	XP_SYS_EQ(0, r);
	XP_EQ(0, pfd.revents);

	// 録音側を POLLIN すれば当然見える
	pfd.fd = fd[rec];
	r = POLL(&pfd, 1, 0);
	XP_SYS_EQ(1, r);
	XP_EQ(POLLIN, pfd.revents);

 abort:
	r = CLOSE(fd[play]);
	XP_SYS_EQ(0, r);
	r = CLOSE(fd[rec]);
	XP_SYS_EQ(0, r);
	free(buf);
}

// kqueue_1 は poll_1 と合わせるため空けておく?

void
test_kqueue_2()
{
	struct audio_info ai;
	struct kevent kev;
	struct timespec ts;
	int kq;
	int fd;
	int r;

	TEST("kqueue_2");

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);

	kq = KQUEUE();
	XP_SYS_OK(kq);

	memset(&ts, 0, sizeof(ts));

	EV_SET(&kev, fd, EV_ADD, EVFILT_WRITE, 0, 0, 0);
	r = KEVENT_SET(kq, &kev, 1);
	XP_SYS_EQ(0, r);

	// 空の状態でチェック。タイムアウト0でも成功するはず
	r = KEVENT_POLL(kq, &kev, 1, &ts);
	XP_SYS_EQ(1, r);
	XP_EQ(fd, kev.ident);
	// kev.data は kqueue(2) によると「書き込みバッファの空き容量」を返す
	// らしく、N7 は buffer_size を返しているが、
	// blocksize * hiwat じゃなくていいんだろうか。
	XP_EQ(ai.play.buffer_size, kev.data);
	//XP_EQ(ai.blocksize * ai.hiwat, kev.data);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
	r = CLOSE(kq);
	XP_SYS_EQ(0, r);
}

// kqueue、バッファフルでテスト
void
test_kqueue_3()
{
	struct audio_info ai;
	struct kevent kev;
	struct timespec ts;
	int kq;
	int fd;
	int r;
	char *buf;
	int buflen;

	TEST("kqueue_3");

	fd = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
	if (fd == -1)
		err(1, "open");

	// pause セット
	AUDIO_INITINFO(&ai);
	ai.play.pause = 1;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// バッファサイズ取得
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// 書き込み
	buflen = ai.play.buffer_size;
	buf = (char *)malloc(buflen);
	if (buf == NULL)
		err(1, "malloc");
	memset(buf, 0, buflen);
	do {
		r = WRITE(fd, buf, buflen);
	} while (r == buflen);
	if (r == -1) {
		XP_SYS_NG(EAGAIN, r);
	}

	// kevent
	kq = KQUEUE();
	XP_SYS_OK(kq);

	memset(&ts, 0, sizeof(ts));

	EV_SET(&kev, fd, EV_ADD, EVFILT_WRITE, 0, 0, 0);
	r = KEVENT_SET(kq, &kev, 1);
	XP_SYS_EQ(0, r);

	r = KEVENT_POLL(kq, &kev, 1, &ts);
	XP_SYS_EQ(0, r);
	if (r > 0) {
		XP_EQ(fd, kev.ident);
		XP_EQ(0, kev.data);
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
	r = CLOSE(kq);
	XP_SYS_EQ(0, r);
	free(buf);
}

// kqueue、hiwat 設定してバッファフルでテスト
void
test_kqueue_4()
{
	struct audio_info ai;
	struct kevent kev;
	struct timespec ts;
	int kq;
	int fd;
	int r;
	char *buf;
	int buflen;
	int newhiwat;

	TEST("kqueue_4");

	fd = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
	if (fd == -1)
		err(1, "open");

	// バッファサイズ、hiwat 取得
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "hiwat");
	XP_SYS_EQ(0, r);
	// なんでもいいので変更する
	newhiwat = ai.hiwat - 1;

	// pause、hiwat 設定
	AUDIO_INITINFO(&ai);
	ai.play.pause = 1;
	ai.hiwat = newhiwat;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "pause=1;hiwat");
	XP_SYS_EQ(0, r);

	// hiwat 再取得
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "hiwat");
	XP_SYS_EQ(0, r);
	XP_EQ(1, ai.play.pause);
	XP_EQ(newhiwat, ai.hiwat);

	// hiwatまでではなく目一杯書き込む
	buflen = ai.blocksize * ai.hiwat;
	buf = (char *)malloc(buflen);
	if (buf == NULL)
		err(1, "malloc");
	memset(buf, 0, buflen);
	do {
		r = WRITE(fd, buf, buflen);
	} while (r == buflen);
	if (r == -1) {
		XP_SYS_NG(EAGAIN, r);
	}

	// kevent
	kq = KQUEUE();
	XP_SYS_OK(kq);

	memset(&ts, 0, sizeof(ts));

	EV_SET(&kev, fd, EV_ADD, EVFILT_WRITE, 0, 0, fd);
	r = KEVENT_SET(kq, &kev, 1);
	XP_SYS_EQ(0, r);

	r = KEVENT_POLL(kq, &kev, 1, &ts);
	if (r > 0)
		DEBUG_KEV("kev", &kev);
	if (netbsd < 9) {
		// N7、N8 は空きが1バイト以上あれば成立する、となっている。
		// lowat を導入しといてその動作はどうなんだろうという気はする。
		// だがフィルタチェインの構造上、空いているかどうかの判断が
		// 難しいのか分からないが、pause して目一杯書き込んだはずなのに
		// 空きがあると判定されてしまうようだ。
		XP_EXPFAIL("r expects 0 but %d (filt_audiowrite is broken)", r);
	} else {
		// AUDIO2 では lowat 以上あるので WRITE は成立しない
		XP_SYS_EQ(0, r);
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
	r = CLOSE(kq);
	XP_SYS_EQ(0, r);
	free(buf);
}

// バッファフルから pause を解除した場合。
void
test_kqueue_5()
{
	struct audio_info ai, ai2;
	struct kevent kev;
	struct timespec ts;
	int fd;
	int r;
	int kq;
	char *buf;
	int buflen;

	TEST("kqueue_5");
	memset(&ts, 0, sizeof(ts));

	for (int emul = 0; emul < 2; emul++) {
		DESC("emul=%d", emul);

		if (x68k && emul == 0) {
			XP_SKIP("cannot set native encoding on vs(4)");
			continue;
		}

		// ノンブロッキングオープン
		fd = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
		if (fd == -1)
			err(1, "open");

		// pause を設定
		AUDIO_INITINFO(&ai);
		ai.play.pause = 1;
		// ついでにエンコーディングも設定
		// XXX 本当は GETENC でチェックすべきだが手抜き
		// emul==0: mulaw/1ch/8kHz は 8000 byte/sec
		// emul==1: s16/2ch/8kHz は  32000 byte/sec
		if (emul == 0) {
			// native
			ai.play.encoding = AUDIO_ENCODING_SLINEAR_NE;
			ai.play.precision = 16;
			ai.play.channels = 2;
			ai.play.sample_rate = 8000;
		}
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "enc;pause=1");
		XP_SYS_EQ(0, r);

		// いろいろ再取得
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);

		// lowat を約半分に再設定
		AUDIO_INITINFO(&ai2);
		ai2.lowat = ai.hiwat / 2;
		r = IOCTL(fd, AUDIO_SETINFO, &ai2, "lowat");
		XP_SYS_EQ(0, r);

		// でまた再取得
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "lowat");
		XP_SYS_EQ(0, r);
		DPRINTF("  > blocksize=%d hiwat=%d lowat=%d\n",
			ai.blocksize, ai.hiwat, ai.lowat);

		// 書き込み
		buflen = ai.blocksize * ai.hiwat;
		buf = (char *)malloc(buflen);
		if (buf == NULL)
			err(1, "malloc");
		memset(buf, 0, buflen);
		do {
			r = WRITE(fd, buf, buflen);
		} while (r == buflen);
		if (r == -1) {
			XP_SYS_NG(EAGAIN, r);
		}

		// kevent
		kq = KQUEUE();
		XP_SYS_OK(kq);

		EV_SET(&kev, fd, EV_ADD, EVFILT_WRITE, 0, 0, fd);
		r = KEVENT_SET(kq, &kev, 1);
		XP_SYS_EQ(0, r);

		// seek 確認
		// バッファフルなので、seek はバッファ一杯であること
		// 一杯の定義が実装によって違うがもう仕方ないな
		// N7(emul=0) は blocksize * hiwat、
		// N7(emul=1) は buffer_size
		// N8 は buffer_size
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "seek");
		XP_SYS_EQ(0, r);
		DPRINTF("  > seek=%d\n", ai.play.seek);
		if (netbsd == 7 && emul == 0) {
			XP_EQ(ai.blocksize * ai.hiwat, ai.play.seek);
		} else {
			XP_EQ(ai.play.buffer_size, ai.play.seek);
		}

		// バッファフルなので kevent は ready でないこと
		r = KEVENT_POLL(kq, &kev, 1, &ts);
		if (r >= 1)
			DEBUG_KEV("kev", &kev);
		XP_SYS_EQ(0, r);

		// pause 解除
		AUDIO_INITINFO(&ai2);
		ai2.play.pause = 0;
		r = IOCTL(fd, AUDIO_SETINFO, &ai2, "pause=0");
		XP_SYS_EQ(0, r);

		while (ai.play.seek > 0) {
			// XXX N7(emul=1) は pause 解除できてないっぽい感じ。バグ?
			if (netbsd == 7 && emul == 1) {
				XP_EXPFAIL("unpause not work on NetBSD7 ??");
				break;
			}

			usleep(250 * 1000);

			// あまり意味はないが極力間を空けずに行いたい
			r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "seek");
			XP_SYS_EQ(0, r);
			r = KEVENT_POLL(kq, &kev, 1, &ts);
			if (r >= 1)
				DEBUG_KEV("kev", &kev);

			DPRINTF("  > seek=%d\n", ai.play.seek);

			if (netbsd == 7) {
				// N7(emul=0) はバッファをクリアしてしまうようだ
				// ただし EVFILT_WRITE は連動しないようだ。
				// バグというかなんだこれ...
				XP_EXPFAIL("ai.play.seek expects !0 but %d", ai.play.seek);
				if (r == 0) {
					XP_EXPFAIL("seek=0 but EVFILT_WRITE not set");
				} else {
					XP_SYS_EQ(0, r);
				}
				break;

			} else {
				// AUDIO2 では lowat を下回ったら EVFILT_WRITE が立つ
				// といっても GETINFO と kqueue の間にはどうしても時間差が
				// あるので lowat 近くだったら検査しない、とかはどうか。
				//
				// N8 は1バイトでも空けばすぐに EVFILT_WRITE が立つように
				// 見えるのだがここのテストはこの検査をパスする。
				// 何故だか分からん。
				if (ai.play.seek >= ai.blocksize * ai.lowat * 12 / 10) {
					// lowat より(おそらくまだ)高いので EVFILT_WRITE は立たない
					XP_SYS_EQ(0, r);
				} else if (ai.play.seek < ai.blocksize * ai.lowat) {
					// lowat より低いので EVFILT_WRITE は立つはず
					XP_SYS_EQ(1, r);
				}
			}
		}

		// ただし再生する必要はないのでフラッシュする
		r = IOCTL(fd, AUDIO_FLUSH, NULL, "");
		XP_SYS_EQ(0, r);

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
		r = CLOSE(kq);
		XP_SYS_EQ(0, r);
		free(buf);
	}
}

// 隣のディスクリプタの影響を受けないこと
void
test_kqueue_6()
{
	struct audio_info ai;
	struct audio_info ai2;
	struct audio_info ai2_initial;
	struct kevent kev[2];
	struct timespec ts;
	int fd[2];
	int r;
	int kq;
	char *buf;
	int buflen;

	TEST("kqueue_6");

	// 多重化は N7 では出来ない
	if (netbsd < 8) {
		XP_SKIP("NetBSD7 does not support multi-open");
		return;
	}

	memset(&ts, 0, sizeof(ts));

	// 順序に影響されないか、前後両方試す
	for (int i = 0; i < 2; i++) {
		int a = i;
		int b = 1 - i;
		DESC("%d", i);

		fd[0] = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
		if (fd[0] == -1)
			err(1, "open");
		fd[1] = OPEN(devaudio, O_WRONLY | O_NONBLOCK);
		if (fd[1] == -1)
			err(1, "open");

		// エンコーディングを設定
		AUDIO_INITINFO(&ai);
		ai.play.encoding = AUDIO_ENCODING_ULAW;
		ai.play.precision = 8;
		ai.play.channels = 2;
		ai.play.sample_rate = 8000;
		r = IOCTL(fd[0], AUDIO_SETINFO, &ai, "mulaw/8/2ch/8000");
		XP_SYS_EQ(0, r);
		r = IOCTL(fd[1], AUDIO_SETINFO, &ai, "mulaw/8/2ch/8000");
		XP_SYS_EQ(0, r);

		// ブロックサイズ、ブロック数のために取得 (手抜きで片方だけ)
		r = IOCTL(fd[0], AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);

		// lowat を約半分に再設定。ついでに pause も設定
		AUDIO_INITINFO(&ai2);
		ai2.lowat = ai.hiwat / 2;
		ai2.play.pause = 1;
		r = IOCTL(fd[a], AUDIO_SETINFO, &ai2, "lowat,pause=1");
		XP_SYS_EQ(0, r);
		r = IOCTL(fd[b], AUDIO_SETINFO, &ai2, "lowat,pause=1");
		XP_SYS_EQ(0, r);

		// でまた再取得
		r = IOCTL(fd[a], AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);
		DPRINTF("  > blocksize=%d hiwat=%d lowat=%d\n",
			ai.blocksize, ai.hiwat, ai.lowat);
		if (ai.lowat > ai.hiwat * 6 / 10) {
			// lowat が hiwat の半分(くらい)に設定できてない?
			XP_FAIL("cannot set lowat=%d hiwat=%d ?\n", ai.lowat, ai.hiwat);
			CLOSE(fd[0]);
			CLOSE(fd[1]);
			continue;
		}

		// 書き込み
		buflen = ai.blocksize * ai.hiwat;
		buf = (char *)malloc(buflen);
		if (buf == NULL)
			err(1, "malloc");
		memset(buf, 0, buflen);
		// fdA に書き込み
		do {
			r = WRITE(fd[a], buf, buflen);
		} while (r == buflen);
		if (r == -1) {
			XP_SYS_NG(EAGAIN, r);
		}
		// fdB に書き込み
		do {
			r = WRITE(fd[b], buf, buflen);
		} while (r == buflen);
		if (r == -1) {
			XP_SYS_NG(EAGAIN, r);
		}

		// kevent
		kq = KQUEUE();
		XP_SYS_OK(kq);

		EV_SET(&kev[0], fd[a], EV_ADD, EVFILT_WRITE, 0, 0, fd[a]);
		EV_SET(&kev[1], fd[b], EV_ADD, EVFILT_WRITE, 0, 0, fd[b]);
		r = KEVENT_SET(kq, kev, 2);
		XP_SYS_EQ(0, r);

		// バッファが埋まっていて一時停止なので EVFILT_WRITE は立たないこと
		r = KEVENT_POLL(kq, kev, 1, &ts);
		XP_SYS_EQ(0, r);

		// seek のために再取得
		r = IOCTL(fd[a], AUDIO_GETBUFINFO, &ai, "seekA");
		XP_SYS_EQ(0, r);
		r = IOCTL(fd[b], AUDIO_GETBUFINFO, &ai2_initial, "seekB");
		XP_SYS_EQ(0, r);

		// fdA だけ pause 解除
		AUDIO_INITINFO(&ai2);
		ai2.play.pause = 0;
		r = IOCTL(fd[a], AUDIO_SETINFO, &ai2, "pause=0");
		XP_SYS_EQ(0, r);

		while (ai.play.seek > 0) {
			usleep(250 * 1000);

			// あまり意味はないが極力間を空けずに行いたい
			r = IOCTL(fd[a], AUDIO_GETBUFINFO, &ai, "seekA");
			XP_SYS_EQ(0, r);
			r = IOCTL(fd[b], AUDIO_GETBUFINFO, &ai2, "seekB");
			XP_SYS_EQ(0, r);
			r = KEVENT_POLL(kq, kev, 2, &ts);
			if (r >= 1)
				DEBUG_KEV("kev[0]", &kev[0]);
			if (r >= 2)
				DEBUG_KEV("kev[1]", &kev[1]);

			DPRINTF("  > seek A=%d B=%d\n", ai.play.seek, ai2.play.seek);

			// 念のため fdB の残量は変わらないこと
			XP_EQ(ai2_initial.play.seek, ai2.play.seek);

			if (1) {
				// AUDIO2 では lowat を下回ったら EVFILT_WRITE が立つ
				// といっても GETINFO と kqueue の間にはどうしても時間差が
				// あるので lowat 近くだったら検査しない、とかはどうか。
				//
				// N8 は1バイトでも空けばすぐに EVFILT_WRITE が立つように
				// 見えるのだがここのテストはこの検査をパスする。
				// 何故だか分からん。
				if (ai.play.seek >= ai.blocksize * ai.lowat * 12 / 10) {
					// lowat より(おそらくまだ)高いので EVFILT_WRITE は立たない
					XP_SYS_EQ(0, r);
				} else if (ai.play.seek < ai.blocksize * ai.lowat) {
					// lowat より低いので EVFILT_WRITE は立つはず
					XP_SYS_EQ(1, r);
				}
			}
		}

		// ただし再生する必要はないのでフラッシュする
		r = IOCTL(fd[0], AUDIO_FLUSH, NULL, "");
		XP_SYS_EQ(0, r);
		r = IOCTL(fd[1], AUDIO_FLUSH, NULL, "");
		XP_SYS_EQ(0, r);

		r = CLOSE(fd[0]);
		XP_SYS_EQ(0, r);
		r = CLOSE(fd[1]);
		XP_SYS_EQ(0, r);
		r = CLOSE(kq);
		XP_SYS_EQ(0, r);
		free(buf);
	}
}

// FIOASYNC が同時に2人設定できるか
void
test_FIOASYNC_1(void)
{
	int r;
	int fd0, fd1;
	int val;

	TEST("FIOASYNC_1");
	if (netbsd < 8) {
		XP_SKIP("NetBSD7 does not support multi-open");
		return;
	}

	// 1人目が ASYNC on
	fd0 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");
	val = 1;
	r = IOCTL(fd0, FIOASYNC, &val, "on");
	XP_SYS_EQ(0, r);

	// 続いて2人目が ASYNC on
	fd1 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");
	val = 1;
	r = IOCTL(fd1, FIOASYNC, &val, "on");
	if (netbsd == 8) {
		// FIOASYNC が分離されていないので失敗する
		XP_EXPFAIL("r expects 0 but %d", r);
	} else {
		XP_SYS_EQ(0, r);
	}

	CLOSE(fd0);
	CLOSE(fd1);
}

// FIOASYNC が別トラックに影響を与えないこと
void
test_FIOASYNC_2(void)
{
	int r;
	int fd0, fd1;
	int val;

	TEST("FIOASYNC_2");
	if (netbsd < 8) {
		XP_SKIP("NetBSD7 does not support multi-open");
		return;
	}

	// 1人目が ASYNC on
	fd0 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");
	val = 1;
	r = IOCTL(fd0, FIOASYNC, &val, "on");
	XP_EQ(0, r);

	// 続いて2人目が ASYNC off
	fd1 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");
	val = 0;
	r = IOCTL(fd1, FIOASYNC, &val, "off");
	XP_EQ(0, r);

	// XXX 中で何が起きてるかはカーネルにログを表示させるしかない。
	// fcntl(F_GETFL) で見えるフラグは audio レイヤーより上で処理された
	// もののようで、一見正しい値が返ってきているように見える。

	CLOSE(fd0);
	CLOSE(fd1);
}

// FIOASYNC リセットのタイミング
void
test_FIOASYNC_3(void)
{
	int r;
	int fd0, fd1;
	int val;

	TEST("FIOASYNC_3");
	if (netbsd < 8) {
		XP_SKIP("NetBSD7 does not support multi-open");
		return;
	}

	// 1人目がオープン
	fd0 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");

	// 2人目が ASYNC on してクローズ。2人目の ASYNC 状態は無効
	fd1 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");
	val = 1;
	r = IOCTL(fd1, FIOASYNC, &val, "on");
	XP_SYS_EQ(0, r);
	CLOSE(fd1);

	// もう一回2人目がオープンして ASYNC on
	fd1 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");
	val = 1;
	r = IOCTL(fd1, FIOASYNC, &val, "on");
	if (netbsd == 8) {
		// FIOASYNC が分離されていないので失敗する
		XP_EXPFAIL("r expects 0 but %d", r);
	} else {
		XP_SYS_EQ(0, r);
	}
	CLOSE(fd1);
	CLOSE(fd0);
}

volatile int sigio_caught;
void
signal_FIOASYNC_4(int signo)
{
	if (signo == SIGIO)
		sigio_caught = 1;
}

// 書き込みで SIGIO が飛んでくるか
// いまいち N7 でシグナルが飛んでくる条件が分からん。
// PLAY_ALL でブロックサイズ書き込んだら飛んでくるようだ。
// PLAY/PLAY_ALL で 4バイト書き込むとかではだめだった。
void
test_FIOASYNC_4(void)
{
	struct audio_info ai;
	int r;
	int fd;
	int val;
	char *data;

	TEST("FIOASYNC_4");
	signal(SIGIO, signal_FIOASYNC_4);
	sigio_caught = 0;

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	if (r == -1)
		err(1, "ioctl");
	if (ai.blocksize == 0)
		errx(1, "blocksize == 0");
	data = (char *)malloc(ai.blocksize);
	if (data == NULL)
		err(1, "malloc");
	memset(data, 0, ai.blocksize);

	val = 1;
	r = IOCTL(fd, FIOASYNC, &val, "on");
	XP_SYS_EQ(0, r);

	r = WRITE(fd, data, ai.blocksize);
	XP_SYS_EQ(ai.blocksize, r);

	// AUDIO2 では1ブロック書き込んだだけではまだミキサーが開始しないので
	// 待っててもシグナル上がってこない。
	if (netbsd == 9) {
		r = WRITE(fd, data, ai.blocksize);
		XP_SYS_EQ(ai.blocksize, r);
	}

	for (int i = 0; i < 10 && sigio_caught == 0; i++) {
		usleep(10000);
	}
	if (netbsd == 8) {
		// N8 ではシグナル飛んでこない。詳細不明。
		XP_EXPFAIL("sigio_caught expects 1 but %d", sigio_caught);
	} else {
		XP_EQ(1, sigio_caught);
	}

	CLOSE(fd);
	free(data);

	signal(SIGIO, SIG_IGN);
	sigio_caught = 0;
}

// 録音で SIGIO が飛んでくるか
void
test_FIOASYNC_5(void)
{
	int r;
	int fd;
	int val;

	TEST("FIOASYNC_5");
	signal(SIGIO, signal_FIOASYNC_4);
	sigio_caught = 0;

	fd = OPEN(devaudio, O_RDONLY);
	if (fd == -1)
		err(1, "open");

	val = 1;
	r = IOCTL(fd, FIOASYNC, &val, "on");
	XP_SYS_EQ(0, r);

	r = READ(fd, &r, 4);
	XP_SYS_EQ(4, r);

	for (int i = 0; i < 10 && sigio_caught == 0; i++) {
		usleep(10000);
	}
	if (netbsd == 8) {
		// N8 ではシグナル飛んでこない。詳細不明。
		XP_EXPFAIL("sigio_caught expects 1 but %d", sigio_caught);
	} else {
		XP_EQ(1, sigio_caught);
	}

	CLOSE(fd);

	signal(SIGIO, SIG_IGN);
	sigio_caught = 0;
}

// AUDIO_WSEEK の動作確認
void
test_AUDIO_WSEEK_1(void)
{
	int r;
	int fd;
	int n;

	TEST("AUDIO_WSEEK_1");

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open: %s", devaudio);

	struct audio_info ai;
	AUDIO_INITINFO(&ai);
	ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
	ai.play.precision = 8;
	ai.play.channels = (netbsd <= 7 && x68k) ? 1 : 2;
	ai.play.sample_rate = 11050;
	ai.play.pause = 1;
	ai.mode = AUMODE_PLAY_ALL;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "pause=1");
	if (r == -1)
		err(1, "AUDIO_SETINFO.pause");

	// 初期状態だと 0 バイトになる
	n = 0;
	r = IOCTL(fd, AUDIO_WSEEK, &n, "");
	XP_SYS_EQ(0, r);
	XP_EQ(0, n);

	// 4バイト書き込むと 4になる
	r = WRITE(fd, &r, 4);
	if (r == -1)
		err(1, "write(4)");
	r = IOCTL(fd, AUDIO_WSEEK, &n, "");
	XP_SYS_EQ(0, r);
	if (netbsd < 9) {
		// N7 では 0 になる。
		// おそらく WSEEK が pustream のバイト数を返しているが、
		// データはすでにこの先のバッファに送られたんじゃないかと。
		XP_EQ(0, n);
	} else {
		XP_EQ(4, n);
	}

	CLOSE(fd);
}

// SETFD は N7 ならオープンモードに関わらず audio layer の状態を変えるの意
// (で同時に MD の状態も必要なら変える)。GETFD は audio layer の duplex 状態
// を取得するだけ。
// N8 はソースコード上踏襲しているので見た目の動作は同じだが、検討した上での
// それかどうかが謎。
// AUDIO2 では GETFD は実質 HW の duplex を取得するのと等価。SETFD は
// 今とは違う方には変更できない。
void
test_AUDIO_SETFD_ONLY(void)
{
	struct audio_info ai;
	int r;
	int fd;
	int n;

	TEST("AUDIO_SETFD_ONLY");
	for (int mode = 0; mode <= 1; mode++) {
		DESC("%s", openmodetable[mode]);

		fd = OPEN(devaudio, mode);
		if (fd == -1)
			err(1, "open: %s", devaudio);

		// オープン直後は常に Half
		n = 0;
		r = IOCTL(fd, AUDIO_GETFD, &n, "");
		XP_SYS_EQ(0, r);
		XP_EQ(0, n);

		// Full duplex に設定しようとすると、
		// N7: HW Full なら設定できる、HW Half ならエラー。
		// AUDIO2: 変更は出来ない
		n = 1;
		r = IOCTL(fd, AUDIO_SETFD, &n, "on");
		if (netbsd <= 7) {
			if (hwfull) {
				XP_SYS_EQ(0, r);
			} else {
				XP_SYS_NG(ENOTTY, r);
			}
		} else if (netbsd == 8) {
			XP_EXPFAIL("not well considered?");
		} else {
			XP_SYS_NG(ENOTTY, r);
		}

		// 取得してみると、
		// N7: HW Full なら 1、HW Half なら 0 のまま。
		// AUDIO2: 変わっていないこと。
		n = 0;
		r = IOCTL(fd, AUDIO_GETFD, &n, "");
		XP_SYS_EQ(0, r);
		if (netbsd <= 7) {
			XP_EQ(hwfull, n);
		} else if (netbsd == 8) {
			XP_EXPFAIL("not well considered?");
		} else {
			XP_EQ(0, n);
		}

		// GETINFO の ai.*.open などトラック状態は変化しない。
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);
		XP_EQ(mode2popen_full[mode], ai.play.open);
		XP_EQ(mode2ropen_full[mode], ai.record.open);

		// Half duplex に設定しようとすると、
		// N7: HW Full なら設定できる、HW Half なら何も起きず成功。
		// AUDIO2: 実質変わらないので成功する。
		n = 0;
		r = IOCTL(fd, AUDIO_SETFD, &n, "off");
		XP_SYS_EQ(0, r);

		// 取得してみると、
		// N7: HW Full なら 0、HW Half なら 0 のまま。
		// AUDIO2: 変わっていないこと。
		n = 0;
		r = IOCTL(fd, AUDIO_GETFD, &n, "");
		XP_SYS_EQ(0, r);
		XP_EQ(0, n);

		// GETINFO の ai.*.open などトラック状態は変化しない。
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);
		XP_EQ(mode2popen_full[mode], ai.play.open);
		XP_EQ(mode2ropen_full[mode], ai.record.open);

		CLOSE(fd);
	}
}

void
test_AUDIO_SETFD_RDWR(void)
{
	struct audio_info ai;
	int r;
	int fd;
	int n;

	TEST("AUDIO_SETFD_RDWR");

	fd = OPEN(devaudio, O_RDWR);
	if (fd == -1)
		err(1, "open: %s", devaudio);

	// O_RDWR オープン直後は必ず Half と manpage に書いてあるが
	// N7: HW Full なら Full になる。manpage のバグということにしておく。
	// AUDIO2: HW Full なら Full、HW Half なら Half になる。
	n = 0;
	r = IOCTL(fd, AUDIO_GETFD, &n, "");
	XP_SYS_EQ(0, r);
	XP_EQ(hwfull, n);

	// Full duplex に設定しようとすると
	// HW Full なら設定できる(何も起きない)、HW Half ならエラー
	n = 1;
	r = IOCTL(fd, AUDIO_SETFD, &n, "on");
	if (hwfull) {
		XP_SYS_EQ(0, r);
	} else {
		XP_SYS_NG(ENOTTY, r);
	}

	// 取得してみると、
	// HW Full なら 1、HW Half なら 0 のまま。
	n = 0;
	r = IOCTL(fd, AUDIO_GETFD, &n, "");
	XP_SYS_EQ(0, r);
	XP_EQ(hwfull, n);

	// GETINFO の ai.*.open などトラック状態は変化しない。
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(1, ai.play.open);
	XP_EQ(1, ai.record.open);

	// Half duplex に設定しようとすると、
	// N7: HW Full なら設定できる、HW Half なら何も起きず成功。
	// AUDIO2: HW Full ならエラー、HW Half なら何も起きず成功。
	n = 0;
	r = IOCTL(fd, AUDIO_SETFD, &n, "off");
	if (netbsd <= 7) {
		XP_SYS_EQ(0, r);
	} else if (netbsd == 8) {
		XP_EXPFAIL("not well considered?");
	} else {
		if (hwfull) {
			XP_SYS_NG(ENOTTY, r);
		} else {
			XP_SYS_EQ(0, r);
		}
	}

	// 取得してみると、
	// N7: HW Full なら 0、HW Half なら 0 のまま。
	// AUDIO2: HW Full なら 1、HW Half なら 0 のまま。
	n = 0;
	r = IOCTL(fd, AUDIO_GETFD, &n, "");
	XP_SYS_EQ(0, r);
	if (netbsd <= 7) {
		XP_EQ(0, n);
	} else if (netbsd == 8) {
		XP_EXPFAIL("not well considered?");
	} else {
		XP_EQ(hwfull, n);
	}

	// GETINFO の ai.*.open などトラック状態は変化しない。
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(1, ai.play.open);
	XP_EQ(1, ai.record.open);

	CLOSE(fd);
}

// ai.play.seek、ai.play.samples が取得できるか確認
void
test_AUDIO_GETINFO_seek()
{
	struct audio_info ai;
	int r;
	int fd;
	char *buf;
	int bufsize;

	TEST("AUDIO_GETINFO_seek");

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	// 1サンプル != 1バイトにしたい
	AUDIO_INITINFO(&ai);
	ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
	ai.play.precision = 16;
	ai.play.channels = x68k ? 1 : 2;
	ai.play.sample_rate = 16000;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// 最初は seek も samples もゼロ
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(0, ai.play.seek);
	XP_EQ(0, ai.play.samples);
	// ついでにここでバッファサイズを取得
	// N7(ここはemul=0) は blocksize * hiwat
	// N8 は buffer_size
	if (netbsd == 7)
		bufsize = ai.blocksize * ai.hiwat;
	else
		bufsize = ai.play.buffer_size;

	buf = (char *)malloc(bufsize);
	if (buf == NULL)
		err(1, "malloc");
	memset(buf, 0, bufsize);

	// 全部を書き込む (リングバッファのポインタが先頭に戻るはず)
	r = WRITE(fd, buf, bufsize);
	XP_SYS_EQ(bufsize, r);
	r = IOCTL(fd, AUDIO_DRAIN, NULL, "");
	XP_SYS_EQ(0, r);

	// seek は折り返して 0 に戻ること?
	// samples は計上を続けることだけど、
	// N7 はカウントがぴったり合わないようだ
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	if (ai.play.seek != 0 && netbsd == 7) {
		XP_EXPFAIL("seek expects zero but %d ?", ai.play.seek);
	} else {
		XP_EQ(0, ai.play.seek);
	}
	if (netbsd <= 7) {
		if (bufsize != ai.play.samples && ai.play.samples > bufsize * 9 / 10) {
			XP_EXPFAIL("ai.play.samples expects %d but %d"
			        " (unknown few drops?)",
				bufsize, ai.play.samples);
		} else {
			XP_EQ(bufsize, ai.play.samples);
		}
	} else if (netbsd == 8) {
		XP_EXPFAIL("ai.play.samples behavior is unknown");
	} else {
		XP_EQ(bufsize, ai.play.samples);
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);

	free(buf);
}

void
test_AUDIO_GETINFO_eof(void)
{
	struct audio_info ai;
	int r;
	int fd, fd1;

	TEST("AUDIO_GETINFO_eof");

	fd = OPEN(devaudio, O_RDWR);
	if (fd == -1)
		err(1, "open");

	// 書き込みを伴うので pause にしとく
	AUDIO_INITINFO(&ai);
	ai.play.pause = 1;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "pause");
	if (r == -1)
		err(1, "ioctl");

	// 最初は 0
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(0, ai.play.eof);
	XP_EQ(0, ai.record.eof);

	// 0バイト書き込むと上がる
	r = WRITE(fd, &r, 0);
	if (r == -1)
		err(1, "write");
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(1, ai.play.eof);
	XP_EQ(0, ai.record.eof);

	// 1バイト以上を書き込んでも上がらない
	r = WRITE(fd, &r, 4);
	if (r == -1)
		err(1, "write");
	memset(&ai, 0, sizeof(ai));
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(1, ai.play.eof);
	XP_EQ(0, ai.record.eof);

	// もう一度0バイト書き込むと上がる
	r = WRITE(fd, &r, 0);
	if (r == -1)
		err(1, "write");
	memset(&ai, 0, sizeof(ai));
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(2, ai.play.eof);
	XP_EQ(0, ai.record.eof);

	// 0バイト読んでも上がらない
	if (hwfull) {
		r = READ(fd, &r, 0);
		if (r == -1)
			err(1, "read");
		memset(&ai, 0, sizeof(ai));
		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);
		XP_EQ(2, ai.play.eof);
		XP_EQ(0, ai.record.eof);
	}

	// 別ディスクリプタと干渉しないこと
	if (netbsd >= 8) {
		fd1 = OPEN(devaudio, O_RDWR);
		if (fd1 == -1)
			err(1, "open");
		memset(&ai, 0, sizeof(ai));
		r = IOCTL(fd1, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);
		if (netbsd == 8) {
			// N8 は別ディスクリプタと干渉する
			XP_EXPFAIL("ai.play.eof expects 0 but %d", ai.play.eof);
		} else {
			XP_EQ(0, ai.play.eof);
		}
		XP_EQ(0, ai.record.eof);
		CLOSE(fd1);
	}

	CLOSE(fd);

	// オープンしなおすとリセット
	fd = OPEN(devaudio, O_RDWR);
	if (fd == -1)
		err(1, "open");

	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(0, ai.play.eof);
	XP_EQ(0, ai.record.eof);

	CLOSE(fd);
}

// オープン直後の mode と、
// SETINFO で mode が切り替わるケース
void
test_AUDIO_SETINFO_mode()
{
	struct audio_info ai;
	char buf[10];
	int r;
	int fd;
	struct {
		int openmode;	// オープン時のモード (O_*)
		int inimode;	// オープン直後の aumode 期待値
		int setmode;	// aumode 設定値
		int expmode7;	// 設定変更後のaumode期待値(N7)
		int expmode;	// 設定変更後のaumode期待値
#define P	AUMODE_PLAY
#define A	AUMODE_PLAY_ALL
#define R	AUMODE_RECORD
	} exptable[] = {
		// open		inimode	setmode		N7expmode	expmode
		{ O_RDONLY,	R,		  0x0,		  0x0,		R },
		{ O_RDONLY,	R,		    P,		    P,		R },
		{ O_RDONLY,	R,		  A  ,		  A|P,		R },
		{ O_RDONLY,	R,		  A|P,		  A|P,		R },
		{ O_RDONLY,	R,		R    ,		R    ,		R },
		{ O_RDONLY,	R,		R|  P,		    P,		R },
		{ O_RDONLY,	R,		R|A  ,		  A|P,		R },
		{ O_RDONLY,	R,		R|A|P,		  A|P,		R },
		{ O_RDONLY,	R,		  0x8,		  0x8,		R },
		{ O_RDONLY,	R,		  0x9,		  0x9,		R },

		{ O_WRONLY,	  A|P,	  0x0,		  0x0,		    P },
		{ O_WRONLY,	  A|P,	    P,		    P,		    P },
		{ O_WRONLY,	  A|P,	  A  ,		  A|P,		  A|P },
		{ O_WRONLY,	  A|P,	  A|P,		  A|P,		  A|P },
		{ O_WRONLY,	  A|P,	R    ,		R    ,		    P },
		{ O_WRONLY,	  A|P,	R|  P,		    P,		    P },
		{ O_WRONLY,	  A|P,	R|A  ,		  A|P,		  A|P },
		{ O_WRONLY,	  A|P,	R|A|P,		  A|P,		  A|P },
		{ O_WRONLY,	  A|P,	  0x8,		  0x8,		    P },
		{ O_WRONLY,	  A|P,	  0x9,		  0x9,		    P },

		// HWFull の場合
		{ O_RDWR,	R|A|P,	  0x0,		  0x0,		R|  P },
		{ O_RDWR,	R|A|P,	    P,		    P,		R|  P },
		{ O_RDWR,	R|A|P,	  A  ,		  A|P,		R|A|P },
		{ O_RDWR,	R|A|P,	  A|P,		  A|P,		R|A|P },
		{ O_RDWR,	R|A|P,	R    ,		R    ,		R|  P },
		{ O_RDWR,	R|A|P,	R|  P,		R|  P,		R|  P },
		{ O_RDWR,	R|A|P,	R|A  ,		R|A|P,		R|A|P },
		{ O_RDWR,	R|A|P,	R|A|P,		R|A|P,		R|A|P },
		{ O_RDWR,	R|A|P,	  0x8,		  0x8,		R|  P },
		{ O_RDWR,	R|A|P,	  0x9,		  0x9,		R|  P },

		// HWHalf の場合 (-O_RDWR を取り出した時に加工する)
		{ -O_RDWR,	  A|P,	  0x0,		  0x0,		    P },
		{ -O_RDWR,	  A|P,	    P,		    P,		    P },
		{ -O_RDWR,	  A|P,	  A  ,		  A|P,		  A|P },
		{ -O_RDWR,	  A|P,	  A|P,		  A|P,		  A|P },
		{ -O_RDWR,	  A|P,	R    ,		R    ,		    P },
		{ -O_RDWR,	  A|P,	R|  P,		    P,		    P },
		{ -O_RDWR,	  A|P,	R|A  ,		  A|P,		  A|P },
		{ -O_RDWR,	  A|P,	R|A|P,		  A|P,		  A|P },
		{ -O_RDWR,	  A|P,	  0x8,		  0x8,		    P },
		{ -O_RDWR,	  A|P,	  0x9,		  0x9,		    P },
	};
#undef P
#undef A
#undef R

	TEST("AUDIO_SETINFO_mode");
	for (int i = 0; i < __arraycount(exptable); i++) {
		int openmode = exptable[i].openmode;
		int inimode = exptable[i].inimode;
		int setmode = exptable[i].setmode;
		int expmode = (netbsd <= 8)
			? exptable[i].expmode7
			: exptable[i].expmode;
		int half;

		half = 0;
		if (hwfull) {
			// HWFull なら O_RDWR のほう
			if (openmode < 0)
				continue;
		} else {
			// HWHalf なら O_RDWR は負数のほう
			if (openmode == O_RDWR)
				continue;
			if (openmode == -O_RDWR) {
				openmode = O_RDWR;
				half = 1;
			}
		}

		char setmodestr[32];
		snprintb_m(setmodestr, sizeof(setmodestr),
			"\177\020b\1REC\0b\2ALL\0b\0PLAY\0", setmode, 0);

		DESC("%s%s,%s",
			half ? "H:" : "",
			openmodetable[openmode], setmodestr);

		fd = OPEN(devaudio, openmode);
		if (fd == -1)
			err(1, "open");

		// オープンした直後の状態
		memset(&ai, 0, sizeof(ai));
		r = IOCTL(fd, AUDIO_GETINFO, &ai, "");
		if (r == -1)
			err(1, "ioctl");
		XP_EQ(inimode, ai.mode);
		XP_EQ(mode2popen_full[openmode], ai.play.open);
		XP_EQ(mode2ropen_full[openmode], ai.record.open);
		// N7、N8 では buffer_size は常に非ゼロなので調べない
		// A2: バッファは O_RDWR なら HWHalf でも両方確保される。
		// Half なのを判定するほうが後なのでやむをえないか。
		// 確保されてたらいけないわけでもないだろうし(無駄ではあるけど)。
		if (netbsd >= 9) {
			XP_BUFFSIZE(mode2popen_full[openmode], ai.play.buffer_size);
			XP_BUFFSIZE(mode2ropen_full[openmode], ai.record.buffer_size);
		}

		// mode を変える
		// ついでに pause にしとく
		ai.mode = setmode;
		ai.play.pause = 1;
		ai.record.pause = 1;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "mode");
		XP_SYS_EQ(0, r);
		if (r == 0) {
			r = IOCTL(fd, AUDIO_GETINFO, &ai, "");
			XP_SYS_EQ(0, r);
			XP_EQ(expmode, ai.mode);
			// mode に関係なく当初のオープンモードを維持するようだ
			XP_EQ(mode2popen_full[openmode], ai.play.open);
			XP_EQ(mode2ropen_full[openmode], ai.record.open);
			// N7、N8 では buffer_size は常に非ゼロなので調べない
			if (netbsd >= 9) {
				XP_BUFFSIZE(mode2popen_full[openmode], ai.play.buffer_size);
				XP_BUFFSIZE(mode2ropen_full[openmode], ai.record.buffer_size);
			}
		}

		// 書き込みが出来るかどうかは
		// N7: オープン時の openmode による
		// A2: オープン時の inimode による、としたい
		// オープン後に変えた mode は適用されない。
		bool canwrite = (netbsd <= 8)
			? (openmode != O_RDONLY)
			: ((inimode & AUMODE_PLAY) != 0);
		r = WRITE(fd, buf, 0);
		if (canwrite) {
			XP_SYS_EQ(0, r);
		} else {
			XP_SYS_NG(EBADF, r);
		}

		// 読み込みが出来るかどうかは
		// N7: オープン時の openmode による
		// A2: オープン時の inimode による、としたい
		// オープン後に変えた mode は適用されない。
		bool canread = (netbsd <= 8)
			? (openmode != O_WRONLY)
			: ((inimode & AUMODE_RECORD) != 0);
		r = READ(fd, buf, 0);
		if (canread) {
			XP_SYS_EQ(0, r);
		} else {
			XP_SYS_NG(EBADF, r);
		}

		CLOSE(fd);
	}
}

// {play,rec}.params の設定がきくか。
void
test_AUDIO_SETINFO_params()
{
	struct audio_info ai;
	int r;
	int fd;

	TEST("AUDIO_SETINFO_params");
	for (int openmode = 0; openmode <= 2; openmode++) {
		for (int aimode = 0; aimode <= 1; aimode++) {
			for (int pause = 0; pause <= 1; pause++) {
				// aimode は ai.mode を変更するかどうか
				// pause は ai.*.pause を変更するかどうか

				// rec のみだと ai.mode は変更できない
				if (openmode == O_RDONLY && aimode == 1)
					continue;

				// half だと O_RDWR は O_WRONLY と同じ
				if (hwfull == 0 && openmode == O_RDWR)
					continue;

				DESC("%s,mode%d,pause%d",
					openmodetable[openmode], aimode, pause);

				fd = OPEN(devaudio, openmode);
				if (fd == -1)
					err(1, "open");

				AUDIO_INITINFO(&ai);
				// params が全部独立して効くかどうかは大変なのでとりあえず
				// sample_rate で代表させる。
				ai.play.sample_rate = 11025;
				ai.record.sample_rate = 11025;
				if (aimode)
					ai.mode = mode2aumode_full[openmode] & ~AUMODE_PLAY_ALL;
				if (pause) {
					ai.play.pause = 1;
					ai.record.pause = 1;
				}

				r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
				XP_SYS_EQ(0, r);

				r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
				XP_SYS_EQ(0, r);
				int expmode = (aimode)
					? mode2aumode_full[openmode] & ~AUMODE_PLAY_ALL
					: mode2aumode_full[openmode];
				XP_EQ(expmode, ai.mode);
				if (openmode == O_RDONLY) {
					// play なし
					if (netbsd <= 8)
						XP_EQ(pause, ai.play.pause);
					else
						XP_EQ(0, ai.play.pause);
				} else {
					// play あり
					XP_EQ(11025, ai.play.sample_rate);
					XP_EQ(pause, ai.play.pause);
				}
				if (openmode == O_WRONLY) {
					// rec なし
					if (netbsd <= 8)
						XP_EQ(pause, ai.record.pause);
					else
						XP_EQ(0, ai.record.pause);
				} else {
					// rec あり
					XP_EQ(11025, ai.record.sample_rate);
					XP_EQ(pause, ai.record.pause);
				}

				r = CLOSE(fd);
				XP_SYS_EQ(0, r);
			}
		}
	}
}

// 別のディスクリプタでの SETINFO に干渉されないこと
void
test_AUDIO_SETINFO_params2()
{
	struct audio_info ai;
	int r;
	int fd0;
	int fd1;

	TEST("AUDIO_SETINFO_params2");
	// N7 は多重オープンはできない
	if (netbsd <= 7) {
		XP_SKIP("NetBSD7 does not support multi-open");
		return;
	}

	fd0 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");

	// 1本目のパラメータを変える
	AUDIO_INITINFO(&ai);
	ai.play.sample_rate = 11025;
	r = IOCTL(fd0, AUDIO_SETINFO, &ai, "");
	XP_SYS_EQ(0, r);

	fd1 = OPEN(devaudio, O_WRONLY);
	if (fd1 == -1)
		err(1, "open");

	// 2本目で同じパラメータを変える
	AUDIO_INITINFO(&ai);
	ai.play.sample_rate = 16000;
	r = IOCTL(fd1, AUDIO_SETINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// 1本目のパラメータ変更の続き
	AUDIO_INITINFO(&ai);
	ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
	r = IOCTL(fd0, AUDIO_SETINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// sample_rate は 11k のままであること
	r = IOCTL(fd0, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(AUDIO_ENCODING_SLINEAR_LE, ai.play.encoding);
	XP_EQ(11025, ai.play.sample_rate);

	r = CLOSE(fd0);
	XP_SYS_EQ(0, r);
	r = CLOSE(fd1);
	XP_SYS_EQ(0, r);
}

void
test_AUDIO_SETINFO_params3()
{
	struct audio_info ai;
	int fd0;
	int fd1;
	int r;

	TEST("AUDIO_SETINFO_params3");
	if (netbsd <= 7) {
		XP_SKIP("NetBSD7 does not support multi-open");
		return;
	}

	// 1本目 /dev/audio を再生側だけ開く
	fd0 = OPEN(devaudio, O_WRONLY);
	if (fd0 == -1)
		err(1, "open");

	// 2本目 /dev/audio を両方開く
	fd1 = OPEN(devaudio, O_RDWR);
	if (fd1 == -1)
		err(1, "open");

	// 2本目で両トラックを SETINFO
	AUDIO_INITINFO(&ai);
	ai.play.sample_rate = 11025;
	ai.record.sample_rate = 11025;
	r = IOCTL(fd1, AUDIO_SETINFO, &ai, "");
	XP_SYS_EQ(0, r);

	// 1本目で GETINFO しても両トラックとも影響を受けていないこと
	memset(&ai, 0, sizeof(ai));
	r = IOCTL(fd0, AUDIO_GETINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(8000, ai.play.sample_rate);
	XP_EQ(8000, ai.record.sample_rate);

	r = CLOSE(fd0);
	XP_SYS_EQ(0, r);
	r = CLOSE(fd1);
	XP_SYS_EQ(0, r);
}

// pause の設定がきくか。
// play のみ、rec のみ、play/rec 両方について
// ai.mode 変更ありなし、ai.play.* 変更ありなしについて調べる
void
test_AUDIO_SETINFO_pause()
{
	struct audio_info ai;
	int r;
	int fd;

	TEST("AUDIO_SETINFO_pause");
	for (int openmode = 0; openmode <= 2; openmode++) {
		for (int aimode = 0; aimode <= 1; aimode++) {
			for (int param = 0; param <= 1; param++) {
				// aimode は ai.mode を変更するかどうか
				// param は ai.*.param を変更するかどうか

				// rec のみだと ai.mode は変更できない
				if (openmode == O_RDONLY && aimode == 1)
					continue;

				// half だと O_RDWR は O_WRONLY と同じ
				if (hwfull == 0 && openmode == O_RDWR)
					continue;

				DESC("%s,mode%d,param%d",
					openmodetable[openmode], aimode, param);

				fd = OPEN(devaudio, openmode);
				if (fd == -1)
					err(1, "open");

				// pause をあげる
				AUDIO_INITINFO(&ai);
				ai.play.pause = 1;
				ai.record.pause = 1;
				if (aimode)
					ai.mode = mode2aumode_full[openmode] & ~AUMODE_PLAY_ALL;
				if (param) {
					ai.play.sample_rate = 11025;
					ai.record.sample_rate = 11025;
				}

				r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
				XP_SYS_EQ(0, r);

				r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
				XP_SYS_EQ(0, r);
				int expmode = (aimode)
					? mode2aumode_full[openmode] & ~AUMODE_PLAY_ALL
					: mode2aumode_full[openmode];
				XP_EQ(expmode, ai.mode);
				if (openmode == O_RDONLY) {
					// play がない
					if (netbsd <= 8)
						XP_EQ(1, ai.play.pause);
					else
						XP_EQ(0, ai.play.pause);
				} else {
					// play がある
					XP_EQ(1, ai.play.pause);
					XP_EQ(param ? 11025 : 8000, ai.play.sample_rate);
				}
				if (openmode == O_WRONLY) {
					// rec がない
					if (netbsd <= 8)
						XP_EQ(1, ai.record.pause);
					else
						XP_EQ(0, ai.record.pause);
				} else {
					XP_EQ(1, ai.record.pause);
					XP_EQ(param ? 11025 : 8000, ai.record.sample_rate);
				}

				// pause を下げるテストもやる?
				AUDIO_INITINFO(&ai);
				ai.play.pause = 0;
				ai.record.pause = 0;
				if (aimode)
					ai.mode = mode2aumode_full[openmode];
				if (param) {
					ai.play.sample_rate = 16000;
					ai.record.sample_rate = 16000;
				}

				r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
				XP_SYS_EQ(0, r);

				r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
				XP_SYS_EQ(0, r);
				XP_EQ(mode2aumode_full[openmode], ai.mode);
				XP_EQ(0, ai.play.pause);
				XP_EQ(0, ai.record.pause);
				if (openmode != O_RDONLY)
					XP_EQ(param ? 16000 : 8000, ai.play.sample_rate);
				if (openmode != O_WRONLY)
					XP_EQ(param ? 16000 : 8000, ai.record.sample_rate);

				r = CLOSE(fd);
				XP_SYS_EQ(0, r);
			}
		}
	}
}

// SETINFO で blocksize をいろいろ設定してみる。
void
test_AUDIO_SETINFO_blocksize()
{
	struct audio_info ai, ai0;
	int fd;
	int r;
	u_int initblksize;
	int expect_inoutparam;

	// SETINFO の blocksize の項目はセットできた値が返ってくる(つまり
	// in-out parameter) とあるが、実装はそうなってない気がする。
	expect_inoutparam = 0;

	TEST("AUDIO_SETINFO_blocksize[inout=%d]", expect_inoutparam);

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	// 最初の blocksize を取得しておく
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	XP_SYS_EQ(0, r);
	initblksize = ai.blocksize;
	DPRINTF("initial blocksize=%d\n", initblksize);

	// ゼロをセットしてみる
	DESC("0");
	AUDIO_INITINFO(&ai);
	ai.blocksize = 0;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "ai.blocksize=0");
	XP_SYS_EQ(0, r);
	if (expect_inoutparam) {
		// manpage 通りならセットされた値(たぶん初期値と同じはず)が帰る
		XP_EQ(initblksize, ai.blocksize);
	} else {
		// 実際には in-out パラメータではないようだ
		XP_EQ(0, ai.blocksize);
	}

	struct {
		u_int blocksize;
		u_int expected;
	} table[] = {
		{ 1,			1 },
		{ 32,			32 },
		{ 64,			64 },
		{ 128,			128 },
		{ 129,			129 },
		{ 0x80000000,	initblksize },	// ほんまかいな
	};
	for (int i = 0; i < __arraycount(table); i++) {
		u_int blocksize = table[i].blocksize;
		u_int expected = table[i].expected;

		DESC("%u", blocksize);

		// ただし MI-MD 仕様が gdgd でテストできないケースが多い。
		if (netbsd == 7) {
			// 32 が最小
			if (expected == 1)
				expected = 32;

			// hw->round_blocksize が keep good alignment とか言って
			// 下位ビットを落としてるやつは、それより小さい blocksize を渡すと
			// 0 が返る。そして呼び出し側は round_blocksize() の戻り値が
			// 0 以下だと panic するようになっている。
			if (strcmp(hwconfig, "auich0") == 0) {
				if ((int)blocksize < 64) {
					XP_EXPFAIL("it causes panic on NetBSD7 + auich");
					continue;
				}
				// 64バイト単位に落とされる
				expected &= -64;
			}
			if (strcmp(hwconfig, "eap0") == 0) {
				// 32バイト単位に落とされる
				expected &= -32;
			}
		}

		// セット
		AUDIO_INITINFO(&ai);
		ai.blocksize = blocksize;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "ai.blocksize=%u", ai.blocksize);
		XP_SYS_EQ(0, r);

		if (expect_inoutparam) {
			// SETINFO が blocksize を更新する場合

			// もう一度取得してみる
			memset(&ai0, 0, sizeof(ai0));
			r = IOCTL(fd, AUDIO_GETBUFINFO, &ai0, "");
			XP_SYS_EQ(0, r);
			if (expected) {
				XP_EQ(expected, ai.blocksize);
			} else {
				XP_EQ(initblksize, ai.blocksize);
			}

			// SETINFO 時の戻り blocksize は setinfo が設定したサイズに
			// なっているはずなので、この2つは一致するはず。
			XP_EQ(ai0.blocksize, ai.blocksize);
		} else {
			// SETINFO が blocksize を更新しない場合
			XP_EQ(blocksize, ai.blocksize);
		}
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// SETINFO で設定した hiwat, lowat が GETINFO でどう見えるか。
void
test_AUDIO_SETINFO_hiwat1()
{
	struct audio_info ai;
	int fd;
	int r;
	u_int inihi;
	u_int inilo;
	u_int max;

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	// オープン直後の状態は open_[23] で調べてあるので、
	// ここでは初期状態だけ取得。
	r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
	inihi = ai.hiwat;
	inilo = ai.lowat;
	max = ai.play.buffer_size / ai.blocksize;
	if (debug) {
		printf("inihi=%d inilow=%d, buff_size=%d blksize=%d\n",
			inihi, inilo, ai.play.buffer_size, ai.blocksize);
	}

	struct {
		u_int hiwat;
		u_int lowat;
		u_int exphi;
		u_int explo;
	} table[] = {
		{ max,		-1,		max,	inilo },	// hi=max
		{ max+1,	-1,		max,	inilo },	// hi=max 越え
		{ max-1,	-1,		max-1,	inilo },	// max-1は指定可能
		{ inihi,	0,		inihi,	0 },		// lo=0 は可能
		{ -1,		1,		inihi,	1 },		// lo=1 は可能
		{ -1,		max-1,	inihi,	max-1 },	// lo=max-1
		{ -1,		max,	inihi,	max-1 },	// lo=max はNG
		{ -1,		max+1,	inihi,	max-1 },	// lo=max+1 もNG
		{ max-1,	max-1,	max-1,	max-2 },	// hi==lo
		{ max-1,	max,	max-1,	max-2 },	// hi<lo
		{ max-1,	max+1,	max-1,	max-2 },	// lo>MAX
		{ max,		max,	max,	max-1 },	// hi==lo==MAX
		{ 0,		0,		2,		0 },		// hi=0 は不可
		{ 1,		0,		2,		0 },		// hi=1 は不可
		{ 2,		0,		2,		0 },		// hi=2 は可
		{ 2,		1,		2,		1 },		// {2,1} は可
	};

	TEST("AUDIO_SETINFO_hiwat1");
	for (int i = 0; i < __arraycount(table); i++) {
		u_int hiwat = table[i].hiwat;
		u_int lowat = table[i].lowat;
		u_int exphi = table[i].exphi;
		u_int explo = table[i].explo;

		// N8 では
		// lowat が 3 未満なら 3 にする。
		// hiwat が lowat + 1 未満なら lowat + 1 にする。
		// なのかな?
		if (netbsd == 8) {
			if (explo < 3) {
				explo = 3;
			}
			if (exphi < explo + 1) {
				exphi = explo + 1;
			}
		}

		DESC("%d,%d", hiwat, lowat);

		AUDIO_INITINFO(&ai);
		ai.hiwat = hiwat;
		ai.lowat = lowat;
		ai.blocksize = 0;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
		XP_SYS_EQ(0, r);

		// XXX どうしてこうなるのか理解出来ない
		if (netbsd <= 8 && x68k) {
			if (exphi > 340)
				exphi = 340;
			if (explo >= exphi)
				explo = exphi - 1;
		}

		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);
		XP_EQ(exphi, ai.hiwat);
		XP_EQ(explo, ai.lowat);
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// SETINFO で設定した params によって hiwat, lowat がどうなるか。
void
test_AUDIO_SETINFO_hiwat2()
{
	struct audio_info ai;
	int fd;
	int r;
	char name[32];
	char val[256];
	size_t len;
	int blk_ms;
	struct {
		int encoding;
		int precision;
		int channels;
		int sample_rate;
	} table[] = {
#define AE_ULAW		AUDIO_ENCODING_ULAW
#define AE_LINEAR	AUDIO_ENCODING_SLINEAR_LE
		// enc	prec	ch	rate
		{ AE_ULAW,	  8, 1, 8000 },
		{ AE_LINEAR, 16, 1, 16000 },
		{ AE_LINEAR, 16, 2, 44100 },
		{ AE_LINEAR, 16, 2, 48000 },
		{ AE_LINEAR, 16, 4, 48000 },
	};

	// blk_ms を取得
	// N7,N8 の実際のブロックサイズは 50msec だが、ここでは N7,N8 は
	// こちらから指定するブロックサイズという意味で使うので 40 でいい。
	blk_ms = 40;
	if (netbsd == 9) {
		snprintf(name, sizeof(name), "hw.%s.buildinfo", hwconfigname());
		len = sizeof(val);
		r = SYSCTLBYNAME(name, val, &len, NULL, 0);
		if (r == -1)
		err(1, "sysctlbyname: %s", name);
		char *p = strstr(val, "AUDIO_BLK_MS=");
		if (p) {
			blk_ms = atoi(p + strlen("AUDIO_BLK_MS="));
		}
	}
	DPRINTF("  > blk_ms=%d\n", blk_ms);

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	TEST("AUDIO_SETINFO_hiwat2");
	for (int i = 0; i < __arraycount(table); i++) {
		int enc = table[i].encoding;
		int prec = table[i].precision;
		int ch = table[i].channels;
		int freq = table[i].sample_rate;
		int expblk;
		int expbuf;
		int exphi;
		int explo;

		DESC("%s,%d,%d,%d", encoding_names[enc], prec, ch, freq);

		// AUDIO2 では、最低3ブロック、それが 64KB 以下なら 64KB に近い
		// 整数倍。
		expblk = freq * ch * prec / NBBY * blk_ms / 1000;
		if (expblk < 65536 / 3) {
			expbuf = (65536 / expblk) * expblk;
		} else {
			expbuf = expblk * 3;
		}

		AUDIO_INITINFO(&ai);
		ai.play.encoding = enc;
		ai.play.precision = prec;
		ai.play.channels = ch;
		ai.play.sample_rate = freq;
		if (netbsd <= 8) {
			// ブロックサイズはこちらから指示する。
			// N7 での blocksize=0 の挙動は理解不能。
			ai.blocksize = expblk;
		} else {
			// A2 では blocksize の sticky は実装しないので、
			// params が変わると自動的に blocksize も変わる。
		}
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
		if (r == -1) {
			XP_SKIP("cannot set encoding on this environment.");
			continue;
		} else {
			XP_SYS_EQ(0, r);
		}

		r = IOCTL(fd, AUDIO_GETBUFINFO, &ai, "");
		XP_SYS_EQ(0, r);
		if (netbsd <= 8) {
			// N7/N8 ではバッファサイズは 65536 固定
			expbuf = 65536;
			// N7/N8 ではブロックサイズは MD によって制約が異なる orz
			if (strcmp(hwconfig, "hdafg0") == 0) {
				// hdafg は 2^n に繰上げ
				int a = expblk;
				for (expblk = 1; expblk < a; expblk *= 2)
					;
			} else if (strcmp(hwconfig, "eap0") == 0 ||
			           strcmp(hwconfig, "auich0") == 0) {
				// eap, auich は下位5ビットを落とす
				expblk &= -32;
			}
		}
		// hiwat, lowat はそのブロックサイズから計算
		exphi = expbuf / expblk;
		explo = expbuf * 3 / 4 / expblk;
		DPRINTF("  > expbuf=%d expblk=%d exphi=%d explo=%d\n",
			expbuf, expblk, exphi, explo);
		XP_EQ(expbuf, ai.play.buffer_size);
		if (netbsd == 8) {
			// 期待値未検証。
		} else {
			XP_EQ(expblk, ai.blocksize);
			XP_EQ(exphi, ai.hiwat);
			XP_EQ(explo, ai.lowat);
		}
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// gain が取得・設定できて外部ミキサーと連動するか。PR kern/52781
void
test_AUDIO_SETINFO_gain1()
{
	struct audio_info ai;
	char buf[32];
	int master;
	int master_backup;
	int gain;
	int fd;
	int r;

	TEST("AUDIO_SETINFO_gain1");

	// 適当に outputs.master を取得
	// XXX ioctl に分解できればしたほうがいいだろうけど
	POPEN_GETS(buf, sizeof(buf), "mixerctl -d %s -n outputs.master",
		devmixer);
	if (buf[0] < '0' || buf[0] > '9')
		err(1, "mixerctl");
	master = atoi(buf);
	DPRINTF("  > outputs.master = %d\n", master);
	master_backup = master;

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	// gain を取得、一致するか
	r = IOCTL(fd, AUDIO_GETINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(master, ai.play.gain);

	// 適当に変更
	AUDIO_INITINFO(&ai);
	if (master == 0) {
		gain = 255;
	} else if (master < 128) {
		gain = master * 2;
	} else {
		gain = master / 2;
	}
	if (strcmp(hwconfig, "auich0") == 0) {
		// よく分からんけど 254、255 あたりの挙動が変なので、避ける
		if (gain > 247)
			gain = 247;
	}
	ai.play.gain = gain;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "play.gain=%d", ai.play.gain);
	XP_SYS_EQ(0, r);

	// 入力値がそのまま設定できるとは限らないので、
	// 期待値はデバイスごとに補正しないといけない。
	if (strcmp(hwconfig, "hdafg0") == 0) {
		// (うちの) hdafg0 は32段階のようだ
		gain = gain / 8 * 8;
	}
	if (strcmp(hwconfig, "auich0") == 0) {
		gain = ((gain / 8) + 1) * 8 - 1;
	}

	// 変更できたか
	r = IOCTL(fd, AUDIO_GETINFO, &ai, "");
	XP_SYS_EQ(0, r);
	XP_EQ(gain, ai.play.gain);

	// outputs.master も連動しているか
	// XXX ioctl に分解できればしたほうがいいだろうけど
	POPEN_GETS(buf, sizeof(buf), "mixerctl -d %s -n outputs.master",
		devmixer);
	if (buf[0] < '0' || buf[0] > '9')
		err(1, "popen");
	master = atoi(buf);
	XP_EQ(gain, master);

	// 戻す
	AUDIO_INITINFO(&ai);
	ai.play.gain = master_backup;
	r = IOCTL(fd, AUDIO_SETINFO, &ai, "play.gain=%d", ai.play.gain);
	XP_SYS_EQ(0, r);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// ロールバックのテスト
void
test_AUDIO_SETINFO_rollback()
{
	struct audio_info ai;
	struct audio_info ai0;
	int fd;
	int r;
	char *buf;
	int buflen;
	bool avail_in_ports = true;
	bool avail_out_ports = true;
	int port;
	int val;

	// mode (HWFull / HWHalf)
	// pparam, rparam
	// port, gain, balance, monitor_gain
	// ppause, rpause
	// hiwat, lowat
	const char *table[] = {
		"pparam",
		"rparam",
		"pport",
		"rport",
	};

	TEST("AUDIO_SETINFO_rollback");
	if (netbsd <= 8) {
		XP_SKIP("netbsd-7,8 not support rollback.");
		return;
	}

	buflen  = 65536;
	buf = (char *)malloc(buflen);
	if (buf == NULL)
		err(1, "malloc");
	memset(buf, 0, buflen);

	for (int i = 0; i < __arraycount(table); i++) {
		DESC("%s", table[i]);

		// pport, rport のテストは切り替え選択肢がなければ実施しない
		if (strcmp(table[i], "pport") == 0 && avail_out_ports == false) {
			XP_SKIP("play.avail_ports not selectable");
			continue;
		}
		if (strcmp(table[i], "rport") == 0 && avail_in_ports == false) {
			XP_SKIP("record.avail_ports not selectable");
			continue;
		}

		// オープン
		fd = OPEN(devaudio, O_RDWR | O_NONBLOCK);
		if (fd == -1)
			err(1, "open");

		// まず pause を設定する
		AUDIO_INITINFO(&ai);
		ai.play.pause = 1;
		ai.record.pause = 1;
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "pause");
		XP_SYS_EQ(0, r);

		// この時の状態を取得(ミキサーも含む)
		r = IOCTL(fd, AUDIO_GETINFO, &ai0, "");
		XP_SYS_EQ(0, r);

		// バッファを適当に埋めておく。全部埋まらなくてもいい
		r = WRITE(fd, buf, buflen);
		if (r == -1) {
			XP_SYS_NG(EAGAIN, r);
		} else {
			XP_SYS_OK(r);
		}

		AUDIO_INITINFO(&ai);

		// mode
		// PLAY_ALL を下げた不正な値にしておく。ただしエラーにはならないので
		// 他のパラメータエラーでこれが反映されないことのテストだけ行う。
		ai.mode = ~AUMODE_PLAY_ALL;

		// pparam
		if (strcmp(table[i], "pparam") == 0) {
			// pparam でエラーが起きるケース
			ai.play.encoding = AUDIO_ENCODING_AC3;
		} else {
			ai.play.encoding = AUDIO_ENCODING_SLINEAR_LE;
			ai.play.precision = 16;
			ai.play.sample_rate = 16000;
			ai.play.channels = 1;
		}
		// rparam
		if (strcmp(table[i], "rparam") == 0) {
			// rparam でエラーが起きるケース
			ai.record.encoding = AUDIO_ENCODING_AC3;
		} else {
			ai.record.encoding = AUDIO_ENCODING_SLINEAR_LE;
			ai.record.precision = 16;
			ai.record.sample_rate = 16000;
			ai.record.channels = 1;
		}

		// port
		port = ai0.play.avail_ports & ~ai0.play.port;
		if (port == 0) {
			// 出力 port がないか切り替え不能
			avail_out_ports = false;
		} else if (strcmp(table[i], "pport") == 0) {
			// pport でエラーが起きるケース
			ai.play.port = 255;
		} else {
			// 現在の port 以外の選択肢のうち MostRightBit を採用
			ai.play.port = port & -port;
		}
		port = ai0.record.avail_ports & ~ai0.record.port;
		if (port == 0) {
			// 入力 port がないか切り替え不能
			avail_in_ports = false;
		} else if (strcmp(table[i], "rport") == 0) {
			// rport でエラーが起きるケース
			ai.record.port = 255;
		} else {
			// 現在の port 以外の選択肢のうち MostRightBit を採用
			ai.record.port = port & -port;
		}

		// pgain
		// エラーを起こす方法がちょっと分からない
		val = ai0.play.gain;
		val = (val > 127) ? val / 2 : val * 2;
		if (strcmp(hwconfig, "hdafg0") == 0) {
			// (うちの) hdafg は32段階のようだ
			val = val / 8 * 8;
		}
		ai.play.gain = val;

		// rgain
		// エラーを起こす方法がちょっと分からない
		val = ai0.record.gain;
		val = (val > 127) ? val / 2 : val * 2;
		if (strcmp(hwconfig, "hdafg0") == 0) {
			// (うちの) hdafg は32段階のようだ
			val = val / 8 * 8;
		}
		ai.record.gain = val;

		// pbalance
		// エラーを起こす方法がちょっと分からない
		val = ai0.play.balance;
		val = (val > 31) ? val / 2 : val * 2;
		ai.play.balance = val;

		// rbalance
		// エラーを起こす方法がちょっと分からない
		val = ai0.record.balance;
		val = (val > 31) ? val / 2 : val * 2;
		ai.record.balance = val;

		// pause はエラーに出来ないので、
		// pause 解除が効いてしまわないことの確認だけする。
		ai.play.pause = 0;
		ai.record.pause = 0;

		// hiwat, lowat は不正値を勝手に丸めるのでエラーは起きない
		ai.hiwat = ai0.hiwat - 1;
		ai.lowat = ai0.lowat + 1;

		// 実行
		r = IOCTL(fd, AUDIO_SETINFO, &ai, "");
		XP_SYS_NG(EINVAL, r);

		// 確認
		r = IOCTL(fd, AUDIO_GETINFO, &ai, "");
		XP_SYS_EQ(0, r);

		// 最初と変わっていないこと
		// ただし balance は端数とかの関係でたぶん前後 1 ずれるのはやむなし?
		XP_EQ(ai0.mode, ai.mode);
		XP_EQ(ai0.play.encoding, ai.play.encoding);
		XP_EQ(ai0.play.precision, ai.play.precision);
		XP_EQ(ai0.play.sample_rate, ai.play.sample_rate);
		XP_EQ(ai0.play.channels, ai.play.channels);
		XP_EQ(ai0.play.pause, ai.play.pause);
		XP_EQ(ai0.play.port, ai.play.port);
		XP_EQ(ai0.play.gain, ai.play.gain);
		if (ai.play.balance < ai0.play.balance - 1 ||
		    ai.play.balance > ai0.play.balance + 1)
			XP_FAIL("ai.play.balance expects (%d..%d) but %d",
				ai0.play.balance - 1, ai0.play.balance + 1, ai.play.balance);
		XP_EQ(ai0.record.encoding, ai.record.encoding);
		XP_EQ(ai0.record.precision, ai.record.precision);
		XP_EQ(ai0.record.sample_rate, ai.record.sample_rate);
		XP_EQ(ai0.record.channels, ai.record.channels);
		XP_EQ(ai0.record.pause, ai.record.pause);
		XP_EQ(ai0.record.port, ai.record.port);
		XP_EQ(ai0.record.gain, ai.record.gain);
		if (ai.record.balance < ai0.record.balance - 1 ||
		    ai.record.balance > ai0.record.balance + 1)
			XP_FAIL("ai.record.balance expects (%d..%d) but %d",
				ai0.record.balance-1, ai0.record.balance+1, ai.record.balance);
		XP_EQ(ai0.hiwat, ai.hiwat);
		XP_EQ(ai0.lowat, ai.lowat);
		// このテストでは params, port も必ず変更しようとするため、常に
		// バッファはクリアされる。
		// params と port をいじらなければ設定中にエラーが起きてもバッファは
		// クリアされないケースもあるが、それはここではテストしない。
		// 個別のパラメータテスト側でやるかなあ。
		XP_EQ(0, ai.play.seek);

		// 再生データは不要なのでクリア
		r = IOCTL(fd, AUDIO_FLUSH, NULL, "");
		XP_SYS_EQ(0, r);

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
	}

	free(buf);
}

void
test_AUDIO_GETENC_1()
{
	char buf[32];
	struct audio_info ai;
	audio_encoding_t e0, *e = &e0;
	int fd;
	int r;
	int idx;
	bool fulldup;
	int enccount = AUDIO_ENCODING_AC3 + 1;
	int preccount = 5;
	int result[enccount][preccount];

	fd = OPEN(devaudioctl, O_RDONLY);
	if (fd == -1)
		err(1, "open");

	// result の1列目は encoding、2列目は precision(4/8/16/24/32)。
	// 1 は GETENC でサポートしているもの
	// 2 は GETENC には現れないが互換性でサポートしているもの
	memset(&result, 0, sizeof(result));

	TEST("AUDIO_GETENC_1");
	for (idx = 0; ; idx++) {
		DESC("GETENC[%d]", idx);

		memset(e, 0, sizeof(*e));
		e->index = idx;
		r = IOCTL(fd, AUDIO_GETENC, e, "index=%d", idx);
		if (r != 0) {
			XP_SYS_NG(EINVAL, r);
			break;
		}
		// 成功なら戻り値についてざっと調べる
		XP_EQ(idx, e->index);
		if (0 <= e->encoding && e->encoding <= AUDIO_ENCODING_AC3) {
			XP_EQ(encoding_names[e->encoding], e->name);
		} else {
			XP_FAIL("e->encoding %d", e->encoding);
		}
		switch (e->precision) {
		 case 4:
		 case 8:
		 case 16:
		 case 24:
		 case 32:
			break;
		 default:
			XP_FAIL("e->precision %d", e->precision);
			break;
		}
		XP_EQ(0, (e->flags & ~AUDIO_ENCODINGFLAG_EMULATED));

		result[e->encoding][e->precision/ 8] = 1;

		// compatibility
		switch (e->precision) {
		 case 8:
			if (e->encoding == AUDIO_ENCODING_SLINEAR) {
				result[AUDIO_ENCODING_SLINEAR_LE][1] = 2;
				result[AUDIO_ENCODING_SLINEAR_BE][1] = 2;
			}
			if (e->encoding == AUDIO_ENCODING_ULINEAR) {
				result[AUDIO_ENCODING_ULINEAR_LE][1] = 2;
				result[AUDIO_ENCODING_ULINEAR_BE][1] = 2;
				result[AUDIO_ENCODING_PCM8][1] = 2;		// うーんこの…
				result[AUDIO_ENCODING_PCM16][1] = 2;	// うーんこの…
			}
			break;
		 case 16:
			if (e->encoding == AUDIO_ENCODING_SLINEAR_NE) {
				result[AUDIO_ENCODING_SLINEAR][2] = 2;
				result[AUDIO_ENCODING_PCM16][2] = 2;	// うーんこの…
			}
			if (e->encoding == AUDIO_ENCODING_ULINEAR_NE) {
				result[AUDIO_ENCODING_ULINEAR][2] = 2;
			}
			break;
#if defined(AUDIO_SUPPORT_LINEAR24)
		 case 24:
			if (e->encoding == AUDIO_ENCODING_SLINEAR_NE) {
				result[AUDIO_ENCODING_SLINEAR][3] = 2;
				result[AUDIO_ENCODING_PCM16][3] = 2;	// うーんこの…
			}
			if (e->encoding == AUDIO_ENCODING_ULINEAR_NE) {
				result[AUDIO_ENCODING_ULINEAR][3] = 2;
			}
			break;
#endif
		 case 32:
			if (e->encoding == AUDIO_ENCODING_SLINEAR_NE) {
				result[AUDIO_ENCODING_SLINEAR][4] = 2;
				result[AUDIO_ENCODING_PCM16][4] = 2;	// うーんこの…
			}
			if (e->encoding == AUDIO_ENCODING_ULINEAR_NE) {
				result[AUDIO_ENCODING_ULINEAR][4] = 2;
			}
			break;
		 default:
			err(1, "e->precision=%d", e->precision);
		}
	}

	// エラーが出た次のインデックスもエラーになるはず
	DESC("GETENC[next]");
	e->index = idx + 1;
	r = IOCTL(fd, AUDIO_GETENC, e, "index=%d", e->index);
	XP_SYS_NG(EINVAL, r);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);

	// デバッグ表示
	if (debug) {
		for (int i = 0; i < enccount; i++) {
			printf("result[%2d] %15s", i, encoding_names[i]);
			for (int j = 0; j < preccount; j++) {
				printf(" %d", result[i][j]);
			}
			printf("\n");
		}
	}

	//
	// そのエンコーディングが実際にセットできるか
	//

	fulldup = (props & AUDIO_PROP_FULLDUPLEX);
	if (fulldup) {
		fd = OPEN(devaudio, O_RDWR);
		if (fd == -1)
			err(1, "open");
	}

	for (int i = 0; i < enccount; i++) {
		for (int j = 0; j < preccount; j++) {
			int prec = (j == 0) ? 4 : j * 8;
			snprintf(buf, sizeof(buf), "%s:%d", encoding_names[i], prec);
			DESC("SET,%s", buf);

			AUDIO_INITINFO(&ai);
			ai.play.encoding = i;
			ai.play.precision = prec;
			ai.play.channels = 1;		/* 知る方法がない */
			ai.play.sample_rate = 8000;	/* 知る方法がない */
			ai.record = ai.play;

			if (fulldup) {
				// Full Dup なら一度に R/W 両方テスト
				ai.mode = AUMODE_PLAY_ALL | AUMODE_RECORD;

				r = IOCTL(fd, AUDIO_SETINFO, &ai, buf);
				if (result[i][j]) {
					// 成功するはず
					XP_SYS_EQ(0, r);
				} else {
					// 失敗するはず
					if (netbsd == 8) {
						// もうよく分からん
						if (prec == 32 && i == AUDIO_ENCODING_PCM16) {
							XP_EXPFAIL("r expects -1,EINVAL but %d", r);
						}

					} else
					if ((prec == 8 && i == AUDIO_ENCODING_PCM16) ||
					    (prec == 8 && i == AUDIO_ENCODING_PCM8) ||
					    (prec == 8 && i == AUDIO_ENCODING_SLINEAR_LE) ||
					    (prec == 8 && i == AUDIO_ENCODING_SLINEAR_BE) ||
					    (prec == 8 && i == AUDIO_ENCODING_ULINEAR_LE) ||
					    (prec == 8 && i == AUDIO_ENCODING_ULINEAR_BE) ||
					    (prec == 8 && i == AUDIO_ENCODING_SLINEAR) ||
					    (prec == 8 && i == AUDIO_ENCODING_ULINEAR))
					{
						// これらは成功するけど、それでいいのかという気はする
						if (r == 0)
							XP_EXPFAIL("loose compatibility?");
					} else {
						XP_SYS_NG(EINVAL, r);
					}
				}
			} else {
				// Half Dup なら R/W いちいち別々にテスト
				fd = OPEN(devaudio, O_WRONLY);
				XP_SYS_OK(fd);

				ai.mode = AUMODE_PLAY_ALL;
				r = IOCTL(fd, AUDIO_SETINFO, &ai, buf);
				if (result[i][j]) {
					// 成功するはず
					XP_SYS_EQ(0, r);
				} else {
					// 失敗するはず
					XP_SYS_NG(EINVAL, r);
				}

				r = CLOSE(fd);
				XP_SYS_EQ(0, r);

				fd = OPEN(devaudio, O_RDONLY);
				XP_SYS_OK(fd);

				ai.mode = AUMODE_RECORD;
				r = IOCTL(fd, AUDIO_SETINFO, &ai, buf);
				if (result[i][j]) {
					// 成功するはず
					XP_SYS_EQ(0, r);
				} else {
					// 失敗するはず
					XP_SYS_NG(EINVAL, r);
				}

				r = CLOSE(fd);
				XP_SYS_EQ(0, r);
			}
		}
	}

	if (fulldup) {
		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
	}
}

// 引数が正しくないケース
void
test_AUDIO_GETENC_2()
{
	audio_encoding_t enc;
	int fd;
	int r;

	TEST("AUDIO_GETENC_2");

	fd = OPEN(devaudio, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	memset(&enc, 0, sizeof(enc));
	enc.index = -1;
	r = IOCTL(fd, AUDIO_GETENC, &enc, "index=-1");
	if (netbsd == 7 && r == 0) {
		// N7 では(HW driver によって?)失敗しない場合があるようだ?
		XP_EXPFAIL("r expects -1,EINVAL but %d", r);
	} else {
		XP_SYS_NG(EINVAL, r);
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// audio デバイスオープン中に audioctl がオープンできること
void
test_audioctl_open_1()
{
	int fd;
	int ctl;
	int r;
	int fmode;
	int cmode;

	TEST("audioctl_open_1");
	for (fmode = 0; fmode <= 2; fmode++) {
		if (fmode == O_WRONLY && (props & AUDIO_PROP_PLAYBACK) == 0)
			continue;
		if (fmode == O_RDONLY && (props & AUDIO_PROP_CAPTURE) == 0)
			continue;

		for (cmode = 0; cmode <= 2; cmode++) {
			DESC("%s,%s", openmodetable[fmode], openmodetable[cmode]);

			fd = OPEN(devaudio, fmode);
			if (fd == -1)
				err(1, "open");

			ctl = OPEN(devaudioctl, cmode);
			XP_SYS_OK(ctl);

			r = CLOSE(ctl);
			XP_SYS_EQ(0, r);

			r = CLOSE(fd);
			XP_SYS_EQ(0, r);
		}
	}
}

// audioctl デバイスオープン中に audio がオープンできること
void
test_audioctl_open_2()
{
	int fd;
	int ctl;
	int r;
	int fmode;
	int cmode;

	TEST("audioctl_open_2");
	for (fmode = 0; fmode <= 2; fmode++) {
		if (fmode == O_WRONLY && (props & AUDIO_PROP_PLAYBACK) == 0)
			continue;
		if (fmode == O_RDONLY && (props & AUDIO_PROP_CAPTURE) == 0)
			continue;

		for (cmode = 0; cmode <= 2; cmode++) {
			DESC("%s,%s", openmodetable[fmode], openmodetable[cmode]);

			ctl = OPEN(devaudioctl, cmode);
			XP_SYS_OK(ctl);

			fd = OPEN(devaudio, fmode);
			XP_SYS_OK(fd);

			r = CLOSE(fd);
			XP_SYS_EQ(0, r);

			r = CLOSE(ctl);
			XP_SYS_EQ(0, r);
		}
	}
}

// audioctl の多重オープン
void
test_audioctl_open_3()
{
	int ctl0;
	int ctl1;
	int r;

	TEST("audioctl_open_3");

	ctl0 = OPEN(devaudioctl, O_RDWR);
	if (ctl0 == -1)
		err(1, "open");

	ctl1 = OPEN(devaudioctl, O_RDWR);
	XP_SYS_OK(ctl1);

	r = CLOSE(ctl0);
	XP_SYS_EQ(0, r);

	r = CLOSE(ctl1);
	XP_SYS_EQ(0, r);
}

// audio とは別ユーザでも audioctl はオープンできること
// パーミッションはもう _1 と _2 でやったのでいいか
void
test_audioctl_open_4()
{
	char name[32];
	char cmd[64];
	int fd;
	int fc;
	int r;
	int multiuser;
	uid_t ouid;

	TEST("audioctl_open_4");
	if (geteuid() != 0) {
		XP_SKIP("This test must be priviledged user");
		return;
	}

	// /dev/audio を root がオープンした後で
	// /dev/audioctl を一般ユーザがオープンする
	for (int i = 0; i <= 1; i++) {
		// N7 には multiuser の概念がない
		// AUDIO2 は未実装
		if (netbsd != 8) {
			if (i == 1)
				break;
		} else {
			multiuser = 1 - i;
			DESC("multiuser%d", multiuser);

			snprintf(name, sizeof(name), "hw.%s.multiuser", hwconfigname());
			snprintf(cmd, sizeof(cmd),
				"sysctl -w %s=%d > /dev/null", name, multiuser);
			r = SYSTEM(cmd);
			if (r == -1)
				err(1, "system: %s", cmd);
			if (r != 0)
				errx(1, "system failed: %s", cmd);

			// 確認
			int newval = 0;
			size_t len = sizeof(newval);
			r = SYSCTLBYNAME(name, &newval, &len, NULL, 0);
			if (r == -1)
				err(1, "multiuser");
			if (newval != multiuser)
				errx(1, "set multiuser=%d failed", multiuser);
		}

		fd = OPEN(devaudio, O_RDWR);
		if (fd == -1)
			err(1, "open");

		ouid = GETUID();
		r = SETEUID(1);
		if (r == -1)
			err(1, "setuid");

		fc = OPEN(devaudioctl, O_RDWR);
		if (netbsd == 8) {
			// N8 は audioctl のオープン条件を間違えている
			XP_EXPFAIL("fc expects success but %d,err#%d", r, errno);
		} else {
			XP_SYS_OK(fc);
		}
		if (fc != -1) {
			r = CLOSE(fc);
			XP_SYS_EQ(0, r);
		}

		r = SETEUID(ouid);
		if (r == -1)
			err(1, "setuid");

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
	}
}

// audioctl とは別ユーザでも audio はオープンできること
void
test_audioctl_open_5()
{
	char name[32];
	char cmd[64];
	int fd;
	int fc;
	int r;
	int multiuser;
	uid_t ouid;

	TEST("audioctl_open_5");
	if (geteuid() != 0) {
		XP_SKIP("This test must be priviledged user");
		return;
	}

	// /dev/audioctl を root がオープンした後で
	// /dev/audio を一般ユーザがオープンする
	for (int i = 0; i <= 1; i++) {
		// N7 には multiuser の概念がない
		// AUDIO2 は未実装
		if (netbsd != 8) {
			if (i == 1)
				break;
		} else {
			multiuser = 1 - i;
			DESC("multiuser%d", multiuser);

			snprintf(name, sizeof(name), "hw.%s.multiuser", hwconfigname());
			snprintf(cmd, sizeof(cmd),
				"sysctl -w %s=%d > /dev/null", name, multiuser);
			r = SYSTEM(cmd);
			if (r == -1)
				err(1, "system: %s", cmd);
			if (r != 0)
				errx(1, "system failed: %s", cmd);

			// 確認
			int newval = 0;
			size_t len = sizeof(newval);
			r = SYSCTLBYNAME(name, &newval, &len, NULL, 0);
			if (r == -1)
				err(1, "multiuser");
			if (newval != multiuser)
				errx(1, "set multiuser=%d failed", multiuser);
		}

		fc = OPEN(devaudioctl, O_RDWR);
		if (fc == -1)
			err(1, "open");

		ouid = GETUID();
		r = SETEUID(1);
		if (r == -1)
			err(1, "setuid");

		fd = OPEN(devaudio, O_RDWR);
		if (netbsd == 8) {
			// N8 は audioctl のオープン条件を間違えている
			XP_EXPFAIL("fd expects success but %d,#%d", r, errno);
		} else {
			XP_SYS_OK(fd);
		}
		if (fd != -1) {
			r = CLOSE(fd);
			XP_SYS_EQ(0, r);
		}

		r = SETEUID(ouid);
		if (r == -1)
			err(1, "setuid");

		r = CLOSE(fc);
		XP_SYS_EQ(0, r);
	}
}

// /dev/audioctl はどのモードでオープンしても read/write できないこと。
void
test_audioctl_rw(void)
{
	char buf[1];
	int fd;
	int r;

	TEST("audioctl_rw");
	for (int mode = 0; mode <= 2; mode++) {
		DESC("%s", openmodetable[mode]);
		fd = OPEN(devaudioctl, mode);
		if (fd == -1)
			err(1, "open");

		if (mode2popen_full[mode]) {
			r = WRITE(fd, buf, sizeof(buf));
			XP_SYS_NG(ENODEV, r);
		}

		if (mode2ropen_full[mode]) {
			r = READ(fd, buf, sizeof(buf));
			XP_SYS_NG(ENODEV, r);
		}

		r = CLOSE(fd);
		XP_SYS_EQ(0, r);
	}
}

// /dev/audioctl への poll は常に成功扱いのようだ
void
test_audioctl_poll()
{
	struct pollfd pfd;
	int fd;
	int r;

	TEST("audioctl_poll");
	fd = OPEN(devaudioctl, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	pfd.fd = fd;
	pfd.events = POLLOUT;
	r = POLL(&pfd, 1, 1);
	XP_SYS_EQ(0, r);
	XP_EQ(0, pfd.revents);

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// /dev/audioctl は kqueue できないこと (mode別はさすがにいいだろう)
void
test_audioctl_kqueue()
{
	struct kevent kev;
	int fd;
	int kq;
	int r;

	TEST("audioctl_kqueue");
	fd = OPEN(devaudioctl, O_WRONLY);
	if (fd == -1)
		err(1, "open");

	kq = KQUEUE();
	XP_SYS_OK(kq);

	EV_SET(&kev, fd, EVFILT_WRITE, EV_ADD, 0, 0, 0);
	r = KEVENT_SET(kq, &kev, 1);
	// N7: たぶん poll と同じ常時成功扱いにしようとしたのだと思うが、
	// バグがあって 1 (== EPERM) が返ってくる。
	// A2 では ENODEV を返すことにした。
	if (netbsd == 7) {
		XP_SYS_NG(1/*EPERM*/, r);
	} else {
		XP_SYS_NG(ENODEV, r);
	}

	r = CLOSE(fd);
	XP_SYS_EQ(0, r);
}

// テスト一覧
#define DEF(x)	{ #x, test_ ## x }
struct testtable testtable[] = {
	DEF(open_1),
	DEF(open_2),
	DEF(open_3),
	DEF(open_4),
	DEF(open_5),
	DEF(open_6),
	DEF(encoding_1),
	DEF(encoding_2),
	DEF(drain_1),
	DEF(drain_2),
	DEF(playsync_1),
	DEF(readwrite_1),
	DEF(readwrite_2),
	DEF(readwrite_3),
	DEF(mmap_1),
	DEF(mmap_2),
	DEF(mmap_3),
	DEF(mmap_4),
	DEF(mmap_5),
	DEF(mmap_6),
	DEF(mmap_7),
	DEF(mmap_8),
	DEF(mmap_9),
	DEF(poll_1),
	DEF(poll_2),
	DEF(poll_3),
	DEF(poll_4),
	DEF(poll_5),
	DEF(poll_6),
	DEF(poll_writeIN_1),
	DEF(kqueue_2),
	DEF(kqueue_3),
	DEF(kqueue_4),
	DEF(kqueue_5),
	DEF(kqueue_6),
	DEF(FIOASYNC_1),
	DEF(FIOASYNC_2),
	DEF(FIOASYNC_3),
	DEF(FIOASYNC_4),
	DEF(FIOASYNC_5),
	DEF(AUDIO_WSEEK_1),
	DEF(AUDIO_SETFD_ONLY),
	DEF(AUDIO_SETFD_RDWR),
	DEF(AUDIO_GETINFO_seek),
	DEF(AUDIO_GETINFO_eof),
	DEF(AUDIO_SETINFO_mode),
	DEF(AUDIO_SETINFO_params),
	DEF(AUDIO_SETINFO_params2),
	DEF(AUDIO_SETINFO_params3),
	DEF(AUDIO_SETINFO_pause),
	DEF(AUDIO_SETINFO_blocksize),
	DEF(AUDIO_SETINFO_hiwat1),
	DEF(AUDIO_SETINFO_hiwat2),
	DEF(AUDIO_SETINFO_gain1),
	DEF(AUDIO_SETINFO_rollback),
	DEF(AUDIO_GETENC_1),
	DEF(AUDIO_GETENC_2),
	DEF(audioctl_open_1),
	DEF(audioctl_open_2),
	DEF(audioctl_open_3),
	DEF(audioctl_open_4),
	DEF(audioctl_open_5),
	DEF(audioctl_rw),
	DEF(audioctl_poll),
	DEF(audioctl_kqueue),
	{ NULL, NULL },
};

