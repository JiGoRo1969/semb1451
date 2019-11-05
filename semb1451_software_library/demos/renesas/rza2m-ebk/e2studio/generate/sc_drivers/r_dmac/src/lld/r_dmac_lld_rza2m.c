/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * File Name     : r_dmac_lld_rza2m.c
 * Device(s)     : RZ/A2M
 * Tool-Chain    : GCC Arm Embedded 6.3.1
 * H/W Platform  : RZ/A2M RSK board
 * Description   : DMAC driver
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 31.08.2018 1.00
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <stdio.h>

#include "rza_io_regrw.h"
#include "iodefine.h"
#include "iobitmask.h"

#include "r_dmac_lld_rza2m.h"        /* Low layer driver header */
#include "r_dmac_hld_prv.h"

#include "r_intc_lld_rza2m.h"

#include "driver.h"

#define NOT_USED(p)                          ((void)(p))         /* suppress function parameter not used warning */

#define DMAC_PRV_DMA_UNIT_1                  (0ul)               /* unit size of DMA transfer = 1 byte */
#define DMAC_PRV_DMA_UNIT_2                  (1ul)               /* unit size of DMA transfer = 2 bytes */
#define DMAC_PRV_DMA_UNIT_4                  (2ul)               /* unit size of DMA transfer = 4 bytes */
#define DMAC_PRV_DMA_UNIT_8                  (3ul)               /* unit size of DMA transfer = 8 bytes */
#define DMAC_PRV_DMA_UNIT_16                 (4ul)               /* unit size of DMA transfer = 16 bytes */
#define DMAC_PRV_DMA_UNIT_32                 (5ul)               /* unit size of DMA transfer = 32 bytes */
#define DMAC_PRV_DMA_UNIT_64                 (6ul)               /* unit size of DMA transfer = 64 bytes */
#define DMAC_PRV_DMA_UNIT_128                (7ul)               /* unit size of DMA transfer = 128 bytes */

/* Magic Number */
#define DMAC_PRV_SHIFT_DMARS_EVEN_CH         (0U)                /* Shift Value for DMARS Register access in Even channel */
#define DMAC_PRV_SHIFT_DMARS_ODD_CH          (16U)               /* Shift Value for DMARS Register access in Odd channel */
#define DMAC_PRV_MASK_DMARS_EVEN_CH          (0xFFFF0000U)       /* Mask value for DMARS Register in Even channel */
#define DMAC_PRV_MASK_DMARS_ODD_CH           (0x0000FFFFU)       /* Mask value for DMARS Register in Odd channel */
#define DMAC_PRV_HIGH_COMMON_REG_OFFSET      (8)                 /* for Common Register Access in channel 0-8 */
#define DMAC_PRV_CHECK_ODD_EVEN_MASK         (0x00000001U)       /* for check value of odd or even */
#define DMAC_PRV_DMA_STOP_WAIT_MAX_CNT       (10U)               /* Loop count for DMA stop (usually, a count is set to 0 or 1) */

/* Register Set Value */
/* Initial Value */
#define DMAC_PRV_N0SA_INIT_VALUE             (0U)                /* HW initial value */
#define DMAC_PRV_N1SA_INIT_VALUE             (0U)                /* HW initial value */
#define DMAC_PRV_N0DA_INIT_VALUE             (0U)                /* HW initial value */
#define DMAC_PRV_N1DA_INIT_VALUE             (0U)                /* HW initial value */
#define DMAC_PRV_N0TB_INIT_VALUE             (0U)                /* HW initial value */
#define DMAC_PRV_N1TB_INIT_VALUE             (0U)                /* HW initial value */
#define DMAC_PRV_CHCTRL_INIT_VALUE           (0U)                /* HW initial value */
#define DMAC_PRV_CHCFG_INIT_VALUE            (0x01000000U)       /* interrupt disable */
#define DMAC_PRV_CHITVL_INIT_VALUE           (0U)                /* DMA interval = 0 */
#define DMAC_PRV_CHEXT_INIT_VALUE            (0U)                /* HW initial value */
#define DMAC_PRV_NXLA_INIT_VALUE             (0U)                /* HW initial value */
#define DMAC_PRV_DCTRL_INIT_VALUE            (0x00000001U)       /* interrupt output : pulse, round robin mode */
#define DMAC_PRV_DMARS_INIT_VALUE            (0U)                /* HW initial value */

/* Fixed Setting for CHCFG */
#define DMAC_PRV_CHCFG_FIXED_VALUE           (0x00000020U)       /* register mode, not buffer sweep, interrupt detect when high pulse */

/* Bit Value & Mask */
/* CHSTAT */
#define DMAC_PRV_CHSTAT_MASK_SR              (0x00000080U)
#define DMAC_PRV_CHSTAT_MASK_END             (0x00000020U)
#define DMAC_PRV_CHSTAT_MASK_ER              (0x00000010U)
#define DMAC_PRV_CHSTAT_MASK_TACT            (0x00000004U)
#define DMAC_PRV_CHSTAT_MASK_EN              (0x00000001U)

/* CHCTRL */
#define DMAC_PRV_CHCTRL_SET_CLRTC            (0x00000040U)
#define DMAC_PRV_CHCTRL_SET_CLREND           (0x00000020U)
#define DMAC_PRV_CHCTRL_SET_SWRST            (0x00000008U)
#define DMAC_PRV_CHCTRL_SET_STG              (0x00000004U)
#define DMAC_PRV_CHCTRL_SET_CLREN            (0x00000002U)
#define DMAC_PRV_CHCTRL_SET_SETEN            (0x00000001U)

/* CHCFG */
#define DMAC_PRV_CHCFG_SET_REN               (0x40000000U)
#define DMAC_PRV_CHCFG_MASK_REN              (0x40000000U)
#define DMAC_PRV_CHCFG_SET_RSW               (0x20000000U)
#define DMAC_PRV_CHCFG_MASK_RSW              (0x20000000U)
#define DMAC_PRV_CHCFG_SET_RSEL              (0x10000000U)
#define DMAC_PRV_CHCFG_MASK_RSEL             (0x10000000U)
#define DMAC_PRV_CHCFG_SET_DEM               (0x01000000U)
#define DMAC_PRV_CHCFG_MASK_DEM              (0x01000000U)
#define DMAC_PRV_CHCFG_SET_TM                (0x00400000U)
#define DMAC_PRV_CHCFG_MASK_DAD              (0x00200000U)
#define DMAC_PRV_CHCFG_MASK_SAD              (0x00100000U)
#define DMAC_PRV_CHCFG_MASK_DDS              (0x000f0000U)
#define DMAC_PRV_CHCFG_MASK_SDS              (0x0000f000U)
#define DMAC_PRV_CHCFG_SET_AM_LEVEL          (0x00000100U)
#define DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE      (0x00000200U)
#define DMAC_PRV_CHCFG_MASK_AM               (0x00000700U)
#define DMAC_PRV_CHCFG_SET_LVL_EDGE          (0x00000000U)
#define DMAC_PRV_CHCFG_SET_LVL_LEVEL         (0x00000040U)
#define DMAC_PRV_CHCFG_MASK_LVL              (0x00000040U)
#define DMAC_PRV_CHCFG_SET_REQD_SRC          (0x00000000U)
#define DMAC_PRV_CHCFG_SET_REQD_DST          (0x00000008U)
#define DMAC_PRV_CHCFG_MASK_REQD             (0x00000008U)
#define DMAC_PRV_CHCFG_SHIFT_DAD             (21U)
#define DMAC_PRV_CHCFG_SHIFT_SAD             (20U)
#define DMAC_PRV_CHCFG_SHIFT_DDS             (16U)
#define DMAC_PRV_CHCFG_SHIFT_SDS             (12U)

/* CHEXT */
#define DMAC_PRV_CHEXT_SET_DCA_NORMAL        (0x00003000U)
#define DMAC_PRV_CHEXT_SET_DCA_STRONG        (0x00000000U)
#define DMAC_PRV_CHEXT_SET_DPR_NON_SECURE    (0x00000200U)
#define DMAC_PRV_CHEXT_SET_SCA_NORMAL        (0x00000030U)
#define DMAC_PRV_CHEXT_SET_SCA_STRONG        (0x00000000U)
#define DMAC_PRV_CHEXT_SET_SPR_NON_SECURE    (0x00000002U)

/* REQD value in CHCFG is undecided on configuration table */
/* used case of a resource is the same and two or more direction value exists */
#define DMAC_PRV_CHCFG_REQD_UNDEFINED        (2)

/* Address of area which is the target of setting change */
#define DMAC_PRV_DMA_EXTERNAL_BUS_START         (0x00000000U)
#define DMAC_PRV_DMA_EXTERNAL_BUS_END           (0x1FFFFFFFU)
#define DMAC_PRV_DMA_EXTERNAL_BUS_MIRROR_START  (0x40000000U)
#define DMAC_PRV_DMA_EXTERNAL_BUS_MIRROR_END    (0x5FFFFFFFU)

