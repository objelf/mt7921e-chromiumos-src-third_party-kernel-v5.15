// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2021 MediaTek Inc.
 */

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/seq_file.h>

#include "clkdbg.h"

#define DUMP_INIT_STATE		0

enum {
	mcusys,
	topckgen,
	infracfg_ao,
	apmixedsys,
	imp_iic_wrap,
	audsys,
	mfgcfg,
	mmsys,
	wpesys,
	imgsys1,
	imgsys2,
	vdecsys,
	vencsys,
	camsys,
	camsys_rawa,
	camsys_rawb,
	mdpsys,
	ipesys,
};

#define REGBASE_V(_phys, _id_name) { .phys = _phys, .name = #_id_name }

static struct regbase rb[] = {
	[mcusys] = REGBASE_V(0x0C53A000, mcusys), /* page limit (4K), so add 0xA000 offset */
	[topckgen] = REGBASE_V(0x10000000, topckgen),
	[infracfg_ao] = REGBASE_V(0x10001000, infracfg_ao),
	[apmixedsys] = REGBASE_V(0x1000C000, apmixedsys),
	[imp_iic_wrap] = REGBASE_V(0x11017000, imp_iic_wrap),
	[audsys] = REGBASE_V(0x11210000, audsys),
	[mfgcfg] = REGBASE_V(0x13000000, mfgcfg),
	[mmsys] = REGBASE_V(0x14000000, mmsys),
	[wpesys] = REGBASE_V(0x14020000, wpesys),
	[imgsys1] = REGBASE_V(0x15020000, imgsys1),
	[imgsys2] = REGBASE_V(0x15820000, imgsys2),
	[vdecsys] = REGBASE_V(0x1602f000, vdecsys),
	[vencsys] = REGBASE_V(0x17000000, vencsys),
	[camsys] = REGBASE_V(0x1a000000, camsys),
	[camsys_rawa] = REGBASE_V(0x1a04f000, camsys_rawa),
	[camsys_rawb] = REGBASE_V(0x1a06f000, camsys_rawb),
	[mdpsys] = REGBASE_V(0x1b000000, mdpsys),
	[ipesys] = REGBASE_V(0x1c000000, ipesys),
};

