/* $NetBSD: pad.c,v 1.38 2017/07/01 05:50:10 nat Exp $ */

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
__KERNEL_RCSID(0, "$NetBSD: pad.c,v 1.38 2017/07/01 05:50:10 nat Exp $");

#include <sys/types.h>
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/buf.h>
#include <sys/kmem.h>
#include <sys/kernel.h>
#include <sys/device.h>
#include <sys/proc.h>
#include <sys/condvar.h>
#include <sys/select.h>
#include <sys/audioio.h>
#include <sys/vnode.h>
#include <sys/module.h>
#include <sys/atomic.h>
#include <sys/time.h>

#include <dev/audio_if.h>
#include <dev/audiovar.h>
#include <dev/auconv.h>
#if defined(AUDIO2)
#include <dev/audio/aufilter.h>
#else
#include <dev/auvolconv.h>
#endif

#include <dev/pad/padvar.h>

/* #define PAD_DEBUG */
#ifdef PAD_DEBUG
#define DPRINTF(fmt...)	printf(fmt)
#else
#define DPRINTF(fmt...) /**/
#endif

#define PADUNIT(x)	minor(x)

#define PADFREQ		44100
#define PADCHAN		2
#define PADPREC		16
#define PADENC		AUDIO_ENCODING_SLINEAR_LE

extern struct cfdriver pad_cd;

typedef struct pad_block {
	uint8_t		*pb_ptr;
	int		pb_len;
} pad_block_t;

enum {
	PAD_OUTPUT_CLASS,
	PAD_INPUT_CLASS,
	PAD_OUTPUT_MASTER_VOLUME,
	PAD_INPUT_DAC_VOLUME,
	PAD_ENUM_LAST,
};

static int	pad_match(device_t, cfdata_t, void *);
static void	pad_attach(device_t, device_t, void *);
static int	pad_detach(device_t, int);
static void	pad_childdet(device_t, device_t);

static int	pad_query_encoding(void *, struct audio_encoding *);
static int	pad_set_params2(void *, int, int,
				const audio_params_t *, const audio_params_t *,
			    audio_filter_reg_t *, audio_filter_reg_t *);
static int	pad_start_output(void *, void *, int,
				    void (*)(void *), void *);
static int	pad_start_input(void *, void *, int,
				   void (*)(void *), void *);
static int	pad_halt_output(void *);
static int	pad_halt_input(void *);
static int	pad_getdev(void *, struct audio_device *);
static int	pad_set_port(void *, mixer_ctrl_t *);
static int	pad_get_port(void *, mixer_ctrl_t *);
static int	pad_query_devinfo(void *, mixer_devinfo_t *);
static int	pad_get_props(void *);
static void	pad_get_locks(void *, kmutex_t **, kmutex_t **);

static void	pad_done_output(void *);
static void	pad_swvol_codec(audio_filter_arg_t *);

static bool	pad_is_attached;	/* Do we have an audio* child? */

static const struct audio_hw_if pad_hw_if = {
	.query_encoding = pad_query_encoding,
	.set_params2 = pad_set_params2,
	.start_output = pad_start_output,
	.start_input = pad_start_input,
	.halt_output = pad_halt_output,
	.halt_input = pad_halt_input,
	.getdev = pad_getdev,
	.set_port = pad_set_port,
	.get_port = pad_get_port,
	.query_devinfo = pad_query_devinfo,
	.get_props = pad_get_props,
	.get_locks = pad_get_locks,
};

#define PAD_NFORMATS	1
#if defined(PAD_SUPPORT_RECORD)
#define PADMODE	(AUMODE_PLAY | AUMODE_RECORD)
#else
#define PADMODE	AUMODE_PLAY
#endif
static const struct audio_format pad_formats[PAD_NFORMATS] = {
	{ NULL, PADMODE, PADENC, PADPREC, PADPREC,
	  PADCHAN, AUFMT_STEREO, 1, { PADFREQ } },
};

extern void	padattach(int);

static int	pad_add_block(pad_softc_t *, uint8_t *, int);
static int	pad_get_block(pad_softc_t *, pad_block_t *, int);

dev_type_open(pad_dev_open);
dev_type_close(pad_dev_close);
dev_type_read(pad_dev_read);

const struct cdevsw pad_cdevsw = {
	.d_open = pad_dev_open,
	.d_close = pad_dev_close,
	.d_read = pad_dev_read,
	.d_write = nowrite,
	.d_ioctl = noioctl,
	.d_stop = nostop,
	.d_tty = notty,
	.d_poll = nopoll,
	.d_mmap = nommap,
	.d_kqfilter = nokqfilter,
	.d_discard = nodiscard,
	.d_flag = D_OTHER | D_MPSAFE,
};

