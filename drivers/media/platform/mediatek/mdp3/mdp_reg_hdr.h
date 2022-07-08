/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022 MediaTek Inc.
 * Author: Ping-Hsun Wu <ping-hsun.wu@mediatek.com>
 */

#ifndef __MDP_REG_HDR_H__
#define __MDP_REG_HDR_H__

#define MDP_HDR_TOP                 (0x000)
#define MDP_HDR_RELAY               (0x004)
#define MDP_HDR_SIZE_0              (0x014)
#define MDP_HDR_SIZE_1              (0x018)
#define MDP_HDR_SIZE_2              (0x01C)
#define MDP_HDR_HIST_CTRL_0         (0x020)
#define MDP_HDR_HIST_CTRL_1         (0x024)
#define MDP_HDR_HIST_ADDR           (0x0DC)
#define MDP_HDR_TILE_POS            (0x118)

// MASK
#define MDP_HDR_RELAY_MASK          (0x01)
#define MDP_HDR_TOP_MASK            (0xFF0FEB6D)
#define MDP_HDR_SIZE_0_MASK         (0x1FFF1FFF)
#define MDP_HDR_SIZE_1_MASK         (0x1FFF1FFF)
#define MDP_HDR_SIZE_2_MASK         (0x1FFF1FFF)
#define MDP_HDR_HIST_CTRL_0_MASK    (0x1FFF1FFF)
#define MDP_HDR_HIST_CTRL_1_MASK    (0x1FFF1FFF)
#define MDP_HDR_HIST_ADDR_MASK      (0xBF3F2F3F)
#define MDP_HDR_TILE_POS_MASK       (0x1FFF1FFF)

#if 0
#define MDP_CCORR_EN                0x000
#define MDP_CCORR_RESET             0x004
#define MDP_CCORR_INTEN             0x008
#define MDP_CCORR_INTSTA            0x00c
#define MDP_CCORR_STATUS            0x010
#define MDP_CCORR_CFG               0x020
#define MDP_CCORR_INPUT_COUNT       0x024
#define MDP_CCORR_OUTPUT_COUNT      0x028
#define MDP_CCORR_CHKSUM            0x02c
#define MDP_CCORR_SIZE              0x030
#define MDP_CCORR_Y2R_00            0x034
#define MDP_CCORR_Y2R_01            0x038
#define MDP_CCORR_Y2R_02            0x03c
#define MDP_CCORR_Y2R_03            0x040
#define MDP_CCORR_Y2R_04            0x044
#define MDP_CCORR_Y2R_05            0x048
#define MDP_CCORR_R2Y_00            0x04c
#define MDP_CCORR_R2Y_01            0x050
#define MDP_CCORR_R2Y_02            0x054
#define MDP_CCORR_R2Y_03            0x058
#define MDP_CCORR_R2Y_04            0x05c
#define MDP_CCORR_R2Y_05            0x060
#define MDP_CCORR_COEF_0            0x080
#define MDP_CCORR_COEF_1            0x084
#define MDP_CCORR_COEF_2            0x088
#define MDP_CCORR_COEF_3            0x08c
#define MDP_CCORR_COEF_4            0x090
#define MDP_CCORR_SHADOW            0x0a0
#define MDP_CCORR_DUMMY_REG         0x0c0
#define MDP_CCORR_ATPG              0x0fc

