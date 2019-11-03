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
 ******************************************************************************/
/*******************************************************************************
 * @file         r_dmac_lld_cfg_rza2m.h
 * @version      1.0
 * @description  DMAC Low layer device driver definitions
 ******************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 31.08.2018 1.00    First Release
 ******************************************************************************/

#include <limits.h>

#ifndef R_DMAC_LLD_CFG_RZA2M_H_
#define R_DMAC_LLD_CFG_RZA2M_H_

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* This define specifies what channels are supported by this driver */
#define DMAC_CFG_LLD_SUPPORTED_CHANNELS     (R_CH0 | R_CH1 | R_CH2 | R_CH3 | R_CH4 | R_CH5 | R_CH6 | R_CH7 | R_CH8 | R_CH9 | R_CH10 | R_CH11 | R_CH12 | R_CH13 | R_CH14 | R_CH15)
#define DMAC_NUMBER_OF_CHANNELS             (16)


typedef enum
{
    DMA_RS_OSTM0TINT,                       /*!< OS Timer channel 0 */
    DMA_RS_OSTM1TINT,                       /*!< OS Timer channel 1 */
    DMA_RS_OSTM2TINT,                       /*!< OS Timer channel 2 */

    DMA_RS_TGIA_0,                          /*!< Multi Function Timer Pulse Unit3a channel 0 */
    DMA_RS_TGIB_0,
    DMA_RS_TGIC_0,
    DMA_RS_TGID_0,
    DMA_RS_TGIA_1,                          /*!< Multi Function Timer Pulse Unit3a channel 1 */
    DMA_RS_TGIB_1,
    DMA_RS_TGIA_2,                          /*!< Multi Function Timer Pulse Unit3a channel 2 */
    DMA_RS_TGIB_2,
    DMA_RS_TGIA_3,                          /*!< Multi Function Timer Pulse Unit3a channel 3 */
    DMA_RS_TGIB_3,
    DMA_RS_TGIC_3,
    DMA_RS_TGID_3,
    DMA_RS_TGIA_4,                          /*!< Multi Function Timer Pulse Unit3a channel 4 */
    DMA_RS_TGIB_4,
    DMA_RS_TGIC_4,
    DMA_RS_TGID_4,
    DMA_RS_TCIV_4,
    DMA_RS_TGIU_5,                          /*!< Multi Function Timer Pulse Unit3a channel 5 */
    DMA_RS_TGIV_5,
    DMA_RS_TGIW_5,
    DMA_RS_TGIA_6,                          /*!< Multi Function Timer Pulse Unit3a channel 6 */
    DMA_RS_TGIB_6,
    DMA_RS_TGIC_6,
    DMA_RS_TGID_6,
    DMA_RS_TGIA_7,                          /*!< Multi Function Timer Pulse Unit3a channel 7 */
    DMA_RS_TGIB_7,
    DMA_RS_TGIC_7,
    DMA_RS_TGID_7,
    DMA_RS_TCIV_7,
    DMA_RS_TGIA_8,                          /*!< Multi Function Timer Pulse Unit3a channel 8 */
    DMA_RS_TGIB_8,
    DMA_RS_TGIC_8,
    DMA_RS_TGID_8,

    DMA_RS_GTCIA_0,                         /*!< General Purpose PWM Timer channel 0 */
    DMA_RS_GTCIB_0,
    DMA_RS_GTCIC_0,
    DMA_RS_GTCID_0,
    DMA_RS_GDTE_0,
    DMA_RS_GTCIH_0,
    DMA_RS_GTCIL_0,
    DMA_RS_GTCIE_0,
    DMA_RS_GTCIF_0,
    DMA_RS_GTCIADA_0,
    DMA_RS_GTCIADB_0,
    DMA_RS_GTCIV_0,
    DMA_RS_GTCIU_0,

    DMA_RS_GTCIA_1,                         /*!< General Purpose PWM Timer channel 1 */
    DMA_RS_GTCIB_1,
    DMA_RS_GTCIC_1,
    DMA_RS_GTCID_1,
    DMA_RS_GDTE_1,
    DMA_RS_GTCIH_1,
    DMA_RS_GTCIL_1,
    DMA_RS_GTCIE_1,
    DMA_RS_GTCIF_1,
    DMA_RS_GTCIADA_1,
    DMA_RS_GTCIADB_1,
    DMA_RS_GTCIV_1,
    DMA_RS_GTCIU_1,

    DMA_RS_GTCIA_2,                         /*!< General Purpose PWM Timer channel 2 */

    DMA_RS_GTCIB_2,
    DMA_RS_GTCIC_2,
    DMA_RS_GTCID_2,
    DMA_RS_GDTE_2,
    DMA_RS_GTCIH_2,
    DMA_RS_GTCIL_2,
    DMA_RS_GTCIE_2,
    DMA_RS_GTCIF_2,
    DMA_RS_GTCIADA_2,
    DMA_RS_GTCIADB_2,
    DMA_RS_GTCIV_2,
    DMA_RS_GTCIU_2,

    DMA_RS_GTCIA_3,                         /*!< General Purpose PWM Timer channel 3 */
    DMA_RS_GTCIB_3,
    DMA_RS_GTCIC_3,
    DMA_RS_GTCID_3,
    DMA_RS_GDTE_3,
    DMA_RS_GTCIH_3,
    DMA_RS_GTCIL_3,
    DMA_RS_GTCIE_3,
    DMA_RS_GTCIF_3,
    DMA_RS_GTCIADA_3,
    DMA_RS_GTCIADB_3,
    DMA_RS_GTCIV_3,
    DMA_RS_GTCIU_3,

    DMA_RS_GTCIA_4,                         /*!< General Purpose PWM Timer channel 4 */
    DMA_RS_GTCIB_4,
    DMA_RS_GTCIC_4,
    DMA_RS_GTCID_4,
    DMA_RS_GDTE_4,
    DMA_RS_GTCIH_4,
    DMA_RS_GTCIL_4,
    DMA_RS_GTCIE_4,
    DMA_RS_GTCIF_4,
    DMA_RS_GTCIADA_4,
    DMA_RS_GTCIADB_4,
    DMA_RS_GTCIV_4,
    DMA_RS_GTCIU_4,

    DMA_RS_GTCIA_5,                         /*!< General Purpose PWM Timer channel 5 */
    DMA_RS_GTCIB_5,
    DMA_RS_GTCIC_5,
    DMA_RS_GTCID_5,
    DMA_RS_GDTE_5,
    DMA_RS_GTCIH_5,
    DMA_RS_GTCIL_5,
    DMA_RS_GTCIE_5,
    DMA_RS_GTCIF_5,
    DMA_RS_GTCIADA_5,
    DMA_RS_GTCIADB_5,
    DMA_RS_GTCIV_5,
    DMA_RS_GTCIU_5,

    DMA_RS_GTCIA_6,                         /*!< General Purpose PWM Timer channel 6 */
    DMA_RS_GTCIB_6,
    DMA_RS_GTCIC_6,
    DMA_RS_GTCID_6,
    DMA_RS_GDTE_6,
    DMA_RS_GTCIH_6,
    DMA_RS_GTCIL_6,
    DMA_RS_GTCIE_6,
    DMA_RS_GTCIF_6,
    DMA_RS_GTCIADA_6,
    DMA_RS_GTCIADB_6,
    DMA_RS_GTCIV_6,
    DMA_RS_GTCIU_6,

    DMA_RS_GTCIA_7,                         /*!< General Purpose PWM Timer channel 7 */
    DMA_RS_GTCIB_7,
    DMA_RS_GTCIC_7,
    DMA_RS_GTCID_7,
    DMA_RS_GDTE_7,
    DMA_RS_GTCIH_7,
    DMA_RS_GTCIL_7,
    DMA_RS_GTCIE_7,
    DMA_RS_GTCIF_7,
    DMA_RS_GTCIADA_7,
    DMA_RS_GTCIADB_7,
    DMA_RS_GTCIV_7,
    DMA_RS_GTCIU_7,

    DMA_RS_S12ADI_0,                        /*!< A/D Converter */
    DMA_RS_S12GBADI_0,
    DMA_RS_S12GCADI_0,

    DMA_RS_INT_SSIF_DMA_RX_0,               /*!< Serial Sound Interface Channel 0 */
    DMA_RS_INT_SSIF_DMA_TX_0,
    DMA_RS_INT_SSIF_DMA_RX_1,               /*!< Serial Sound Interface Channel 1 */
    DMA_RS_INT_SSIF_DMA_TX_1,
    DMA_RS_INT_SSIF_DMA_RX_2,               /*!< Serial Sound Interface Channel 2 */
    DMA_RS_INT_SSIF_DMA_TX_2,
    DMA_RS_INT_SSIF_DMA_RX_3,               /*!< Serial Sound Interface Channel 3 */
    DMA_RS_INT_SSIF_DMA_TX_3,

    DMA_RS_SPDIFTXI,                        /*!< Renesas SPDIF Intercace */
    DMA_RS_SPDIFRXI,

    DMA_RS_INTRIIC_RI0,                     /*!< IIC Channel 0 */
    DMA_RS_INTRIIC_TI0,
    DMA_RS_INTRIIC_RI1,                     /*!< IIC Channel 1 */
    DMA_RS_INTRIIC_TI1,
    DMA_RS_INTRIIC_RI2,                     /*!< IIC Channel 2 */
    DMA_RS_INTRIIC_TI2,
    DMA_RS_INTRIIC_RI3,                     /*!< IIC Channel 3 */
    DMA_RS_INTRIIC_TI3,

    DMA_RS_RXI0,                            /*!< Serial Communication Interface With FIFO Channel 0 */
    DMA_RS_TXI0,
    DMA_RS_RXI1,                            /*!< Serial Communication Interface With FIFO Channel 1 */
    DMA_RS_TXI1,
    DMA_RS_RXI2,                            /*!< Serial Communication Interface With FIFO Channel 2 */
    DMA_RS_TXI2,
    DMA_RS_RXI3,                            /*!< Serial Communication Interface With FIFO Channel 3 */
    DMA_RS_TXI3,
    DMA_RS_RXI4,                            /*!< Serial Communication Interface With FIFO Channel 4 */
    DMA_RS_TXI4,

    DMA_RS_RXF_DMA0,                        /*!< CAN Interface */
    DMA_RS_RXF_DMA1,
    DMA_RS_RXF_DMA2,
    DMA_RS_RXF_DMA3,
    DMA_RS_RXF_DMA4,
    DMA_RS_RXF_DMA5,
    DMA_RS_RXF_DMA6,
    DMA_RS_RXF_DMA7,
    DMA_RS_COM_DMA0,
    DMA_RS_COM_DMA1,

    DMA_RS_SPRI0,                           /*!< Serial Peripheral Interface Channel 0 */
    DMA_RS_SPTI0,
    DMA_RS_SPRI1,                           /*!< Serial Peripheral Interface Channel 1 */
    DMA_RS_SPTI1,
    DMA_RS_SPRI2,                           /*!< Serial Peripheral Interface Channel 2 */
    DMA_RS_SPTI2,

    DMA_RS_SCI_RXI0,                        /*!< Serial Communication Interface channel 0 */
    DMA_RS_SCI_TXI0,
    DMA_RS_SCI_RXI1,                        /*!< Serial Communication Interface channel 1 */
    DMA_RS_SCI_TXI1,

    DMA_RS_IPLS,                            /*!< Ethernet MAC Controller */

    DMA_RS_TILE_0_PAFI,                     /*!< Dynamic Reconfigurable Processor Tile 0 */
    DMA_RS_TILE_0_PAEI,
    DMA_RS_TILE_1_PAFI,                     /*!< Dynamic Reconfigurable Processor Tile 1 */
    DMA_RS_TILE_1_PAEI,
    DMA_RS_TILE_2_PAFI,                     /*!< Dynamic Reconfigurable Processor Tile 2 */
    DMA_RS_TILE_2_PAEI,
    DMA_RS_TILE_3_PAFI,                     /*!< Dynamic Reconfigurable Processor Tile 3 */
    DMA_RS_TILE_3_PAEI,
    DMA_RS_TILE_4_PAFI,                     /*!< Dynamic Reconfigurable Processor Tile 4 */
    DMA_RS_TILE_4_PAEI,
    DMA_RS_TILE_5_PAFI,                     /*!< Dynamic Reconfigurable Processor Tile 5 */
    DMA_RS_TILE_5_PAEI,

    DREQ0,                                  /*!< External Request */

    DMA_MEM_2_MEM,                          /*!< memory to memory */

    LAST_RESOURCE_MARKER,
    MAKE_ENUM_32BIT = INT_MAX               /*!< force enum to 32-bit type for struct alignment */
} e_r_drv_dmac_xfer_resource_t;

#endif /* R_DMAC_LLD_CFG_RZA2M_H_ */
