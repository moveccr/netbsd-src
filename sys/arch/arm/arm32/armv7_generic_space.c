/*	$NetBSD: armv7_generic_space.c,v 1.10 2018/11/19 10:45:47 jmcneill Exp $	*/

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
__KERNEL_RCSID(0, "$NetBSD: armv7_generic_space.c,v 1.10 2018/11/19 10:45:47 jmcneill Exp $");

#include <sys/param.h>
#include <sys/systm.h>

#include <uvm/uvm_extern.h>

#include <sys/bus.h>

/* Prototypes for all the bus_space structure functions */
bs_protos(armv7_generic);
bs_protos(armv7_generic_a4x);
bs_protos(a4x);
bs_protos(bs_notimpl);
bs_protos(generic);
bs_protos(generic_armv4);

#if __ARMEB__
#define NSWAP(n)	n ## _swap
#else
#define NSWAP(n)	n
#endif

__strong_alias(arm_generic_bs_tag,armv7_generic_bs_tag);
__strong_alias(arm_generic_a4x_bs_tag,armv7_generic_a4x_bs_tag);

struct bus_space armv7_generic_bs_tag = {
	/* cookie */
	.bs_cookie = (void *) 0,

	/* mapping/unmapping */
	.bs_map = armv7_generic_bs_map,
	.bs_unmap = armv7_generic_bs_unmap,
	.bs_subregion = armv7_generic_bs_subregion,

	/* allocation/deallocation */
	.bs_alloc = armv7_generic_bs_alloc,	/* not implemented */
	.bs_free = armv7_generic_bs_free,	/* not implemented */

	/* get kernel virtual address */
	.bs_vaddr = armv7_generic_bs_vaddr,

	/* mmap */
	.bs_mmap = armv7_generic_bs_mmap,

	/* barrier */
	.bs_barrier = armv7_generic_bs_barrier,

	/* read (single) */
	.bs_r_1 = generic_bs_r_1,
	.bs_r_2 = NSWAP(generic_armv4_bs_r_2),
	.bs_r_4 = NSWAP(generic_bs_r_4),
	.bs_r_8 = bs_notimpl_bs_r_8,

	/* read multiple */
	.bs_rm_1 = generic_bs_rm_1,
	.bs_rm_2 = NSWAP(generic_armv4_bs_rm_2),
	.bs_rm_4 = NSWAP(generic_bs_rm_4),
	.bs_rm_8 = bs_notimpl_bs_rm_8,

	/* read region */
	.bs_rr_1 = generic_bs_rr_1,
	.bs_rr_2 = NSWAP(generic_armv4_bs_rr_2),
	.bs_rr_4 = NSWAP(generic_bs_rr_4),
	.bs_rr_8 = bs_notimpl_bs_rr_8,

	/* write (single) */
	.bs_w_1 = generic_bs_w_1,
	.bs_w_2 = NSWAP(generic_armv4_bs_w_2),
	.bs_w_4 = NSWAP(generic_bs_w_4),
	.bs_w_8 = bs_notimpl_bs_w_8,

	/* write multiple */
	.bs_wm_1 = generic_bs_wm_1,
	.bs_wm_2 = NSWAP(generic_armv4_bs_wm_2),
	.bs_wm_4 = NSWAP(generic_bs_wm_4),
	.bs_wm_8 = bs_notimpl_bs_wm_8,

	/* write region */
	.bs_wr_1 = generic_bs_wr_1,
	.bs_wr_2 = NSWAP(generic_armv4_bs_wr_2),
	.bs_wr_4 = NSWAP(generic_bs_wr_4),
	.bs_wr_8 = bs_notimpl_bs_wr_8,

	/* set multiple */
	.bs_sm_1 = bs_notimpl_bs_sm_1,
	.bs_sm_2 = bs_notimpl_bs_sm_2,
	.bs_sm_4 = bs_notimpl_bs_sm_4,
	.bs_sm_8 = bs_notimpl_bs_sm_8,

