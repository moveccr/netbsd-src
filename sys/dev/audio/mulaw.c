/*	$NetBSD: mulaw.c,v 1.3 2020/01/11 04:06:13 isaki Exp $	*/

/*
 * Copyright (C) 2017 Tetsuya Isaki. All rights reserved.
 * Copyright (C) 2017 Y.Sugahara (moveccr). All rights reserved.
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

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: mulaw.c,v 1.3 2020/01/11 04:06:13 isaki Exp $");

#include <sys/types.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <dev/audio/audiovar.h>
#include <dev/audio/mulaw.h>

/*
 * audio_internal_to_mulaw has two implementations.
 *
 * 1. Use 8bit table (MULAW_LQ_ENC)
 *  It's traditional implementation and its precision is 8bit.
 *  It's faster but the size is larger.  And you can hear a little noise
 *  in silent part.
 *
 * 2. Calculation (default)
 *  It calculates mu-law with full spec and its precision is 14bit.
 *  It's about 10 times slower but the size is less than a half (on m68k,
 *  for example).
 *
 * mu-law is no longer a popular format.  I think size-optimized is better.
 */
/* #define MULAW_LQ_ENC */

/*
 * About mulaw32 format.
 *
 * The format which I call ``mulaw32'' is only used in dev/tc/bba.c .
 * It is 8bit mu-law but 16bit left-shifted and its containter is 32bit.
 * Not mu-law calculated in 32bit.
 *
 * When MULAW32 is not defined (it's default), this file outputs
 * audio_internal_to_mulaw() and audio_mulaw_to_internal().  When
 * MULAW32 is defined, this file outputs audio_internal_to_mulaw32()
 * and audio_mulaw32_to_internal() instead.
 *
 * Since mu-law is used as userland format and is mandatory, all audio
 * drivers (including tc/bba) link this mulaw.c in ordinary procedure.
 * On the other hand, only tc/bba also needs audio_internal_to_mulaw32()
 * and audio_mulaw32_to_internal() as its hardware drivers codec, so
 * define MULAW32 and include this file.  It's a bit tricky but I think
 * this is the simplest way.
 */

#if 0
#define MPRINTF(fmt, ...)	printf(fmt, ## __VA_ARGS__)
#else
#define MPRINTF(fmt, ...)	/**/
#endif

static const int16_t mulaw_to_slinear16[256] = {
	0x8284, 0x8684, 0x8a84, 0x8e84, 0x9284, 0x9684, 0x9a84, 0x9e84,
	0xa284, 0xa684, 0xaa84, 0xae84, 0xb284, 0xb684, 0xba84, 0xbe84,
	0xc184, 0xc384, 0xc584, 0xc784, 0xc984, 0xcb84, 0xcd84, 0xcf84,
	0xd184, 0xd384, 0xd584, 0xd784, 0xd984, 0xdb84, 0xdd84, 0xdf84,
	0xe104, 0xe204, 0xe304, 0xe404, 0xe504, 0xe604, 0xe704, 0xe804,
	0xe904, 0xea04, 0xeb04, 0xec04, 0xed04, 0xee04, 0xef04, 0xf004,
	0xf0c4, 0xf144, 0xf1c4, 0xf244, 0xf2c4, 0xf344, 0xf3c4, 0xf444,
	0xf4c4, 0xf544, 0xf5c4, 0xf644, 0xf6c4, 0xf744, 0xf7c4, 0xf844,
	0xf8a4, 0xf8e4, 0xf924, 0xf964, 0xf9a4, 0xf9e4, 0xfa24, 0xfa64,
	0xfaa4, 0xfae4, 0xfb24, 0xfb64, 0xfba4, 0xfbe4, 0xfc24, 0xfc64,
	0xfc94, 0xfcb4, 0xfcd4, 0xfcf4, 0xfd14, 0xfd34, 0xfd54, 0xfd74,
	0xfd94, 0xfdb4, 0xfdd4, 0xfdf4, 0xfe14, 0xfe34, 0xfe54, 0xfe74,
	0xfe8c, 0xfe9c, 0xfeac, 0xfebc, 0xfecc, 0xfedc, 0xfeec, 0xfefc,
	0xff0c, 0xff1c, 0xff2c, 0xff3c, 0xff4c, 0xff5c, 0xff6c, 0xff7c,
	0xff88, 0xff90, 0xff98, 0xffa0, 0xffa8, 0xffb0, 0xffb8, 0xffc0,
	0xffc8, 0xffd0, 0xffd8, 0xffe0, 0xffe8, 0xfff0, 0xfff8, 0xfffc,
	0x7d7c, 0x797c, 0x757c, 0x717c, 0x6d7c, 0x697c, 0x657c, 0x617c,
	0x5d7c, 0x597c, 0x557c, 0x517c, 0x4d7c, 0x497c, 0x457c, 0x417c,
	0x3e7c, 0x3c7c, 0x3a7c, 0x387c, 0x367c, 0x347c, 0x327c, 0x307c,
	0x2e7c, 0x2c7c, 0x2a7c, 0x287c, 0x267c, 0x247c, 0x227c, 0x207c,
	0x1efc, 0x1dfc, 0x1cfc, 0x1bfc, 0x1afc, 0x19fc, 0x18fc, 0x17fc,
	0x16fc, 0x15fc, 0x14fc, 0x13fc, 0x12fc, 0x11fc, 0x10fc, 0x0ffc,
	0x0f3c, 0x0ebc, 0x0e3c, 0x0dbc, 0x0d3c, 0x0cbc, 0x0c3c, 0x0bbc,
	0x0b3c, 0x0abc, 0x0a3c, 0x09bc, 0x093c, 0x08bc, 0x083c, 0x07bc,
	0x075c, 0x071c, 0x06dc, 0x069c, 0x065c, 0x061c, 0x05dc, 0x059c,
	0x055c, 0x051c, 0x04dc, 0x049c, 0x045c, 0x041c, 0x03dc, 0x039c,
	0x036c, 0x034c, 0x032c, 0x030c, 0x02ec, 0x02cc, 0x02ac, 0x028c,
	0x026c, 0x024c, 0x022c, 0x020c, 0x01ec, 0x01cc, 0x01ac, 0x018c,
	0x0174, 0x0164, 0x0154, 0x0144, 0x0134, 0x0124, 0x0114, 0x0104,
	0x00f4, 0x00e4, 0x00d4, 0x00c4, 0x00b4, 0x00a4, 0x0094, 0x0084,
	0x0078, 0x0070, 0x0068, 0x0060, 0x0058, 0x0050, 0x0048, 0x0040,
	0x0038, 0x0030, 0x0028, 0x0020, 0x0018, 0x0010, 0x0008, 0x0000,
};