/* MASK */
#define MDP_CCORR_EN_MASK           0x00000001
#define MDP_CCORR_RESET_MASK        0x00000001
#define MDP_CCORR_INTEN_MASK        0x00000003
#define MDP_CCORR_INTSTA_MASK       0x00000003
#define MDP_CCORR_STATUS_MASK       0xfffffff3
#define MDP_CCORR_CFG_MASK          0x70001317
#define MDP_CCORR_INPUT_COUNT_MASK  0x1fff1fff
#define MDP_CCORR_OUTPUT_COUNT_MASK 0x1fff1fff
#define MDP_CCORR_CHKSUM_MASK       0xffffffff
#define MDP_CCORR_SIZE_MASK         0x1fff1fff
#define MDP_CCORR_Y2R_00_MASK       0x01ff01ff
#define MDP_CCORR_Y2R_01_MASK       0x1fff01ff
#define MDP_CCORR_Y2R_02_MASK       0x1fff1fff
#define MDP_CCORR_Y2R_03_MASK       0x1fff1fff
#define MDP_CCORR_Y2R_04_MASK       0x1fff1fff
#define MDP_CCORR_Y2R_05_MASK       0x1fff1fff
#define MDP_CCORR_R2Y_00_MASK       0x01ff01ff
#define MDP_CCORR_R2Y_01_MASK       0x07ff01ff
#define MDP_CCORR_R2Y_02_MASK       0x07ff07ff
#define MDP_CCORR_R2Y_03_MASK       0x07ff07ff
#define MDP_CCORR_R2Y_04_MASK       0x07ff07ff
#define MDP_CCORR_R2Y_05_MASK       0x07ff07ff
#define MDP_CCORR_COEF_0_MASK       0x1fff1fff
#define MDP_CCORR_COEF_1_MASK       0x1fff1fff
#define MDP_CCORR_COEF_2_MASK       0x1fff1fff
#define MDP_CCORR_COEF_3_MASK       0x1fff1fff
#define MDP_CCORR_COEF_4_MASK       0x1fff1fff
#define MDP_CCORR_SHADOW_MASK       0x00000007
#define MDP_CCORR_DUMMY_REG_MASK    0xffffffff
#define MDP_CCORR_ATPG_MASK         0x00000003
#endif

#define MDP_HDR_INTERR              (0x00000008)
#define MDP_HDR_INTSTA              (0x0000000C)
#define MDP_HDR_ENGSTA              (0x00000010)
#define MDP_HDR_DEMO_CTRL_0         (0x00000028)
#define MDP_HDR_DEMO_CTRL_1         (0x0000002C)
#define MDP_HDR_3x3_COEF_0          (0x00000038)
#define MDP_HDR_3x3_COEF_1          (0x0000003C)
#define MDP_HDR_3x3_COEF_2          (0x00000040)
#define MDP_HDR_3x3_COEF_3          (0x00000044)
#define MDP_HDR_3x3_COEF_4          (0x00000048)
#define MDP_HDR_3x3_COEF_5          (0x0000004C)
#define MDP_HDR_3x3_COEF_6          (0x00000050)
#define MDP_HDR_3x3_COEF_7          (0x00000054)
#define MDP_HDR_3x3_COEF_8          (0x00000058)
#define MDP_HDR_3x3_COEF_9          (0x0000005C)
#define MDP_HDR_3x3_COEF_10         (0x00000060)
#define MDP_HDR_3x3_COEF_11         (0x00000064)
#define MDP_HDR_3x3_COEF_12         (0x00000068)
#define MDP_HDR_3x3_COEF_13         (0x0000006C)
#define MDP_HDR_3x3_COEF_14         (0x00000070)
#define MDP_HDR_3x3_COEF_15         (0x00000074)
#define MDP_HDR_CURSOR_CTRL         ( 0x0000010C)
#define MDP_HDR_CURSOR_POS          ( 0x00000110)
#define MDP_HDR_CURSOR_COLOR        ( 0x00000114)
#define MDP_HDR_CURSOR_BUF0         ( 0x0000011C)
#define MDP_HDR_CURSOR_BUF1         ( 0x00000120)
#define MDP_HDR_CURSOR_BUF2         ( 0x00000124)
#define MDP_HDR_R2Y_00              ( 0x00000128)
#define MDP_HDR_R2Y_01              ( 0x0000012C)
#define MDP_HDR_R2Y_02              ( 0x00000130)
#define MDP_HDR_R2Y_03              ( 0x00000134)
#define MDP_HDR_R2Y_04              ( 0x00000138)
#define MDP_HDR_R2Y_05              ( 0x0000013C)
#define MDP_HDR_R2Y_06              ( 0x00000140)
#define MDP_HDR_R2Y_07              ( 0x00000144)
#define MDP_HDR_R2Y_08              ( 0x00000148)
#define MDP_HDR_R2Y_09              ( 0x0000014C)
#define MDP_HDR_Y2R_00              ( 0x00000150)
#define MDP_HDR_Y2R_01              ( 0x00000154)
#define MDP_HDR_Y2R_02              ( 0x0000015C)
#define MDP_HDR_Y2R_03              ( 0x00000160)
#define MDP_HDR_Y2R_04              ( 0x00000164)
#define MDP_HDR_Y2R_05              ( 0x00000168)
#define MDP_HDR_Y2R_06              ( 0x0000016C)
#define MDP_HDR_Y2R_07              ( 0x00000170)
#define MDP_HDR_Y2R_08              ( 0x00000174)
#define MDP_HDR_Y2R_09              ( 0x00000178)
#define MDP_HDR_Y2R_10              ( 0x0000017C)
#define MDP_HDR_PROG_EOTF_0         ( 0x00000180)
#define MDP_HDR_PROG_EOTF_1         ( 0x00000184)
#define MDP_HDR_PROG_EOTF_2         ( 0x00000188)
#define MDP_HDR_PROG_EOTF_3         ( 0x0000018C)
#define MDP_HDR_PROG_EOTF_4         ( 0x00000190)
#define MDP_HDR_PROG_EOTF_5         ( 0x00000194)
#define MDP_HDR_EOTF_TABLE_0        ( 0x0000019C)
#define MDP_HDR_EOTF_TABLE_1        ( 0x000001A0)
#define MDP_HDR_EOTF_TABLE_2        ( 0x000001A4)
#define MDP_HDR_OETF_TABLE_0        ( 0x000001A8)
#define MDP_HDR_OETF_TABLE_1        ( 0x000001AC)
#define MDP_HDR_EOTF_ACCURACY_0     ( 0x000001B4)
#define MDP_HDR_EOTF_ACCURACY_1     ( 0x000001B8)
#define MDP_HDR_EOTF_ACCURACY_2     ( 0x000001BC)
#define MDP_HDR_DUMMY0              ( 0x000001D4)
#define MDP_HDR_DUMMY1              ( 0x000001D8)
#define MDP_HDR_DUMMY2              ( 0x000001DC)



