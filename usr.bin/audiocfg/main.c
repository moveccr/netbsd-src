/* $NetBSD: main.c,v 1.7 2013/08/12 14:03:18 joerg Exp $ */

/*
 * Copyright (c) 2010 Jared D. McNeill <jmcneill@invisible.ca>
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

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "audiodev.h"
#include "drvctl.h"

__dead static void
usage(const char *p)
{
	fprintf(stderr, "usage: %s list\n", p);
	fprintf(stderr, "       %s default <index>\n", p);
	fprintf(stderr, "       %s test <index>\n", p);
	exit(EXIT_FAILURE);
}

static const char *encoding_names[] = {
	"none",
	AudioEmulaw,
	AudioEalaw,
	"pcm16",
	"pcm8",
	AudioEadpcm,
	AudioEslinear_le,
	AudioEslinear_be,
	AudioEulinear_le,
	AudioEulinear_be,
	AudioEslinear,
	AudioEulinear,
	AudioEmpeg_l1_stream,
	AudioEmpeg_l1_packets,
	AudioEmpeg_l1_system,
	AudioEmpeg_l2_stream,
	AudioEmpeg_l2_packets,
	AudioEmpeg_l2_system,
	AudioEac3,
};

static void
print_audiodev(struct audiodev *adev, int i)
{
	struct audiofmt *f;
	int j;

	assert(adev != NULL);

	printf("%u: [%c] %s @ %s: ",
	    i, adev->defaultdev ? '*' : ' ',
	    adev->xname, adev->pxname);
	printf("%s", adev->audio_device.name);
	if (strlen(adev->audio_device.version) > 0)
		printf(" %s", adev->audio_device.version);
#if 1
	printf("\n");
#else
	printf(", %u playback channel%s\n",
	    adev->pchan, adev->pchan == 1 ? "" : "s");
#endif

	TAILQ_FOREACH(f, &adev->formats, next) {
		printf("       ");
#if BYTE_ORDER == LITTLE_ENDIAN
		if (f->fmt.encoding != AUDIO_ENCODING_SLINEAR_LE)
#else
		if (f->fmt.encoding != AUDIO_ENCODING_SLINEAR_BE)
#endif
			printf("(  ) ");
		else if ((f->fmt.mode & (AUMODE_PLAY | AUMODE_RECORD))
		    == (AUMODE_PLAY | AUMODE_RECORD))
			printf("(PR) ");
		else if ((f->fmt.mode & AUMODE_PLAY))
			printf("(P-) ");
		else if ((f->fmt.mode & AUMODE_RECORD))
			printf("(-R) ");

		if (f->fmt.encoding < __arraycount(encoding_names))
			printf("%s", encoding_names[f->fmt.encoding]);
		else
			printf("unknown_encoding_%d", f->fmt.encoding);
		printf(" %d/%d, %dch, ",
		    f->fmt.validbits,
		    f->fmt.precision,
		    f->fmt.channels);
		if (f->fmt.frequency_type == 0) {
			printf("%d-%dHz",
			    f->fmt.frequency[0],
			    f->fmt.frequency[1]);
		} else {
			for (j = 0; j < f->fmt.frequency_type; j++) {
				printf("%c%d",
				    (j == 0) ? '{' : ',',
				    f->fmt.frequency[j]);
			}
			printf("}");
		}
		printf("\n");
	}
}

int
main(int argc, char *argv[])
{
	struct audiodev *adev;
	unsigned int n, i;

	if (audiodev_refresh() == -1)
		return EXIT_FAILURE;

	if (argc < 2)
		usage(argv[0]);
		/* NOTREACHED */

	if (strcmp(argv[1], "list") == 0) {
		n = audiodev_count();
		for (i = 0; i < n; i++)
			print_audiodev(audiodev_get(i), i);
	} else if (strcmp(argv[1], "default") == 0 && argc == 3) {
		if (*argv[2] < '0' || *argv[2] > '9')
			usage(argv[0]);
			/* NOTREACHED */
		errno = 0;
		i = strtoul(argv[2], NULL, 10);
		if (errno)
			usage(argv[0]);
			/* NOTREACHED */
		adev = audiodev_get(i);
		if (adev == NULL) {
			fprintf(stderr, "no such device\n");
			return EXIT_FAILURE;
		}
		printf("setting default audio device to %s\n", adev->xname);
		if (audiodev_set_default(adev) == -1) {
			perror("couldn't set default device");
			return EXIT_FAILURE;
		}
	} else if (strcmp(argv[1], "test") == 0 && argc == 3) {
		if (*argv[2] < '0' || *argv[2] > '9')
			usage(argv[0]);
			/* NOTREACHED */
		errno = 0;
		i = strtoul(argv[2], NULL, 10);
		if (errno)
			usage(argv[0]);
			/* NOTREACHED */
		adev = audiodev_get(i);
		if (adev == NULL) {
			fprintf(stderr, "no such device\n");
			return EXIT_FAILURE;
		}
		print_audiodev(adev, i);
		for (i = 0; i < adev->pchan; i++) {
			printf("  testing channel %d...", i);
			fflush(stdout);
			if (audiodev_test(adev, 1 << i) == -1)
				return EXIT_FAILURE;
			printf(" done\n");
		}
	} else
		usage(argv[0]);
		/* NOTREACHED */

	return EXIT_SUCCESS;
}
