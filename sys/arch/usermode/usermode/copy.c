/* $NetBSD: copy.c,v 1.9 2019/04/06 03:06:28 thorpej Exp $ */

/*-
 * Copyright (c) 2007 Jared D. McNeill <jmcneill@invisible.ca>
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
__KERNEL_RCSID(0, "$NetBSD: copy.c,v 1.9 2019/04/06 03:06:28 thorpej Exp $");

#define	__UFETCHSTORE_PRIVATE
#define	__UCAS_PRIVATE

#include <sys/types.h>
#include <sys/systm.h>
#include <machine/thunk.h>

/* XXX until strnlen(3) has been added to the kernel, we *could* panic on it */
#define strnlen(str, maxlen) uimin(strlen((str)), maxlen)

int
copyin(const void *uaddr, void *kaddr, size_t len)
{
//	thunk_printf("copyin uaddr %p, kaddr %p, len %d\n", uaddr, kaddr, (int) len);
	memcpy(kaddr, uaddr, len);
	return 0;
}

int
copyout(const void *kaddr, void *uaddr, size_t len)
{
//	thunk_printf("copyout kaddr %p, uaddr %p, len %d\n", kaddr, uaddr, (int) len);
	memcpy(uaddr, kaddr, len);
	return 0;
}

int
copyinstr(const void *uaddr, void *kaddr, size_t len, size_t *done)
{
	len = uimin(strnlen(uaddr, len), len) + 1;
	strncpy(kaddr, uaddr, len);
	if (done)
		*done = len;
	return 0;
}

int
copyoutstr(const void *kaddr, void *uaddr, size_t len, size_t *done)
{
	len = uimin(strnlen(kaddr, len), len) + 1;
	strncpy(uaddr, kaddr, len);
	if (done)
		*done = len;
	return 0;
}

int
copystr(const void *kfaddr, void *kdaddr, size_t len, size_t *done)
{
	len = uimin(strnlen(kfaddr, len), len) + 1;
	strncpy(kdaddr, kfaddr, len);
	if (done)
		*done = len;
	return 0;
}

int
kcopy(const void *src, void *dst, size_t len)
{
	memcpy(dst, src, len);
#ifdef DEBUG
	if (memcmp(dst, src, len) != 0)
		panic("kcopy not finished correctly\n");
#endif
	return 0;
}

int
_ucas_32(volatile uint32_t *uaddr, uint32_t old, uint32_t new, uint32_t *ret)
{
	*ret = atomic_cas_32(uaddr, old, new);
}

#ifdef _LP64
int
_ucas_64(volatile uint64_t *uaddr, uint64_t old, uint64_t new, uint64_t *ret)
{
	*ret = atomic_cas_64(uaddr, old, new);
}
#endif /* _LP64 */

int
_ufetch_8(const uint8_t *uaddr, uint8_t *valp)
{
	*valp = *uaddr;
	return 0
}

int
_ufetch_16(const uint16_t *uaddr, uint16_t *valp)
{
	*valp = *uaddr;
	return 0
}

int
_ufetch_32(const uint32_t *uaddr, uint32_t *valp)
{
	*valp = *uaddr;
	return 0
}

#ifdef _LP64
int
_ufetch_64(const uint64_t *uaddr, uint64_t *valp)
{
	*valp = *uaddr;
	return 0
}
#endif /* _LP64 */

int
_ustore_8(uint8_t *uaddr, uint8_t val)
{
	*uaddr = val;
	return 0
}

int
_ustore_16(uint16_t *uaddr, uint16_t val)
{
	*uaddr = val;
	return 0
}

int
_ustore_32(uint32_t *uaddr, uint32_t val)
{
	*uaddr = val;
	return 0
}

#ifdef _LP64
int
_ustore_64(uint64_t *uaddr, uint64_t val)
{
	*uaddr = val;
	return 0
}
#endif /* _LP64 */
