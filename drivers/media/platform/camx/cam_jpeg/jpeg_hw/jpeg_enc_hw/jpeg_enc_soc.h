/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
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

#ifndef _CAM_JPEG_ENC_SOC_H_
#define _CAM_JPEG_ENC_SOC_H_

#include "cam_soc_util.h"

int cam_jpeg_enc_init_soc_resources(struct cam_hw_soc_info *soc_info,
	irq_handler_t jpeg_enc_irq_handler, void *irq_data);

int cam_jpeg_enc_enable_soc_resources(struct cam_hw_soc_info *soc_info);

int cam_jpeg_enc_disable_soc_resources(struct cam_hw_soc_info *soc_info);

#endif /* _CAM_JPEG_ENC_SOC_H_*/