#define REGNAME(_base, _ofs, _name)     \
	{ .base = &rb[_base], .ofs = _ofs, .name = #_name }

static struct regname rn[] = {
	REGNAME(apmixedsys, 0x204, ARMPLL_LL_CON0),
	REGNAME(apmixedsys, 0x208, ARMPLL_LL_CON1),
	REGNAME(apmixedsys, 0x20c, ARMPLL_LL_CON2),
	REGNAME(apmixedsys, 0x210, ARMPLL_LL_CON3),
	REGNAME(apmixedsys, 0x214, ARMPLL_BL_CON0),
	REGNAME(apmixedsys, 0x218, ARMPLL_BL_CON1),
	REGNAME(apmixedsys, 0x21c, ARMPLL_BL_CON2),
	REGNAME(apmixedsys, 0x220, ARMPLL_BL_CON3),
	REGNAME(apmixedsys, 0x224, CCIPLL_CON0),
	REGNAME(apmixedsys, 0x228, CCIPLL_CON1),
	REGNAME(apmixedsys, 0x22c, CCIPLL_CON2),
	REGNAME(apmixedsys, 0x230, CCIPLL_CON3),
	REGNAME(apmixedsys, 0x244, MAINPLL_CON0),
	REGNAME(apmixedsys, 0x248, MAINPLL_CON1),
	REGNAME(apmixedsys, 0x24c, MAINPLL_CON2),
	REGNAME(apmixedsys, 0x250, MAINPLL_CON3),
	REGNAME(apmixedsys, 0x254, MMPLL_CON0),
	REGNAME(apmixedsys, 0x258, MMPLL_CON1),
	REGNAME(apmixedsys, 0x25c, MMPLL_CON2),
	REGNAME(apmixedsys, 0x260, MMPLL_CON3),
	REGNAME(apmixedsys, 0x264, TVDPLL_CON0),
	REGNAME(apmixedsys, 0x268, TVDPLL_CON1),
	REGNAME(apmixedsys, 0x26c, TVDPLL_CON2),
	REGNAME(apmixedsys, 0x270, TVDPLL_CON3),
	REGNAME(apmixedsys, 0x274, MPLL_CON0),
	REGNAME(apmixedsys, 0x278, MPLL_CON1),
	REGNAME(apmixedsys, 0x27c, MPLL_CON2),
	REGNAME(apmixedsys, 0x280, MPLL_CON3),
	REGNAME(apmixedsys, 0x304, ADSPPLL_CON0),
	REGNAME(apmixedsys, 0x308, ADSPPLL_CON1),
	REGNAME(apmixedsys, 0x30c, ADSPPLL_CON2),
	REGNAME(apmixedsys, 0x310, ADSPPLL_CON3),
	REGNAME(apmixedsys, 0x314, MFGPLL_CON0),
	REGNAME(apmixedsys, 0x318, MFGPLL_CON1),
	REGNAME(apmixedsys, 0x31c, MFGPLL_CON2),
	REGNAME(apmixedsys, 0x320, MFGPLL_CON3),
	REGNAME(apmixedsys, 0x324, UNIVPLL_CON0),
	REGNAME(apmixedsys, 0x328, UNIVPLL_CON1),
	REGNAME(apmixedsys, 0x32c, UNIVPLL_CON2),
	REGNAME(apmixedsys, 0x330, UNIVPLL_CON3),
	REGNAME(apmixedsys, 0x334, APLL1_CON0),
	REGNAME(apmixedsys, 0x338, APLL1_CON1),
	REGNAME(apmixedsys, 0x33c, APLL1_CON2),
	REGNAME(apmixedsys, 0x340, APLL1_CON3),
	REGNAME(apmixedsys, 0x344, APLL1_CON4),
	REGNAME(apmixedsys, 0x348, APLL2_CON0),
	REGNAME(apmixedsys, 0x34c, APLL2_CON1),
	REGNAME(apmixedsys, 0x350, APLL2_CON2),
	REGNAME(apmixedsys, 0x354, APLL2_CON3),
	REGNAME(apmixedsys, 0x358, APLL2_CON4),
	REGNAME(apmixedsys, 0x35c, NNAPLL_CON0),
	REGNAME(apmixedsys, 0x360, NNAPLL_CON1),
	REGNAME(apmixedsys, 0x364, NNAPLL_CON2),
	REGNAME(apmixedsys, 0x368, NNAPLL_CON3),
	REGNAME(apmixedsys, 0x36c, NNA2PLL_CON0),
	REGNAME(apmixedsys, 0x370, NNA2PLL_CON1),
	REGNAME(apmixedsys, 0x374, NNA2PLL_CON2),
	REGNAME(apmixedsys, 0x378, NNA2PLL_CON3),
	REGNAME(apmixedsys, 0x38c, MSDCPLL_CON0),
	REGNAME(apmixedsys, 0x390, MSDCPLL_CON1),
	REGNAME(apmixedsys, 0x394, MSDCPLL_CON2),
	REGNAME(apmixedsys, 0x398, MSDCPLL_CON3),
	REGNAME(mcusys, 0x02A0, CPU_PLLDIV_CFG0), /* page limit (4K), so minus 0xA000 offset */
	REGNAME(mcusys, 0x02A4, CPU_PLLDIV_CFG1), /* page limit (4K), so minus 0xA000 offset */
	REGNAME(mcusys, 0x02E0, BUS_PLLDIV_CFG),  /* page limit (4K), so miuns 0xA000 offset */
	REGNAME(topckgen, 0x0040, CLK_CFG_0),
	REGNAME(topckgen, 0x0050, CLK_CFG_1),
	REGNAME(topckgen, 0x0060, CLK_CFG_2),
	REGNAME(topckgen, 0x0070, CLK_CFG_3),
	REGNAME(topckgen, 0x0080, CLK_CFG_4),
	REGNAME(topckgen, 0x0090, CLK_CFG_5),
	REGNAME(topckgen, 0x00a0, CLK_CFG_6),
	REGNAME(topckgen, 0x00b0, CLK_CFG_7),
	REGNAME(topckgen, 0x00c0, CLK_CFG_8),
	REGNAME(topckgen, 0x00d0, CLK_CFG_9),
	REGNAME(topckgen, 0x00e0, CLK_CFG_10),
	REGNAME(topckgen, 0x00ec, CLK_CFG_11),
	REGNAME(topckgen, 0x0100, CLK_CFG_12),
	REGNAME(topckgen, 0x0110, CLK_CFG_13),
	REGNAME(topckgen, 0x0120, CLK_CFG_14),
	REGNAME(topckgen, 0x0180, CLK_CFG_15),
	REGNAME(topckgen, 0x0320, CLK_AUDDIV_0),
	REGNAME(topckgen, 0x0328, CLK_AUDDIV_2),
	REGNAME(topckgen, 0x0334, CLK_AUDDIV_3),
	REGNAME(infracfg_ao, 0x90, MODULE_SW_CG_0),
	REGNAME(infracfg_ao, 0x94, MODULE_SW_CG_1),
	REGNAME(infracfg_ao, 0xac, MODULE_SW_CG_2),
	REGNAME(infracfg_ao, 0xc8, MODULE_SW_CG_3),
	REGNAME(imp_iic_wrap, 0xe00, AP_CLOCK_CG),
	REGNAME(audsys, 0x0, AUDIO_TOP_0),
	REGNAME(audsys, 0x4, AUDIO_TOP_1),
	REGNAME(audsys, 0x8, AUDIO_TOP_2),
	REGNAME(mfgcfg, 0x0, MFG_CG),
	REGNAME(mmsys, 0x100, MMSYS_CG_0),
	REGNAME(mmsys, 0x1a0, MMSYS_CG_2),
	REGNAME(wpesys, 0x0, WPESYS_RG),
	REGNAME(imgsys1, 0x0, IMG_CG),
	REGNAME(imgsys2, 0x0, IMG_CG),
	REGNAME(vdecsys, 0x8, LARB_CKEN_CON),
	REGNAME(vdecsys, 0x200, LAT_CKEN),
	REGNAME(vdecsys, 0x190, MINI_MDP_CFG_0),
	REGNAME(vdecsys, 0x0, VDEC_CKEN),
	REGNAME(vencsys, 0x0, VENCSYS_CG),
	REGNAME(camsys, 0x0, CAMSYS_CG),
	REGNAME(camsys_rawa, 0x0, CAMSYS_CG),
	REGNAME(camsys_rawb, 0x0, CAMSYS_CG),
	REGNAME(mdpsys, 0x100, MDPSYS_CG_0),
	REGNAME(mdpsys, 0x110, MDPSYS_CG_1),
	REGNAME(mdpsys, 0x120, MDPSYS_CG_2),
	REGNAME(ipesys, 0x0, IMG_CG),
	{}
};

static const struct regname *get_all_regnames(void)
{
	return rn;
}

static void __init init_regbase(void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(rb); i++)
		rb[i].virt = ioremap(rb[i].phys, PAGE_SIZE);
}