CFATTACH_DECL2_NEW(pad, sizeof(pad_softc_t), pad_match, pad_attach, pad_detach,
    NULL, NULL, pad_childdet);

void
padattach(int n)
{
	int i, err;
	cfdata_t cf;

	aprint_debug("pad: requested %d units\n", n);
	DPRINTF("%s: requested %d units\n", __func__, n);

	err = config_cfattach_attach(pad_cd.cd_name, &pad_ca);
	if (err) {
		aprint_error("%s: couldn't register cfattach: %d\n",
		    pad_cd.cd_name, err);
		config_cfdriver_detach(&pad_cd);
		return;
	}

	for (i = 0; i < n; i++) {
		cf = kmem_alloc(sizeof(struct cfdata), KM_SLEEP);
		cf->cf_name = pad_cd.cd_name;
		cf->cf_atname = pad_cd.cd_name;
		cf->cf_unit = i;
		cf->cf_fstate = FSTATE_STAR;

		(void)config_attach_pseudo(cf);
	}

	return;
}

static int
pad_add_block(pad_softc_t *sc, uint8_t *blk, int blksize)
{
	int l;

	if (sc->sc_buflen + blksize > PAD_BUFSIZE)
		return ENOBUFS;

	if (sc->sc_wpos + blksize <= PAD_BUFSIZE)
		memcpy(sc->sc_audiobuf + sc->sc_wpos, blk, blksize);
	else {
		l = PAD_BUFSIZE - sc->sc_wpos;
		memcpy(sc->sc_audiobuf + sc->sc_wpos, blk, l);
		memcpy(sc->sc_audiobuf, blk + l, blksize - l);
	}

	sc->sc_wpos += blksize;
	if (sc->sc_wpos > PAD_BUFSIZE)
		sc->sc_wpos -= PAD_BUFSIZE;

	sc->sc_buflen += blksize;

	return 0;
}

static int
pad_get_block(pad_softc_t *sc, pad_block_t *pb, int blksize)
{
	int l;

	KASSERT(pb != NULL);

	pb->pb_ptr = (sc->sc_audiobuf + sc->sc_rpos);
	if (sc->sc_rpos + blksize < PAD_BUFSIZE) {
		pb->pb_len = blksize;
		sc->sc_rpos += blksize;
	} else {
		l = PAD_BUFSIZE - sc->sc_rpos;
		pb->pb_len = l;
		sc->sc_rpos = 0;
	}
	sc->sc_buflen -= pb->pb_len;

	return 0;
}

static int
pad_match(device_t parent, cfdata_t data, void *opaque)
{

	return 1;
}

static void
pad_childdet(device_t self, device_t child)
{
	pad_softc_t *sc = device_private(self);

	sc->sc_audiodev = NULL;
}

static void
pad_attach(device_t parent, device_t self, void *opaque)
{
	pad_softc_t *sc = device_private(self);

	aprint_normal_dev(self, "outputs: 44100Hz, 16-bit, stereo\n");

	sc->sc_dev = self;
	sc->sc_open = 0;
	if (auconv_create_encodings(pad_formats, PAD_NFORMATS,
	    &sc->sc_encodings) != 0) {
		aprint_error_dev(self, "couldn't create encodings\n");
		return;
	}

	cv_init(&sc->sc_condvar, device_xname(self));
	mutex_init(&sc->sc_cond_lock, MUTEX_DEFAULT, IPL_NONE);
	mutex_init(&sc->sc_lock, MUTEX_DEFAULT, IPL_NONE);
	mutex_init(&sc->sc_intr_lock, MUTEX_DEFAULT, IPL_NONE);
	callout_init(&sc->sc_pcallout, 0/*XXX?*/);

	sc->sc_swvol = 255;
	sc->sc_buflen = 0;
	sc->sc_rpos = sc->sc_wpos = 0;
	sc->sc_audiodev = audio_attach_mi(&pad_hw_if, sc, sc->sc_dev);

	if (!pmf_device_register(self, NULL, NULL))
		aprint_error_dev(self, "couldn't establish power handler\n");

	pad_is_attached = true;
	return;
}

