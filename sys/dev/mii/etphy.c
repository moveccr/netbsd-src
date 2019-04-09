/*	$NetBSD: etphy.c,v 1.4 2019/03/25 09:29:08 msaitoh Exp $	*/
/*	$OpenBSD: etphy.c,v 1.4 2008/04/02 20:12:58 brad Exp $	*/

/*
 * Copyright (c) 2007 The DragonFly Project.  All rights reserved.
 * 
 * This code is derived from software contributed to The DragonFly Project
 * by Sepherosa Ziehau <sepherosa@gmail.com>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of The DragonFly Project nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific, prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * $DragonFly: src/sys/dev/netif/mii_layer/truephy.c,v 1.1 2007/10/12 14:12:42 sephe Exp $
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: etphy.c,v 1.4 2019/03/25 09:29:08 msaitoh Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/device.h>
#include <sys/socket.h>

#include <net/if.h>
#include <net/if_media.h>

#include <dev/mii/mii.h>
#include <dev/mii/miivar.h>
#include <dev/mii/miidevs.h>

#define ETPHY_INDEX		0x10	/* XXX reserved in DS */
#define ETPHY_INDEX_MAGIC	0x402
#define ETPHY_DATA		0x11	/* XXX reserved in DS */

#define ETPHY_CTRL		0x12
#define ETPHY_CTRL_DIAG		0x0004
#define ETPHY_CTRL_RSV1		0x0002	/* XXX reserved */
#define ETPHY_CTRL_RSV0		0x0001	/* XXX reserved */

#define ETPHY_CONF		0x16
#define ETPHY_CONF_TXFIFO_MASK	0x3000
#define ETPHY_CONF_TXFIFO_8	0x0000
#define ETPHY_CONF_TXFIFO_16	0x1000
#define ETPHY_CONF_TXFIFO_24	0x2000
#define ETPHY_CONF_TXFIFO_32	0x3000

#define ETPHY_SR		0x1a
#define ETPHY_SR_SPD_MASK	0x0300
#define ETPHY_SR_SPD_1000T	0x0200
#define ETPHY_SR_SPD_100TX	0x0100
#define ETPHY_SR_SPD_10T	0x0000
#define ETPHY_SR_FDX		0x0080


int	etphy_service(struct mii_softc *, struct mii_data *, int);
void	etphy_attach(device_t, device_t, void *);
int	etphy_match(device_t, cfdata_t, void *);
void	etphy_reset(struct mii_softc *);
void	etphy_status(struct mii_softc *);

const struct mii_phy_funcs etphy_funcs = {
	etphy_service, etphy_status, etphy_reset,
};

static const struct mii_phydesc etphys[] = {
	MII_PHY_DESC(AGERE, ET1011),
	MII_PHY_END,
};

CFATTACH_DECL_NEW(etphy, sizeof(struct mii_softc),
	etphy_match, etphy_attach, mii_phy_detach, mii_phy_activate);

static const struct etphy_dsp {
	uint16_t	index;
	uint16_t	data;
} etphy_dspcode[] = {
	{ 0x880b,	0x0926 },	/* AfeIfCreg4B1000Msbs */
	{ 0x880c,	0x0926 },	/* AfeIfCreg4B100Msbs */
	{ 0x880d,	0x0926 },	/* AfeIfCreg4B10Msbs */

	{ 0x880e,	0xb4d3 },	/* AfeIfCreg4B1000Lsbs */
	{ 0x880f,	0xb4d3 },	/* AfeIfCreg4B100Lsbs */
	{ 0x8810,	0xb4d3 },	/* AfeIfCreg4B10Lsbs */

	{ 0x8805,	0xb03e },	/* AfeIfCreg3B1000Msbs */
	{ 0x8806,	0xb03e },	/* AfeIfCreg3B100Msbs */
	{ 0x8807,	0xff00 },	/* AfeIfCreg3B10Msbs */

	{ 0x8808,	0xe090 },	/* AfeIfCreg3B1000Lsbs */
	{ 0x8809,	0xe110 },	/* AfeIfCreg3B100Lsbs */
	{ 0x880a,	0x0000 },	/* AfeIfCreg3B10Lsbs */

	{ 0x300d,	1      },	/* DisableNorm */

	{ 0x280c,	0x0180 },	/* LinkHoldEnd */

	{ 0x1c21,	0x0002 },	/* AlphaM */

	{ 0x3821,	6      },	/* FfeLkgTx0 */
	{ 0x381d,	1      },	/* FfeLkg1g4 */
	{ 0x381e,	1      },	/* FfeLkg1g5 */
	{ 0x381f,	1      },	/* FfeLkg1g6 */
	{ 0x3820,	1      },	/* FfeLkg1g7 */

	{ 0x8402,	0x01f0 },	/* Btinact */
	{ 0x800e,	20     },	/* LftrainTime */
	{ 0x800f,	24     },	/* DvguardTime */
	{ 0x8010,	46     }	/* IdlguardTime */
};