// MASK

#define MDP_HDR_INTERR_MASK         (0x0F)
#define MDP_HDR_INTSTA_MASK         (0x0F)
#define MDP_HDR_ENGSTA_MASK         (0x07)

#define MDP_HDR_DEMO_CTRL_0_MASK    (0x1FFF1FFF)
#define MDP_HDR_DEMO_CTRL_1_MASK    (0xDFFF1FFF)
#define MDP_HDR_3x3_COEF_0_MASK     (0x03)
#define MDP_HDR_3x3_COEF_1_MASK     (0xFFFFFFFF)
#define MDP_HDR_3x3_COEF_2_MASK     (0xFFFFFFFF)
#define MDP_HDR_3x3_COEF_3_MASK     (0xFFFFFFFF)
#define MDP_HDR_3x3_COEF_4_MASK     (0xFFFFFFFF)
#define MDP_HDR_3x3_COEF_5_MASK     (0x0FFFF)
#define MDP_HDR_3x3_COEF_6_MASK     (0x01FFFF)
#define MDP_HDR_3x3_COEF_7_MASK     (0x01FFFF)
#define MDP_HDR_3x3_COEF_8_MASK     (0x01FFFF)
#define MDP_HDR_3x3_COEF_9_MASK     (0x01FFFF)
#define MDP_HDR_3x3_COEF_10_MASK    (0x01FFFF)
#define MDP_HDR_3x3_COEF_11_MASK    (0x01FFFF)
#define MDP_HDR_3x3_COEF_12_MASK    (0x01FFFF)
#define MDP_HDR_3x3_COEF_13_MASK    (0x01FFFF)
#define MDP_HDR_3x3_COEF_14_MASK    (0x01FFFF)
#define MDP_HDR_3x3_COEF_15_MASK    (0x01FFFF)