/*
 * clkdbg fmeter
 */


#define clk_readl(addr)		readl(addr)
#define clk_writel(addr, val)   \
	do { writel(val, addr); wmb(); } while (0) /* sync write */

#define FMCLK(_t, _i, _n) { .type = _t, .id = _i, .name = _n }

static const struct fmeter_clk fclks[] = {
	FMCLK(CKGEN, 1, "hd_faxi_ck"),
	FMCLK(CKGEN, 2, "hf_fscp_ck"),
	FMCLK(CKGEN, 3, "hf_fmfg_ck"),
	FMCLK(CKGEN, 4, "f_fcamtg_ck"),
	FMCLK(CKGEN, 5, "f_fcamtg1_ck"),
	FMCLK(CKGEN, 6, "f_fcamtg2_ck"),
	FMCLK(CKGEN, 7, "f_fcamtg3_ck"),
	FMCLK(CKGEN, 8, "f_fcamtg4_ck"),
	FMCLK(CKGEN, 9, "f_fcamtg5_ck"),
	FMCLK(CKGEN, 10, "f_fcamtg6_ck"),
	FMCLK(CKGEN, 11, "f_fuart_ck"),
	FMCLK(CKGEN, 12, "hf_fspi_ck"),
	FMCLK(CKGEN, 13, "hf_fmsdc50_0_hclk_ck"),
	FMCLK(CKGEN, 14, "hf_fmsdc50_0_ck"),
	FMCLK(CKGEN, 15, "hf_fmsdc30_1_ck"),
	FMCLK(CKGEN, 16, "hf_faudio_ck"),
	FMCLK(CKGEN, 17, "hf_faud_intbus_ck"),
	FMCLK(CKGEN, 18, "hf_faud_1_ck"),
	FMCLK(CKGEN, 19, "hf_faud_2_ck"),
	FMCLK(CKGEN, 20, "hf_faud_engen1_ck"),
	FMCLK(CKGEN, 21, "hf_faud_engen2_ck"),
	FMCLK(CKGEN, 22, "f_fdisp_pwm_ck"),
	FMCLK(CKGEN, 23, "hf_fsspm_ck"),
	FMCLK(CKGEN, 24, "hf_fdxcc_ck"),
	FMCLK(CKGEN, 25, "hf_fusb_top_ck"),
	FMCLK(CKGEN, 26, "hf_fsrck_ck"),
	FMCLK(CKGEN, 27, "hf_fspm_ck"),
	FMCLK(CKGEN, 28, "hf_fi2c_ck"),
	FMCLK(CKGEN, 29, "f_fpwm_ck"),
	FMCLK(CKGEN, 30, "f_fseninf_ck"),
	FMCLK(CKGEN, 31, "f_fseninf1_ck"),
	FMCLK(CKGEN, 32, "f_fseninf2_ck"),
	FMCLK(CKGEN, 33, "f_fseninf3_ck"),
	FMCLK(CKGEN, 34, "hdf_faes_msdcfde_ck"),
	FMCLK(CKGEN, 35, "f_fpwrap_ulposc_ck"),
	FMCLK(CKGEN, 36, "f_fcamtm_ck"),
	FMCLK(CKGEN, 37, "hf_fvenc_ck"),
	FMCLK(CKGEN, 38, "hf_fcam_ck"),
	FMCLK(CKGEN, 39, "hf_fimg1_ck"),
	FMCLK(CKGEN, 40, "hf_fipe_ck"),
	FMCLK(CKGEN, 41, "hf_dpmaif_ck"),
	FMCLK(CKGEN, 42, "hf_fvdec_ck"),
	FMCLK(CKGEN, 43, "hf_fdisp_ck"),
	FMCLK(CKGEN, 44, "hf_fmdp_ck"),
	FMCLK(CKGEN, 45, "hf_faudio_h_ck"),
	FMCLK(CKGEN, 46, "hf_fufs_ck"),
	FMCLK(CKGEN, 47, "hf_faes_fde_ck"),
	FMCLK(CKGEN, 48, "hf_audiodsp_ck"),
	FMCLK(CKGEN, 49, "hg_fdvfsrc_ck"),
	FMCLK(CKGEN, 50, "hf_dsi_occ_ck"),
	FMCLK(CKGEN, 51, "hf_fspmi_mst_ck"),
	FMCLK(CKGEN, 52, "hf_spinor_ck"),
	FMCLK(CKGEN, 53, "hf_fnna_ck"),
	FMCLK(CKGEN, 54, "hf_fnna1_ck"),
	FMCLK(CKGEN, 55, "hf_fnna2_ck"),
	FMCLK(CKGEN, 56, "f_fssusb_xhci_ck"),
	FMCLK(CKGEN, 57, "f_fssusb_top_1p_ck"),
	FMCLK(CKGEN, 58, "f_fssusb_xhci_1p_ck"),
	FMCLK(CKGEN, 59, "hf_fwpe_ck"),
	FMCLK(CKGEN, 60, "hf_fdpi_ck"),
	FMCLK(CKGEN, 61, "u3_occ_250m_ck"),
	FMCLK(CKGEN, 62, "u3_occ_500m_ck"),
	FMCLK(CKGEN, 63, "hf_fadsp_bus_ck"),
	FMCLK(ABIST, 1, "ad_usb20_clk480m_ip1"),
	FMCLK(ABIST, 7, "AD_CCIPLL_CK"),
	FMCLK(ABIST, 8, "AD_ARMPLL_BL_CK"),
	FMCLK(ABIST, 9, "AD_ARMPLL_LL_CK"),
	FMCLK(ABIST, 10, "AD_PLLGP1_TST_CK"),
	FMCLK(ABIST, 11, "AD_MDBPIPLL_CK"),
	FMCLK(ABIST, 12, "AD_MDBRPPLL_CK"),
	FMCLK(ABIST, 13, "AD_NNAPLL2_CK"),
	FMCLK(ABIST, 14, "AD_NNAPLL_CK"),
	FMCLK(ABIST, 15, "AD_APLL2_CK"),
	FMCLK(ABIST, 16, "AD_APLL1_CK"),
	FMCLK(ABIST, 17, "AD_USB20_192M_CK"),
	FMCLK(ABIST, 19, "AD_TVDPLL_CK"),
	FMCLK(ABIST, 20, "AD_DSI0_LNTC_DSICLK"),
	FMCLK(ABIST, 21, "AD_DSI0_MPPLL_TST_CK"),
	FMCLK(ABIST, 24, "AD_MAINPLL_CK"),
	FMCLK(ABIST, 25, "AD_MDPLL1_FS26M_CK"),
	FMCLK(ABIST, 26, "AD_MFGPLL_CK"),
	FMCLK(ABIST, 27, "AD_MMPLL_CK"),
	FMCLK(ABIST, 28, "AD_ADSPPLL_CK"),
	FMCLK(ABIST, 29, "AD_MPLL_208M_CK"),
	FMCLK(ABIST, 30, "AD_MSDCPLL_CK"),
	FMCLK(ABIST, 34, "AD_ULPOSC2_CK"),
	FMCLK(ABIST, 35, "AD_ULPOSC1_CK"),
	FMCLK(ABIST, 36, "AD_UNIVPLL_CK"),
	FMCLK(ABIST, 40, "ad_wbg_dig_bpll_ck"),
	FMCLK(ABIST, 42, "AD_RCLRPLL_DIV4_CK"),
	FMCLK(ABIST, 43, "AD_RPHYPLL_DIV4_CK"),
	FMCLK(ABIST, 44, "fmem_ck_aft_dcm_ch0"),
	FMCLK(ABIST, 45, "fmem_ck_aft_dcm_ch1"),
	FMCLK(ABIST, 46, "fmem_ck_bfe_dcm_ch0"),
	FMCLK(ABIST, 47, "fmem_ck_bfe_dcm_ch1"),
	FMCLK(ABIST, 48, "mcusys_arm_clk_out_all"),
	FMCLK(ABIST, 49, "AD_CSI0A_CDPHY_DELAYCAL_CK"),
	FMCLK(ABIST, 50, "AD_CSI0B_CDPHY_DELAYCAL_CK"),
	FMCLK(ABIST, 51, "AD_CSI1A_CDPHY_DELAYCAL_CK"),
	FMCLK(ABIST, 52, "AD_CSI1B_CDPHY_DELAYCAL_CK"),
	FMCLK(ABIST, 58, "rtc32k_ck_i_vao"),
	FMCLK(ABIST, 60, "ckmon1_ck"),
	FMCLK(ABIST, 61, "ckmon2_ck"),
	FMCLK(ABIST, 62, "ckmon3_ck"),
	FMCLK(ABIST, 63, "ckmon4_ck"),
	{},
};