int
etphy_match(device_t parent, cfdata_t match, void *aux)
{
	struct mii_attach_args *ma = aux;

	if (mii_phy_match(ma, etphys) != NULL)
		return 10;

	return 0;
}

void
etphy_attach(device_t parent, device_t self, void *aux)
{
	struct mii_softc *sc = device_private(self);
	struct mii_attach_args *ma = aux;
	struct mii_data *mii = ma->mii_data;
	const struct mii_phydesc *mpd;

	mpd = mii_phy_match(ma, etphys);
	aprint_normal(": %s, rev. %d\n", mpd->mpd_name, MII_REV(ma->mii_id2));

	sc->mii_dev = self;
	sc->mii_inst = mii->mii_instance;
	sc->mii_phy = ma->mii_phyno;
	sc->mii_funcs = &etphy_funcs;
	sc->mii_mpd_model = MII_MODEL(ma->mii_id2);
	sc->mii_pdata = mii;
	sc->mii_flags = ma->mii_flags;

	sc->mii_flags |= MIIF_NOISOLATE | MIIF_NOLOOP;

	PHY_RESET(sc);

	PHY_READ(sc, MII_BMSR, &sc->mii_capabilities);
	sc->mii_capabilities &= ma->mii_capmask;
	if (sc->mii_capabilities & BMSR_EXTSTAT) {
		PHY_READ(sc, MII_EXTSR, &sc->mii_extcapabilities);
		/* No 1000baseT half-duplex support */
		sc->mii_extcapabilities &= ~EXTSR_1000THDX;
	}
	aprint_normal_dev(self, "");
	if ((sc->mii_capabilities & BMSR_MEDIAMASK) == 0)
		aprint_error("no media present");
	else
		mii_phy_add_media(sc);
	aprint_normal("\n");

	if (!pmf_device_register(self, NULL, mii_phy_resume))
		aprint_error_dev(self, "couldn't establish power handler\n");
}

int
etphy_service(struct mii_softc *sc, struct mii_data *mii, int cmd)
{
	struct ifmedia_entry *ife = mii->mii_media.ifm_cur;
	uint16_t bmcr;

	switch (cmd) {
	case MII_POLLSTAT:
		/* If we're not polling our PHY instance, just return. */
		if (IFM_INST(ife->ifm_media) != sc->mii_inst)
			return 0;
		break;

	case MII_MEDIACHG:
		/*
		 * If the media indicates a different PHY instance,
		 * isolate ourselves.
		 */
		if (IFM_INST(ife->ifm_media) != sc->mii_inst) {
			PHY_READ(sc, MII_BMCR, &bmcr);
			PHY_WRITE(sc, MII_BMCR, bmcr | BMCR_ISO);
			return 0;
		}

		/* If the interface is not up, don't do anything. */
		if ((mii->mii_ifp->if_flags & IFF_UP) == 0)
			break;

		if (IFM_SUBTYPE(ife->ifm_media) != IFM_AUTO) {
			PHY_READ(sc, MII_BMCR, &bmcr);
			bmcr &= ~BMCR_AUTOEN;
			PHY_WRITE(sc, MII_BMCR, bmcr);
			PHY_WRITE(sc, MII_BMCR, bmcr | BMCR_PDOWN);
		}

		mii_phy_setmedia(sc);

		if (IFM_SUBTYPE(ife->ifm_media) != IFM_AUTO) {
			PHY_READ(sc, MII_BMCR, &bmcr);
			bmcr &= ~BMCR_PDOWN;
			PHY_WRITE(sc, MII_BMCR, bmcr);

			if (IFM_SUBTYPE(ife->ifm_media) == IFM_1000_T) {
				PHY_WRITE(sc, MII_BMCR,
					  bmcr | BMCR_AUTOEN | BMCR_STARTNEG);
			}
		}
		break;

	case MII_TICK:
		/* If we're not currently selected, just return. */
		if (IFM_INST(ife->ifm_media) != sc->mii_inst)
			return 0;

		if (mii_phy_tick(sc) == EJUSTRETURN)
			return 0;
		break;
	}

	/* Update the media status. */
	mii_phy_status(sc);

	/* Callback if something changed. */
	mii_phy_update(sc, cmd);
	return 0;
}

