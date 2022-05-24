// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */
#include <linux/device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/slab.h>
#include "clk-fhctl.h"
#include "clk-fhctl-pll.h"
#include "clk-fhctl-util.h"

#define FHCTL_TARGET FHCTL_AP

#define PERCENT_TO_DDSLMT(dds, percent_m10) \
	((((dds) * (percent_m10)) >> 5) / 100)

struct fh_ap_match {
	char *name;
	struct fh_hdlr *hdlr;
	int (*init)(struct pll_dts *array, struct fh_ap_match *match);
};
struct hdlr_data {
	struct pll_dts *array;
	struct fh_pll_domain *domain;
	spinlock_t *lock;
};

static int fhctl_set_ssc_regs(struct fh_pll_regs *regs,
			      struct fh_pll_data *data,
			      int fh_id, int rate)
{
	unsigned int updnlmt_val;

	if (rate > 0) {
		fh_set_field(regs->reg_cfg, data->frddsx_en, 0);
		fh_set_field(regs->reg_cfg, data->sfstrx_en, 0);
		fh_set_field(regs->reg_cfg, data->fhctlx_en, 0);

		/* Set the relative parameter registers (dt/df/upbnd/downbnd) */
		fh_set_field(regs->reg_cfg, data->msk_frddsx_dys, data->df_val);
		fh_set_field(regs->reg_cfg, data->msk_frddsx_dts, data->dt_val);

		writel((readl(regs->reg_con_pcw) & data->dds_mask) |
			data->tgl_org, regs->reg_dds);

		/* Calculate UPDNLMT */
		updnlmt_val = PERCENT_TO_DDSLMT((readl(regs->reg_dds) &
						 data->dds_mask), rate) <<
						 data->updnlmt_shft;

		writel(updnlmt_val, regs->reg_updnlmt);

		fh_set_field(regs->reg_hp_en, BIT(fh_id), 1);

		/* Enable SSC */
		fh_set_field(regs->reg_cfg, data->frddsx_en, 1);
		/* Enable Hopping control */
		fh_set_field(regs->reg_cfg, data->fhctlx_en, 1);

	} else {
		fh_set_field(regs->reg_cfg, data->frddsx_en, 0);
		fh_set_field(regs->reg_cfg, data->sfstrx_en, 0);
		fh_set_field(regs->reg_cfg, data->fhctlx_en, 0);

		/* Switch to APMIXEDSYS control */
		fh_set_field(regs->reg_hp_en, BIT(fh_id), 0);

		/* Wait for DDS to be stable */
		udelay(30);
	}

	return 0;
}

static int fhctl_ap_hopping(void *priv_data, char *domain_name, int fh_id,
			    unsigned int new_dds, int postdiv)
{
	struct fh_pll_domain *domain;
	struct fh_pll_regs *regs;
	struct fh_pll_data *data;
	unsigned int dds_mask;
	unsigned int mon_dds = 0;
	int ret = 0;
	unsigned int con_pcw_tmp;
	struct hdlr_data *d = (struct hdlr_data *)priv_data;
	spinlock_t *lock = d->lock;
	struct pll_dts *array = d->array;
	unsigned long flags = 0;

	spin_lock_irqsave(lock, flags);

	domain = d->domain;
	regs = &domain->regs[fh_id];
	data = &domain->data[fh_id];
	dds_mask = data->dds_mask;

	if (array->ssc_rate)
		fhctl_set_ssc_regs(regs, data, fh_id, 0);

	writel((readl(regs->reg_con_pcw) & dds_mask) |
		data->tgl_org, regs->reg_dds);

	fh_set_field(regs->reg_cfg, data->sfstrx_en, 1);
	fh_set_field(regs->reg_cfg, data->fhctlx_en, 1);
	writel(data->slope0_value, regs->reg_slope0);
	writel(data->slope1_value, regs->reg_slope1);

	fh_set_field(regs->reg_hp_en, BIT(fh_id), 1);
	writel((new_dds) | (data->dvfs_tri), regs->reg_dvfs);

	/* Wait 1000 us until DDS stable */
	ret = readl_poll_timeout_atomic(regs->reg_mon, mon_dds,
				(mon_dds & dds_mask) == new_dds, 10, 1000);
	if (ret)
		mb();

	con_pcw_tmp = readl(regs->reg_con_pcw) & (~dds_mask);
	con_pcw_tmp = (con_pcw_tmp | (readl(regs->reg_mon) & dds_mask) |
		       data->pcwchg);

	writel(con_pcw_tmp, regs->reg_con_pcw);

	fh_set_field(regs->reg_hp_en, BIT(fh_id), 0);

	if (array->ssc_rate)
		fhctl_set_ssc_regs(regs, data, fh_id, array->ssc_rate);

	spin_unlock_irqrestore(lock, flags);
	return ret;
}

