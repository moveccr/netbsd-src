/*	$NetBSD: imx7_ocotpreg.h,v 1.1 2016/05/17 06:44:45 ryo Exp $	*/

/*
 * Copyright (c) 2015 Internet Initiative Japan, Inc.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ARM_IMX_IMX7_OCOTP_H_
#define _ARM_IMX_IMX7_OCOTP_H_

#define OCOTP_CTRL			0x00000000
#define OCOTP_CTRL_SET			0x00000004
#define OCOTP_CTRL_CLR			0x00000008
#define OCOTP_CTRL_TOG			0x0000000c
#define OCOTP_TIMING			0x00000010
#define OCOTP_DATA0			0x00000020
#define OCOTP_DATA1			0x00000030
#define OCOTP_DATA2			0x00000040
#define OCOTP_DATA3			0x00000050
#define OCOTP_READ_CTRL			0x00000060
#define OCOTP_READ_FUSE_DATA0		0x00000070
#define OCOTP_READ_FUSE_DATA1		0x00000080
#define OCOTP_READ_FUSE_DATA2		0x00000090
#define OCOTP_READ_FUSE_DATA3		0x000000a0
#define OCOTP_SW_STICKY			0x000000b0
#define OCOTP_SCS			0x000000c0
#define OCOTP_SCS_SET			0x000000c4
#define OCOTP_SCS_CLR			0x000000c8
#define OCOTP_SCS_TOG			0x000000cc
#define OCOTP_CRC_ADDR			0x000000d0
#define OCOTP_CRC_VALUE			0x000000e0
#define OCOTP_VERSION			0x000000f0
#define OCOTP_LOCK			0x00000400
#define OCOTP_TESTER0			0x00000410
#define OCOTP_TESTER1			0x00000420
#define OCOTP_TESTER2			0x00000430
#define OCOTP_TESTER3			0x00000440
#define OCOTP_TESTER4			0x00000450
#define OCOTP_TESTER5			0x00000460
#define OCOTP_BOOT_CFG0			0x00000470
#define OCOTP_BOOT_CFG1			0x00000480
#define OCOTP_BOOT_CFG2			0x00000490
#define OCOTP_BOOT_CFG3			0x000004a0
#define OCOTP_MEM_TRIM0			0x000004c0
#define OCOTP_MEM_TRIM1			0x000004d0
#define OCOTP_ANA0			0x000004e0
#define OCOTP_ANA1			0x000004f0
#define OCOTP_OTPMK0			0x00000500
#define OCOTP_OTPMK1			0x00000510
#define OCOTP_OTPMK2			0x00000520
#define OCOTP_OTPMK3			0x00000530
#define OCOTP_OTPMK4			0x00000540
#define OCOTP_OTPMK5			0x00000550
#define OCOTP_OTPMK6			0x00000560
#define OCOTP_OTPMK7			0x00000570
#define OCOTP_SRK0			0x00000580
#define OCOTP_SRK1			0x00000590
#define OCOTP_SRK2			0x000005a0
#define OCOTP_SRK3			0x000005b0
#define OCOTP_SRK4			0x000005c0
#define OCOTP_SRK5			0x000005d0
#define OCOTP_SRK6			0x000005e0
#define OCOTP_SRK7			0x000005f0
#define OCOTP_SJC_RESP0			0x00000600
#define OCOTP_SJC_RESP1			0x00000610
#define OCOTP_USB_ID			0x00000620
#define OCOTP_FIELD_RETURN		0x00000630
#define OCOTP_MAC_ADDR0			0x00000640
#define OCOTP_MAC_ADDR1			0x00000650
#define OCOTP_MAC_ADDR2			0x00000660
#define OCOTP_SRK_REVOKE		0x00000670
#define OCOTP_MAU_KEY0			0x00000680
#define OCOTP_MAU_KEY1			0x00000690
#define OCOTP_MAU_KEY2			0x000006a0
#define OCOTP_MAU_KEY3			0x000006b0
#define OCOTP_MAU_KEY4			0x000006c0
#define OCOTP_MAU_KEY5			0x000006d0
#define OCOTP_MAU_KEY6			0x000006e0
#define OCOTP_MAU_KEY7			0x000006f0
#define OCOTP_GP10			0x00000780
#define OCOTP_GP11			0x00000790
#define OCOTP_GP20			0x000007a0
#define OCOTP_GP21			0x000007b0
#define OCOTP_CRC_GP10			0x000007c0
#define OCOTP_CRC_GP11			0x000007d0
#define OCOTP_CRC_GP20			0x000007e0
#define OCOTP_CRC_GP21			0x000007f0

#define AIPS2_OCOTP_CTRL_SIZE		0x00000800

#endif /* _ARM_IMX_IMX7_OCOTP_H_ */