#if defined(MULAW_LQ_ENC)
static const uint8_t slinear8_to_mulaw[256] = {
	0xff, 0xe7, 0xdb, 0xd3, 0xcd, 0xc9, 0xc5, 0xc1,
	0xbe, 0xbc, 0xba, 0xb8, 0xb6, 0xb4, 0xb2, 0xb0,
	0xaf, 0xae, 0xad, 0xac, 0xab, 0xaa, 0xa9, 0xa8,
	0xa7, 0xa6, 0xa5, 0xa4, 0xa3, 0xa2, 0xa1, 0xa0,
	0x9f, 0x9f, 0x9e, 0x9e, 0x9d, 0x9d, 0x9c, 0x9c,
	0x9b, 0x9b, 0x9a, 0x9a, 0x99, 0x99, 0x98, 0x98,
	0x97, 0x97, 0x96, 0x96, 0x95, 0x95, 0x94, 0x94,
	0x93, 0x93, 0x92, 0x92, 0x91, 0x91, 0x90, 0x90,
	0x8f, 0x8f, 0x8f, 0x8f, 0x8e, 0x8e, 0x8e, 0x8e,
	0x8d, 0x8d, 0x8d, 0x8d, 0x8c, 0x8c, 0x8c, 0x8c,
	0x8b, 0x8b, 0x8b, 0x8b, 0x8a, 0x8a, 0x8a, 0x8a,
	0x89, 0x89, 0x89, 0x89, 0x88, 0x88, 0x88, 0x88,
	0x87, 0x87, 0x87, 0x87, 0x86, 0x86, 0x86, 0x86,
	0x85, 0x85, 0x85, 0x85, 0x84, 0x84, 0x84, 0x84,
	0x83, 0x83, 0x83, 0x83, 0x82, 0x82, 0x82, 0x82,
	0x81, 0x81, 0x81, 0x81, 0x80, 0x80, 0x80, 0x80,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
	0x01, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03,
	0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05,
	0x05, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07,
	0x07, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09,
	0x09, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x0b,
	0x0b, 0x0c, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d, 0x0d,
	0x0d, 0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f,
	0x0f, 0x10, 0x10, 0x11, 0x11, 0x12, 0x12, 0x13,
	0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x16, 0x17,
	0x17, 0x18, 0x18, 0x19, 0x19, 0x1a, 0x1a, 0x1b,
	0x1b, 0x1c, 0x1c, 0x1d, 0x1d, 0x1e, 0x1e, 0x1f,
	0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
	0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
	0x2f, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c,
	0x3e, 0x41, 0x45, 0x49, 0x4d, 0x53, 0x5b, 0x67,
};
#endif

/*
 * audio_mulaw_to_internal:
 *	This filter performs conversion from mu-law to internal format.
 *
 * audio_mulaw32_to_internal:
 *	This filter performs conversion from mulaw32 used only in tc/bba.c
 *	to internal format.
 */
