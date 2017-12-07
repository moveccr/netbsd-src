#if defined(_KERNEL)
#include <dev/audio/aumix.h>
#include <dev/audio/auring.h>
#include <dev/audio/aucodec.h>
#include <sys/intr.h>
#else
#include "aumix.h"
#include <errno.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include "auring.h"
#include "audev.h"
#include "aucodec.h"
#include "auformat.h"
#include "uio.h"
#include "auintr.h"
#endif // !_KERNEL

/*
 * -N7
 * audio_write {
 *  uioからバッファを作成
 *  audiostartp {
 *   start/trigger ..1ブロック出力/trigger開始
 *  }
 * }
 * audio_pint {
 *  (必要なら)(フィルタ内に留まってるデータから)バッファを作成
 *  start_output ..1ブロック出力
 * }
 *
 * ---
 *
 * audio_write {
 *  audio_trackmixer_play {
 *   audio_trackmixer_mixall {
 *    audio_track_play .. uioからoutbufまでの変換
 *    audio_mixer_play_mix_track ..合成
 *   }
 *   audio_mixer_play_period ..HW変換
 *   if (ready)
 *    audio_trackmixer_output ..1ブロック出力
 *  }
 * }
 * audio2_pintr {
 *  audio_trackmixer_intr {
 *   audio_trackmixer_output ..1ブロック出力
 *   audio_trackmixer_mixall {
 *    audio_track_play .. uioからoutbufまでの変換
 *    audio_mixer_play_mix_track ..合成
 *   }
 *   audio_mixer_play_period
 * }
 */

#if defined(_KERNEL)
#define lock(x)					/*とりあえず*/
#define unlock(x)				/*とりあえず*/
#endif

void *audio_realloc(void *memblock, size_t bytes);
void audio_free(void *memblock);
int16_t audio_volume_to_inner(uint8_t v);
uint8_t audio_volume_to_outer(int16_t v);
void audio_trackmixer_output(audio_trackmixer_t *mixer);
#if defined(AUDIO_SOFTINTR)
static void audio_trackmixer_softintr(void *arg);
#endif

#if !defined(_KERNEL)
static int audio_waitio(struct audio_softc *sc, audio_track_t *track);
#endif // !_KERNEL


void
audio_trace0(const char *funcname, const char *fmt, ...)
{
	struct timeval tv;
	va_list ap;

	getmicrotime(&tv);
	printf("%d.%06d ", (int)tv.tv_sec%60, (int)tv.tv_usec);
	printf("%s ", funcname);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\n");
}

void
audio_trace(const char *funcname, audio_track_t *track, const char *fmt, ...)
{
	struct timeval tv;
	va_list ap;

	getmicrotime(&tv);
	printf("%d.%06d ", (int)tv.tv_sec%60, (int)tv.tv_usec);
	printf("%s #%d ", funcname, track->id);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\n");
}

/* メモリアロケーションの STUB */

void *
audio_realloc(void *memblock, size_t bytes)
{
	if (memblock != NULL) {
		if (bytes != 0) {
			return kern_realloc(memblock, bytes, M_NOWAIT);
		} else {
			kern_free(memblock);
			return NULL;
		}
	} else {
		if (bytes != 0) {
			return kern_malloc(bytes, M_NOWAIT);
		} else {
			return NULL;
		}
	}
}

void
audio_free(void *memblock)
{
	if (memblock != NULL) {
		kern_free(memblock);
	}
}


/*
 * ***** audio_rational *****
 */

/* r = 0 */
static inline void
audio_rational_clear(audio_rational_t *r)
{
	KASSERT(r != NULL);
	r->i = 0;
	r->n = 0;
}

/* 共通分母 d の正規化された帯分数 r, a に対し、 r += a を実行し、結果の整数部を返します。 */
static inline int
audio_rational_add(audio_rational_t *r, audio_rational_t *a, int d)
{
	KASSERT(r != NULL);
	KASSERT(a != NULL);
	KASSERT(d != 0);
	KASSERT(r->n < d);
	KASSERT(a->n < d);

	r->i += a->i;
	r->n += a->n;
	if (r->n >= d) {
		r->i += 1;
		r->n -= d;
	}
	return r->i;
}

/* a > b なら + 、a == b なら 0 , a < b なら - を返します。*/
static inline int
audio_rational_cmp(audio_rational_t *a, audio_rational_t *b)
{
	int r = a->i - b->i;
	if (r == 0) {
		r = a->n - b->n;
	}
	return r;
}

/*
 * audio_volume
 */

 /* ユーザランドなどで使用される 0.255 ボリュームを、トラック内の 0..256 ボリュームに変換します。 */
int16_t
audio_volume_to_inner(uint8_t v)
{
	return v < 127 ? v : (int16_t)v + 1;
}

/* トラック内の 0..256 ボリュームを、外部で使用される 0..255 ボリュームに変換します。 */
uint8_t
audio_volume_to_outer(int16_t v)
{
	return v < 127 ? v : v - 1;
}


/*
 * ***** audio_track *****
 */

#if !defined(AUDIO_SOFTINTR)
// track_cl フラグをセットします。
// 割り込みコンテキストから呼び出すことはできません。
static inline void
audio_track_cl(audio_track_t *track)
{
	struct audio_softc *sc = track->mixer->sc;
	mutex_enter(sc->sc_intr_lock);
	track->track_cl = 1;
	mutex_exit(sc->sc_intr_lock);
}

// track_cl フラグをリセットします。
// 割り込みコンテキストから呼び出すことはできません。
static inline void
audio_track_uncl(audio_track_t *track)
{
	struct audio_softc *sc = track->mixer->sc;
	mutex_enter(sc->sc_intr_lock);
	track->track_cl = 0;
	mutex_exit(sc->sc_intr_lock);
}
#endif

static void
audio_track_chvol(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));
	KASSERT(arg->srcfmt->channels == arg->dstfmt->channels);
	KASSERT(arg->context != NULL);
	KASSERT(arg->srcfmt->channels <= AUDIO_MAX_CHANNELS);

	int16_t *ch_volume = arg->context;
	const internal_t *sptr = arg->src;
	internal_t *dptr = arg->dst;

	for (int i = 0; i < arg->count; i++) {
		for (int ch = 0; ch < arg->srcfmt->channels; ch++, sptr++, dptr++) {
			*dptr = (internal_t)(((internal2_t)*sptr) * ch_volume[ch] / 256);
		}
	}
}

static void
audio_track_chmix_mixLR(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));

	const internal_t *sptr = arg->src;
	internal_t *dptr = arg->dst;

	for (int i = 0; i < arg->count; i++) {
#if false
		internal2_t s;
		s = (internal2_t)sptr[0];
		s += (internal2_t)sptr[1];
		*dptr = s / 2;
#else
		*dptr = sptr[0] / 2 + sptr[1] / 2;
#endif
		dptr++;
		sptr += arg->srcfmt->channels;
	}
}

static void
audio_track_chmix_dupLR(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));

	const internal_t *sptr = arg->src;
	internal_t *dptr = arg->dst;

	for (int i = 0; i < arg->count; i++) {
		dptr[0] = sptr[0];
		dptr[1] = sptr[0];
		dptr += arg->dstfmt->channels;
		sptr++;
	}
	if (arg->dstfmt->channels > 2) {
		dptr = arg->dst;
		for (int i = 0; i < arg->count; i++) {
			for (int ch = 2; ch < arg->dstfmt->channels; ch++) {
				dptr[ch] = 0;
			}
			dptr += arg->dstfmt->channels;
		}
	}
}

