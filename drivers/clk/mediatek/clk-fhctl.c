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
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include "clk-fhctl.h"
#include "clk-fhctl-util.h"
#include "clk-mtk.h"

static struct pll_dts *_array = NULL;

bool mtk_fh_set_rate(const char *pll_name, unsigned long dds, int postdiv)
{
	int i;
	struct fh_hdlr *hdlr = NULL;
	struct pll_dts *array;
	int num_pll;

	if(_array == NULL)
		return false;

	array = _array;
	num_pll = array->num_pll;

	for (i = 0; i < num_pll; i++, array++) {
		if (!strcmp(pll_name, array->pll_name)) {
			hdlr = array->hdlr;
			break;
		}
	}

	if (hdlr && (array->perms & PERM_DRV_HOP)) {
		hdlr->ops->hopping(hdlr->data, array->domain, array->fh_id,
				   dds, 9999);
		return true;
	}

	return false;
}
EXPORT_SYMBOL(mtk_fh_set_rate);

static struct pll_dts *parse_dt(struct platform_device *pdev)
{
	struct device_node *root, *of_pll;
	char *domain, *method;
	unsigned int num_pll = 0;
	int iomap_idx = 0;
	struct pll_dts *array;
	int pll_idx = 0;
	const struct of_device_id *match;
	int size;
	void __iomem *fhctl_base, *apmixed_base;

	root = pdev->dev.of_node;
	match = of_match_node(pdev->dev.driver->of_match_table, root);

	of_property_read_string(root, "domain", (const char **)&domain);
	of_property_read_string(root, "method", (const char **)&method);
	fhctl_base = of_iomap(root, iomap_idx++);
	apmixed_base = of_iomap(root, iomap_idx++);

	/* iterate dts to get pll count */
	for_each_child_of_node(root, of_pll)
		num_pll++;

	size = sizeof(*array) * num_pll;
	array = kzalloc(size, GFP_KERNEL);

	for_each_child_of_node(root, of_pll) {
		int fh_id, pll_id, perms, ssc_rate;

		if (pll_idx >= num_pll) {
			pll_idx++;
			continue;
		}

		/* default for optional field */
		perms = 0xffffffff;
		ssc_rate = 0;

		of_property_read_u32(of_pll, "fh-id", &fh_id);
		of_property_read_u32(of_pll, "pll-id", &pll_id);
		of_property_read_u32(of_pll, "perms", &perms);
		of_property_read_u32(of_pll, "ssc-rate", &ssc_rate);
		array[pll_idx].num_pll = num_pll;
		array[pll_idx].comp = (char *)match->compatible;
		array[pll_idx].pll_name = (char *)of_pll->name;
		array[pll_idx].fh_id = fh_id;
		array[pll_idx].pll_id = pll_id;
		array[pll_idx].perms = perms;
		array[pll_idx].ssc_rate = ssc_rate;
		array[pll_idx].domain = domain;
		array[pll_idx].method = method;
		array[pll_idx].fhctl_base = fhctl_base;
		array[pll_idx].apmixed_base = apmixed_base;
		pll_idx++;
	}

	return array;
}

static int fh_plt_drv_probe(struct platform_device *pdev)
{
	int i, ret;
	int num_pll;
	struct pll_dts *array;

	/* convert dt to data */
	array = parse_dt(pdev);

	ret = fhctl_ap_init(array);
	if (ret)
		return -1;

	fhctl_debugfs_init(array);

	/* make sure array is complete */
	num_pll = array->num_pll;

	for (i = 0; i < num_pll; i++) {
		struct fh_hdlr *hdlr = array[i].hdlr;

		if (!hdlr) {
			dev_err(&pdev->dev, "Failed to set %s hdlr\n",
				array->pll_name);
			return -1;
		}
	}

	mb();
	_array = array;

	return 0;
}

static void fh_plt_drv_shutdown(struct platform_device *pdev)
{
	struct pll_dts *array = _array;
	int num_pll = array->num_pll;
	int i;

	for (i = 0; i < num_pll; i++, array++) {
		struct fh_hdlr *hdlr = array->hdlr;

		if (array->ssc_rate)
			hdlr->ops->ssc_disable(hdlr->data, array->domain,
					       array->fh_id);
	}
}

static const struct of_device_id fh_of_match[] = {
	{ .compatible = "mediatek,mt8186-fhctl"},
	{}
};

static struct platform_driver fhctl_driver = {
	.probe = fh_plt_drv_probe,
	.shutdown = fh_plt_drv_shutdown,
	.driver = {
		.name = "mtk-fhctl",
		.owner = THIS_MODULE,
		.of_match_table = fh_of_match,
	},
};

static int __init fhctl_driver_init(void)
{
        return platform_driver_register(&fhctl_driver);
}
device_initcall_sync(fhctl_driver_init);

static void __exit fhctl_driver_exit(void)
{
        platform_driver_unregister(&fhctl_driver);
}
module_exit(fhctl_driver_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("MediaTek FHCTL Driver");
MODULE_AUTHOR("Kuan-Hsin Lee <kuan-hsin.lee@mediatek.com>");
