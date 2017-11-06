/*	$NetBSD: msm6258.c,v 1.24 2017/09/02 12:57:35 isaki Exp $	*/

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


#include <stdint.h>
#include <stdbool.h>
#include "compat.h"

#include "auring.h"
#include "auformat.h"
#include "aucodec.h"

struct msm6258_codecvar {
	int16_t		mc_amp;
	int8_t		mc_estim;

	// フレーム境界がバイト境界にないため、1 サンプルをここでキューする。
	// ドレイン時にはドロップされるが、許容する。
	internal_t pending_sample;
	bool is_pending;
};


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


void *
msm6258_context_create()
{
#if 0
	struct msm6258_codecvar *this;

	this = kmem_alloc(sizeof(struct msm6258_codecvar), KM_SLEEP);
	this->base.base.fetch_to = fetch_to;
	this->base.dtor = msm6258_dtor;
	this->base.set_fetcher = stream_filter_set_fetcher;
	this->base.set_inputbuffer = stream_filter_set_inputbuffer;
	return &this->base;
#endif
	void *rv = malloc(sizeof(struct msm6258_codecvar));
	memset(rv, 0, sizeof(struct msm6258_codecvar));
	return rv;
}

void
msm6258_context_destroy(void *context)
{
#if 0
	if (this != NULL)
		kmem_free(this, sizeof(struct msm6258_codecvar));
#endif
	free(context);
}



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

void
internal_to_msm6258(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));
	KASSERT(arg->dst_fmt->encoding == AUDIO_ENCODING_MSM6258);
	KASSERT(arg->dst_fmt->stride == 4);
	KASSERT(is_internal_format(arg->src_fmt));
	KASSERT(arg->src_fmt->channels == arg->dst_fmt->channels);
	KASSERT((arg->count & 1) == 0);

	const internal_t *sptr = arg->src;
	uint8_t *dptr = arg->dst;
	int sample_count = arg->count * arg->src_fmt->channels;

	struct msm6258_codecvar *mc = arg->context;

	for (int i = 0; i < sample_count / 2; i++) {
		internal_t s;
		uint8_t f;

		s = *sptr++;
#if AUDIO_INTERNAL_BITS == 32
		s >>= 16;
#endif
		f = pcm2adpcm_step(mc, s);

		s = *sptr++;
#if AUDIO_INTERNAL_BITS == 32
		s >>= 16;
#endif
		f |= pcm2adpcm_step(mc, s) << 4;

		*dptr++ = (uint8_t)f;
	}
}


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

void
msm6258_to_internal(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));
	KASSERT(arg->src_fmt->encoding == AUDIO_ENCODING_MSM6258);
	KASSERT(arg->src_fmt->stride == 4);
	KASSERT(is_internal_format(arg->dst_fmt));
	KASSERT(arg->src_fmt->channels == arg->dst_fmt->channels);
	KASSERT((arg->count & 1) == 0);

	const uint8_t *sptr = arg->src;
	internal_t *dptr = arg->dst;
	int sample_count = arg->count * arg->src_fmt->channels;

	struct msm6258_codecvar *mc = arg->context;

	for (int i = 0; i < sample_count / 2; i++) {
		uint8_t a = *sptr++;
		internal_t s;

		s = adpcm2pcm_step(mc, a & 0x0f);
#if AUDIO_INTERNAL_BITS == 32
		s <<= 16;
#endif
		*dptr++ = s;

		s = adpcm2pcm_step(mc, a >> 4);
#if AUDIO_INTERNAL_BITS == 32
		s <<= 16;
#endif
		*dptr++ = s;
	}
}