static int
pad_detach(device_t self, int flags)
{
	pad_softc_t *sc = device_private(self);
	int cmaj, mn, rc;

	if (!pad_is_attached)
		return ENXIO;

	cmaj = cdevsw_lookup_major(&pad_cdevsw);
	mn = device_unit(self);
	vdevgone(cmaj, mn, mn, VCHR);

	if ((rc = config_detach_children(self, flags)) != 0)
		return rc;

	pmf_device_deregister(self);

	mutex_destroy(&sc->sc_lock);
	mutex_destroy(&sc->sc_intr_lock);
	cv_destroy(&sc->sc_condvar);
	callout_destroy(&sc->sc_pcallout);

	auconv_delete_encodings(sc->sc_encodings);

	pad_is_attached = false;
	return 0;
}

int
pad_dev_open(dev_t dev, int flags, int fmt, struct lwp *l)
{
	pad_softc_t *sc;

	sc = device_lookup_private(&pad_cd, PADUNIT(dev));
	if (sc == NULL)
		return ENXIO;

	if (atomic_swap_uint(&sc->sc_open, 1) != 0)
		return EBUSY;

	DPRINTF("%s -> %d\n", __func__, sc->sc_open);

	return 0;
}

int
pad_dev_close(dev_t dev, int flags, int fmt, struct lwp *l)
{
	pad_softc_t *sc;

	sc = device_lookup_private(&pad_cd, PADUNIT(dev));
	if (sc == NULL)
		return ENXIO;

	// こっちも atomic にしないとだめなんでは
	KASSERT(sc->sc_open > 0);
	sc->sc_open = 0;
	DPRINTF("%s -> %d\n", __func__, sc->sc_open);

	return 0;
}

int
pad_dev_read(dev_t dev, struct uio *uio, int flags)
{
	pad_softc_t *sc;
	pad_block_t pb;
	int len;
	int err;

	sc = device_lookup_private(&pad_cd, PADUNIT(dev));
	if (sc == NULL)
		return ENXIO;

	err = 0;
	DPRINTF("%s: resid=%zu\n", __func__, uio->uio_resid);
	while (uio->uio_resid > 0 && !err) {
		mutex_enter(&sc->sc_cond_lock);
		if (sc->sc_buflen == 0) {
			DPRINTF("%s: wait\n", __func__);
			err = cv_wait_sig(&sc->sc_condvar, &sc->sc_cond_lock);
			DPRINTF("%s: wake up %d\n", __func__, err);
			mutex_exit(&sc->sc_cond_lock);
			if (err) {
				if (err == ERESTART)
					err = EINTR;
				break;
			}
			if (sc->sc_buflen == 0)
				break;
			continue;
		}

		len = min(uio->uio_resid, sc->sc_buflen);
		err = pad_get_block(sc, &pb, len);
		mutex_exit(&sc->sc_cond_lock);
		if (err)
			break;
		DPRINTF("%s: move %d\n", __func__, pb.pb_len);
		uiomove(pb.pb_ptr, pb.pb_len, uio);
	}

	return err;
}

static int
pad_query_encoding(void *opaque, struct audio_encoding *ae)
{
	pad_softc_t *sc;

	sc = (pad_softc_t *)opaque;

	KASSERT(mutex_owned(&sc->sc_lock));

	return auconv_query_encoding(sc->sc_encodings, ae);
}

static int
pad_set_params2(void *opaque, int setmode, int usemode,
    const audio_params_t *play, const audio_params_t *rec,
	audio_filter_reg_t *pfil, audio_filter_reg_t *rfil)
{
	pad_softc_t *sc __diagused;

	sc = (pad_softc_t *)opaque;

	KASSERT(mutex_owned(&sc->sc_lock));

	if ((setmode & AUMODE_PLAY) != 0) {
		if (auconv_set_converter2(pad_formats, PAD_NFORMATS,
		    AUMODE_PLAY, play) < 0)
			return EINVAL;
	}
	if ((setmode & AUMODE_RECORD) != 0) {
		if (auconv_set_converter2(pad_formats, PAD_NFORMATS,
		    AUMODE_RECORD, rec) < 0)
			return EINVAL;
	}

	// 元々再生側しかなかった
	pfil->codec = pad_swvol_codec;
	pfil->context = sc;

	return 0;
}