static int fhctl_ap_ssc_enable(void *priv_data, char *domain_name,
			       int fh_id, int rate)
{
	struct fh_pll_domain *domain;
	struct fh_pll_regs *regs;
	struct fh_pll_data *data;
	struct hdlr_data *d = (struct hdlr_data *)priv_data;
	spinlock_t *lock = d->lock;
	struct pll_dts *array = d->array;
	unsigned long flags = 0;

	spin_lock_irqsave(lock, flags);

	domain = d->domain;
	regs = &domain->regs[fh_id];
	data = &domain->data[fh_id];

	fhctl_set_ssc_regs(regs, data, fh_id, rate);

	array->ssc_rate = rate;

	spin_unlock_irqrestore(lock, flags);

	return 0;
}

static int fhctl_ap_ssc_disable(void *priv_data, char *domain_name, int fh_id)
{
	struct fh_pll_domain *domain;
	struct fh_pll_regs *regs;
	struct fh_pll_data *data;
	struct hdlr_data *d = (struct hdlr_data *)priv_data;
	spinlock_t *lock = d->lock;
	struct pll_dts *array = d->array;
	unsigned long flags = 0;

	spin_lock_irqsave(lock, flags);

	domain = d->domain;
	regs = &domain->regs[fh_id];
	data = &domain->data[fh_id];

	fhctl_set_ssc_regs(regs, data, fh_id, 0);

	array->ssc_rate = 0;

	spin_unlock_irqrestore(lock, flags);

	return 0;
}

static int fhctl_ap_hw_init(struct pll_dts *array, struct fh_ap_match *match)
{
	static DEFINE_SPINLOCK(lock);
	struct hdlr_data *priv_data;
	struct fh_hdlr *hdlr;
	struct fh_pll_domain *domain;
	int fh_id = array->fh_id;
	struct fh_pll_regs *regs;
	struct fh_pll_data *data;
	int mask = BIT(fh_id);

	priv_data = kzalloc(sizeof(*priv_data), GFP_KERNEL);
	hdlr = kzalloc(sizeof(*hdlr), GFP_KERNEL);
	init_fh_domain(array->domain, array->comp, array->fhctl_base,
		       array->apmixed_base);

	priv_data->array = array;
	priv_data->lock = &lock;
	priv_data->domain = get_fh_domain(array->domain);

	/* do HW init */
	domain = priv_data->domain;
	regs = &domain->regs[fh_id];
	data = &domain->data[fh_id];

	fh_set_field(regs->reg_clk_con, mask, 1);
	fh_set_field(regs->reg_rst_con, mask, 0);
	fh_set_field(regs->reg_rst_con, mask, 1);
	writel(0x0, regs->reg_cfg);
	writel(0x0, regs->reg_updnlmt);
	writel(0x0, regs->reg_dds);

	/* hook to array */
	hdlr->data = priv_data;
	hdlr->ops = match->hdlr->ops;
	/* hook hdlr to array is the last step */
	mb();
	array->hdlr = hdlr;

	/* do SSC */
	if (array->ssc_rate) {
		struct fh_hdlr *hdlr = array->hdlr;

		hdlr->ops->ssc_enable(hdlr->data, array->domain, array->fh_id,
				      array->ssc_rate);
	}

	return 0;
}

static struct fh_operation fhctl_ap_ops = {
	.hopping = fhctl_ap_hopping,
	.ssc_enable = fhctl_ap_ssc_enable,
	.ssc_disable = fhctl_ap_ssc_disable,
};
static struct fh_hdlr fhctl_ap_hdlr = {
	.ops = &fhctl_ap_ops,
};

static struct fh_ap_match mt8186_match = {
        .name = "mediatek,mt8186-fhctl",
        .hdlr = &fhctl_ap_hdlr,
        .init = &fhctl_ap_hw_init,
};

static struct fh_ap_match *matches[] = {
	&mt8186_match,
	NULL,
};

int fhctl_ap_init(struct pll_dts *array)
{
	int i;
	int num_pll = array->num_pll;
	struct fh_ap_match **match = matches;

	/* find match by compatible */
	for (i = 0; i < ARRAY_SIZE(matches); i++) {
		char *comp = (*match)->name;
		char *target = array->comp;

		if (!strcmp(comp, target))
			break;
		match++;
	}

	if (*match == NULL)
		return -1;


	/* init flow for every pll */
	for (i = 0; i < num_pll; i++, array++) {
		char *method = array->method;

		if (!strcmp(method, FHCTL_TARGET)) {
			(*match)->init(array, *match);
		}
	}

	return 0;
}