#define CLK_MISC_CFG_0	(rb[topckgen].virt + 0x140)
#define CLK_DBG_CFG	(rb[topckgen].virt + 0x17C)
#define CLK26CALI_0	(rb[topckgen].virt + 0x220)
#define CLK26CALI_1	(rb[topckgen].virt + 0x224)

static unsigned int mt_get_abist_freq(unsigned int ID)
{
	int output = 0, i = 0;
	unsigned int temp, clk26cali_0, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

	clk_dbg_cfg = clk_readl(CLK_DBG_CFG);
	clk_writel(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFC0FFFC)|(ID << 16));
	clk_misc_cfg_0 = clk_readl(CLK_MISC_CFG_0);
	clk_writel(CLK_MISC_CFG_0, ((clk_misc_cfg_0 & 0x00FFFFFF) | 0x01000000));
	clk26cali_0 = clk_readl(CLK26CALI_0);
	clk26cali_1 = clk_readl(CLK26CALI_1);
	clk_writel(CLK26CALI_0, 0x1000);
	clk_writel(CLK26CALI_0, 0x1010);

	/* wait frequency meter finish */
	while (clk_readl(CLK26CALI_0) & 0x10) {
		mdelay(10);
		i++;
		if (i > 10) {
			pr_err("abist[%d] timtout\n", ID);
			output = 0;
			goto absit_timeout;
		}
	}

	temp = clk_readl(CLK26CALI_1) & 0xFFFF;

	output = ((temp * 26000)) / 1024; // Khz
	output *= 2;

