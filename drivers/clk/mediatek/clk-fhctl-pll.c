// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */
#include <linux/device.h>
#include <linux/module.h>
#include "clk-fhctl-pll.h"
#include "clk-fhctl-util.h"

#define REG_ADDR(base, x) ((void __iomem *)((unsigned long)base + (x)))

struct fh_pll_match {
	char *compatible;
	struct fh_pll_domain **domain_list;
};

static int fhctl_pll_init(struct fh_pll_domain *d, void __iomem *fhctl_base,
			  void __iomem *apmixed_base)
{
	struct fh_pll_data *data = d->data;
	struct fh_pll_offset *offset = d->offset;
	struct fh_pll_regs *regs = d->regs;

	if (regs->reg_hp_en)
		return 0;

	while (data->dds_mask != 0) {
		int regs_offset;

		/* fhctl common part */
		regs->reg_hp_en = REG_ADDR(fhctl_base, offset->offset_hp_en);
		regs->reg_clk_con = REG_ADDR(fhctl_base,
					     offset->offset_clk_con);
		regs->reg_rst_con = REG_ADDR(fhctl_base,
					     offset->offset_rst_con);
		regs->reg_slope0 = REG_ADDR(fhctl_base, offset->offset_slope0);
		regs->reg_slope1 = REG_ADDR(fhctl_base, offset->offset_slope1);

		/* fhctl pll part */
		regs_offset = offset->offset_fhctl + offset->offset_cfg;
		regs->reg_cfg = REG_ADDR(fhctl_base, regs_offset);
		regs->reg_updnlmt = REG_ADDR(regs->reg_cfg,
					     offset->offset_updnlmt);
		regs->reg_dds = REG_ADDR(regs->reg_cfg, offset->offset_dds);
		regs->reg_dvfs = REG_ADDR(regs->reg_cfg, offset->offset_dvfs);
		regs->reg_mon = REG_ADDR(regs->reg_cfg, offset->offset_mon);

		/* apmixed part */
		regs->reg_con_pcw = REG_ADDR(apmixed_base,
					     offset->offset_con_pcw);

		data++;
		offset++;
		regs++;
	}

	return 0;
}

#define SIZE_8186_TOP (sizeof(mt8186_top_data)\
	/sizeof(struct fh_pll_data))
#define DATA_8186_TOP(_name) {				\
		.name = _name,						\
		.dds_mask = GENMASK(21, 0),			\
		.slope0_value = 0x6003c97,			\
		.slope1_value = 0x6003c97,			\
		.sfstrx_en = BIT(2),				\
		.frddsx_en = BIT(1),				\
		.fhctlx_en = BIT(0),				\
		.tgl_org = BIT(31),					\
		.dvfs_tri = BIT(31),				\
		.pcwchg = BIT(31),					\
		.dt_val = 0x0,						\
		.df_val = 0x9,						\
		.updnlmt_shft = 16,					\
		.msk_frddsx_dys = GENMASK(23, 20),	\
		.msk_frddsx_dts = GENMASK(19, 16),	\
	}
#define OFFSET_8186_TOP(_fhctl, _con_pcw) {	\
		.offset_fhctl = _fhctl,				\
		.offset_con_pcw = _con_pcw,			\
		.offset_hp_en = 0x0,				\
		.offset_clk_con = 0x8,				\
		.offset_rst_con = 0xc,				\
		.offset_slope0 = 0x10,				\
		.offset_slope1 = 0x14,				\
		.offset_cfg = 0x0,					\
		.offset_updnlmt = 0x4,				\
		.offset_dds = 0x8,					\
		.offset_dvfs = 0xc,					\
		.offset_mon = 0x10,					\
	}
static struct fh_pll_data mt8186_top_data[] = {
	DATA_8186_TOP("armpll_ll"),
	DATA_8186_TOP("armpll_bl"),
	DATA_8186_TOP("ccipll"),
	DATA_8186_TOP("mainpll"),
	DATA_8186_TOP("mmpll"),
	DATA_8186_TOP("tvdpll"),
	DATA_8186_TOP("mpll"),
	DATA_8186_TOP("adsppll"),
	DATA_8186_TOP("mfgpll"),
	DATA_8186_TOP("nnapll"),
	DATA_8186_TOP("nna2pll"),
	DATA_8186_TOP("msdcpll"),
	DATA_8186_TOP("mempll"),
	{}
};
static struct fh_pll_offset mt8186_top_offset[] = {
	OFFSET_8186_TOP(0x003C, 0x0208),
	OFFSET_8186_TOP(0x0050, 0x0218),
	OFFSET_8186_TOP(0x0064, 0x0228),
	OFFSET_8186_TOP(0x0078, 0x0248),
	OFFSET_8186_TOP(0x008C, 0x0258),
	OFFSET_8186_TOP(0x00A0, 0x0268),
	OFFSET_8186_TOP(0x00B4, 0x0278),
	OFFSET_8186_TOP(0x00C8, 0x0308),
	OFFSET_8186_TOP(0x00DC, 0x0318),
	OFFSET_8186_TOP(0x00F0, 0x0360),
	OFFSET_8186_TOP(0x0104, 0x0370),
	OFFSET_8186_TOP(0x0118, 0x0390),
	OFFSET_8186_TOP(0x012c, 0xdeb1),
	{}
};
static struct fh_pll_regs mt8186_top_regs[SIZE_8186_TOP];
static struct fh_pll_domain mt8186_top = {
	.name = "top",
	.data = (struct fh_pll_data *)&mt8186_top_data,
	.offset = (struct fh_pll_offset *)&mt8186_top_offset,
	.regs = (struct fh_pll_regs *)&mt8186_top_regs,
	.init = &fhctl_pll_init,
};
static struct fh_pll_domain *mt8186_domain[] = {
	&mt8186_top,
	NULL,
};
static struct fh_pll_match mt8186_match = {
	.compatible = "mediatek,mt8186-fhctl",
	.domain_list = (struct fh_pll_domain **)mt8186_domain,
};

static const struct fh_pll_match *matches[] = {
	&mt8186_match,
	NULL
};

static struct fh_pll_domain **get_list(char *comp)
{
	struct fh_pll_match **match;
	static struct fh_pll_domain **list;
	int i;

	match = (struct fh_pll_match **)matches;

	/* name used only if !list */
	if (!list) {
		for (i = 0; i < ARRAY_SIZE(matches); i++) {
			if (!strcmp(comp, (*match)->compatible)) {
				list = (*match)->domain_list;
				break;
			}
			match++;
		}
	}
	return list;
}
void init_fh_domain(const char *domain, char *comp, void __iomem *fhctl_base,
		    void __iomem *apmixed_base)
{
	struct fh_pll_domain **list;

	list = get_list(comp);

	while (*list != NULL) {
		if (!strcmp(domain, (*list)->name)) {
			(*list)->init(*list, fhctl_base, apmixed_base);
			return;
		}
		list++;
	}
}

struct fh_pll_domain *get_fh_domain(const char *domain)
{
	struct fh_pll_domain **list;

	list = get_list(NULL);

	/* find instance */
	while (*list != NULL) {
		if (!strcmp(domain, (*list)->name))
			return *list;
		list++;
	}
	return NULL;
}