/* DMA channel number */
typedef enum
{
    DMA_CH_0 = 0,
    DMA_CH_1 = 1,
    DMA_CH_2 = 2,
    DMA_CH_3 = 3,
    DMA_CH_4 = 4,
    DMA_CH_5 = 5,
    DMA_CH_6 = 6,
    DMA_CH_7 = 7,
    DMA_CH_8 = 8,
    DMA_CH_9 = 9,
    DMA_CH_10 = 10,
    DMA_CH_11 = 11,
    DMA_CH_12 = 12,
    DMA_CH_13 = 13,
    DMA_CH_14 = 14,
    DMA_CH_15 = 15
} e_dma_ch_num_t;

/* DMA Register (every Channel) */
typedef struct
{
    volatile uint32_t n0sa_n;
    volatile uint32_t n0da_n;
    volatile uint32_t n0tb_n;
    volatile uint32_t n1sa_n;
    volatile uint32_t n1da_n;
    volatile uint32_t n1tb_n;
    volatile uint32_t crsa_n;
    volatile uint32_t crda_n;
    volatile uint32_t crtb_n;
    volatile uint32_t chstat_n;
    volatile uint32_t chctrl_n;
    volatile uint32_t chcfg_n;
    volatile uint32_t chitvl_n;
    volatile uint32_t chext_n;
    volatile uint32_t nxla_n;
    volatile uint32_t crla_n;
} st_r_dmac_t;

/* DMA Register (Common) */
typedef struct
{
    volatile uint32_t dctrl_0_7;
    volatile uint8_t dummy1[12];
    volatile uint32_t dstat_en_0_7;
    volatile uint32_t dstat_er_0_7;
    volatile uint32_t dstat_end_0_7;
    volatile uint32_t dstat_tc_0_7;
    volatile uint32_t dstat_sus_0_7;
} st_r_dmaccommon_t;

/*************************************************************************
 Enumerated Types
*************************************************************************/

/* DMA channel configuration table */
typedef struct
{
    e_r_drv_dmac_xfer_resource_t resource;          /* this will be equal to the index */
    uint32_t dmars;                                 /* set value for DMARS Register */
    uint32_t tm_am;                                 /* set value for TM / AM bits (CHCFG Register) */
    uint32_t lvl;                                   /* set value for LVL bit (CHCFG Register) */
    uint32_t reqd;                                  /* set value for REQD bit (CHCFG Register) */
} st_dma_configuration_t;

/* address table of register set for each channel */
static volatile st_r_dmac_t *gsp_dma_ch_register_addr_table[DMAC_NUMBER_OF_CHANNELS] =
{
#ifdef USE_SECURE_MODULE
    (volatile st_r_dmac_t *) &DMAC0.N0SA_0S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_1S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_2S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_3S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_4S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_5S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_6S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_7S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_8S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_9S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_10S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_11S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_12S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_13S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_14S,
    (volatile st_r_dmac_t *) &DMAC0.N0SA_15S,
#else
    (volatile st_r_dmac_t *) &DMAC1.N0SA_0,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_1,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_2,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_3,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_4,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_5,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_6,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_7,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_8,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_9,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_10,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_11,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_12,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_13,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_14,
    (volatile st_r_dmac_t *) &DMAC1.N0SA_15,
#endif
};

/* address table of register set for common register */
static volatile st_r_dmaccommon_t *gsp_dma_common_reg_addr_table[DMAC_NUMBER_OF_CHANNELS] =
{
#ifdef USE_SECURE_MODULE
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_0_7S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_0_7S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_0_7S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_0_7S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_0_7S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_0_7S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_0_7S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_0_7S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_8_15S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_8_15S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_8_15S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_8_15S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_8_15S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_8_15S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_8_15S,
    (volatile st_r_dmaccommon_t *) &DMAC0.DCTRL_8_15S,
#else
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_0_7,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_0_7,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_0_7,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_0_7,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_0_7,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_0_7,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_0_7,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_0_7,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_8_15,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_8_15,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_8_15,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_8_15,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_8_15,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_8_15,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_8_15,
    (volatile st_r_dmaccommon_t *) &DMAC1.DCTRL_8_15,
#endif
};

/* address table of register set for DMARS */
static volatile uint32_t *gsp_dmars_register_addr_table[DMAC_NUMBER_OF_CHANNELS] =
{
#ifdef USE_SECURE_MODULE
    (volatile uint32_t *) &DMAC0.DMARS0S,
    (volatile uint32_t *) &DMAC0.DMARS0S,
    (volatile uint32_t *) &DMAC0.DMARS1S,
    (volatile uint32_t *) &DMAC0.DMARS1S,
    (volatile uint32_t *) &DMAC0.DMARS2S,
    (volatile uint32_t *) &DMAC0.DMARS2S,
    (volatile uint32_t *) &DMAC0.DMARS3S,
    (volatile uint32_t *) &DMAC0.DMARS3S,
    (volatile uint32_t *) &DMAC0.DMARS4S,
    (volatile uint32_t *) &DMAC0.DMARS4S,
    (volatile uint32_t *) &DMAC0.DMARS5S,
    (volatile uint32_t *) &DMAC0.DMARS5S,
    (volatile uint32_t *) &DMAC0.DMARS6S,
    (volatile uint32_t *) &DMAC0.DMARS6S,
    (volatile uint32_t *) &DMAC0.DMARS7S,
    (volatile uint32_t *) &DMAC0.DMARS7S,
#else
    (volatile uint32_t *) &DMAC1.DMARS0,
    (volatile uint32_t *) &DMAC1.DMARS0,
    (volatile uint32_t *) &DMAC1.DMARS1,
    (volatile uint32_t *) &DMAC1.DMARS1,
    (volatile uint32_t *) &DMAC1.DMARS2,
    (volatile uint32_t *) &DMAC1.DMARS2,
    (volatile uint32_t *) &DMAC1.DMARS3,
    (volatile uint32_t *) &DMAC1.DMARS3,
    (volatile uint32_t *) &DMAC1.DMARS4,
    (volatile uint32_t *) &DMAC1.DMARS4,
    (volatile uint32_t *) &DMAC1.DMARS5,
    (volatile uint32_t *) &DMAC1.DMARS5,
    (volatile uint32_t *) &DMAC1.DMARS6,
    (volatile uint32_t *) &DMAC1.DMARS6,
    (volatile uint32_t *) &DMAC1.DMARS7,
    (volatile uint32_t *) &DMAC1.DMARS7,
#endif
};

static const e_r_drv_intc_intid_t gs_dma_int_num_table[DMAC_NUMBER_OF_CHANNELS] =
{
#ifdef USE_SECURE_MODULE
    INTC_ID_DMAC30_DMAINT0, INTC_ID_DMAC30_DMAINT1, INTC_ID_DMAC30_DMAINT2, INTC_ID_DMAC30_DMAINT3,
    INTC_ID_DMAC30_DMAINT4, INTC_ID_DMAC30_DMAINT5, INTC_ID_DMAC30_DMAINT6, INTC_ID_DMAC30_DMAINT7,
    INTC_ID_DMAC30_DMAINT8, INTC_ID_DMAC30_DMAINT9, INTC_ID_DMAC30_DMAINT10, INTC_ID_DMAC30_DMAINT11,
    INTC_ID_DMAC30_DMAINT12, INTC_ID_DMAC30_DMAINT13, INTC_ID_DMAC30_DMAINT14, INTC_ID_DMAC30_DMAINT15
#else
    INTC_ID_DMAC31_DMAINT0, INTC_ID_DMAC31_DMAINT1, INTC_ID_DMAC31_DMAINT2, INTC_ID_DMAC31_DMAINT3,
    INTC_ID_DMAC31_DMAINT4, INTC_ID_DMAC31_DMAINT5, INTC_ID_DMAC31_DMAINT6, INTC_ID_DMAC31_DMAINT7,
    INTC_ID_DMAC31_DMAINT8, INTC_ID_DMAC31_DMAINT9, INTC_ID_DMAC31_DMAINT10, INTC_ID_DMAC31_DMAINT11,
    INTC_ID_DMAC31_DMAINT12, INTC_ID_DMAC31_DMAINT13, INTC_ID_DMAC31_DMAINT14, INTC_ID_DMAC31_DMAINT15
#endif
};

static void end0_interrupt_handler (void);
static void end1_interrupt_handler (void);
static void end2_interrupt_handler (void);
static void end3_interrupt_handler (void);
static void end4_interrupt_handler (void);
static void end5_interrupt_handler (void);
static void end6_interrupt_handler (void);
static void end7_interrupt_handler (void);
static void end8_interrupt_handler (void);
static void end9_interrupt_handler (void);
static void end10_interrupt_handler (void);
static void end11_interrupt_handler (void);
static void end12_interrupt_handler (void);
static void end13_interrupt_handler (void);
static void end14_interrupt_handler (void);
static void end15_interrupt_handler (void);