void
etphy_reset(struct mii_softc *sc)
{
	uint16_t reg;
	int i;

	for (i = 0; i < 2; ++i) {
		PHY_READ(sc, MII_PHYIDR1, &reg);
		PHY_READ(sc, MII_PHYIDR2, &reg);

		PHY_READ(sc, ETPHY_CTRL, &reg);
		PHY_WRITE(sc, ETPHY_CTRL,
		    ETPHY_CTRL_DIAG | ETPHY_CTRL_RSV1);

		PHY_WRITE(sc, ETPHY_INDEX, ETPHY_INDEX_MAGIC);
		PHY_READ(sc, ETPHY_DATA, &reg);

		PHY_WRITE(sc, ETPHY_CTRL, ETPHY_CTRL_RSV1);
	}

	PHY_READ(sc, MII_BMCR, &reg);
	PHY_READ(sc, ETPHY_CTRL, &reg);
	PHY_WRITE(sc, MII_BMCR, BMCR_AUTOEN | BMCR_PDOWN | BMCR_S1000);
	PHY_WRITE(sc, ETPHY_CTRL,
	    ETPHY_CTRL_DIAG | ETPHY_CTRL_RSV1 | ETPHY_CTRL_RSV0);

#define N(arr)	(int)(sizeof(arr) / sizeof(arr[0]))

	for (i = 0; i < N(etphy_dspcode); ++i) {
		const struct etphy_dsp *dsp = &etphy_dspcode[i];

		PHY_WRITE(sc, ETPHY_INDEX, dsp->index);
		PHY_WRITE(sc, ETPHY_DATA, dsp->data);

		PHY_WRITE(sc, ETPHY_INDEX, dsp->index);
		PHY_READ(sc, ETPHY_DATA, &reg);
	}

#undef N

	PHY_READ(sc, MII_BMCR, &reg);
	PHY_READ(sc, ETPHY_CTRL, &reg);
	PHY_WRITE(sc, MII_BMCR, BMCR_AUTOEN |  BMCR_S1000);
	PHY_WRITE(sc, ETPHY_CTRL, ETPHY_CTRL_RSV1);

	mii_phy_reset(sc);
}

void
etphy_status(struct mii_softc *sc)
{
	struct mii_data *mii = sc->mii_pdata;
	uint16_t bmsr, bmcr, sr;

	mii->mii_media_status = IFM_AVALID;
	mii->mii_media_active = IFM_ETHER;

	PHY_READ(sc, ETPHY_SR, &sr);
	PHY_READ(sc, MII_BMCR, &bmcr);

	PHY_READ(sc, MII_BMSR, &bmsr);
	PHY_READ(sc, MII_BMSR, &bmsr);
	if (bmsr & BMSR_LINK)
		mii->mii_media_status |= IFM_ACTIVE;

	if (bmcr & BMCR_AUTOEN) {
		if ((bmsr & BMSR_ACOMP) == 0) {
			mii->mii_media_active |= IFM_NONE;
			return;
		}
	}

	switch (sr & ETPHY_SR_SPD_MASK) {
	case ETPHY_SR_SPD_1000T:
		mii->mii_media_active |= IFM_1000_T;
		break;
	case ETPHY_SR_SPD_100TX:
		mii->mii_media_active |= IFM_100_TX;
		break;
	case ETPHY_SR_SPD_10T:
		mii->mii_media_active |= IFM_10_T;
		break;
	default:
		mii->mii_media_active |= IFM_NONE;
		return;
	}

	if (sr & ETPHY_SR_FDX)
		mii->mii_media_active |= IFM_FDX;
	else
		mii->mii_media_active |= IFM_HDX;
}
