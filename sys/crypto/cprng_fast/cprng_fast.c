/*	$NetBSD: cprng_fast.c,v 1.15 2020/04/30 03:29:45 riastradh Exp $	*/

/*-
 * Copyright (c) 2014 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Taylor R. Campbell.
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

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: cprng_fast.c,v 1.15 2020/04/30 03:29:45 riastradh Exp $");

#include <sys/types.h>
#include <sys/param.h>
#include <sys/bitops.h>
#include <sys/cprng.h>
#include <sys/cpu.h>
#include <sys/entropy.h>
#include <sys/evcnt.h>
#include <sys/intr.h>
#include <sys/kmem.h>
#include <sys/percpu.h>

/* ChaCha core */

#define	crypto_core_OUTPUTWORDS	16
#define	crypto_core_INPUTWORDS	4
#define	crypto_core_KEYWORDS	8
#define	crypto_core_CONSTWORDS	4

#define	crypto_core_ROUNDS	8

static uint32_t
rotate(uint32_t u, unsigned c)
{

	return (u << c) | (u >> (32 - c));
}

#define	QUARTERROUND(a, b, c, d) do {					      \
	(a) += (b); (d) ^= (a); (d) = rotate((d), 16);			      \
	(c) += (d); (b) ^= (c); (b) = rotate((b), 12);			      \
	(a) += (b); (d) ^= (a); (d) = rotate((d),  8);			      \
	(c) += (d); (b) ^= (c); (b) = rotate((b),  7);			      \
} while (0)

static void
crypto_core(uint32_t *out, const uint32_t *in, const uint32_t *k,
    const uint32_t *c)
{
	uint32_t x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15;
	int i;

	x0 = c[0];
	x1 = c[1];
	x2 = c[2];
	x3 = c[3];
	x4 = k[0];
	x5 = k[1];
	x6 = k[2];
	x7 = k[3];
	x8 = k[4];
	x9 = k[5];
	x10 = k[6];
	x11 = k[7];
	x12 = in[0];
	x13 = in[1];
	x14 = in[2];
	x15 = in[3];

	for (i = crypto_core_ROUNDS; i > 0; i -= 2) {
		QUARTERROUND( x0, x4, x8,x12);
		QUARTERROUND( x1, x5, x9,x13);
		QUARTERROUND( x2, x6,x10,x14);
		QUARTERROUND( x3, x7,x11,x15);
		QUARTERROUND( x0, x5,x10,x15);
		QUARTERROUND( x1, x6,x11,x12);
		QUARTERROUND( x2, x7, x8,x13);
		QUARTERROUND( x3, x4, x9,x14);
	}

	out[0] = x0 + c[0];
	out[1] = x1 + c[1];
	out[2] = x2 + c[2];
	out[3] = x3 + c[3];
	out[4] = x4 + k[0];
	out[5] = x5 + k[1];
	out[6] = x6 + k[2];
	out[7] = x7 + k[3];
	out[8] = x8 + k[4];
	out[9] = x9 + k[5];
	out[10] = x10 + k[6];
	out[11] = x11 + k[7];
	out[12] = x12 + in[0];
	out[13] = x13 + in[1];
	out[14] = x14 + in[2];
	out[15] = x15 + in[3];
}

/* `expand 32-byte k' */
static const uint32_t crypto_core_constant32[4] = {
	0x61707865U, 0x3320646eU, 0x79622d32U, 0x6b206574U,
};

/*
 * Test vector for ChaCha20 from
 * <http://tools.ietf.org/html/draft-strombergson-chacha-test-vectors-00>,
 * test vectors for ChaCha12 and ChaCha8 generated by the same
 * crypto_core code with crypto_core_ROUNDS varied.
 */

