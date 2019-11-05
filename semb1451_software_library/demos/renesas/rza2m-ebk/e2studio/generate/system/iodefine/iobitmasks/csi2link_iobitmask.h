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

#ifndef CSI2LINK_IOBITMASK_H
#define CSI2LINK_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define CSI2LINK_TREF_TREF                                                     (0x00000001u)
#define CSI2LINK_TREF_TREF_SHIFT                                               (0u)
#define CSI2LINK_SRST_SRST                                                     (0x00000001u)
#define CSI2LINK_SRST_SRST_SHIFT                                               (0u)
#define CSI2LINK_PHYCNT_ENABLE_0                                               (0x00000001u)
#define CSI2LINK_PHYCNT_ENABLE_0_SHIFT                                         (0u)
#define CSI2LINK_PHYCNT_ENABLE_1                                               (0x00000002u)
#define CSI2LINK_PHYCNT_ENABLE_1_SHIFT                                         (1u)
#define CSI2LINK_PHYCNT_ENABLECLK                                              (0x00000010u)
#define CSI2LINK_PHYCNT_ENABLECLK_SHIFT                                        (4u)
#define CSI2LINK_PHYCNT_RSTZ                                                   (0x00010000u)
#define CSI2LINK_PHYCNT_RSTZ_SHIFT                                             (16u)
#define CSI2LINK_PHYCNT_SHUTDOWNZ                                              (0x00020000u)
#define CSI2LINK_PHYCNT_SHUTDOWNZ_SHIFT                                        (17u)
#define CSI2LINK_CHKSUM_CRC_EN                                                 (0x00000001u)
#define CSI2LINK_CHKSUM_CRC_EN_SHIFT                                           (0u)
#define CSI2LINK_CHKSUM_ECC_EN                                                 (0x00000002u)
#define CSI2LINK_CHKSUM_ECC_EN_SHIFT                                           (1u)
#define CSI2LINK_VCDT_SEL_DT                                                   (0x0000003Fu)
#define CSI2LINK_VCDT_SEL_DT_SHIFT                                             (0u)
#define CSI2LINK_VCDT_SEL_DT_ON                                                (0x00000040u)
#define CSI2LINK_VCDT_SEL_DT_ON_SHIFT                                          (6u)
#define CSI2LINK_VCDT_SEL_VC                                                   (0x00000300u)
#define CSI2LINK_VCDT_SEL_VC_SHIFT                                             (8u)
#define CSI2LINK_VCDT_VCDT_EN                                                  (0x00008000u)
#define CSI2LINK_VCDT_VCDT_EN_SHIFT                                            (15u)
#define CSI2LINK_FRDT_DT_FE                                                    (0x003F0000u)
#define CSI2LINK_FRDT_DT_FE_SHIFT                                              (16u)
#define CSI2LINK_FRDT_DT_FS                                                    (0x3F000000u)
#define CSI2LINK_FRDT_DT_FS_SHIFT                                              (24u)
#define CSI2LINK_FLD_FLD_EN                                                    (0x00000001u)
#define CSI2LINK_FLD_FLD_EN_SHIFT                                              (0u)
#define CSI2LINK_FLD_FLD_DET_SEL                                               (0x00000030u)
#define CSI2LINK_FLD_FLD_DET_SEL_SHIFT                                         (4u)
#define CSI2LINK_FLD_FLD_NUM                                                   (0xFFFF0000u)
#define CSI2LINK_FLD_FLD_NUM_SHIFT                                             (16u)
#define CSI2LINK_ASTBY_AUTO_STANDBY_EN                                         (0x0000001Fu)
#define CSI2LINK_ASTBY_AUTO_STANDBY_EN_SHIFT                                   (0u)
#define CSI2LINK_ASTBY_VD_MSK_EN                                               (0x00000020u)
#define CSI2LINK_ASTBY_VD_MSK_EN_SHIFT                                         (5u)
#define CSI2LINK_ASTBY_VD_MSK_CYCLE                                            (0x00003F00u)
#define CSI2LINK_ASTBY_VD_MSK_CYCLE_SHIFT                                      (8u)
#define CSI2LINK_LNGDT0_LNGDT0                                                 (0xFFFFFFFFu)
#define CSI2LINK_LNGDT0_LNGDT0_SHIFT                                           (0u)
#define CSI2LINK_LNGDT1_LNGDT1                                                 (0xFFFFFFFFu)
#define CSI2LINK_LNGDT1_LNGDT1_SHIFT                                           (0u)
#define CSI2LINK_INTEN_IEN                                                     (0xFFFFFFFFu)
#define CSI2LINK_INTEN_IEN_SHIFT                                               (0u)
#define CSI2LINK_INTCLOSE_ICL                                                  (0xFFFFFFFFu)
#define CSI2LINK_INTCLOSE_ICL_SHIFT                                            (0u)
#define CSI2LINK_INTSTATE_IST                                                  (0xFFFFFFFFu)
#define CSI2LINK_INTSTATE_IST_SHIFT                                            (0u)
#define CSI2LINK_INTERRSTATE_IEST                                              (0x0000FFFFu)
#define CSI2LINK_INTERRSTATE_IEST_SHIFT                                        (0u)
#define CSI2LINK_SHPDAT_DT                                                     (0x0000003Fu)
#define CSI2LINK_SHPDAT_DT_SHIFT                                               (0u)
#define CSI2LINK_SHPDAT_VC                                                     (0x000000C0u)
#define CSI2LINK_SHPDAT_VC_SHIFT                                               (6u)
#define CSI2LINK_SHPDAT_DATA                                                   (0x00FFFF00u)
#define CSI2LINK_SHPDAT_DATA_SHIFT                                             (8u)
#define CSI2LINK_SHPDAT_ECC                                                    (0xFF000000u)
#define CSI2LINK_SHPDAT_ECC_SHIFT                                              (24u)
#define CSI2LINK_SHPCNT_NUM                                                    (0x0000000Fu)
#define CSI2LINK_SHPCNT_NUM_SHIFT                                              (0u)
#define CSI2LINK_SHPCNT_OVF                                                    (0x00010000u)
#define CSI2LINK_SHPCNT_OVF_SHIFT                                              (16u)
#define CSI2LINK_LINKCNT_REG_MONI_PACT_EN                                      (0x02000000u)
#define CSI2LINK_LINKCNT_REG_MONI_PACT_EN_SHIFT                                (25u)
#define CSI2LINK_LINKCNT_MONITOR_EN                                            (0x80000000u)
#define CSI2LINK_LINKCNT_MONITOR_EN_SHIFT                                      (31u)
#define CSI2LINK_LSWAP_L0SEL                                                   (0x00000003u)
#define CSI2LINK_LSWAP_L0SEL_SHIFT                                             (0u)
#define CSI2LINK_LSWAP_L1SEL                                                   (0x0000000Cu)
#define CSI2LINK_LSWAP_L1SEL_SHIFT                                             (2u)
#define CSI2LINK_PHEERM_ERRCONTROL_0                                           (0x00000001u)
#define CSI2LINK_PHEERM_ERRCONTROL_0_SHIFT                                     (0u)
#define CSI2LINK_PHEERM_ERRCONTROL_1                                           (0x00000002u)
#define CSI2LINK_PHEERM_ERRCONTROL_1_SHIFT                                     (1u)
#define CSI2LINK_PHEERM_ERRESC_0                                               (0x00000100u)
#define CSI2LINK_PHEERM_ERRESC_0_SHIFT                                         (8u)
#define CSI2LINK_PHEERM_ERRESC_1                                               (0x00000200u)
#define CSI2LINK_PHEERM_ERRESC_1_SHIFT                                         (9u)
#define CSI2LINK_PHEERM_CL_ERRCONTROL                                          (0x00001000u)
#define CSI2LINK_PHEERM_CL_ERRCONTROL_SHIFT                                    (12u)
#define CSI2LINK_PHCLM_STOPSTATECLK                                            (0x00000001u)
#define CSI2LINK_PHCLM_STOPSTATECLK_SHIFT                                      (0u)
#define CSI2LINK_PHCLM_RXCLKACTIVEHS                                           (0x00000002u)
#define CSI2LINK_PHCLM_RXCLKACTIVEHS_SHIFT                                     (1u)
#define CSI2LINK_PHCLM_RXULPSCLKNOT                                            (0x00000004u)
#define CSI2LINK_PHCLM_RXULPSCLKNOT_SHIFT                                      (2u)
#define CSI2LINK_PHCLM_ULPSACTIVENOTCLK                                        (0x00000008u)
#define CSI2LINK_PHCLM_ULPSACTIVENOTCLK_SHIFT                                  (3u)
#define CSI2LINK_PHDLM_STOPSTATEDATA_0                                         (0x00000001u)
#define CSI2LINK_PHDLM_STOPSTATEDATA_0_SHIFT                                   (0u)
#define CSI2LINK_PHDLM_STOPSTATEDATA_1                                         (0x00000002u)
#define CSI2LINK_PHDLM_STOPSTATEDATA_1_SHIFT                                   (1u)
#define CSI2LINK_PHDLM_RXULPSESC_0                                             (0x00000100u)
#define CSI2LINK_PHDLM_RXULPSESC_0_SHIFT                                       (8u)
#define CSI2LINK_PHDLM_RXULPSESC_1                                             (0x00000200u)
#define CSI2LINK_PHDLM_RXULPSESC_1_SHIFT                                       (9u)
#define CSI2LINK_PHDLM_ULPSACTIVENOT_0                                         (0x00001000u)
#define CSI2LINK_PHDLM_ULPSACTIVENOT_0_SHIFT                                   (12u)
#define CSI2LINK_PHDLM_ULPSACTIVENOT_1                                         (0x00002000u)
#define CSI2LINK_PHDLM_ULPSACTIVENOT_1_SHIFT                                   (13u)
#define CSI2LINK_PH0M0_DT                                                      (0x0000003Fu)
#define CSI2LINK_PH0M0_DT_SHIFT                                                (0u)
#define CSI2LINK_PH0M0_VC                                                      (0x000000C0u)
#define CSI2LINK_PH0M0_VC_SHIFT                                                (6u)
#define CSI2LINK_PH0M0_WC                                                      (0x00FFFF00u)
#define CSI2LINK_PH0M0_WC_SHIFT                                                (8u)
#define CSI2LINK_PH0M1_PH_CNT                                                  (0x0000FFFFu)
#define CSI2LINK_PH0M1_PH_CNT_SHIFT                                            (0u)
#define CSI2LINK_PH1M0_DT                                                      (0x0000003Fu)
#define CSI2LINK_PH1M0_DT_SHIFT                                                (0u)
#define CSI2LINK_PH1M0_VC                                                      (0x000000C0u)
#define CSI2LINK_PH1M0_VC_SHIFT                                                (6u)
#define CSI2LINK_PH1M0_WC                                                      (0x00FFFF00u)
#define CSI2LINK_PH1M0_WC_SHIFT                                                (8u)
#define CSI2LINK_PH1M1_PH_CNT                                                  (0x0000FFFFu)
#define CSI2LINK_PH1M1_PH_CNT_SHIFT                                            (0u)
#define CSI2LINK_PH2M0_DT                                                      (0x0000003Fu)
#define CSI2LINK_PH2M0_DT_SHIFT                                                (0u)
#define CSI2LINK_PH2M0_VC                                                      (0x000000C0u)
#define CSI2LINK_PH2M0_VC_SHIFT                                                (6u)
#define CSI2LINK_PH2M0_WC                                                      (0x00FFFF00u)
#define CSI2LINK_PH2M0_WC_SHIFT                                                (8u)
#define CSI2LINK_PH2M1_PH_CNT                                                  (0x0000FFFFu)
#define CSI2LINK_PH2M1_PH_CNT_SHIFT                                            (0u)
#define CSI2LINK_PH3M0_DT                                                      (0x0000003Fu)
#define CSI2LINK_PH3M0_DT_SHIFT                                                (0u)
#define CSI2LINK_PH3M0_VC                                                      (0x000000C0u)
#define CSI2LINK_PH3M0_VC_SHIFT                                                (6u)
#define CSI2LINK_PH3M0_WC                                                      (0x00FFFF00u)
#define CSI2LINK_PH3M0_WC_SHIFT                                                (8u)
#define CSI2LINK_PH3M1_PH_CNT                                                  (0x0000FFFFu)
#define CSI2LINK_PH3M1_PH_CNT_SHIFT                                            (0u)
#define CSI2LINK_PHRM0_DT                                                      (0x0000003Fu)
#define CSI2LINK_PHRM0_DT_SHIFT                                                (0u)
#define CSI2LINK_PHRM0_VC                                                      (0x000000C0u)
#define CSI2LINK_PHRM0_VC_SHIFT                                                (6u)
#define CSI2LINK_PHRM0_WC                                                      (0x00FFFF00u)
#define CSI2LINK_PHRM0_WC_SHIFT                                                (8u)
#define CSI2LINK_PHRM0_ECC                                                     (0xFF000000u)
#define CSI2LINK_PHRM0_ECC_SHIFT                                               (24u)
#define CSI2LINK_PHRM1_DT                                                      (0x0000003Fu)
#define CSI2LINK_PHRM1_DT_SHIFT                                                (0u)
#define CSI2LINK_PHRM1_VC                                                      (0x000000C0u)
#define CSI2LINK_PHRM1_VC_SHIFT                                                (6u)
#define CSI2LINK_PHRM1_WC                                                      (0x00FFFF00u)
#define CSI2LINK_PHRM1_WC_SHIFT                                                (8u)
#define CSI2LINK_PHRM1_ECC                                                     (0xFF000000u)
#define CSI2LINK_PHRM1_ECC_SHIFT                                               (24u)
#define CSI2LINK_PHRM2_DT                                                      (0x0000003Fu)
#define CSI2LINK_PHRM2_DT_SHIFT                                                (0u)
#define CSI2LINK_PHRM2_VC                                                      (0x000000C0u)
#define CSI2LINK_PHRM2_VC_SHIFT                                                (6u)
#define CSI2LINK_PHRM2_WC                                                      (0x00FFFF00u)
#define CSI2LINK_PHRM2_WC_SHIFT                                                (8u)
#define CSI2LINK_PHRM2_ECC                                                     (0xFF000000u)
#define CSI2LINK_PHRM2_ECC_SHIFT                                               (24u)
#define CSI2LINK_PHCM0_DT                                                      (0x0000003Fu)
#define CSI2LINK_PHCM0_DT_SHIFT                                                (0u)
#define CSI2LINK_PHCM0_VC                                                      (0x000000C0u)
#define CSI2LINK_PHCM0_VC_SHIFT                                                (6u)
#define CSI2LINK_PHCM0_WC                                                      (0x00FFFF00u)
#define CSI2LINK_PHCM0_WC_SHIFT                                                (8u)
#define CSI2LINK_PHCM0_CAL_PARITY                                              (0xFF000000u)
#define CSI2LINK_PHCM0_CAL_PARITY_SHIFT                                        (24u)
#define CSI2LINK_PHCM1_DT                                                      (0x0000003Fu)
#define CSI2LINK_PHCM1_DT_SHIFT                                                (0u)
#define CSI2LINK_PHCM1_VC                                                      (0x000000C0u)
#define CSI2LINK_PHCM1_VC_SHIFT                                                (6u)
#define CSI2LINK_PHCM1_WC                                                      (0x00FFFF00u)
#define CSI2LINK_PHCM1_WC_SHIFT                                                (8u)
#define CSI2LINK_PHCM1_CAL_PARITY                                              (0xFF000000u)
#define CSI2LINK_PHCM1_CAL_PARITY_SHIFT                                        (24u)
#define CSI2LINK_CRCM0_CAL_CRC                                                 (0x0000FFFFu)
#define CSI2LINK_CRCM0_CAL_CRC_SHIFT                                           (0u)
#define CSI2LINK_CRCM0_CRC                                                     (0xFFFF0000u)
#define CSI2LINK_CRCM0_CRC_SHIFT                                               (16u)
#define CSI2LINK_CRCM1_CAL_CRC                                                 (0x0000FFFFu)
#define CSI2LINK_CRCM1_CAL_CRC_SHIFT                                           (0u)
#define CSI2LINK_CRCM1_CRC                                                     (0xFFFF0000u)
#define CSI2LINK_CRCM1_CRC_SHIFT                                               (16u)
#define CSI2LINK_SERRCNT_ERRSOTHS_CNT                                          (0x000000FFu)
#define CSI2LINK_SERRCNT_ERRSOTHS_CNT_SHIFT                                    (0u)
#define CSI2LINK_SSERRCNT_ERRSOTSYNCHS                                         (0x0000000Fu)
#define CSI2LINK_SSERRCNT_ERRSOTSYNCHS_SHIFT                                   (0u)
#define CSI2LINK_ECCCM_ECC_CRCT_CNT                                            (0x000000FFu)
#define CSI2LINK_ECCCM_ECC_CRCT_CNT_SHIFT                                      (0u)
#define CSI2LINK_ECECM_ECC_ERR_CNT                                             (0x000000FFu)
#define CSI2LINK_ECECM_ECC_ERR_CNT_SHIFT                                       (0u)
#define CSI2LINK_CRCECM_CRC_ERR_CNT                                            (0x000000FFu)
#define CSI2LINK_CRCECM_CRC_ERR_CNT_SHIFT                                      (0u)
#define CSI2LINK_LCNT_LINE_CNT                                                 (0x0000FFFFu)
#define CSI2LINK_LCNT_LINE_CNT_SHIFT                                           (0u)
#define CSI2LINK_LCNTM_MONI_LINECNT                                            (0x0000FFFFu)
#define CSI2LINK_LCNTM_MONI_LINECNT_SHIFT                                      (0u)
#define CSI2LINK_FCNTM_MONI_FCOUNT                                             (0x0000FFFFu)
#define CSI2LINK_FCNTM_MONI_FCOUNT_SHIFT                                       (0u)
#define CSI2LINK_PHYDIM_RXDATAHS_0                                             (0x000000FFu)
#define CSI2LINK_PHYDIM_RXDATAHS_0_SHIFT                                       (0u)
#define CSI2LINK_PHYDIM_RXDATAHS_1                                             (0x0000FF00u)
#define CSI2LINK_PHYDIM_RXDATAHS_1_SHIFT                                       (8u)
#define CSI2LINK_PHYIM_RXSYNCHS_0_CNT                                          (0x0000000Fu)
#define CSI2LINK_PHYIM_RXSYNCHS_0_CNT_SHIFT                                    (0u)
#define CSI2LINK_PHYIM_RXSYNCHS_1_CNT                                          (0x000000F0u)
#define CSI2LINK_PHYIM_RXSYNCHS_1_CNT_SHIFT                                    (4u)
#define CSI2LINK_PHYIM_RXACTIVEHS_0                                            (0x00010000u)
#define CSI2LINK_PHYIM_RXACTIVEHS_0_SHIFT                                      (16u)
#define CSI2LINK_PHYIM_RXACTIVEHS_1                                            (0x00020000u)
#define CSI2LINK_PHYIM_RXACTIVEHS_1_SHIFT                                      (17u)
#define CSI2LINK_PHYIM_RXVALIDHS_0                                             (0x00100000u)
#define CSI2LINK_PHYIM_RXVALIDHS_0_SHIFT                                       (20u)
#define CSI2LINK_PHYIM_RXVALIDHS_1                                             (0x00200000u)
#define CSI2LINK_PHYIM_RXVALIDHS_1_SHIFT                                       (21u)
#define CSI2LINK_PHYIM_RXCLK_CNT                                               (0x80000000u)
#define CSI2LINK_PHYIM_RXCLK_CNT_SHIFT                                         (31u)
#define CSI2LINK_VINDM_CSIR_DAT                                                (0xFFFFFFFFu)
#define CSI2LINK_VINDM_CSIR_DAT_SHIFT                                          (0u)
#define CSI2LINK_VINSM1_CSIR_HD_CNT                                            (0x00000FFFu)
#define CSI2LINK_VINSM1_CSIR_HD_CNT_SHIFT                                      (0u)
#define CSI2LINK_VINSM1_CSIR_VD_CNT                                            (0x0000F000u)
#define CSI2LINK_VINSM1_CSIR_VD_CNT_SHIFT                                      (12u)
#define CSI2LINK_VINSM3_CSIR_PE                                                (0x00000001u)
#define CSI2LINK_VINSM3_CSIR_PE_SHIFT                                          (0u)
#define CSI2LINK_VINSM3_CSIR_PEB                                               (0x000000F0u)
#define CSI2LINK_VINSM3_CSIR_PEB_SHIFT                                         (4u)
#define CSI2LINK_VINSM3_CSIR_FLD                                               (0x00000F00u)
#define CSI2LINK_VINSM3_CSIR_FLD_SHIFT                                         (8u)
#define CSI2LINK_VINSM3_CSIR_TAG                                               (0x00003000u)
#define CSI2LINK_VINSM3_CSIR_TAG_SHIFT                                         (12u)
#define CSI2LINK_VINSM3_CSIR_ERRC                                              (0x00004000u)
#define CSI2LINK_VINSM3_CSIR_ERRC_SHIFT                                        (14u)
#define CSI2LINK_VINSM3_CSIR_ERRE                                              (0x00008000u)
#define CSI2LINK_VINSM3_CSIR_ERRE_SHIFT                                        (15u)
#define CSI2LINK_PHYOM_ENABLE_0                                                (0x00000001u)
#define CSI2LINK_PHYOM_ENABLE_0_SHIFT                                          (0u)
#define CSI2LINK_PHYOM_ENABLE_1                                                (0x00000002u)
#define CSI2LINK_PHYOM_ENABLE_1_SHIFT                                          (1u)
#define CSI2LINK_PHYOM_ENABLECLK                                               (0x00000010u)
#define CSI2LINK_PHYOM_ENABLECLK_SHIFT                                         (4u)
#define CSI2LINK_PHM1_DT                                                       (0x0000003Fu)
#define CSI2LINK_PHM1_DT_SHIFT                                                 (0u)
#define CSI2LINK_PHM1_VC                                                       (0x000000C0u)
#define CSI2LINK_PHM1_VC_SHIFT                                                 (6u)
#define CSI2LINK_PHM1_WC                                                       (0x00FFFF00u)
#define CSI2LINK_PHM1_WC_SHIFT                                                 (8u)
#define CSI2LINK_PHM1_ECC                                                      (0xFF000000u)
#define CSI2LINK_PHM1_ECC_SHIFT                                                (24u)
#define CSI2LINK_PHM2_DT                                                       (0x0000003Fu)
#define CSI2LINK_PHM2_DT_SHIFT                                                 (0u)
#define CSI2LINK_PHM2_VC                                                       (0x000000C0u)
#define CSI2LINK_PHM2_VC_SHIFT                                                 (6u)
#define CSI2LINK_PHM2_WC                                                       (0x00FFFF00u)
#define CSI2LINK_PHM2_WC_SHIFT                                                 (8u)
#define CSI2LINK_PHM2_ECC                                                      (0xFF000000u)
#define CSI2LINK_PHM2_ECC_SHIFT                                                (24u)
#define CSI2LINK_PHM3_DT                                                       (0x0000003Fu)
#define CSI2LINK_PHM3_DT_SHIFT                                                 (0u)
#define CSI2LINK_PHM3_VC                                                       (0x000000C0u)
#define CSI2LINK_PHM3_VC_SHIFT                                                 (6u)
#define CSI2LINK_PHM3_WC                                                       (0x00FFFF00u)
#define CSI2LINK_PHM3_WC_SHIFT                                                 (8u)
#define CSI2LINK_PHM3_ECC                                                      (0xFF000000u)
#define CSI2LINK_PHM3_ECC_SHIFT                                                (24u)
#define CSI2LINK_PHM4_DT                                                       (0x0000003Fu)
#define CSI2LINK_PHM4_DT_SHIFT                                                 (0u)
#define CSI2LINK_PHM4_VC                                                       (0x000000C0u)
#define CSI2LINK_PHM4_VC_SHIFT                                                 (6u)
#define CSI2LINK_PHM4_WC                                                       (0x00FFFF00u)
#define CSI2LINK_PHM4_WC_SHIFT                                                 (8u)
#define CSI2LINK_PHM4_ECC                                                      (0xFF000000u)
#define CSI2LINK_PHM4_ECC_SHIFT                                                (24u)
#define CSI2LINK_PHM5_DT                                                       (0x0000003Fu)
#define CSI2LINK_PHM5_DT_SHIFT                                                 (0u)
#define CSI2LINK_PHM5_VC                                                       (0x000000C0u)
#define CSI2LINK_PHM5_VC_SHIFT                                                 (6u)
#define CSI2LINK_PHM5_WC                                                       (0x00FFFF00u)
#define CSI2LINK_PHM5_WC_SHIFT                                                 (8u)
#define CSI2LINK_PHM5_ECC                                                      (0xFF000000u)
#define CSI2LINK_PHM5_ECC_SHIFT                                                (24u)
#define CSI2LINK_PHM6_DT                                                       (0x0000003Fu)
#define CSI2LINK_PHM6_DT_SHIFT                                                 (0u)
#define CSI2LINK_PHM6_VC                                                       (0x000000C0u)
#define CSI2LINK_PHM6_VC_SHIFT                                                 (6u)
#define CSI2LINK_PHM6_WC                                                       (0x00FFFF00u)
#define CSI2LINK_PHM6_WC_SHIFT                                                 (8u)
#define CSI2LINK_PHM6_ECC                                                      (0xFF000000u)
#define CSI2LINK_PHM6_ECC_SHIFT                                                (24u)
#define CSI2LINK_PHM7_DT                                                       (0x0000003Fu)
#define CSI2LINK_PHM7_DT_SHIFT                                                 (0u)
#define CSI2LINK_PHM7_VC                                                       (0x000000C0u)
#define CSI2LINK_PHM7_VC_SHIFT                                                 (6u)
#define CSI2LINK_PHM7_WC                                                       (0x00FFFF00u)
#define CSI2LINK_PHM7_WC_SHIFT                                                 (8u)
#define CSI2LINK_PHM7_ECC                                                      (0xFF000000u)
#define CSI2LINK_PHM7_ECC_SHIFT                                                (24u)
#define CSI2LINK_PHM8_DT                                                       (0x0000003Fu)
#define CSI2LINK_PHM8_DT_SHIFT                                                 (0u)
#define CSI2LINK_PHM8_VC                                                       (0x000000C0u)
#define CSI2LINK_PHM8_VC_SHIFT                                                 (6u)
#define CSI2LINK_PHM8_WC                                                       (0x00FFFF00u)
#define CSI2LINK_PHM8_WC_SHIFT                                                 (8u)
#define CSI2LINK_PHM8_ECC                                                      (0xFF000000u)
#define CSI2LINK_PHM8_ECC_SHIFT                                                (24u)
#define CSI2LINK_PHYTIM1_T_INIT_SLAVE                                          (0x0000FFFFu)
#define CSI2LINK_PHYTIM1_T_INIT_SLAVE_SHIFT                                    (0u)
#define CSI2LINK_PHYTIM2_TCLK_PREPARE                                          (0x0000001Fu)
#define CSI2LINK_PHYTIM2_TCLK_PREPARE_SHIFT                                    (0u)
#define CSI2LINK_PHYTIM2_TCLK_SETTLE                                           (0x00003F00u)
#define CSI2LINK_PHYTIM2_TCLK_SETTLE_SHIFT                                     (8u)
#define CSI2LINK_PHYTIM2_TCLK_MISS                                             (0x001F0000u)
#define CSI2LINK_PHYTIM2_TCLK_MISS_SHIFT                                       (16u)
#define CSI2LINK_PHYTIM3_THS_PREPARE                                           (0x0000003Fu)
#define CSI2LINK_PHYTIM3_THS_PREPARE_SHIFT                                     (0u)
#define CSI2LINK_PHYTIM3_THS_SETTLE                                            (0x00003F00u)
#define CSI2LINK_PHYTIM3_THS_SETTLE_SHIFT                                      (8u)

#endif