static void
audio_track_chmix_shrink(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));

	const internal_t *sptr = arg->src;
	internal_t *dptr = arg->dst;

	for (int i = 0; i < arg->count; i++) {
		for (int ch = 0; ch < arg->dstfmt->channels; ch++) {
			*dptr++ = sptr[ch];
		}
		sptr += arg->srcfmt->channels;
	}
}

static void
audio_track_chmix_expand(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));

	const internal_t *sptr = arg->src;
	internal_t *dptr = arg->dst;

	for (int i = 0; i < arg->count; i++) {
		for (int ch = 0; ch < arg->srcfmt->channels; ch++) {
			*dptr++ = *sptr++;
		}
		for (int ch = arg->srcfmt->channels; ch < arg->dstfmt->channels; ch++) {
			*dptr++ = 0;
		}
	}
}

static void
audio_track_freq_up(audio_filter_arg_t *arg)
{
	audio_track_t *track = arg->context;
	audio_ring_t *src = &track->freq.srcbuf;
	audio_ring_t *dst = track->freq.dst;

	KASSERT(track);
	KASSERT(is_valid_ring(dst));
	KASSERT(is_valid_ring(src));
	KASSERT(src->count > 0);
	KASSERT(src->fmt.channels == dst->fmt.channels);
	KASSERT(src->top == 0);

	const internal_t *sptr = arg->src;
	internal_t *dptr = arg->dst;
	audio_rational_t tmp = track->freq_current;
	const internal_t *sptr1;

	for (int i = 0; i < arg->count; i++) {
		if (tmp.n == 0) {
			if (src->count <= 0) {
				break;
			}
			for (int ch = 0; ch < dst->fmt.channels; ch++) {
				*dptr++ = sptr[ch];
			}
		} else {
			sptr1 = sptr + src->fmt.channels;
			if (src->count <= 1) {
				break;
			}
			// 加算前の下駄 2^22 を脱ぐ
			int b = tmp.n * track->freq_coef / (1 << 22);
			int a = 256 - b;
			for (int ch = 0; ch < dst->fmt.channels; ch++) {
				// 加算後の下駄 2^8 を脱ぐ
				*dptr++ = (sptr[ch] * a + sptr1[ch] * b) / 256;
			}
		}
		dst->count++;
		audio_rational_add(&tmp, &track->freq_step, dst->fmt.sample_rate);
		if (tmp.i > 0) {
			// 周波数を上げるので、ソース側は 1 以下のステップが保証されている
			KASSERT(tmp.i == 1);
			sptr += src->fmt.channels;
			tmp.i = 0;
			src->top++;
			src->count--;
		}
	}

	track->freq_current = tmp;
}

static void
audio_track_freq_down(audio_filter_arg_t *arg)
{
	audio_track_t *track = arg->context;
	audio_ring_t *src = &track->freq.srcbuf;
	audio_ring_t *dst = track->freq.dst;

	KASSERT(track);
	KASSERT(is_valid_ring(dst));
	KASSERT(is_valid_ring(src));
	KASSERT(src->count > 0);
	KASSERT(src->fmt.channels == dst->fmt.channels);
	KASSERT(src->top == 0);

	const internal_t *sptr0 = arg->src;
	internal_t *dptr = arg->dst;
	audio_rational_t tmp = track->freq_current;
	const internal_t *sptr1;
	int src_taken = -1;

	for (int i = 0; i < arg->count; i++) {
//#define AUDIO_FREQ_HQ
#if defined(AUDIO_FREQ_HQ)
		if (tmp.n == 0) {
			if (tmp.i >= src->count) {
				break;
			}
			sptr1 = sptr0 + tmp.i * src->fmt.channels;
			for (int ch = 0; ch < dst->fmt.channels; ch++) {
				*dptr++ = sptr1[ch];
			}
		} else {
			const internal_t *sptr2;
			if (tmp.i + 1 >= src->count) {
				break;
			}
			sptr1 = sptr0 + tmp.i * src->fmt.channels;
			sptr2 = sptr1 + src->fmt.channels;
			// 加算前の下駄 2^22 を脱ぐ
			int b = tmp.n * track->freq_coef / (1 << 22);
			int a = 256 - b;
			for (int ch = 0; ch < dst->fmt.channels; ch++) {
				// 加算後の下駄 2^8 を脱ぐ
				*dptr++ = (sptr1[ch] * a + sptr2[ch] * b) / 256;
			}
		}
#else
		if (tmp.i >= src->count) {
			break;
		}
		sptr1 = sptr0 + tmp.i * src->fmt.channels;
		for (int ch = 0; ch < dst->fmt.channels; ch++) {
			*dptr++ = sptr1[ch];
		}
#endif
		dst->count++;
		src_taken = tmp.i;
		audio_rational_add(&tmp, &track->freq_step, dst->fmt.sample_rate);
	}
	audio_ring_tookfromtop(src, src_taken + 1);
	tmp.i = 0;
	track->freq_current = tmp;
}

void
audio_track_init(audio_track_t *track, audio_trackmixer_t *mixer, int mode)
{
	struct audio_softc *sc = mixer->sc;
	audio_format2_t *default_format;
	const char *cvname;
	static int newid = 0;

	memset(track, 0, sizeof(audio_track_t));
	track->id = newid++;
	// ここだけ id が決まってから表示
	TRACE(track, "");

	if (mode == AUMODE_PLAY) {
		cvname = "audiowr";
		default_format = &sc->sc_pparams;
	} else {
		cvname = "audiord";
		default_format = &sc->sc_rparams;
	}

	track->mixer = mixer;
	track->mode = mode;
	cv_init(&track->outchan, cvname);
#if !defined(AUDIO_SOFTINTR)
	track->track_cl = 0;
#endif

	// 固定初期値
	track->volume = 256;
	for (int i = 0; i < AUDIO_MAX_CHANNELS; i++) {
		track->ch_volume[i] = 256;
	}

	// デフォルトフォーマットでセット
#if defined(AUDIO_SOFTINTR)
	mutex_enter(&track->mixer->softintrlock);
#else
	mutex_enter(track->mixer->sc->sc_intr_lock);
#endif
	audio_track_set_format(track, default_format);
#if defined(AUDIO_SOFTINTR)
	mutex_exit(&track->mixer->softintrlock);
#else
	mutex_exit(track->mixer->sc->sc_intr_lock);
#endif
}

// track 内のすべてのリソースを解放します。
// track 自身は解放しません。(file 内のメンバとして確保されているため)
void
audio_track_destroy(audio_track_t *track)
{
	audio_free(track->usrbuf.sample);
	audio_free(track->codec.srcbuf.sample);
	audio_free(track->chvol.srcbuf.sample);
	audio_free(track->chmix.srcbuf.sample);
	audio_free(track->freq.srcbuf.sample);
	audio_free(track->outputbuf.sample);
	cv_destroy(&track->outchan);
}

static inline int
framecount_roundup_byte_boundary(int framecount, int stride)
{
	/* stride が、、、 */
	if ((stride & 7) == 0) {
		/* 8 の倍数なのでそのままでいい */
		return framecount;
	} else if ((stride & 3) == 0) {
		/* 4 の倍数なので framecount が奇数なら +1 して 2 の倍数を返す */
		return framecount + (framecount & 1);
	} else if ((stride & 1) == 0) {
		/* 2 の倍数なので {0, 3, 2, 1} を足して 4 の倍数を返す */
		return framecount + ((4 - (framecount & 3)) & 3);
	} else {
		/* 8 とは互いに素なので {0,7,6,5,4,3,2,1} を足して 8 の倍数を返す */
		return framecount + ((8 - (framecount & 7)) & 7);
	}
}