	/* set region */
	.bs_sr_1 = generic_bs_sr_1,
	.bs_sr_2 = NSWAP(generic_armv4_bs_sr_2),
	.bs_sr_4 = NSWAP(generic_bs_sr_4),
	.bs_sr_8 = bs_notimpl_bs_sr_8,

	/* copy */
	.bs_c_1 = bs_notimpl_bs_c_1,
	.bs_c_2 = generic_armv4_bs_c_2,
	.bs_c_4 = bs_notimpl_bs_c_4,
	.bs_c_8 = bs_notimpl_bs_c_8,

#ifdef __BUS_SPACE_HAS_STREAM_METHODS
	/* read (single) */
	.bs_r_1_s = generic_bs_r_1,
	.bs_r_2_s = NSWAP(generic_armv4_bs_r_2),
	.bs_r_4_s = NSWAP(generic_bs_r_4),
	.bs_r_8_s = bs_notimpl_bs_r_8,

	/* read multiple */
	.bs_rm_1_s = generic_bs_rm_1,
	.bs_rm_2_s = NSWAP(generic_armv4_bs_rm_2),
	.bs_rm_4_s = NSWAP(generic_bs_rm_4),
	.bs_rm_8_s = bs_notimpl_bs_rm_8,

	/* read region */
	.bs_rr_1_s = generic_bs_rr_1,
	.bs_rr_2_s = NSWAP(generic_armv4_bs_rr_2),
	.bs_rr_4_s = NSWAP(generic_bs_rr_4),
	.bs_rr_8_s = bs_notimpl_bs_rr_8,

	/* write (single) */
	.bs_w_1_s = generic_bs_w_1,
	.bs_w_2_s = NSWAP(generic_armv4_bs_w_2),
	.bs_w_4_s = NSWAP(generic_bs_w_4),
	.bs_w_8_s = bs_notimpl_bs_w_8,

	/* write multiple */
	.bs_wm_1_s = generic_bs_wm_1,
	.bs_wm_2_s = NSWAP(generic_armv4_bs_wm_2),
	.bs_wm_4_s = NSWAP(generic_bs_wm_4),
	.bs_wm_8_s = bs_notimpl_bs_wm_8,

	/* write region */
	.bs_wr_1_s = generic_bs_wr_1,
	.bs_wr_2_s = NSWAP(generic_armv4_bs_wr_2),
	.bs_wr_4_s = NSWAP(generic_bs_wr_4),
	.bs_wr_8_s = bs_notimpl_bs_wr_8,
#endif
};

struct bus_space armv7_generic_a4x_bs_tag = {
	/* cookie */
	.bs_cookie = (void *) 0,

	/* mapping/unmapping */
	.bs_map = armv7_generic_bs_map,
	.bs_unmap = armv7_generic_bs_unmap,
	.bs_subregion = armv7_generic_a4x_bs_subregion,

	/* allocation/deallocation */
	.bs_alloc = armv7_generic_bs_alloc,	/* not implemented */
	.bs_free = armv7_generic_bs_free,	/* not implemented */

	/* get kernel virtual address */
	.bs_vaddr = armv7_generic_bs_vaddr,

	/* mmap */
	.bs_mmap = armv7_generic_a4x_bs_mmap,

	/* barrier */
	.bs_barrier = armv7_generic_bs_barrier,

	/* read (single) */
	.bs_r_1 = a4x_bs_r_1,
	.bs_r_2 = NSWAP(a4x_bs_r_2),
	.bs_r_4 = NSWAP(a4x_bs_r_4),
	.bs_r_8 = bs_notimpl_bs_r_8,

	/* read multiple */
	.bs_rm_1 = a4x_bs_rm_1,
	.bs_rm_2 = NSWAP(a4x_bs_rm_2),
	.bs_rm_4 = NSWAP(a4x_bs_rm_4),
	.bs_rm_8 = bs_notimpl_bs_rm_8,

	/* read region */
	.bs_rr_1 = bs_notimpl_bs_rr_1,
	.bs_rr_2 = bs_notimpl_bs_rr_2,
	.bs_rr_4 = bs_notimpl_bs_rr_4,
	.bs_rr_8 = bs_notimpl_bs_rr_8,