static int
pad_start_output(void *opaque, void *block, int blksize,
    void (*intr)(void *), void *intrarg)
{
	pad_softc_t *sc;
	int err;
	int ms;

	sc = (pad_softc_t *)opaque;

	KASSERT(mutex_owned(&sc->sc_intr_lock));

	sc->sc_intr = intr;
	sc->sc_intrarg = intrarg;
	sc->sc_blksize = blksize;

	DPRINTF("%s: blksize=%d\n", __func__, blksize);
	mutex_enter(&sc->sc_cond_lock);
	err = pad_add_block(sc, block, blksize);
	mutex_exit(&sc->sc_cond_lock);
	cv_broadcast(&sc->sc_condvar);

	ms = blksize * 1000 / PADCHAN / (PADPREC / NBBY) / PADFREQ;
	DPRINTF("%s: callout ms=%d\n", __func__, ms);
	callout_reset(&sc->sc_pcallout, mstohz(ms), pad_done_output, sc);

	return err;
}

static int
pad_start_input(void *opaque, void *block, int blksize,
    void (*intr)(void *), void *intrarg)
{
	pad_softc_t *sc __diagused;

	sc = (pad_softc_t *)opaque;

	KASSERT(mutex_owned(&sc->sc_intr_lock));

	return EOPNOTSUPP;
}

static int
pad_halt_output(void *opaque)
{
	pad_softc_t *sc;

	sc = (pad_softc_t *)opaque;

	DPRINTF("%s\n", __func__);
	KASSERT(mutex_owned(&sc->sc_intr_lock));

	cv_broadcast(&sc->sc_condvar);
	callout_stop(&sc->sc_pcallout);
	sc->sc_intr = NULL;
	sc->sc_intrarg = NULL;
	sc->sc_buflen = 0;
	sc->sc_rpos = sc->sc_wpos = 0;

	return 0;
}

static int
pad_halt_input(void *opaque)
{
	pad_softc_t *sc __diagused;

	sc = (pad_softc_t *)opaque;

	KASSERT(mutex_owned(&sc->sc_intr_lock));

	return 0;
}

static void
pad_done_output(void *arg)
{
	pad_softc_t *sc;

	DPRINTF("%s\n", __func__);
	sc = (pad_softc_t *)arg;
	callout_stop(&sc->sc_pcallout);

	mutex_enter(&sc->sc_intr_lock);
	(*sc->sc_intr)(sc->sc_intrarg);
	mutex_exit(&sc->sc_intr_lock);
}

static int
pad_getdev(void *opaque, struct audio_device *ret)
{
	strlcpy(ret->name, "Virtual Audio", sizeof(ret->name));
	strlcpy(ret->version, osrelease, sizeof(ret->version));
	strlcpy(ret->config, "pad", sizeof(ret->config));

	return 0;
}

static int
pad_set_port(void *opaque, mixer_ctrl_t *mc)
{
	pad_softc_t *sc;

	sc = (pad_softc_t *)opaque;

	KASSERT(mutex_owned(&sc->sc_lock));

	switch (mc->dev) {
	case PAD_OUTPUT_MASTER_VOLUME:
	case PAD_INPUT_DAC_VOLUME:
		sc->sc_swvol = mc->un.value.level[AUDIO_MIXER_LEVEL_MONO];
		return 0;
	}

	return ENXIO;
}

static int
pad_get_port(void *opaque, mixer_ctrl_t *mc)
{
	pad_softc_t *sc;

	sc = (pad_softc_t *)opaque;

	KASSERT(mutex_owned(&sc->sc_lock));

	switch (mc->dev) {
	case PAD_OUTPUT_MASTER_VOLUME:
	case PAD_INPUT_DAC_VOLUME:
		mc->un.value.level[AUDIO_MIXER_LEVEL_MONO] = sc->sc_swvol;
		return 0;
	}

	return ENXIO;
}

static int
pad_query_devinfo(void *opaque, mixer_devinfo_t *di)
{
	pad_softc_t *sc __diagused;

	sc = (pad_softc_t *)opaque;

	KASSERT(mutex_owned(&sc->sc_lock));

	switch (di->index) {
	case PAD_OUTPUT_CLASS:
		di->mixer_class = PAD_OUTPUT_CLASS;
		strcpy(di->label.name, AudioCoutputs);
		di->type = AUDIO_MIXER_CLASS;
		di->next = di->prev = AUDIO_MIXER_LAST;
		return 0;
	case PAD_INPUT_CLASS:
		di->mixer_class = PAD_INPUT_CLASS;
		strcpy(di->label.name, AudioCinputs);
		di->type = AUDIO_MIXER_CLASS;
		di->next = di->prev = AUDIO_MIXER_LAST;
		return 0;
	case PAD_OUTPUT_MASTER_VOLUME:
		di->mixer_class = PAD_OUTPUT_CLASS;
		strcpy(di->label.name, AudioNmaster);
		di->type = AUDIO_MIXER_VALUE;
		di->next = di->prev = AUDIO_MIXER_LAST;
		di->un.v.num_channels = 1;
		strcpy(di->un.v.units.name, AudioNvolume);
		return 0;
	case PAD_INPUT_DAC_VOLUME:
		di->mixer_class = PAD_INPUT_CLASS;
		strcpy(di->label.name, AudioNdac);
		di->type = AUDIO_MIXER_VALUE;
		di->next = di->prev = AUDIO_MIXER_LAST;
		di->un.v.num_channels = 1;
		strcpy(di->un.v.units.name, AudioNvolume);
		return 0;
	}

	return ENXIO;
}