/* stride に応じて、バイト境界に整列するフレーム数を求めます。 */
static inline int
audio_framealign(int stride)
{
	/* stride が、、、 */
	if ((stride & 7) == 0) {
		/* 8 の倍数なのでそのままでバイト境界 */
		return 1;
	} else if ((stride & 3) == 0) {
		/* 4 の倍数なので 2 フレームでバイト境界 */
		return 2;
	} else if ((stride & 1) == 0) {
		/* 2 の倍数なので 4 フレームでバイト境界 */
		return 4;
	} else {
		/* 8 とは互いに素なので 8 フレームでバイト境界 */
		return 8;
	}
}

static audio_ring_t *
init_codec(audio_track_t *track, audio_ring_t *last_dst)
{
	audio_format2_t *srcfmt = &track->inputfmt;
	audio_format2_t *dstfmt = &last_dst->fmt;

	if (srcfmt->encoding == dstfmt->encoding
	 && srcfmt->precision == dstfmt->precision
	 && srcfmt->stride == dstfmt->stride) {
		// チャンネル数以外が等しければエンコーディング変換不要
		track->codec.filter = NULL;
		audio_free(track->codec.srcbuf.sample);
		track->codec.srcbuf.sample = NULL;
		return last_dst;
	} else {
		// エンコーディングを変換する
		track->codec.dst = last_dst;

		// arg のために先にフォーマットを作る
		track->codec.srcbuf.fmt = *dstfmt;
		track->codec.srcbuf.fmt.encoding = srcfmt->encoding;
		track->codec.srcbuf.fmt.precision = srcfmt->precision;
		track->codec.srcbuf.fmt.stride = srcfmt->stride;

		track->codec.arg.srcfmt = &track->codec.srcbuf.fmt;
		track->codec.arg.dstfmt = dstfmt;
		track->codec.filter = audio_MI_codec_filter_init(&track->codec.arg);

		// TODO: codec デストラクタコール
		// XXX: インライン変換はとりあえず置いておく
		track->codec.srcbuf.top = 0;
		track->codec.srcbuf.count = 0;
		track->codec.srcbuf.capacity = frame_per_block_roundup(track->mixer, &track->codec.srcbuf.fmt);
		track->codec.srcbuf.sample = audio_realloc(track->codec.srcbuf.sample, RING_BYTELEN(&track->codec.srcbuf));

		return &track->codec.srcbuf;
	}
}

static audio_ring_t *
init_chvol(audio_track_t *track, audio_ring_t *last_dst)
{
	audio_format2_t *srcfmt = &track->inputfmt;
	audio_format2_t *dstfmt = &last_dst->fmt;

	// チャンネルボリュームが有効かどうか
	bool use_chvol = false;
	for (int ch = 0; ch < srcfmt->channels; ch++) {
		if (track->ch_volume[ch] != 256) {
			use_chvol = true;
			break;
		}
	}

	if (use_chvol == false) {
		track->chvol.filter = NULL;
		audio_free(track->chvol.srcbuf.sample);
		track->chvol.srcbuf.sample = NULL;
		return last_dst;
	} else {
		track->chvol.filter = audio_track_chvol;
		track->chvol.dst = last_dst;

		// 周波数とチャンネル数がユーザ指定値。
		track->chvol.srcbuf.fmt = *dstfmt;
		track->chvol.srcbuf.capacity = frame_per_block_roundup(track->mixer, &track->chvol.srcbuf.fmt);
		track->chvol.srcbuf.sample = audio_realloc(track->chvol.srcbuf.sample, RING_BYTELEN(&track->chvol.srcbuf));

		track->chvol.arg.count = track->chvol.srcbuf.capacity;
		track->chvol.arg.context = track->ch_volume;
		return &track->chvol.srcbuf;
	}
}


static audio_ring_t *
init_chmix(audio_track_t *track, audio_ring_t *last_dst)
{
	audio_format2_t *srcfmt = &track->inputfmt;
	audio_format2_t *dstfmt = &last_dst->fmt;

	int srcch = srcfmt->channels;
	int dstch = dstfmt->channels;

	if (srcch == dstch) {
		track->chmix.filter = NULL;
		audio_free(track->chmix.srcbuf.sample);
		track->chmix.srcbuf.sample = NULL;
		return last_dst;
	} else {
		if (srcch >= 2 && dstch == 1) {
			track->chmix.filter = audio_track_chmix_mixLR;
		} else if (srcch == 1 && dstch >= 2) {
			track->chmix.filter = audio_track_chmix_dupLR;
		} else if (srcch > dstch) {
			track->chmix.filter = audio_track_chmix_shrink;
		} else {
			track->chmix.filter = audio_track_chmix_expand;
		}

		track->chmix.dst = last_dst;
		// チャンネル数を srcch にする
		track->chmix.srcbuf.fmt = *dstfmt;
		track->chmix.srcbuf.fmt.channels = srcch;
		track->chmix.srcbuf.top = 0;
		track->chmix.srcbuf.count = 0;
		// バッファサイズは計算で決められるはずだけど。とりあえず。
		track->chmix.srcbuf.capacity = frame_per_block_roundup(track->mixer, &track->chmix.srcbuf.fmt);
		track->chmix.srcbuf.sample = audio_realloc(track->chmix.srcbuf.sample, RING_BYTELEN(&track->chmix.srcbuf));

		track->chmix.arg.srcfmt = &track->chmix.srcbuf.fmt;
		track->chmix.arg.dstfmt = dstfmt;

		return &track->chmix.srcbuf;
	}
}

static audio_ring_t*
init_freq(audio_track_t *track, audio_ring_t *last_dst)
{
	audio_format2_t *srcfmt = &track->inputfmt;
	audio_format2_t *dstfmt = &last_dst->fmt;

	uint32_t srcfreq = srcfmt->sample_rate;
	uint32_t dstfreq = dstfmt->sample_rate;

	if (srcfreq == dstfreq) {
		track->freq.filter = NULL;
		audio_free(track->freq.srcbuf.sample);
		track->freq.srcbuf.sample = NULL;
		return last_dst;
	} else {
		track->freq_step.i = srcfreq / dstfreq;
		track->freq_step.n = srcfreq % dstfreq;
		audio_rational_clear(&track->freq_current);
		// 除算をループから追い出すため、変換先周波数の逆数を求める。
		// 変換先周波数の逆数を << (8 + 22) したもの
		// 8 は加算後の下駄で、22 は加算前の下駄。
		track->freq_coef = (1 << (8 + 22)) / dstfreq;

		track->freq.arg.context = track;
		track->freq.arg.srcfmt = &track->freq.srcbuf.fmt;
		track->freq.arg.dstfmt = &last_dst->fmt;

		if (srcfreq < dstfreq) {
			track->freq.filter = audio_track_freq_up;
		} else {
			track->freq.filter = audio_track_freq_down;
		}
		track->freq.dst = last_dst;
		// 周波数のみ srcfreq
		track->freq.srcbuf.fmt = *dstfmt;
		track->freq.srcbuf.fmt.sample_rate = srcfreq;
		track->freq.srcbuf.top = 0;
		track->freq.srcbuf.count = 0;
		track->freq.srcbuf.capacity = frame_per_block_roundup(track->mixer, &track->freq.srcbuf.fmt);
		track->freq.srcbuf.sample = audio_realloc(track->freq.srcbuf.sample, RING_BYTELEN(&track->freq.srcbuf));
		return &track->freq.srcbuf;
	}
}

