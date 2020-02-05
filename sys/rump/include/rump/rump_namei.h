/*	$NetBSD: rump_namei.h,v 1.38 2020/01/08 12:06:09 ad Exp $	*/


/*
 * WARNING: GENERATED FILE.  DO NOT EDIT
 * (edit namei.src and run make namei in src/sys/sys)
 *   by:   NetBSD: gennameih.awk,v 1.5 2009/12/23 14:17:19 pooka Exp 
 *   from: NetBSD: namei.src,v 1.48 2020/01/08 12:04:56 ad Exp 
 */

#ifndef _RUMP_RUMP_NAMEI_H_
#define _RUMP_RUMP_NAMEI_H_

#define RUMP_NAMEI_LOOKUP	0
#define RUMP_NAMEI_CREATE	1
#define RUMP_NAMEI_DELETE	2
#define RUMP_NAMEI_RENAME	3
#define RUMP_NAMEI_OPMASK	3
#define RUMP_NAMEI_LOCKLEAF	0x00000004
#define RUMP_NAMEI_LOCKPARENT	0x00000008
#define RUMP_NAMEI_TRYEMULROOT	0x00000010
#define RUMP_NAMEI_NOCACHE	0x00000020
#define RUMP_NAMEI_FOLLOW	0x00000040
#define RUMP_NAMEI_NOFOLLOW	0x00000000
#define RUMP_NAMEI_EMULROOTSET	0x00000080
#define RUMP_NAMEI_NOCHROOT	0x01000000
#define RUMP_NAMEI_MODMASK	0x010000fc
#define RUMP_NAMEI_NOCROSSMOUNT	0x0000100
#define RUMP_NAMEI_RDONLY	0x0000200
#define RUMP_NAMEI_ISDOTDOT	0x0002000
#define RUMP_NAMEI_MAKEENTRY	0x0004000
#define RUMP_NAMEI_ISLASTCN	0x0008000
#define RUMP_NAMEI_ISWHITEOUT	0x0020000
#define RUMP_NAMEI_DOWHITEOUT	0x0040000
#define RUMP_NAMEI_REQUIREDIR	0x0080000
#define RUMP_NAMEI_CREATEDIR	0x0200000
#define RUMP_NAMEI_PARAMASK	0x02ee300

#endif /* _RUMP_RUMP_NAMEI_H_ */
