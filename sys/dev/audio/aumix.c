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

static int audio_waitio(struct audio_softc *sc, void *kcondvar, audio_track_t *track);


#define AUDIO_TRACE
#ifdef AUDIO_TRACE
#define TRACE0(fmt, ...)	do {	\
	printf("%s ", __func__);	\
	printf(fmt, ## __VA_ARGS__);	\
	printf("\n");	\
} while (0)
#define TRACE(t, fmt, ...)	do {	\
	printf("%s #%d ", __func__, (t)->id);		\
	printf(fmt, ## __VA_ARGS__);	\
	printf("\n");	\
} while (0)
#else
#define TRACE0(fmt, ...)	/**/
#define TRACE(t, fmt, ...)	/**/
#endif

int
gcd(int a, int b)
{
	int t;
	if (a == b) return a;
	if (a < b) {
		t = a;
		a = b;
		b = t;
	}

	while (b > 0) {
		t = b;
		b = a % b;
		a = t;
	}
	return a;
}

/* メモリアロケーションの STUB */

void *
audio_realloc(void *memblock, size_t bytes)
{
	if (memblock != NULL) {
		if (bytes != 0) {
			return realloc(memblock, bytes);
		} else {
			free(memblock);
			return NULL;
		}
	} else {
		if (bytes != 0) {
			return malloc(bytes);
		} else {
			return NULL;
		}
	}
}

void *
audio_free(void *memblock)
{
	if (memblock != NULL) {
		free(memblock);
	}
	return NULL;
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

static void
audio_track_chvol(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));
	KASSERT(arg->src_fmt->channels == arg->dst_fmt->channels);
	KASSERT(arg->context != NULL);
	KASSERT(arg->src_fmt->channels <= AUDIO_MAX_CH);

	int16_t *ch_volume = arg->context;
	const internal_t *sptr = arg->src;
	internal_t *dptr = arg->dst;

	for (int i = 0; i < arg->count; i++) {
		for (int ch = 0; ch < arg->src_fmt->channels; ch++, sptr++, dptr++) {
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
		internal2_t s;
		s = (internal2_t)sptr[0];
		s += (internal2_t)sptr[1];
		*dptr = s / 2;
		dptr++;
		sptr += arg->src_fmt->channels;
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
		dptr += arg->dst_fmt->channels;
		sptr++;
	}
	if (arg->dst_fmt->channels > 2) {
		dptr = arg->dst;
		for (int i = 0; i < arg->count; i++) {
			for (int ch = 2; ch < arg->dst_fmt->channels; ch++) {
				dptr[ch] = 0;
			}
			dptr += arg->dst_fmt->channels;
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
		for (int ch = 0; ch < arg->dst_fmt->channels; ch++) {
			*dptr++ = sptr[ch];
		}
		sptr += arg->src_fmt->channels;
	}
}

static void
audio_track_chmix_expand(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));

	const internal_t *sptr = arg->src;
	internal_t *dptr = arg->dst;

	for (int i = 0; i < arg->count; i++) {
		for (int ch = 0; ch < arg->src_fmt->channels; ch++) {
			*dptr++ = *sptr++;
		}
		for (int ch = arg->src_fmt->channels; ch < arg->dst_fmt->channels; ch++) {
			*dptr++ = 0;
		}
	}
}

audio_format_t default_format = {
	AUDIO_ENCODING_MULAW,
	8000, /* freq */
	1, /* channels */
	8, /* precision */
	8, /* stride */
};