/* DMA transfer resource */
static const st_dma_configuration_t gs_dma_configuration_table[] =
{
  /* resource,                 DMARS, CHCFG AM / TM                  , CHCFG LVL                   , CHCFG REQD */
    {DMA_RS_OSTM0TINT,         0x023, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< OS Timer channel 0 */
    {DMA_RS_OSTM1TINT,         0x073, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< OS Timer channel 1 */
    {DMA_RS_OSTM2TINT,         0x02b, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< OS Timer channel 2 */

    {DMA_RS_TGIA_0,            0x043, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Multi Function Timer Pulse Unit3a channel 0 */
    {DMA_RS_TGIB_0,            0x047, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIC_0,            0x04B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGID_0,            0x04F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIA_1,            0x053, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Multi Function Timer Pulse Unit3a channel 1 */
    {DMA_RS_TGIB_1,            0x057, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIA_2,            0x05B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Multi Function Timer Pulse Unit3a channel 2 */
    {DMA_RS_TGIB_2,            0x05F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIA_3,            0x063, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Multi Function Timer Pulse Unit3a channel 3 */
    {DMA_RS_TGIB_3,            0x067, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIC_3,            0x06B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGID_3,            0x06F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIA_4,            0x073, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Multi Function Timer Pulse Unit3a channel 4 */
    {DMA_RS_TGIB_4,            0x077, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIC_4,            0x07B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGID_4,            0x07F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TCIV_4,            0x083, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIU_5,            0x087, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Multi Function Timer Pulse Unit3a channel 5 */
    {DMA_RS_TGIV_5,            0x08B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIW_5,            0x08F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIA_6,            0x093, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Multi Function Timer Pulse Unit3a channel 6 */
    {DMA_RS_TGIB_6,            0x097, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIC_6,            0x09B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGID_6,            0x09F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIA_7,            0x0A3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Multi Function Timer Pulse Unit3a channel 7 */
    {DMA_RS_TGIB_7,            0x0A7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIC_7,            0x0AB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGID_7,            0x0AF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TCIV_7,            0x0B3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIA_8,            0x0B7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Multi Function Timer Pulse Unit3a channel 8 */
    {DMA_RS_TGIB_8,            0x0BB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGIC_8,            0x0BF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_TGID_8,            0x0C3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_GTCIA_0,           0x0C7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< General Purpose PWM Timer channel 0 */
    {DMA_RS_GTCIB_0,           0x0CB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIC_0,           0x0CF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCID_0,           0x0D3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GDTE_0,            0x0D7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIH_0,           0x0DB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIL_0,           0x0DF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIE_0,           0x0E3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIF_0,           0x0E7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADA_0,         0x0EB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADB_0,         0x0EF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIV_0,           0x0F3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIU_0,           0x0F7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_GTCIA_1,           0x0FB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< General Purpose PWM Timer channel 1 */
    {DMA_RS_GTCIB_1,           0x0FF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIC_1,           0x103, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCID_1,           0x107, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GDTE_1,            0x10B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIH_1,           0x10F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIL_1,           0x113, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIE_1,           0x117, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIF_1,           0x11B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADA_1,         0x11F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADB_1,         0x123, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIV_1,           0x127, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIU_1,           0x12B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_GTCIA_2,           0x12F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< General Purpose PWM Timer channel 2 */
    {DMA_RS_GTCIB_2,           0x133, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIC_2,           0x137, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCID_2,           0x13B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GDTE_2,            0x13F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIH_2,           0x143, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIL_2,           0x147, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIE_2,           0x14B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIF_2,           0x14F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADA_2,         0x153, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADB_2,         0x157, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIV_2,           0x15B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIU_2,           0x15F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_GTCIA_3,           0x163, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< General Purpose PWM Timer channel 3 */
    {DMA_RS_GTCIB_3,           0x167, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIC_3,           0x16B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCID_3,           0x16F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GDTE_3,            0x173, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIH_3,           0x177, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIL_3,           0x17B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIE_3,           0x17F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIF_3,           0x183, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADA_3,         0x187, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADB_3,         0x18B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIV_3,           0x18F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIU_3,           0x193, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_GTCIA_4,           0x197, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< General Purpose PWM Timer channel 4 */
    {DMA_RS_GTCIB_4,           0x19B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIC_4,           0x19F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCID_4,           0x1A3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GDTE_4,            0x1A7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIH_4,           0x1AB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIL_4,           0x1AF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIE_4,           0x1B3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIF_4,           0x1B7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADA_4,         0x1BB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADB_4,         0x1BF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIV_4,           0x1C3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIU_4,           0x1C7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_GTCIA_5,           0x1CB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< General Purpose PWM Timer channel 5 */
    {DMA_RS_GTCIB_5,           0x1CF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIC_5,           0x1D3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCID_5,           0x1D7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GDTE_5,            0x1DB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIH_5,           0x1DF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIL_5,           0x1E3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIE_5,           0x1E7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIF_5,           0x1EB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADA_5,         0x1EF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADB_5,         0x1F3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIV_5,           0x1F7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIU_5,           0x1FB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_GTCIA_6,           0x1FF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< General Purpose PWM Timer channel 6 */
    {DMA_RS_GTCIB_6,           0x203, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIC_6,           0x207, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCID_6,           0x20B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GDTE_6,            0x20F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIH_6,           0x213, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIL_6,           0x217, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIE_6,           0x21B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIF_6,           0x21F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADA_6,         0x223, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADB_6,         0x227, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIV_6,           0x22B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIU_6,           0x22F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_GTCIA_7,           0x233, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< General Purpose PWM Timer channel 7 */
    {DMA_RS_GTCIB_7,           0x237, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIC_7,           0x23B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCID_7,           0x23F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GDTE_7,            0x243, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIH_7,           0x247, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIL_7,           0x24B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIE_7,           0x24F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIF_7,           0x253, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADA_7,         0x257, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIADB_7,         0x25B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIV_7,           0x25F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_GTCIU_7,           0x263, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_S12ADI_0,          0x267, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< A/D Converter */
    {DMA_RS_S12GBADI_0,        0x26B, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },
    {DMA_RS_S12GCADI_0,        0x26F, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },

    {DMA_RS_INT_SSIF_DMA_RX_0, 0x272, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Sound Interface Channel 0 */
    {DMA_RS_INT_SSIF_DMA_TX_0, 0x271, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_INT_SSIF_DMA_RX_1, 0x276, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Sound Interface Channel 1 */
    {DMA_RS_INT_SSIF_DMA_TX_1, 0x275, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_INT_SSIF_DMA_RX_2, 0x27B, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Sound Interface Channel 2 */  /* possible Data Sheet error */
    {DMA_RS_INT_SSIF_DMA_TX_2, 0x27B, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },                                                /* possible Data Sheet error */
    {DMA_RS_INT_SSIF_DMA_RX_3, 0x27E, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Sound Interface Channel 3 */
    {DMA_RS_INT_SSIF_DMA_TX_3, 0x27D, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },

    {DMA_RS_SPDIFTXI,          0x283, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Renesas SPDIF Interface */
    {DMA_RS_SPDIFRXI,          0x287, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },

    {DMA_RS_INTRIIC_RI0,       0x28A, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< IIC Channel 0 */
    {DMA_RS_INTRIIC_TI0,       0x289, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_INTRIIC_RI1,       0x28E, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< IIC Channel 1 */
    {DMA_RS_INTRIIC_TI1,       0x28D, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_INTRIIC_RI2,       0x292, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< IIC Channel 2 */
    {DMA_RS_INTRIIC_TI2,       0x291, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_INTRIIC_RI3,       0x296, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< IIC Channel 3 */
    {DMA_RS_INTRIIC_TI3,       0x295, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },

    {DMA_RS_RXI0,              0x29A, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Communication Interface With FIFO Channel 0 */
    {DMA_RS_TXI0,              0x299, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_RXI1,              0x29E, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Communication Interface With FIFO Channel 1 */
    {DMA_RS_TXI1,              0x29D, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_RXI2,              0x2A2, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Communication Interface With FIFO Channel 2 */
    {DMA_RS_TXI2,              0x2A1, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_RXI3,              0x2A6, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Communication Interface With FIFO Channel 3 */
    {DMA_RS_TXI3,              0x2A5, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_RXI4,              0x2AA, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Communication Interface With FIFO Channel 4 */
    {DMA_RS_TXI4,              0x2A9, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_SET_REQD_DST  },

    {DMA_RS_RXF_DMA0,          0x2AF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< CAN Interface */
    {DMA_RS_RXF_DMA1,          0x2B3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_RXF_DMA2,          0x2B7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_RXF_DMA3,          0x2BB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_RXF_DMA4,          0x2BF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_RXF_DMA5,          0x2C3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_RXF_DMA6,          0x2C7, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_RXF_DMA7,          0x2CB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_COM_DMA0,          0x2CF, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},
    {DMA_RS_COM_DMA1,          0x2D3, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_EDGE , DMAC_PRV_CHCFG_REQD_UNDEFINED},

    {DMA_RS_SPRI0,             0x2D6, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Peripheral Interface Channel 0 */
    {DMA_RS_SPTI0,             0x2D5, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_SPRI1,             0x2DA, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Peripheral Interface Channel 1 */
    {DMA_RS_SPTI1,             0x2D9, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_SPRI2,             0x2DE, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Peripheral Interface Channel 2 */
    {DMA_RS_SPTI2,             0x2DD, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },

    {DMA_RS_SCI_RXI0,          0x2E2, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Communication Interface channel 0 */
    {DMA_RS_SCI_TXI0,          0x2E1, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_SCI_RXI1,          0x2E6, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Serial Communication Interface channel 1 */
    {DMA_RS_SCI_TXI1,          0x2E5, DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE, DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },

    {DMA_RS_IPLS,              0x2EB, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< Ethernet MAC Controller */

    {DMA_RS_TILE_0_PAFI,       0x3CE, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Dynamic Reconfigurable Processor Tile 0 */
    {DMA_RS_TILE_0_PAEI,       0x3CD, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_TILE_1_PAFI,       0x3D2, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Dynamic Reconfigurable Processor Tile 1 */
    {DMA_RS_TILE_1_PAEI,       0x3D1, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_TILE_2_PAFI,       0x3D6, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Dynamic Reconfigurable Processor Tile 2 */
    {DMA_RS_TILE_2_PAEI,       0x3D5, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_TILE_3_PAFI,       0x3DA, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Dynamic Reconfigurable Processor Tile 3 */
    {DMA_RS_TILE_3_PAEI,       0x3D9, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_TILE_4_PAFI,       0x3DE, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Dynamic Reconfigurable Processor Tile 4 */
    {DMA_RS_TILE_4_PAEI,       0x3DD, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },
    {DMA_RS_TILE_5_PAFI,       0x3E2, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_SRC  },      /*!< Dynamic Reconfigurable Processor Tile 5 */
    {DMA_RS_TILE_5_PAEI,       0x3E1, DMAC_PRV_CHCFG_SET_AM_LEVEL    , DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_SET_REQD_DST  },

    {DREQ0,                    0x003, 0                              , 0                           , DMAC_PRV_CHCFG_REQD_UNDEFINED},      /*!< External Request */

    {DMA_MEM_2_MEM,            0x0  , DMAC_PRV_CHCFG_SET_AM_BUS_CYCLE | DMAC_PRV_CHCFG_SET_TM, DMAC_PRV_CHCFG_SET_LVL_LEVEL, DMAC_PRV_CHCFG_REQD_UNDEFINED},    /*!< memory to memory */

	{LAST_RESOURCE_MARKER,     0    , 0                              , 0                           , 0                            },
};

/*************************************************************************
 Structures
*************************************************************************/

typedef struct
{
    void (*p_dmaComplete)();
    void (*p_dmaError)();
} st_channel_settings_t;

/*******************************************************************************
 Private variables
 *******************************************************************************/
/* ====  Prototype function ==== */

static const st_drv_info_t gs_lld_info =
{
    {
        ((R_DRV_DMAC_LLD_VERSION_MAJOR << 16) + R_DRV_DMAC_LLD_VERSION_MINOR)
    },
    R_DRV_DMAC_LLD_BUILD_NUM,
    R_DRV_DMAC_LLD_DRV_NAME
};

/* stores per-channel settings */
static st_channel_settings_t s_channel_settings[DMAC_NUMBER_OF_CHANNELS];

/*******************************************************************************
 Private functions
 *******************************************************************************/

static bool_t validate_settings (const st_r_drv_dmac_channel_config_t *dmac_config);
static uint32_t determine_chcfg_n_value (uint_t channel, const st_r_drv_dmac_channel_config_t *dmac_config,
        st_dma_configuration_t *dma_configuration, uint32_t request_direction, uint8_t register_set);
static uint32_t determine_chext_n_value (uint32_t source_address, uint32_t destination_address);

/*******************************************************************************
 Global variables
 *******************************************************************************/

/* Interrupt handlers table */
/* DMA end interrupt handler for ch0 - ch15 */
static const IRQHandler gs_dma_int_handler_table[DMAC_NUMBER_OF_CHANNELS] =
{
    &end0_interrupt_handler, &end1_interrupt_handler, &end2_interrupt_handler, &end3_interrupt_handler,
    &end4_interrupt_handler, &end5_interrupt_handler, &end6_interrupt_handler, &end7_interrupt_handler,
    &end8_interrupt_handler, &end9_interrupt_handler, &end10_interrupt_handler, &end11_interrupt_handler,
    &end12_interrupt_handler, &end13_interrupt_handler, &end14_interrupt_handler, &end15_interrupt_handler
};


/**
 *              validate_settings
 * @brief       validate all of the DMAC configuration settings
 * @param[in]   dmac_config : DMAC configuration settings
 * @retval      true if all settings are valid, false if not
 */
static bool_t validate_settings(const st_r_drv_dmac_channel_config_t *dmac_config)
{
    if (dmac_config->resource >= LAST_RESOURCE_MARKER)
    {
        return false;
    }

    if ((dmac_config->source_width < DMA_DATA_SIZE_MIN) || (dmac_config->source_width > DMA_DATA_SIZE_128))
    {
        return false;
    }

    if ((dmac_config->destination_width < DMA_DATA_SIZE_MIN) || (dmac_config->destination_width > DMA_DATA_SIZE_128))
    {
        return false;
    }

    if (dmac_config->source_address_type > DMA_ADDRESS_FIX)
    {
        return false;
    }

    if (dmac_config->destination_address_type > DMA_ADDRESS_FIX)
    {
        return false;
    }

    if (dmac_config->direction > DMA_REQUEST_DESTINATION)
    {
        return false;
    }

    return true;
}
/******************************************************************************
 * End of function validate_settings
 ******************************************************************************/

/**
 *              R_DMAC_ErrInterruptHandler
 * @brief       DMA error interrupt handler
 *              Notify error information to the module function which called DMA driver
 * @param[in]   int_sense
 * @retval      None
 */
void R_DMAC_ErrInterruptHandler(uint32_t int_sense)
{
    NOT_USED(int_sense);
    uint32_t dstat_er_0_7;
    uint32_t dstat_er_8_15;
    uint32_t channel_mask;
    uint32_t channel_bit;
    uint_t channel;

    /* get error channel number */
    dstat_er_0_7 = gsp_dma_common_reg_addr_table[DMA_CH_0]->dstat_er_0_7;
    dstat_er_8_15 = gsp_dma_common_reg_addr_table[DMA_CH_8]->dstat_er_0_7;

    channel_mask = (dstat_er_0_7 | (dstat_er_8_15 << 8));

    channel_bit = 1u;

    for (channel = 0; channel < DMAC_NUMBER_OF_CHANNELS; channel++)
    {
        /* is there an error on this channel? */
        if ((channel_mask & channel_bit) > 0)
        {
            /* if it's been set, then call the DMA error callback function */
            if (NULL != s_channel_settings[channel].p_dmaError)
            {
                (*s_channel_settings[channel].p_dmaError)();
            }
        }

        channel_bit <<= 1u;
    }
}
/******************************************************************************
 End of function R_DMAC_ErrInterruptHandler
 ******************************************************************************/

/**
 *              R_DMAC_EndHandlerProcess
 * @brief       Carry out DMA end interrupt handler process
 *              Notify DMA transfer finished to the module's notification function
 * @param[in]   channel: channel number
 * @retval      None
 */
static void R_DMAC_EndHandlerProcess(const uint_t channel)
{
    if (NULL != s_channel_settings[channel].p_dmaComplete)
    {
        (*s_channel_settings[channel].p_dmaComplete)();
    }
}
/******************************************************************************
 End of function R_DMAC_EndHandlerProcess
 ******************************************************************************/

/**
 *              end0_interrupt_handler
 * @brief       DMA end interrupt handler for channel 0
 * @param       None
 * @retval      None
 */
static void end0_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_0);
}
/******************************************************************************
 End of function end0_interrupt_handler
 ******************************************************************************/

/**
 *              end1_interrupt_handler
 * @brief       DMA end interrupt handler for channel 1
 * @param       None
 * @retval      None
 */
static void end1_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_1);
}
/******************************************************************************
 End of function end1_interrupt_handler
 ******************************************************************************/

 /**
  *              end2_interrupt_handler
  * @brief       DMA end interrupt handler for channel 2
  * @param       None
  * @retval      None
  */
static void end2_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_2);
}
/******************************************************************************
 End of function end2_interrupt_handler
 ******************************************************************************/

/**
 *              end3_interrupt_handler
 * @brief       DMA end interrupt handler for channel 3
 * @param       None
 * @retval      None
 */
static void end3_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_3);
}
/******************************************************************************
 End of function end3_interrupt_handler
 ******************************************************************************/

/**
 *              end4_interrupt_handler
 * @brief       DMA end interrupt handler for channel 4
 * @param       None
 * @retval      None
 */
static void end4_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_4);
}
/******************************************************************************
 End of function end4_interrupt_handler
 ******************************************************************************/

/**
 *              end5_interrupt_handler
 * @brief       DMA end interrupt handler for channel 5
 * @param       None
 * @retval      None
 */
static void end5_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_5);
}
/******************************************************************************
 End of function end5_interrupt_handler
 ******************************************************************************/

/**
 *              end6_interrupt_handler
 * @brief       DMA end interrupt handler for channel 6
 * @param       None
 * @retval      None
 */
static void end6_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_6);
}
/******************************************************************************
 End of function end6_interrupt_handler
 ******************************************************************************/

/**
 *              end7_interrupt_handler
 * @brief       DMA end interrupt handler for channel 7
 * @param       None
 * @retval      None
 */
static void end7_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_7);
}
/******************************************************************************
 End of function end7_interrupt_handler
 ******************************************************************************/

/**
 *              end8_interrupt_handler
 * @brief       DMA end interrupt handler for channel 8
 * @param       None
 * @retval      None
 */
static void end8_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_8);
}
/******************************************************************************
 End of function end8_interrupt_handler
 ******************************************************************************/

 /**
 *              end9_interrupt_handler
 * @brief       DMA end interrupt handler for channel 9
 * @param       None
 * @retval      None
 */
static void end9_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_9);
}
/******************************************************************************
 End of function end9_interrupt_handler
 ******************************************************************************/

 /**
  *              end10_interrupt_handler
  * @brief       DMA end interrupt handler for channel 10
  * @param       None
  * @retval      None
  */
static void end10_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_10);
}
/******************************************************************************
 End of function end10_interrupt_handler
 ******************************************************************************/

/**
 *              end11_interrupt_handler
 * @brief       DMA end interrupt handler for channel 11
 * @param       None
 * @retval      None
 */
static void end11_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_11);
}
/******************************************************************************
 End of function end11_interrupt_handler
 ******************************************************************************/

/**
 *              end12_interrupt_handler
 * @brief       DMA end interrupt handler for channel 12
 * @param       None
 * @retval      None
 */
static void end12_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_12);
}
/******************************************************************************
 End of function end12_interrupt_handler
 ******************************************************************************/

/**
 *              end13_interrupt_handler
 * @brief       DMA end interrupt handler for channel 13
 * @param       None
 * @retval      None
 */
static void end13_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_13);
}
/******************************************************************************
 End of function end13_interrupt_handler
 ******************************************************************************/

/**
 *              end14_interrupt_handler
 * @brief       DMA end interrupt handler for channel 14
 * @param       None
 * @retval      None
 */
static void end14_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_14);
}
/******************************************************************************
 End of function end14_interrupt_handler
 ******************************************************************************/

/**
 *              end15_interrupt_handler
 * @brief       DMA end interrupt handler for channel 15
 * @param       None
 * @retval      None
 */
static void end15_interrupt_handler(void)
{
    R_DMAC_EndHandlerProcess(DMA_CH_15);
}
/******************************************************************************
 End of function end15_interrupt_handler
 ******************************************************************************/

/**
 * @brief       Get version
 *              Gets the version number of this low-layer driver
 * @param[out]  pinfo: returns the driver information
 * @retval      DRV_SUCCESS Always returned
 */
uint32_t R_DMAC_GetVersion(st_drv_info_t *pinfo)
{
    pinfo->version.sub.major = gs_lld_info.version.sub.major;
    pinfo->version.sub.minor = gs_lld_info.version.sub.minor;
    pinfo->build = gs_lld_info.build;
    pinfo->p_szdriver_name  = gs_lld_info.p_szdriver_name;

    return (DRV_SUCCESS);
}
/******************************************************************************
 * End of function R_DMAC_GetVersion
 ******************************************************************************/

/**
 * @brief      R_DMAC_OpenChannel
 *             Open DMA channel
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @retval     DMAC_SUCCESS  Successful operation
 * @retval     error_code    The error code on failure
 ******************************************************************************/
e_r_drv_dmac_err_t R_DMAC_OpenChannel(uint_t sc_config_index)
{
    /* Smart Configure the channel */
    return R_DMAC_SmartConfigureChannel(sc_config_index, 0);
}
/******************************************************************************
 * End of function R_DMAC_OpenChannel
 ******************************************************************************/

/**
 * @brief      R_DMAC_CloseChannel
 *             Close DMA channel
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @retval     DMAC_SUCCESS  Successful operation
 * @retval     error_code    The error code on failure
 ******************************************************************************/
e_r_drv_dmac_err_t R_DMAC_CloseChannel(uint_t sc_config_index)
{
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;

    /* clear the relevant half of the DMARS register */
    /* high word is odd channels, low word is even channels */
    if ((channel & 1u) > 0)
    {
        (*gsp_dmars_register_addr_table[channel]) &= 0xffffu;
    }
    else
    {
        (*gsp_dmars_register_addr_table[channel]) &= 0xffff0000u;
    }

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_CloseChannel
 ******************************************************************************/

/**
 * @brief      R_DMAC_Enable
 *             Enable (start) a DMA transfer
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @param[in]  restart_flag: true to restart
 * @retval     DMAC_SUCCESS  Successful operation
 * @retval     error_code    The error code on failure
 ******************************************************************************/
e_r_drv_dmac_err_t R_DMAC_Enable(uint_t sc_config_index, bool_t restart_flag)
{
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;

    if (restart_flag)
    {
        /* clear continuous DMA setting */
        gsp_dma_ch_register_addr_table[channel]->chcfg_n &= (~(uint32_t) (DMAC_PRV_CHCFG_SET_RSW | DMAC_PRV_CHCFG_SET_RSEL | DMAC_PRV_CHCFG_SET_REN));
    }

    /* reset DMA */
    gsp_dma_ch_register_addr_table[channel]->chctrl_n = DMAC_PRV_CHCTRL_SET_SWRST;

    /* clear DMA transfer end */
    gsp_dma_ch_register_addr_table[channel]->chcfg_n &= (~((uint32_t) DMAC_PRV_CHCFG_SET_DEM));

    /* enable DMA transfer */
    gsp_dma_ch_register_addr_table[channel]->chctrl_n = DMAC_PRV_CHCTRL_SET_SETEN;

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_Enable
 ******************************************************************************/

/**
 * @brief      R_DMAC_Disable
 *             Disable (stop) a DMA transfer
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @param[out] remaining_data_length: length of untransferred data
 * @retval     DMAC_SUCCESS  Successful operation
 * @retval     error_code    The error code on failure
 ******************************************************************************/
e_r_drv_dmac_err_t R_DMAC_Disable(uint_t sc_config_index, uint32_t *remaining_data_length)
{
    uint32_t stop_wait_count;
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;

    /* stop DMA transfer */
    gsp_dma_ch_register_addr_table[channel]->chctrl_n = DMAC_PRV_CHCTRL_SET_CLREN;

    /* wait DMA stop */
    stop_wait_count = 0;
    while ((0 != (gsp_dma_ch_register_addr_table[channel]->chstat_n & DMAC_PRV_CHSTAT_MASK_TACT)) && (DMAC_PRV_DMA_STOP_WAIT_MAX_CNT > stop_wait_count))
    {
        stop_wait_count++;
    }

    if (DMAC_PRV_DMA_STOP_WAIT_MAX_CNT <= stop_wait_count)
    {
        /* NON_NOTICE_ASSERT: wait count is abnormal value (usually, a count is set to 0 or 1) */
        R_COMPILER_Nop();
    }

    /* get remaining data size */
    *remaining_data_length = gsp_dma_ch_register_addr_table[channel]->crtb_n;

    /* set DMA transfer end interrupt mask */
    gsp_dma_ch_register_addr_table[channel]->chcfg_n |= (uint32_t) DMAC_PRV_CHCFG_SET_DEM;

    /* clear continuous DMA setting */
    gsp_dma_ch_register_addr_table[channel]->chcfg_n &= (~(uint32_t) (DMAC_PRV_CHCFG_SET_RSW | DMAC_PRV_CHCFG_SET_RSEL));

    /* clear TC, END bit */
    gsp_dma_ch_register_addr_table[channel]->chctrl_n = (DMAC_PRV_CHCTRL_SET_CLRTC | DMAC_PRV_CHCTRL_SET_CLREND);

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_Disable
 ******************************************************************************/

/**
 * @brief      R_DMAC_SoftwareTrigger
 *             Trigger a DMA transfer
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @retval     DMAC_SUCCESS  Successful operation
 * @retval     error_code    The error code on failure
 ******************************************************************************/
e_r_drv_dmac_err_t R_DMAC_SoftwareTrigger(uint_t sc_config_index)
{
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;

    gsp_dma_ch_register_addr_table[channel]->chctrl_n = DMAC_PRV_CHCTRL_SET_STG;

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_SoftwareTrigger
 ******************************************************************************/

/**
 * @brief      map_transfer_unit_size_to_reg
 *             map specified data size to register setting value
 * @param[in]  data_size: data size enumeration value
 * @retval     4-bit value for DDS or SDS field in the CHCFG_n register
 ******************************************************************************/
static uint32_t map_transfer_unit_size_to_reg(e_r_drv_dmac_data_size_t data_size)
{
    uint32_t unit_size;

    switch (data_size)
    {
        case DMA_DATA_SIZE_MIN:
        case DMA_DATA_SIZE_1:
        default:
        {
            unit_size = DMAC_PRV_DMA_UNIT_1;
            break;
        }

        case DMA_DATA_SIZE_2:
        {
            unit_size = DMAC_PRV_DMA_UNIT_2;
            break;
        }

        case DMA_DATA_SIZE_4:
        {
            unit_size = DMAC_PRV_DMA_UNIT_4;
            break;
        }

        case DMA_DATA_SIZE_8:
        {
            unit_size = DMAC_PRV_DMA_UNIT_8;
            break;
        }

        case DMA_DATA_SIZE_16:
        {
            unit_size = DMAC_PRV_DMA_UNIT_16;
            break;
        }

        case DMA_DATA_SIZE_32:
        {
            unit_size = DMAC_PRV_DMA_UNIT_32;
            break;
        }

        case DMA_DATA_SIZE_64:
        {
            unit_size = DMAC_PRV_DMA_UNIT_64;
            break;
        }

        case DMA_DATA_SIZE_128:
        case DMA_DATA_SIZE_MAX:
        {
            unit_size = DMAC_PRV_DMA_UNIT_128;
            break;
        }
    }

    return unit_size;
}
/******************************************************************************
 * End of function map_transfer_unit_size_to_reg
 ******************************************************************************/

/**
 * @brief      determine_chcfg_n_value
 *             Put together the value for the CHCFG_n register
 * @param[in]  channel: the DMA channel
 * @param[in]  dmac_config: channel configuration settings
 * @param[in]  dma_configuration: DMA register settings from lookup table
 * @param[in]  request_direction: request direction
 * @param[in]  register_set: the register set (0 or 1)
 * @retval     CHCFG_n value
 ******************************************************************************/
static uint32_t determine_chcfg_n_value(uint_t channel, const st_r_drv_dmac_channel_config_t *dmac_config,
        st_dma_configuration_t *dma_configuration, uint32_t request_direction, uint8_t register_set)
{
    uint32_t chcfg_value;

    chcfg_value = DMAC_PRV_CHCFG_FIXED_VALUE;
    chcfg_value |= ((((uint32_t) dmac_config->destination_address_type) << DMAC_PRV_CHCFG_SHIFT_DAD) & DMAC_PRV_CHCFG_MASK_DAD);                /* destination address incrementing or fixed */
    chcfg_value |= ((((uint32_t) dmac_config->source_address_type) << DMAC_PRV_CHCFG_SHIFT_SAD) & DMAC_PRV_CHCFG_MASK_SAD);                     /* source address incrementing or fixed */
    chcfg_value |= ((map_transfer_unit_size_to_reg(dmac_config->destination_width) << DMAC_PRV_CHCFG_SHIFT_DDS) & DMAC_PRV_CHCFG_MASK_DDS);     /* destination data size */
    chcfg_value |= ((map_transfer_unit_size_to_reg(dmac_config->source_width) << DMAC_PRV_CHCFG_SHIFT_SDS) & DMAC_PRV_CHCFG_MASK_SDS);          /* source data size */
    chcfg_value |= (channel & 0x7u);                                                                                                            /* channel number */
    chcfg_value |= dma_configuration->lvl;                                                                                                      /* DMA level or edge triggered */
    chcfg_value |= dma_configuration->tm_am;                                                                                                    /* transfer mode and ACK mode */
    chcfg_value |= request_direction;                                                                                                           /* request direction */

    if (1 == register_set)
    {
        chcfg_value |= DMAC_PRV_CHCFG_MASK_RSEL;                                                                                                /* select register set */
    }

    return chcfg_value;
}
/******************************************************************************
 * End of function determine_chcfg_n_value
 ******************************************************************************/

/**
 * @brief      determine_chext_n_value
 *             Put together the value for the CHEXT_n register
 * @param[in]  source_address: source address
 * @param[in]  destination_address: destination address
 * @retval     CHEXT_n value
 ******************************************************************************/
static uint32_t determine_chext_n_value(uint32_t source_address, uint32_t destination_address)
{
	NOT_USED(source_address);
	NOT_USED(destination_address);
    uint32_t chext_value;

    chext_value = (DMAC_PRV_CHEXT_SET_DPR_NON_SECURE | DMAC_PRV_CHEXT_SET_SPR_NON_SECURE);

    /* set bus parameter for source */
    if ((DMAC_PRV_DMA_EXTERNAL_BUS_END >= source_address) || ((DMAC_PRV_DMA_EXTERNAL_BUS_MIRROR_START <= source_address) && (DMAC_PRV_DMA_EXTERNAL_BUS_MIRROR_END >= source_address)))
    {
        chext_value |= DMAC_PRV_CHEXT_SET_SCA_NORMAL;
    }
    else
    {
        chext_value |= DMAC_PRV_CHEXT_SET_SCA_STRONG;
    }

    /* set bus parameter for destination */
    if ((DMAC_PRV_DMA_EXTERNAL_BUS_END >= destination_address) || ((DMAC_PRV_DMA_EXTERNAL_BUS_MIRROR_START <= destination_address) && (DMAC_PRV_DMA_EXTERNAL_BUS_MIRROR_END >= destination_address)))
    {
        chext_value |= DMAC_PRV_CHEXT_SET_DCA_NORMAL;
    }
    else
    {
        chext_value |= DMAC_PRV_CHEXT_SET_DCA_STRONG;
    }

    return chext_value;
}
/******************************************************************************
 * End of function determine_chext_n_value
 ******************************************************************************/

/**
 * @brief      R_DMAC_ConfigureChannel
 *             Configure channel
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @param[in]  dmac_config: DMA channel configuration settings
 * @param[in]  register_set: the register set (0 or 1)
 * @retval     DMAC_SUCCESS  Successful operation
 * @retval     error_code    The error code on failure
 ******************************************************************************/
e_r_drv_dmac_err_t R_DMAC_ConfigureChannel(uint_t sc_config_index, const st_r_drv_dmac_channel_config_t *dmac_config, uint8_t register_set)
{
    uint32_t dmars_register_value;
    uint32_t dmars_value;
    uint32_t chcfg_value;
    uint32_t chext_value;
    uint32_t request_direction;
    uint32_t source_address;
    uint32_t destination_address;
    uint32_t count;
    st_dma_configuration_t dma_configuration;
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;

    if (!validate_settings(dmac_config))
    {
        return DMAC_ERR_INVALID_CFG;
    }

    /* cast source address to uint32_t */
    source_address = (uint32_t) dmac_config->source_address;

    /* cast destination address to uint32_t */
    destination_address = (uint32_t) dmac_config->destination_address;
    count = dmac_config->count;

    /* set up source address, destination address, and transfer count */
    if (0 == register_set)
    {
        gsp_dma_ch_register_addr_table[channel]->n0tb_n = count;
        gsp_dma_ch_register_addr_table[channel]->n0sa_n = source_address;
        gsp_dma_ch_register_addr_table[channel]->n0da_n = destination_address;
    }
    else
    {
        gsp_dma_ch_register_addr_table[channel]->n1tb_n = count;
        gsp_dma_ch_register_addr_table[channel]->n1sa_n = source_address;
        gsp_dma_ch_register_addr_table[channel]->n1da_n = destination_address;
    }

    /* look up the configuration from the DMA configuration table */
    dma_configuration = gs_dma_configuration_table[dmac_config->resource];

    /* get the request direction from the configuration table (if specified) */
    if (DMAC_PRV_CHCFG_REQD_UNDEFINED == dma_configuration.reqd)
    {
        /* set the request direction value to the passed value */
        if (DMA_REQUEST_SOURCE == dmac_config->direction)
        {
            request_direction = DMAC_PRV_CHCFG_SET_REQD_SRC;
        }
        else
        {
            request_direction = DMAC_PRV_CHCFG_SET_REQD_DST;
        }
    }
    else
    {
        /* set the request direction value to the configured value */
        request_direction = dma_configuration.reqd;
    }

    chcfg_value = determine_chcfg_n_value(channel, dmac_config, &dma_configuration, request_direction, register_set);
    chext_value = determine_chext_n_value(source_address, destination_address);

    /* set DMARS register value */
    dmars_value = dma_configuration.dmars;
    dmars_register_value = *gsp_dmars_register_addr_table[channel];

    /* set the relevant half of the DMARS register */
    /* high word is odd channels, low word is even channels */
    if ((channel & 1u) > 0)
    {
        dmars_value <<= 16;
        dmars_register_value &= 0xffffu;
    }
    else
    {
        dmars_register_value &= 0xffff0000u;
    }

    dmars_register_value |= dmars_value;
    *gsp_dmars_register_addr_table[channel] = dmars_register_value;

    gsp_dma_ch_register_addr_table[channel]->chcfg_n = chcfg_value;
    gsp_dma_ch_register_addr_table[channel]->chext_n = chext_value;

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_ConfigureChannel
 ******************************************************************************/

/**
 * @brief      R_DMAC_SmartConfigureChannel
 *             Configure channel from Smart Configurator table entry
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @param[in]  register_set: the register set (0 or 1)
 * @retval     DMAC_SUCCESS  Successful operation
 * @retval     error_code    The error code on failure
 ******************************************************************************/
e_r_drv_dmac_err_t R_DMAC_SmartConfigureChannel(uint_t sc_config_index, uint8_t register_set)
{
    uint_t sc_table_size = (sizeof(DMAC_SC_TABLE)) / sizeof(st_r_drv_dmac_sc_config_t);
    uint_t channel;

    /* check that we have a valid Smart Configuration index */
    if (sc_config_index >= sc_table_size)
    {
        return DMAC_ERR_INVALID_CHANNEL;
    }

    channel = DMAC_SC_TABLE[sc_config_index].channel;

    s_channel_settings[channel].p_dmaComplete = DMAC_SC_TABLE[sc_config_index].config.p_dmaComplete;
    s_channel_settings[channel].p_dmaError = DMAC_SC_TABLE[sc_config_index].config.p_dmaError;

    return R_DMAC_ConfigureChannel(sc_config_index, &DMAC_SC_TABLE[sc_config_index].config, register_set);
}
/******************************************************************************
 * End of function R_DMAC_SmartConfigureChannel
 ******************************************************************************/

/**
 * @brief      map_reg_to_transfer_unit_size
 *             map specified data size to register setting value
 * @param[in]  register_field: the register field value
 * @retval     4-bit value for DDS or SDS field in the CHCFG_n register
 ******************************************************************************/
static e_r_drv_dmac_data_size_t map_reg_to_transfer_unit_size(uint32_t register_field)
{
    e_r_drv_dmac_data_size_t dmac_data_size;

    switch (register_field)
    {
        case DMAC_PRV_DMA_UNIT_1:
        default:
        {
            dmac_data_size = DMA_DATA_SIZE_1;
            break;
        }

        case DMAC_PRV_DMA_UNIT_2:
        {
            dmac_data_size = DMA_DATA_SIZE_2;
            break;
        }

        case DMAC_PRV_DMA_UNIT_4:
        {
            dmac_data_size = DMA_DATA_SIZE_4;
            break;
        }

        case DMAC_PRV_DMA_UNIT_8:
        {
            dmac_data_size = DMA_DATA_SIZE_8;
            break;
        }

        case DMAC_PRV_DMA_UNIT_16:
        {
            dmac_data_size = DMA_DATA_SIZE_16;
            break;
        }

        case DMAC_PRV_DMA_UNIT_32:
        {
            dmac_data_size = DMA_DATA_SIZE_32;
            break;
        }

        case DMAC_PRV_DMA_UNIT_64:
        {
            dmac_data_size = DMA_DATA_SIZE_64;
            break;
        }

        case DMAC_PRV_DMA_UNIT_128:
        {
            dmac_data_size = DMA_DATA_SIZE_128;
            break;
        }
    }

    return dmac_data_size;
}
/******************************************************************************
 * End of function map_reg_to_transfer_unit_size
 ******************************************************************************/

/**
 * @brief      R_DMAC_GetChannelConfiguration
 *             Get channel configuration
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @param[out] dmac_config: channel configuration settings
 * @retval     DMAC_SUCCESS  Successful operation
 * @retval     error_code    The error code on failure
 ******************************************************************************/
e_r_drv_dmac_err_t R_DMAC_GetChannelConfiguration(uint_t sc_config_index, st_r_drv_dmac_channel_config_t *dmac_config)
{
    uint32_t i;
    uint32_t dmars_register_value;
    uint32_t chcfg_register_value;
    uint32_t register_set_select;
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;

    /* read the DMARS register value */
    dmars_register_value = *gsp_dmars_register_addr_table[channel];

    /* get the relevant half of the DMARS register */
    /* high word is odd channels, low word is even channels */
    if ((channel & 1u) > 0)
    {
        dmars_register_value >>= 16;
    }

    dmars_register_value &= 0xffffu;

    /* set the resource to an invalid value so that it can be determined whether or not it's been found */
    dmac_config->resource = (e_r_drv_dmac_xfer_resource_t) -1;

    /* use the DMARS value to look up the resource */
    for (i = 0; LAST_RESOURCE_MARKER != gs_dma_configuration_table[i].resource; i++)
    {
        if (gs_dma_configuration_table[i].dmars == dmars_register_value)
        {
            dmac_config->resource = gs_dma_configuration_table[i].resource;
            break;
        }
    }

    chcfg_register_value = gsp_dma_ch_register_addr_table[channel]->chcfg_n;

    /* the register value maps directly on to the enumeration */
    dmac_config->destination_address_type = (e_r_drv_dmac_address_type_t) ((chcfg_register_value & DMAC_PRV_CHCFG_MASK_DAD) >> DMAC_PRV_CHCFG_SHIFT_DAD);   /* destination address incrementing or fixed */

    /* the register value maps directly on to the enumeration */
    dmac_config->source_address_type = (e_r_drv_dmac_address_type_t) ((chcfg_register_value & DMAC_PRV_CHCFG_MASK_SAD) >> DMAC_PRV_CHCFG_SHIFT_SAD);        /* source address incrementing or fixed */

    dmac_config->destination_width = map_reg_to_transfer_unit_size((chcfg_register_value & DMAC_PRV_CHCFG_MASK_DDS) >> DMAC_PRV_CHCFG_SHIFT_DDS);           /* destination data size */
    dmac_config->source_width = map_reg_to_transfer_unit_size((chcfg_register_value & DMAC_PRV_CHCFG_MASK_SDS) >> DMAC_PRV_CHCFG_SHIFT_SDS);                /* source data size */

    /* get the request direction */
    if ((chcfg_register_value & DMAC_PRV_CHCFG_SET_REQD_DST) > 0)
    {
        dmac_config->direction = DMA_REQUEST_DESTINATION;
    }
    else
    {
        dmac_config->direction = DMA_REQUEST_SOURCE;
    }

    /* get register set selection */
    register_set_select = chcfg_register_value & DMAC_PRV_CHCFG_MASK_RSEL;

    /* get source address, destination address, and transfer count */
    if (0 == register_set_select)
    {
        dmac_config->count = gsp_dma_ch_register_addr_table[channel]->n0tb_n;
        dmac_config->source_address = (void *) gsp_dma_ch_register_addr_table[channel]->n0sa_n;
        dmac_config->destination_address = (void *) gsp_dma_ch_register_addr_table[channel]->n0da_n;
    }
    else
    {
        dmac_config->count = gsp_dma_ch_register_addr_table[channel]->n1tb_n;
        dmac_config->source_address = (void *) gsp_dma_ch_register_addr_table[channel]->n1sa_n;
        dmac_config->destination_address = (void *) gsp_dma_ch_register_addr_table[channel]->n1da_n;
    }

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_GetChannelConfiguration
 ******************************************************************************/

/**
 * @brief   Hardware initialise
 *          Initialise the hardware
 * @retval  DMAC_SUCCESS Always returned
 */
e_r_drv_dmac_err_t R_DMAC_HWInitialise(void)
{
    uint_t channel;

    for (channel = 0; channel < DMAC_NUMBER_OF_CHANNELS; channel++)
    {
        gsp_dma_ch_register_addr_table[channel]->n0sa_n = DMAC_PRV_N0SA_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->n1sa_n = DMAC_PRV_N1SA_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->n0da_n = DMAC_PRV_N0DA_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->n1da_n = DMAC_PRV_N1DA_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->n0tb_n = DMAC_PRV_N0TB_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->n1tb_n = DMAC_PRV_N1TB_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->chctrl_n = DMAC_PRV_CHCTRL_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->chcfg_n = DMAC_PRV_CHCFG_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->chitvl_n = DMAC_PRV_CHITVL_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->chext_n = DMAC_PRV_CHEXT_INIT_VALUE;
        gsp_dma_ch_register_addr_table[channel]->nxla_n = DMAC_PRV_NXLA_INIT_VALUE;
        *gsp_dmars_register_addr_table[channel] = DMAC_PRV_DMARS_INIT_VALUE;
    }

    /* initialise common register for channel 0 - 7 */
    /* set interrupt output : pulse, set round robin mode */
    gsp_dma_common_reg_addr_table[DMA_CH_0]->dctrl_0_7 = DMAC_PRV_DCTRL_INIT_VALUE;

    /* initialise common register for channel 8 - 15 */
    /* set interrupt output : pulse, set round robin mode */
    gsp_dma_common_reg_addr_table[DMA_CH_8]->dctrl_0_7 = DMAC_PRV_DCTRL_INIT_VALUE;

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_HWInitialise
 ******************************************************************************/

/**
 * @brief   Initialise DMA channel data
 * @retval  DMAC_SUCCESS always returned
 */
e_r_drv_dmac_err_t R_DMAC_InitialiseData(void)
{
    uint_t channel;

    for (channel = 0; channel < DMAC_NUMBER_OF_CHANNELS; channel++)
    {
        /* initialise channel configuration data */
        s_channel_settings[channel].p_dmaComplete = NULL;
        s_channel_settings[channel].p_dmaError = NULL;
    }

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_InitialiseData
 ******************************************************************************/

/**
 * @brief      Set the DMA complete callback for a configuration
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @param[out] p_dmaComplete: pointer to DMA complete callback function
 * @retval     DMAC_SUCCESS
 */
e_r_drv_dmac_err_t R_DMAC_SetDmaCompleteCallback(uint_t sc_config_index, void (*p_dmaComplete)())
{
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;
    s_channel_settings[channel].p_dmaComplete = p_dmaComplete;

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_SetDmaCompleteCallback
 ******************************************************************************/

/**
 * @brief      Set the DMA error callback for a configuration
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @param[out] p_dmaError: pointer to DMA error callback function
 * @retval     DMAC_SUCCESS
 */
e_r_drv_dmac_err_t R_DMAC_SetDmaErrorCallback(uint_t sc_config_index, void (*p_dmaError)())
{
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;
    s_channel_settings[channel].p_dmaError = p_dmaError;

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_SetDmaErrorCallback
 ******************************************************************************/

/**
 * @brief      Get the DMA complete callback for a configuration
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @param[out] p_dmaComplete: pointer to DMA complete callback function pointer
 * @retval     DMAC_SUCCESS
 */
e_r_drv_dmac_err_t R_DMAC_GetDmaCompleteCallback(uint_t sc_config_index, void (**p_dmaComplete)())
{
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;
    *p_dmaComplete = s_channel_settings[channel].p_dmaComplete;

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_GetDmaCompleteCallback
 ******************************************************************************/

/**
 * @brief      Get the DMA error callback for a configuration
 * @param[in]  sc_config_index: the index into the Smart Configuration table
 * @param[out] p_dmaError: pointer to DMA error callback function pointer
 * @retval     DMAC_SUCCESS
 */
e_r_drv_dmac_err_t R_DMAC_GetDmaErrorCallback(uint_t sc_config_index, void (**p_dmaError)())
{
    uint_t channel;

    channel = DMAC_SC_TABLE[sc_config_index].channel;
    *p_dmaError = s_channel_settings[channel].p_dmaError;

    return DMAC_SUCCESS;
}
/******************************************************************************
 * End of function R_DMAC_GetDmaErrorCallback
 ******************************************************************************/

/**
 * @brief   Initialise DMA interrupts
 * @retval  None
 */
void R_DMAC_InitialiseInterrupts(void)
{
    uint_t channel;

    for (channel = 0; channel < DMAC_NUMBER_OF_CHANNELS; channel++)
    {
        /* hook up interrupts for the channel */
        R_INTC_RegistIntFunc(gs_dma_int_num_table[channel], gs_dma_int_handler_table[channel]);

        /* set the priority */
        R_INTC_SetPriority(gs_dma_int_num_table[channel], 28);
    }

    /* hook up error interrupt DMA err 0 */
    R_INTC_RegistIntFunc(INTC_ID_DMAC30_DMAERR0, R_DMAC_ErrInterruptHandler);

    /* hook up error interrupt DMA err 1 */
    R_INTC_RegistIntFunc(INTC_ID_DMAC30_DMAERR1, R_DMAC_ErrInterruptHandler);

    /* set the priority DMA err 0 */
    R_INTC_SetPriority(INTC_ID_DMAC30_DMAERR0, 28);

    /* set the priority DMA err 1 */
    R_INTC_SetPriority(INTC_ID_DMAC30_DMAERR1, 28);
}
/******************************************************************************
 * End of function R_DMAC_InitialiseInterrupts
 ******************************************************************************/

/**
 * @brief   Hardware uninitialise
 *          Uninitialise the hardware
 * @retval  DMAC_SUCCESS Always returned
 */
e_r_drv_dmac_err_t R_DMAC_HWUninitialise(void)
{
    uint_t channel;

    /* clear DMA registers */
    for (channel = 0; channel < DMAC_NUMBER_OF_CHANNELS; channel++)
    {
        gsp_dma_ch_register_addr_table[channel]->chctrl_n = 0;
        gsp_dma_ch_register_addr_table[channel]->chcfg_n = 0;
        gsp_dma_ch_register_addr_table[channel]->n0sa_n = 0;
        gsp_dma_ch_register_addr_table[channel]->n1sa_n = 0;
        gsp_dma_ch_register_addr_table[channel]->n0da_n = 0;
        gsp_dma_ch_register_addr_table[channel]->n1da_n = 0;
        gsp_dma_ch_register_addr_table[channel]->n0tb_n = 0;
        gsp_dma_ch_register_addr_table[channel]->n1tb_n = 0;
        gsp_dma_ch_register_addr_table[channel]->chitvl_n = 0;
        gsp_dma_ch_register_addr_table[channel]->chext_n = 0;
        gsp_dma_ch_register_addr_table[channel]->nxla_n = 0;
        *gsp_dmars_register_addr_table[channel] = 0;
    }

    /* clear common register for channel 0 - 7 */
    gsp_dma_common_reg_addr_table[DMA_CH_0]->dctrl_0_7 = 0;

    /* clear common register for channel 8 - 15 */
    gsp_dma_common_reg_addr_table[DMA_CH_8]->dctrl_0_7 = 0;

    return (DMAC_SUCCESS);
}
/******************************************************************************
 * End of function R_DMAC_HWUninitialise
 ******************************************************************************/

/**
 * @brief   Uninitialise DMA interrupts
 * @retval  None
 */
void R_DMAC_UnInitialiseInterrupts(void)
{
    uint_t channel;

    /* disable DMA interrupts */
    for (channel = 0; channel < DMAC_NUMBER_OF_CHANNELS; channel++)
    {
        R_INTC_Disable(gs_dma_int_num_table[channel]);
        R_INTC_RegistIntFunc(gs_dma_int_num_table[channel], NULL);
    }

    /* disable DMA error interrupt DMA err 0 */
    R_INTC_Disable(INTC_ID_DMAC30_DMAERR0);

    /* disable DMA error interrupt DMA err 1 */
    R_INTC_Disable(INTC_ID_DMAC30_DMAERR1);

    /* unregister DMA interrupt error handler DMA err 0 */
    R_INTC_RegistIntFunc(INTC_ID_DMAC30_DMAERR0, NULL);

    /* unregister DMA interrupt error handler DMA err 1 */
    R_INTC_RegistIntFunc(INTC_ID_DMAC30_DMAERR1, NULL);
}
/******************************************************************************
 * End of function R_DMAC_UnInitialiseInterrupts
 ******************************************************************************/

/**
 * @brief   Enable interrupts for a DMA channel
 * @retval  None
 */
void R_DMAC_EnableChannelInterrupt(uint_t sc_config_index)
{
    int_t channel;

    /* we need to know the channel number for setting interrupts */
    channel = R_DMAC_GetChannel(sc_config_index);

    /* enable DMA end interrupt */
    if (channel >= 0)
    {
        R_INTC_Enable(gs_dma_int_num_table[channel]);
    }
}
/******************************************************************************
 * End of function R_DMAC_EnableChannelInterrupt
 ******************************************************************************/

/**
 * @brief   Disable interrupts for a DMA channel
 * @retval  None
 */
void R_DMAC_DisableChannelInterrupt(uint_t sc_config_index)
{
    int_t channel;

    /* we need to know the channel number for setting interrupts */
    channel = R_DMAC_GetChannel(sc_config_index);

    /* enable DMA end interrupt */
    if (channel >= 0)
    {
        R_INTC_Disable(gs_dma_int_num_table[channel]);
    }
}
/******************************************************************************
 * End of function R_DMAC_DisableChannelInterrupt
 ******************************************************************************/

/**
 * @brief R_DMAC_GetChannel Converts SC table index into DMAC channel
 *
 * This function interrogates SC table to determine which
 * channel is described by sc_config
 *
 * @param[in] sc_config_index Configuration ID
 *
 * @retval  =>0: Corresponding DMAC channel
 * @retval  -1: DRV_ERROR
 */
int_t R_DMAC_GetChannel(uint_t sc_config_index)
{
    int_t ret = DRV_ERROR;
    uint_t sc_table_size = (sizeof(DMAC_SC_TABLE)) / sizeof(st_r_drv_dmac_sc_config_t);

    /* Perform range check of sc_config to determine if index is available in st_r_drv_scifa_sc_config_t */
    if (sc_config_index < sc_table_size)
    {
        ret = (int_t) DMAC_SC_TABLE[sc_config_index].channel;
    }

    return (ret);
}
/*******************************************************************************
 End of function R_DMAC_GetChannel
 ******************************************************************************/

/**
 * R_DMAC_SetNextTransfer
 * @brief      Set addresses and count for next transfer
 * @param[in]  source_address: the source address
 * @param[in]  destination_address: the destination address
 * @param[in]  count: data length
 * @retval     DMAC_SUCCESS Always returned
 */
e_r_drv_dmac_err_t R_DMAC_SetNextTransfer(uint_t sc_config_index, void *source_address, void *destination_address, uint32_t count)
{
    uint_t channel;
    uint8_t current_register_set;
    uint8_t register_set;

    channel = DMAC_SC_TABLE[sc_config_index].channel;

    /* if count is zero, then we turn off continuous DMA */
    if (0 == count)
    {
        gsp_dma_ch_register_addr_table[channel]->chcfg_n &= (~(DMAC_PRV_CHCFG_SET_REN | DMAC_PRV_CHCFG_SET_RSW));
        return (DMAC_SUCCESS);
    }

    /* select the next register set */
    if ((gsp_dma_ch_register_addr_table[channel]->chcfg_n & DMAC_PRV_CHCFG_MASK_RSEL) > 0)
    {
        current_register_set = 1;
    }
    else
    {
        current_register_set = 0;
    }

    /* switch to the next register set */
    register_set = current_register_set ^ 1;

    /* set up source address, destination address, and transfer count */
    if (0 == register_set)
    {
        gsp_dma_ch_register_addr_table[channel]->n0tb_n = count;
        gsp_dma_ch_register_addr_table[channel]->n0sa_n = (uint32_t) source_address;
        gsp_dma_ch_register_addr_table[channel]->n0da_n = (uint32_t) destination_address;
    }
    else
    {
        gsp_dma_ch_register_addr_table[channel]->n1tb_n = count;
        gsp_dma_ch_register_addr_table[channel]->n1sa_n = (uint32_t) source_address;
        gsp_dma_ch_register_addr_table[channel]->n1da_n = (uint32_t) destination_address;
    }

    /* set REN (Register Set Enable) to continue transfers and RSW (Register Select Switch) to automatically invert the register select bit */
    gsp_dma_ch_register_addr_table[channel]->chcfg_n |= (DMAC_PRV_CHCFG_SET_REN | DMAC_PRV_CHCFG_SET_RSW);

    return (DMAC_SUCCESS);
}
/*******************************************************************************
 End of function R_DMAC_SetNextTransfer
 ******************************************************************************/

/* End of File */
