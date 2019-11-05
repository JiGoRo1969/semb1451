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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/ 
/*******************************************************************************
* Rev: 2.00
* Description : IO bitmask header
*******************************************************************************/

#ifndef LVDS_IOBITMASK_H
#define LVDS_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define LVDS_LVDS_UPDATE_LVDS_UPDATE                                           (0x00010000u)
#define LVDS_LVDS_UPDATE_LVDS_UPDATE_SHIFT                                     (16u)
#define LVDS_LVDSFCL_LVDS_SEL0                                                 (0x0000000Fu)
#define LVDS_LVDSFCL_LVDS_SEL0_SHIFT                                           (0u)
#define LVDS_LVDSFCL_LVDS_SEL1                                                 (0x000000F0u)
#define LVDS_LVDSFCL_LVDS_SEL1_SHIFT                                           (4u)
#define LVDS_LVDSFCL_LVDS_SEL2                                                 (0x00000F00u)
#define LVDS_LVDSFCL_LVDS_SEL2_SHIFT                                           (8u)
#define LVDS_LVDSFCL_SYNC_POL                                                  (0x00C00000u)
#define LVDS_LVDSFCL_SYNC_POL_SHIFT                                            (22u)
#define LVDS_LVDSFCL_SYNC_MODE                                                 (0x10000000u)
#define LVDS_LVDSFCL_SYNC_MODE_SHIFT                                           (28u)
#define LVDS_LCLKSELR_LVDS_CLK_EN                                              (0x00000010u)
#define LVDS_LCLKSELR_LVDS_CLK_EN_SHIFT                                        (4u)
#define LVDS_LCLKSELR_LVDS_ODIV_SET                                            (0x00000300u)
#define LVDS_LCLKSELR_LVDS_ODIV_SET_SHIFT                                      (8u)
#define LVDS_LCLKSELR_LVDSPLL_TST                                              (0x0000FC00u)
#define LVDS_LCLKSELR_LVDSPLL_TST_SHIFT                                        (10u)
#define LVDS_LCLKSELR_LVDS_IN_CLK_SEL                                          (0x03000000u)
#define LVDS_LCLKSELR_LVDS_IN_CLK_SEL_SHIFT                                    (24u)
#define LVDS_LPLLSETR_LVDSPLL_PD                                               (0x00000001u)
#define LVDS_LPLLSETR_LVDSPLL_PD_SHIFT                                         (0u)
#define LVDS_LPLLSETR_LVDSPLL_OD                                               (0x00000030u)
#define LVDS_LPLLSETR_LVDSPLL_OD_SHIFT                                         (4u)
#define LVDS_LPLLSETR_LVDSPLL_RD                                               (0x00000700u)
#define LVDS_LPLLSETR_LVDSPLL_RD_SHIFT                                         (8u)
#define LVDS_LPLLSETR_LVDSPLL_FD                                               (0x007F0000u)
#define LVDS_LPLLSETR_LVDSPLL_FD_SHIFT                                         (16u)

#endif