/*
* トラックのユーザランド側フォーマットを設定します。
* 変換用内部バッファは一度破棄されます。
*/
void
audio_track_set_format(audio_track_t *track, audio_format2_t *fmt)
{
	TRACE(track, "");
	KASSERT(is_valid_format(fmt));
#if defined(AUDIO_SOFTINTR)
	KASSERT(mutex_owned(&track->mixer->softintrlock));
#else
	KASSERT(mutex_owned(track->mixer->sc->sc_intr_lock));
#endif

	// 入力値チェック
#if defined(_KERNEL)
	// XXX audio.c にある。どうしたもんか
	audio_check_params2(fmt);
#endif

	// TODO: まず現在のバッファとかを全部破棄すると分かり易いが。

	audio_ring_t *last_dst = &track->outputbuf;
	if (track->mode == AUMODE_PLAY) {
		// 再生はトラックミキサ側から作る

		track->inputfmt = *fmt;

		track->outputbuf.fmt =  track->mixer->track_fmt;

		last_dst = init_freq(track, last_dst);
		last_dst = init_chmix(track, last_dst);
		last_dst = init_chvol(track, last_dst);
		last_dst = init_codec(track, last_dst);
	} else {
		// 録音はユーザランド側から作る

		track->inputfmt = track->mixer->track_fmt;

		track->outputbuf.fmt = *fmt;

		last_dst = init_codec(track, last_dst);
		last_dst = init_chvol(track, last_dst);
		last_dst = init_chmix(track, last_dst);
		last_dst = init_freq(track, last_dst);
	}

	// 入力バッファは先頭のステージ相当品
	track->input = last_dst;

	// 入力フォーマットに従って usrbuf を作る
	track->usrbuf.fmt = *fmt;
	track->usrbuf.top = 0;
	track->usrbuf.count = 0;
	track->usrbuf.capacity = NBLKOUT *
	    frametobyte(&track->inputfmt, track->input->capacity);
	track->usrbuf.sample = audio_realloc(track->usrbuf.sample,
	    track->usrbuf.capacity);

	// 出力フォーマットに従って outputbuf を作る
	track->outputbuf.top = 0;
	track->outputbuf.count = 0;
	track->outputbuf.capacity = NBLKOUT * frame_per_block_roundup(track->mixer, &track->outputbuf.fmt);
	track->outputbuf.sample = audio_realloc(track->outputbuf.sample, RING_BYTELEN(&track->outputbuf));
}

// ring が空でなく 1 ブロックに満たない時、1ブロックまで無音を追加します。
// 追加したフレーム数を返します。
static int
audio_append_silence(audio_track_t *track, audio_ring_t *ring)
{
	KASSERT(track);
	KASSERT(is_internal_format(&ring->fmt));

	if (ring->count == 0) return 0;

	int fpb = frame_per_block_roundup(track->mixer, &ring->fmt);
	if (ring->count >= fpb) {
		return 0;
	}

	int n = (ring->capacity - ring->count) % fpb;
	
	TRACE(track, "Append silence %d frames", n);
	KASSERT(audio_ring_unround_free_count(ring) >= n);

	memset(RING_BOT_UINT8(ring), 0, n * ring->fmt.channels * sizeof(internal_t));
	audio_ring_appended(ring, n);
	return n;
}

// このステージで処理を中断するときは false を返します。
static void
audio_apply_stage(audio_track_t *track, audio_stage_t *stage, bool isfreq)
{
	if (stage->filter != NULL) {
		int srccount = audio_ring_unround_count(&stage->srcbuf);
		int dstcount = audio_ring_unround_free_count(stage->dst);
		
		int count;
		if (isfreq) {
			if (srccount == 0) {
				panic("freq but srccount=0");
			}
			count = min(dstcount, track->mixer->frames_per_block);
		} else {
			count = min(srccount, dstcount);
		}

		if (count > 0) {
			audio_filter_arg_t *arg = &stage->arg;

			arg->src = RING_TOP_UINT8(&stage->srcbuf);
			arg->dst = RING_BOT_UINT8(stage->dst);
			arg->count = count;

			stage->filter(arg);

			if (!isfreq) {
				audio_ring_tookfromtop(&stage->srcbuf, count);
				audio_ring_appended(stage->dst, count);
			}
		}
	}
}

static int
audio_track_play_input(audio_track_t *track, struct uio *uio)
{
	audio_ring_t *usrbuf;
	int freebytes, movebytes;
	int error;
	uint8_t *dptr;
	KASSERT(uio);

	usrbuf = &track->usrbuf;
	TRACE(track, "resid=%zu usrbuf=%d/%d/%d", uio->uio_resid,
		usrbuf->top, usrbuf->count, usrbuf->capacity);

	// usrbuf の空きバイト数を求める
	if (usrbuf->top + usrbuf->count < usrbuf->capacity) {
		freebytes = usrbuf->capacity - (usrbuf->top + usrbuf->count);
	} else {
		freebytes = usrbuf->capacity - usrbuf->count;
	}
	if (freebytes == 0)
		return EAGAIN;

	// 今回 uiomove するバイト数
	movebytes = min(freebytes, (int)uio->uio_resid);

	TRACE(track, "freebytes=%d movebytes=%d", freebytes, movebytes);
	dptr = (uint8_t *)usrbuf->sample + audio_ring_bottom(usrbuf);
	error = uiomove(dptr, movebytes, uio);
	if (error) {
		TRACE(track, "uiomove error=%d", error);
		return error;
	}
	audio_ring_appended(usrbuf, movebytes);
	track->inputcounter += movebytes;

	return 0;
}

/*
 * 再生時の入力データを変換してトラックバッファに投入します。
 */