	/* write (single) */
	.bs_w_1 = a4x_bs_w_1,
	.bs_w_2 = NSWAP(a4x_bs_w_2),
	.bs_w_4 = NSWAP(a4x_bs_w_4),
	.bs_w_8 = bs_notimpl_bs_w_8,

	/* write multiple */
	.bs_wm_1 = a4x_bs_wm_1,
	.bs_wm_2 = NSWAP(a4x_bs_wm_2),
	.bs_wm_4 = NSWAP(a4x_bs_wm_4),
	.bs_wm_8 = bs_notimpl_bs_wm_8,

	/* write region */
	.bs_wr_1 = bs_notimpl_bs_wr_1,
	.bs_wr_2 = bs_notimpl_bs_wr_2,
	.bs_wr_4 = bs_notimpl_bs_wr_4,
	.bs_wr_8 = bs_notimpl_bs_wr_8,

	/* set multiple */
	.bs_sm_1 = bs_notimpl_bs_sm_1,
	.bs_sm_2 = bs_notimpl_bs_sm_2,
	.bs_sm_4 = bs_notimpl_bs_sm_4,
	.bs_sm_8 = bs_notimpl_bs_sm_8,

	/* set region */
	.bs_sr_1 = bs_notimpl_bs_sr_1,
	.bs_sr_2 = bs_notimpl_bs_sr_2,
	.bs_sr_4 = bs_notimpl_bs_sr_4,
	.bs_sr_8 = bs_notimpl_bs_sr_8,

	/* copy */
	.bs_c_1 = bs_notimpl_bs_c_1,
	.bs_c_2 = bs_notimpl_bs_c_2,
	.bs_c_4 = bs_notimpl_bs_c_4,
	.bs_c_8 = bs_notimpl_bs_c_8,

#ifdef __BUS_SPACE_HAS_STREAM_METHODS
	/* read (single) */
	.bs_r_1_s = a4x_bs_r_1,
	.bs_r_2_s = NSWAP(a4x_bs_r_2),
	.bs_r_4_s = NSWAP(a4x_bs_r_4),
	.bs_r_8_s = bs_notimpl_bs_r_8,

	/* read multiple */
	.bs_rm_1_s = a4x_bs_rm_1,
	.bs_rm_2_s = NSWAP(a4x_bs_rm_2),
	.bs_rm_4_s = NSWAP(a4x_bs_rm_4),
	.bs_rm_8_s = bs_notimpl_bs_rm_8,

	/* read region */
	.bs_rr_1_s = bs_notimpl_bs_rr_1,
	.bs_rr_2_s = bs_notimpl_bs_rr_2,
	.bs_rr_4_s = bs_notimpl_bs_rr_4,
	.bs_rr_8_s = bs_notimpl_bs_rr_8,

	/* write (single) */
	.bs_w_1_s = a4x_bs_w_1,
	.bs_w_2_s = NSWAP(a4x_bs_w_2),
	.bs_w_4_s = NSWAP(a4x_bs_w_4),
	.bs_w_8_s = bs_notimpl_bs_w_8,

	/* write multiple */
	.bs_wm_1_s = a4x_bs_wm_1,
	.bs_wm_2_s = NSWAP(a4x_bs_wm_2),
	.bs_wm_4_s = NSWAP(a4x_bs_wm_4),
	.bs_wm_8_s = bs_notimpl_bs_wm_8,

	/* write region */
	.bs_wr_1_s = bs_notimpl_bs_wr_1,
	.bs_wr_2_s = bs_notimpl_bs_wr_2,
	.bs_wr_4_s = bs_notimpl_bs_wr_4,
	.bs_wr_8_s = bs_notimpl_bs_wr_8,
#endif
};

