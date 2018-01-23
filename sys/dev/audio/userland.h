#pragma once

#include <string.h>

// アサートするとき定義
#define AUDIO_ASSERT

// ユーザランドはグローバル変数 audio_blk_ms を使う。
// デフォルト 40msec で -m オプションで変更可能。
#define AUDIO_BLK_MS audio_blk_ms

#define DPRINTF(n, fmt, ...)	printf(fmt, ## __VA_ARGS__)

#ifdef AUDIO_ASSERT
#define KASSERT(expr)	do {\
	if (!(expr)) panic(#expr);\
} while (0)
#define KASSERTMSG(expr, fmt, ...)	do {\
	if (!(expr)) panic(fmt, ## __VA_ARGS__);\
} while (0)
#else
#define KASSERT(expr)	/**/
#define KASSERTMSG(expr, fmt, ...)	/**/
#endif

/*
 * sys/audioio.h にある他の定数は compat.h で定義する。
 * NetBSD 側プログラムは定数がシステムと一致してないといけないため。
 */
#define AUDIO_ENCODING_RAWBYTE		32767	/* これは AUDIO2 独自 */

/* サポートする最大のチャンネル数 */
#define AUDIO_MAX_CHANNELS	18

#define IO_NDELAY	0

typedef struct kcondvar kcondvar_t;
typedef struct kmutex kmutex_t;

struct kcondvar {
	volatile int v;
};

struct kmutex {
	volatile int v;
};

struct audio_params {
	int sample_rate;
	int channels;
	int encoding;
	int validbits;
	int precision;
};
typedef struct audio_params audio_params_t;

struct audio_hw_if {
	void *(*allocm)(void *, int, size_t);
	void (*freem)(void *, void *, size_t);

	int (*start_output)(void *, void *, int, void(*)(void *), void *);
	int (*trigger_output)(void *, void *, void *, int, void(*)(void *), void *, const audio_params_t *);
	int (*halt_output)(void *);

	int (*start_input)(void *, void *, int, void(*)(void *), void *);
	int (*trigger_input)(void *, void *, void *, int, void(*)(void *), void *, const audio_params_t *);
	int (*halt_input)(void *);

	audio_filter_t (*get_swcode)(void *, int, audio_filter_arg_t *);
	int (*round_blocksize)(void *, int, int, const audio_params_t *);
	size_t (*round_buffersize)(void *, int, size_t);
};

// audiovar.h の前方参照
typedef struct audio_trackmixer audio_trackmixer_t;
typedef struct audio_track audio_track_t;
typedef struct audio_file audio_file_t;

struct audio_softc
{
	SLIST_HEAD(files_head, audio_file) sc_files;		/* 開いているファイルのリスト */
	audio_trackmixer_t  *sc_pmixer;		/* 接続されている再生ミキサ */
	audio_trackmixer_t  *sc_rmixer;		/* 接続されている録音ミキサ */
	kmutex_t *sc_lock;
	kmutex_t *sc_intr_lock;
	struct audio_hw_if *hw_if;
	void *hw_hdl;
	int sc_eof;

	bool sc_pbusy;
	bool sc_rbusy;
	void *dev;
	audio_filter_reg_t sc_xxx_pfilreg;
	audio_filter_reg_t sc_xxx_rfilreg;
	audio_format2_t sc_pparams;
	audio_format2_t sc_rparams;
	audio_format2_t sc_phwfmt;
	audio_format2_t sc_rhwfmt;
	void *sc_sih_rd;
	void *sc_sih_wr;
	bool sc_dying;

	void *phys; // 実物理デバイス

	kmutex_t sc_lock0;
	kmutex_t sc_intr_lock0;
	struct audio_hw_if hw_if0;
};

void audio_softc_init(struct audio_softc *sc);

static inline void
cv_init(kcondvar_t *cv, const char *msg)
{
	cv->v = 0;
}

static inline void
cv_destroy(kcondvar_t *cv)
{
	// nop
}

static inline void
cv_broadcast(kcondvar_t *cv)
{
	cv->v = 1;
}

int cv_wait_sig(kcondvar_t *cv, kmutex_t *lock);

static inline int
mutex_owned(kmutex_t *mutex)
{
	return (mutex->v != 0);
}

static inline void
mutex_enter(kmutex_t *mutex)
{
	mutex->v = 1;
}

static inline void
mutex_exit(kmutex_t *mutex)
{
	mutex->v = 0;
}

static inline int
mutex_tryenter(kmutex_t *mutex)
{
	if (mutex_owned(mutex)) {
		return 0;
	} else {
		mutex_enter(mutex);
		return 1;
	}
}

#define M_NOWAIT	(0)
static inline void *
kern_malloc(size_t size, int flags)
{
	return malloc(size);
}

static inline void *
kern_realloc(void *ptr, size_t size, int flags)
{
	return realloc(ptr, size);
}

static inline void
kern_free(void *ptr)
{
	free(ptr);
}

#define KM_SLEEP	(0)

static inline void *
kmem_zalloc(size_t size, int flags)
{
	void *p;
	p = kern_malloc(size, 0);
	if (p)
		memset(p, 0, size);
	return p;
}

static inline void
kmem_free(void *p, size_t size)
{
	kern_free(p);
}

void aprint_error_dev(void *, const char *fmt, ...);

void aprint_normal_dev(void *, const char *fmt, ...);

static inline uint32_t
atomic_cas_32(volatile uint32_t *ptr, uint32_t expected, uint32_t newvalue)
{
	uint32_t rv = *ptr;
	if (rv == expected) {
		*ptr = newvalue;
	}
	return rv;
}


#define SOFTINT_SERIAL 3

struct softintr_XXX
{
	void (*func)(void *);
	void *arg;
};

static inline void *
softint_establish(int level, void(*fun)(void *), void *arg)
{
	struct softintr_XXX *rv = malloc(sizeof(struct softintr_XXX));
	rv->func = fun;
	rv->arg = arg;
	return rv;
}

static inline void
softint_disestablish(void *cookie)
{
	free(cookie);
}

static inline void
softint_schedule(void *cookie)
{
	struct softintr_XXX *intr = cookie;
	intr->func(intr->arg);
}

static inline void
kpreempt_enable(void)
{
}

static inline void
kpreempt_disable(void)
{
}

extern bool audio_track_is_playback(const audio_track_t *track);
extern bool audio_track_is_record(const audio_track_t *track);
static inline int
audio_check_params2(audio_format2_t *f2)
{
	// 後方互換性からの変換は起きないはずだし
	// 入力に問題がなければ何もおきないので、とりあえずこれでいい。
	return 0;
}

/* system call emulation */
extern audio_file_t *sys_open(struct audio_softc *sc, int mode);
extern int/*ssize_t*/ sys_write(audio_file_t *file, void* buf, size_t len);	/* write syscall の Userland 側エミュレート */
extern int sys_ioctl_drain(audio_track_t *track);	/* ioctl(AUDIO_DRAIN) */

extern void audio_format2_tostr(char *buf, size_t bufsize,
	const audio_format2_t *fmt);

extern void audio_attach(struct audio_softc **softc, bool hw);
extern void audio_detach(struct audio_softc *sc);

extern void audio_softc_play_start(struct audio_softc *sc);
extern bool audio_softc_play_busy(struct audio_softc *sc);

extern void lock(struct audio_softc *sc);
extern void unlock(struct audio_softc *sc);

extern int audio_blk_ms;

// OBSOLETE
extern void WAIT();

