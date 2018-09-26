/*	$NetBSD: msm6258.c,v 1.25 2018/09/03 16:29:31 riastradh Exp $	*/

/*
 * Copyright (c) 2001 Tetsuya Isaki. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * OKI MSM6258 ADPCM voice synthesizer codec.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: msm6258.c,v 1.25 2018/09/03 16:29:31 riastradh Exp $");

#include <sys/systm.h>
#include <sys/device.h>
#include <sys/kmem.h>
#include <sys/select.h>
#include <sys/audioio.h>

#include <dev/audio_if.h>
#include <dev/audiovar.h>
#include <dev/ic/msm6258var.h>

#if !defined(AUDIO2)
struct msm6258_codecvar {
	stream_filter_t	base;
	short		mc_amp;
	char		mc_estim;
};

static stream_filter_t *msm6258_factory
	(struct audio_softc *,
	 int (*)(struct audio_softc *, stream_fetcher_t *, audio_stream_t *, int));
static void msm6258_dtor(struct stream_filter *);
#endif /* !AUDIO2 */
static inline uint8_t	pcm2adpcm_step(struct msm6258_codecvar *, int16_t);
static inline int16_t	adpcm2pcm_step(struct msm6258_codecvar *, uint8_t);

static const int8_t adpcm_estimindex[16] = {
	 2,  6,  10,  14,  18,  22,  26,  30,
	-2, -6, -10, -14, -18, -22, -26, -30
};

static const int16_t adpcm_estim[49] = {
	 16,  17,  19,  21,  23,  25,  28,  31,  34,  37,
	 41,  45,  50,  55,  60,  66,  73,  80,  88,  97,
	107, 118, 130, 143, 157, 173, 190, 209, 230, 253,
	279, 307, 337, 371, 408, 449, 494, 544, 598, 658,
	724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552
};

static const int8_t adpcm_estimstep[16] = {
	-1, -1, -1, -1, 2, 4, 6, 8,
	-1, -1, -1, -1, 2, 4, 6, 8
};

#if !defined(AUDIO2)
static int16_t buzzer;	/* sound for debug */

static stream_filter_t *
msm6258_factory(struct audio_softc *asc,
    int (*fetch_to)(struct audio_softc *, stream_fetcher_t *, audio_stream_t *, int))
{
	struct msm6258_codecvar *this;

	this = kmem_alloc(sizeof(struct msm6258_codecvar), KM_SLEEP);
	this->base.base.fetch_to = fetch_to;
	this->base.dtor = msm6258_dtor;
	this->base.set_fetcher = stream_filter_set_fetcher;
	this->base.set_inputbuffer = stream_filter_set_inputbuffer;
	return &this->base;
}

static void
msm6258_dtor(struct stream_filter *this)
{
	if (this != NULL)
		kmem_free(this, sizeof(struct msm6258_codecvar));
}
#endif /* !AUDIO2 */

/*
 * signed 16bit linear PCM -> OkiADPCM
 */
static inline uint8_t
pcm2adpcm_step(struct msm6258_codecvar *mc, int16_t a)
{
	int estim = (int)mc->mc_estim;
	int32_t ea;
	int32_t df;
	int16_t dl, c;
	uint8_t b;
	uint8_t s;

	df = (int32_t)a - (int32_t)mc->mc_amp;
	dl = adpcm_estim[estim];
	c = (df / 16) * 8 / dl;
	if (df < 0) {
		b = (uint8_t)(-c) / 2;
		s = 0x08;
	} else {
		b = (uint8_t)(c) / 2;
		s = 0;
	}
	if (b > 7)
		b = 7;
	s |= b;
	ea = mc->mc_amp + adpcm_estimindex[s] * dl;
	if (ea > 32767) {
		ea = 32767;
	} else if (ea < -32768) {
		ea = -32768;
	}
	mc->mc_amp = ea;
	estim += adpcm_estimstep[b];
	if (estim < 0)
		estim = 0;
	else if (estim > 48)
		estim = 48;

	mc->mc_estim = estim;
	return s;
}

