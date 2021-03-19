/* $NetBSD: a9ptmr_fdt.c,v 1.3 2021/01/27 03:10:19 thorpej Exp $ */

/*-
 * Copyright (c) 2019 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Nick Hudson
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
__KERNEL_RCSID(0, "$NetBSD: a9ptmr_fdt.c,v 1.3 2021/01/27 03:10:19 thorpej Exp $");

#include <sys/param.h>
#include <sys/bus.h>

#include <sys/device.h>
#include <sys/intr.h>

#include <arm/cortex/mpcore_var.h>
#include <arm/cortex/a9ptmr_var.h>

#include <dev/fdt/fdtvar.h>
#include <arm/fdt/arm_fdtvar.h>

static int	a9ptmr_fdt_match(device_t, cfdata_t, void *);
static void	a9ptmr_fdt_attach(device_t, device_t, void *);

static void	a9ptmr_fdt_cpu_hatch(void *, struct cpu_info *);

struct a9ptmr_fdt_softc {
	device_t	sc_dev;
	struct clk	*sc_clk;
};

CFATTACH_DECL_NEW(a9ptmr_fdt, sizeof(struct a9ptmr_fdt_softc),
    a9ptmr_fdt_match, a9ptmr_fdt_attach, NULL, NULL);

static const struct device_compatible_entry compat_data[] = {
	{ .compat = "arm,cortex-a9-twd-timer" },
	{ .compat = "arm,cortex-a5-twd-timer" },
	DEVICE_COMPAT_EOL
};

static int
a9ptmr_fdt_match(device_t parent, cfdata_t cf, void *aux)
{
	struct fdt_attach_args * const faa = aux;

	return of_compatible_match(faa->faa_phandle, compat_data);
}

static void
a9ptmr_fdt_attach(device_t parent, device_t self, void *aux)
{
	struct a9ptmr_fdt_softc * const sc = device_private(self);
	struct fdt_attach_args * const faa = aux;
	const int phandle = faa->faa_phandle;
	bus_space_handle_t bsh;

	sc->sc_dev = self;
	sc->sc_clk = fdtbus_clock_get_index(phandle, 0);
	if (sc->sc_clk == NULL) {
		aprint_error(": couldn't get clock\n");
		return;
	}
	if (clk_enable(sc->sc_clk) != 0) {
		aprint_error(": couldn't enable clock\n");
		return;
	}

	uint32_t rate = clk_get_rate(sc->sc_clk);
	prop_dictionary_t dict = device_properties(self);
	prop_dictionary_set_uint32(dict, "frequency", rate);

	char intrstr[128];
	if (!fdtbus_intr_str(phandle, 0, intrstr, sizeof(intrstr))) {
		aprint_error(": failed to decode interrupt\n");
		return;
	}

	aprint_naive("\n");
	aprint_normal("\n");

	void *ih = fdtbus_intr_establish_xname(phandle, 0, IPL_CLOCK,
	    FDT_INTR_MPSAFE, a9ptmr_intr, NULL, device_xname(self));
	if (ih == NULL) {
		aprint_error_dev(self, "couldn't install interrupt handler\n");
		return;
	}
	aprint_normal_dev(self, "interrupting on %s\n", intrstr);

	bus_addr_t addr;
	bus_size_t size;
	if (fdtbus_get_reg(phandle, 0, &addr, &size) != 0) {
		aprint_error(": couldn't get registers\n");
		return;
	}
	if (bus_space_map(faa->faa_bst, addr, size, 0, &bsh)) {
		aprint_error(": couldn't map registers\n");
		return;
	}

	struct mpcore_attach_args mpcaa = {
		.mpcaa_name = "arma9ptmr",
		.mpcaa_memt = faa->faa_bst,
		.mpcaa_memh = bsh,
		.mpcaa_irq = -1,
	};

	config_found(self, &mpcaa, NULL);

	arm_fdt_cpu_hatch_register(self, a9ptmr_fdt_cpu_hatch);
	arm_fdt_timer_register(a9ptmr_cpu_initclocks);
}

static void
a9ptmr_fdt_cpu_hatch(void *priv, struct cpu_info *ci)
{
	a9ptmr_init_cpu_clock(ci);
}
