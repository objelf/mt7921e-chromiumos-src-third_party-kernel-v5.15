/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022 MediaTek Inc.
 * Author: Ping-Hsun Wu <ping-hsun.wu@mediatek.com>
 */

#ifndef __MDP_REG_AAL_H__
#define __MDP_REG_AAL_H__

#define MDP_AAL_OUTPUT_SIZE_MASK              (0x1FFF1FFF)
#define MDP_AAL_OUTPUT_OFFSET_MASK            (0x0FF00FF)

#define MDP_AAL_EN                0x000
#define MDP_AAL_RESET             0x004
#define MDP_AAL_INTEN             0x008
#define MDP_AAL_INTSTA            0x00c
#define MDP_AAL_STATUS            0x010
#define MDP_AAL_CFG               0x020
#define MDP_AAL_INPUT_COUNT       0x024
#define MDP_AAL_OUTPUT_COUNT      0x028
#define MDP_AAL_CHKSUM            0x02c
#define MDP_AAL_SIZE              0x030
#define MDP_AAL_OUT_SIZE          0x034
#define MDP_AAL_OUT_OFFSET        0x038
#define MDP_AAL_CFG_MAIN          0x200
#define MDP_AAL_Y2R_00            0x4bc
#define MDP_AAL_Y2R_01            0x4c0
#define MDP_AAL_Y2R_02            0x4c4
#define MDP_AAL_Y2R_03            0x4c8
#define MDP_AAL_Y2R_04            0x4cc
#define MDP_AAL_Y2R_05            0x4d0
#define MDP_AAL_R2Y_00            0x4d4
#define MDP_AAL_R2Y_01            0x4d8
#define MDP_AAL_R2Y_02            0x4dc
#define MDP_AAL_R2Y_03            0x4e0
#define MDP_AAL_R2Y_04            0x4e4
#define MDP_AAL_R2Y_05            0x4e8
#define MDP_AAL_SHADOW            0x0f0
#define MDP_AAL_DUMMY_REG         0x0c0
#define MDP_AAL_ATPG              0x0fc

/* MASK */
#define MDP_AAL_EN_MASK           0x00000001
#define MDP_AAL_RESET_MASK        0x00000001
#define MDP_AAL_INTEN_MASK        0x00000003
#define MDP_AAL_INTSTA_MASK       0x00000003
#define MDP_AAL_STATUS_MASK       0xfffffff3
#define MDP_AAL_CFG_MASK          0x70001317
#define MDP_AAL_OUT_SIZE_MASK     0xffffffff
#define MDP_AAL_OUT_OFFSET_MASK   0xffffffff
#define MDP_AAL_CFG_MAIN_MASK     0xffffffff
#define MDP_AAL_INPUT_COUNT_MASK  0x1fff1fff
#define MDP_AAL_OUTPUT_COUNT_MASK 0x1fff1fff
#define MDP_AAL_CHKSUM_MASK       0xffffffff
#define MDP_AAL_SIZE_MASK         0x1fff1fff
#define MDP_AAL_Y2R_00_MASK       0x01ff01ff
#define MDP_AAL_Y2R_01_MASK       0x1fff01ff
#define MDP_AAL_Y2R_02_MASK       0x1fff1fff
#define MDP_AAL_Y2R_03_MASK       0x1fff1fff
#define MDP_AAL_Y2R_04_MASK       0x1fff1fff
#define MDP_AAL_Y2R_05_MASK       0x1fff1fff
#define MDP_AAL_R2Y_00_MASK       0x01ff01ff
#define MDP_AAL_R2Y_01_MASK       0x07ff01ff
#define MDP_AAL_R2Y_02_MASK       0x07ff07ff
#define MDP_AAL_R2Y_03_MASK       0x07ff07ff
#define MDP_AAL_R2Y_04_MASK       0x07ff07ff
#define MDP_AAL_R2Y_05_MASK       0x07ff07ff
#define MDP_AAL_SHADOW_MASK       0x00000007
#define MDP_AAL_DUMMY_REG_MASK    0xffffffff
#define MDP_AAL_ATPG_MASK         0x00000003


#endif  // __MDP_REG_AAL_H__