#define MDP_HDR_CURSOR_CTRL_MASK    (0x0F1)
#define MDP_HDR_CURSOR_POS_MASK     (0x3FFF3FFF)
#define MDP_HDR_CURSOR_COLOR_MASK   (0x0FFF)
#define MDP_HDR_CURSOR_BUF0_MASK    (0xFFFFFFFF)
#define MDP_HDR_CURSOR_BUF1_MASK    (0x0FFFF)

#define MDP_HDR_R2Y_00_MASK         (0xFFFFFFFF)
#define MDP_HDR_R2Y_01_MASK         (0xFFFFFFFF)
#define MDP_HDR_R2Y_02_MASK         (0xFFFFFFFF)
#define MDP_HDR_R2Y_03_MASK         (0xFFFFFFFF)
#define MDP_HDR_R2Y_04_MASK         (0xFFFF)
#define MDP_HDR_R2Y_05_MASK         (0x1FFF1FFF)
#define MDP_HDR_R2Y_06_MASK         (0xFFFF8000)
#define MDP_HDR_R2Y_07_MASK         (0x7FF07FF)
#define MDP_HDR_R2Y_08_MASK         (0xFFFFFC00)
#define MDP_HDR_R2Y_09_MASK         (0x0F)

#define MDP_HDR_Y2R_00_MASK         (0xFFFFFFFF)
#define MDP_HDR_Y2R_01_MASK         (0xFFFFFFFF)
#define MDP_HDR_Y2R_02_MASK         (0xFFFFFFFF)
#define MDP_HDR_Y2R_03_MASK         (0xFFFFFFFF)
#define MDP_HDR_Y2R_04_MASK         (0xFFFF)
#define MDP_HDR_Y2R_05_MASK         (0x7FF07FF)
#define MDP_HDR_Y2R_06_MASK         (0x7FF)
#define MDP_HDR_Y2R_07_MASK         (0x1FFF1FFF)
#define MDP_HDR_Y2R_08_MASK         (0x1FFF)
#define MDP_HDR_Y2R_09_MASK         (0x0F)
#define MDP_HDR_Y2R_10_MASK         (0xFFFFFFFF)

#define MDP_HDR_PROG_EOTF_0_MASK    (0x0007FFFFF)
#define MDP_HDR_PROG_EOTF_1_MASK    (0x000001FF)
#define MDP_HDR_PROG_EOTF_2_MASK    (0x0007FFFFF)
#define MDP_HDR_PROG_EOTF_3_MASK    (0x000001FF)
#define MDP_HDR_PROG_EOTF_4_MASK    (0x0007FFFFF)
#define MDP_HDR_PROG_EOTF_5_MASK    (0x000001FF)

#define MDP_HDR_PROG_EOTF_MASK      (0xFFE1FFFF)
#define MDP_HDR_EOTF_TABLE_0_MASK   (0x8000F9FF)
#define MDP_HDR_EOTF_TABLE_1_MASK   (0xFFFFFFFF)
#define MDP_HDR_EOTF_TABLE_2_MASK   (0x003FFFFF)
#define MDP_HDR_OETF_TABLE_0_MASK   (0x8000FBFF)
#define MDP_HDR_OETF_TABLE_1_MASK   (0xFFFFFFFF)

#define MDP_HDR_EOTF_ACCURACY_0_MASK (0x00000001)
#define MDP_HDR_EOTF_ACCURACY_1_MASK (0x03FF03FF)
#define MDP_HDR_EOTF_ACCURACY_2_MASK (0x03FF03FF)

#define MDP_HDR_DUMMY0_MASK         (0xFFFFFFFF)
#define MDP_HDR_DUMMY1_MASK         (0xFFFFFFFF)
#define MDP_HDR_DUMMY2_MASK         (0xFFFFFFFF)

#define MDP_HDR_HLG_SG_MASK         (0x0000FFFF)

#define MDP_HDR_TONE_MDP_TOP_MASK   (0x01)
//#define MDP_HDR_DUMMY0_MASK         (0xFFFFFFFF)
//#define MDP_HDR_DUMMY1_MASK         (0xFFFFFFFF)
//#define MDP_HDR_DUMMY2_MASK         (0xFFFFFFFF)


#endif  // __MDP_REG_HDR_H__