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
 * @file         r_dmac_hld_prv.c
 * @version      1.0
 * @description  DMAC high layer device driver private functions
 ******************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 03.09.2018 1.00    First Release
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_dmac_drv_api.h"
#include "r_dmac_drv_link.h"    /* Link include to low layer driver */

#include "r_dmac_hld_prv.h"

/*****************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 External global variables
 ******************************************************************************/

/**
 *                 r_dmac_hld_prv_dma_init
 * @brief          Initialise the DMAC driver
 * @retval         None
 */
void r_dmac_hld_prv_dma_init (void)
{
    /* initialise data used by the Low Layer Driver */
    R_DMAC_InitialiseData();

    /* initialise the hardware */
    R_DMAC_HWInitialise();

    /* initialise interrupts */
    R_DMAC_InitialiseInterrupts();
}
/***********************************************************************************************************************
 End of function r_dmac_hld_prv_dma_init
 **********************************************************************************************************************/

/**
 *                 r_dmac_hld_prv_dma_uninit
 * @brief          Uninitialise the DMAC driver
 * @retval         None
 */
void r_dmac_hld_prv_dma_uninit (void)
{
    R_DMAC_UnInitialiseInterrupts();

    R_DMAC_HWUninitialise();
}
/***********************************************************************************************************************
 End of function r_dmac_hld_prv_dma_uninit
 **********************************************************************************************************************/

/**
 *                 r_dmac_hld_prv_dma_enable
 * @brief          Enable a DMA transfer
 * @param[in]      sc_config_index: the SC config index
 * @retval         DRV_SUCCESS: Success
 *                 DRV_ERROR:   Failure
 */
e_r_drv_dmac_err_t r_dmac_hld_prv_dma_enable (uint_t sc_config_index)
{
    R_DMAC_EnableChannelInterrupt(sc_config_index);

    return R_DMAC_Enable(sc_config_index, false);
}
/***********************************************************************************************************************
 End of function r_dmac_hld_prv_dma_enable
 **********************************************************************************************************************/

/**
 *                 r_dmac_hld_prv_dma_disable
 * @brief          Disable a DMA transfer
 * @param[in]      sc_config_index: the SC config index
 * @retval         DRV_SUCCESS: Success
 *                 DRV_ERROR:   Failure
 */
e_r_drv_dmac_err_t r_dmac_hld_prv_dma_disable (uint_t sc_config_index, uint32_t *remaining_data_length)
{
    R_DMAC_DisableChannelInterrupt(sc_config_index);

    return R_DMAC_Disable(sc_config_index, remaining_data_length);
}
/***********************************************************************************************************************
 End of function r_dmac_hld_prv_dma_disable
 **********************************************************************************************************************/

/**
 *                 r_dmac_hld_prv_open
 * @brief          Configures a DMA channel according to the Smart Configurator
 * @param[in]      sc_config_index: the SC config index to open
 * @retval         DRV_SUCCESS: Success
 *                 DRV_ERROR:   Failure
 */
int_t r_dmac_hld_prv_open (uint_t sc_config_index)
{
    int_t return_value = DRV_ERROR;

    if (R_DMAC_OpenChannel(sc_config_index) == DMAC_SUCCESS)
    {
        return_value = DRV_SUCCESS;
    }

    return return_value;
}
/***********************************************************************************************************************
 End of function r_dmac_hld_prv_open
 **********************************************************************************************************************/
