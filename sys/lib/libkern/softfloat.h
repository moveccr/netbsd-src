/* $NetBSD: softfloat.h,v 1.6 2020/09/02 03:41:56 thorpej Exp $ */

/* This is a derivative work. */

/*-
 * Copyright (c) 2001 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Ross Harvey.
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

/*============================================================================

This C header file template is part of the Berkeley SoftFloat IEEE Floating-
Point Arithmetic Package, Release 2c, by John R. Hauser.

THIS SOFTWARE IS DISTRIBUTED AS IS, FOR FREE.  Although reasonable effort has
been made to avoid it, THIS SOFTWARE MAY CONTAIN FAULTS THAT WILL AT TIMES
RESULT IN INCORRECT BEHAVIOR.  USE OF THIS SOFTWARE IS RESTRICTED TO PERSONS
AND ORGANIZATIONS WHO CAN AND WILL TOLERATE ALL LOSSES, COSTS, OR OTHER
PROBLEMS THEY INCUR DUE TO THE SOFTWARE WITHOUT RECOMPENSE FROM JOHN HAUSER OR
THE INTERNATIONAL COMPUTER SCIENCE INSTITUTE, AND WHO FURTHERMORE EFFECTIVELY
INDEMNIFY JOHN HAUSER AND THE INTERNATIONAL COMPUTER SCIENCE INSTITUTE
(possibly via similar legal notice) AGAINST ALL LOSSES, COSTS, OR OTHER
PROBLEMS INCURRED BY THEIR CUSTOMERS AND CLIENTS DUE TO THE SOFTWARE, OR
INCURRED BY ANYONE DUE TO A DERIVATIVE WORK THEY CREATE USING ANY PART OF THE
SOFTWARE.

Derivative works require also that (1) the source code for the derivative work
includes prominent notice that the work is derivative, and (2) the source code
includes prominent notice of these three paragraphs for those parts of this
code that are retained.

=============================================================================*/

#include <sys/types.h>

#if !defined(_KERNEL) && !defined(_STANDALONE)
#include <inttypes.h>
#include <ieeefp.h>
#else
#include "sys/inttypes.h"
#include "machine/ieeefp.h"
#endif
#include <sys/endian.h>

/*----------------------------------------------------------------------------
| The macro `FLOATX80' must be defined to enable the double-extended-precision
| floating-point format `floatx80'.  If this macro is not defined, the
| `floatx80' type will not be defined, and none of the functions that either
| input or output the `floatx80' type will be defined.  The same applies to
| the `FLOAT128' macro and the quadruple-precision format `float128'.
*----------------------------------------------------------------------------*/
/* #define FLOATX80 */
/* #define FLOAT128 */

/*----------------------------------------------------------------------------
| Software IEEE floating-point types.
*----------------------------------------------------------------------------*/
typedef u_int32_t float32;
typedef u_int64_t float64;
#ifdef FLOATX80
typedef struct {
#if BYTE_ORDER == BIG_ENDIAN
    u_int16_t high;
    u_int64_t low;
#else
    u_int64_t low;
    u_int16_t high;
#endif
} floatx80;
#endif
#ifdef FLOAT128
typedef struct {
    u_int64_t high, low;
} float128;
#endif

/*
 * Some of the global variables that used to be here have been removed for
 * fairly obvious (defopt-MULTIPROCESSOR) reasons.  The rest (which don't
 * change dynamically) will be removed later. [ross]
 */

#define float_rounding_mode() fpgetround()

/*----------------------------------------------------------------------------
| Software IEEE floating-point underflow tininess-detection mode.
*----------------------------------------------------------------------------*/
extern int float_detect_tininess;
enum {
    float_tininess_after_rounding  = 1,
    float_tininess_before_rounding = 0
};

/*----------------------------------------------------------------------------
| Software IEEE floating-point rounding mode.
*----------------------------------------------------------------------------*/
enum {
    float_round_nearest_even = FP_RN,
    float_round_to_zero      = FP_RZ,
    float_round_down         = FP_RM,
    float_round_up           = FP_RP
};

