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

#ifndef SDMMC_IOBITMASK_H
#define SDMMC_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define SDMMC_SCC_DTCNTL_TAPEN                                                 (0x00000001u)
#define SDMMC_SCC_DTCNTL_TAPEN_SHIFT                                           (0u)
#define SDMMC_SCC_DTCNTL_TAPNUM                                                (0x00000000u)
#define SDMMC_SCC_DTCNTL_TAPNUM_SHIFT                                          (16u)
#define SDMMC_SCC_TAPSET_TAPSET                                                (0x000000FFu)
#define SDMMC_SCC_TAPSET_TAPSET_SHIFT                                          (0u)
#define SDMMC_SCC_DT2FF_DT2NSSET                                               (0x000000FFu)
#define SDMMC_SCC_DT2FF_DT2NSSET_SHIFT                                         (0u)
#define SDMMC_SCC_DT2FF_DT2NESET                                               (0x0000FF00u)
#define SDMMC_SCC_DT2FF_DT2NESET_SHIFT                                         (8u)
#define SDMMC_SCC_CKSEL_DTSEL                                                  (0x00000001u)
#define SDMMC_SCC_CKSEL_DTSEL_SHIFT                                            (0u)
#define SDMMC_SCC_RVSCNTL_RVSEN                                                (0x00000001u)
#define SDMMC_SCC_RVSCNTL_RVSEN_SHIFT                                          (0u)
#define SDMMC_SCC_RVSCNTL_RVSW                                                 (0x00000002u)
#define SDMMC_SCC_RVSCNTL_RVSW_SHIFT                                           (1u)
#define SDMMC_SCC_RVSCNTL_TAPSEL                                               (0x0000FF00u)
#define SDMMC_SCC_RVSCNTL_TAPSEL_SHIFT                                         (8u)
#define SDMMC_SCC_RVSREQ_REQTAPDWN                                             (0x00000001u)
#define SDMMC_SCC_RVSREQ_REQTAPDWN_SHIFT                                       (0u)
#define SDMMC_SCC_RVSREQ_REQTAPUP                                              (0x00000002u)
#define SDMMC_SCC_RVSREQ_REQTAPUP_SHIFT                                        (1u)
#define SDMMC_SCC_RVSREQ_RVSERR                                                (0x00000004u)
#define SDMMC_SCC_RVSREQ_RVSERR_SHIFT                                          (2u)
#define SDMMC_SCC_SMPCMP_CMPNGD                                                (0x000001FFu)
#define SDMMC_SCC_SMPCMP_CMPNGD_SHIFT                                          (0u)
#define SDMMC_SCC_SMPCMP_CMPNGU                                                (0x01FF0000u)
#define SDMMC_SCC_SMPCMP_CMPNGU_SHIFT                                          (16u)
#define SDMMC_SD_CMD_CF                                                        (0x0000000000000000u)
#define SDMMC_SD_CMD_CF_SHIFT                                                  (0u)
#define SDMMC_SD_CMD_C0                                                        (0x0000000000000040u)
#define SDMMC_SD_CMD_C0_SHIFT                                                  (6u)
#define SDMMC_SD_CMD_C1                                                        (0x0000000000000080u)
#define SDMMC_SD_CMD_C1_SHIFT                                                  (7u)
#define SDMMC_SD_CMD_MD0                                                       (0x0000000000000100u)
#define SDMMC_SD_CMD_MD0_SHIFT                                                 (8u)
#define SDMMC_SD_CMD_MD1                                                       (0x0000000000000200u)
#define SDMMC_SD_CMD_MD1_SHIFT                                                 (9u)
#define SDMMC_SD_CMD_MD2                                                       (0x0000000000000400u)
#define SDMMC_SD_CMD_MD2_SHIFT                                                 (10u)
#define SDMMC_SD_CMD_MD3                                                       (0x0000000000000800u)
#define SDMMC_SD_CMD_MD3_SHIFT                                                 (11u)
#define SDMMC_SD_CMD_MD4                                                       (0x0000000000001000u)
#define SDMMC_SD_CMD_MD4_SHIFT                                                 (12u)
#define SDMMC_SD_CMD_MD5                                                       (0x0000000000002000u)
#define SDMMC_SD_CMD_MD5_SHIFT                                                 (13u)
#define SDMMC_SD_CMD_MD6                                                       (0x0000000000004000u)
#define SDMMC_SD_CMD_MD6_SHIFT                                                 (14u)
#define SDMMC_SD_CMD_MD7                                                       (0x0000000000008000u)
#define SDMMC_SD_CMD_MD7_SHIFT                                                 (15u)
#define SDMMC_SD_ARG_CF                                                        (0x00000000FFFFFF00u)
#define SDMMC_SD_ARG_CF_SHIFT                                                  (0u)
#define SDMMC_SD_ARG1_CF                                                       (0x0000000000000000u)
#define SDMMC_SD_ARG1_CF_SHIFT                                                 (0u)
#define SDMMC_SD_STOP_STP                                                      (0x0000000000000001u)
#define SDMMC_SD_STOP_STP_SHIFT                                                (0u)
#define SDMMC_SD_STOP_SEC                                                      (0x0000000000000100u)
#define SDMMC_SD_STOP_SEC_SHIFT                                                (8u)
#define SDMMC_SD_STOP_HPICMD                                                   (0x0000000000010000u)
#define SDMMC_SD_STOP_HPICMD_SHIFT                                             (16u)
#define SDMMC_SD_STOP_HPIMODE                                                  (0x0000000000020000u)
#define SDMMC_SD_STOP_HPIMODE_SHIFT                                            (17u)
#define SDMMC_SD_SECCNT_CNT                                                    (0x00000000FFFFFFFFu)
#define SDMMC_SD_SECCNT_CNT_SHIFT                                              (0u)
#define SDMMC_SD_RSP10_R                                                       (0xFFFFFFFFFFFFFF00u)
#define SDMMC_SD_RSP10_R_SHIFT                                                 (0u)
#define SDMMC_SD_RSP1_R                                                        (0x0000000000000000u)
#define SDMMC_SD_RSP1_R_SHIFT                                                  (0u)
#define SDMMC_SD_RSP32_R                                                       (0x0000000000000000u)
#define SDMMC_SD_RSP32_R_SHIFT                                                 (0u)
#define SDMMC_SD_RSP3_R                                                        (0x0000000000000000u)
#define SDMMC_SD_RSP3_R_SHIFT                                                  (0u)
#define SDMMC_SD_RSP54_R                                                       (0x0000000000000000u)
#define SDMMC_SD_RSP54_R_SHIFT                                                 (0u)
#define SDMMC_SD_RSP5_R                                                        (0x0000000000000000u)
#define SDMMC_SD_RSP5_R_SHIFT                                                  (0u)
#define SDMMC_SD_RSP76_R                                                       (0x0000000000000000u)
#define SDMMC_SD_RSP76_R_SHIFT                                                 (0u)
#define SDMMC_SD_RSP7_R                                                        (0x0000000000000000u)
#define SDMMC_SD_RSP7_R_SHIFT                                                  (0u)
#define SDMMC_SD_INFO1_INFO0                                                   (0x0000000000000001u)
#define SDMMC_SD_INFO1_INFO0_SHIFT                                             (0u)
#define SDMMC_SD_INFO1_INFO2                                                   (0x0000000000000004u)
#define SDMMC_SD_INFO1_INFO2_SHIFT                                             (2u)
#define SDMMC_SD_INFO1_INFO3                                                   (0x0000000000000008u)
#define SDMMC_SD_INFO1_INFO3_SHIFT                                             (3u)
#define SDMMC_SD_INFO1_INFO4                                                   (0x0000000000000010u)
#define SDMMC_SD_INFO1_INFO4_SHIFT                                             (4u)
#define SDMMC_SD_INFO1_INFO5                                                   (0x0000000000000020u)
#define SDMMC_SD_INFO1_INFO5_SHIFT                                             (5u)
#define SDMMC_SD_INFO1_INFO7                                                   (0x0000000000000080u)
#define SDMMC_SD_INFO1_INFO7_SHIFT                                             (7u)
#define SDMMC_SD_INFO1_INFO8                                                   (0x0000000000000100u)
#define SDMMC_SD_INFO1_INFO8_SHIFT                                             (8u)
#define SDMMC_SD_INFO1_INFO9                                                   (0x0000000000000200u)
#define SDMMC_SD_INFO1_INFO9_SHIFT                                             (9u)
#define SDMMC_SD_INFO1_INFO10                                                  (0x0000000000000400u)
#define SDMMC_SD_INFO1_INFO10_SHIFT                                            (10u)
#define SDMMC_SD_INFO1_HPIRES                                                  (0x0000000000010000u)
#define SDMMC_SD_INFO1_HPIRES_SHIFT                                            (16u)
#define SDMMC_SD_INFO2_ERR0                                                    (0x0000000000000001u)
#define SDMMC_SD_INFO2_ERR0_SHIFT                                              (0u)
#define SDMMC_SD_INFO2_ERR1                                                    (0x0000000000000002u)
#define SDMMC_SD_INFO2_ERR1_SHIFT                                              (1u)
#define SDMMC_SD_INFO2_ERR2                                                    (0x0000000000000004u)
#define SDMMC_SD_INFO2_ERR2_SHIFT                                              (2u)
#define SDMMC_SD_INFO2_ERR3                                                    (0x0000000000000008u)
#define SDMMC_SD_INFO2_ERR3_SHIFT                                              (3u)
#define SDMMC_SD_INFO2_ERR4                                                    (0x0000000000000010u)
#define SDMMC_SD_INFO2_ERR4_SHIFT                                              (4u)
#define SDMMC_SD_INFO2_ERR5                                                    (0x0000000000000020u)
#define SDMMC_SD_INFO2_ERR5_SHIFT                                              (5u)
#define SDMMC_SD_INFO2_ERR6                                                    (0x0000000000000040u)
#define SDMMC_SD_INFO2_ERR6_SHIFT                                              (6u)
#define SDMMC_SD_INFO2_DAT0                                                    (0x0000000000000080u)
#define SDMMC_SD_INFO2_DAT0_SHIFT                                              (7u)
#define SDMMC_SD_INFO2_BRE                                                     (0x0000000000000100u)
#define SDMMC_SD_INFO2_BRE_SHIFT                                               (8u)
#define SDMMC_SD_INFO2_BWE                                                     (0x0000000000000200u)
#define SDMMC_SD_INFO2_BWE_SHIFT                                               (9u)
#define SDMMC_SD_INFO2_SCLKDIVEN                                               (0x0000000000002000u)
#define SDMMC_SD_INFO2_SCLKDIVEN_SHIFT                                         (13u)
#define SDMMC_SD_INFO2_CBSY                                                    (0x0000000000004000u)
#define SDMMC_SD_INFO2_CBSY_SHIFT                                              (14u)
#define SDMMC_SD_INFO2_ILA                                                     (0x0000000000008000u)
#define SDMMC_SD_INFO2_ILA_SHIFT                                               (15u)
#define SDMMC_SD_INFO1_MASK_IMASK0                                             (0x0000000000000001u)
#define SDMMC_SD_INFO1_MASK_IMASK0_SHIFT                                       (0u)
#define SDMMC_SD_INFO1_MASK_IMASK2                                             (0x0000000000000004u)
#define SDMMC_SD_INFO1_MASK_IMASK2_SHIFT                                       (2u)
#define SDMMC_SD_INFO1_MASK_IMASK3                                             (0x0000000000000008u)
#define SDMMC_SD_INFO1_MASK_IMASK3_SHIFT                                       (3u)
#define SDMMC_SD_INFO1_MASK_IMASK4                                             (0x0000000000000010u)
#define SDMMC_SD_INFO1_MASK_IMASK4_SHIFT                                       (4u)
#define SDMMC_SD_INFO1_MASK_IMASK8                                             (0x0000000000000100u)
#define SDMMC_SD_INFO1_MASK_IMASK8_SHIFT                                       (8u)
#define SDMMC_SD_INFO1_MASK_IMASK9                                             (0x0000000000000200u)
#define SDMMC_SD_INFO1_MASK_IMASK9_SHIFT                                       (9u)
#define SDMMC_SD_INFO1_MASK_IMASK16                                            (0x0000000000010000u)
#define SDMMC_SD_INFO1_MASK_IMASK16_SHIFT                                      (16u)
#define SDMMC_SD_INFO2_MASK_EMASK0                                             (0x0000000000000001u)
#define SDMMC_SD_INFO2_MASK_EMASK0_SHIFT                                       (0u)
#define SDMMC_SD_INFO2_MASK_EMASK1                                             (0x0000000000000002u)
#define SDMMC_SD_INFO2_MASK_EMASK1_SHIFT                                       (1u)
#define SDMMC_SD_INFO2_MASK_EMASK2                                             (0x0000000000000004u)
#define SDMMC_SD_INFO2_MASK_EMASK2_SHIFT                                       (2u)
#define SDMMC_SD_INFO2_MASK_EMASK3                                             (0x0000000000000008u)
#define SDMMC_SD_INFO2_MASK_EMASK3_SHIFT                                       (3u)
#define SDMMC_SD_INFO2_MASK_EMASK4                                             (0x0000000000000010u)
#define SDMMC_SD_INFO2_MASK_EMASK4_SHIFT                                       (4u)
#define SDMMC_SD_INFO2_MASK_EMASK5                                             (0x0000000000000020u)
#define SDMMC_SD_INFO2_MASK_EMASK5_SHIFT                                       (5u)
#define SDMMC_SD_INFO2_MASK_EMASK6                                             (0x0000000000000040u)
#define SDMMC_SD_INFO2_MASK_EMASK6_SHIFT                                       (6u)
#define SDMMC_SD_INFO2_MASK_BMASK0                                             (0x0000000000000100u)
#define SDMMC_SD_INFO2_MASK_BMASK0_SHIFT                                       (8u)
#define SDMMC_SD_INFO2_MASK_BMASK1                                             (0x0000000000000200u)
#define SDMMC_SD_INFO2_MASK_BMASK1_SHIFT                                       (9u)
#define SDMMC_SD_INFO2_MASK_IMASK                                              (0x0000000000008000u)
#define SDMMC_SD_INFO2_MASK_IMASK_SHIFT                                        (15u)
#define SDMMC_SD_CLK_CTRL_DIV                                                  (0x00000000000000FFu)
#define SDMMC_SD_CLK_CTRL_DIV_SHIFT                                            (0u)
#define SDMMC_SD_CLK_CTRL_SCLKEN                                               (0x0000000000000100u)
#define SDMMC_SD_CLK_CTRL_SCLKEN_SHIFT                                         (8u)
#define SDMMC_SD_CLK_CTRL_SDCLKOFFEN                                           (0x0000000000000200u)
#define SDMMC_SD_CLK_CTRL_SDCLKOFFEN_SHIFT                                     (9u)
#define SDMMC_SD_SIZE_LEN                                                      (0x00000000000003FFu)
#define SDMMC_SD_SIZE_LEN_SHIFT                                                (0u)
#define SDMMC_SD_OPTION_CTOP21                                                 (0x0000000000000001u)
#define SDMMC_SD_OPTION_CTOP21_SHIFT                                           (0u)
#define SDMMC_SD_OPTION_CTOP22                                                 (0x0000000000000002u)
#define SDMMC_SD_OPTION_CTOP22_SHIFT                                           (1u)
#define SDMMC_SD_OPTION_CTOP23                                                 (0x0000000000000004u)
#define SDMMC_SD_OPTION_CTOP23_SHIFT                                           (2u)
#define SDMMC_SD_OPTION_CTOP24                                                 (0x0000000000000008u)
#define SDMMC_SD_OPTION_CTOP24_SHIFT                                           (3u)
#define SDMMC_SD_OPTION_TOP24                                                  (0x0000000000000010u)
#define SDMMC_SD_OPTION_TOP24_SHIFT                                            (4u)
#define SDMMC_SD_OPTION_TOP25                                                  (0x0000000000000020u)
#define SDMMC_SD_OPTION_TOP25_SHIFT                                            (5u)
#define SDMMC_SD_OPTION_TOP26                                                  (0x0000000000000040u)
#define SDMMC_SD_OPTION_TOP26_SHIFT                                            (6u)
#define SDMMC_SD_OPTION_TOP27                                                  (0x0000000000000080u)
#define SDMMC_SD_OPTION_TOP27_SHIFT                                            (7u)
#define SDMMC_SD_OPTION_TOUTMASK                                               (0x0000000000000100u)
#define SDMMC_SD_OPTION_TOUTMASK_SHIFT                                         (8u)
#define SDMMC_SD_OPTION_EXTOP                                                  (0x0000000000000200u)
#define SDMMC_SD_OPTION_EXTOP_SHIFT                                            (9u)
#define SDMMC_SD_OPTION_WIDTH8                                                 (0x0000000000002000u)
#define SDMMC_SD_OPTION_WIDTH8_SHIFT                                           (13u)
#define SDMMC_SD_OPTION_WIDTH                                                  (0x0000000000008000u)
#define SDMMC_SD_OPTION_WIDTH_SHIFT                                            (15u)
#define SDMMC_SD_ERR_STS1_E0                                                   (0x0000000000000001u)
#define SDMMC_SD_ERR_STS1_E0_SHIFT                                             (0u)
#define SDMMC_SD_ERR_STS1_E1                                                   (0x0000000000000002u)
#define SDMMC_SD_ERR_STS1_E1_SHIFT                                             (1u)
#define SDMMC_SD_ERR_STS1_E2                                                   (0x0000000000000004u)
#define SDMMC_SD_ERR_STS1_E2_SHIFT                                             (2u)
#define SDMMC_SD_ERR_STS1_E3                                                   (0x0000000000000008u)
#define SDMMC_SD_ERR_STS1_E3_SHIFT                                             (3u)
#define SDMMC_SD_ERR_STS1_E4                                                   (0x0000000000000010u)
#define SDMMC_SD_ERR_STS1_E4_SHIFT                                             (4u)
#define SDMMC_SD_ERR_STS1_E5                                                   (0x0000000000000020u)
#define SDMMC_SD_ERR_STS1_E5_SHIFT                                             (5u)
#define SDMMC_SD_ERR_STS1_E8                                                   (0x0000000000000100u)
#define SDMMC_SD_ERR_STS1_E8_SHIFT                                             (8u)
#define SDMMC_SD_ERR_STS1_E9                                                   (0x0000000000000200u)
#define SDMMC_SD_ERR_STS1_E9_SHIFT                                             (9u)
#define SDMMC_SD_ERR_STS1_E10                                                  (0x0000000000000400u)
#define SDMMC_SD_ERR_STS1_E10_SHIFT                                            (10u)
#define SDMMC_SD_ERR_STS1_E11                                                  (0x0000000000000800u)
#define SDMMC_SD_ERR_STS1_E11_SHIFT                                            (11u)
#define SDMMC_SD_ERR_STS1_E12                                                  (0x0000000000001000u)
#define SDMMC_SD_ERR_STS1_E12_SHIFT                                            (12u)
#define SDMMC_SD_ERR_STS1_E13                                                  (0x0000000000002000u)
#define SDMMC_SD_ERR_STS1_E13_SHIFT                                            (13u)
#define SDMMC_SD_ERR_STS1_E14                                                  (0x0000000000004000u)
#define SDMMC_SD_ERR_STS1_E14_SHIFT                                            (14u)
#define SDMMC_SD_ERR_STS2_E0                                                   (0x0000000000000001u)
#define SDMMC_SD_ERR_STS2_E0_SHIFT                                             (0u)
#define SDMMC_SD_ERR_STS2_E1                                                   (0x0000000000000002u)
#define SDMMC_SD_ERR_STS2_E1_SHIFT                                             (1u)
#define SDMMC_SD_ERR_STS2_E2                                                   (0x0000000000000004u)
#define SDMMC_SD_ERR_STS2_E2_SHIFT                                             (2u)
#define SDMMC_SD_ERR_STS2_E3                                                   (0x0000000000000008u)
#define SDMMC_SD_ERR_STS2_E3_SHIFT                                             (3u)
#define SDMMC_SD_ERR_STS2_E4                                                   (0x0000000000000010u)
#define SDMMC_SD_ERR_STS2_E4_SHIFT                                             (4u)
#define SDMMC_SD_ERR_STS2_E5                                                   (0x0000000000000020u)
#define SDMMC_SD_ERR_STS2_E5_SHIFT                                             (5u)
#define SDMMC_SD_ERR_STS2_E6                                                   (0x0000000000000040u)
#define SDMMC_SD_ERR_STS2_E6_SHIFT                                             (6u)
#define SDMMC_SD_BUF0_BUF                                                      (0xFFFFFFFFFFFFFFFFu)
#define SDMMC_SD_BUF0_BUF_SHIFT                                                (0u)
#define SDMMC_SDIO_MODE_IOMOD                                                  (0x0000000000000001u)
#define SDMMC_SDIO_MODE_IOMOD_SHIFT                                            (0u)
#define SDMMC_SDIO_MODE_RWREQ                                                  (0x0000000000000004u)
#define SDMMC_SDIO_MODE_RWREQ_SHIFT                                            (2u)
#define SDMMC_SDIO_MODE_IOABT                                                  (0x0000000000000100u)
#define SDMMC_SDIO_MODE_IOABT_SHIFT                                            (8u)
#define SDMMC_SDIO_MODE_C52PUB                                                 (0x0000000000000200u)
#define SDMMC_SDIO_MODE_C52PUB_SHIFT                                           (9u)
#define SDMMC_SDIO_INFO1_IOIRQ                                                 (0x0000000000000001u)
#define SDMMC_SDIO_INFO1_IOIRQ_SHIFT                                           (0u)
#define SDMMC_SDIO_INFO1_EXPUB52                                               (0x0000000000004000u)
#define SDMMC_SDIO_INFO1_EXPUB52_SHIFT                                         (14u)
#define SDMMC_SDIO_INFO1_EXWT                                                  (0x0000000000008000u)
#define SDMMC_SDIO_INFO1_EXWT_SHIFT                                            (15u)
#define SDMMC_SDIO_INFO1_MASK_IOMSK                                            (0x0000000000000001u)
#define SDMMC_SDIO_INFO1_MASK_IOMSK_SHIFT                                      (0u)
#define SDMMC_SDIO_INFO1_MASK_MEXPUB52                                         (0x0000000000004000u)
#define SDMMC_SDIO_INFO1_MASK_MEXPUB52_SHIFT                                   (14u)
#define SDMMC_SDIO_INFO1_MASK_MEXWT                                            (0x0000000000008000u)
#define SDMMC_SDIO_INFO1_MASK_MEXWT_SHIFT                                      (15u)
#define SDMMC_CC_EXT_MODE_DMASDRW                                              (0x0000000000000002u)
#define SDMMC_CC_EXT_MODE_DMASDRW_SHIFT                                        (1u)
#define SDMMC_SOFT_RST_SDRST                                                   (0x0000000000000001u)
#define SDMMC_SOFT_RST_SDRST_SHIFT                                             (0u)
#define SDMMC_VERSION_IP                                                       (0x00000000000000FFu)
#define SDMMC_VERSION_IP_SHIFT                                                 (0u)
#define SDMMC_VERSION_UR                                                       (0x000000000000FF00u)
#define SDMMC_VERSION_UR_SHIFT                                                 (8u)
#define SDMMC_HOST_MODE_WMODE                                                  (0x0000000000000001u)
#define SDMMC_HOST_MODE_WMODE_SHIFT                                            (0u)
#define SDMMC_HOST_MODE_ENDIAN                                                 (0x0000000000000002u)
#define SDMMC_HOST_MODE_ENDIAN_SHIFT                                           (1u)
#define SDMMC_HOST_MODE_BUSWIDTH                                               (0x0000000000000100u)
#define SDMMC_HOST_MODE_BUSWIDTH_SHIFT                                         (8u)
#define SDMMC_SDIF_MODE_DDR                                                    (0x0000000000000001u)
#define SDMMC_SDIF_MODE_DDR_SHIFT                                              (0u)
#define SDMMC_SDIF_MODE_NOCHKCR                                                (0x0000000000000100u)
#define SDMMC_SDIF_MODE_NOCHKCR_SHIFT                                          (8u)
#define SDMMC_SD_STATUS_SD_RST                                                 (0x0000000000000002u)
#define SDMMC_SD_STATUS_SD_RST_SHIFT                                           (1u)
#define SDMMC_DM_CM_DTRAN_MODE_BUS_WIDTH                                       (0x0000000000000030u)
#define SDMMC_DM_CM_DTRAN_MODE_BUS_WIDTH_SHIFT                                 (4u)
#define SDMMC_DM_CM_DTRAN_MODE_CH_NUM                                          (0x0000000000030000u)
#define SDMMC_DM_CM_DTRAN_MODE_CH_NUM_SHIFT                                    (16u)
#define SDMMC_DM_CM_DTRAN_CTRL_DM_START                                        (0x0000000000000001u)
#define SDMMC_DM_CM_DTRAN_CTRL_DM_START_SHIFT                                  (0u)
#define SDMMC_DM_CM_RST_SEQRST                                                 (0x0000000000000001u)
#define SDMMC_DM_CM_RST_SEQRST_SHIFT                                           (0u)
#define SDMMC_DM_CM_RST_DTRANRST0                                              (0x0000000000000100u)
#define SDMMC_DM_CM_RST_DTRANRST0_SHIFT                                        (8u)
#define SDMMC_DM_CM_RST_DTRANRST1                                              (0x0000000000000200u)
#define SDMMC_DM_CM_RST_DTRANRST1_SHIFT                                        (9u)
#define SDMMC_DM_CM_INFO1_SEQEND                                               (0x0000000000000001u)
#define SDMMC_DM_CM_INFO1_SEQEND_SHIFT                                         (0u)
#define SDMMC_DM_CM_INFO1_DTRANEND0                                            (0x0000000000010000u)
#define SDMMC_DM_CM_INFO1_DTRANEND0_SHIFT                                      (16u)
#define SDMMC_DM_CM_INFO1_DTRANEND1                                            (0x0000000000100000u)
#define SDMMC_DM_CM_INFO1_DTRANEND1_SHIFT                                      (20u)
#define SDMMC_DM_CM_INFO1_MASK_SEQEND_MASK                                     (0x0000000000000001u)
#define SDMMC_DM_CM_INFO1_MASK_SEQEND_MASK_SHIFT                               (0u)
#define SDMMC_DM_CM_INFO1_MASK_DTRANEND0_MASK                                  (0x0000000000010000u)
#define SDMMC_DM_CM_INFO1_MASK_DTRANEND0_MASK_SHIFT                            (16u)
#define SDMMC_DM_CM_INFO1_MASK_DTRANEND1_MASK                                  (0x0000000000100000u)
#define SDMMC_DM_CM_INFO1_MASK_DTRANEND1_MASK_SHIFT                            (20u)
#define SDMMC_DM_CM_INFO2_SEQERR                                               (0x0000000000000001u)
#define SDMMC_DM_CM_INFO2_SEQERR_SHIFT                                         (0u)
#define SDMMC_DM_CM_INFO2_DTRANERR0                                            (0x0000000000010000u)
#define SDMMC_DM_CM_INFO2_DTRANERR0_SHIFT                                      (16u)
#define SDMMC_DM_CM_INFO2_DTRANERR1                                            (0x0000000000020000u)
#define SDMMC_DM_CM_INFO2_DTRANERR1_SHIFT                                      (17u)
#define SDMMC_DM_CM_INFO2_MASK_SEQERR_MASK                                     (0x0000000000000001u)
#define SDMMC_DM_CM_INFO2_MASK_SEQERR_MASK_SHIFT                               (0u)
#define SDMMC_DM_CM_INFO2_MASK_DTRANERR0_MASK                                  (0x0000000000010000u)
#define SDMMC_DM_CM_INFO2_MASK_DTRANERR0_MASK_SHIFT                            (16u)
#define SDMMC_DM_CM_INFO2_MASK_DTRANERR1_MASK                                  (0x0000000000020000u)
#define SDMMC_DM_CM_INFO2_MASK_DTRANERR1_MASK_SHIFT                            (17u)
#define SDMMC_DM_DTRAN_ADDR_DADDR                                              (0x00000000FFFFFFC0u)
#define SDMMC_DM_DTRAN_ADDR_DADDR_SHIFT                                        (3u)

#endif
