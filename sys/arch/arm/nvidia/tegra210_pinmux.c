/* $NetBSD: tegra210_pinmux.c,v 1.1 2017/09/22 14:36:22 jmcneill Exp $ */

/*-
 * Copyright (c) 2017 Jared McNeill <jmcneill@invisible.ca>
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
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: tegra210_pinmux.c,v 1.1 2017/09/22 14:36:22 jmcneill Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/types.h>

#include <arm/nvidia/tegra_pinmux.h>

static const struct tegra_pinmux_pins tegra210_pins[] = {
	{ "sdmmc1_clk_pm0",		0x00, { "sdmmc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc1_cmd_pm1",		0x04, { "sdmmc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc1_dat3_pm2",		0x08, { "sdmmc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc1_dat2_pm3",		0x0c, { "sdmmc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc1_dat1_pm4",		0x10, { "sdmmc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc1_dat0_pm5",		0x14, { "sdmmc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc3_clk_pp0",		0x1c, { "sdmmc3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc3_cmd_pp1",		0x20, { "sdmmc3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc3_dat0_pp5",		0x24, { "sdmmc3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc3_dat1_pp4",		0x28, { "sdmmc3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc3_dat2_pp3",		0x2c, { "sdmmc3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sdmmc3_dat3_pp2",		0x30, { "sdmmc3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pex_l0_rst_n_pa0",		0x38, { "pe0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pex_l0_clkreq_n_pa1",	0x3c, { "pe0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pex_wake_n_pa2",		0x40, { "pe", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pex_l1_rst_n_pa3",		0x44, { "pe1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pex_l1_clkreq_n_pa4",	0x48, { "pe1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "sata_led_active_pa5",	0x4c, { "sata", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi1_mosi_pc0",		0x50, { "spi1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi1_miso_pc1",		0x54, { "spi1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi1_sck_pc2",		0x58, { "spi1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi1_cs0_pc3",		0x5c, { "spi1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi1_cs1_pc4",		0x60, { "spi1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi2_mosi_pb4",		0x64, { "spi2", "dtv", "rsvd2", "rsvd3" } },
	{ "spi2_miso_pb5",		0x68, { "spi2", "dtv", "rsvd2", "rsvd3" } },
	{ "spi2_sck_pb6",		0x6c, { "spi2", "dtv", "rsvd2", "rsvd3" } },
	{ "spi2_cs0_pb7",		0x70, { "spi2", "dtv", "rsvd2", "rsvd3" } },
	{ "spi2_cs1_pdd0",		0x74, { "spi2", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi4_mosi_pc7",		0x78, { "spi4", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi4_miso_pd0",		0x7c, { "spi4", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi4_sck_pc5",		0x80, { "spi4", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spi4_cs0_pc6",		0x84, { "spi4", "rsvd1", "rsvd2", "rsvd3" } },
	{ "qspi_sck_pee0",		0x88, { "qspi", "rsvd1", "rsvd2", "rsvd3" } },
	{ "qspi_cs_n_pee1",		0x8c, { "qspi", "rsvd1", "rsvd2", "rsvd3" } },
	{ "qspi_io0_pee2",		0x90, { "qspi", "rsvd1", "rsvd2", "rsvd3" } },
	{ "qspi_io1_pee3",		0x94, { "qspi", "rsvd1", "rsvd2", "rsvd3" } },
	{ "qspi_io2_pee4",		0x98, { "qspi", "rsvd1", "rsvd2", "rsvd3" } },
	{ "qspi_io3_pee5",		0x9c, { "qspi", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dmic1_clk_pe0",		0xa4, { "dmic1", "i2s3", "rsvd2", "rsvd3" } },
	{ "dmic1_dat_pe1",		0xa8, { "dmic1", "i2s3", "rsvd2", "rsvd3" } },
	{ "dmic2_clk_pe2",		0xac, { "dmic2", "i2s3", "rsvd2", "rsvd3" } },
	{ "dmic2_dat_pe3",		0xb0, { "dmic2", "i2s3", "rsvd2", "rsvd3" } },
	{ "dmic3_clk_pe4",		0xb4, { "dmic3", "i2s5a", "rsvd2", "rsvd3" } },
	{ "dmic3_dat_pe5",		0xb8, { "dmic3", "i2s5a", "rsvd2", "rsvd3" } },
	{ "gen1_i2c_scl_pj1",		0xbc, { "i2c1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "gen1_i2c_sda_pj0",		0xc0, { "i2c1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "gen2_i2c_scl_pj2",		0xc4, { "i2c2", "rsvd1", "rsvd2", "rsvd3" } },
	{ "gen2_i2c_sda_pj3",		0xc8, { "i2c2", "rsvd1", "rsvd2", "rsvd3" } },
	{ "gen3_i2c_scl_pf0",		0xcc, { "i2c3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "gen3_i2c_sda_pf1",		0xd0, { "i2c3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "cam_i2c_scl_ps2",		0xd4, { "i2c3", "i2cvi", "rsvd2", "rsvd3" } },
	{ "cam_i2c_sda_ps3",		0xd8, { "i2c3", "i2cvi", "rsvd2", "rsvd3" } },
	{ "pwr_i2c_scl_py3",		0xdc, { "i2cpmu", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pwr_i2c_sda_py4",		0xe0, { "i2cpmu", "rsvd1", "rsvd2", "rsvd3" } },
	{ "uart1_tx_pu0",		0xe4, { "uarta", "rsvd1", "rsvd2", "rsvd3" } },
	{ "uart1_rx_pu1",		0xe8, { "uarta", "rsvd1", "rsvd2", "rsvd3" } },
	{ "uart1_rts_pu2",		0xec, { "uarta", "rsvd1", "rsvd2", "rsvd3" } },
	{ "uart1_cts_pu3",		0xf0, { "uarta", "rsvd1", "rsvd2", "rsvd3" } },
	{ "uart2_tx_pg0",		0xf4, { "uartb", "i2s4a", "spdif", "uart" } },
	{ "uart2_rx_pg1",		0xf8, { "uartb", "i2s4a", "spdif", "uart" } },
	{ "uart2_rts_pg2",		0xfc, { "uartb", "i2s4a", "rsvd2", "uart" } },
	{ "uart2_cts_pg3",		0x100, { "uartb", "i2s4a", "rsvd2", "uart" } },
	{ "uart3_tx_pd1",		0x104, { "uartc", "spi4", "rsvd2", "rsvd3" } },
	{ "uart3_rx_pd2",		0x108, { "uartc", "spi4", "rsvd2", "rsvd3" } },
	{ "uart3_rts_pd3",		0x10c, { "uartc", "spi4", "rsvd2", "rsvd3" } },
	{ "uart3_cts_pd4",		0x110, { "uartc", "spi4", "rsvd2", "rsvd3" } },
	{ "uart4_tx_pi4",		0x114, { "uartd", "uart", "rsvd2", "rsvd3" } },
	{ "uart4_rx_pi5",		0x118, { "uartd", "uart", "rsvd2", "rsvd3" } },
	{ "uart4_rts_pi6",		0x11c, { "uartd", "uart", "rsvd2", "rsvd3" } },
	{ "uart4_cts_pi7",		0x120, { "uartd", "uart", "rsvd2", "rsvd3" } },
	{ "dap1_fs_pb0",		0x124, { "i2s1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap1_din_pb1",		0x128, { "i2s1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap1_dout_pb2",		0x12c, { "i2s1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap1_sclk_pb3",		0x130, { "i2s1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap2_fs_paa0",		0x134, { "i2s2", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap2_din_paa2",		0x138, { "i2s2", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap2_dout_paa3",		0x13c, { "i2s2", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap2_sclk_paa1",		0x140, { "i2s2", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap4_fs_pj4",		0x144, { "i2s4b", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap4_din_pj5",		0x148, { "i2s4b", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap4_dout_pj6",		0x14c, { "i2s4b", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dap4_sclk_pj7",		0x150, { "i2s4b", "rsvd1", "rsvd2", "rsvd3" } },
	{ "cam1_mclk_ps0",		0x154, { "extperiph3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "cam2_mclk_ps1",		0x158, { "extperiph3", "rsvd1", "rsvd2", "rsvd3" } },
	{ "jtag_rtck",			0x15c, { "jtag", "rsvd1", "rsvd2", "rsvd3" } },
	{ "clk_32k_in",			0x160, { } },
	{ "clk_32k_out_py5",		0x164, { "soc", "blink", "rsvd2", "rsvd3" } },
	{ "batt_bcl",			0x168, { "bcl", "rsvd1", "rsvd2", "rsvd3" } },
	{ "clk_req",			0x16c, { } },
	{ "cpu_pwr_req",		0x170, { } },
	{ "pwr_int_n",			0x174, { } },
	{ "shutdown",			0x178, { } },
	{ "core_pwr_req",		0x17c, { } },
	{ "aud_mclk_pbb0",		0x180, { "aud", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dvfs_pwm_pbb1",		0x184, { "rsvd0", "cldvfs", "spi3", "rsvd3" } },
	{ "dvfs_clk_pbb2",		0x188, { "rsvd0", "cldvfs", "spi3", "rsvd3" } },
	{ "gpio_x1_aud_pbb3",		0x18c, { "rsvd0", "rsvd1", "spi3", "rsvd3" } },
	{ "gpio_x3_aud_pbb4",		0x190, { "rsvd0", "rsvd1", "spi3", "rsvd3" } },
	{ "pcc7",			0x194, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "hdmi_cec_pcc0",		0x198, { "cec", "rsvd1", "rsvd2", "rsvd3" } },
	{ "hdmi_int_dp_hpd_pcc1",	0x19c, { "dp", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spdif_out_pcc2",		0x1a0, { "spdif", "rsvd1", "rsvd2", "rsvd3" } },
	{ "spdif_in_pcc3",		0x1a4, { "spdif", "rsvd1", "rsvd2", "rsvd3" } },
	{ "usb_vbus_en0_pcc4",		0x1a8, { "usb", "rsvd1", "rsvd2", "rsvd3" } },
	{ "usb_vbus_en1_pcc5",		0x1ac, { "usb", "rsvd1", "rsvd2", "rsvd3" } },
	{ "dp_hpd0_pcc6",		0x1b0, { "dp", "rsvd1", "rsvd2", "rsvd3" } },
	{ "wifi_en_ph0",		0x1b4, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "wifi_rst_ph1",		0x1b8, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "wifi_wake_ap_ph2",		0x1bc, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "ap_wake_bt_ph3",		0x1c0, { "rsvd0", "uartb", "spdif", "rsvd3" } },
	{ "bt_rst_ph4",			0x1c4, { "rsvd0", "uartb", "spdif", "rsvd3" } },
	{ "bt_wake_ap_ph5",		0x1c8, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "ap_wake_nfc_ph7",		0x1cc, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "nfc_en_pi0",			0x1d0, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "nfc_int_pi1",		0x1d4, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "gps_en_pi2",			0x1d8, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "gps_rst_pi3",		0x1dc, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "cam_rst_ps4",		0x1e0, { "vgp1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "cam_af_en_ps5",		0x1e4, { "vimclk", "vgp2", "rsvd2", "rsvd3" } },
	{ "cam_flash_en_ps6",		0x1e8, { "vimclk", "vgp3", "rsvd2", "rsvd3" } },
	{ "cam1_pwdn_ps7",		0x1ec, { "vgp4", "rsvd1", "rsvd2", "rsvd3" } },
	{ "cam2_pwdn_pt0",		0x1f0, { "vgp5", "rsvd1", "rsvd2", "rsvd3" } },
	{ "cam1_strobe_pt1",		0x1f4, { "vgp6", "rsvd1", "rsvd2", "rsvd3" } },
	{ "lcd_te_py2",			0x1f8, { "displaya", "rsvd1", "rsvd2", "rsvd3" } },
	{ "lcd_bl_pwm_pv0",		0x1fc, { "displaya", "pwm0", "sor0", "rsvd3" } },
	{ "lcd_bl_en_pv1",		0x200, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "lcd_rst_pv2",		0x204, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "lcd_gpio1_pv3",		0x208, { "displayb", "rsvd1", "rsvd2", "rsvd3" } },
	{ "lcd_gpio2_pv4",		0x20c, { "displayb", "pwm1", "rsvd2", "sor1" } },
	{ "ap_ready_pv5",		0x210, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "touch_rst_pv6",		0x214, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "touch_clk_pv7",		0x218, { "touch", "rsvd1", "rsvd2", "rsvd3" } },
	{ "modem_wake_ap_px0",		0x21c, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "touch_int_px1",		0x220, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "motion_int_px2",		0x224, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "als_prox_int_px3",		0x228, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "temp_alert_px4",		0x22c, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "button_power_on_px5",	0x230, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "button_vol_up_px6",		0x234, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "button_vol_down_px7",	0x238, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "button_slide_sw_py0",	0x23c, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "button_home_py1",		0x240, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pa6",			0x244, { "sata", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pe6",			0x248, { "rsvd0", "i2s5a", "pwm2", "rsvd3" } },
	{ "pe7",			0x24c, { "rsvd0", "i2s5a", "pwm3", "rsvd3" } },
	{ "ph6",			0x250, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pk0",			0x254, { "iqc0", "i2s5b", "rsvd2", "rsvd3" } },
	{ "pk1",			0x258, { "iqc0", "i2s5b", "rsvd2", "rsvd3" } },
	{ "pk2",			0x25c, { "iqc0", "i2s5b", "rsvd2", "rsvd3" } },
	{ "pk3",			0x260, { "iqc0", "i2s5b", "rsvd2", "rsvd3" } },
	{ "pk4",			0x264, { "iqc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pk5",			0x268, { "iqc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pk6",			0x26c, { "iqc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pk7",			0x270, { "iqc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pl0",			0x274, { "rsvd0", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pl1",			0x278, { "soc", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pz0",			0x27c, { "vimclk2", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pz1",			0x280, { "vimclk2", "sdmmc1", "rsvd2", "rsvd3" } },
	{ "pz2",			0x284, { "sdmmc3", "ccla", "rsvd2", "rsvd3" } },
	{ "pz3",			0x288, { "sdmmc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pz4",			0x28c, { "sdmmc1", "rsvd1", "rsvd2", "rsvd3" } },
	{ "pz5",			0x290, { "soc", "rsvd1", "rsvd2", "rsvd3" } },
};

const struct tegra_pinmux_conf tegra210_pinmux_conf = {
	.npins = __arraycount(tegra210_pins),
	.pins = tegra210_pins,
};
