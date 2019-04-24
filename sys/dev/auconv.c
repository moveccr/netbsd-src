/*	$NetBSD: auconv.c,v 1.37 2018/09/03 16:29:30 riastradh Exp $	*/

/*
 * Copyright (c) 1996 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the Computer Systems
 *	Engineering Group at Lawrence Berkeley Laboratory.
 * 4. Neither the name of the University nor of the Laboratory may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: auconv.c,v 1.37 2018/09/03 16:29:30 riastradh Exp $");

#include <sys/types.h>
#include <sys/audioio.h>
#include <sys/device.h>
#include <sys/errno.h>
#include <sys/malloc.h>
#include <sys/null.h>
#include <sys/systm.h>
#include <dev/audio_if.h>
#include <dev/auconv.h>
#include <machine/limits.h>
#ifndef _KERNEL
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#endif

/* #define AUCONV_DEBUG */
#ifdef AUCONV_DEBUG
# define DPRINTF(x)	printf x
#else
# define DPRINTF(x)
#endif

#ifdef AUCONV_DEBUG
static void auconv_dump_formats(const struct audio_format *, int);
#endif
static void auconv_dump_params(const audio_params_t *);
static int auconv_exact_match(const struct audio_format *, int, int,
			      const struct audio_params *);
static u_int auconv_normalize_encoding(u_int, u_int);
static int auconv_is_supported_rate(const struct audio_format *, u_int);
static int auconv_add_encoding(int, int, int, struct audio_encoding_set **,
			       int *);

#ifdef _KERNEL
#define AUCONV_MALLOC(size)	malloc(size, M_DEVBUF, M_NOWAIT)
#define AUCONV_REALLOC(p, size)	realloc(p, size, M_DEVBUF, M_NOWAIT)
#define AUCONV_FREE(p)		free(p, M_DEVBUF)
#else
#define AUCONV_MALLOC(size)	malloc(size)
#define AUCONV_REALLOC(p, size)	realloc(p, size)
#define AUCONV_FREE(p)		free(p)
#endif

struct audio_encoding_set {
	int size;
	audio_encoding_t items[1];
};
#define ENCODING_SET_SIZE(n)	(offsetof(struct audio_encoding_set, items) \
				+ sizeof(audio_encoding_t) * (n))

struct conv_table {
	u_int encoding;
	u_int validbits;
	u_int precision;
	stream_filter_factory_t *play_conv;
	stream_filter_factory_t *rec_conv;
};

#ifdef AUCONV_DEBUG
static const char *encoding_dbg_names[] = {
	"none", AudioEmulaw, AudioEalaw, "pcm16",
	"pcm8", AudioEadpcm, AudioEslinear_le, AudioEslinear_be,
	AudioEulinear_le, AudioEulinear_be,
	AudioEslinear, AudioEulinear,
	AudioEmpeg_l1_stream, AudioEmpeg_l1_packets,
	AudioEmpeg_l1_system, AudioEmpeg_l2_stream,
	AudioEmpeg_l2_packets, AudioEmpeg_l2_system,
	AudioEac3
};
#endif

/**
 * Set appropriate parameters in `param,' and return the index in
 * the hardware capability array `formats.'
 *
 * @param formats	[IN] An array of formats which a hardware can support.
 * @param nformats	[IN] The number of elements of the array.
 * @param mode		[IN] Either AUMODE_PLAY or AUMODE_RECORD.
 * @param param		[IN] Requested format.  param->sw_code may be set.
 * @param rateconv	[IN] true if aurateconv may be used.
 * @param list		[OUT] stream_filters required for param.
 * @return The index of selected audio_format entry.  -1 if the device
 *	can not support the specified param.
 */
/*
 * XXX AUDIO2
 * It is only for backward compatibility and should be removed.
 */
int
auconv_set_converter(const struct audio_format *formats, int nformats,
    int mode, const audio_params_t *param, int rateconv,
    stream_filter_list_t *list)
{
	int i;

	printf("NOTICE: %s is obsoleted in AUDIO2\n", __func__);

#ifdef AUCONV_DEBUG
	DPRINTF(("%s: ENTER rateconv=%d\n", __func__, rateconv));
	auconv_dump_formats(formats, nformats);
#endif

	/* check support by native format */
	i = auconv_exact_match(formats, nformats, mode, param);
	if (i >= 0) {
		DPRINTF(("%s: LEAVE with %d (exact)\n", __func__, i));
		return i;
	}

	DPRINTF(("%s: LEAVE with -1 (bottom)\n", __func__));
	return -1;
}

