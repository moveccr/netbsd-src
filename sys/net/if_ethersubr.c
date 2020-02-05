/*	$NetBSD: if_ethersubr.c,v 1.282 2020/01/29 04:11:35 thorpej Exp $	*/

/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
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
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Copyright (c) 1982, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	@(#)if_ethersubr.c	8.2 (Berkeley) 4/4/96
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: if_ethersubr.c,v 1.282 2020/01/29 04:11:35 thorpej Exp $");

#ifdef _KERNEL_OPT
#include "opt_inet.h"
#include "opt_atalk.h"
#include "opt_mbuftrace.h"
#include "opt_mpls.h"
#include "opt_gateway.h"
#include "opt_pppoe.h"
#include "opt_net_mpsafe.h"
#endif

#include "vlan.h"
#include "pppoe.h"
#include "bridge.h"
#include "arp.h"
#include "agr.h"

#include <sys/sysctl.h>
#include <sys/mbuf.h>
#include <sys/mutex.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/device.h>
#include <sys/rnd.h>
#include <sys/rndsource.h>
#include <sys/cpu.h>
#include <sys/kmem.h>

#include <net/if.h>
#include <net/netisr.h>
#include <net/route.h>
#include <net/if_llc.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <net/pktqueue.h>

#include <net/if_media.h>
#include <dev/mii/mii.h>
#include <dev/mii/miivar.h>

#if NARP == 0
/*
 * XXX there should really be a way to issue this warning from within config(8)
 */
#error You have included NETATALK or a pseudo-device in your configuration that depends on the presence of ethernet interfaces, but have no such interfaces configured. Check if you really need pseudo-device bridge, pppoe, vlan or options NETATALK.
#endif

#include <net/bpf.h>

#include <net/if_ether.h>
#include <net/if_vlanvar.h>

#if NPPPOE > 0
#include <net/if_pppoe.h>
#endif

#if NAGR > 0
#include <net/agr/ieee8023_slowprotocols.h>	/* XXX */
#include <net/agr/ieee8023ad.h>
#include <net/agr/if_agrvar.h>
#endif

#if NBRIDGE > 0
#include <net/if_bridgevar.h>
#endif

#include <netinet/in.h>
#ifdef INET
#include <netinet/in_var.h>
#endif
#include <netinet/if_inarp.h>

#ifdef INET6
#ifndef INET
#include <netinet/in.h>
#endif
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>
#endif

#include "carp.h"
#if NCARP > 0
#include <netinet/ip_carp.h>
#endif

#ifdef NETATALK
#include <netatalk/at.h>
#include <netatalk/at_var.h>
#include <netatalk/at_extern.h>

#define llc_snap_org_code llc_un.type_snap.org_code
#define llc_snap_ether_type llc_un.type_snap.ether_type

extern u_char	at_org_code[3];
extern u_char	aarp_org_code[3];
#endif /* NETATALK */

#ifdef MPLS
#include <netmpls/mpls.h>
#include <netmpls/mpls_var.h>
#endif

#ifdef DIAGNOSTIC
static struct timeval bigpktppslim_last;
static int bigpktppslim = 2;	/* XXX */
static int bigpktpps_count;
static kmutex_t bigpktpps_lock __cacheline_aligned;
#endif

const uint8_t etherbroadcastaddr[ETHER_ADDR_LEN] =
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
const uint8_t ethermulticastaddr_slowprotocols[ETHER_ADDR_LEN] =
    { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x02 };
#define senderr(e) { error = (e); goto bad;}

static int ether_output(struct ifnet *, struct mbuf *,
    const struct sockaddr *, const struct rtentry *);

/*
 * Ethernet output routine.
 * Encapsulate a packet of type family for the local net.
 * Assumes that ifp is actually pointer to ethercom structure.
 */
static int
ether_output(struct ifnet * const ifp0, struct mbuf * const m0,
    const struct sockaddr * const dst, const struct rtentry *rt)
{
	uint8_t esrc[ETHER_ADDR_LEN], edst[ETHER_ADDR_LEN];
	uint16_t etype = 0;
	int error = 0, hdrcmplt = 0;
	struct mbuf *m = m0;
	struct mbuf *mcopy = NULL;
	struct ether_header *eh;
	struct ifnet *ifp = ifp0;
#ifdef INET
	struct arphdr *ah;
#endif
#ifdef NETATALK
	struct at_ifaddr *aa;
#endif

#ifdef MBUFTRACE
	m_claimm(m, ifp->if_mowner);
#endif

#if NCARP > 0
	if (ifp->if_type == IFT_CARP) {
		struct ifaddr *ifa;
		int s = pserialize_read_enter();

		/* loop back if this is going to the carp interface */
		if (dst != NULL && ifp0->if_link_state == LINK_STATE_UP &&
		    (ifa = ifa_ifwithaddr(dst)) != NULL) {
			if (ifa->ifa_ifp == ifp0) {
				pserialize_read_exit(s);
				return looutput(ifp0, m, dst, rt);
			}
		}
		pserialize_read_exit(s);

		ifp = ifp->if_carpdev;
		/* ac = (struct arpcom *)ifp; */

		if ((ifp0->if_flags & (IFF_UP | IFF_RUNNING)) !=
		    (IFF_UP | IFF_RUNNING))
			senderr(ENETDOWN);
	}
#endif

	if ((ifp->if_flags & (IFF_UP | IFF_RUNNING)) != (IFF_UP | IFF_RUNNING))
		senderr(ENETDOWN);

	switch (dst->sa_family) {

#ifdef INET
	case AF_INET:
		if (m->m_flags & M_BCAST) {
			memcpy(edst, etherbroadcastaddr, sizeof(edst));
		} else if (m->m_flags & M_MCAST) {
			ETHER_MAP_IP_MULTICAST(&satocsin(dst)->sin_addr, edst);
		} else {
			error = arpresolve(ifp0, rt, m, dst, edst, sizeof(edst));
			if (error)
				return (error == EWOULDBLOCK) ? 0 : error;
		}
		/* If broadcasting on a simplex interface, loopback a copy */
		if ((m->m_flags & M_BCAST) && (ifp->if_flags & IFF_SIMPLEX))
			mcopy = m_copypacket(m, M_DONTWAIT);
		etype = htons(ETHERTYPE_IP);
		break;

	case AF_ARP:
		ah = mtod(m, struct arphdr *);
		if (m->m_flags & M_BCAST) {
			memcpy(edst, etherbroadcastaddr, sizeof(edst));
		} else {
			void *tha = ar_tha(ah);

			if (tha == NULL) {
				/* fake with ARPHRD_IEEE1394 */
				m_freem(m);
				return 0;
			}
			memcpy(edst, tha, sizeof(edst));
		}

		ah->ar_hrd = htons(ARPHRD_ETHER);

		switch (ntohs(ah->ar_op)) {
		case ARPOP_REVREQUEST:
		case ARPOP_REVREPLY:
			etype = htons(ETHERTYPE_REVARP);
			break;

		case ARPOP_REQUEST:
		case ARPOP_REPLY:
		default:
			etype = htons(ETHERTYPE_ARP);
		}
		break;
#endif

#ifdef INET6
	case AF_INET6:
		if (m->m_flags & M_BCAST) {
			memcpy(edst, etherbroadcastaddr, sizeof(edst));
		} else if (m->m_flags & M_MCAST) {
			ETHER_MAP_IPV6_MULTICAST(&satocsin6(dst)->sin6_addr,
			    edst);
		} else {
			error = nd6_resolve(ifp0, rt, m, dst, edst,
			    sizeof(edst));
			if (error)
				return (error == EWOULDBLOCK) ? 0 : error;
		}
		etype = htons(ETHERTYPE_IPV6);
		break;
#endif

#ifdef NETATALK
	case AF_APPLETALK: {
		struct ifaddr *ifa;
		int s;

		KERNEL_LOCK(1, NULL);

		if (!aarpresolve(ifp, m, (const struct sockaddr_at *)dst, edst)) {
			KERNEL_UNLOCK_ONE(NULL);
			return 0;
		}

		/*
		 * ifaddr is the first thing in at_ifaddr
		 */
		s = pserialize_read_enter();
		ifa = at_ifawithnet((const struct sockaddr_at *)dst, ifp);
		if (ifa == NULL) {
			pserialize_read_exit(s);
			KERNEL_UNLOCK_ONE(NULL);
			senderr(EADDRNOTAVAIL);
		}
		aa = (struct at_ifaddr *)ifa;

		/*
		 * In the phase 2 case, we need to prepend an mbuf for the
		 * llc header.
		 */
		if (aa->aa_flags & AFA_PHASE2) {
			struct llc llc;

			M_PREPEND(m, sizeof(struct llc), M_DONTWAIT);
			if (m == NULL) {
				pserialize_read_exit(s);
				KERNEL_UNLOCK_ONE(NULL);
				senderr(ENOBUFS);
			}

			llc.llc_dsap = llc.llc_ssap = LLC_SNAP_LSAP;
			llc.llc_control = LLC_UI;
			memcpy(llc.llc_snap_org_code, at_org_code,
			    sizeof(llc.llc_snap_org_code));
			llc.llc_snap_ether_type = htons(ETHERTYPE_ATALK);
			memcpy(mtod(m, void *), &llc, sizeof(struct llc));
		} else {
			etype = htons(ETHERTYPE_ATALK);
		}
		pserialize_read_exit(s);
		KERNEL_UNLOCK_ONE(NULL);
		break;
	}
#endif /* NETATALK */

	case pseudo_AF_HDRCMPLT:
		hdrcmplt = 1;
		memcpy(esrc,
		    ((const struct ether_header *)dst->sa_data)->ether_shost,
		    sizeof(esrc));
		/* FALLTHROUGH */

	case AF_UNSPEC:
		memcpy(edst,
		    ((const struct ether_header *)dst->sa_data)->ether_dhost,
		    sizeof(edst));
		/* AF_UNSPEC doesn't swap the byte order of the ether_type. */
		etype = ((const struct ether_header *)dst->sa_data)->ether_type;
		break;

	default:
		printf("%s: can't handle af%d\n", ifp->if_xname,
		    dst->sa_family);
		senderr(EAFNOSUPPORT);
	}

#ifdef MPLS
	{
		struct m_tag *mtag;
		mtag = m_tag_find(m, PACKET_TAG_MPLS);
		if (mtag != NULL) {
			/* Having the tag itself indicates it's MPLS */
			etype = htons(ETHERTYPE_MPLS);
			m_tag_delete(m, mtag);
		}
	}
#endif

	if (mcopy)
		(void)looutput(ifp, mcopy, dst, rt);

	KASSERT((m->m_flags & M_PKTHDR) != 0);

	/*
	 * If no ether type is set, this must be a 802.2 formatted packet.
	 */
	if (etype == 0)
		etype = htons(m->m_pkthdr.len);

	/*
	 * Add local net header. If no space in first mbuf, allocate another.
	 */
	M_PREPEND(m, sizeof(struct ether_header), M_DONTWAIT);
	if (m == NULL)
		senderr(ENOBUFS);

	eh = mtod(m, struct ether_header *);
	/* Note: etype is already in network byte order. */
	memcpy(&eh->ether_type, &etype, sizeof(eh->ether_type));
	memcpy(eh->ether_dhost, edst, sizeof(edst));
	if (hdrcmplt) {
		memcpy(eh->ether_shost, esrc, sizeof(eh->ether_shost));
	} else {
	 	memcpy(eh->ether_shost, CLLADDR(ifp->if_sadl),
		    sizeof(eh->ether_shost));
	}

#if NCARP > 0
	if (ifp0 != ifp && ifp0->if_type == IFT_CARP) {
	 	memcpy(eh->ether_shost, CLLADDR(ifp0->if_sadl),
		    sizeof(eh->ether_shost));
	}
#endif

	if ((error = pfil_run_hooks(ifp->if_pfil, &m, ifp, PFIL_OUT)) != 0)
		return error;
	if (m == NULL)
		return 0;

#if NBRIDGE > 0
	/*
	 * Bridges require special output handling.
	 */
	if (ifp->if_bridge)
		return bridge_output(ifp, m, NULL, NULL);
#endif

#if NCARP > 0
	if (ifp != ifp0)
		if_statadd(ifp0, if_obytes, m->m_pkthdr.len + ETHER_HDR_LEN);
#endif

#ifdef ALTQ
	KERNEL_LOCK(1, NULL);
	/*
	 * If ALTQ is enabled on the parent interface, do
	 * classification; the queueing discipline might not
	 * require classification, but might require the
	 * address family/header pointer in the pktattr.
	 */
	if (ALTQ_IS_ENABLED(&ifp->if_snd))
		altq_etherclassify(&ifp->if_snd, m);
	KERNEL_UNLOCK_ONE(NULL);
#endif
	return ifq_enqueue(ifp, m);

bad:
	if (m)
		m_freem(m);
	return error;
}

#ifdef ALTQ
/*
 * This routine is a slight hack to allow a packet to be classified
 * if the Ethernet headers are present.  It will go away when ALTQ's
 * classification engine understands link headers.
 *
 * XXX: We may need to do m_pullups here. First to ensure struct ether_header
 * is indeed contiguous, then to read the LLC and so on.
 */
void
altq_etherclassify(struct ifaltq *ifq, struct mbuf *m)
{
	struct ether_header *eh;
	struct mbuf *mtop = m;
	uint16_t ether_type;
	int hlen, af, hdrsize;
	void *hdr;

	KASSERT((mtop->m_flags & M_PKTHDR) != 0);

	hlen = ETHER_HDR_LEN;
	eh = mtod(m, struct ether_header *);

	ether_type = htons(eh->ether_type);

	if (ether_type < ETHERMTU) {
		/* LLC/SNAP */
		struct llc *llc = (struct llc *)(eh + 1);
		hlen += 8;

		if (m->m_len < hlen ||
		    llc->llc_dsap != LLC_SNAP_LSAP ||
		    llc->llc_ssap != LLC_SNAP_LSAP ||
		    llc->llc_control != LLC_UI) {
			/* Not SNAP. */
			goto bad;
		}

		ether_type = htons(llc->llc_un.type_snap.ether_type);
	}

	switch (ether_type) {
	case ETHERTYPE_IP:
		af = AF_INET;
		hdrsize = 20;		/* sizeof(struct ip) */
		break;

	case ETHERTYPE_IPV6:
		af = AF_INET6;
		hdrsize = 40;		/* sizeof(struct ip6_hdr) */
		break;

	default:
		af = AF_UNSPEC;
		hdrsize = 0;
		break;
	}

	while (m->m_len <= hlen) {
		hlen -= m->m_len;
		m = m->m_next;
		if (m == NULL)
			goto bad;
	}

	if (m->m_len < (hlen + hdrsize)) {
		/*
		 * protocol header not in a single mbuf.
		 * We can't cope with this situation right
		 * now (but it shouldn't ever happen, really, anyhow).
		 */
#ifdef DEBUG
		printf("altq_etherclassify: headers span multiple mbufs: "
		    "%d < %d\n", m->m_len, (hlen + hdrsize));
#endif
		goto bad;
	}

	m->m_data += hlen;
	m->m_len -= hlen;

	hdr = mtod(m, void *);

	if (ALTQ_NEEDS_CLASSIFY(ifq)) {
		mtop->m_pkthdr.pattr_class =
		    (*ifq->altq_classify)(ifq->altq_clfier, m, af);
	}
	mtop->m_pkthdr.pattr_af = af;
	mtop->m_pkthdr.pattr_hdr = hdr;

	m->m_data -= hlen;
	m->m_len += hlen;

	return;

bad:
	mtop->m_pkthdr.pattr_class = NULL;
	mtop->m_pkthdr.pattr_hdr = NULL;
	mtop->m_pkthdr.pattr_af = AF_UNSPEC;
}
#endif /* ALTQ */

/*
 * Process a received Ethernet packet;
 * the packet is in the mbuf chain m with
 * the ether header.
 */
void
ether_input(struct ifnet *ifp, struct mbuf *m)
{
	struct ethercom *ec = (struct ethercom *) ifp;
	pktqueue_t *pktq = NULL;
	struct ifqueue *inq = NULL;
	uint16_t etype;
	struct ether_header *eh;
	size_t ehlen;
	static int earlypkts;
	int isr = 0;
#if defined (LLC) || defined(NETATALK)
	struct llc *l;
#endif

	KASSERT(!cpu_intr_p());
	KASSERT((m->m_flags & M_PKTHDR) != 0);

	if ((ifp->if_flags & IFF_UP) == 0) {
		m_freem(m);
		return;
	}
	if (m->m_len < sizeof(*eh)) {
		m = m_pullup(m, sizeof(*eh));
		if (m == NULL)
			return;
	}

#ifdef MBUFTRACE
	m_claimm(m, &ec->ec_rx_mowner);
#endif
	eh = mtod(m, struct ether_header *);
	etype = ntohs(eh->ether_type);
	ehlen = sizeof(*eh);

	if (__predict_false(earlypkts < 100 || !rnd_initial_entropy)) {
		rnd_add_data(NULL, eh, ehlen, 0);
		earlypkts++;
	}

	/*
	 * Determine if the packet is within its size limits. For MPLS the
	 * header length is variable, so we skip the check.
	 */
	if (etype != ETHERTYPE_MPLS && m->m_pkthdr.len >
	    ETHER_MAX_FRAME(ifp, etype, m->m_flags & M_HASFCS)) {
#ifdef DIAGNOSTIC
		mutex_enter(&bigpktpps_lock);
		if (ppsratecheck(&bigpktppslim_last, &bigpktpps_count,
		    bigpktppslim)) {
			printf("%s: discarding oversize frame (len=%d)\n",
			    ifp->if_xname, m->m_pkthdr.len);
		}
		mutex_exit(&bigpktpps_lock);
#endif
		if_statinc(ifp, if_iqdrops);
		m_freem(m);
		return;
	}

	if (ETHER_IS_MULTICAST(eh->ether_dhost)) {
		/*
		 * If this is not a simplex interface, drop the packet
		 * if it came from us.
		 */
		if ((ifp->if_flags & IFF_SIMPLEX) == 0 &&
		    memcmp(CLLADDR(ifp->if_sadl), eh->ether_shost,
		    ETHER_ADDR_LEN) == 0) {
			m_freem(m);
			return;
		}

		if (memcmp(etherbroadcastaddr,
		    eh->ether_dhost, ETHER_ADDR_LEN) == 0)
			m->m_flags |= M_BCAST;
		else
			m->m_flags |= M_MCAST;
		if_statinc(ifp, if_imcasts);
	}

	/* If the CRC is still on the packet, trim it off. */
	if (m->m_flags & M_HASFCS) {
		m_adj(m, -ETHER_CRC_LEN);
		m->m_flags &= ~M_HASFCS;
	}

	if_statadd(ifp, if_ibytes, m->m_pkthdr.len);

#if NCARP > 0
	if (__predict_false(ifp->if_carp && ifp->if_type != IFT_CARP)) {
		/*
		 * Clear M_PROMISC, in case the packet comes from a
		 * vlan.
		 */
		m->m_flags &= ~M_PROMISC;
		if (carp_input(m, (uint8_t *)&eh->ether_shost,
		    (uint8_t *)&eh->ether_dhost, eh->ether_type) == 0)
			return;
	}
#endif

	if ((m->m_flags & (M_BCAST | M_MCAST | M_PROMISC)) == 0 &&
	    (ifp->if_flags & IFF_PROMISC) != 0 &&
	    memcmp(CLLADDR(ifp->if_sadl), eh->ether_dhost,
	     ETHER_ADDR_LEN) != 0) {
		m->m_flags |= M_PROMISC;
	}

	if ((m->m_flags & M_PROMISC) == 0) {
		if (pfil_run_hooks(ifp->if_pfil, &m, ifp, PFIL_IN) != 0)
			return;
		if (m == NULL)
			return;

		eh = mtod(m, struct ether_header *);
		etype = ntohs(eh->ether_type);
	}

#if NAGR > 0
	if (ifp->if_agrprivate &&
	    __predict_true(etype != ETHERTYPE_SLOWPROTOCOLS)) {
		m->m_flags &= ~M_PROMISC;
		agr_input(ifp, m);
		return;
	}
#endif

	/*
	 * If VLANs are configured on the interface, check to
	 * see if the device performed the decapsulation and
	 * provided us with the tag.
	 */
	if (ec->ec_nvlans && vlan_has_tag(m)) {
#if NVLAN > 0
		/*
		 * vlan_input() will either recursively call ether_input()
		 * or drop the packet.
		 */
		vlan_input(ifp, m);
#else
		m_freem(m);
#endif
		return;
	}

	/*
	 * Handle protocols that expect to have the Ethernet header
	 * (and possibly FCS) intact.
	 */
	switch (etype) {
	case ETHERTYPE_VLAN: {
		struct ether_vlan_header *evl = (void *)eh;

		/*
		 * If there is a tag of 0, then the VLAN header was probably
		 * just being used to store the priority.  Extract the ether
		 * type, and if IP or IPV6, let them deal with it.
		 */
		if (m->m_len >= sizeof(*evl) &&
		    EVL_VLANOFTAG(evl->evl_tag) == 0) {
			etype = ntohs(evl->evl_proto);
			ehlen = sizeof(*evl);
			if ((m->m_flags & M_PROMISC) == 0 &&
			    (etype == ETHERTYPE_IP ||
			     etype == ETHERTYPE_IPV6))
				break;
		}

#if NVLAN > 0
		/*
		 * vlan_input() will either recursively call ether_input()
		 * or drop the packet.
		 */
		if (ec->ec_nvlans != 0)
			vlan_input(ifp, m);
		else
#endif
			m_freem(m);

		return;
	}

#if NPPPOE > 0
	case ETHERTYPE_PPPOEDISC:
		pppoedisc_input(ifp, m);
		return;

	case ETHERTYPE_PPPOE:
		pppoe_input(ifp, m);
		return;
#endif

	case ETHERTYPE_SLOWPROTOCOLS: {
		uint8_t subtype;

		if (m->m_pkthdr.len < sizeof(*eh) + sizeof(subtype)) {
			m_freem(m);
			return;
		}

		m_copydata(m, sizeof(*eh), sizeof(subtype), &subtype);
		switch (subtype) {
#if NAGR > 0
		case SLOWPROTOCOLS_SUBTYPE_LACP:
			if (ifp->if_agrprivate) {
				ieee8023ad_lacp_input(ifp, m);
				return;
			}
			break;

		case SLOWPROTOCOLS_SUBTYPE_MARKER:
			if (ifp->if_agrprivate) {
				ieee8023ad_marker_input(ifp, m);
				return;
			}
			break;
#endif

		default:
			if (subtype == 0 || subtype > 10) {
				/* illegal value */
				m_freem(m);
				return;
			}
			/* unknown subtype */
			break;
		}
	}
	/* FALLTHROUGH */
	default:
		if (m->m_flags & M_PROMISC) {
			m_freem(m);
			return;
		}
	}

	/* If the CRC is still on the packet, trim it off. */
	if (m->m_flags & M_HASFCS) {
		m_adj(m, -ETHER_CRC_LEN);
		m->m_flags &= ~M_HASFCS;
	}

	if (etype > ETHERMTU + sizeof(struct ether_header)) {
		/* Strip off the Ethernet header. */
		m_adj(m, ehlen);

		switch (etype) {
#ifdef INET
		case ETHERTYPE_IP:
#ifdef GATEWAY
			if (ipflow_fastforward(m))
				return;
#endif
			pktq = ip_pktq;
			break;

		case ETHERTYPE_ARP:
			isr = NETISR_ARP;
			inq = &arpintrq;
			break;

		case ETHERTYPE_REVARP:
			revarpinput(m);	/* XXX queue? */
			return;
#endif

#ifdef INET6
		case ETHERTYPE_IPV6:
			if (__predict_false(!in6_present)) {
				m_freem(m);
				return;
			}
#ifdef GATEWAY
			if (ip6flow_fastforward(&m))
				return;
#endif
			pktq = ip6_pktq;
			break;
#endif

#ifdef NETATALK
		case ETHERTYPE_ATALK:
			isr = NETISR_ATALK;
			inq = &atintrq1;
			break;

		case ETHERTYPE_AARP:
			aarpinput(ifp, m); /* XXX queue? */
			return;
#endif

#ifdef MPLS
		case ETHERTYPE_MPLS:
			isr = NETISR_MPLS;
			inq = &mplsintrq;
			break;
#endif

		default:
			m_freem(m);
			return;
		}
	} else {
		KASSERT(ehlen == sizeof(*eh));
#if defined (LLC) || defined (NETATALK)
		if (m->m_len < sizeof(*eh) + sizeof(struct llc)) {
			goto dropanyway;
		}
		l = (struct llc *)(eh+1);

		switch (l->llc_dsap) {
#ifdef NETATALK
		case LLC_SNAP_LSAP:
			switch (l->llc_control) {
			case LLC_UI:
				if (l->llc_ssap != LLC_SNAP_LSAP) {
					goto dropanyway;
				}

				if (memcmp(&(l->llc_snap_org_code)[0],
				    at_org_code, sizeof(at_org_code)) == 0 &&
				    ntohs(l->llc_snap_ether_type) ==
				    ETHERTYPE_ATALK) {
					inq = &atintrq2;
					m_adj(m, sizeof(struct ether_header)
					    + sizeof(struct llc));
					isr = NETISR_ATALK;
					break;
				}

				if (memcmp(&(l->llc_snap_org_code)[0],
				    aarp_org_code,
				    sizeof(aarp_org_code)) == 0 &&
				    ntohs(l->llc_snap_ether_type) ==
				    ETHERTYPE_AARP) {
					m_adj(m, sizeof(struct ether_header)
					    + sizeof(struct llc));
					aarpinput(ifp, m); /* XXX queue? */
					return;
				}

			default:
				goto dropanyway;
			}
			break;
#endif
		dropanyway:
		default:
			m_freem(m);
			return;
		}
#else /* LLC || NETATALK */
		m_freem(m);
		return;
#endif /* LLC || NETATALK */
	}

	if (__predict_true(pktq)) {
#ifdef NET_MPSAFE
		const u_int h = curcpu()->ci_index;
#else
		const uint32_t h = pktq_rps_hash(m);
#endif
		if (__predict_false(!pktq_enqueue(pktq, m, h))) {
			m_freem(m);
		}
		return;
	}

	if (__predict_false(!inq)) {
		/* Should not happen. */
		m_freem(m);
		return;
	}

	IFQ_LOCK(inq);
	if (IF_QFULL(inq)) {
		IF_DROP(inq);
		IFQ_UNLOCK(inq);
		m_freem(m);
	} else {
		IF_ENQUEUE(inq, m);
		IFQ_UNLOCK(inq);
		schednetisr(isr);
	}
}

/*
 * Convert Ethernet address to printable (loggable) representation.
 */
char *
ether_sprintf(const u_char *ap)
{
	static char etherbuf[3 * ETHER_ADDR_LEN];
	return ether_snprintf(etherbuf, sizeof(etherbuf), ap);
}

char *
ether_snprintf(char *buf, size_t len, const u_char *ap)
{
	char *cp = buf;
	size_t i;

	for (i = 0; i < len / 3; i++) {
		*cp++ = hexdigits[*ap >> 4];
		*cp++ = hexdigits[*ap++ & 0xf];
		*cp++ = ':';
	}
	*--cp = '\0';
	return buf;
}

/*
 * Perform common duties while attaching to interface list
 */
void
ether_ifattach(struct ifnet *ifp, const uint8_t *lla)
{
	struct ethercom *ec = (struct ethercom *)ifp;

	ifp->if_type = IFT_ETHER;
	ifp->if_hdrlen = ETHER_HDR_LEN;
	ifp->if_dlt = DLT_EN10MB;
	ifp->if_mtu = ETHERMTU;
	ifp->if_output = ether_output;
	ifp->_if_input = ether_input;
	if (ifp->if_baudrate == 0)
		ifp->if_baudrate = IF_Mbps(10);		/* just a default */

	if (lla != NULL)
		if_set_sadl(ifp, lla, ETHER_ADDR_LEN, !ETHER_IS_LOCAL(lla));

	LIST_INIT(&ec->ec_multiaddrs);
	SIMPLEQ_INIT(&ec->ec_vids);
	ec->ec_lock = mutex_obj_alloc(MUTEX_DEFAULT, IPL_NET);
	ec->ec_flags = 0;
	ifp->if_broadcastaddr = etherbroadcastaddr;
	bpf_attach(ifp, DLT_EN10MB, sizeof(struct ether_header));
#ifdef MBUFTRACE
	strlcpy(ec->ec_tx_mowner.mo_name, ifp->if_xname,
	    sizeof(ec->ec_tx_mowner.mo_name));
	strlcpy(ec->ec_tx_mowner.mo_descr, "tx",
	    sizeof(ec->ec_tx_mowner.mo_descr));
	strlcpy(ec->ec_rx_mowner.mo_name, ifp->if_xname,
	    sizeof(ec->ec_rx_mowner.mo_name));
	strlcpy(ec->ec_rx_mowner.mo_descr, "rx",
	    sizeof(ec->ec_rx_mowner.mo_descr));
	MOWNER_ATTACH(&ec->ec_tx_mowner);
	MOWNER_ATTACH(&ec->ec_rx_mowner);
	ifp->if_mowner = &ec->ec_tx_mowner;
#endif
}

void
ether_ifdetach(struct ifnet *ifp)
{
	struct ethercom *ec = (void *) ifp;
	struct ether_multi *enm;

	IFNET_ASSERT_UNLOCKED(ifp);
	/*
	 * Prevent further calls to ioctl (for example turning off
	 * promiscuous mode from the bridge code), which eventually can
	 * call if_init() which can cause panics because the interface
	 * is in the process of being detached. Return device not configured
	 * instead.
	 */
	ifp->if_ioctl = __FPTRCAST(int (*)(struct ifnet *, u_long, void *),
	    enxio);

#if NBRIDGE > 0
	if (ifp->if_bridge)
		bridge_ifdetach(ifp);
#endif
	bpf_detach(ifp);
#if NVLAN > 0
	if (ec->ec_nvlans)
		vlan_ifdetach(ifp);
#endif

	ETHER_LOCK(ec);
	KASSERT(ec->ec_nvlans == 0);
	while ((enm = LIST_FIRST(&ec->ec_multiaddrs)) != NULL) {
		LIST_REMOVE(enm, enm_list);
		kmem_free(enm, sizeof(*enm));
		ec->ec_multicnt--;
	}
	ETHER_UNLOCK(ec);

	mutex_obj_free(ec->ec_lock);
	ec->ec_lock = NULL;

	ifp->if_mowner = NULL;
	MOWNER_DETACH(&ec->ec_rx_mowner);
	MOWNER_DETACH(&ec->ec_tx_mowner);
}

#if 0
/*
 * This is for reference.  We have a table-driven version
 * of the little-endian crc32 generator, which is faster
 * than the double-loop.
 */
uint32_t
ether_crc32_le(const uint8_t *buf, size_t len)
{
	uint32_t c, crc, carry;
	size_t i, j;

	crc = 0xffffffffU;	/* initial value */

	for (i = 0; i < len; i++) {
		c = buf[i];
		for (j = 0; j < 8; j++) {
			carry = ((crc & 0x01) ? 1 : 0) ^ (c & 0x01);
			crc >>= 1;
			c >>= 1;
			if (carry)
				crc = (crc ^ ETHER_CRC_POLY_LE);
		}
	}

	return (crc);
}
#else
uint32_t
ether_crc32_le(const uint8_t *buf, size_t len)
{
	static const uint32_t crctab[] = {
		0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
		0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
		0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
		0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
	};
	uint32_t crc;
	size_t i;

	crc = 0xffffffffU;	/* initial value */

	for (i = 0; i < len; i++) {
		crc ^= buf[i];
		crc = (crc >> 4) ^ crctab[crc & 0xf];
		crc = (crc >> 4) ^ crctab[crc & 0xf];
	}

	return (crc);
}
#endif

uint32_t
ether_crc32_be(const uint8_t *buf, size_t len)
{
	uint32_t c, crc, carry;
	size_t i, j;

	crc = 0xffffffffU;	/* initial value */

	for (i = 0; i < len; i++) {
		c = buf[i];
		for (j = 0; j < 8; j++) {
			carry = ((crc & 0x80000000U) ? 1 : 0) ^ (c & 0x01);
			crc <<= 1;
			c >>= 1;
			if (carry)
				crc = (crc ^ ETHER_CRC_POLY_BE) | carry;
		}
	}

	return (crc);
}

#ifdef INET
const uint8_t ether_ipmulticast_min[ETHER_ADDR_LEN] =
    { 0x01, 0x00, 0x5e, 0x00, 0x00, 0x00 };
const uint8_t ether_ipmulticast_max[ETHER_ADDR_LEN] =
    { 0x01, 0x00, 0x5e, 0x7f, 0xff, 0xff };
#endif
#ifdef INET6
const uint8_t ether_ip6multicast_min[ETHER_ADDR_LEN] =
    { 0x33, 0x33, 0x00, 0x00, 0x00, 0x00 };
const uint8_t ether_ip6multicast_max[ETHER_ADDR_LEN] =
    { 0x33, 0x33, 0xff, 0xff, 0xff, 0xff };
#endif

/*
 * ether_aton implementation, not using a static buffer.
 */
int
ether_aton_r(u_char *dest, size_t len, const char *str)
{
	const u_char *cp = (const void *)str;
	u_char *ep;

#define atox(c)	(((c) <= '9') ? ((c) - '0') : ((toupper(c) - 'A') + 10))

	if (len < ETHER_ADDR_LEN)
		return ENOSPC;

	ep = dest + ETHER_ADDR_LEN;

	while (*cp) {
		if (!isxdigit(*cp))
			return EINVAL;

		*dest = atox(*cp);
		cp++;
		if (isxdigit(*cp)) {
			*dest = (*dest << 4) | atox(*cp);
			cp++;
		}
		dest++;

		if (dest == ep)
			return (*cp == '\0') ? 0 : ENAMETOOLONG;

		switch (*cp) {
		case ':':
		case '-':
		case '.':
			cp++;
			break;
		}
	}
	return ENOBUFS;
}

/*
 * Convert a sockaddr into an Ethernet address or range of Ethernet
 * addresses.
 */
int
ether_multiaddr(const struct sockaddr *sa, uint8_t addrlo[ETHER_ADDR_LEN],
    uint8_t addrhi[ETHER_ADDR_LEN])
{
#ifdef INET
	const struct sockaddr_in *sin;
#endif
#ifdef INET6
	const struct sockaddr_in6 *sin6;
#endif

	switch (sa->sa_family) {

	case AF_UNSPEC:
		memcpy(addrlo, sa->sa_data, ETHER_ADDR_LEN);
		memcpy(addrhi, addrlo, ETHER_ADDR_LEN);
		break;

#ifdef INET
	case AF_INET:
		sin = satocsin(sa);
		if (sin->sin_addr.s_addr == INADDR_ANY) {
			/*
			 * An IP address of INADDR_ANY means listen to
			 * or stop listening to all of the Ethernet
			 * multicast addresses used for IP.
			 * (This is for the sake of IP multicast routers.)
			 */
			memcpy(addrlo, ether_ipmulticast_min, ETHER_ADDR_LEN);
			memcpy(addrhi, ether_ipmulticast_max, ETHER_ADDR_LEN);
		} else {
			ETHER_MAP_IP_MULTICAST(&sin->sin_addr, addrlo);
			memcpy(addrhi, addrlo, ETHER_ADDR_LEN);
		}
		break;
#endif
#ifdef INET6
	case AF_INET6:
		sin6 = satocsin6(sa);
		if (IN6_IS_ADDR_UNSPECIFIED(&sin6->sin6_addr)) {
			/*
			 * An IP6 address of 0 means listen to or stop
			 * listening to all of the Ethernet multicast
			 * address used for IP6.
			 * (This is used for multicast routers.)
			 */
			memcpy(addrlo, ether_ip6multicast_min, ETHER_ADDR_LEN);
			memcpy(addrhi, ether_ip6multicast_max, ETHER_ADDR_LEN);
		} else {
			ETHER_MAP_IPV6_MULTICAST(&sin6->sin6_addr, addrlo);
			memcpy(addrhi, addrlo, ETHER_ADDR_LEN);
		}
		break;
#endif

	default:
		return EAFNOSUPPORT;
	}
	return 0;
}

/*
 * Add an Ethernet multicast address or range of addresses to the list for a
 * given interface.
 */
int
ether_addmulti(const struct sockaddr *sa, struct ethercom *ec)
{
	struct ether_multi *enm, *_enm;
	u_char addrlo[ETHER_ADDR_LEN];
	u_char addrhi[ETHER_ADDR_LEN];
	int error = 0;

	/* Allocate out of lock */
	enm = kmem_alloc(sizeof(*enm), KM_SLEEP);

	ETHER_LOCK(ec);
	error = ether_multiaddr(sa, addrlo, addrhi);
	if (error != 0)
		goto out;

	/*
	 * Verify that we have valid Ethernet multicast addresses.
	 */
	if (!ETHER_IS_MULTICAST(addrlo) || !ETHER_IS_MULTICAST(addrhi)) {
		error = EINVAL;
		goto out;
	}

	/*
	 * See if the address range is already in the list.
	 */
	_enm = ether_lookup_multi(addrlo, addrhi, ec);
	if (_enm != NULL) {
		/*
		 * Found it; just increment the reference count.
		 */
		++_enm->enm_refcount;
		error = 0;
		goto out;
	}

	/*
	 * Link a new multicast record into the interface's multicast list.
	 */
	memcpy(enm->enm_addrlo, addrlo, ETHER_ADDR_LEN);
	memcpy(enm->enm_addrhi, addrhi, ETHER_ADDR_LEN);
	enm->enm_refcount = 1;
	LIST_INSERT_HEAD(&ec->ec_multiaddrs, enm, enm_list);
	ec->ec_multicnt++;

	/*
	 * Return ENETRESET to inform the driver that the list has changed
	 * and its reception filter should be adjusted accordingly.
	 */
	error = ENETRESET;
	enm = NULL;

out:
	ETHER_UNLOCK(ec);
	if (enm != NULL)
		kmem_free(enm, sizeof(*enm));
	return error;
}

/*
 * Delete a multicast address record.
 */
int
ether_delmulti(const struct sockaddr *sa, struct ethercom *ec)
{
	struct ether_multi *enm;
	u_char addrlo[ETHER_ADDR_LEN];
	u_char addrhi[ETHER_ADDR_LEN];
	int error;

	ETHER_LOCK(ec);
	error = ether_multiaddr(sa, addrlo, addrhi);
	if (error != 0)
		goto error;

	/*
	 * Look up the address in our list.
	 */
	enm = ether_lookup_multi(addrlo, addrhi, ec);
	if (enm == NULL) {
		error = ENXIO;
		goto error;
	}
	if (--enm->enm_refcount != 0) {
		/*
		 * Still some claims to this record.
		 */
		error = 0;
		goto error;
	}

	/*
	 * No remaining claims to this record; unlink and free it.
	 */
	LIST_REMOVE(enm, enm_list);
	ec->ec_multicnt--;
	ETHER_UNLOCK(ec);
	kmem_free(enm, sizeof(*enm));

	/*
	 * Return ENETRESET to inform the driver that the list has changed
	 * and its reception filter should be adjusted accordingly.
	 */
	return ENETRESET;

error:
	ETHER_UNLOCK(ec);
	return error;
}

void
ether_set_ifflags_cb(struct ethercom *ec, ether_cb_t cb)
{
	ec->ec_ifflags_cb = cb;
}

void
ether_set_vlan_cb(struct ethercom *ec, ether_vlancb_t cb)
{

	ec->ec_vlan_cb = cb;
}

static int
ether_ioctl_reinit(struct ethercom *ec)
{
	struct ifnet *ifp = &ec->ec_if;
	int error;

	switch (ifp->if_flags & (IFF_UP | IFF_RUNNING)) {
	case IFF_RUNNING:
		/*
		 * If interface is marked down and it is running,
		 * then stop and disable it.
		 */
		(*ifp->if_stop)(ifp, 1);
		break;
	case IFF_UP:
		/*
		 * If interface is marked up and it is stopped, then
		 * start it.
		 */
		return (*ifp->if_init)(ifp);
	case IFF_UP | IFF_RUNNING:
		error = 0;
		if (ec->ec_ifflags_cb != NULL) {
			error = (*ec->ec_ifflags_cb)(ec);
			if (error == ENETRESET) {
				/*
				 * Reset the interface to pick up
				 * changes in any other flags that
				 * affect the hardware state.
				 */
				return (*ifp->if_init)(ifp);
			}
		} else
			error = (*ifp->if_init)(ifp);
		return error;
	case 0:
		break;
	}

	return 0;
}

/*
 * Common ioctls for Ethernet interfaces.  Note, we must be
 * called at splnet().
 */
int
ether_ioctl(struct ifnet *ifp, u_long cmd, void *data)
{
	struct ethercom *ec = (void *)ifp;
	struct eccapreq *eccr;
	struct ifreq *ifr = (struct ifreq *)data;
	struct if_laddrreq *iflr = data;
	const struct sockaddr_dl *sdl;
	static const uint8_t zero[ETHER_ADDR_LEN];
	int error;

	switch (cmd) {
	case SIOCINITIFADDR:
	    {
		struct ifaddr *ifa = (struct ifaddr *)data;
		if (ifa->ifa_addr->sa_family != AF_LINK
		    && (ifp->if_flags & (IFF_UP | IFF_RUNNING)) !=
		       (IFF_UP | IFF_RUNNING)) {
			ifp->if_flags |= IFF_UP;
			if ((error = (*ifp->if_init)(ifp)) != 0)
				return error;
		}
#ifdef INET
		if (ifa->ifa_addr->sa_family == AF_INET)
			arp_ifinit(ifp, ifa);
#endif
		return 0;
	    }

	case SIOCSIFMTU:
	    {
		int maxmtu;

		if (ec->ec_capabilities & ETHERCAP_JUMBO_MTU)
			maxmtu = ETHERMTU_JUMBO;
		else
			maxmtu = ETHERMTU;

		if (ifr->ifr_mtu < ETHERMIN || ifr->ifr_mtu > maxmtu)
			return EINVAL;
		else if ((error = ifioctl_common(ifp, cmd, data)) != ENETRESET)
			return error;
		else if (ifp->if_flags & IFF_UP) {
			/* Make sure the device notices the MTU change. */
			return (*ifp->if_init)(ifp);
		} else
			return 0;
	    }

	case SIOCSIFFLAGS:
		if ((error = ifioctl_common(ifp, cmd, data)) != 0)
			return error;
		return ether_ioctl_reinit(ec);
	case SIOCGIFFLAGS:
		error = ifioctl_common(ifp, cmd, data);
		if (error == 0) {
			/* Set IFF_ALLMULTI for backcompat */
			ifr->ifr_flags |= (ec->ec_flags & ETHER_F_ALLMULTI) ?
			    IFF_ALLMULTI : 0;
		}
		return error;
	case SIOCGETHERCAP:
		eccr = (struct eccapreq *)data;
		eccr->eccr_capabilities = ec->ec_capabilities;
		eccr->eccr_capenable = ec->ec_capenable;
		return 0;
	case SIOCSETHERCAP:
		eccr = (struct eccapreq *)data;
		if ((eccr->eccr_capenable & ~ec->ec_capabilities) != 0)
			return EINVAL;
		if (eccr->eccr_capenable == ec->ec_capenable)
			return 0;
#if 0 /* notyet */
		ec->ec_capenable = (ec->ec_capenable & ETHERCAP_CANTCHANGE)
		    | (eccr->eccr_capenable & ~ETHERCAP_CANTCHANGE);
#else
		ec->ec_capenable = eccr->eccr_capenable;
#endif
		return ether_ioctl_reinit(ec);
	case SIOCADDMULTI:
		return ether_addmulti(ifreq_getaddr(cmd, ifr), ec);
	case SIOCDELMULTI:
		return ether_delmulti(ifreq_getaddr(cmd, ifr), ec);
	case SIOCSIFMEDIA:
	case SIOCGIFMEDIA:
		if (ec->ec_mii != NULL)
			return ifmedia_ioctl(ifp, ifr, &ec->ec_mii->mii_media,
			    cmd);
		else if (ec->ec_ifmedia != NULL)
			return ifmedia_ioctl(ifp, ifr, ec->ec_ifmedia, cmd);
		else
			return ENOTTY;
		break;
	case SIOCALIFADDR:
		sdl = satocsdl(sstocsa(&iflr->addr));
		if (sdl->sdl_family != AF_LINK)
			;
		else if (ETHER_IS_MULTICAST(CLLADDR(sdl)))
			return EINVAL;
		else if (memcmp(zero, CLLADDR(sdl), sizeof(zero)) == 0)
			return EINVAL;
		/*FALLTHROUGH*/
	default:
		return ifioctl_common(ifp, cmd, data);
	}
	return 0;
}

/*
 * Enable/disable passing VLAN packets if the parent interface supports it.
 * Return:
 * 	 0: Ok
 *	-1: Parent interface does not support vlans
 *	>0: Error
 */
int
ether_enable_vlan_mtu(struct ifnet *ifp)
{
	int error;
	struct ethercom *ec = (void *)ifp;

	/* Parent does not support VLAN's */
	if ((ec->ec_capabilities & ETHERCAP_VLAN_MTU) == 0)
		return -1;

	/*
	 * Parent supports the VLAN_MTU capability,
	 * i.e. can Tx/Rx larger than ETHER_MAX_LEN frames;
	 * enable it.
	 */
	ec->ec_capenable |= ETHERCAP_VLAN_MTU;

	/* Interface is down, defer for later */
	if ((ifp->if_flags & IFF_UP) == 0)
		return 0;

	if ((error = if_flags_set(ifp, ifp->if_flags)) == 0)
		return 0;

	ec->ec_capenable &= ~ETHERCAP_VLAN_MTU;
	return error;
}

int
ether_disable_vlan_mtu(struct ifnet *ifp)
{
	int error;
	struct ethercom *ec = (void *)ifp;

	/* We still have VLAN's, defer for later */
	if (ec->ec_nvlans != 0)
		return 0;

	/* Parent does not support VLAB's, nothing to do. */
	if ((ec->ec_capenable & ETHERCAP_VLAN_MTU) == 0)
		return -1;

	/*
	 * Disable Tx/Rx of VLAN-sized frames.
	 */
	ec->ec_capenable &= ~ETHERCAP_VLAN_MTU;

	/* Interface is down, defer for later */
	if ((ifp->if_flags & IFF_UP) == 0)
		return 0;

	if ((error = if_flags_set(ifp, ifp->if_flags)) == 0)
		return 0;

	ec->ec_capenable |= ETHERCAP_VLAN_MTU;
	return error;
}

static int
ether_multicast_sysctl(SYSCTLFN_ARGS)
{
	struct ether_multi *enm;
	struct ifnet *ifp;
	struct ethercom *ec;
	int error = 0;
	size_t written;
	struct psref psref;
	int bound;
	unsigned int multicnt;
	struct ether_multi_sysctl *addrs;
	int i;

	if (namelen != 1)
		return EINVAL;

	bound = curlwp_bind();
	ifp = if_get_byindex(name[0], &psref);
	if (ifp == NULL) {
		error = ENODEV;
		goto out;
	}
	if (ifp->if_type != IFT_ETHER) {
		if_put(ifp, &psref);
		*oldlenp = 0;
		goto out;
	}
	ec = (struct ethercom *)ifp;

	if (oldp == NULL) {
		if_put(ifp, &psref);
		*oldlenp = ec->ec_multicnt * sizeof(*addrs);
		goto out;
	}

	/*
	 * ec->ec_lock is a spin mutex so we cannot call sysctl_copyout, which
	 * is sleepable, while holding it. Copy data to a local buffer first
	 * with the lock taken and then call sysctl_copyout without holding it.
	 */
retry:
	multicnt = ec->ec_multicnt;

	if (multicnt == 0) {
		if_put(ifp, &psref);
		*oldlenp = 0;
		goto out;
	}

	addrs = kmem_zalloc(sizeof(*addrs) * multicnt, KM_SLEEP);

	ETHER_LOCK(ec);
	if (multicnt != ec->ec_multicnt) {
		/* The number of multicast addresses has changed */
		ETHER_UNLOCK(ec);
		kmem_free(addrs, sizeof(*addrs) * multicnt);
		goto retry;
	}

	i = 0;
	LIST_FOREACH(enm, &ec->ec_multiaddrs, enm_list) {
		struct ether_multi_sysctl *addr = &addrs[i];
		addr->enm_refcount = enm->enm_refcount;
		memcpy(addr->enm_addrlo, enm->enm_addrlo, ETHER_ADDR_LEN);
		memcpy(addr->enm_addrhi, enm->enm_addrhi, ETHER_ADDR_LEN);
		i++;
	}
	ETHER_UNLOCK(ec);

	error = 0;
	written = 0;
	for (i = 0; i < multicnt; i++) {
		struct ether_multi_sysctl *addr = &addrs[i];

		if (written + sizeof(*addr) > *oldlenp)
			break;
		error = sysctl_copyout(l, addr, oldp, sizeof(*addr));
		if (error)
			break;
		written += sizeof(*addr);
		oldp = (char *)oldp + sizeof(*addr);
	}
	kmem_free(addrs, sizeof(*addrs) * multicnt);

	if_put(ifp, &psref);

	*oldlenp = written;
out:
	curlwp_bindx(bound);
	return error;
}

static void
ether_sysctl_setup(struct sysctllog **clog)
{
	const struct sysctlnode *rnode = NULL;

	sysctl_createv(clog, 0, NULL, &rnode,
		       CTLFLAG_PERMANENT,
		       CTLTYPE_NODE, "ether",
		       SYSCTL_DESCR("Ethernet-specific information"),
		       NULL, 0, NULL, 0,
		       CTL_NET, CTL_CREATE, CTL_EOL);

	sysctl_createv(clog, 0, &rnode, NULL,
		       CTLFLAG_PERMANENT,
		       CTLTYPE_NODE, "multicast",
		       SYSCTL_DESCR("multicast addresses"),
		       ether_multicast_sysctl, 0, NULL, 0,
		       CTL_CREATE, CTL_EOL);
}

void
etherinit(void)
{

#ifdef DIAGNOSTIC
	mutex_init(&bigpktpps_lock, MUTEX_DEFAULT, IPL_NET);
#endif
	ether_sysctl_setup(NULL);
}