/*----------------------------------------------------------------------------
| Software IEEE floating-point exception flags.
*----------------------------------------------------------------------------*/
enum {
    float_flag_inexact   =  FP_X_IMP,
    float_flag_underflow =  FP_X_UFL,
    float_flag_overflow  =  FP_X_OFL,
    float_flag_divbyzero =  FP_X_DZ,
    float_flag_invalid   =  FP_X_INV
};

/*----------------------------------------------------------------------------
| Routine to raise any or all of the software IEEE floating-point exception
| flags.
*----------------------------------------------------------------------------*/
/*
 * Routines provided by <machine/ieeefp.h>:
 *
 * float_raise()
 * float_set_inexact()
 * float_set_invalid()
 */

/*----------------------------------------------------------------------------
| Software IEEE integer-to-floating-point conversion routines.
*----------------------------------------------------------------------------*/
float32 int32_to_float32( int );
float64 int32_to_float64( int );
#ifdef FLOATX80
floatx80 int32_to_floatx80( int );
#endif
#ifdef FLOAT128
float128 int32_to_float128( int );
#endif
#ifndef SOFTFLOAT_FOR_GCC /* __floatdi?f is in libgcc2.c */
float32 int64_to_float32( int64_t );
float64 int64_to_float64( int64_t );
#ifdef FLOATX80
floatx80 int64_to_floatx80( int64_t );
#endif
#ifdef FLOAT128
float128 int64_to_float128( int64_t );
#endif
#endif

/*----------------------------------------------------------------------------
| Software IEEE single-precision conversion routines.
*----------------------------------------------------------------------------*/
int float32_to_int32( float32 );
int float32_to_int32_round_to_zero( float32 );
#ifndef SOFTFLOAT_FOR_GCC /* __fix?fdi provided by libgcc2.c */
int64_t float32_to_int64( float32 );
int64_t float32_to_int64_round_to_zero( float32 );
#endif
float64 float32_to_float64( float32 );
#ifdef FLOATX80
floatx80 float32_to_floatx80( float32 );
#endif
#ifdef FLOAT128
float128 float32_to_float128( float32 );
#endif

/*----------------------------------------------------------------------------
| Software IEEE single-precision operations.
*----------------------------------------------------------------------------*/
float32 float32_round_to_int( float32 );
float32 float32_add( float32, float32 );
float32 float32_sub( float32, float32 );
float32 float32_mul( float32, float32 );
float32 float32_div( float32, float32 );
float32 float32_rem( float32, float32 );
float32 float32_sqrt( float32 );
int float32_eq( float32, float32 );
int float32_le( float32, float32 );
int float32_lt( float32, float32 );
int float32_eq_signaling( float32, float32 );
int float32_le_quiet( float32, float32 );
int float32_lt_quiet( float32, float32 );
#ifndef SOFTFLOAT_FOR_GCC
int float32_is_signaling_nan( float32 );
#endif

/*----------------------------------------------------------------------------
| Software IEEE double-precision conversion routines.
*----------------------------------------------------------------------------*/
int float64_to_int32( float64 );
int float64_to_int32_round_to_zero( float64 );
#ifndef SOFTFLOAT_FOR_GCC /* __fix?fdi provided by libgcc2.c */
int64_t float64_to_int64( float64 );
uint64_t float64_to_uint64( float64 );
int64_t float64_to_int64_round_to_zero( float64 );
#endif
float32 float64_to_float32( float64 );
#ifdef FLOATX80
floatx80 float64_to_floatx80( float64 );
#endif
#ifdef FLOAT128
float128 float64_to_float128( float64 );
#endif

/*----------------------------------------------------------------------------
| Software IEEE double-precision operations.
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
| The pattern for a default generated double-precision NaN.
*----------------------------------------------------------------------------*/
#define float64_default_nan 0xFFF8000000000000LL

/*----------------------------------------------------------------------------
| Returns 1 if the double-precision floating-point value `a' is a NaN;
| otherwise returns 0.
*----------------------------------------------------------------------------*/

static __inline int
float64_is_nan(float64 a)
{
	return 0xFFE0000000000000LL < a << 1;
}

