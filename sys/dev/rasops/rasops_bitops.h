/* 	$NetBSD: rasops_bitops.h,v 1.23 2019/08/02 04:39:09 rin Exp $	*/

/*-
 * Copyright (c) 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Andrew Doran.
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _RASOPS_BITOPS_H_
#define _RASOPS_BITOPS_H_ 1

#define	NAME(name)		NAME1(RASOPS_DEPTH, name)
#define	NAME1(depth, name)	NAME2(depth, name)
#define	NAME2(depth, name)	rasops ## depth ## _ ## name

#if   RASOPS_DEPTH == 1
#define	PIXEL_SHIFT	0
#elif RASOPS_DEPTH == 2
#define	PIXEL_SHIFT	1
#elif RASOPS_DEPTH == 4
#define	PIXEL_SHIFT	2
#else
#error "Depth not supported"
#endif

#define	PIXEL_BITS	RASOPS_DEPTH
#define	COLOR_MASK	__BITS(32 - PIXEL_BITS, 31)

#if RASOPS_DEPTH != 1
/*
 * Paint a single character. This function is also applicable to
 * monochrome, but that in rasops1.c is much simpler and faster.
 */
static void
NAME(putchar)(void *cookie, int row, int col, u_int uc, long attr)
{
	struct rasops_info *ri = (struct rasops_info *)cookie;
	struct wsdisplay_font *font = PICK_FONT(ri, uc);
	int full, cnt, bit;
	uint32_t fs, rs, fb, bg, fg, lmask, rmask, lbg, rbg, clr[2];
	uint32_t height, width;
	uint32_t *rp, *bp, *hp, tmp;
	uint8_t *fr;
	bool space;

	hp = NULL;	/* XXX GCC */

#ifdef RASOPS_CLIPPING
	/* Catches 'row < 0' case too */
	if ((unsigned)row >= (unsigned)ri->ri_rows)
		return;

	if ((unsigned)col >= (unsigned)ri->ri_cols)
		return;
#endif

	width = font->fontwidth << PIXEL_SHIFT;
	col *= width;
	height = font->fontheight;
	rp = (uint32_t *)(ri->ri_bits + row * ri->ri_yscale +
	    ((col >> 3) & ~3));
	if (ri->ri_hwbits)
		hp = (uint32_t *)(ri->ri_hwbits + row * ri->ri_yscale +
		    ((col >> 3) & ~3));
	col &= 31;
	rs = ri->ri_stride;

	bg = ri->ri_devcmap[((uint32_t)attr >> 16) & 0xf];
	fg = ri->ri_devcmap[((uint32_t)attr >> 24) & 0xf];

	/* If fg and bg match this becomes a space character */
	if (uc == ' ' || __predict_false(fg == bg)) {
		space = true;
		fr = NULL;	/* XXX GCC */
		fs = 0;		/* XXX GCC */
	} else {
		space = false;
		fr = FONT_GLYPH(uc, font, ri);
		fs = font->stride;
	}

	if (col + width <= 32) {
		/* Single word, only one mask */

		rmask = rasops_pmask[col][width & 31];
		lmask = ~rmask;

		if (space) {
			bg &= rmask;
			while (height--) {
				tmp = (*rp & lmask) | bg;
				*rp = tmp;
				DELTA(rp, rs, uint32_t *);
				if (ri->ri_hwbits) {
					*hp = tmp;
					DELTA(hp, rs, uint32_t *);
				}
			}
		} else {
			clr[0] = bg & COLOR_MASK;
			clr[1] = fg & COLOR_MASK;
			while (height--) {
				fb = be32uatoh(fr);
				fr += fs;
				tmp = 0;
				for (bit = col; bit < col + width;
				    bit += PIXEL_BITS) {
					tmp |= clr[(fb >> 31) & 1] >> bit;
					fb <<= 1;
				}
				tmp = (*rp & lmask) | MBE(tmp);
				*rp = tmp;
				DELTA(rp, rs, uint32_t *);
				if (ri->ri_hwbits) {
					*hp = tmp;
					DELTA(hp, rs, uint32_t *);
				}
			}
		}

		/* Do underline */
		if ((attr & WSATTR_UNDERLINE) != 0) {
			DELTA(rp, -(ri->ri_stride << 1), uint32_t *);
			tmp = (*rp & lmask) | (fg & rmask);
			*rp = tmp;
			if (ri->ri_hwbits) {
				DELTA(hp, -(ri->ri_stride << 1), uint32_t *);
				*hp = tmp;
			}
		}

		return;
	}

	/* Word boundary, two masks needed */

	lmask = ~rasops_lmask[col];
	rmask = ~rasops_rmask[(col + width) & 31];

	if (lmask != -1)
		width -= 32 - col;
	full = width / 32;
	width -= full * 32;

	if (space) {
		lbg = bg & ~lmask;
		rbg = bg & ~rmask;

		while (height--) {
			bp = rp;

			if (lmask != -1) {
				*bp = (*bp & lmask) | lbg;
				bp++;
			}

			for (cnt = full; cnt; cnt--)
				*bp++ = bg;

			if (rmask != -1)
				*bp = (*bp & rmask) | rbg;

			if (ri->ri_hwbits) {
				memcpy(hp, rp, ((lmask != -1) + full +
				    (rmask != -1)) << 2);
				DELTA(hp, rs, uint32_t *);
			}
			DELTA(rp, rs, uint32_t *);
		}
	} else {
		clr[0] = bg & COLOR_MASK;
		clr[1] = fg & COLOR_MASK;

		while (height--) {
			bp = rp;

			fb = be32uatoh(fr);
			fr += fs;

			if (lmask != -1) {
				tmp = 0;
				for (bit = col; bit < 32; bit += PIXEL_BITS) {
					tmp |= clr[(fb >> 31) & 1] >> bit;
					fb <<= 1;
				}
				*bp = (*bp & lmask) | MBE(tmp);
				bp++;
			}

			for (cnt = full; cnt; cnt--) {
				tmp = 0;
				for (bit = 0; bit < 32; bit += PIXEL_BITS) {
					tmp |= clr[(fb >> 31) & 1] >> bit;
					fb <<= 1;
				}
				*bp++ = MBE(tmp);
			}

			if (rmask != -1) {
				tmp = 0;
				for (bit = 0; bit < width; bit += PIXEL_BITS) {
					tmp |= clr[(fb >> 31) & 1] >> bit;
					fb <<= 1;
				}
				*bp = (*bp & rmask) | MBE(tmp);
			}

			if (ri->ri_hwbits) {
				memcpy(hp, rp, ((lmask != -1) + full +
				    (rmask != -1)) << 2);
				DELTA(hp, rs, uint32_t *);
			}

			DELTA(rp, rs, uint32_t *);
		}
	}

	/* Do underline */
	if ((attr & WSATTR_UNDERLINE) != 0) {
		DELTA(rp, -(ri->ri_stride << 1), uint32_t *);
		bp = rp;
		if (lmask != -1) {
			*bp = (*bp & lmask) | (fg & ~lmask);
			bp++;
		}
		for (cnt = full; cnt; cnt--)
			*bp++ = fg;
		if (rmask != -1)
			*bp = (*bp & rmask) | (fg & ~rmask);
		if (ri->ri_hwbits) {
			DELTA(hp, -(ri->ri_stride << 1), uint32_t *);
			memcpy(hp, rp, ((lmask != -1) + full +
			    (rmask != -1)) << 2);
		}
	}
}
#endif /* RASOPS_DEPTH != 1 */