#if defined(AUDIO2)
void
internal_to_msm6258(audio_filter_arg_t *arg)
{
	KASSERT(arg->dstfmt->encoding == AUDIO_ENCODING_ADPCM);
	KASSERT(arg->dstfmt->stride == 4);
	KASSERT(is_internal_format(arg->srcfmt));
	KASSERT(arg->srcfmt->channels == arg->dstfmt->channels);
	KASSERT((arg->count & 1) == 0);

	const aint_t *sptr = arg->src;
	uint8_t *dptr = arg->dst;
	int sample_count = arg->count * arg->srcfmt->channels;

	struct msm6258_codecvar *mc = arg->context;

	for (int i = 0; i < sample_count / 2; i++) {
		aint_t s;
		uint8_t f;

		s = *sptr++;
		s >>= AUDIO_INTERNAL_BITS - 16;
		f = pcm2adpcm_step(mc, s);

		s = *sptr++;
		s >>= AUDIO_INTERNAL_BITS - 16;
		f |= pcm2adpcm_step(mc, s) << 4;

		*dptr++ = (uint8_t)f;
	}
}
#else

#define DEFINE_FILTER(name)	\
static int \
name##_fetch_to(struct audio_softc *, stream_fetcher_t *, audio_stream_t *, int); \
stream_filter_t * \
name(struct audio_softc *sc, const audio_params_t *from, \
     const audio_params_t *to) \
{ \
	return msm6258_factory(sc, name##_fetch_to); \
} \
static int \
name##_fetch_to(struct audio_softc *asc, stream_fetcher_t *self, audio_stream_t *dst, int max_used)

DEFINE_FILTER(msm6258_slinear16_to_adpcm)
{
	stream_filter_t *this;
	struct msm6258_codecvar *mc;
	uint8_t *d;
	const uint8_t *s;
	int m, err, enc_src;

	this = (stream_filter_t *)self;
	mc = (struct msm6258_codecvar *)self;
	if ((err = this->prev->fetch_to(asc, this->prev, this->src, max_used * 4)))
		return err;
	m = dst->end - dst->start;
	m = uimin(m, max_used);
	d = dst->inp;
	s = this->src->outp;
	enc_src = this->src->param.encoding;
	if (enc_src == AUDIO_ENCODING_SLINEAR_LE) {
		while (dst->used < m && this->src->used >= 4) {
			uint8_t f;
			int16_t ss;
			ss = le16toh(*(const int16_t*)s);
			f  = pcm2adpcm_step(mc, ss);
			s = audio_stream_add_outp(this->src, s, 2);
			ss = le16toh(*(const int16_t*)s);
			f |= pcm2adpcm_step(mc, ss) << 4;
			s = audio_stream_add_outp(this->src, s, 2);
			*d = f;
			d = audio_stream_add_inp(dst, d, 1);
		}
	} else if (enc_src == AUDIO_ENCODING_SLINEAR_BE) {
		while (dst->used < m && this->src->used >= 4) {
			uint8_t f;
			int16_t ss;
			ss = be16toh(*(const int16_t*)s);
			s = audio_stream_add_outp(this->src, s, 2);
			f  = pcm2adpcm_step(mc, ss);
			ss = be16toh(*(const int16_t*)s);
			s = audio_stream_add_outp(this->src, s, 2);
			f |= pcm2adpcm_step(mc, ss) << 4;
			*d = f;
			d = audio_stream_add_inp(dst, d, 1);
		}
	} else {
#if defined(DIAGNOSTIC)
		panic("msm6258_slinear16_to_adpcm: unsupported enc_src(%d)", enc_src);
#endif
		/* dummy run */
		while (dst->used < m && this->src->used >= 4) {
			s = audio_stream_add_outp(this->src, s, 2);
			s = audio_stream_add_outp(this->src, s, 2);
			*d = buzzer++;
			d = audio_stream_add_inp(dst, d, 1);
		}
	}
	dst->inp = d;
	this->src->outp = s;
	return 0;
}

DEFINE_FILTER(msm6258_linear8_to_adpcm)
{
	stream_filter_t *this;
	struct msm6258_codecvar *mc;
	uint8_t *d;
	const uint8_t *s;
	int m, err, enc_src;

	this = (stream_filter_t *)self;
	mc = (struct msm6258_codecvar *)self;
	if ((err = this->prev->fetch_to(asc, this->prev, this->src, max_used * 2)))
		return err;
	m = dst->end - dst->start;
	m = uimin(m, max_used);
	d = dst->inp;
	s = this->src->outp;
	enc_src = this->src->param.encoding;
	if (enc_src == AUDIO_ENCODING_SLINEAR_LE
	 || enc_src == AUDIO_ENCODING_SLINEAR_BE) {
		while (dst->used < m && this->src->used >= 4) {
			uint8_t f;
			int16_t ss;
			ss = ((int16_t)s[0]) * 256;
			s = audio_stream_add_outp(this->src, s, 1);
			f  = pcm2adpcm_step(mc, ss);
			ss = ((int16_t)s[0]) * 256;
			s = audio_stream_add_outp(this->src, s, 1);
			f |= pcm2adpcm_step(mc, ss) << 4;
			*d = f;
			d = audio_stream_add_inp(dst, d, 1);
		}
	} else if (enc_src == AUDIO_ENCODING_ULINEAR_LE
	        || enc_src == AUDIO_ENCODING_ULINEAR_BE) {
		while (dst->used < m && this->src->used >= 4) {
			uint8_t f;
			int16_t ss;
			ss = ((int16_t)(s[0] ^ 0x80)) * 256;
			s = audio_stream_add_outp(this->src, s, 1);
			f  = pcm2adpcm_step(mc, ss);
			ss = ((int16_t)(s[0] ^ 0x80)) * 256;
			s = audio_stream_add_outp(this->src, s, 1);
			f |= pcm2adpcm_step(mc, ss) << 4;
			*d = f;
			d = audio_stream_add_inp(dst, d, 1);
		}
	} else {
#if defined(DIAGNOSTIC)
		panic("msm6258_linear8_to_adpcm: unsupported enc_src(%d)", enc_src);
#endif
		/* dummy run */
		while (dst->used < m && this->src->used >= 4) {
			s = audio_stream_add_outp(this->src, s, 1);
			s = audio_stream_add_outp(this->src, s, 1);
			*d = buzzer++;
			d = audio_stream_add_inp(dst, d, 1);
		}
	}
	dst->inp = d;
	this->src->outp = s;
	return 0;
}
#endif /* AUDIO2 */


/*
 * OkiADPCM -> signed 16bit linear PCM
 */
static inline int16_t
adpcm2pcm_step(struct msm6258_codecvar *mc, uint8_t b)
{
	int estim = (int)mc->mc_estim;
	KASSERT(estim >= 0);
	KASSERT(estim < 49);
	KASSERT(b < 16);

	int d = adpcm_estim[estim] * adpcm_estimindex[b];
	int a = mc->mc_amp + d;
	if (a > 32767) {
		a = 32767;
	} else if (a < -32768) {
		a = -32768;
	}
	mc->mc_amp = a;
	estim += adpcm_estimstep[b];

	if (estim < 0)
		estim = 0;
	else if (estim > 48)
		estim = 48;

	mc->mc_estim = estim;

	return mc->mc_amp;
}

#if defined(AUDIO2)
void
msm6258_to_internal(audio_filter_arg_t *arg)
{
	KASSERT(arg->srcfmt->encoding == AUDIO_ENCODING_ADPCM);
	KASSERT(arg->srcfmt->stride == 4);
	KASSERT(is_internal_format(arg->dstfmt));
	KASSERT(arg->srcfmt->channels == arg->dstfmt->channels);
	KASSERT((arg->count & 1) == 0);

	const uint8_t *sptr = arg->src;
	aint_t *dptr = arg->dst;
	int sample_count = arg->count * arg->srcfmt->channels;

	struct msm6258_codecvar *mc = arg->context;

	for (int i = 0; i < sample_count / 2; i++) {
		uint8_t a = *sptr++;
		aint_t s;

		s = adpcm2pcm_step(mc, a & 0x0f);
		s <<= AUDIO_INTERNAL_BITS - 16;
		*dptr++ = s;

		s = adpcm2pcm_step(mc, a >> 4);
		s <<= AUDIO_INTERNAL_BITS - 16;
		*dptr++ = s;
	}
}
#else

DEFINE_FILTER(msm6258_adpcm_to_slinear16)
{
	stream_filter_t *this;
	struct msm6258_codecvar *mc;
	uint8_t *d;
	const uint8_t *s;
	int m, err, enc_dst;

	this = (stream_filter_t *)self;
	mc = (struct msm6258_codecvar *)self;
	max_used = (max_used + 3) & ~3; /* round up multiple of 4 */
	if ((err = this->prev->fetch_to(asc, this->prev, this->src, max_used / 4)))
		return err;
	m = (dst->end - dst->start) & ~3;
	m = uimin(m, max_used);
	d = dst->inp;
	s = this->src->outp;
	enc_dst = dst->param.encoding;
	if (enc_dst == AUDIO_ENCODING_SLINEAR_LE) {
		while (dst->used < m && this->src->used >= 1) {
			uint8_t a;
			int16_t s1, s2;
			a = s[0];
			s1 = adpcm2pcm_step(mc, a & 0x0f);
			s2 = adpcm2pcm_step(mc, a >> 4);
			*(int16_t*)d = htole16(s1);
			d = audio_stream_add_inp(dst, d, 2);
			*(int16_t*)d = htole16(s2);
			d = audio_stream_add_inp(dst, d, 2);
			s = audio_stream_add_outp(this->src, s, 1);
		}
	} else if (enc_dst == AUDIO_ENCODING_SLINEAR_BE) {
		while (dst->used < m && this->src->used >= 1) {
			uint8_t a;
			int16_t s1, s2;
			a = s[0];
			s1 = adpcm2pcm_step(mc, a & 0x0f);
			s2 = adpcm2pcm_step(mc, a >> 4);
			*(int16_t*)d = htobe16(s1);
			d = audio_stream_add_inp(dst, d, 2);
			*(int16_t*)d = htobe16(s2);
			d = audio_stream_add_inp(dst, d, 2);
			s = audio_stream_add_outp(this->src, s, 1);
		}
	} else {
#if defined(DIAGNOSTIC)
		panic("msm6258_adpcm_to_slinear16: unsupported enc_dst(%d)", enc_dst);
#endif
		/* dummy run */
		while (dst->used < m && this->src->used >= 1) {
			*d = buzzer++;
			d = audio_stream_add_inp(dst, d, 2);
			*d = buzzer++;
			d = audio_stream_add_inp(dst, d, 2);
			s = audio_stream_add_outp(this->src, s, 1);
		}
	}
	dst->inp = d;
	this->src->outp = s;
	return 0;
}

DEFINE_FILTER(msm6258_adpcm_to_linear8)
{
	stream_filter_t *this;
	struct msm6258_codecvar *mc;
	uint8_t *d;
	const uint8_t *s;
	int m, err, enc_dst;

	this = (stream_filter_t *)self;
	mc = (struct msm6258_codecvar *)self;
	max_used = (max_used + 1) & ~1; /* round up multiple of 4 */
	if ((err = this->prev->fetch_to(asc, this->prev, this->src, max_used / 2)))
		return err;
	m = (dst->end - dst->start) & ~1;
	m = uimin(m, max_used);
	d = dst->inp;
	s = this->src->outp;
	enc_dst = dst->param.encoding;
	if (enc_dst == AUDIO_ENCODING_SLINEAR_LE) {
		while (dst->used < m && this->src->used >= 1) {
			uint8_t a;
			int16_t s1, s2;
			a = s[0];
			s = audio_stream_add_outp(this->src, s, 1);
			s1 = adpcm2pcm_step(mc, a & 0x0f);
			s2 = adpcm2pcm_step(mc, a >> 4);
			d[0] = s1 / 256;
			d = audio_stream_add_inp(dst, d, 1);
			d[0] = s2 / 256;
			d = audio_stream_add_inp(dst, d, 1);
		}
	} else if (enc_dst == AUDIO_ENCODING_ULINEAR_LE) {
		while (dst->used < m && this->src->used >= 1) {
			uint8_t a;
			int16_t s1, s2;
			a = s[0];
			s = audio_stream_add_outp(this->src, s, 1);
			s1 = adpcm2pcm_step(mc, a & 0x0f);
			s2 = adpcm2pcm_step(mc, a >> 4);
			d[0] = (s1 / 256) ^ 0x80;
			d = audio_stream_add_inp(dst, d, 1);
			d[0] = (s2 / 256) ^ 0x80;
			d = audio_stream_add_inp(dst, d, 1);
		}
	} else {
#if defined(DIAGNOSTIC)
		panic("msm6258_adpcm_to_linear8: unsupported enc_dst(%d)", enc_dst);
#endif
		/* dummy run */
		while (dst->used < m && this->src->used >= 1) {
			*d = buzzer++;
			d = audio_stream_add_inp(dst, d, 1);
			*d = buzzer++;
			d = audio_stream_add_inp(dst, d, 1);
			s = audio_stream_add_outp(this->src, s, 1);
		}
	}
	dst->inp = d;
	this->src->outp = s;
	return 0;
}
#endif /* AUDIO2 */
