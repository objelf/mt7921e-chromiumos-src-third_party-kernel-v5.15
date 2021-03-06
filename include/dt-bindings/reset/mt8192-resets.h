/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2020 MediaTek Inc.
 * Author: Yong Liang <yong.liang@mediatek.com>
 */

#ifndef _DT_BINDINGS_RESET_CONTROLLER_MT8192
#define _DT_BINDINGS_RESET_CONTROLLER_MT8192

/* TOPRGU resets */
#define MT8192_TOPRGU_MM_SW_RST					1
#define MT8192_TOPRGU_MFG_SW_RST				2
#define MT8192_TOPRGU_VENC_SW_RST				3
#define MT8192_TOPRGU_VDEC_SW_RST				4
#define MT8192_TOPRGU_IMG_SW_RST				5
#define MT8192_TOPRGU_MD_SW_RST					7
#define MT8192_TOPRGU_CONN_SW_RST				9
#define MT8192_TOPRGU_CONN_MCU_SW_RST			12
#define MT8192_TOPRGU_IPU0_SW_RST				14
#define MT8192_TOPRGU_IPU1_SW_RST				15
#define MT8192_TOPRGU_AUDIO_SW_RST				17
#define MT8192_TOPRGU_CAMSYS_SW_RST				18
#define MT8192_TOPRGU_MJC_SW_RST				19
#define MT8192_TOPRGU_C2K_S2_SW_RST				20
#define MT8192_TOPRGU_C2K_SW_RST				21
#define MT8192_TOPRGU_PERI_SW_RST				22
#define MT8192_TOPRGU_PERI_AO_SW_RST			23

#define MT8192_TOPRGU_SW_RST_NUM				23

/* MMSYS resets */
#define MT8192_MMSYS_SW0_RST_B_DISP_DSI0			15

/* INFRA resets */
#define MT8192_INFRA_RST0_THERM_CTRL_SWRST		0
#define MT8192_INFRA_RST2_PEXTP_PHY_SWRST		1
#define MT8192_INFRA_RST3_THERM_CTRL_PTP_SWRST	2
#define MT8192_INFRA_RST4_PCIE_TOP_SWRST		3
#define MT8192_INFRA_RST4_THERM_CTRL_MCU_SWRST	4

#endif  /* _DT_BINDINGS_RESET_CONTROLLER_MT8192 */