/*
 * Erase columns.
 */
static void
NAME(erasecols)(void *cookie, int row, int col, int num, long attr)
{
	struct rasops_info *ri = (struct rasops_info *)cookie;
	uint32_t lclr, rclr, clr;
	uint32_t *dp, *rp, *hp, tmp, lmask, rmask;
	int height, cnt;

	hp = NULL;	/* XXX GCC */

#ifdef RASOPS_CLIPPING
	if ((unsigned)row >= (unsigned)ri->ri_rows)
		return;

	if (col < 0) {
		num += col;
		col = 0;
	}

	if (col + num > ri->ri_cols)
		num = ri->ri_cols - col;

	if (num <= 0)
		return;
#endif

	col *= ri->ri_font->fontwidth << PIXEL_SHIFT;
	num *= ri->ri_font->fontwidth << PIXEL_SHIFT;
	height = ri->ri_font->fontheight;
	clr = ri->ri_devcmap[((uint32_t)attr >> 16) & 0xf];
	rp = (uint32_t *)(ri->ri_bits + row*ri->ri_yscale + ((col >> 3) & ~3));
	if (ri->ri_hwbits)
		hp = (uint32_t *)(ri->ri_hwbits + row*ri->ri_yscale +
		    ((col >> 3) & ~3));
	col &= 31;

	if (col + num <= 32) {
		lmask = ~rasops_pmask[col][num & 31];
		lclr = clr & ~lmask;

		while (height--) {
			dp = rp;
			DELTA(rp, ri->ri_stride, uint32_t *);

			tmp = (*dp & lmask) | lclr;
			*dp = tmp;
			if (ri->ri_hwbits) {
				*hp = tmp;
				DELTA(hp, ri->ri_stride, uint32_t *);
			}
		}
	} else {
		lmask = rasops_rmask[col];
		rmask = rasops_lmask[(col + num) & 31];

		if (lmask)
			num = (num - (32 - col)) >> 5;
		else
			num = num >> 5;

		lclr = clr & ~lmask;
		rclr = clr & ~rmask;

		while (height--) {
			dp = rp;

			if (lmask) {
				*dp = (*dp & lmask) | lclr;
				dp++;
			}

			for (cnt = num; cnt > 0; cnt--)
				*dp++ = clr;

			if (rmask)
				*dp = (*dp & rmask) | rclr;

			if (ri->ri_hwbits) {
				memcpy(hp, rp, ((lmask != 0) + num +
				    (rmask != 0)) << 2);
				DELTA(hp, ri->ri_stride, uint32_t *);
			}
			DELTA(rp, ri->ri_stride, uint32_t *);
		}
	}
}

