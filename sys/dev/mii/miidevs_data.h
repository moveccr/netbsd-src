/*	$NetBSD: miidevs_data.h,v 1.148 2020/02/05 06:38:36 msaitoh Exp $	*/

/*
 * THIS FILE AUTOMATICALLY GENERATED.  DO NOT EDIT.
 *
 * generated from:
 *	NetBSD: miidevs,v 1.162 2020/02/05 06:38:20 msaitoh Exp
 */

/*-
 * Copyright (c) 1998, 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe of the Numerical Aerospace Simulation Facility,
 * NASA Ames Research Center.
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
struct mii_knowndev {
	int oui;
	int model;
	const char *descr;
};
struct mii_knowndev mii_knowndevs[] = {
 { MII_OUI_AGERE, MII_MODEL_AGERE_ET1011, MII_STR_AGERE_ET1011 },
 { MII_OUI_AGERE, MII_MODEL_AGERE_ET1011C, MII_STR_AGERE_ET1011C },
 { MII_OUI_xxASIX, MII_MODEL_xxASIX_AX88X9X, MII_STR_xxASIX_AX88X9X },
 { MII_OUI_yyASIX, MII_MODEL_yyASIX_AX88772, MII_STR_yyASIX_AX88772 },
 { MII_OUI_yyASIX, MII_MODEL_yyASIX_AX88772A, MII_STR_yyASIX_AX88772A },
 { MII_OUI_yyASIX, MII_MODEL_yyASIX_AX88772B, MII_STR_yyASIX_AX88772B },
 { MII_OUI_ALTIMA, MII_MODEL_ALTIMA_ACXXX, MII_STR_ALTIMA_ACXXX },
 { MII_OUI_ALTIMA, MII_MODEL_ALTIMA_AC101L, MII_STR_ALTIMA_AC101L },
 { MII_OUI_ALTIMA, MII_MODEL_ALTIMA_AC101, MII_STR_ALTIMA_AC101 },
 { MII_OUI_ALTIMA, MII_MODEL_ALTIMA_Am79C875, MII_STR_ALTIMA_Am79C875 },
 { MII_OUI_ALTIMA, MII_MODEL_ALTIMA_Am79C874, MII_STR_ALTIMA_Am79C874 },
 { MII_OUI_AMLOGIC, MII_MODEL_AMLOGIC_GXL, MII_STR_AMLOGIC_GXL },
 { MII_OUI_xxAMLOGIC, MII_MODEL_xxAMLOGIC_GXL, MII_STR_xxAMLOGIC_GXL },
 { MII_OUI_ATHEROS, MII_MODEL_ATHEROS_F1, MII_STR_ATHEROS_F1 },
 { MII_OUI_ATHEROS, MII_MODEL_ATHEROS_F2, MII_STR_ATHEROS_F2 },
 { MII_OUI_ATTANSIC, MII_MODEL_ATTANSIC_L1, MII_STR_ATTANSIC_L1 },
 { MII_OUI_ATTANSIC, MII_MODEL_ATTANSIC_L2, MII_STR_ATTANSIC_L2 },
 { MII_OUI_ATTANSIC, MII_MODEL_ATTANSIC_AR8021, MII_STR_ATTANSIC_AR8021 },
 { MII_OUI_ATTANSIC, MII_MODEL_ATTANSIC_AR8035, MII_STR_ATTANSIC_AR8035 },
 { MII_OUI_yyAMD, MII_MODEL_yyAMD_79C972_10T, MII_STR_yyAMD_79C972_10T },
 { MII_OUI_yyAMD, MII_MODEL_yyAMD_79c973phy, MII_STR_yyAMD_79c973phy },
 { MII_OUI_yyAMD, MII_MODEL_yyAMD_79c901, MII_STR_yyAMD_79c901 },
 { MII_OUI_yyAMD, MII_MODEL_yyAMD_79c901home, MII_STR_yyAMD_79c901home },
 { MII_OUI_xxBROADCOM, MII_MODEL_xxBROADCOM_3C905B, MII_STR_xxBROADCOM_3C905B },
 { MII_OUI_xxBROADCOM, MII_MODEL_xxBROADCOM_3C905C, MII_STR_xxBROADCOM_3C905C },
 { MII_OUI_xxBROADCOM, MII_MODEL_xxBROADCOM_BCM5221, MII_STR_xxBROADCOM_BCM5221 },
 { MII_OUI_xxBROADCOM, MII_MODEL_xxBROADCOM_BCM5201, MII_STR_xxBROADCOM_BCM5201 },
 { MII_OUI_xxBROADCOM, MII_MODEL_xxBROADCOM_BCM5214, MII_STR_xxBROADCOM_BCM5214 },
 { MII_OUI_xxBROADCOM, MII_MODEL_xxBROADCOM_BCM5222, MII_STR_xxBROADCOM_BCM5222 },
 { MII_OUI_xxBROADCOM, MII_MODEL_xxBROADCOM_BCM4401, MII_STR_xxBROADCOM_BCM4401 },
 { MII_OUI_xxBROADCOM, MII_MODEL_xxBROADCOM_BCM5365, MII_STR_xxBROADCOM_BCM5365 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5400, MII_STR_BROADCOM_BCM5400 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5401, MII_STR_BROADCOM_BCM5401 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5402, MII_STR_BROADCOM_BCM5402 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5411, MII_STR_BROADCOM_BCM5411 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5404, MII_STR_BROADCOM_BCM5404 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5424, MII_STR_BROADCOM_BCM5424 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5464, MII_STR_BROADCOM_BCM5464 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5461, MII_STR_BROADCOM_BCM5461 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5462, MII_STR_BROADCOM_BCM5462 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5421, MII_STR_BROADCOM_BCM5421 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5752, MII_STR_BROADCOM_BCM5752 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5701, MII_STR_BROADCOM_BCM5701 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5706, MII_STR_BROADCOM_BCM5706 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5703, MII_STR_BROADCOM_BCM5703 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5750, MII_STR_BROADCOM_BCM5750 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5704, MII_STR_BROADCOM_BCM5704 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5705, MII_STR_BROADCOM_BCM5705 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM54K2, MII_STR_BROADCOM_BCM54K2 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5714, MII_STR_BROADCOM_BCM5714 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5780, MII_STR_BROADCOM_BCM5780 },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5708C, MII_STR_BROADCOM_BCM5708C },
 { MII_OUI_BROADCOM, MII_MODEL_BROADCOM_BCM5466, MII_STR_BROADCOM_BCM5466 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5325, MII_STR_BROADCOM2_BCM5325 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5906, MII_STR_BROADCOM2_BCM5906 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5478, MII_STR_BROADCOM2_BCM5478 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5488, MII_STR_BROADCOM2_BCM5488 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5481, MII_STR_BROADCOM2_BCM5481 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5482, MII_STR_BROADCOM2_BCM5482 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5755, MII_STR_BROADCOM2_BCM5755 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5756, MII_STR_BROADCOM2_BCM5756 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5754, MII_STR_BROADCOM2_BCM5754 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5708S, MII_STR_BROADCOM2_BCM5708S },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5785, MII_STR_BROADCOM2_BCM5785 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5709CAX, MII_STR_BROADCOM2_BCM5709CAX },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5722, MII_STR_BROADCOM2_BCM5722 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5784, MII_STR_BROADCOM2_BCM5784 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5709C, MII_STR_BROADCOM2_BCM5709C },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5761, MII_STR_BROADCOM2_BCM5761 },
 { MII_OUI_BROADCOM2, MII_MODEL_BROADCOM2_BCM5709S, MII_STR_BROADCOM2_BCM5709S },
 { MII_OUI_BROADCOM3, MII_MODEL_BROADCOM3_BCM57780, MII_STR_BROADCOM3_BCM57780 },
 { MII_OUI_BROADCOM3, MII_MODEL_BROADCOM3_BCM5717C, MII_STR_BROADCOM3_BCM5717C },
 { MII_OUI_BROADCOM3, MII_MODEL_BROADCOM3_BCM5719C, MII_STR_BROADCOM3_BCM5719C },
 { MII_OUI_BROADCOM3, MII_MODEL_BROADCOM3_BCM57765, MII_STR_BROADCOM3_BCM57765 },
 { MII_OUI_BROADCOM3, MII_MODEL_BROADCOM3_BCM53125, MII_STR_BROADCOM3_BCM53125 },
 { MII_OUI_BROADCOM3, MII_MODEL_BROADCOM3_BCM5720C, MII_STR_BROADCOM3_BCM5720C },
 { MII_OUI_BROADCOM4, MII_MODEL_BROADCOM4_BCM5725C, MII_STR_BROADCOM4_BCM5725C },
 { MII_OUI_xxBROADCOM_ALT1, MII_MODEL_xxBROADCOM_ALT1_BCM5906, MII_STR_xxBROADCOM_ALT1_BCM5906 },
 { MII_OUI_xxCICADA, MII_MODEL_xxCICADA_CIS8201, MII_STR_xxCICADA_CIS8201 },
 { MII_OUI_xxCICADA, MII_MODEL_xxCICADA_CIS8204, MII_STR_xxCICADA_CIS8204 },
 { MII_OUI_xxCICADA, MII_MODEL_xxCICADA_VSC8211, MII_STR_xxCICADA_VSC8211 },
 { MII_OUI_xxCICADA, MII_MODEL_xxCICADA_VSC8221, MII_STR_xxCICADA_VSC8221 },
 { MII_OUI_xxCICADA, MII_MODEL_xxCICADA_VSC8224, MII_STR_xxCICADA_VSC8224 },
 { MII_OUI_xxCICADA, MII_MODEL_xxCICADA_CIS8201A, MII_STR_xxCICADA_CIS8201A },
 { MII_OUI_xxCICADA, MII_MODEL_xxCICADA_CIS8201B, MII_STR_xxCICADA_CIS8201B },
 { MII_OUI_xxCICADA, MII_MODEL_xxCICADA_VSC8234, MII_STR_xxCICADA_VSC8234 },
 { MII_OUI_xxCICADA, MII_MODEL_xxCICADA_VSC8244, MII_STR_xxCICADA_VSC8244 },
 { MII_OUI_DAVICOM, MII_MODEL_DAVICOM_DM9101, MII_STR_DAVICOM_DM9101 },
 { MII_OUI_xxDAVICOM, MII_MODEL_xxDAVICOM_DM9101, MII_STR_xxDAVICOM_DM9101 },
 { MII_OUI_xxDAVICOM, MII_MODEL_xxDAVICOM_DM9102, MII_STR_xxDAVICOM_DM9102 },
 { MII_OUI_xxDAVICOM, MII_MODEL_xxDAVICOM_DM9161, MII_STR_xxDAVICOM_DM9161 },
 { MII_OUI_xxDAVICOM, MII_MODEL_xxDAVICOM_DM9161A, MII_STR_xxDAVICOM_DM9161A },
 { MII_OUI_xxDAVICOM, MII_MODEL_xxDAVICOM_DM9161B, MII_STR_xxDAVICOM_DM9161B },
 { MII_OUI_xxDAVICOM, MII_MODEL_xxDAVICOM_DM9601, MII_STR_xxDAVICOM_DM9601 },
 { MII_OUI_xxICPLUS, MII_MODEL_xxICPLUS_IP100, MII_STR_xxICPLUS_IP100 },
 { MII_OUI_xxICPLUS, MII_MODEL_xxICPLUS_IP101, MII_STR_xxICPLUS_IP101 },
 { MII_OUI_xxICPLUS, MII_MODEL_xxICPLUS_IP1000A, MII_STR_xxICPLUS_IP1000A },
 { MII_OUI_xxICPLUS, MII_MODEL_xxICPLUS_IP1001, MII_STR_xxICPLUS_IP1001 },
 { MII_OUI_ICS, MII_MODEL_ICS_1889, MII_STR_ICS_1889 },
 { MII_OUI_ICS, MII_MODEL_ICS_1890, MII_STR_ICS_1890 },
 { MII_OUI_ICS, MII_MODEL_ICS_1892, MII_STR_ICS_1892 },
 { MII_OUI_ICS, MII_MODEL_ICS_1893, MII_STR_ICS_1893 },
 { MII_OUI_ICS, MII_MODEL_ICS_1893C, MII_STR_ICS_1893C },
 { MII_OUI_xxINTEL, MII_MODEL_xxINTEL_I82553, MII_STR_xxINTEL_I82553 },
 { MII_OUI_yyINTEL, MII_MODEL_yyINTEL_I82555, MII_STR_yyINTEL_I82555 },
 { MII_OUI_yyINTEL, MII_MODEL_yyINTEL_I82562EH, MII_STR_yyINTEL_I82562EH },
 { MII_OUI_yyINTEL, MII_MODEL_yyINTEL_I82562G, MII_STR_yyINTEL_I82562G },
 { MII_OUI_yyINTEL, MII_MODEL_yyINTEL_I82562EM, MII_STR_yyINTEL_I82562EM },
 { MII_OUI_yyINTEL, MII_MODEL_yyINTEL_I82562ET, MII_STR_yyINTEL_I82562ET },
 { MII_OUI_yyINTEL, MII_MODEL_yyINTEL_I82553, MII_STR_yyINTEL_I82553 },
 { MII_OUI_yyINTEL, MII_MODEL_yyINTEL_IGP01E1000, MII_STR_yyINTEL_IGP01E1000 },
 { MII_OUI_yyINTEL, MII_MODEL_yyINTEL_I82566, MII_STR_yyINTEL_I82566 },
 { MII_OUI_INTEL, MII_MODEL_INTEL_I82577, MII_STR_INTEL_I82577 },
 { MII_OUI_INTEL, MII_MODEL_INTEL_I82579, MII_STR_INTEL_I82579 },
 { MII_OUI_INTEL, MII_MODEL_INTEL_I217, MII_STR_INTEL_I217 },
 { MII_OUI_INTEL, MII_MODEL_INTEL_X540, MII_STR_INTEL_X540 },
 { MII_OUI_INTEL, MII_MODEL_INTEL_X550, MII_STR_INTEL_X550 },
 { MII_OUI_INTEL, MII_MODEL_INTEL_X557, MII_STR_INTEL_X557 },
 { MII_OUI_INTEL, MII_MODEL_INTEL_I82580, MII_STR_INTEL_I82580 },
 { MII_OUI_INTEL, MII_MODEL_INTEL_I350, MII_STR_INTEL_I350 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_I210, MII_STR_xxMARVELL_I210 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_I82563, MII_STR_xxMARVELL_I82563 },
 { MII_OUI_ATTANSIC, MII_MODEL_ATTANSIC_I82578, MII_STR_ATTANSIC_I82578 },
 { MII_OUI_JMICRON, MII_MODEL_JMICRON_JMP211, MII_STR_JMICRON_JMP211 },
 { MII_OUI_JMICRON, MII_MODEL_JMICRON_JMP202, MII_STR_JMICRON_JMP202 },
 { MII_OUI_xxLEVEL1, MII_MODEL_xxLEVEL1_LXT970, MII_STR_xxLEVEL1_LXT970 },
 { MII_OUI_LEVEL1, MII_MODEL_LEVEL1_LXT1000_OLD, MII_STR_LEVEL1_LXT1000_OLD },
 { MII_OUI_LEVEL1, MII_MODEL_LEVEL1_LXT974, MII_STR_LEVEL1_LXT974 },
 { MII_OUI_LEVEL1, MII_MODEL_LEVEL1_LXT975, MII_STR_LEVEL1_LXT975 },
 { MII_OUI_LEVEL1, MII_MODEL_LEVEL1_LXT1000, MII_STR_LEVEL1_LXT1000 },
 { MII_OUI_LEVEL1, MII_MODEL_LEVEL1_LXT971, MII_STR_LEVEL1_LXT971 },
 { MII_OUI_LEVEL1, MII_MODEL_LEVEL1_LXT973, MII_STR_LEVEL1_LXT973 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1000, MII_STR_xxMARVELL_E1000 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1011, MII_STR_xxMARVELL_E1011 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1000_3, MII_STR_xxMARVELL_E1000_3 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1000S, MII_STR_xxMARVELL_E1000S },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1000_5, MII_STR_xxMARVELL_E1000_5 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1101, MII_STR_xxMARVELL_E1101 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E3082, MII_STR_xxMARVELL_E3082 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1112, MII_STR_xxMARVELL_E1112 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1149, MII_STR_xxMARVELL_E1149 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1111, MII_STR_xxMARVELL_E1111 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1145, MII_STR_xxMARVELL_E1145 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E6060, MII_STR_xxMARVELL_E6060 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_I347, MII_STR_xxMARVELL_I347 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1512, MII_STR_xxMARVELL_E1512 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1340M, MII_STR_xxMARVELL_E1340M },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1116, MII_STR_xxMARVELL_E1116 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1118, MII_STR_xxMARVELL_E1118 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1240, MII_STR_xxMARVELL_E1240 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1116R, MII_STR_xxMARVELL_E1116R },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1149R, MII_STR_xxMARVELL_E1149R },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E3016, MII_STR_xxMARVELL_E3016 },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_PHYG65G, MII_STR_xxMARVELL_PHYG65G },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1318S, MII_STR_xxMARVELL_E1318S },
 { MII_OUI_xxMARVELL, MII_MODEL_xxMARVELL_E1543, MII_STR_xxMARVELL_E1543 },
 { MII_OUI_MARVELL, MII_MODEL_MARVELL_E1000_0, MII_STR_MARVELL_E1000_0 },
 { MII_OUI_MARVELL, MII_MODEL_MARVELL_E1011, MII_STR_MARVELL_E1011 },
 { MII_OUI_MARVELL, MII_MODEL_MARVELL_E1000_3, MII_STR_MARVELL_E1000_3 },
 { MII_OUI_MARVELL, MII_MODEL_MARVELL_E1000_5, MII_STR_MARVELL_E1000_5 },
 { MII_OUI_MARVELL, MII_MODEL_MARVELL_E1000_6, MII_STR_MARVELL_E1000_6 },
 { MII_OUI_MARVELL, MII_MODEL_MARVELL_E1111, MII_STR_MARVELL_E1111 },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KSZ8041, MII_STR_MICREL_KSZ8041 },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KSZ8041RNLI, MII_STR_MICREL_KSZ8041RNLI },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KSZ8051, MII_STR_MICREL_KSZ8051 },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KSZ8081, MII_STR_MICREL_KSZ8081 },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KSZ8061, MII_STR_MICREL_KSZ8061 },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KSZ9021_8001_8721, MII_STR_MICREL_KSZ9021_8001_8721 },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KSZ9031, MII_STR_MICREL_KSZ9031 },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KSZ9477, MII_STR_MICREL_KSZ9477 },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KSZ9131, MII_STR_MICREL_KSZ9131 },
 { MII_OUI_MICREL, MII_MODEL_MICREL_KS8737, MII_STR_MICREL_KS8737 },
 { MII_OUI_xxMYSON, MII_MODEL_xxMYSON_MTD972, MII_STR_xxMYSON_MTD972 },
 { MII_OUI_MYSON, MII_MODEL_MYSON_MTD803, MII_STR_MYSON_MTD803 },
 { MII_OUI_xxNATSEMI, MII_MODEL_xxNATSEMI_DP83840, MII_STR_xxNATSEMI_DP83840 },
 { MII_OUI_xxNATSEMI, MII_MODEL_xxNATSEMI_DP83843, MII_STR_xxNATSEMI_DP83843 },
 { MII_OUI_xxNATSEMI, MII_MODEL_xxNATSEMI_DP83815, MII_STR_xxNATSEMI_DP83815 },
 { MII_OUI_xxNATSEMI, MII_MODEL_xxNATSEMI_DP83847, MII_STR_xxNATSEMI_DP83847 },
 { MII_OUI_xxNATSEMI, MII_MODEL_xxNATSEMI_DP83891, MII_STR_xxNATSEMI_DP83891 },
 { MII_OUI_xxNATSEMI, MII_MODEL_xxNATSEMI_DP83861, MII_STR_xxNATSEMI_DP83861 },
 { MII_OUI_xxNATSEMI, MII_MODEL_xxNATSEMI_DP83865, MII_STR_xxNATSEMI_DP83865 },
 { MII_OUI_xxNATSEMI, MII_MODEL_xxNATSEMI_DP83849, MII_STR_xxNATSEMI_DP83849 },
 { MII_OUI_xxPMCSIERRA, MII_MODEL_xxPMCSIERRA_PM8351, MII_STR_xxPMCSIERRA_PM8351 },
 { MII_OUI_xxPMCSIERRA2, MII_MODEL_xxPMCSIERRA2_PM8352, MII_STR_xxPMCSIERRA2_PM8352 },
 { MII_OUI_xxPMCSIERRA2, MII_MODEL_xxPMCSIERRA2_PM8353, MII_STR_xxPMCSIERRA2_PM8353 },
 { MII_OUI_PMCSIERRA, MII_MODEL_PMCSIERRA_PM8354, MII_STR_PMCSIERRA_PM8354 },
 { MII_OUI_xxQUALSEMI, MII_MODEL_xxQUALSEMI_QS6612, MII_STR_xxQUALSEMI_QS6612 },
 { MII_OUI_xxRDC, MII_MODEL_xxRDC_R6040, MII_STR_xxRDC_R6040 },
 { MII_OUI_xxREALTEK, MII_MODEL_xxREALTEK_RTL8169S, MII_STR_xxREALTEK_RTL8169S },
 { MII_OUI_yyREALTEK, MII_MODEL_yyREALTEK_RTL8201L, MII_STR_yyREALTEK_RTL8201L },
 { MII_OUI_REALTEK, MII_MODEL_REALTEK_RTL8251, MII_STR_REALTEK_RTL8251 },
 { MII_OUI_REALTEK, MII_MODEL_REALTEK_RTL8201E, MII_STR_REALTEK_RTL8201E },
 { MII_OUI_REALTEK, MII_MODEL_REALTEK_RTL8169S, MII_STR_REALTEK_RTL8169S },
 { MII_OUI_SEEQ, MII_MODEL_SEEQ_80220, MII_STR_SEEQ_80220 },
 { MII_OUI_SEEQ, MII_MODEL_SEEQ_84220, MII_STR_SEEQ_84220 },
 { MII_OUI_SEEQ, MII_MODEL_SEEQ_80225, MII_STR_SEEQ_80225 },
 { MII_OUI_SIS, MII_MODEL_SIS_900, MII_STR_SIS_900 },
 { MII_OUI_SMSC, MII_MODEL_SMSC_LAN83C185, MII_STR_SMSC_LAN83C185 },
 { MII_OUI_SMSC, MII_MODEL_SMSC_LAN8700, MII_STR_SMSC_LAN8700 },
 { MII_OUI_SMSC, MII_MODEL_SMSC_LAN911X, MII_STR_SMSC_LAN911X },
 { MII_OUI_SMSC, MII_MODEL_SMSC_LAN75XX, MII_STR_SMSC_LAN75XX },
 { MII_OUI_SMSC, MII_MODEL_SMSC_LAN8710_LAN8720, MII_STR_SMSC_LAN8710_LAN8720 },
 { MII_OUI_SMSC, MII_MODEL_SMSC_LAN8740, MII_STR_SMSC_LAN8740 },
 { MII_OUI_SMSC, MII_MODEL_SMSC_LAN8741A, MII_STR_SMSC_LAN8741A },
 { MII_OUI_SMSC, MII_MODEL_SMSC_LAN8742, MII_STR_SMSC_LAN8742 },
 { MII_OUI_TI, MII_MODEL_TI_TLAN10T, MII_STR_TI_TLAN10T },
 { MII_OUI_TI, MII_MODEL_TI_100VGPMI, MII_STR_TI_100VGPMI },
 { MII_OUI_TI, MII_MODEL_TI_TNETE2101, MII_STR_TI_TNETE2101 },
 { MII_OUI_xxTSC, MII_MODEL_xxTSC_78Q2120, MII_STR_xxTSC_78Q2120 },
 { MII_OUI_xxTSC, MII_MODEL_xxTSC_78Q2121, MII_STR_xxTSC_78Q2121 },
 { MII_OUI_VIA, MII_MODEL_VIA_VT6103, MII_STR_VIA_VT6103 },
 { MII_OUI_VIA, MII_MODEL_VIA_VT6103_2, MII_STR_VIA_VT6103_2 },
 { MII_OUI_xxVITESSE, MII_MODEL_xxVITESSE_VSC8601, MII_STR_xxVITESSE_VSC8601 },
 { MII_OUI_xxVITESSE, MII_MODEL_xxVITESSE_VSC8641, MII_STR_xxVITESSE_VSC8641 },
 { MII_OUI_xxVITESSE, MII_MODEL_xxVITESSE_VSC8501, MII_STR_xxVITESSE_VSC8501 },
 { MII_OUI_xxXAQTI, MII_MODEL_xxXAQTI_XMACII, MII_STR_xxXAQTI_XMACII },
 { 0, 0, NULL }
};