/*----------------------------------------------------------------------------
| Returns 1 if the double-precision floating-point value `a' is a signaling
| NaN; otherwise returns 0.
*----------------------------------------------------------------------------*/

static __inline int
float64_is_signaling_nan(float64 a)
{
	return (a >> 51 & 0xFFF) == 0xFFE && (a & 0x0007FFFFFFFFFFFFLL);
}

float64 float64_round_to_int( float64 );
float64 float64_add( float64, float64 );
float64 float64_sub( float64, float64 );
float64 float64_mul( float64, float64 );
float64 float64_div( float64, float64 );
float64 float64_rem( float64, float64 );
float64 float64_sqrt( float64 );
int float64_eq( float64, float64 );
int float64_le( float64, float64 );
int float64_lt( float64, float64 );
int float64_eq_signaling( float64, float64 );
int float64_le_quiet( float64, float64 );
int float64_lt_quiet( float64, float64 );
#ifndef SOFTFLOAT_FOR_GCC
int float64_is_signaling_nan( float64 );
#endif

#ifdef FLOATX80

/*----------------------------------------------------------------------------
| Software IEEE double-extended-precision conversion routines.
*----------------------------------------------------------------------------*/
int floatx80_to_int32( floatx80 );
int floatx80_to_int32_round_to_zero( floatx80 );
int64_t floatx80_to_int64( floatx80 );
int64_t floatx80_to_int64_round_to_zero( floatx80 );
float32 floatx80_to_float32( floatx80 );
float64 floatx80_to_float64( floatx80 );
#ifdef FLOAT128
float128 floatx80_to_float128( floatx80 );
#endif

/*----------------------------------------------------------------------------
| Software IEEE double-extended-precision rounding precision.  Valid values
| are 32, 64, and 80.
*----------------------------------------------------------------------------*/
extern int floatx80_rounding_precision;

/*----------------------------------------------------------------------------
| Software IEEE double-extended-precision operations.
*----------------------------------------------------------------------------*/
floatx80 floatx80_round_to_int( floatx80 );
floatx80 floatx80_add( floatx80, floatx80 );
floatx80 floatx80_sub( floatx80, floatx80 );
floatx80 floatx80_mul( floatx80, floatx80 );
floatx80 floatx80_div( floatx80, floatx80 );
floatx80 floatx80_rem( floatx80, floatx80 );
floatx80 floatx80_sqrt( floatx80 );
int floatx80_eq( floatx80, floatx80 );
int floatx80_le( floatx80, floatx80 );
int floatx80_lt( floatx80, floatx80 );
int floatx80_eq_signaling( floatx80, floatx80 );
int floatx80_le_quiet( floatx80, floatx80 );
int floatx80_lt_quiet( floatx80, floatx80 );
int floatx80_is_signaling_nan( floatx80 );

#endif

#ifdef FLOAT128

/*----------------------------------------------------------------------------
| Software IEEE quadruple-precision conversion routines.
*----------------------------------------------------------------------------*/
int float128_to_int32( float128 );
int float128_to_int32_round_to_zero( float128 );
int64_t float128_to_int64( float128 );
int64_t float128_to_int64_round_to_zero( float128 );
float32 float128_to_float32( float128 );
float64 float128_to_float64( float128 );
#ifdef FLOATX80
floatx80 float128_to_floatx80( float128 );
#endif

/*----------------------------------------------------------------------------
| Software IEEE quadruple-precision operations.
*----------------------------------------------------------------------------*/
float128 float128_round_to_int( float128 );
float128 float128_add( float128, float128 );
float128 float128_sub( float128, float128 );
float128 float128_mul( float128, float128 );
float128 float128_div( float128, float128 );
float128 float128_rem( float128, float128 );
float128 float128_sqrt( float128 );
int float128_eq( float128, float128 );
int float128_le( float128, float128 );
int float128_lt( float128, float128 );
int float128_eq_signaling( float128, float128 );
int float128_le_quiet( float128, float128 );
int float128_lt_quiet( float128, float128 );
int float128_is_signaling_nan( float128 );

#endif

