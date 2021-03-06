/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _CAM_CSID_PPI_DEV_H_
#define _CAM_CSID_PPI_DEV_H_

#include "cam_isp_hw.h"

irqreturn_t cam_csid_ppi_irq(int irq_num, void *data);
int cam_csid_ppi_probe(struct platform_device *pdev);
int cam_csid_ppi_remove(struct platform_device *pdev);

#endif /*_CAM_CSID_PPI_DEV_H_ */