absit_timeout:

	clk_writel(CLK_DBG_CFG, clk_dbg_cfg);
	clk_writel(CLK_MISC_CFG_0, clk_misc_cfg_0);
	clk_writel(CLK26CALI_0, clk26cali_0);
	clk_writel(CLK26CALI_1, clk26cali_1);

	//pr_info("abist meter[%d] = %d Khz\n", ID, output);
	return output;
}

static unsigned int mt_get_ckgen_freq(unsigned int ID)
{
	int output = 0, i = 0;
	unsigned int temp, clk26cali_0, clk_dbg_cfg, clk_misc_cfg_0, clk26cali_1;

	clk_dbg_cfg = clk_readl(CLK_DBG_CFG);
	clk_writel(CLK_DBG_CFG, (clk_dbg_cfg & 0xFFFFC0FC)|(ID << 8)|(0x1));

	clk_misc_cfg_0 = clk_readl(CLK_MISC_CFG_0);
	clk_writel(CLK_MISC_CFG_0, ((clk_misc_cfg_0 & 0x00FFFFFF) | 0x01000000));
	clk26cali_0 = clk_readl(CLK26CALI_0);
	clk26cali_1 = clk_readl(CLK26CALI_1);
	clk_writel(CLK26CALI_0, 0x1000);
	clk_writel(CLK26CALI_0, 0x1010);

	/* wait frequency meter finish */
	while (clk_readl(CLK26CALI_0) & 0x10) {
		mdelay(10);
		i++;
		if (i > 10) {
			pr_err("ckgen[%d] timtout\n", ID);
			output = 0;
			goto ckgen_timeout;
		}
	}

	temp = clk_readl(CLK26CALI_1) & 0xFFFF;

	output = ((temp * 26000)) / 1024; // Khz
	output *= 2;

ckgen_timeout:

	clk_writel(CLK_DBG_CFG, clk_dbg_cfg);
	clk_writel(CLK_MISC_CFG_0, clk_misc_cfg_0);
	clk_writel(CLK26CALI_0, clk26cali_0);
	clk_writel(CLK26CALI_1, clk26cali_1);

	//pr_info("ckgen meter[%d] = %d Khz\n", ID, output);
	return output;
}

static u32 fmeter_freq_op(const struct fmeter_clk *fclk)
{
	if (fclk->type == ABIST)
		return mt_get_abist_freq(fclk->id);
	else if (fclk->type == CKGEN)
		return mt_get_ckgen_freq(fclk->id);
	return 0;
}

static const struct fmeter_clk *get_all_fmeter_clks(void)
{
	return fclks;
}

/*
 * clkdbg dump_state
 */

