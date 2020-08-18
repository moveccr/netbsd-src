/*	$NetBSD: entropy.h,v 1.4 2020/08/14 00:53:16 riastradh Exp $	*/

/*-
 * Copyright (c) 2019 The NetBSD Foundation, Inc.
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

#ifndef	_SYS_ENTROPY_H
#define	_SYS_ENTROPY_H

#ifndef _KERNEL
#error This header is known to the state of California to cause cancer in users.
#endif

#include <sys/types.h>

#include <lib/libkern/entpool.h>

struct knote;

#define	ENTROPY_CAPACITY	ENTPOOL_CAPACITY	/* bytes */

#define	ENTROPY_WAIT		0x01
#define	ENTROPY_SIG		0x02
#define	ENTROPY_HARDFAIL	0x04

void	entropy_bootrequest(void);
void	entropy_consolidate(void);
unsigned entropy_epoch(void);
bool	entropy_ready(void);
int	entropy_extract(void *, size_t, int);
int	entropy_poll(int);
int	entropy_kqfilter(struct knote *);
int	entropy_ioctl(unsigned long, void *);

#endif	/* _SYS_ENTROPY_H */
