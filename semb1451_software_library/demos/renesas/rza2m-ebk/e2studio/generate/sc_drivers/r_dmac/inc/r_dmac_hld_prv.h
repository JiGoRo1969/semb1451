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
 * @file         r_dmac_hld_prv.h
 * @version      1.0
 * @description  DMAC high layer device driver private functions
 ******************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 03.09.2018 1.00    First Release
 ******************************************************************************/


#include "driver.h"

#ifndef DRIVERS_R_DMAC_INC_R_DMAC_HLD_PRV_H_
#define DRIVERS_R_DMAC_INC_R_DMAC_HLD_PRV_H_

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

typedef struct
{
    uint_t open_count;
} st_channel_configuration_t;

typedef struct
{
    bool_t driver_initialised;
    st_channel_configuration_t channel_configuration[DMAC_NUMBER_OF_CHANNELS];
} st_dmac_internal_configuration_t;

/***********************************************************************************************************************
 Function prototypes
 **********************************************************************************************************************/

void r_dmac_hld_prv_dma_init (void);
void r_dmac_hld_prv_dma_error (uint32_t channel_mask);
void r_dmac_hld_prv_dma_uninit (void);
e_r_drv_dmac_err_t r_dmac_hld_prv_dma_enable (uint_t sc_config_index);
e_r_drv_dmac_err_t r_dmac_hld_prv_dma_disable (uint_t sc_config_index, uint32_t *remaining_data_length);
int_t r_dmac_hld_prv_open (uint_t sc_config_index);

#endif /* DRIVERS_R_DMAC_INC_R_DMAC_HLD_PRV_H_ */
