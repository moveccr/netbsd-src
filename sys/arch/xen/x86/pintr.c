/*	NetBSD: intr.c,v 1.15 2004/04/10 14:49:55 kochi Exp 	*/

/*
 * Copyright 2002 (c) Wasabi Systems, Inc.
 * All rights reserved.
 *
 * Written by Frank van der Linden for Wasabi Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed for the NetBSD Project by
 *      Wasabi Systems, Inc.
 * 4. The name of Wasabi Systems, Inc. may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY WASABI SYSTEMS, INC. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL WASABI SYSTEMS, INC
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*-
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *     @(#)isa.c       7.2 (Berkeley) 5/13/91
 */
/*-
 * Copyright (c) 1993, 1994 Charles Hannum.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *     This product includes software developed by the University of
 *     California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *     @(#)isa.c       7.2 (Berkeley) 5/13/91
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: pintr.c,v 1.9 2018/10/10 02:34:08 cherry Exp $");

#include "opt_multiprocessor.h"
#include "opt_xen.h"
#include "isa.h"
#include "pci.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/syslog.h>
#include <sys/device.h>
#include <sys/malloc.h>
#include <sys/proc.h>
#include <sys/errno.h>
#include <sys/cpu.h>

#include <uvm/uvm_extern.h>

#include <machine/i8259.h>
#include <machine/pio.h>
#include <xen/evtchn.h>
#include <xen/intr.h>

#include "acpica.h"
#include "ioapic.h"
#include "opt_mpbios.h"

#if NIOAPIC > 0
/* XXX: todo - compat with lapic.c and XEN for x2apic */
bool x2apic_mode __read_mostly = false;
/* for x86/i8259.c */
struct intrstub legacy_stubs[NUM_LEGACY_IRQS] = {{0,0}};
/* for x86/ioapic.c */
struct intrstub ioapic_edge_stubs[MAX_INTR_SOURCES] = {{0,0}};
struct intrstub ioapic_level_stubs[MAX_INTR_SOURCES] = {{0,0}};
struct intrstub x2apic_edge_stubs[MAX_INTR_SOURCES] = {{0,0}};
struct intrstub x2apic_level_stubs[MAX_INTR_SOURCES] = {{0,0}};
#include <machine/i82093var.h>
int irq2port[NR_EVENT_CHANNELS] = {0}; /* actually port + 1, so that 0 is invaid */
static int irq2vect[256] = {0};
static int vect2irq[256] = {0};
#endif /* NIOAPIC */
#if NACPICA > 0
#include <machine/mpconfig.h>
#include <machine/mpacpi.h>
#endif
#ifdef MPBIOS
#include <machine/mpbiosvar.h>
#endif

#if NPCI > 0
#include <dev/pci/ppbreg.h>
#endif

#if defined(DOM0OPS) || NPCI > 0
int
xen_vec_alloc(int gsi)
{
	physdev_op_t op;

	KASSERT(gsi < 255);

	if (irq2port[gsi] == 0) {
		op.cmd = PHYSDEVOP_ASSIGN_VECTOR;
		op.u.irq_op.irq = gsi;
		if (HYPERVISOR_physdev_op(&op) < 0) {
			panic("PHYSDEVOP_ASSIGN_VECTOR gsi %d", gsi);
		}
		KASSERT(irq2vect[gsi] == 0 ||
			irq2vect[gsi] == op.u.irq_op.vector);
		irq2vect[gsi] = op.u.irq_op.vector;
		KASSERT(vect2irq[op.u.irq_op.vector] == 0 ||
			 vect2irq[op.u.irq_op.vector] == gsi);
		vect2irq[op.u.irq_op.vector] = gsi;
	}

	return (irq2vect[gsi]);
}

/*
 * This function doesn't "allocate" anything. It merely translates our
 * understanding of PIC to the XEN 'gsi' namespace. In the case of
 * MSIs, pirq == irq. In the case of everything else, the hypervisor
 * doesn't really care, so we just use the native conventions that
 * have been setup during boot by mpbios.c/mpacpi.c
 */
int
xen_pic_to_gsi(struct pic *pic, int pin)
{
	int gsi;

	KASSERT(pic != NULL);

	/*
	 * We assume that mpbios/mpacpi have done the right thing.
	 * If so, legacy_irq should identity map correctly to gsi.
	 */
	gsi = pic->pic_vecbase + pin;

	switch (pic->pic_type) {
	case PIC_I8259:
		KASSERT(gsi < 16);
		break;
	case PIC_IOAPIC:
		break;
	default: /* XXX: MSI Support */
		panic("XXX: MSI(X) Support");
		break;
	}

	KASSERT(gsi < 255);
	return gsi;
}


#endif /* defined(DOM0OPS) || NPCI > 0 */
