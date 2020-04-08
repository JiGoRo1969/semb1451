/******************************************************************************
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
*******************************************************************************
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.

*******************************************************************************
* File Name    : r_devlink_wrapper_cfg.h
* Description  : Define and Configure driver subsystem
*******************************************************************************
* History      : DD.MM.YYYY Ver. Description
*              : 07.02.2018 1.00 First Release
******************************************************************************/

#include "compiler_settings.h"
#include "devlink.h"
#include "r_cpg_drv_api.h"
#include "r_gpio_drv_api.h"
/* This code is auto-generated. Do not edit manually */
#include "r_riic_drv_api.h"
#include "r_adc_drv_api.h"
#include "r_scifa_drv_api.h"
#include "r_dmac_drv_api.h"
#include "r_ostm_drv_api.h"
/* End of modification */
 /* Modified by user, drivers that are not under the control of sc added here */
 /* End of user modification */


#ifndef RENESAS_CONFIGURATION_R_DEVLINK_WRAPPER_CFG_H_
#define RENESAS_CONFIGURATION_R_DEVLINK_WRAPPER_CFG_H_


/* Mount table should only be access directly by this file */
static st_mount_table_t gs_mount_table[] =
{
        /* "cpg" and "gpio" are fixed entry.
         * users can not use these name as configuration name.
         */
        {"cpg",      (st_r_driver_t *)&g_cpg_driver,   R_SC0},
        {"gpio",     (st_r_driver_t *)&g_gpio_driver,  R_SC0},
        /* This code is auto-generated. Do not edit manually */
        { "riic0", (st_r_driver_t *)&g_riic_driver, R_SC0 },
        { "riic2", (st_r_driver_t *)&g_riic_driver, R_SC1 },
        { "riic1", (st_r_driver_t *)&g_riic_driver, R_SC2 },
        { "riic3", (st_r_driver_t *)&g_riic_driver, R_SC3 },
        { "adc0", (st_r_driver_t *)&g_adc_driver, R_SC0 },
        { "scifa4", (st_r_driver_t *)&g_scifa_driver, R_SC0 },
        { "scifa0", (st_r_driver_t *)&g_scifa_driver, R_SC1 },
        { "scifa3", (st_r_driver_t *)&g_scifa_driver, R_SC2 },
        { "scifa1", (st_r_driver_t *)&g_scifa_driver, R_SC3 },
        { "scifa2", (st_r_driver_t *)&g_scifa_driver, R_SC4 },
        { "dmac9_rs_txi4", (st_r_driver_t *)&g_dmac_driver, R_SC0 },
        { "dmac3_rs_txi1", (st_r_driver_t *)&g_dmac_driver, R_SC1 },
        { "dmac7_rs_txi3", (st_r_driver_t *)&g_dmac_driver, R_SC2 },
        { "dmac5_rs_txi2", (st_r_driver_t *)&g_dmac_driver, R_SC3 },
        { "dmac4_rs_rxi2", (st_r_driver_t *)&g_dmac_driver, R_SC4 },
        { "dmac2_rs_rxi1", (st_r_driver_t *)&g_dmac_driver, R_SC5 },
        { "dmac0_rs_rxi0", (st_r_driver_t *)&g_dmac_driver, R_SC6 },
        { "dmac8_rs_rxi4", (st_r_driver_t *)&g_dmac_driver, R_SC7 },
        { "dmac1_rs_txi0", (st_r_driver_t *)&g_dmac_driver, R_SC8 },
        { "dmac6_rs_rxi3", (st_r_driver_t *)&g_dmac_driver, R_SC9 },
        { "ostm1", (st_r_driver_t *)&g_ostm_driver, R_SC0 },
        { "ostm0", (st_r_driver_t *)&g_ostm_driver, R_SC1 },
        /* End of modification */

 /* Modified by user, drivers that are not under the control of sc added here */
		/** SCI Channel 4 Driver added by USER */
		   {"stdin", (st_r_driver_t *)&g_scifa_driver, R_SC4},
		/** SCI Channel 4 Driver added by USER */
		   {"stdout", (st_r_driver_t *)&g_scifa_driver, R_SC4},
		/** SCI Channel 4 Driver added by USER */
		   {"stderr", (st_r_driver_t *)&g_scifa_driver, R_SC4},
 /* End of user modification */

};

#endif /* RENESAS_CONFIGURATION_R_DEVLINK_WRAPPER_CFG_H_ */
