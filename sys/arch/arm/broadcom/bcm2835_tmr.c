/*	$NetBSD: bcm2835_tmr.c,v 1.10 2019/06/07 13:43:44 skrll Exp $	*/

/*-
 * Copyright (c) 2012 The NetBSD Foundation, Inc.
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
__KERNEL_RCSID(0, "$NetBSD: bcm2835_tmr.c,v 1.10 2019/06/07 13:43:44 skrll Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <sys/kernel.h>
#include <sys/timetc.h>
#include <sys/bus.h>

#include <arm/broadcom/bcm2835_intr.h>
#include <arm/broadcom/bcm2835reg.h>
#include <arm/broadcom/bcm2835var.h>

#include <dev/fdt/fdtvar.h>

#include <arm/fdt/arm_fdtvar.h>

/* Use the 3rd timer*/
#define BCMTIMER	3

#define	BCM2835_STIMER_CS	0x00
#define	 BCM2835_STIMER_M0	 __BIT(0)
#define	 BCM2835_STIMER_M1	 __BIT(1)
#define	 BCM2835_STIMER_M2	 __BIT(2)
#define	 BCM2835_STIMER_M3	 __BIT(3)
#define	BCM2835_STIMER_CLO	0x04
#define	BCM2835_STIMER_CHI	0x08
#define	BCM2835_STIMER_C0	0x0c
#define	BCM2835_STIMER_C1	0x10
#define	BCM2835_STIMER_C2	0x14
#define	BCM2835_STIMER_C3	0x18

#define	BCM2835_STIMER_HZ	1000000

static const uint32_t counts_per_usec = (BCM2835_STIMER_HZ / 1000000);
static uint32_t counts_per_hz = ~0;

struct bcm2835tmr_softc {
	device_t		sc_dev;

	bus_space_tag_t sc_iot;
	bus_space_handle_t sc_ioh;

	void *sc_ih;
};

static int bcmtmr_match(device_t, cfdata_t, void *);
static void bcmtmr_attach(device_t, device_t, void *);

static int clockhandler(void *);

static u_int bcm2835tmr_get_timecount(struct timecounter *);
void bcm2835_tmr_setstatclockrate(int);

static struct bcm2835tmr_softc *bcm2835tmr_sc;

static struct timecounter bcm2835tmr_timecounter = {
	.tc_get_timecount = bcm2835tmr_get_timecount,
	.tc_poll_pps = 0,
	.tc_counter_mask = ~0u,
	.tc_frequency = BCM2835_STIMER_HZ,
	.tc_name = NULL,			/* set by attach */
	.tc_quality = 100,
	.tc_priv = NULL,
	.tc_next = NULL,
};

CFATTACH_DECL_NEW(bcmtmr_fdt, sizeof(struct bcm2835tmr_softc),
    bcmtmr_match, bcmtmr_attach, NULL, NULL);

/* ARGSUSED */
static int
bcmtmr_match(device_t parent, cfdata_t match, void *aux)
{
	const char * const compatible[] = {
	    "brcm,bcm2835-system-timer",
	    NULL
	};
	struct fdt_attach_args * const faa = aux;
	return of_match_compatible(faa->faa_phandle, compatible);
}

static void
bcmtmr_attach(device_t parent, device_t self, void *aux)
{
	struct bcm2835tmr_softc *sc = device_private(self);
	struct fdt_attach_args * const faa = aux;

	aprint_naive("\n");
	aprint_normal(": VC System Timer\n");

	if (bcm2835tmr_sc == NULL)
		bcm2835tmr_sc = sc;

	sc->sc_dev = self;
	sc->sc_iot = faa->faa_bst;
	const int phandle = faa->faa_phandle;

	bus_addr_t addr;
	bus_size_t size;

	if (fdtbus_get_reg(phandle, 0, &addr, &size) != 0) {
		aprint_error(": missing 'reg' property\n");
		return;
	}

	if (bus_space_map(sc->sc_iot, addr, size, 0, &sc->sc_ioh) != 0) {
		aprint_error_dev(sc->sc_dev, "unable to map device\n");
		return;
	}

	char intrstr[128];
	if (!fdtbus_intr_str(phandle, BCMTIMER, intrstr, sizeof(intrstr))) {
		aprint_error(": failed to decode interrupt\n");
		return;
	}

	sc->sc_ih = fdtbus_intr_establish(phandle, BCMTIMER, IPL_CLOCK,
	    FDT_INTR_MPSAFE, clockhandler, NULL);
	if (sc->sc_ih == NULL) {
		aprint_error(": failed to establish interrupt on %s\n",
		    intrstr);
		return;
	}
	aprint_normal_dev(self, "interrupting on %s\n", intrstr);

	bcm2835tmr_timecounter.tc_name = device_xname(self);
}

void
cpu_initclocks(void)
{
	struct bcm2835tmr_softc *sc = bcm2835tmr_sc;
	uint32_t stcl;

	KASSERT(sc != NULL);

	bcm2835tmr_timecounter.tc_priv = sc;

	counts_per_hz = BCM2835_STIMER_HZ / hz;

	stcl = bus_space_read_4(sc->sc_iot, sc->sc_ioh, BCM2835_STIMER_CLO);
	stcl += counts_per_hz;

	bus_space_write_4(sc->sc_iot, sc->sc_ioh, BCM2835_STIMER_C3, stcl);

	tc_init(&bcm2835tmr_timecounter);
}

void
bcm2835_tmr_delay(unsigned int n)
{
	struct bcm2835tmr_softc *sc = bcm2835tmr_sc;
	uint32_t last, curr;
	uint32_t delta, usecs;

	KASSERT(sc != NULL);

	last = bus_space_read_4(sc->sc_iot, sc->sc_ioh, BCM2835_STIMER_CLO);

	delta = usecs = 0;
	while (n > usecs) {
		curr = bus_space_read_4(sc->sc_iot, sc->sc_ioh,
		    BCM2835_STIMER_CLO);

		/* Check to see if the timer has wrapped around. */
		if (curr < last)
			delta += curr + (UINT32_MAX - last);
		else
			delta += curr - last;

		last = curr;

		if (delta >= counts_per_usec) {
			usecs += delta / counts_per_usec;
			delta %= counts_per_usec;
		}
	}
}

/*
 * clockhandler:
 *
 *	Handle the hardclock interrupt.
 */
static int
clockhandler(void *arg)
{
	struct bcm2835tmr_softc *sc = bcm2835tmr_sc;
	struct clockframe *frame = arg;
	uint32_t curr, status;

	status = bus_space_read_4(sc->sc_iot, sc->sc_ioh,
	    BCM2835_STIMER_CS);

	if (!(status & BCM2835_STIMER_M3))
		return 0;

	bus_space_write_4(sc->sc_iot, sc->sc_ioh, BCM2835_STIMER_CS,
	    BCM2835_STIMER_M3);

	hardclock(frame);

	curr = bus_space_read_4(sc->sc_iot, sc->sc_ioh, BCM2835_STIMER_CLO);

	curr += counts_per_hz;
	bus_space_write_4(sc->sc_iot, sc->sc_ioh, BCM2835_STIMER_C3, curr);

	return 1;
}

void
setstatclockrate(int newhz)
{
}

static u_int
bcm2835tmr_get_timecount(struct timecounter *tc)
{
	struct bcm2835tmr_softc *sc = tc->tc_priv;

	return bus_space_read_4(sc->sc_iot, sc->sc_ioh, BCM2835_STIMER_CLO);
}