void
#if !defined(MULAW32)
audio_mulaw_to_internal(audio_filter_arg_t *arg)
#else
audio_mulaw32_to_internal(audio_filter_arg_t *arg)
#endif
{
#if defined(MULAW32)
	const uint32_t *s;
#else
	const uint8_t *s;
#endif
	aint_t *d;
	u_int sample_count;
	u_int i;

	DIAGNOSTIC_filter_arg(arg);
#if !defined(MULAW32)
	KASSERT(arg->srcfmt->encoding == AUDIO_ENCODING_ULAW);
	KASSERT(arg->srcfmt->stride == 8);
	KASSERT(arg->srcfmt->precision == 8);
#endif
	KASSERT(audio_format2_is_internal(arg->dstfmt));
	KASSERT(arg->srcfmt->channels == arg->dstfmt->channels);

	s = arg->src;
	d = arg->dst;
	sample_count = arg->count * arg->srcfmt->channels;

	for (i = 0; i < sample_count; i++) {
		aint_t val;
		uint m;
		m = *s++;
#if defined(MULAW32)
		/* 32bit container used only in tc/bba.c */
		m = (m >> 16) & 0xff;
#endif
		val = mulaw_to_slinear16[m];
		val <<= AUDIO_INTERNAL_BITS - 16;
		*d++ = val;
	}
}

/*
 * audio_internal_to_mulaw:
 *	This filter performs conversion from internal format to mu-law.
 *
 * audio_internal_to_mulaw32:
 *	This filter performs conversion from internal format to mulaw32
 *	used only in tc/bba.c.
 */
void
#if !defined(MULAW32)
audio_internal_to_mulaw(audio_filter_arg_t *arg)
#else
audio_internal_to_mulaw32(audio_filter_arg_t *arg)
#endif
{
	const aint_t *s;
#if defined(MULAW32)
	uint32_t *d;
#else
	uint8_t *d;
#endif
	u_int sample_count;
	u_int i;

	DIAGNOSTIC_filter_arg(arg);
#if !defined(MULAW32)
	KASSERT(arg->dstfmt->encoding == AUDIO_ENCODING_ULAW);
	KASSERT(arg->dstfmt->stride == 8);
	KASSERT(arg->dstfmt->precision == 8);
#endif
	KASSERT(audio_format2_is_internal(arg->srcfmt));
	KASSERT(arg->srcfmt->channels == arg->dstfmt->channels);

	s = arg->src;
	d = arg->dst;
	sample_count = arg->count * arg->srcfmt->channels;

	for (i = 0; i < sample_count; i++) {
		uint8_t m;
#if defined(MULAW_LQ_ENC)
		/* 8bit (low quality, fast but fat) encoder */
		uint8_t val;
		val = (*s++) >> (AUDIO_INTERNAL_BITS - 8);
		m = slinear8_to_mulaw[val];
#else
		/* 14bit (fullspec, slow but small) encoder */
#if 1
		int16_t val;
		int c;

		val = (int16_t)(*s++ >> (AUDIO_INTERNAL_BITS - 16));
		if (val < 0) {
			m = 0;
		} else {
			val = ~val;
			m = 0x80;
		}
		/* limit */
		if (val < -8158 * 4)
			val = -8158 * 4;
		val -= 33 * 4;	/* bias */

		val <<= 1;
		for (c = 0; c < 7; c++) {
			if (val >= 0) {
				break;
			}

			m += (1 << 4);	/* exponent */
			val <<= 1;
		}
		val <<= 1;

		m += (val >> 12) & 0x0f; /* mantissa */
#else
		uint16_t val;
		int c;

		val = *s++ >> (AUDIO_INTERNAL_BITS - 16);
		if ((int16_t)val < 0) {
			m = 0;
		} else {
			val = ~val;
			m = 0x80;
		}
		/* limit */
		if ((int16_t)val < -8158 * 4)
			val = -8158 * 4;
		val -= 33 * 4;	/* bias */

		// Before(1)         Before(2)         Before(3)
		// S0MMMMxx_xxxxxxxx 0MMMMxxx_xxxxxxx0 c=0,v=0MMMMxxx_xxxxxxx0
		// S10MMMMx_xxxxxxxx 10MMMMxx_xxxxxxx0 c=1,v=0MMMMxxx_xxxxxx00
		// S110MMMM_xxxxxxxx 110MMMMx_xxxxxxx0 c=2,v=0MMMMxxx_xxxxx000
		// :                 :                 :
		// S1111110_MMMMxxxx 1111110M_MMMxxxx0 c=6,v=0MMMMxxx_x0000000

		// (1) Push out sign bit
		val <<= 1;

		// (2) Find first zero (and align val to left)
		c = 0;
		if (val >= 0xf000) c += 4, val <<= 4;
		if (val >= 0xc000) c += 2, val <<= 2;
		if (val >= 0x8000) c += 1, val <<= 1;

		// (3)
		m += (c << 4);
		m += (val >> 11) & 0x0f;
#endif
#endif

#if defined(MULAW32)
		/* 8bit mu-law in 32bit container used only in tc/bba.c */
		*d++ = m << 16;
#else
		*d++ = m;
#endif
	}
}