static const char * const *get_all_clk_names(void)
{
	static const char * const clks[] = {
		"armpll_ll",
		"armpll_bl",
		"ccipll",
		"mainpll",
		"univ2pll",
		"msdcpll",
		"mmpll",
		"nnapll",
		"nna2pll",
		"adsppll",
		"mfgpll",
		"tvdpll",
		"apll1",
		"apll2",
		"mcu_armpll_ll_sel",
		"mcu_armpll_bl_sel",
		"mcu_armpll_bus_sel",
		"top_axi",
		"top_scp",
		"top_mfg",
		"top_camtg",
		"top_camtg1",
		"top_camtg2",
		"top_camtg3",
		"top_camtg4",
		"top_camtg5",
		"top_camtg6",
		"top_uart",
		"top_spi",
		"top_msdc5hclk",
		"top_msdc50_0",
		"top_msdc30_1",
		"top_audio",
		"top_aud_intbus",
		"top_aud_1",
		"top_aud_2",
		"top_aud_engen1",
		"top_aud_engen2",
		"top_disp_pwm",
		"top_sspm",
		"top_dxcc",
		"top_usb",
		"top_srck",
		"top_spm",
		"top_i2c",
		"top_pwm",
		"top_seninf",
		"top_seninf1",
		"top_seninf2",
		"top_seninf3",
		"top_aes_msdcfde",
		"top_pwrap_ulposc",
		"top_camtm",
		"top_venc",
		"top_cam",
		"top_img1",
		"top_ipe",
		"top_dpmaif",
		"top_vdec",
		"top_disp",
		"top_mdp",
		"top_audio_h",
		"top_ufs",
		"top_aes_fde",
		"top_audiodsp",
		"top_dsi_occ",
		"top_spmi_mst",
		"top_spinor",
		"top_nna",
		"top_nna1",
		"top_nna2",
		"top_ssusb_xhci",
		"top_ssusb_1p",
		"top_ssusb_xhci_1p",
		"top_wpe",
		"top_dpi",
		"top_u3_occ_250m",
		"top_u3_occ_500m",
		"top_adsp_bus",
		"apll_i2s0_mck_sel",
		"apll_i2s1_mck_sel",
		"apll_i2s2_mck_sel",
		"apll_i2s4_mck_sel",
		"apll_tdmout_mck_sel",
		"mainpll_d2",
		"mainpll_d2_d2",
		"mainpll_d2_d4",
		"mainpll_d2_d16",
		"mainpll_d3",
		"mainpll_d3_d2",
		"mainpll_d3_d4",
		"mainpll_d5",
		"mainpll_d5_d2",
		"mainpll_d5_d4",
		"mainpll_d7",
		"mainpll_d7_d2",
		"mainpll_d7_d4",
		"univpll",
		"univpll_d2",
		"univpll_d2_d2",
		"univpll_d2_d4",
		"univpll_d3",
		"univpll_d3_d2",
		"univpll_d3_d4",
		"univpll_d3_d8",
		"univpll_d3_d32",
		"univpll_d5",
		"univpll_d5_d2",
		"univpll_d5_d4",
		"univpll_d7",
		"univpll_192m_d4",
		"univpll_192m_d8",
		"univpll_192m_d16",
		"univpll_192m_d32",
		"apll1_d2",
		"apll1_d4",
		"apll1_d8",
		"apll2_d2",
		"apll2_d4",
		"apll2_d8",
		"mmpll_d2",
		"tvdpll_d2",
		"tvdpll_d4",
		"tvdpll_d8",
		"tvdpll_d16",
		"tvdpll_d32",
		"msdcpll_d2",
		"ulposc1",
		"ulposc1_d2",
		"ulposc1_d4",
		"ulposc1_d8",
		"ulposc1_d10",
		"ulposc1_d16",
		"ulposc1_d32",
		"adsppll_d2",
		"adsppll_d4",
		"adsppll_d8",
		"nnapll_d2",
		"nnapll_d4",
		"nnapll_d8",
		"nna2pll_d2",
		"f_bist2fpc_ck",
		"hd_466m_fmem_ck",
		"mpll_ck",
		"apll12_div0",
		"apll12_div1",
		"apll12_div2",
		"apll12_div4",
		"apll12_div_tdmout_m",
		"infra_ao_pmic_tmr",
		"infra_ao_pmic_ap",
		"infra_ao_pmic_md",
		"infra_ao_pmic_conn",
		"infra_ao_sej",
		"infra_ao_apxgpt",
		"infra_ao_icusb",
		"infra_ao_gce",
		"infra_ao_therm",
		"infra_ao_i2c_ap",
		"infra_ao_i2c_ccu",
		"infra_ao_i2c_sspm",
		"infra_ao_i2c_rsv",
		"infra_ao_pwm_hclk",
		"infra_ao_pwm1",
		"infra_ao_pwm2",
		"infra_ao_pwm3",
		"infra_ao_pwm4",
		"infra_ao_pwm5",
		"infra_ao_pwm",
		"infra_ao_uart0",
		"infra_ao_uart1",
		"infra_ao_uart2",
		"infra_ao_gce_26m",
		"infra_ao_dma",
		"infra_ao_btif",
		"infra_ao_spi0",
		"infra_ao_msdc0",
		"infra_ao_msdcfde",
		"infra_ao_msdc1",
		"infra_ao_gcpu",
		"infra_ao_trng",
		"infra_ao_auxadc",
		"infra_ao_cpum",
		"infra_ao_ccif1_ap",
		"infra_ao_ccif1_md",
		"infra_ao_auxadc_md",
		"infra_ao_ap_dma",
		"infra_ao_xiu",
		"infra_ao_dapc",
		"infra_ao_ccif_ap",
		"infra_ao_debugtop",
		"infra_ao_audio",
		"infra_ao_ccif_md",
		"infra_ao_secore",
		"infra_ao_dxcc_ao",
		"infra_ao_imp_iic",
		"infra_ao_dramc26",
		"infra_ao_pwm_fbclk6",
		"infra_ao_ssusb_hclk",
		"infra_ao_disp_pwm",
		"infra_ao_cldmabclk",
		"infra_ao_audio26m",
		"infra_ao_ssusb_p1_hclk",
		"infra_ao_spi1",
		"infra_ao_i2c4",
		"infra_ao_mdtemp",
		"infra_ao_spi2",
		"infra_ao_spi3",
		"infra_ao_ssusb_ref",
		"infra_ao_ssusb_xhci",
		"infra_ao_ssusb_p1_ref",
		"infra_ao_ssusb_p1_xhci",
		"infra_ao_sspm",
		"infra_ao_ssusb_p1_sys",
		"infra_ao_i2c5",
		"infra_ao_i2c5a",
		"infra_ao_i2c5_imm",
		"infra_ao_i2c1a",
		"infra_ao_i2c1_imm",
		"infra_ao_i2c2a",
		"infra_ao_i2c2_imm",
		"infra_ao_spi4",
		"infra_ao_spi5",
		"infra_ao_cq_dma",
		"infra_ao_bist2fpc",
		"infra_ao_msdc0sf",
		"infra_ao_msdc1sf",
		"infra_ao_sspm_26m",
		"infra_ao_sspm_32k",
		"infra_ao_i2c6",
		"infra_ao_ap_msdc0",
		"infra_ao_md_msdc0",
		"infra_ao_msdc0_clk",
		"infra_ao_msdc1_clk",
		"infra_ao_sej_f13m",
		"infra_ao_aes_top0_bclk",
		"infra_ao_mcu_pm_bclk",
		"infra_ao_ccif2_ap",
		"infra_ao_ccif2_md",
		"infra_ao_ccif3_ap",
		"infra_ao_ccif3_md",
		"infra_ao_fadsp_26m",
		"infra_ao_fadsp_32k",
		"infra_ao_ccif4_ap",
		"infra_ao_ccif4_md",
		"infra_ao_fadsp_ck",
		"infra_ao_flashif_133m",
		"infra_ao_flashif_66m",
		"imp_iic_wrap_ap_clock_i2c6",
		"aud_afe",
		"aud_22m",
		"aud_24m",
		"aud_apll2_tuner",
		"aud_apll_tuner",
		"aud_tdm_ck",
		"aud_adc",
		"aud_dac",
		"aud_dac_predis",
		"aud_tml",
		"aud_nle",
		"aud_i2s1_bclk",
		"aud_i2s2_bclk",
		"aud_i2s3_bclk",
		"aud_i2s4_bclk",
		"aud_connsys_i2s_asrc",
		"aud_general1_asrc",
		"aud_general2_asrc",
		"aud_dac_hires",
		"aud_adc_hires",
		"aud_adc_hires_tml",
		"aud_adda6_adc",
		"aud_adda6_adc_hires",
		"aud_3rd_dac",
		"aud_3rd_dac_predis",
		"aud_3rd_dac_tml",
		"aud_3rd_dac_hires",
		"aud_etdm_in1_bclk",
		"aud_etdm_out1_bclk",
		"mfg_bg3d",
		"mm_disp_mutex0",
		"mm_apb_mm_bus",
		"mm_disp_ovl0",
		"mm_disp_rdma0",
		"mm_disp_ovl0_2l",
		"mm_disp_wdma0",
		"mm_disp_rsz0",
		"mm_disp_aal0",
		"mm_disp_ccorr0",
		"mm_disp_color0",
		"mm_smi_infra",
		"mm_disp_dsc_wrap0",
		"mm_disp_gamma0",
		"mm_disp_postmask0",
		"mm_disp_dither0",
		"mm_smi_common",
		"mm_dsi0",
		"mm_disp_fake_eng0",
		"mm_disp_fake_eng1",
		"mm_smi_gals",
		"mm_smi_iommu",
		"mm_dsi0_dsi_domain",
		"mm_disp_26m_ck",
		"wpe",
		"wpe_smi_larb8",
		"wpe_sys_event_tx",
		"wpe_smi_larb8_p_en",
		"img1_larb9_img1",
		"img1_larb10_img1",
		"img1_dip",
		"img1_gals_img1",
		"img2_larb9_img2",
		"img2_larb10_img2",
		"img2_mfb",
		"img2_wpe",
		"img2_mss",
		"img2_gals_img2",
		"vdec_larb1_cken",
		"vdec_lat_cken",
		"vdec_lat_active",
		"vdec_lat_cken_eng",
		"vdec_mini_mdp_cken_cfg_rg",
		"vdec_cken",
		"vdec_active",
		"vdec_cken_eng",
		"venc_cke0_larb",
		"venc_cke1_venc",
		"venc_cke2_jpgenc",
		"venc_cke5_gals",
		"cam_larb13",
		"cam_dfp_vad",
		"cam_larb14",
		"cam",
		"camtg",
		"cam_seninf",
		"camsv1",
		"camsv2",
		"camsv3",
		"cam_ccu0",
		"cam_ccu1",
		"cam_mraw0",
		"cam_fake_eng",
		"cam_ccu_gals",
		"cam2mm_gals",
		"cam_rawa_larbx_rawa",
		"cam_rawa",
		"cam_rawa_camtg_rawa",
		"cam_rawb_larbx_rawb",
		"cam_rawb",
		"cam_rawb_camtg_rawb",
		"mdp_rdma0",
		"mdp_tdshp0",
		"mdp_img_dl_async0",
		"mdp_img_dl_async1",
		"mdp_disp_rdma",
		"mdp_hms",
		"mdp_smi0",
		"mdp_apb_bus",
		"mdp_wrot0",
		"mdp_rsz0",
		"mdp_hdr0",
		"mdp_mutex0",
		"mdp_wrot1",
		"mdp_rsz1",
		"mdp_fake_eng0",
		"mdp_aal0",
		"mdp_disp_wdma",
		"mdp_color",
		"mdp_img_dl_async2",
		"mdp_img_dl_rel0_as0",
		"mdp_img_dl_rel1_as1",
		"mdp_img_dl_rel2_as2",
		"ipe_larb19",
		"ipe_larb20",
		"ipe_smi_subcom",
		"ipe_fd",
		"ipe_fe",
		"ipe_rsc",
		"ipe_dpe",
		"ipe_gals_ipe",
		/* end */
		NULL
	};

	return clks;
}