void
audio_track_play(audio_track_t *track, bool isdrain)
{
	KASSERT(track);

	int track_count_0 = track->outputbuf.count;

	// usrbuf からコピー
	// XXX usrbuf が1ブロック以上たまってなくてもここに来るかどうか
	int count = audio_ring_unround_free_count(track->input);
	int bytes = frametobyte(&track->inputfmt, count);
	if (track->usrbuf.count < bytes) {
		return;
	}
	memcpy(RING_BOT(internal_t, track->input),
	    RING_TOP(uint8_t, &track->usrbuf),
		bytes);
	audio_ring_appended(track->input, count);
	audio_ring_tookfromtop(&track->usrbuf, bytes);

	/* エンコーディング変換 */
	audio_apply_stage(track, &track->codec, false);

	/* チャンネルボリューム */
	audio_apply_stage(track, &track->chvol, false);

	/* チャンネルミキサ */
	audio_apply_stage(track, &track->chmix, false);

	/* 周波数変換 */
	if (track->freq.filter != NULL) {
		int n = 0;
		if (isdrain) {
			n = audio_append_silence(track, &track->freq.srcbuf);
			if (n > 0) {
				TRACE(track, "freq.srcbuf appended silence %d frames", n);
			}
		}
		if (track->freq.srcbuf.count > 0) {
			audio_apply_stage(track, &track->freq, true);
			// freq の入力はバッファ先頭から。
			// サブフレームの問題があるので、top 位置以降の全域をずらす。
			if (track->freq.srcbuf.top != 0) {
#if defined(AUDIO_DEBUG)
				if (track->freq.srcbuf.top + track->freq.srcbuf.count > track->freq.srcbuf.capacity) {
					panic("srcbuf broken, %d/%d/%d\n",
						track->freq.srcbuf.top,
						track->freq.srcbuf.count,
						track->freq.srcbuf.capacity);
				}
#endif
				uint8_t *s = track->freq.srcbuf.sample;
				uint8_t *p = RING_TOP_UINT8(&track->freq.srcbuf);
				uint8_t *e = RING_END_UINT8(&track->freq.srcbuf);
				memmove(s, p, e - p);
				track->freq.srcbuf.top = 0;
			}
		}
		if (n > 0 && track->freq.srcbuf.count > 0) {
			TRACE(track, "freq.srcbuf cleanup count=%d", track->freq.srcbuf.count);
			track->freq.srcbuf.count = 0;
		}
	}

	if (isdrain) {
		/* 無音をブロックサイズまで埋める */
		/* 内部フォーマットだとわかっている */
		/* 周波数変換の結果、ブロック未満の端数フレームが出ることもあるし、
		変換なしのときは入力自体が半端なときもあろう */
		int n = audio_append_silence(track, &track->outputbuf);
		if (n > 0) {
			TRACE(track, "track.outputbuf appended silence %d frames", n);
		}
	}

	if (track->input == &track->outputbuf) {
		track->outputcounter = track->inputcounter;
	} else {
		track->outputcounter += track->outputbuf.count - track_count_0;
	}

#if AUDIO_DEBUG > 2
	char buf[100];
	int n = 0;
	if (track->freq.filter)
		n += snprintf(buf + n, 100 - n, " f=%d", track->freq.srcbuf.count);
	if (track->chmix.filter)
		n += snprintf(buf + n, 100 - n, " m=%d", track->chmix.srcbuf.count);
	if (track->chvol.filter)
		n += snprintf(buf + n, 100 - n, " v=%d", track->chvol.srcbuf.count);
	if (track->codec.filter)
		n += snprintf(buf + n, 100 - n, " e=%d", track->codec.srcbuf.count);
	TRACE(track, "end pbusy=%d outbuf=%d/%d/%d%s", track->mixer->sc->sc_pbusy,
	    track->outputbuf.top, track->outputbuf.count, track->outputbuf.capacity,
	    buf);
#endif
}

int
audio_mixer_init(struct audio_softc *sc, audio_trackmixer_t *mixer, int mode)
{
	memset(mixer, 0, sizeof(audio_trackmixer_t));
	mixer->sc = sc;

#if defined(AUDIO_SOFTINTR)
	mixer->softintr = softint_establish(SOFTINT_SERIAL, audio_trackmixer_softintr, mixer);
#endif

	mixer->blktime_d = 1000;
	mixer->blktime_n = AUDIO_BLK_MS;
	mixer->hwblks = 16;

#if defined(_KERNEL)
	// XXX とりあえず
	if (mode == AUMODE_PLAY)
		mixer->hwbuf.fmt = sc->sc_phwfmt;
	else
		mixer->hwbuf.fmt = sc->sc_rhwfmt;
#else
	mixer->hwbuf.fmt = audio_softc_get_hw_format(mixer->sc, mode);
#endif

	mixer->frames_per_block = frame_per_block_roundup(mixer, &mixer->hwbuf.fmt)
		* audio_framealign(mixer->hwbuf.fmt.stride);
	int blksize = frametobyte(&mixer->hwbuf.fmt, mixer->frames_per_block);
	if (sc->hw_if->round_blocksize) {
		int rounded;
		audio_params_t p = format2_to_params(&mixer->hwbuf.fmt);
		mutex_enter(sc->sc_lock);
		rounded = sc->hw_if->round_blocksize(sc->hw_hdl, blksize, mode, &p);
		mutex_exit(sc->sc_lock);
		// 違っていても困る?
		if (rounded != blksize) {
			if ((rounded * 8) % (mixer->hwbuf.fmt.stride * mixer->hwbuf.fmt.channels) != 0) {
				aprint_error_dev(sc->dev, "blksize not configured"
					" %d -> %d\n", blksize, rounded);
				return ENXIO;
			}
			// 再計算
			mixer->frames_per_block = rounded * 8 / (mixer->hwbuf.fmt.stride * mixer->hwbuf.fmt.channels);
		}
	}
	mixer->blktime_n = mixer->frames_per_block;
	mixer->blktime_d = mixer->hwbuf.fmt.sample_rate;

	int capacity = mixer->frames_per_block * mixer->hwblks;
	size_t bufsize = frametobyte(&mixer->hwbuf.fmt, capacity);
	if (sc->hw_if->round_buffersize) {
		size_t rounded;
		mutex_enter(sc->sc_lock);
		rounded = sc->hw_if->round_buffersize(sc->hw_hdl, mode, bufsize);
		mutex_exit(sc->sc_lock);
		// 縮められても困る?
		if (rounded != bufsize) {
			aprint_error_dev(sc->dev, "buffer size not configured"
			    " %zu -> %zu\n", bufsize, rounded);
			return ENXIO;
		}
	}
	mixer->hwbuf.capacity = capacity;

	if (sc->hw_if->allocm) {
		mixer->hwbuf.sample = sc->hw_if->allocm(sc->hw_hdl, mode,
		    bufsize);
	} else {
		mixer->hwbuf.sample = kern_malloc(bufsize, M_NOWAIT);
	}

	mixer->track_fmt.encoding = AUDIO_ENCODING_SLINEAR_HE;
	mixer->track_fmt.channels = mixer->hwbuf.fmt.channels;
	mixer->track_fmt.sample_rate = mixer->hwbuf.fmt.sample_rate;
	mixer->track_fmt.precision = mixer->track_fmt.stride = AUDIO_INTERNAL_BITS;

	if (mode == AUMODE_PLAY) {
		// 合成バッファ
		mixer->mixfmt = mixer->track_fmt;
		mixer->mixfmt.precision *= 2;
		mixer->mixfmt.stride *= 2;
		int n = mixer->frames_per_block * mixer->mixfmt.channels * mixer->mixfmt.stride / 8;
		mixer->mixsample = audio_realloc(mixer->mixsample, n);
	} else {
		// 合成バッファは使用しない
	}

	// XXX どうするか
	audio_filter_reg_t *reg;
	if (mode == AUMODE_PLAY) {
		reg = &sc->sc_xxx_pfilreg;
	} else {
		reg = &sc->sc_xxx_rfilreg;
	}
	mixer->codec = reg->codec;
	mixer->codecarg.context = reg->context;
	mixer->codecarg.srcfmt = &mixer->track_fmt;
	mixer->codecarg.dstfmt = &mixer->hwbuf.fmt;
	mixer->codecbuf.fmt = mixer->track_fmt;
	mixer->codecbuf.capacity = mixer->frames_per_block;
	mixer->codecbuf.sample = audio_realloc(mixer->codecbuf.sample, RING_BYTELEN(&mixer->codecbuf));

	mixer->volume = 256;

	cv_init(&mixer->intrcv, "audiodr");
	return 0;
}