#ifdef AUCONV_DEBUG
static void
auconv_dump_formats(const struct audio_format *formats, int nformats)
{
	const struct audio_format *f;
	int i, j;

	for (i = 0; i < nformats; i++) {
		f = &formats[i];
		printf("[%2d]: mode=", i);
		if (!AUFMT_IS_VALID(f)) {
			printf("INVALID");
		} else if (f->mode == AUMODE_PLAY) {
			printf("PLAY");
		} else if (f->mode == AUMODE_RECORD) {
			printf("RECORD");
		} else if (f->mode == (AUMODE_PLAY | AUMODE_RECORD)) {
			printf("PLAY|RECORD");
		} else {
			printf("0x%x", f->mode);
		}
		printf(" enc=%s", encoding_dbg_names[f->encoding]);
		printf(" %u/%ubit", f->validbits, f->precision);
		printf(" %uch", f->channels);

		printf(" channel_mask=");
		if (f->channel_mask == AUFMT_MONAURAL) {
			printf("MONAURAL");
		} else if (f->channel_mask == AUFMT_STEREO) {
			printf("STEREO");
		} else if (f->channel_mask == AUFMT_SURROUND4) {
			printf("SURROUND4");
		} else if (f->channel_mask == AUFMT_DOLBY_5_1) {
			printf("DOLBY5.1");
		} else {
			printf("0x%x", f->channel_mask);
		}

		if (f->frequency_type == 0) {
			printf(" %uHz-%uHz", f->frequency[0],
			       f->frequency[1]);
		} else {
			printf(" %uHz", f->frequency[0]);
			for (j = 1; j < f->frequency_type; j++)
				printf(",%uHz", f->frequency[j]);
		}
		printf("\n");
	}
}

static void
auconv_dump_params(const audio_params_t *p)
{
	printf("enc=%s", encoding_dbg_names[p->encoding]);
	printf(" %u/%ubit", p->validbits, p->precision);
	printf(" %uch", p->channels);
	printf(" %uHz", p->sample_rate);
	printf("\n");
}
#else
static void
auconv_dump_params(const audio_params_t *p)
{
}
#endif /* AUCONV_DEBUG */

/**
 * a sub-routine for auconv_set_converter()
 */
static int
auconv_exact_match(const struct audio_format *formats, int nformats,
		   int mode, const audio_params_t *param)
{
	int i, enc, f_enc;

	DPRINTF(("%s: ENTER: mode=0x%x target:", __func__, mode));
	auconv_dump_params(param);
	enc = auconv_normalize_encoding(param->encoding,
					param->precision);
	DPRINTF(("%s: target normalized: %s\n", __func__,
	    encoding_dbg_names[enc]));
	for (i = 0; i < nformats; i++) {
		if (!AUFMT_IS_VALID(&formats[i]))
			continue;
		if ((formats[i].mode & mode) == 0)
			continue;
		f_enc = auconv_normalize_encoding(formats[i].encoding,
						  formats[i].precision);
		DPRINTF(("%s: format[%d] normalized: %s\n",
			 __func__, i, encoding_dbg_names[f_enc]));
		if (f_enc != enc)
			continue;
		/**
		 * XXX	we need encoding-dependent check.
		 * XXX	Is to check precision/channels meaningful for
		 *	MPEG encodings?
		 */
		if (enc != AUDIO_ENCODING_AC3) {
			if (formats[i].validbits != param->validbits)
				continue;
			if (formats[i].precision != param->precision)
				continue;
			if (formats[i].channels != param->channels)
				continue;
		}
		if (!auconv_is_supported_rate(&formats[i],
					      param->sample_rate))
			continue;
		return i;
	}
	return -1;
}

/**
 * a sub-routine for auconv_set_converter()
 *   SLINEAR ==> SLINEAR_<host-endian>
 *   ULINEAR ==> ULINEAR_<host-endian>
 *   SLINEAR_BE 8bit ==> SLINEAR_LE 8bit
 *   ULINEAR_BE 8bit ==> ULINEAR_LE 8bit
 * This should be the same rule as audio_check_params()
 */
static u_int
auconv_normalize_encoding(u_int encoding, u_int precision)
{
	int enc;

	enc = encoding;
	if (enc == AUDIO_ENCODING_SLINEAR_LE)
		return enc;
	if (enc == AUDIO_ENCODING_ULINEAR_LE)
		return enc;
#if BYTE_ORDER == LITTLE_ENDIAN
	if (enc == AUDIO_ENCODING_SLINEAR)
		return AUDIO_ENCODING_SLINEAR_LE;
	else if (enc == AUDIO_ENCODING_ULINEAR)
		return AUDIO_ENCODING_ULINEAR_LE;
#else
	if (enc == AUDIO_ENCODING_SLINEAR)
		enc = AUDIO_ENCODING_SLINEAR_BE;
	else if (enc == AUDIO_ENCODING_ULINEAR)
		enc = AUDIO_ENCODING_ULINEAR_BE;
#endif
	if (precision == 8 && enc == AUDIO_ENCODING_SLINEAR_BE)
		return AUDIO_ENCODING_SLINEAR_LE;
	if (precision == 8 && enc == AUDIO_ENCODING_ULINEAR_BE)
		return AUDIO_ENCODING_ULINEAR_LE;
	return enc;
}

/**
 * a sub-routine for auconv_set_converter()
 */
static int
auconv_is_supported_rate(const struct audio_format *format, u_int rate)
{
	u_int i;

	if (format->frequency_type == 0) {
		return format->frequency[0] <= rate
			&& rate <= format->frequency[1];
	}
	for (i = 0; i < format->frequency_type; i++) {
		if (format->frequency[i] == rate)
			return true;
	}
	return false;
}