#define	check(E)	do						\
{									\
	if (!(E))							\
		panic("crypto self-test failed: %s", #E);		\
} while (0)

static void
crypto_core_selftest(void)
{
	const uint32_t zero32[8] = {0};
	const uint8_t sigma[] = "expand 32-byte k";
	uint32_t block[16];
	unsigned i;

#if crypto_core_ROUNDS == 8
	static const uint8_t out[64] = {
		0x3e,0x00,0xef,0x2f,0x89,0x5f,0x40,0xd6,
		0x7f,0x5b,0xb8,0xe8,0x1f,0x09,0xa5,0xa1,
		0x2c,0x84,0x0e,0xc3,0xce,0x9a,0x7f,0x3b,
		0x18,0x1b,0xe1,0x88,0xef,0x71,0x1a,0x1e,
		0x98,0x4c,0xe1,0x72,0xb9,0x21,0x6f,0x41,
		0x9f,0x44,0x53,0x67,0x45,0x6d,0x56,0x19,
		0x31,0x4a,0x42,0xa3,0xda,0x86,0xb0,0x01,
		0x38,0x7b,0xfd,0xb8,0x0e,0x0c,0xfe,0x42,
	};
#elif crypto_core_ROUNDS == 12
	static const uint8_t out[64] = {
		0x9b,0xf4,0x9a,0x6a,0x07,0x55,0xf9,0x53,
		0x81,0x1f,0xce,0x12,0x5f,0x26,0x83,0xd5,
		0x04,0x29,0xc3,0xbb,0x49,0xe0,0x74,0x14,
		0x7e,0x00,0x89,0xa5,0x2e,0xae,0x15,0x5f,
		0x05,0x64,0xf8,0x79,0xd2,0x7a,0xe3,0xc0,
		0x2c,0xe8,0x28,0x34,0xac,0xfa,0x8c,0x79,
		0x3a,0x62,0x9f,0x2c,0xa0,0xde,0x69,0x19,
		0x61,0x0b,0xe8,0x2f,0x41,0x13,0x26,0xbe,
	};
#elif crypto_core_ROUNDS == 20
	static const uint8_t out[64] = {
		0x76,0xb8,0xe0,0xad,0xa0,0xf1,0x3d,0x90,
		0x40,0x5d,0x6a,0xe5,0x53,0x86,0xbd,0x28,
		0xbd,0xd2,0x19,0xb8,0xa0,0x8d,0xed,0x1a,
		0xa8,0x36,0xef,0xcc,0x8b,0x77,0x0d,0xc7,
		0xda,0x41,0x59,0x7c,0x51,0x57,0x48,0x8d,
		0x77,0x24,0xe0,0x3f,0xb8,0xd8,0x4a,0x37,
		0x6a,0x43,0xb8,0xf4,0x15,0x18,0xa1,0x1c,
		0xc3,0x87,0xb6,0x69,0xb2,0xee,0x65,0x86,
	};
#else
#error crypto_core_ROUNDS must be 8, 12, or 20.
#endif

	check(crypto_core_constant32[0] == le32dec(&sigma[0]));
	check(crypto_core_constant32[1] == le32dec(&sigma[4]));
	check(crypto_core_constant32[2] == le32dec(&sigma[8]));
	check(crypto_core_constant32[3] == le32dec(&sigma[12]));

	crypto_core(block, zero32, zero32, crypto_core_constant32);
	for (i = 0; i < 16; i++)
		check(block[i] == le32dec(&out[i*4]));
}

#undef check

#define	CPRNG_FAST_SEED_BYTES	(crypto_core_KEYWORDS * sizeof(uint32_t))

struct cprng_fast {
	uint32_t 	buffer[crypto_core_OUTPUTWORDS];
	uint32_t 	key[crypto_core_KEYWORDS];
	uint32_t 	nonce[crypto_core_INPUTWORDS];
	struct evcnt	*reseed_evcnt;
	unsigned	epoch;
};

__CTASSERT(sizeof ((struct cprng_fast *)0)->key == CPRNG_FAST_SEED_BYTES);

static void	cprng_fast_init_cpu(void *, void *, struct cpu_info *);
static void	cprng_fast_schedule_reseed(struct cprng_fast *);
static void	cprng_fast_intr(void *);

static void	cprng_fast_seed(struct cprng_fast *, const void *);
static void	cprng_fast_buf(struct cprng_fast *, void *, unsigned);

static void	cprng_fast_buf_short(void *, size_t);
static void	cprng_fast_buf_long(void *, size_t);

static percpu_t	*cprng_fast_percpu	__read_mostly;
static void	*cprng_fast_softint	__read_mostly;

void
cprng_fast_init(void)
{

	crypto_core_selftest();
	cprng_fast_percpu = percpu_create(sizeof(struct cprng_fast),
	    cprng_fast_init_cpu, NULL, NULL);
	cprng_fast_softint = softint_establish(SOFTINT_SERIAL|SOFTINT_MPSAFE,
	    &cprng_fast_intr, NULL);
}

static void
cprng_fast_init_cpu(void *p, void *arg __unused, struct cpu_info *ci)
{
	struct cprng_fast *const cprng = p;
	uint8_t seed[CPRNG_FAST_SEED_BYTES];

	cprng->epoch = entropy_epoch();
	cprng_strong(kern_cprng, seed, sizeof seed, 0);
	cprng_fast_seed(cprng, seed);
	(void)explicit_memset(seed, 0, sizeof seed);

	cprng->reseed_evcnt = kmem_alloc(sizeof(*cprng->reseed_evcnt),
	    KM_SLEEP);
	evcnt_attach_dynamic(cprng->reseed_evcnt, EVCNT_TYPE_MISC, NULL,
	    ci->ci_cpuname, "cprng_fast reseed");
}

static inline int
cprng_fast_get(struct cprng_fast **cprngp)
{
	struct cprng_fast *cprng;
	int s;

	*cprngp = cprng = percpu_getref(cprng_fast_percpu);
	s = splvm();

	if (__predict_false(cprng->epoch != entropy_epoch()))
		cprng_fast_schedule_reseed(cprng);

	return s;
}

static inline void
cprng_fast_put(struct cprng_fast *cprng, int s)
{

	KASSERT((cprng == percpu_getref(cprng_fast_percpu)) &&
	    (percpu_putref(cprng_fast_percpu), true));
	splx(s);
	percpu_putref(cprng_fast_percpu);
}

static void
cprng_fast_schedule_reseed(struct cprng_fast *cprng __unused)
{

	softint_schedule(cprng_fast_softint);
}

static void
cprng_fast_intr(void *cookie __unused)
{
	unsigned epoch = entropy_epoch();
	struct cprng_fast *cprng;
	uint8_t seed[CPRNG_FAST_SEED_BYTES];
	int s;

	cprng_strong(kern_cprng, seed, sizeof(seed), 0);

	cprng = percpu_getref(cprng_fast_percpu);
	s = splvm();
	cprng_fast_seed(cprng, seed);
	cprng->epoch = epoch;
	cprng->reseed_evcnt->ev_count++;
	splx(s);
	percpu_putref(cprng_fast_percpu);

	explicit_memset(seed, 0, sizeof(seed));
}

/* CPRNG algorithm */

/*
 * The state consists of a key, the current nonce, and a 64-byte buffer
 * of output.  Since we fill the buffer only when we need output, and
 * eat a 32-bit word at a time, one 32-bit word of the buffer would be
 * wasted.  Instead, we repurpose it to count the number of entries in
 * the buffer remaining, counting from high to low in order to allow
 * comparison to zero to detect when we need to refill it.
 */
#define	CPRNG_FAST_BUFIDX	(crypto_core_OUTPUTWORDS - 1)

static void
cprng_fast_seed(struct cprng_fast *cprng, const void *seed)
{

	(void)memset(cprng->buffer, 0, sizeof cprng->buffer);
	(void)memcpy(cprng->key, seed, sizeof cprng->key);
	(void)memset(cprng->nonce, 0, sizeof cprng->nonce);
}

static inline uint32_t
cprng_fast_word(struct cprng_fast *cprng)
{
	uint32_t v;

	if (__predict_true(0 < cprng->buffer[CPRNG_FAST_BUFIDX])) {
		v = cprng->buffer[--cprng->buffer[CPRNG_FAST_BUFIDX]];
	} else {
		/* If we don't have enough words, refill the buffer.  */
		crypto_core(cprng->buffer, cprng->nonce, cprng->key,
		    crypto_core_constant32);
		if (__predict_false(++cprng->nonce[0] == 0)) {
			cprng->nonce[1]++;
			cprng_fast_schedule_reseed(cprng);
		}
		v = cprng->buffer[CPRNG_FAST_BUFIDX];
		cprng->buffer[CPRNG_FAST_BUFIDX] = CPRNG_FAST_BUFIDX;
	}

	return v;
}

static inline void
cprng_fast_buf(struct cprng_fast *cprng, void *buf, unsigned n)
{
	uint8_t *p = buf;
	uint32_t v;
	unsigned w, r;

	w = n / sizeof(uint32_t);
	while (w--) {
		v = cprng_fast_word(cprng);
		(void)memcpy(p, &v, 4);
		p += 4;
	}

	r = n % sizeof(uint32_t);
	if (r) {
		v = cprng_fast_word(cprng);
		while (r--) {
			*p++ = (v & 0xff);
			v >>= 8;
		}
	}
}

/*
 * crypto_onetimestream: Expand a short unpredictable one-time seed
 * into a long unpredictable output.
 */
static void
crypto_onetimestream(const uint32_t seed[crypto_core_KEYWORDS], void *buf,
    size_t n)
{
	uint32_t block[crypto_core_OUTPUTWORDS];
	uint32_t nonce[crypto_core_INPUTWORDS] = {0};
	uint8_t *p8;
	uint32_t *p32;
	size_t ni, nb, nf;

	/*
	 * Guarantee we can generate up to n bytes.  We have
	 * 2^(32*INPUTWORDS) possible inputs yielding output of
	 * 4*OUTPUTWORDS*2^(32*INPUTWORDS) bytes.  It suffices to
	 * require that sizeof n > (1/CHAR_BIT) log_2 n be less than
	 * (1/CHAR_BIT) log_2 of the total output stream length.  We
	 * have
	 *
	 *	log_2 (4 o 2^(32 i)) = log_2 (4 o) + log_2 2^(32 i)
	 *	  = 2 + log_2 o + 32 i.
	 */
	__CTASSERT(CHAR_BIT*sizeof n <=
	    (2 + ilog2(crypto_core_OUTPUTWORDS) + 32*crypto_core_INPUTWORDS));

	p8 = buf;
	p32 = (uint32_t *)roundup2((uintptr_t)p8, sizeof(uint32_t));
	ni = (uint8_t *)p32 - p8;
	if (n < ni)
		ni = n;
	nb = (n - ni) / sizeof block;
	nf = (n - ni) % sizeof block;

	KASSERT(((uintptr_t)p32 & 3) == 0);
	KASSERT(ni <= n);
	KASSERT(nb <= (n / sizeof block));
	KASSERT(nf <= n);
	KASSERT(n == (ni + (nb * sizeof block) + nf));
	KASSERT(ni < sizeof(uint32_t));
	KASSERT(nf < sizeof block);

	if (ni) {
		crypto_core(block, nonce, seed, crypto_core_constant32);
		nonce[0]++;
		(void)memcpy(p8, block, ni);
	}
	while (nb--) {
		crypto_core(p32, nonce, seed, crypto_core_constant32);
		if (++nonce[0] == 0)
			nonce[1]++;
		p32 += crypto_core_OUTPUTWORDS;
	}
	if (nf) {
		crypto_core(block, nonce, seed, crypto_core_constant32);
		if (++nonce[0] == 0)
			nonce[1]++;
		(void)memcpy(p32, block, nf);
	}

	if (ni | nf)
		(void)explicit_memset(block, 0, sizeof block);
}

/* Public API */

uint32_t
cprng_fast32(void)
{
	struct cprng_fast *cprng;
	uint32_t v;
	int s;

	s = cprng_fast_get(&cprng);
	v = cprng_fast_word(cprng);
	cprng_fast_put(cprng, s);

	return v;
}

uint64_t
cprng_fast64(void)
{
	struct cprng_fast *cprng;
	uint32_t hi, lo;
	int s;

	s = cprng_fast_get(&cprng);
	hi = cprng_fast_word(cprng);
	lo = cprng_fast_word(cprng);
	cprng_fast_put(cprng, s);

	return ((uint64_t)hi << 32) | lo;
}

static void
cprng_fast_buf_short(void *buf, size_t len)
{
	struct cprng_fast *cprng;
	int s;

	s = cprng_fast_get(&cprng);
	cprng_fast_buf(cprng, buf, len);
	cprng_fast_put(cprng, s);
}

static __noinline void
cprng_fast_buf_long(void *buf, size_t len)
{
	uint32_t seed[crypto_core_KEYWORDS];
	struct cprng_fast *cprng;
	int s;

	s = cprng_fast_get(&cprng);
	cprng_fast_buf(cprng, seed, sizeof seed);
	cprng_fast_put(cprng, s);

	crypto_onetimestream(seed, buf, len);

	(void)explicit_memset(seed, 0, sizeof seed);
}

size_t
cprng_fast(void *buf, size_t len)
{

	/*
	 * We don't want to hog the CPU, so we use the short version,
	 * to generate output without preemption, only if we can do it
	 * with at most one crypto_core.
	 */
	if (len <= (sizeof(uint32_t) * crypto_core_OUTPUTWORDS))
		cprng_fast_buf_short(buf, len);
	else
		cprng_fast_buf_long(buf, len);

	return len;
}