void
audio_mixer_destroy(audio_trackmixer_t *mixer, int mode)
{
	struct audio_softc *sc = mixer->sc;

	if (mixer->hwbuf.sample != NULL) {
		if (sc->hw_if->freem) {
			sc->hw_if->freem(sc->hw_hdl, mixer->hwbuf.sample, mode);
		} else {
			kern_free(mixer->hwbuf.sample);
		}
		mixer->hwbuf.sample = NULL;
	}

	if (mode == AUMODE_PLAY) {
		kern_free(mixer->mixsample);
	} else {
		// 合成バッファは使用しない
	}

#if defined(AUDIO_SOFTINTR)
	softint_disestablish(mixer->softintr);
#endif

	// intrcv を cv_destroy() してはいけないっぽい。KASSERT で死ぬ。
}

// 全トラックを req フレーム分合成します。
// 合成が行われれば 1 以上を返す?
// mixer->softintrlock を取得して呼び出してください。
static int
audio_trackmixer_mixall(audio_trackmixer_t *mixer, int req, bool isintr)
{
	struct audio_softc *sc;
	audio_file_t *f;
	int mixed = 0;

	sc = mixer->sc;
#if defined(AUDIO_SOFTINTR)
	KASSERT(mutex_owned(&mixer->softintrlock));
#endif

	SLIST_FOREACH(f, &sc->sc_files, entry) {
		audio_track_t *track = &f->ptrack;

#if !defined(AUDIO_SOFTINTR)
		if (isintr) {
			// 協調的ロックされているトラックは、今回ミキシングしない。
			if (track->track_cl) continue;
		}
#endif

		if (track->outputbuf.count < req) {
			audio_track_play(track, isintr);
		}

		// 合成
		if (track->outputbuf.count > 0) {
			mixed = audio_mixer_play_mix_track(mixer, track, req, mixed);
		}
	}
	return mixed;
}

static void
audio2_pintr(void *arg)
{
	struct audio_softc *sc;
	audio_trackmixer_t *mixer;

	sc = arg;
	KASSERT(mutex_owned(sc->sc_intr_lock));

	mixer = sc->sc_pmixer;
	TRACE0("hwbuf.count=%d", mixer->hwbuf.count);

	audio_trackmixer_intr(mixer);
}

static int
audio2_trigger_output(audio_trackmixer_t *mixer, void *start, void *end, int blksize)
{
	struct audio_softc *sc = mixer->sc;

	KASSERT(sc->hw_if->trigger_output != NULL);
	KASSERT(mutex_owned(sc->sc_intr_lock));

	audio_params_t params;
	// TODO: params 作る
	params = format2_to_params(&mixer->hwbuf.fmt);
	int error = sc->hw_if->trigger_output(sc->hw_hdl, start, end, blksize, audio2_pintr, sc, &params);
	return error;
}

static int
audio2_start_output(audio_trackmixer_t *mixer, void *start, int blksize)
{
	struct audio_softc *sc = mixer->sc;

	KASSERT(sc->hw_if->start_output != NULL);
	KASSERT(mutex_owned(sc->sc_intr_lock));

	int error = sc->hw_if->start_output(sc->hw_hdl, start, blksize, audio2_pintr, sc);
	return error;
}

int
audio2_halt_output(struct audio_softc *sc)
{
	int error;

	TRACE0("");
	KASSERT(mutex_owned(sc->sc_lock));
	KASSERT(mutex_owned(sc->sc_intr_lock));

	error = sc->hw_if->halt_output(sc->hw_hdl);
	// エラーが起きても停止は停止する
	sc->sc_pbusy = false;
	sc->sc_pmixer->hwbuf.top = 0;
	sc->sc_pmixer->hwbuf.count = 0;

	return error;
}

// トラックミキサ起動になる可能性のある再生要求
// トラックミキサが起動したら true を返します。
// 割り込みコンテキストから呼び出してはいけません。
bool
audio_trackmixer_play(audio_trackmixer_t *mixer, bool force)
{
	struct audio_softc *sc;

	sc = mixer->sc;
	KASSERT(mutex_owned(sc->sc_lock));
	KASSERT(!mutex_owned(sc->sc_intr_lock));

	// トラックミキサが起動していたら、割り込みから駆動されるので true を返しておく
	if (sc->sc_pbusy) return true;

	TRACE0("begin mixseq=%d hwseq=%d hwbuf=%d/%d/%d",
		(int)mixer->mixseq, (int)mixer->hwseq,
		mixer->hwbuf.top, mixer->hwbuf.count, mixer->hwbuf.capacity);

	// ハードウェアバッファを埋める

	if (mixer->hwbuf.capacity - mixer->hwbuf.count >= mixer->frames_per_block) {
		int mixed = audio_trackmixer_mixall(mixer, mixer->frames_per_block, false);

		// バッファの準備ができたら転送。
		if (mixed) {
			audio_mixer_play_period(mixer);
		}
		int minimum = (force) ? 1 : mixer->hwblks;
		if (mixer->hwbuf.count >= mixer->frames_per_block * minimum) {
			// トラックミキサ出力開始
			mutex_enter(sc->sc_intr_lock);
			audio_trackmixer_output(mixer);
			mutex_exit(sc->sc_intr_lock);
		}
	}

	TRACE0("end   mixseq=%d hwseq=%d hwbuf=%d/%d/%d",
		(int)mixer->mixseq, (int)mixer->hwseq,
		mixer->hwbuf.top, mixer->hwbuf.count, mixer->hwbuf.capacity);

	return sc->sc_pbusy;
}

/*
* トラックバッファから取り出し、ミキシングします。
* mixed には呼び出し時点までの合成済みトラック数を渡します。
* 戻り値はこの関数終了時での合成済みトラック数)です。
* つまりこのトラックを合成すれば mixed + 1 を返します。
*/
int
audio_mixer_play_mix_track(audio_trackmixer_t *mixer, audio_track_t *track, int req, int mixed)
{
	/* req フレーム貯まるまで待つ */
	if (track->outputbuf.count < req) {
		TRACE(track, "track count(%d) < req(%d); return",
		    track->outputbuf.count, req);
		return mixed;
	}

	// このトラックが処理済みならなにもしない
	if (mixer->mixseq < track->seq) return mixed;

	int count = mixer->frames_per_block;

	KASSERT(audio_ring_unround_count(&track->outputbuf) >= count);

	internal_t *sptr = RING_TOP(internal_t, &track->outputbuf);
	internal2_t *dptr = mixer->mixsample;

	/* 整数倍精度へ変換し、トラックボリュームを適用して加算合成 */
	int sample_count = count * mixer->mixfmt.channels;
	if (mixed == 0) {
		// 最初のトラック合成は代入
		if (track->volume == 256) {
			for (int i = 0; i < sample_count; i++) {
				*dptr++ = ((internal2_t)*sptr++);
			}
		} else {
			for (int i = 0; i < sample_count; i++) {
				*dptr++ = ((internal2_t)*sptr++) * track->volume / 256;
			}
		}
	} else {
		// 2本め以降なら加算合成
		if (track->volume == 256) {
			for (int i = 0; i < sample_count; i++) {
				*dptr++ += ((internal2_t)*sptr++);
			}
		} else {
			for (int i = 0; i < sample_count; i++) {
				*dptr++ += ((internal2_t)*sptr++) * track->volume / 256;
			}
		}
	}

	audio_ring_tookfromtop(&track->outputbuf, count);

	/* トラックバッファを取り込んだことを反映 */
	// mixseq はこの時点ではまだ前回の値なのでトラック側へは +1 
	track->seq = mixer->mixseq + 1;

	// audio_write() に空きが出来たことを通知
	cv_broadcast(&track->outchan);

	TRACE(track, "broadcast; trseq=%d count=%d", (int)track->seq, count);
	return mixed + 1;
}