/**
 * Create an audio_encoding_set besed on hardware capability represented
 * by audio_format.
 *
 * Usage:
 *	foo_attach(...) {
 *		:
 *		if (auconv_create_encodings(formats, nformats,
 *			&sc->sc_encodings) != 0) {
 *			// attach failure
 *		}
 *
 * @param formats	[IN] An array of formats which a hardware can support.
 * @param nformats	[IN] The number of elements of the array.
 * @param encodings	[OUT] receives an address of an audio_encoding_set.
 * @return errno; 0 for success.
 */
int
auconv_create_encodings(const struct audio_format *formats, int nformats,
			struct audio_encoding_set **encodings)
{
	struct audio_encoding_set *buf;
	int capacity;
	int i;
	int err;

#define	ADD_ENCODING(enc, prec, flags)	do { \
	err = auconv_add_encoding(enc, prec, flags, &buf, &capacity); \
	if (err != 0) goto err_exit; \
} while (/*CONSTCOND*/0)

	capacity = nformats;
	buf = AUCONV_MALLOC(ENCODING_SET_SIZE(capacity));
	if (buf == NULL) {
		err = ENOMEM;
		goto err_exit;
	}
	buf->size = 0;
	for (i = 0; i < nformats; i++) {
		if (!AUFMT_IS_VALID(&formats[i]))
			continue;

		ADD_ENCODING(formats[i].encoding,
			     formats[i].precision, 0);
	}
	*encodings = buf;
	return 0;

 err_exit:
	if (buf != NULL)
		AUCONV_FREE(buf);
	*encodings = NULL;
	return err;
}

/**
 * a sub-routine for auconv_create_encodings()
 */
static int
auconv_add_encoding(int enc, int prec, int flags,
		    struct audio_encoding_set **buf, int *capacity)
{
	static const char *encoding_names[] = {
		NULL, AudioEmulaw, AudioEalaw, NULL,
		NULL, AudioEadpcm, AudioEslinear_le, AudioEslinear_be,
		AudioEulinear_le, AudioEulinear_be,
		AudioEslinear, AudioEulinear,
		AudioEmpeg_l1_stream, AudioEmpeg_l1_packets,
		AudioEmpeg_l1_system, AudioEmpeg_l2_stream,
		AudioEmpeg_l2_packets, AudioEmpeg_l2_system,
		AudioEac3
	};
	struct audio_encoding_set *set;
	audio_encoding_t *e;
	int i;

	set = *buf;
	/* already has the same encoding? */
	e = set->items;
	for (i = 0; i < set->size; i++, e++) {
		if (e->encoding == enc && e->precision == prec) {
			/* overwrite EMULATED flag */
			if ((e->flags & AUDIO_ENCODINGFLAG_EMULATED)
			    && (flags & AUDIO_ENCODINGFLAG_EMULATED) == 0) {
				e->flags &= ~AUDIO_ENCODINGFLAG_EMULATED;
			}
			return 0;
		}
	}
	/* We don't have the specified one. */

	if (set->size >= *capacity) {
		panic("set->size(%d) >= *capacity(%d)",
		    set->size, *capacity);
	}

	e = &set->items[set->size];
	e->index = 0;
	strlcpy(e->name, encoding_names[enc], MAX_AUDIO_DEV_LEN);
	e->encoding = enc;
	e->precision = prec;
	e->flags = flags;
	set->size += 1;
	return 0;
}

/**
 * Delete an audio_encoding_set created by auconv_create_encodings().
 *
 * Usage:
 *	foo_detach(...) {
 *		:
 *		auconv_delete_encodings(sc->sc_encodings);
 *		:
 *	}
 *
 * @param encodings	[IN] An audio_encoding_set which was created by
 *			auconv_create_encodings().
 * @return errno; 0 for success.
 */
int auconv_delete_encodings(struct audio_encoding_set *encodings)
{
	if (encodings != NULL)
		AUCONV_FREE(encodings);
	return 0;
}

/**
 * Copy the element specified by aep->index.
 *
 * Usage:
 * int foo_query_encoding(void *v, audio_encoding_t *aep) {
 *	struct foo_softc *sc = (struct foo_softc *)v;
 *	return auconv_query_encoding(sc->sc_encodings, aep);
 * }
 *
 * @param encodings	[IN] An audio_encoding_set created by
 *			auconv_create_encodings().
 * @param aep		[IN/OUT] resultant audio_encoding_t.
 */
int
auconv_query_encoding(const struct audio_encoding_set *encodings,
		      audio_encoding_t *aep)
{
	if (aep->index < 0 || aep->index >= encodings->size)
		return EINVAL;
	strlcpy(aep->name, encodings->items[aep->index].name,
		MAX_AUDIO_DEV_LEN);
	aep->encoding = encodings->items[aep->index].encoding;
	aep->precision = encodings->items[aep->index].precision;
	aep->flags = encodings->items[aep->index].flags;
	return 0;
}