void
audio_track_init(audio_track_t *track, audio_trackmixer_t *mixer)
{
	static int newid = 0;
	memset(track, 0, sizeof(audio_track_t));
	track->mixer = mixer;

	/* userio_buf はユーザフォーマット。 */
	track->userio_fmt = default_format;
	track->userio_buf.fmt = &track->userio_fmt;

	/* codec_buf は内部フォーマットだが userio 周波数, userio チャンネル */
	track->codec_fmt = mixer->track_fmt;
	track->codec_buf.fmt = &track->codec_fmt;

	/* step2 は内部フォーマットだが userio 周波数と処理用チャンネル */
	track->chmix_fmt = mixer->track_fmt;
	track->chmix_buf.fmt = &track->chmix_fmt;

	/* track_buf は内部フォーマット */
	track->track_buf.fmt = &mixer->track_fmt;
	track->track_buf.top = 0;
	track->track_buf.count = 0;
	track->track_buf.capacity = 16 * mixer->frames_per_block;
	track->track_buf.sample = audio_realloc(track->track_buf.sample, RING_BYTELEN(&track->track_buf));

	audio_track_set_format(track, &default_format);

	track->volume = 256;
	track->id = newid++;
	// ここだけ id が決まってから表示
	TRACE(track, "");
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

/*
* トラックのユーザランド側フォーマットを設定します。
* 変換用内部バッファは一度破棄されます。
*/
void
audio_track_set_format(audio_track_t *track, audio_format_t *fmt)
{
	TRACE(track, "");
	KASSERT(is_valid_format(fmt));

	// TODO: 入力値チェックをどこかでやる。

	// XXX: どっかで持つか引数か関数分離か
	int mode = AUMODE_PLAY;

	// TODO: まず現在のバッファとかを全部破棄すると分かり易いが。

	audio_format_t *track_fmt = track->track_buf.fmt;

	track->userio_fmt = *fmt;
	track->userio_frames_per_block = fmt->frequency * AUDIO_BLK_MS / 1000;
	track->framealign = audio_framealign(fmt->stride);

	// チャンネルボリュームが有効かどうか
	bool use_chvol = false;
	for (int ch = 0; ch < fmt->channels; ch++) {
		if (track->ch_volume[ch] != 256) {
			use_chvol = true;
			break;
		}
	}

	if (use_chvol == false && memcmp(fmt, track_fmt, sizeof(audio_format_t)) == 0) {
		// 無変換保証されたら直接書き込む
		track->userio_inout = &track->track_buf;
		track->userio_buf.sample = audio_free(track->userio_buf.sample);
	} else {
		track->userio_inout = &track->userio_buf;
		track->userio_buf.top = 0;
		track->userio_buf.count = 0;
		// バッファの容量を framealign の倍数にしておけば全体としてバイト境界問題が解決できる
		// レコーディングのときはこの条件が必須になる。再生時は codec のほうで必須になる。
		track->userio_buf.capacity = track->userio_frames_per_block * track->framealign;
		track->userio_buf.sample = audio_realloc(track->userio_buf.sample, RING_BYTELEN(&track->userio_buf));
		// TODO: バッファサイズをページサイズとの間で調整しても良いかも
	}

	/* codec 通過後は内部フォーマットだが userio 周波数, userio チャンネル */
	// そのほかのフィールドは init で初期化されている
	track->codec_fmt.frequency = fmt->frequency;
	track->codec_fmt.channels = fmt->channels;

	track->codec_in = track->userio_inout;
	if (fmt->encoding == track_fmt->encoding
	 && fmt->precision == track_fmt->precision
	 && fmt->stride == track_fmt->stride) {
		// チャンネル数以外が等しければエンコーディング変換不要
		track->codec_out = track->userio_inout;
		// TODO: codec デストラクタコール
		track->codec = NULL;
		track->codec_buf.sample = audio_free(track->codec_buf.sample);
	} else {
		track->codec_out = &track->codec_buf;
		// TODO: codec デストラクタコール
		track->codec_arg.src_fmt = track->codec_in->fmt;
		track->codec_arg.dst_fmt = track->codec_out->fmt;
		track->codec = audio_MI_codec_filter_init(&track->codec_arg);
		// XXX: インライン変換はとりあえず置いておく
		track->codec_buf.top = 0;
		track->codec_buf.count = 0;
		// バッファの容量を framealign の倍数にしておけば全体としてバイト境界問題が解決できる
		// ほかのバッファはともかく、このバッファはこの条件が必須。
		track->codec_buf.capacity = track->userio_frames_per_block * track->framealign;
		track->codec_buf.sample = audio_realloc(track->codec_buf.sample, RING_BYTELEN(&track->codec_buf));
	}

	track->chvol_in = track->codec_out;
	if (use_chvol == false) {
		track->chvol = NULL;
		track->chvol_out = track->chvol_in;
		track->chvol_buf.sample = audio_free(track->chvol_buf.sample);
	} else {
		track->chvol_out = &track->chvol_buf;
		track->chvol_buf.fmt = track->codec_out->fmt;
		track->chvol_arg.src_fmt = track->chvol_in->fmt;
		track->chvol_arg.dst_fmt = track->chvol_out->fmt;
		track->chvol_arg.count = track->userio_frames_per_block;
		track->chvol_arg.context = track->ch_volume;
		track->chvol = audio_track_chvol;
		track->chvol_buf.capacity = track->userio_frames_per_block;
		track->chvol_buf.sample = audio_realloc(track->chvol_buf.sample, RING_BYTELEN(&track->chvol_buf));
	}

	/* chmix_buf は内部フォーマットだが userio 周波数 */
	// そのほかのフィールドは init で初期化されている
	track->chmix_fmt.frequency = fmt->frequency;

	track->chmix_in = track->chvol_out;
	if (fmt->channels == track_fmt->channels) {
		track->chmix_out = track->chmix_in;
		track->chmix = NULL;
	} else {
		track->chmix_out = &track->chmix_buf;
		track->chmix_arg.src_fmt = track->chmix_in->fmt;
		track->chmix_arg.dst_fmt = track->chmix_out->fmt;
		track->chmix_arg.count = track->userio_frames_per_block;
		track->chmix_buf.top = 0;
		track->chmix_buf.count = 0;
		track->chmix_buf.capacity = track->chmix_in->capacity;
		track->chmix_buf.sample = audio_realloc(track->chmix_buf.sample, RING_BYTELEN(&track->chmix_buf));

		if (fmt->channels == 2 && track_fmt->channels == 1) {
			track->chmix = audio_track_chmix_mixLR;
		} else if (fmt->channels == 1 && track_fmt->channels == 2) {
			track->chmix = audio_track_chmix_dupLR;
		} else if (fmt->channels > track_fmt->channels) {
			track->chmix = audio_track_chmix_shrink;
		} else {
			track->chmix = audio_track_chmix_expand;
		}
	}

	track->freq_in = track->chmix_out;
	track->freq_out = &track->track_buf;

	track->freq_step.i = fmt->frequency / track->mixer->mix_fmt.frequency;
	track->freq_step.n = fmt->frequency % track->mixer->mix_fmt.frequency;
	audio_rational_clear(&track->freq_current);

	if (debug) {
		printf("%s: userfmt=%s\n", __func__, fmt_tostring(&track->userio_fmt));
	}
}


void
audio_track_lock(audio_track_t *track)
{
	// STUB
}

void
audio_track_unlock(audio_track_t *track)
{
	// STUB
}

/*
 src のエンコーディングを変換して dst に投入します。
*/
void
audio_track_enconvert(audio_track_t *track, audio_filter_t filter, audio_ring_t *dst, audio_ring_t *src)
{
	KASSERT(track != NULL);
	KASSERT(filter != NULL);
	KASSERT(is_valid_ring(dst));
	KASSERT(is_valid_ring(src));

}

void
audio_track_channel_mix(audio_track_t *track, audio_ring_t *dst, audio_ring_t *src)
{
#if false
	// 残骸。またあとで考えるかも
		case AUDIO_TRACK_CHANNEL_MIXALL:
			for (int i = 0; i < slice_count; i++) {
				internal2_t s = 0;
				for (int ch = 0; ch < src->fmt->channels; ch++, dptr++) {
					s += (internal2_t)dptr[ch];
				}
				*dptr = s / src->fmt->channels;
				dptr++;
			}
			break;

		case AUDIO_TRACK_CHANNEL_DUPALL:
			for (int i = 0; i < slice_count; i++) {
				for (int ch = 0; ch < dst->fmt->channels; ch++) {
					*dptr = dptr[0];
					dptr++;
				}
				dptr++;
			}
			break;
#endif
}

void
audio_track_freq(audio_track_t *track, audio_ring_t *dst, audio_ring_t *src)
{
	KASSERT(track);
	KASSERT(is_valid_ring(dst));
	KASSERT(is_valid_ring(src));
	KASSERT(src->count > 0);
	KASSERT(src->fmt->channels == dst->fmt->channels);

	int count = audio_ring_unround_free_count(dst);
	
	if (count <= 0) {
		return;
//		panic("not impl");
	}
	
	// 単純法
	// XXX: 高速化が必要
	internal_t *dptr = RING_BOT(internal_t, dst);
	for (int i = 0; i < count; i++) {
		if (src->count <= 0) break;

		internal_t *sptr = RING_TOP(internal_t, src);
		for (int ch = 0; ch < dst->fmt->channels; ch++, dptr++, sptr++) {
			*dptr = *sptr;
		}
		
		audio_rational_add(&track->freq_current, &track->freq_step, dst->fmt->frequency);
		audio_ring_tookfromtop(src, track->freq_current.i);
		track->freq_current.i = 0;
		audio_ring_appended(dst, 1);
	}
}

/*
 * 再生時の入力データを変換してトラックバッファに投入します。
 */
void
audio_track_play(audio_track_t *track)
{
	KASSERT(track);

	int track_count_0 = track->track_buf.count;

	/* エンコーディング変換 */
	if (track->codec != NULL) {
		int dst_count = audio_ring_unround_free_count(track->codec_out);
		if (audio_ring_unround_free_count(track->codec_out) > 0) {
			// stride に応じてアラインする最小ブロックまでを処理する
			int count = track->userio_frames_per_block * track->framealign;
			count = min(count, track->codec_in->count);
			count = min(count, dst_count);

			// フレームのアライメント位置まで切り捨てる
			count = count & ~(track->framealign - 1);

			if (count > 0) {
				audio_filter_arg_t *arg = &track->codec_arg;

				arg->dst = RING_BOT_UINT8(track->codec_out);
				arg->src = RING_TOP_UINT8(track->codec_in);
				arg->count = count;

				track->codec(arg);

				audio_ring_tookfromtop(track->codec_in, count);
				audio_ring_appended(track->codec_out, count);
			}
		}
	}

	if (track->codec_out->count == 0) return;

	/* ブロックサイズに整形 */
	int n = track->userio_frames_per_block - track->codec_out->count;
	if (n > 0) {
		if (track->is_draining) {
			/* TODO: ドレインの条件を、ユーザ指定ドレインとミキサ要求ドレインで分離することになりそう。*/
			/* 無音をブロックサイズまで埋める */
			/* 内部フォーマットだとわかっている */
			/* ここは入力がブロックサイズ未満の端数だった場合。次段以降がブロックサイズを想定しているので、ここでまず追加。*/
			TRACE(track, "Append silence %d frames", n);
			KASSERT(audio_ring_unround_free_count(track->codec_out) >= n);

			memset(RING_BOT_UINT8(track->codec_out), 0, n * track->codec_out->fmt->channels * sizeof(internal_t));
			audio_ring_appended(track->codec_out, n);
		} else {
			// ブロックサイズたまるまで処理をしない
			return;
		}
	}

	KASSERT(track->codec_out->count >= track->userio_frames_per_block);

	/* チャンネルボリューム */
	if (track->chvol != NULL
	 && audio_ring_unround_count(track->chvol_in) >= track->chvol_arg.count
	 && audio_ring_unround_free_count(track->chvol_out) >= track->chvol_arg.count) {
		track->chvol_arg.src = RING_TOP(internal_t, track->chvol_in);
		track->chvol_arg.dst = RING_BOT(internal_t, track->chvol_out);
		track->chvol(&track->chvol_arg);
		audio_ring_appended(track->chvol_out, track->chvol_arg.count);
		audio_ring_tookfromtop(track->chvol_in, track->chvol_arg.count);
	}

	/* チャンネルミキサ */
	if (track->chmix != NULL
	 && audio_ring_unround_count(track->chmix_in) >= track->chmix_arg.count
	 && audio_ring_unround_free_count(track->chmix_out) >= track->chmix_arg.count) {
		track->chmix_arg.src = RING_TOP(internal_t, track->chmix_in);
		track->chmix_arg.dst = RING_BOT(internal_t, track->chmix_out);
		track->chmix(&track->chmix_arg);
		audio_ring_appended(track->chmix_out, track->chmix_arg.count);
		audio_ring_tookfromtop(track->chmix_in, track->chmix_arg.count);
	}

	/* 周波数変換 */
	if (track->freq_in->fmt->frequency != track->freq_out->fmt->frequency) {
		audio_track_freq(track, track->freq_out, track->freq_in);
	} else {
		if (track->freq_in != track->freq_out) {
			audio_ring_concat(track->freq_out, track->freq_in, track->mixer->frames_per_block);
		}
	}

	if (track->is_draining) {
		/* TODO: ドレインの条件を、ユーザ指定ドレインとミキサ要求ドレインで分離することになりそう。*/
		/* 無音をブロックサイズまで埋める */
		/* 内部フォーマットだとわかっている */
		/* 周波数変換の結果、ブロック未満の端数フレームが出る */
		int n = track->track_buf.count % track->mixer->frames_per_block;
		if (n > 0) {
			n = track->mixer->frames_per_block - n;
			TRACE(track, "Append silence %d frames to track_buf", n);
			KASSERT(audio_ring_unround_free_count(&track->track_buf) >= n);

			memset(RING_BOT_UINT8(&track->track_buf), 0, n * track->track_buf.fmt->channels * sizeof(internal_t));
			audio_ring_appended(&track->track_buf, n);
		}
	}

	if (track->userio_inout == &track->track_buf) {
		track->track_counter = track->userio_counter;
	} else {
		track->track_counter += track->track_buf.count - track_count_0;
	}

	TRACE(track, "trackbuf=%d", track->track_buf.count);
	audio_mixer_play(track->mixer);
}

void
audio_mixer_init(audio_trackmixer_t *mixer, struct audio_softc *sc, int mode)
{
	TRACE0("");
	memset(mixer, 0, sizeof(audio_trackmixer_t));
	mixer->sc = sc;

	mixer->hw_fmt = audio_softc_get_hw_format(mixer->sc, mode);
	mixer->hw_buf.fmt = &mixer->hw_fmt;
	mixer->hw_buf.capacity = audio_softc_get_hw_capacity(mixer->sc);
	mixer->hw_buf.sample = audio_softc_allocm(mixer->sc, RING_BYTELEN(&mixer->hw_buf));

	mixer->frames_per_block = mixer->hw_fmt.frequency * AUDIO_BLK_MS / 1000;

	mixer->track_fmt.encoding = AUDIO_ENCODING_SLINEAR_HE;
	mixer->track_fmt.channels = mixer->hw_fmt.channels;
	mixer->track_fmt.frequency = mixer->hw_fmt.frequency;
	mixer->track_fmt.precision = mixer->track_fmt.stride = AUDIO_INTERNAL_BITS;

	mixer->mix_fmt = mixer->track_fmt;
	mixer->mix_fmt.precision = mixer->mix_fmt.stride = AUDIO_INTERNAL_BITS * 2;

	/* 40ms double buffer */
	mixer->mix_buf.fmt = &mixer->mix_fmt;
	mixer->mix_buf.capacity = 2 * mixer->frames_per_block;
	mixer->mix_buf.sample = audio_realloc(mixer->mix_buf.sample, RING_BYTELEN(&mixer->mix_buf));
	memset(mixer->mix_buf.sample, 0, RING_BYTELEN(&mixer->mix_buf));

	mixer->volume = 256;
}


/*
 * トラックバッファから 最大 1 ブロックを取り出し、
 * ミキシングして、ハードウェアに再生を通知します。
 */
void
audio_mixer_play(audio_trackmixer_t *mixer)
{
	TRACE0("");
	/* 全部のトラックに聞く */

	audio_file_t *f;
	int track_count = 0;
	int track_ready = 0;
	int mixed = 0;
	SLIST_FOREACH(f, &mixer->sc->sc_files, entry) {
		track_count++;
		audio_track_t *track = &f->ptrack;

		// 合成
		if (track->track_buf.count > 0) {
			audio_mixer_play_mix_track(mixer, track);
		}

		if (track->is_draining
			|| track->mixed_count >= mixer->frames_per_block) {
			track_ready++;
		}

		if (track->mixed_count != 0) {
			if (mixed == 0) {
				mixed = track->mixed_count;
			} else {
				mixed = min(mixed, track->mixed_count);
			}
		}
	}

	mixer->mix_buf.count = mixed;

	/* 全員準備できたか、時間切れならハードウェアに転送 */
	if (track_ready == track_count
	|| audio_softc_play_busy(mixer->sc) == false) {
		audio_mixer_play_period(mixer);
	}
}

/*
* トラックバッファから取り出し、ミキシングします。
*/
void
audio_mixer_play_mix_track(audio_trackmixer_t *mixer, audio_track_t *track)
{
	int count = track->track_buf.count;

	audio_ring_t mix_tmp;
	mix_tmp = mixer->mix_buf;
	mix_tmp.count = track->mixed_count;

	/* 1 ブロック貯まるまで待つ */
	if (count < mixer->frames_per_block) return;
	count = mixer->frames_per_block;

	if (mix_tmp.capacity - mix_tmp.count < count) {
		TRACE(track, "mix_buf full");
		return;
	}

	KASSERT(audio_ring_unround_count(&track->track_buf) >= count);
	KASSERT(audio_ring_unround_free_count(&mix_tmp) >= count);

	internal_t *sptr = RING_TOP(internal_t, &track->track_buf);
	internal2_t *dptr = RING_BOT(internal2_t, &mix_tmp);

	/* 整数倍精度へ変換し、トラックボリュームを適用して加算合成 */
	int sample_count = count * mixer->mix_fmt.channels;
	if (track->volume == 256) {
		for (int i = 0; i < sample_count; i++) {
			*dptr++ += ((internal2_t)*sptr++);
		}
	} else {
		for (int i = 0; i < sample_count; i++) {
			*dptr++ += ((internal2_t)*sptr++) * track->volume / 256;
		}
	}

	audio_ring_tookfromtop(&track->track_buf, count);

	/* トラックバッファを取り込んだことを反映 */
	track->mixed_count += count;
	TRACE(track, "mixed+=%d", count);
}

/*
 * ミキシングバッファから物理デバイスバッファへ
 */
void
audio_mixer_play_period(audio_trackmixer_t *mixer /*, bool force */)
{
	/* 今回取り出すフレーム数を決定 */

	int mix_count = audio_ring_unround_count(&mixer->mix_buf);
	int hw_free_count = audio_ring_unround_free_count(&mixer->hw_buf);
	int count = min(mix_count, hw_free_count);
	if (count <= 0) {
		TRACE0("count too short: mix_count=%d hw_free=%d", mix_count, hw_free_count);
		return;
	}
	count = min(count, mixer->frames_per_block);

	/* オーバーフロー検出 */
	internal2_t ovf_plus = AUDIO_INTERNAL_T_MAX;
	internal2_t ovf_minus = AUDIO_INTERNAL_T_MIN;

	internal2_t *mptr0 = RING_TOP(internal2_t, &mixer->mix_buf);
	internal2_t *mptr = mptr0;

	int sample_count = count * mixer->mix_fmt.channels;
	for (int i = 0; i < sample_count; i++) {
		if (*mptr > ovf_plus) ovf_plus = *mptr;
		if (*mptr < ovf_minus) ovf_minus = *mptr;

		mptr++;
	}

	/* マスタボリュームの自動制御 */
	int vol = mixer->volume;
	if (ovf_plus * vol / 256 > AUDIO_INTERNAL_T_MAX) {
		/* オーバーフローしてたら少なくとも今回はボリュームを下げる */
		vol = (int)((internal2_t)AUDIO_INTERNAL_T_MAX * 256 / ovf_plus);
	}
	if (ovf_minus * vol / 256 < AUDIO_INTERNAL_T_MIN) {
		vol = (int)((internal2_t)AUDIO_INTERNAL_T_MIN * 256 / ovf_minus);
	}
	if (vol < mixer->volume) {
		/* 128 までは自動でマスタボリュームを下げる */
		if (mixer->volume > 128) {
mixer->volume--;
		}
	}

	/* ここから ハードウェアチャンネル */

	/* マスタボリューム適用 */
	if (vol != 256) {
		mptr = mptr0;
		for (int i = 0; i < sample_count; i++) {
			*mptr = *mptr * vol / 256;
			mptr++;
		}
	}

	/* ハードウェアバッファへ転送 */
	lock(mixer->sc);
	mptr = mptr0;
	internal_t *hptr = RING_BOT(internal_t, &mixer->hw_buf);
	for (int i = 0; i < sample_count; i++) {
		*hptr++ = *mptr++;
	}
	audio_ring_appended(&mixer->hw_buf, count);
	unlock(mixer->sc);

	/* 使用済みミキサメモリを次回のために 0 フィルする */
	memset(mptr0, 0, sample_count * sizeof(internal2_t));
	audio_ring_tookfromtop(&mixer->mix_buf, count);

	/* トラックにハードウェアへ転送されたことを通知する */
	audio_file_t *f;
	SLIST_FOREACH(f, &mixer->sc->sc_files, entry) {
		audio_track_t *track = &f->ptrack;
		if (track->mixed_count > 0) {
			KASSERT(track->completion_blkcount < _countof(track->completion_blkID));

			track->completion_blkID[track->completion_blkcount] = mixer->hw_blkID;
			track->completion_blkcount++;

			if (track->mixed_count <= count) {
				/* 要求転送量が全部転送されている */
				track->mixer_hw_counter += track->mixed_count;
				track->mixed_count = 0;
			} else {
				/* のこりがある */
				track->mixer_hw_counter += count;
				track->mixed_count -= count;
			}
		}
	}

	/* ハードウェアへ通知する */
	TRACE0("start count=%d blkid=%d", count, mixer->hw_blkID);
	audio_softc_play_start(mixer->sc);
	mixer->hw_output_counter += count;

	// この blkID の出力は終わり。次。
	mixer->hw_blkID++;
}

void
audio_trackmixer_intr(audio_trackmixer_t *mixer, int count)
{
	TRACE0("");
	KASSERT(count != 0);

	mixer->hw_complete_counter += count;

	/* トラックにハードウェア出力が完了したことを通知する */
	audio_file_t *f;
	SLIST_FOREACH(f, &mixer->sc->sc_files, entry) {
		audio_track_t *track = &f->ptrack;
		if (track->completion_blkcount > 0) {
			if (track->completion_blkID[0] < mixer->hw_cmplID) {
				panic("missing block");
			}

			if (track->completion_blkID[0] == mixer->hw_cmplID) {
				track->hw_complete_counter += mixer->frames_per_block;
				// キューは小さいのでポインタをごそごそするより速いんではないか
				for (int i = 0; i < track->completion_blkcount - 1; i++) {
					track->completion_blkID[i] = track->completion_blkID[i + 1];
				}
				track->completion_blkcount--;
			}
		}
	}
	mixer->hw_cmplID++;

#if !false
	/* XXX win32 は割り込みから再生 API をコール出来ないので、ポーリングする */
	if (audio_softc_play_busy(mixer->sc) == false) {
		audio_softc_play_start(mixer->sc);
	}
#endif
	audio_mixer_play(mixer, true);
}

#ifdef AUDIO_INTR_EMULATED
void
audio_track_play_drain(audio_track_t *track)
{
	// 割り込みエミュレートしているときはメインループに制御を戻さないといけない
	audio_track_play_drain_core(track, false);
}
#else
void
audio_track_play_drain(audio_track_t *track)
{
	audio_track_play_drain_core(track, true);
}
#endif

void
audio_track_play_drain_core(audio_track_t *track, bool wait)
{
	TRACE(track, "");
	track->is_draining = true;

	// 無音挿入させる
	audio_track_play(track);

	/* フレームサイズ未満のため待たされていたデータを破棄 */
	track->subframe_buf_used = 0;

	/* userio_buf は待つ必要はない */
	/* chmix_buf は待つ必要はない */
	if (wait) {
		do {
			audio_waitio(track->mixer->sc, NULL, track);
			//audio_mixer_play(track->mixer);
		} while (track->track_buf.count > 0
			|| track->mixed_count > 0
			|| track->completion_blkcount > 0);

		track->is_draining = false;
		printf("#%d: uio_count=%lld trk_count=%lld tm=%lld mixhw=%lld hw_complete=%lld\n", track->id,
			track->userio_counter, track->track_counter, 
			track->track_mixer_counter, track->mixer_hw_counter,
			track->hw_complete_counter);
	}
}

/* write の MI 側 */
int
audio_write(struct audio_softc *sc, struct uio *uio, int ioflag, audio_file_t *file)
{
	int error;
	audio_track_t *track = &file->ptrack;
	TRACE(track, "");

	while (uio->uio_resid > 0) {

		/* userio の空きバイト数を求める */
		int free_count = audio_ring_unround_free_count(&track->userio_buf);
		int free_bytelen = free_count * track->userio_fmt.channels * track->userio_fmt.stride / 8 - track->subframe_buf_used;

		if (free_bytelen == 0) {
			audio_waitio(sc, NULL, track);
		}

		// 今回 uiomove するバイト数 */
		int move_bytelen = min(free_bytelen, (int)uio->uio_resid);

		// 今回出来上がるフレーム数 */
		int framecount = (move_bytelen + track->subframe_buf_used) * 8 / (track->userio_fmt.channels * track->userio_fmt.stride);

		// コピー先アドレスは subframe_buf_used で調整する必要がある
		uint8_t *dptr = RING_BOT_UINT8(&track->userio_buf) + track->subframe_buf_used;
		// min(bytelen, uio->uio_resid) は uiomove が保証している
		error = uiomove(dptr, move_bytelen, uio);
		if (error) {
			panic("uiomove");
		}
		audio_ring_appended(&track->userio_buf, framecount);
		track->userio_counter += framecount;
		
		// 今回 userio_buf に置いたサブフレームを次回のために求める
		track->subframe_buf_used = move_bytelen - framecount * track->userio_fmt.channels * track->userio_fmt.stride / 8;

		// 今回作った userio を全部トラック再生へ渡す
		audio_track_play(track);
	}

	return 0;
}

static int
audio_waitio(struct audio_softc *sc, void *kcondvar, audio_track_t *track)
{
	// 本当は割り込みハンドラからトラックが消費されるんだけど
	// ここで消費をエミュレート。

//	TRACE0("");

	emu_intr_check();

#if false
	/* 全部のトラックに聞く */

	audio_file_t *f;
	SLIST_FOREACH(f, &sc->sc_files, entry) {
		audio_track_t *ptrack = &f->ptrack;

		audio_track_play(ptrack);
	}
#endif
	return 0;
}

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

	int error = audio_write(file->sc, &uio, 0, file);
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
		audio_track_init(&file->ptrack, &sc->sc_pmixer);
	} else {
		audio_track_init(&file->rtrack, &sc->sc_rmixer);
	}

	SLIST_INSERT_HEAD(&sc->sc_files, file, entry);

	return file;
}
