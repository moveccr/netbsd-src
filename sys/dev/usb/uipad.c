/*	$NetBSD: uipad.c,v 1.9 2020/01/07 06:42:26 maxv Exp $	*/

/*-
 * Copyright (c) 2011 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Christos Zoulas.
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
__KERNEL_RCSID(0, "$NetBSD: uipad.c,v 1.9 2020/01/07 06:42:26 maxv Exp $");

#ifdef _KERNEL_OPT
#include "opt_usb.h"
#endif

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/device.h>
#include <sys/ioctl.h>
#include <sys/conf.h>
#include <sys/file.h>
#include <sys/select.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/poll.h>
#include <sys/bus.h>

#include <dev/usb/usb.h>
#include <dev/usb/usbdi.h>
#include <dev/usb/usbdi_util.h>
#include <dev/usb/usbdivar.h>

#include <dev/usb/usbdevs.h>

#ifdef UIPAD_DEBUG
#define DPRINTF(x)	if (uipaddebug) printf x
#define DPRINTFN(n, x)	if (uipaddebug > n) printf x
int	uipaddebug = 0;
#else
#define DPRINTF(x)
#define DPRINTFN(n, x)
#endif

struct uipad_softc {
	device_t		sc_dev;
	struct usbd_device *	sc_udev;
};

static const struct usb_devno uipad_devs[] = {
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPAD },
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPAD_2 },
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPAD_3 },
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPAD_MINI },
#if 0
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPHONE },
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPHONE_3G },
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPHONE_3GS },
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPHONE_4 },
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPHONE_4_VZW },
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPHONE_4S },
	{ USB_VENDOR_APPLE, USB_PRODUCT_APPLE_IPHONE_5 },
#endif
};

#define uipad_lookup(v, p) usb_lookup(uipad_devs, v, p)

static int	uipad_match(device_t, cfdata_t, void *);
static void	uipad_attach(device_t, device_t, void *);
static int	uipad_detach(device_t, int);

CFATTACH_DECL_NEW(uipad, sizeof(struct uipad_softc), uipad_match,
    uipad_attach, uipad_detach, NULL);

static void
uipad_cmd(struct uipad_softc *sc, uint8_t requestType, uint8_t reqno,
    uint16_t value, uint16_t index)
{
	usb_device_request_t req;
	usbd_status err;

	DPRINTF(("ipad cmd type=%x, number=%x, value=%d, index=%d\n",
	    requestType, reqno, value, index));
        req.bmRequestType = requestType;
        req.bRequest = reqno;
        USETW(req.wValue, value);
        USETW(req.wIndex, index);
        USETW(req.wLength, 0);

        if ((err = usbd_do_request(sc->sc_udev, &req, NULL)) != 0)
		aprint_error_dev(sc->sc_dev, "sending command failed %d\n",
		    err);
}

static void
uipad_charge(struct uipad_softc *sc)
{
	if (sc->sc_udev->ud_power != USB_MAX_POWER)
		uipad_cmd(sc, UT_VENDOR | UT_WRITE, 0x40, 0x6400, 0x6400);
}

static int
uipad_match(device_t parent, cfdata_t match, void *aux)
{
	struct usb_attach_arg *uaa = aux;

	DPRINTFN(50, ("uipad_match\n"));
	return uipad_lookup(uaa->uaa_vendor, uaa->uaa_product) != NULL ?
	    UMATCH_VENDOR_PRODUCT : UMATCH_NONE;
}

static void
uipad_attach(device_t parent, device_t self, void *aux)
{
	struct uipad_softc *sc = device_private(self);
	struct usb_attach_arg *uaa = aux;
	struct usbd_device *	dev = uaa->uaa_device;
	char			*devinfop;

	DPRINTFN(10,("uipad_attach: sc=%p\n", sc));

	sc->sc_dev = self;
	sc->sc_udev = dev;

	aprint_naive("\n");
	aprint_normal("\n");

	devinfop = usbd_devinfo_alloc(dev, 0);
	aprint_normal_dev(self, "%s\n", devinfop);
	usbd_devinfo_free(devinfop);

	uipad_charge(sc);

	DPRINTFN(10, ("uipad_attach: %p\n", sc->sc_udev));

	if (!pmf_device_register(self, NULL, NULL))
		aprint_error_dev(self, "couldn't establish power handler\n");

	usbd_add_drv_event(USB_EVENT_DRIVER_ATTACH, sc->sc_udev, sc->sc_dev);
	return;
}

static int
uipad_detach(device_t self, int flags)
{
	struct uipad_softc *sc = device_private(self);
	DPRINTF(("uipad_detach: sc=%p flags=%d\n", sc, flags));

	pmf_device_deregister(self);

	usbd_add_drv_event(USB_EVENT_DRIVER_DETACH, sc->sc_udev, sc->sc_dev);

	return 0;
}