/*
 * Actually paint the cursor.
 */
static void
NAME(do_cursor)(struct rasops_info *ri)
{
	int height, row, col, num, cnt;
	uint32_t *dp, *rp, *hp, tmp, lmask, rmask;

	hp = NULL;	/* XXX GCC */

	row = ri->ri_crow;
	col = ri->ri_ccol * ri->ri_font->fontwidth << PIXEL_SHIFT;
	height = ri->ri_font->fontheight;
	num = ri->ri_font->fontwidth << PIXEL_SHIFT;
	rp = (uint32_t *)(ri->ri_bits + row * ri->ri_yscale +
	    ((col >> 3) & ~3));
	if (ri->ri_hwbits)
		hp = (uint32_t *)(ri->ri_hwbits + row * ri->ri_yscale +
		    ((col >> 3) & ~3));
	col &= 31;

	if (col + num <= 32) {
		lmask = rasops_pmask[col][num & 31];

		while (height--) {
			tmp = *rp ^ lmask;
			*rp = tmp;
			if (ri->ri_hwbits) {
				*hp = tmp;
				DELTA(hp, ri->ri_stride, uint32_t *);
			}
			DELTA(rp, ri->ri_stride, uint32_t *);
		}
	} else {
		lmask = ~rasops_rmask[col];
		rmask = ~rasops_lmask[(col + num) & 31];

		if (lmask != -1)
			num = (num - (32 - col)) >> 5;
		else
			num = num >> 5;

		while (height--) {
			dp = rp;

			if (lmask != -1) {
				*dp = *dp ^ lmask;
				dp++;
			}

			for (cnt = num; cnt; cnt--) {
				*dp = ~*dp;
				dp++;
			}

			if (rmask != -1)
				*dp = *dp ^ rmask;

			if (ri->ri_hwbits) {
				memcpy(hp, rp, ((lmask != -1) + num +
				    (rmask != -1)) << 2);
				DELTA(hp, ri->ri_stride, uint32_t *);
			}

			DELTA(rp, ri->ri_stride, uint32_t *);
		}
	}
}

/*
 * Copy columns. Ick!
 */