/*
 * ミキシングバッファから物理デバイスバッファへ
 */
// 呼び出し時の sc_intr_lock の状態はどちらでもよく、hwbuf へのアクセスを
// sc_intr_lock でこの関数が保護します。
void
audio_mixer_play_period(audio_trackmixer_t *mixer /*, bool force */)
{
	struct audio_softc *sc;

	sc = mixer->sc;

	/* 今回取り出すフレーム数を決定 */

	int mix_count = mixer->frames_per_block;
	int hw_free_count = audio_ring_unround_free_count(&mixer->hwbuf);
	int count = min(mix_count, hw_free_count);
	if (count <= 0) {
		TRACE0("count too short: mix_count=%d hw_free=%d", mix_count, hw_free_count);
		return;
	}

	mixer->mixseq++;

	/* オーバーフロー検出 */
	internal2_t ovf_plus = AUDIO_INTERNAL_T_MAX;
	internal2_t ovf_minus = AUDIO_INTERNAL_T_MIN;

	internal2_t *mptr0 = mixer->mixsample;
	internal2_t *mptr = mptr0;

	int sample_count = count * mixer->mixfmt.channels;
	for (int i = 0; i < sample_count; i++) {
		if (*mptr > ovf_plus) ovf_plus = *mptr;
		if (*mptr < ovf_minus) ovf_minus = *mptr;

		mptr++;
	}

	/* マスタボリュームの自動制御 */
	int vol = mixer->volume;
	if (ovf_plus > (internal2_t)AUDIO_INTERNAL_T_MAX
	 || ovf_minus < (internal2_t)AUDIO_INTERNAL_T_MIN) {
		// TODO: AUDIO_INTERNAL2_T_MIN チェック?
		internal2_t ovf = ovf_plus;
		if (ovf < -ovf_minus) ovf = -ovf_minus;

		/* オーバーフローしてたら少なくとも今回はボリュームを下げる */
		int vol2 = (int)((internal2_t)AUDIO_INTERNAL_T_MAX * 256 / ovf);
		if (vol2 < vol) vol = vol2;

		if (vol < mixer->volume) {
			// 128 までは自動でマスタボリュームを下げる
			// 今の値の 95% ずつに下げていってみる
			if (mixer->volume > 128) {
				mixer->volume = mixer->volume * 95 / 100;
				aprint_normal_dev(sc->dev, "auto volume adjust: volume %d\n",
					mixer->volume);
			}
		}
	}

	/* マスタボリューム適用 */
	if (vol != 256) {
		mptr = mptr0;
		for (int i = 0; i < sample_count; i++) {
			*mptr = *mptr * vol / 256;
			mptr++;
		}
	}

	/* ここから ハードウェアチャンネル */

	/* ハードウェアバッファへ転送 */
	int need_exit = mutex_tryenter(sc->sc_intr_lock);

	mptr = mptr0;
	internal_t *hptr;
	// MD 側フィルタがあれば internal2_t -> internal_t を codecbuf へ
	if (mixer->codec) {
		hptr = RING_BOT(internal_t, &mixer->codecbuf);
	} else {
		hptr = RING_BOT(internal_t, &mixer->hwbuf);
	}

	for (int i = 0; i < sample_count; i++) {
		*hptr++ = *mptr++;
	}

	// MD 側フィルタ
	if (mixer->codec) {
		audio_ring_appended(&mixer->codecbuf, count);
		mixer->codecarg.src = RING_TOP_UINT8(&mixer->codecbuf);
		mixer->codecarg.dst = RING_BOT_UINT8(&mixer->hwbuf);
		mixer->codecarg.count = count;
		mixer->codec(&mixer->codecarg);
		audio_ring_tookfromtop(&mixer->codecbuf, mixer->codecarg.count);
	}

	audio_ring_appended(&mixer->hwbuf, count);

	TRACE0("hwbuf=%d/%d/%d",
		mixer->hwbuf.top, mixer->hwbuf.count, mixer->hwbuf.capacity);

	if (need_exit) {
		mutex_exit(sc->sc_intr_lock);
	}

}

// ハードウェアバッファから 1 ブロック出力
// sc_intr_lock で呼び出してください。
void
audio_trackmixer_output(audio_trackmixer_t *mixer)
{
	struct audio_softc *sc;

	sc = mixer->sc;
	KASSERT(mutex_owned(sc->sc_intr_lock));

	TRACE0("pbusy=%d hwbuf=%d/%d/%d",
	    sc->sc_pbusy,
	    mixer->hwbuf.top, mixer->hwbuf.count, mixer->hwbuf.capacity);
	KASSERT(mixer->hwbuf.count >= mixer->frames_per_block);

	if (sc->hw_if->trigger_output) {
		if (!sc->sc_pbusy) {
			audio2_trigger_output(
				mixer,
				mixer->hwbuf.sample,
				RING_END_UINT8(&mixer->hwbuf),
				frametobyte(&mixer->hwbuf.fmt, mixer->frames_per_block));
		}
	} else {
		audio2_start_output(
			mixer,
			RING_TOP_UINT8(&mixer->hwbuf),
			frametobyte(&mixer->hwbuf.fmt, mixer->frames_per_block));
	}
	sc->sc_pbusy = true;
}

// 割り込みハンドラ
// sc_intr_lock で呼び出されます。
void
audio_trackmixer_intr(audio_trackmixer_t *mixer)
{
	struct audio_softc *sc __diagused;

	sc = mixer->sc;
	KASSERT(mutex_owned(sc->sc_intr_lock));

	mixer->hw_complete_counter += mixer->frames_per_block;
	mixer->hwseq++;

	audio_ring_tookfromtop(&mixer->hwbuf, mixer->frames_per_block);

	// まず出力待ちのシーケンスを出力
	if (mixer->hwbuf.count >= mixer->frames_per_block) {
		audio_trackmixer_output(mixer);
	}

#if defined(AUDIO_SOFTINTR)
	// ハードウェア割り込みでは待機関数が使えないため、
	// softintr へ転送。
	softint_schedule(mixer->softintr);

#else
	bool later = false;

	if (mixer->hwbuf.count < mixer->frames_per_block) {
		later = true;
	}

	// 次のバッファを用意する
	int mixed = audio_trackmixer_mixall(mixer, mixer->frames_per_block, true);
	if (mixed) {
		audio_mixer_play_period(mixer);
	}

	if (mixer->hwbuf.count == 0) {
		// 無音挿入
		memset(mixer->mixsample, 0, frametobyte(&mixer->mixfmt, mixer->frames_per_block));
		audio_mixer_play_period(mixer);
	}

	if (later) {
		audio_trackmixer_output(mixer);
	}

	// drain 待ちしている人のために通知
	cv_broadcast(&mixer->intrcv);
#endif

	TRACE0("HW_INT ++hwsec=%d cmplcnt=%d hwbuf=%d/%d/%d",
		(int)mixer->hwseq,
		(int)mixer->hw_complete_counter,
		mixer->hwbuf.top, mixer->hwbuf.count, mixer->hwbuf.capacity);
}

