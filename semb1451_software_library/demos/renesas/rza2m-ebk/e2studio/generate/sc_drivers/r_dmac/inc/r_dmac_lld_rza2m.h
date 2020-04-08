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
 * File Name     : r_dmac_lld_rza2m.h
 * Description   : DMAC low layer driver API header
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 31.08.2018 1.00
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_dmac_drv_sc_cfg.h"
#include "r_dmac_drv_api.h"
#include "driver.h"

/* Multiple inclusion prevention macro */
#ifndef R_DMAC_INC_R_DMAC_LLD_RZA2M_H_
#define R_DMAC_INC_R_DMAC_LLD_RZA2M_H_

/*******************************************************************************
 Macro definitions
 *******************************************************************************/

/* uncomment this #define to use the secure DMA module */
/* #define USE_SECURE_MODULE */

/* Version Number of API */

#define R_DRV_DMAC_LLD_DRV_NAME ("LLD EBK_RZA2M DMAC")

/** Major Version Number of API */
#define R_DRV_DMAC_LLD_VERSION_MAJOR      (2)
/** Minor Version Number of API */
#define R_DRV_DMAC_LLD_VERSION_MINOR      (1)
/** Minor Version Number of API */
#define R_DRV_DMAC_LLD_BUILD_NUM          (0)
/** Unique ID */
#define R_DRV_DMAC_LLD_UID                (704)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef void (*IRQHandler)();

/******************************************************************************
 Function prototypes
 ******************************************************************************/
/* ==== API for low layer DMAC driver ==== */

e_r_drv_dmac_err_t R_DMAC_HWInitialise (void);
e_r_drv_dmac_err_t R_DMAC_HWUninitialise (void);
uint32_t R_DMAC_GetVersion (st_drv_info_t *pinfo);
e_r_drv_dmac_err_t R_DMAC_SoftwareTrigger (uint_t sc_config_index);
e_r_drv_dmac_err_t R_DMAC_Enable (uint_t sc_config_index, bool_t restart_flag);
e_r_drv_dmac_err_t R_DMAC_Disable (uint_t sc_config_index, uint32_t *remaining_data_length);
e_r_drv_dmac_err_t R_DMAC_OpenChannel (uint_t sc_config_index);
e_r_drv_dmac_err_t R_DMAC_CloseChannel (uint_t sc_config_index);
e_r_drv_dmac_err_t R_DMAC_ConfigureChannel (uint_t sc_config_index, const st_r_drv_dmac_channel_config_t *dmac_config, uint8_t register_set);
e_r_drv_dmac_err_t R_DMAC_SmartConfigureChannel (uint_t sc_config_index, uint8_t register_set);
e_r_drv_dmac_err_t R_DMAC_GetChannelConfiguration (uint_t sc_config_index, st_r_drv_dmac_channel_config_t *dmac_config);
void R_DMAC_ErrInterruptHandler (uint32_t int_sense);
int_t R_DMAC_GetChannel (uint_t sc_config_index);
e_r_drv_dmac_err_t R_DMAC_SetNextTransfer (uint_t sc_config_index, void *source_address, void *destination_address, uint32_t count);
e_r_drv_dmac_err_t R_DMAC_GetCrtbRegisterValue(uint_t sc_config_index, uint32_t * p_crtb_value);

void R_DMAC_InitialiseInterrupts (void);
void R_DMAC_UnInitialiseInterrupts (void);
void R_DMAC_EnableChannelInterrupt (uint_t sc_config_index);
void R_DMAC_DisableChannelInterrupt (uint_t sc_config_index);

e_r_drv_dmac_err_t R_DMAC_InitialiseData (void);
e_r_drv_dmac_err_t R_DMAC_SetDmaCompleteCallback (uint_t sc_config_index, void (*p_dmaComplete)());
e_r_drv_dmac_err_t R_DMAC_SetDmaErrorCallback (uint_t sc_config_index, void (*p_dmaError)());
e_r_drv_dmac_err_t R_DMAC_GetDmaCompleteCallback (uint_t sc_config_index, void (**p_dmaComplete)());
e_r_drv_dmac_err_t R_DMAC_GetDmaErrorCallback (uint_t sc_config_index, void (**p_dmaError)());

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* R_DMAC_INC_R_DMAC_LLD_RZA2M_H_ */

/* End of File */