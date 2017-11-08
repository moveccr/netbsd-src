#include <stdint.h>
#include <stdbool.h>
#include "compat.h"

#include "auring.h"
#include "auformat.h"
#include "aucodec.h"


/*
* ***** 変換関数 *****
* 変換関数では、
* arg->src の arg->count 個の有効フレームと
* arg->dst の arg->count 個の空きフレームに
* アンラウンディングでアクセス出来ることを呼び出し側が保証します。
* 変換に伴い、arg->src, arg->dst の ring ポインタを進めてください。
* src から読み取られたフレーム数を count にセットしてください。(等しい場合は何もしなくていい)
*/

/* XXX: 値がおかしい? */
static const uint16_t mulaw_to_slinear16HE[256] = {
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
	0xffc8, 0xffd0, 0xffd8, 0xffe0, 0xffe8, 0xfff0, 0xfff8, 0xffff,
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

static const uint16_t alaw_to_slinear16[256] = {
	0xea80, 0xeb80, 0xe880, 0xe980, 0xee80, 0xef80, 0xec80, 0xed80,
	0xe280, 0xe380, 0xe080, 0xe180, 0xe680, 0xe780, 0xe480, 0xe580,
	0xf540, 0xf5c0, 0xf440, 0xf4c0, 0xf740, 0xf7c0, 0xf640, 0xf6c0,
	0xf140, 0xf1c0, 0xf040, 0xf0c0, 0xf340, 0xf3c0, 0xf240, 0xf2c0,
	0xaa00, 0xae00, 0xa200, 0xa600, 0xba00, 0xbe00, 0xb200, 0xb600,
	0x8a00, 0x8e00, 0x8200, 0x8600, 0x9a00, 0x9e00, 0x9200, 0x9600,
	0xd500, 0xd700, 0xd100, 0xd300, 0xdd00, 0xdf00, 0xd900, 0xdb00,
	0xc500, 0xc700, 0xc100, 0xc300, 0xcd00, 0xcf00, 0xc900, 0xcb00,
	0xfea8, 0xfeb8, 0xfe88, 0xfe98, 0xfee8, 0xfef8, 0xfec8, 0xfed8,
	0xfe28, 0xfe38, 0xfe08, 0xfe18, 0xfe68, 0xfe78, 0xfe48, 0xfe58,
	0xffa8, 0xffb8, 0xff88, 0xff98, 0xffe8, 0xfff8, 0xffc8, 0xffd8,
	0xff28, 0xff38, 0xff08, 0xff18, 0xff68, 0xff78, 0xff48, 0xff58,
	0xfaa0, 0xfae0, 0xfa20, 0xfa60, 0xfba0, 0xfbe0, 0xfb20, 0xfb60,
	0xf8a0, 0xf8e0, 0xf820, 0xf860, 0xf9a0, 0xf9e0, 0xf920, 0xf960,
	0xfd50, 0xfd70, 0xfd10, 0xfd30, 0xfdd0, 0xfdf0, 0xfd90, 0xfdb0,
	0xfc50, 0xfc70, 0xfc10, 0xfc30, 0xfcd0, 0xfcf0, 0xfc90, 0xfcb0,
	0x1580, 0x1480, 0x1780, 0x1680, 0x1180, 0x1080, 0x1380, 0x1280,
	0x1d80, 0x1c80, 0x1f80, 0x1e80, 0x1980, 0x1880, 0x1b80, 0x1a80,
	0x0ac0, 0x0a40, 0x0bc0, 0x0b40, 0x08c0, 0x0840, 0x09c0, 0x0940,
	0x0ec0, 0x0e40, 0x0fc0, 0x0f40, 0x0cc0, 0x0c40, 0x0dc0, 0x0d40,
	0x5600, 0x5200, 0x5e00, 0x5a00, 0x4600, 0x4200, 0x4e00, 0x4a00,
	0x7600, 0x7200, 0x7e00, 0x7a00, 0x6600, 0x6200, 0x6e00, 0x6a00,
	0x2b00, 0x2900, 0x2f00, 0x2d00, 0x2300, 0x2100, 0x2700, 0x2500,
	0x3b00, 0x3900, 0x3f00, 0x3d00, 0x3300, 0x3100, 0x3700, 0x3500,
	0x0158, 0x0148, 0x0178, 0x0168, 0x0118, 0x0108, 0x0138, 0x0128,
	0x01d8, 0x01c8, 0x01f8, 0x01e8, 0x0198, 0x0188, 0x01b8, 0x01a8,
	0x0058, 0x0048, 0x0078, 0x0068, 0x0018, 0x0008, 0x0038, 0x0028,
	0x00d8, 0x00c8, 0x00f8, 0x00e8, 0x0098, 0x0088, 0x00b8, 0x00a8,
	0x0560, 0x0520, 0x05e0, 0x05a0, 0x0460, 0x0420, 0x04e0, 0x04a0,
	0x0760, 0x0720, 0x07e0, 0x07a0, 0x0660, 0x0620, 0x06e0, 0x06a0,
	0x02b0, 0x0290, 0x02f0, 0x02d0, 0x0230, 0x0210, 0x0270, 0x0250,
	0x03b0, 0x0390, 0x03f0, 0x03d0, 0x0330, 0x0310, 0x0370, 0x0350,
};

void
mulaw_to_internal(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));
	KASSERT(arg->srcfmt->encoding == AUDIO_ENCODING_MULAW);
	KASSERT(arg->srcfmt->stride == 8);
	KASSERT(arg->srcfmt->precision == 8);
	KASSERT(is_internal_format(arg->dstfmt));
	KASSERT(arg->srcfmt->channels == arg->dstfmt->channels);

	const uint8_t *sptr = arg->src;
	internal_t *dptr = arg->dst;

	int sample_count = arg->count * arg->srcfmt->channels;
	for (int i = 0; i < sample_count; i++) {
		internal_t s;
		s = mulaw_to_slinear16HE[*sptr++];
#if AUDIO_INTERNAL_BITS == 32
		s *= 0x00010000;
#endif
		*dptr++ = s;
	}
}

void
internal_to_mulaw(audio_filter_arg_t *arg)
{
	KASSERT(is_valid_filter_arg(arg));
	KASSERT(arg->dstfmt->encoding == AUDIO_ENCODING_MULAW);
	KASSERT(arg->dstfmt->stride == 8);
	KASSERT(arg->dstfmt->precision == 8);
	KASSERT(is_internal_format(arg->srcfmt));
	KASSERT(arg->srcfmt->channels == arg->dstfmt->channels);

	const internal_t *sptr = arg->src;
	uint8_t *dptr = arg->dst;

	int sample_count = arg->count * arg->srcfmt->channels;
	for (int i = 0; i < sample_count; i++) {
		internal_t s = *sptr++;
#if AUDIO_INTERNAL_BITS == 32
		s >>= 16;
#endif
		int16_t s16 = s;
		s16 >>= 2;
		uint8_t r = 0;

		/* XXX NOTIMPL */
		panic("notimpl");

		*dptr++ = r;
	}
}