static void
NAME(copycols)(void *cookie, int row, int src, int dst, int num)
{
	struct rasops_info *ri = (struct rasops_info *)cookie;
	int height, lnum, rnum, sb, db, cnt, full;
	uint32_t tmp, lmask, rmask;
	uint32_t *sp, *dp, *srp, *drp, *dhp, *hp;

	dhp = hp = NULL;	/* XXX GCC */

#ifdef RASOPS_CLIPPING
	if (dst == src)
		return;

	/* Catches < 0 case too */
	if ((unsigned)row >= (unsigned)ri->ri_rows)
		return;

	if (src < 0) {
		num += src;
		src = 0;
	}

	if (src + num > ri->ri_cols)
		num = ri->ri_cols - src;

	if (dst < 0) {
		num += dst;
		dst = 0;
	}

	if (dst + num > ri->ri_cols)
		num = ri->ri_cols - dst;

	if (num <= 0)
		return;
#endif

	cnt = ri->ri_font->fontwidth << PIXEL_SHIFT;
	src *= cnt;
	dst *= cnt;
	num *= cnt;
	row *= ri->ri_yscale;
	height = ri->ri_font->fontheight;
	db = dst & 31;

	if (db + num <= 32) {
		/* Destination is contained within a single word */
		srp = (uint32_t *)(ri->ri_bits + row + ((src >> 3) & ~3));
		drp = (uint32_t *)(ri->ri_bits + row + ((dst >> 3) & ~3));
		if (ri->ri_hwbits)
			dhp = (uint32_t *)(ri->ri_hwbits + row +
			    ((dst >> 3) & ~3));
		sb = src & 31;

		while (height--) {
			GETBITS(srp, sb, num, tmp);
			PUTBITS(tmp, db, num, drp);
			if (ri->ri_hwbits) {
				PUTBITS(tmp, db, num, dhp);
				DELTA(dhp, ri->ri_stride, uint32_t *);
			}	
			DELTA(srp, ri->ri_stride, uint32_t *);
			DELTA(drp, ri->ri_stride, uint32_t *);
		}

		return;
	}

	lmask = rasops_rmask[db];
	rmask = rasops_lmask[(dst + num) & 31];
	lnum = (32 - db) & 31;
	rnum = (dst + num) & 31;

	if (lmask)
		full = (num - lnum) >> 5;
	else
		full = num >> 5;

	if (src < dst && src + num > dst) {
		/* Copy right-to-left */
		bool sbover;
		int sboff;

		srp = (uint32_t *)(ri->ri_bits + row +
		    (((src + num) >> 3) & ~3));
		drp = (uint32_t *)(ri->ri_bits + row +
		    (((dst + num) >> 3) & ~3));
		if (ri->ri_hwbits)
			dhp = (uint32_t *)(ri->ri_hwbits + row +
			    (((dst + num) >> 3) & ~3));

		sb = src & 31;
		sbover = (sb + lnum) >= 32;
		sboff = (src + num) & 31;
		if ((sboff -= rnum) < 0) {
			srp--;
			sboff += 32;
		}

		while (height--) {
			sp = srp;
			dp = drp;

			if (rnum) {
				GETBITS(sp, sboff, rnum, tmp);
				PUTBITS(tmp, 0, rnum, dp);
			}

			/* Now aligned to 32-bits wrt dp */
			for (cnt = full; cnt; cnt--) {
				--dp;
				--sp;
				GETBITS(sp, sboff, 32, tmp);
				*dp = tmp;
			}

			if (lmask) {
				if (sbover)
					--sp;
				--dp;
				GETBITS(sp, sb, lnum, tmp);
				PUTBITS(tmp, db, lnum, dp);
 			}

			if (ri->ri_hwbits) {
				hp = dhp;
				hp -= (lmask != 0) + full;
				memcpy(hp, dp, ((lmask != 0) + full +
				    (rnum != 0)) << 2);
				DELTA(dhp, ri->ri_stride, uint32_t *);
			}

			DELTA(srp, ri->ri_stride, uint32_t *);
			DELTA(drp, ri->ri_stride, uint32_t *);
 		}
	} else {
		/* Copy left-to-right */
		srp = (uint32_t *)(ri->ri_bits + row + ((src >> 3) & ~3));
		drp = (uint32_t *)(ri->ri_bits + row + ((dst >> 3) & ~3));
		if (ri->ri_hwbits)
			dhp = (uint32_t *)(ri->ri_hwbits + row +
			    ((dst >> 3) & ~3));

		while (height--) {
			sb = src & 31;
			sp = srp;
			dp = drp;

			if (lmask) {
				GETBITS(sp, sb, lnum, tmp);
				PUTBITS(tmp, db, lnum, dp);
				dp++;

				sb += lnum;
				if (sb > 31) {
					sp++;
					sb -= 32;
				}
			}

			/* Now aligned to 32-bits wrt dp */
			for (cnt = full; cnt; cnt--, sp++) {
				GETBITS(sp, sb, 32, tmp);
				*dp++ = tmp;
			}

			if (rmask) {
				GETBITS(sp, sb, rnum, tmp);
				PUTBITS(tmp, 0, rnum, dp);
 			}

			if (ri->ri_hwbits) {
				memcpy(dhp, drp, ((lmask != 0) + full +
				    (rmask != 0)) << 2);
				DELTA(dhp, ri->ri_stride, uint32_t *);
			}

			DELTA(srp, ri->ri_stride, uint32_t *);
			DELTA(drp, ri->ri_stride, uint32_t *);
 		}
 	}
}

#undef	PIXEL_SHIFT
#undef	PIXEL_BITS
#undef	COLOR_MASK

#endif /* _RASOPS_BITOPS_H_ */