static const char * const *get_pwr_names(void)
{
	static const char * const pwr_names[] = {
		[0] = "WPE",
		[1] = "CONN_ON",
		[2] = "MFG0",
		[3] = "MFG1",
		[4] = "MFG2",
		[5] = "MFG3",
		[6] = "CSIRX_TOP",
		[7] = "MGF5",
		[8] = "MFG6",
		[9] = "IFR",
		[10] = "ADSP_INFRA",
		[11] = "DPY",
		[12] = "DRAMC_MD32",
		[13] = "IMG",
		[14] = "IMG2",
		[15] = "IPE",
		[16] = "VDEC",
		[17] = "ADSP_AO",
		[18] = "VENC",
		[19] = "SSUSB_P1",
		[20] = "SSUSB",
		[21] = "DIS",
		[22] = "AUDIO",
		[23] = "CAM",
		[24] = "CAM_RAWA",
		[25] = "CAM_RAWB",
		[26] = "NNA3",
		[27] = "NNA0",
		[28] = "NNA1",
		[29] = "NNA2",
		[30] = "NNA",
		[31] = "ADSP_TOP",
		[32] = NULL,
	};

	return pwr_names;
}

static u32 _get_pwr_status(u32 pwr_sta_ofs, u32 pwr_sta_2nd_ofs)
{
	static void __iomem *scpsys_base, *pwr_sta, *pwr_sta_2nd;

	scpsys_base = ioremap(0x10006000, PAGE_SIZE);
	pwr_sta = scpsys_base + pwr_sta_ofs;
	pwr_sta_2nd = scpsys_base + pwr_sta_2nd_ofs;

	return clk_readl(pwr_sta) & clk_readl(pwr_sta_2nd);
}