int
armv7_generic_bs_map(void *t, bus_addr_t bpa, bus_size_t size, int flag,
    bus_space_handle_t *bshp)
{
	u_long startpa, endpa, pa;
	const struct pmap_devmap *pd;
	int pmapflags;
	vaddr_t va;

	if ((pd = pmap_devmap_find_pa(bpa, size)) != NULL) {
		/* Device was statically mapped. */
		*bshp = pd->pd_va + (bpa - pd->pd_pa);
		return 0;
	}

	startpa = trunc_page(bpa);
	endpa = round_page(bpa + size);

	/* XXX use extent manager to check duplicate mapping */

	va = uvm_km_alloc(kernel_map, endpa - startpa, 0,
	    UVM_KMF_VAONLY | UVM_KMF_NOWAIT | UVM_KMF_COLORMATCH);
	if (!va)
		return ENOMEM;

	*bshp = (bus_space_handle_t)(va + (bpa - startpa));

	if (flag & BUS_SPACE_MAP_PREFETCHABLE)
		pmapflags = PMAP_WRITE_COMBINE;
	else if (flag & BUS_SPACE_MAP_CACHEABLE)
		pmapflags = 0;
	else
		pmapflags = PMAP_NOCACHE;

	for (pa = startpa; pa < endpa; pa += PAGE_SIZE, va += PAGE_SIZE) {
		pmap_kenter_pa(va, pa, VM_PROT_READ | VM_PROT_WRITE, pmapflags);
	}
	pmap_update(pmap_kernel());

	return 0;
}

void
armv7_generic_bs_unmap(void *t, bus_space_handle_t bsh, bus_size_t size)
{
	vaddr_t	va;
	vsize_t	sz;

	if (pmap_devmap_find_va(bsh, size) != NULL) {
		/* Device was statically mapped; nothing to do. */
		return;
	}

	va = trunc_page(bsh);
	sz = round_page(bsh + size) - va;

	pmap_kremove(va, sz);
	pmap_update(pmap_kernel());
	uvm_km_free(kernel_map, va, sz, UVM_KMF_VAONLY);
}


int
armv7_generic_bs_subregion(void *t, bus_space_handle_t bsh, bus_size_t offset,
    bus_size_t size, bus_space_handle_t *nbshp)
{

	*nbshp = bsh + offset;
	return 0;
}

int
armv7_generic_a4x_bs_subregion(void *t, bus_space_handle_t bsh, bus_size_t offset,
    bus_size_t size, bus_space_handle_t *nbshp)
{

	*nbshp = bsh + 4 * offset;
	return 0;
}

void
armv7_generic_bs_barrier(void *t, bus_space_handle_t bsh, bus_size_t offset,
    bus_size_t len, int flags)
{
	flags &= BUS_SPACE_BARRIER_READ|BUS_SPACE_BARRIER_WRITE;

	if (flags)
		arm_dsb();
}

void *
armv7_generic_bs_vaddr(void *t, bus_space_handle_t bsh)
{

	return (void *)bsh;
}

paddr_t
armv7_generic_bs_mmap(void *t, bus_addr_t bpa, off_t offset, int prot, int flags)
{
	paddr_t bus_flags = 0;

	if (flags & BUS_SPACE_MAP_PREFETCHABLE)
		bus_flags |= ARM32_MMAP_WRITECOMBINE;

	return arm_btop(bpa + offset) | bus_flags;
}

paddr_t
armv7_generic_a4x_bs_mmap(void *t, bus_addr_t bpa, off_t offset, int prot, int flags)
{
	paddr_t bus_flags = 0;

	if (flags & BUS_SPACE_MAP_PREFETCHABLE)
		bus_flags |= ARM32_MMAP_WRITECOMBINE;

	return arm_btop(bpa + 4 * offset) | bus_flags;
}

int
armv7_generic_bs_alloc(void *t, bus_addr_t rstart, bus_addr_t rend,
    bus_size_t size, bus_size_t alignment, bus_size_t boundary, int flags,
    bus_addr_t *bpap, bus_space_handle_t *bshp)
{

	panic("%s(): not implemented\n", __func__);
}

void
armv7_generic_bs_free(void *t, bus_space_handle_t bsh, bus_size_t size)
{

	panic("%s(): not implemented\n", __func__);
}