static int
pad_get_props(void *opaque)
{
	pad_softc_t *sc __diagused;

	sc = (pad_softc_t *)opaque;

	KASSERT(mutex_owned(&sc->sc_lock));

#if defined(PAD_SUPPORT_RECORD)
	return 0;
#else
	return AUDIO_PROP_PLAYBACK;
#endif
}

static void
pad_get_locks(void *opaque, kmutex_t **intr, kmutex_t **thread)
{
	pad_softc_t *sc;

	sc = (pad_softc_t *)opaque;

	*intr = &sc->sc_intr_lock;
	*thread = &sc->sc_lock;
}

static void
pad_swvol_codec(audio_filter_arg_t *arg)
{
	struct pad_softc *sc = arg->context;
	const aint_t *src;
	aint_t *dst;
	int i;

	src = arg->src;
	dst = arg->dst;
	for (i = 0; i < arg->count; i++) {
		aint2_t v = (aint2_t)(*src++);
		v = v * sc->sc_swvol / 255;
		*dst++ = (aint_t)v;
	}
}

#ifdef _MODULE

MODULE(MODULE_CLASS_DRIVER, pad, "audio");

static const struct cfiattrdata audiobuscf_iattrdata = {
	"audiobus", 0, { { NULL, NULL, 0 }, }
};
static const struct cfiattrdata * const pad_attrs[] = {
	&audiobuscf_iattrdata, NULL
};

CFDRIVER_DECL(pad, DV_DULL, pad_attrs);
extern struct cfattach pad_ca;
static int padloc[] = { -1, -1 };

static struct cfdata pad_cfdata[] = {
	{
		.cf_name = "pad",
		.cf_atname = "pad",
		.cf_unit = 0,
		.cf_fstate = FSTATE_STAR,
		.cf_loc = padloc,
		.cf_flags = 0,
		.cf_pspec = NULL,
	},
	{ NULL, NULL, 0, 0, NULL, 0, NULL }
};

static int
pad_modcmd(modcmd_t cmd, void *arg)
{
	devmajor_t cmajor = NODEVMAJOR, bmajor = NODEVMAJOR;
	int error;

	switch (cmd) {
	case MODULE_CMD_INIT:
		error = config_cfdriver_attach(&pad_cd);
		if (error) {
			return error;
		}

		error = config_cfattach_attach(pad_cd.cd_name, &pad_ca);
		if (error) {
			config_cfdriver_detach(&pad_cd);
			aprint_error("%s: unable to register cfattach\n",
				pad_cd.cd_name);

			return error;
		}

		error = config_cfdata_attach(pad_cfdata, 1);
		if (error) {
			config_cfattach_detach(pad_cd.cd_name, &pad_ca);
			config_cfdriver_detach(&pad_cd);
			aprint_error("%s: unable to register cfdata\n",
				pad_cd.cd_name);

			return error;
		}

		error = devsw_attach(pad_cd.cd_name, NULL, &bmajor,
		    &pad_cdevsw, &cmajor);
		if (error) {
			error = config_cfdata_detach(pad_cfdata);
			if (error) {
				return error;
			}
			config_cfattach_detach(pad_cd.cd_name, &pad_ca);
			config_cfdriver_detach(&pad_cd);
			aprint_error("%s: unable to register devsw\n",
				pad_cd.cd_name);

			return error;
		}

		(void)config_attach_pseudo(pad_cfdata);

		return 0;
	case MODULE_CMD_FINI:
		error = config_cfdata_detach(pad_cfdata);
		if (error) {
			return error;
		}

		config_cfattach_detach(pad_cd.cd_name, &pad_ca);
		config_cfdriver_detach(&pad_cd);
		devsw_detach(NULL, &pad_cdevsw);

		return 0;
	default:
		return ENOTTY;
	}
}

#endif