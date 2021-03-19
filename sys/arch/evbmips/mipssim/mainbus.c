/* $NetBSD: mainbus.c,v 1.2 2021/02/15 22:39:46 reinoud Exp $ */

/*-
 * Copyright (c) 2021 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Simon Burge.
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
__KERNEL_RCSID(0, "$NetBSD: mainbus.c,v 1.2 2021/02/15 22:39:46 reinoud Exp $");

#include <sys/param.h>
#include <sys/device.h>

#include <evbmips/mipssim/autoconf.h>
#include <evbmips/mipssim/mipssimreg.h>
#include <evbmips/mipssim/mipssimvar.h>

static int	mainbus_match(device_t, cfdata_t, void *);
static void	mainbus_attach(device_t, device_t, void *);
static int	mainbus_print(void *, const char *);

CFATTACH_DECL_NEW(mainbus, 0,
    mainbus_match, mainbus_attach, NULL, NULL);

/* There can be only one. */
static int	mainbus_found;

struct mainbusdev {
	const char *md_name;
	paddr_t md_addr;
	int md_irq;
};

static struct mainbusdev mainbusdevs[] = {
	{ "cpu",					},
	{ "com",	MIPSSIM_UART0_ADDR,	2	},
#ifdef notyet
	{ "mipsnet",	MIPSSIM_MIPSNET0_ADDR,	0	},
#endif
	{ NULL,		}
};

static int
mainbus_match(device_t parent, cfdata_t match, void *aux)
{

	if (mainbus_found)
		return (0);

	return (1);
}

static void
mainbus_attach(device_t parent, device_t self, void *aux)
{
	struct mainbus_attach_args maa;
	struct mipssim_config *mcp = &mipssim_configuration;
	const struct mainbusdev *md;

	mainbus_found = 1;
	printf("\n");

	/* attach children */
	for (md = mainbusdevs; md->md_name != NULL; md++) {
		maa.ma_name = md->md_name;
		maa.ma_addr = md->md_addr;
		maa.ma_irq = md->md_irq;
		maa.ma_iot = &mcp->mc_iot;
		maa.ma_dmat = &mcp->mc_dmat;
		config_found_ia(self, "mainbus", &maa, mainbus_print);
	}

	/* attach virtio children */
	for (int i = 0; i < VIRTIO_NUM_TRANSPORTS; i++) {
		maa.ma_name = "virtio";
		maa.ma_addr = MIPSSIM_VIRTIO_ADDR + VIRTIO_STRIDE * i;
		maa.ma_irq  = 1;
		maa.ma_iot  = &mcp->mc_iot;
		maa.ma_dmat = &mcp->mc_dmat;
		config_found_ia(self, "mainbus", &maa, mainbus_print);
	}
}

static int
mainbus_print(void *aux, const char *pnp)
{

	return (QUIET);
}