static u32 *get_all_pwr_status(void)
{
	static struct regs {
		u32 pwr_sta_ofs;
		u32 pwr_sta_2nd_ofs;
	} g[] = {
		{0x16c, 0x170},
	};

	static u32 pwr_sta[PWR_STA_GROUP_NR];
	int i;

	for (i = 0; i < PWR_STA_GROUP_NR; i++)
		pwr_sta[i] = _get_pwr_status(g[i].pwr_sta_ofs, g[i].pwr_sta_2nd_ofs);

	return pwr_sta;
}


/*
 * clkdbg dump_clks
 */

static void setup_provider_clk(struct provider_clk *pvdck)
{
	static const struct {
		const char *pvdname;
		u32 pwr_sta_group;
		u32 pwr_mask;
	} pvd_pwr_mask[] = {
		{"adspsys", 0, BIT(17)},
		{"nnasys", 0, BIT(30)},
		{"usb3_sif", 0, BIT(19)},
		{"mfgsys", 0, BIT(3)},
		{"mmsys", 0, BIT(21)},
		{"wpesys", 0, BIT(0)},
		{"imgsys1", 0, BIT(13)},
		{"imgsys2", 0, BIT(14)},
		{"vdecsys", 0, BIT(16)},
		{"vencsys", 0, BIT(18)},
		{"camsys", 0, BIT(23)},
		{"camsys_rawa", 0, BIT(24)},
		{"camsys_rawb", 0, BIT(25)},
		{"ipesys", 0, BIT(15)},
		{"mdpsys", 0, BIT(21)},
	};

	size_t i;
	const char *pvdname = pvdck->provider_name;

	if (pvdname == NULL)
		return;

	for (i = 0; i < ARRAY_SIZE(pvd_pwr_mask); i++) {
		if (strcmp(pvdname, pvd_pwr_mask[i].pvdname) == 0) {
			pvdck->pwr_sta_group = pvd_pwr_mask[i].pwr_sta_group;
			pvdck->pwr_mask = pvd_pwr_mask[i].pwr_mask;
			return;
		}
	}
}

static struct device_node *get_power_controller(void)
{
	return of_find_compatible_node(NULL, NULL, "mediatek,mt8186-power-controller");
}

static struct clkdbg_ops clkdbg_mt8186_ops = {
	.get_all_fmeter_clks = get_all_fmeter_clks,
	.fmeter_freq = fmeter_freq_op,
	.get_all_regnames = get_all_regnames,
	.get_all_clk_names = get_all_clk_names,
	.get_pwr_names = get_pwr_names,
	.setup_provider_clk = setup_provider_clk,
	.get_all_pwr_status = get_all_pwr_status,
	.get_power_controller = get_power_controller,
};

static int __init clkdbg_mt8186_init(void)
{
	if (of_machine_is_compatible("mediatek,mt8186") == 0)
		return -ENODEV;

	init_regbase();
	set_clkdbg_ops(&clkdbg_mt8186_ops);

#if DUMP_INIT_STATE
	print_regs();
	print_fmeter_all();
#endif /* DUMP_INIT_STATE */

	return 0;
}

static void __exit clkdbg_mt8186_exit(void)
{
}

module_init(clkdbg_mt8186_init);
module_exit(clkdbg_mt8186_exit);
MODULE_LICENSE("GPL");