#if defined(AUDIO_SOFTINTR)
static void
audio_trackmixer_softintr(void *arg)
{
	audio_trackmixer_t *mixer = arg;
	struct audio_softc *sc = mixer->sc;

	KASSERT(!mutex_owned(sc->sc_intr_lock));

	mutex_enter(&mixer->softintrlock);

	bool later = false;

	if (mixer->hwbuf.count < mixer->frames_per_block) {
		later = true;
	}

	// 次のバッファを用意する
	while (mixer->hwbuf.count < mixer->frames_per_block * 2) {
		int mixed = audio_trackmixer_mixall(mixer, mixer->frames_per_block, true);
		if (mixed == 0) break;
		audio_mixer_play_period(mixer);
	}

	if (mixer->hwbuf.count == 0) {
		// 無音挿入
		memset(mixer->mixsample, 0, frametobyte(&mixer->mixfmt, mixer->frames_per_block));
		audio_mixer_play_period(mixer);
	}

	if (later) {
		mutex_enter(sc->sc_intr_lock);
		audio_trackmixer_output(mixer);
		mutex_exit(sc->sc_intr_lock);
	}
	// finally
	mutex_exit(&mixer->softintrlock);

	// drain 待ちしている人のために通知
	cv_broadcast(&mixer->intrcv);
	TRACE0("SW_INT ++hwsec=%d cmplcnt=%d hwbuf=%d/%d/%d",
		(int)mixer->hwseq,
		(int)mixer->hw_complete_counter,
		mixer->hwbuf.top, mixer->hwbuf.count, mixer->hwbuf.capacity);
}
#endif

#if !defined(_KERNEL)
int
audio_track_play_drain(audio_track_t *track, bool wait)
{
	// 割り込みエミュレートしているときはメインループに制御を戻さないといけない
	audio_trackmixer_t *mixer = track->mixer;
	struct audio_softc *sc = mixer->sc;
	mutex_enter(sc->sc_lock);
	audio_track_play_drain_core(track, wait);
	mutex_exit(sc->sc_lock);
	return 0;
}
#else
int
audio_track_play_drain(audio_track_t *track)
{
	return audio_track_play_drain_core(track, true);
}
#endif

// errno を返します。
int
audio_track_play_drain_core(audio_track_t *track, bool wait)
{
	audio_trackmixer_t *mixer = track->mixer;
	struct audio_softc *sc = mixer->sc;
	int error;

	TRACE(track, "");
	KASSERT(mutex_owned(sc->sc_lock));
	KASSERT(!mutex_owned(sc->sc_intr_lock));

	track->is_draining = true;

	// 必要があれば無音挿入させる
#if defined(AUDIO_SOFTINTR)
	mutex_enter(&mixer->softintrlock);
#else
	audio_track_cl(track);
#endif

	audio_track_play(track, true);

	if (sc->sc_pbusy == false) {
		// トラックバッファが空になっても、ミキサ側で処理中のデータが
		// あるかもしれない
		// トラックミキサが動作していないときは、動作させる
		audio_trackmixer_play(mixer, true);
	}

#if defined(AUDIO_SOFTINTR)
	mutex_exit(&mixer->softintrlock);
#else
	audio_track_uncl(track);
#endif

	if (wait) {
		while (track->seq > mixer->hwseq) {
			error = cv_wait_sig(&mixer->intrcv, sc->sc_lock);
			if (error) {
				printf("cv_wait_sig failed %d\n", error);
				if (error < 0)
					error = EINTR;
				return error;
			}
		}

		track->is_draining = false;
		TRACE(track, "uio_count=%d trk_count=%d tm=%d mixhw=%d hw_complete=%d",
			(int)track->inputcounter, (int)track->outputcounter,
			(int)track->track_mixer_counter, (int)track->mixer_hw_counter,
			(int)track->hw_complete_counter);
	}
	return 0;
}

/* write の MI 側 */
int
audio_write(struct audio_softc *sc, struct uio *uio, int ioflag, audio_file_t *file)
{
	int error;
	audio_track_t *track = &file->ptrack;
	TRACE(track, "resid=%u", (int)uio->uio_resid);

	KASSERT(mutex_owned(sc->sc_lock));

	if (sc->hw_if == NULL)
		return ENXIO;

	if (uio->uio_resid == 0) {
		sc->sc_eof++;
		return 0;
	}

#ifdef AUDIO_PM_IDLE
	if (device_is_active(&sc->dev) || sc->sc_idle)
		device_active(&sc->dev, DVA_SYSTEM);
#endif

#if defined(_KERNEL)
	/*
	 * If half-duplex and currently recording, throw away data.
	 */
	// half-duplex で録音中なら、このデータは捨てる。XXX どうするか
	if (!sc->sc_full_duplex && file->rtrack.mode != 0) {
		uio->uio_offset += uio->uio_resid;
		uio->uio_resid = 0;
		DPRINTF(1, "audio_write: half-dpx read busy\n");
		return 0;
	}

	// XXX playdrop と PLAY_ALL はちょっと後回し
#endif // _KERNEL

	error = 0;

#if defined(AUDIO_SOFTINTR)
	audio_trackmixer_t *mixer = track->mixer;

	mutex_enter(&mixer->softintrlock);
#else
	audio_track_cl(track);
#endif

	while (uio->uio_resid > 0) {
		error = audio_track_play_input(track, uio);
		if (error == EAGAIN) {
#if defined(AUDIO_SOFTINTR)
			mutex_exit(&mixer->softintrlock);
#else
			audio_track_uncl(track);
#endif
			error = audio_waitio(sc, track);

#if defined(AUDIO_SOFTINTR)
			mutex_enter(&mixer->softintrlock);
#else
			audio_track_cl(track);
#endif
			if (error < 0) {
				error = EINTR;
			}
			if (error) {
				break;
			}
		} else if (error) {
			break;
		}
		audio_track_play(track, false);

		audio_trackmixer_play(sc->sc_pmixer, false);

#if !defined(_KERNEL)
		emu_intr_check();
#endif
	}
#if defined(AUDIO_SOFTINTR)
	mutex_exit(&mixer->softintrlock);
#else
	audio_track_uncl(track);
#endif

	return error;
}

static int
audio_waitio(struct audio_softc *sc, audio_track_t *track)
{
	// XXX 自分がいなくなることを想定する必要があるのかどうか
	int error;

	KASSERT(mutex_owned(sc->sc_lock));

	TRACE(track, "wait");
	/* Wait for pending I/O to complete. */
	error = cv_wait_sig(&track->outchan, sc->sc_lock);

	TRACE(track, "error=%d", error);
	return error;
}

#if !defined(_KERNEL)
/*
 * ***** audio_file *****
 */
int//ssize_t
sys_write(audio_file_t *file, void* buf, size_t len)
{
	KASSERT(buf);

	if (len > INT_MAX) {
		errno = EINVAL;
		return -1;
	}

	struct uio uio = buf_to_uio(buf, len, UIO_READ);

	mutex_enter(file->sc->sc_lock);
	int error = audio_write(file->sc, &uio, 0, file);
	mutex_exit(file->sc->sc_lock);
	if (error) {
		errno = error;
		return -1;
	}
	return (int)len;
}

audio_file_t *
sys_open(struct audio_softc *sc, int mode)
{
	audio_file_t *file;

	file = calloc(1, sizeof(audio_file_t));
	file->sc = sc;

	if (mode == AUMODE_PLAY) {
		audio_track_init(&file->ptrack, sc->sc_pmixer, AUMODE_PLAY);
	} else {
		audio_track_init(&file->rtrack, sc->sc_rmixer, AUMODE_RECORD);
	}

	SLIST_INSERT_HEAD(&sc->sc_files, file, entry);

	return file;
}
#endif // !_KERNEL
