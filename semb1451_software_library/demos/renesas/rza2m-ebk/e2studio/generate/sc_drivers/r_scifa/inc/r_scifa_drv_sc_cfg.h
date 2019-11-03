/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_scifa_drv_sc_cfg.h
 * Description  : SCIFA driver configuration header for Smart Configurator
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 15.05.2018 1.00    Release
 *              : 10.05.2019 1.01    Fixed issue with channel 0 Baud rate setting
 *              : 23.05.2019 1.02    Restructuring to ensure that other LLDs are only accessed from the SCIFA LLD
 *********************************************************************************************************************/

#include "r_typedefs.h"
#include "r_scifa_drv_link.h"
#include "r_gpio_drv_api.h"
#include "r_gpio_drv_sc_cfg.h"

#ifndef SRC_RENESAS_CONFIG_R_SCIFA_DRV_SC_CFG_H_
#define SRC_RENESAS_CONFIG_R_SCIFA_DRV_SC_CFG_H_
/******************************************************************************************************************//**
 * @ingroup RENESAS_DRIVER_LIBRARY
 * @defgroup RENESAS_SC_CFG Renesas Smart Configuration Settings
 *
 * @section RENESAS_SC_CFG_SUMMARY Summary
 * This configuration file determines the driver configuration making specific
 * settings for the peripheral.
 *
 * This file is normally generated by the Renesas Smart Configurator tool. It is
 * it is possible to generate this file for different configurations, and replace
 * the file in different builds to change the system configuration.
 *
 * Do not edit this file if using SC
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
/* This code is auto-generated. Do not edit manually */
extern void r_callback_scifa0_teie(void);
extern void r_callback_scifa0_reie(void);
extern void r_callback_scifa1_teie(void);
extern void r_callback_scifa1_reie(void);
extern void r_callback_scifa2_teie(void);
extern void r_callback_scifa3_teie(void);
/* End of modification */
#if defined(__cplusplus)
}
#endif

/**
 * @typedef st_r_drv_scifa_sc_config_t Smart Configurator Interface
 */
typedef struct
{
    r_channel_t                     channel;
    scifa_config_t                  config;
    st_r_drv_gpio_pin_init_table_t  pin;
} st_r_drv_scifa_sc_config_t;

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Variable Externs
 *********************************************************************************************************************/

/**
 * @section R_SCIFA_SC_CFG_80_TABLE Smart Configurator settings table.
 */
static const st_r_drv_scifa_sc_config_t SCIFA_SC_TABLE[] =
{
    /* This code is auto-generated. Do not edit manually */
    { 0, 
        {
            SCIFA_MODE_ASYNC, 
            1000000, 
            SCIFA_CLK_SRC_INT_SCK_IN, 
            SCIFA_CLK_16X, 
            SCIFA_DATA_8BIT, 
            SCIFA_PARITY_OFF, 
            SCIFA_EVEN_PARITY, 
            SCIFA_STOPBITS_1, 
            SCIFA_NOISE_CANCEL_DISABLE, 
            SCIFA_LSB_FIRST, 
            SCIFA_LOOPBACK_DISABLE, 
            SCIFA_MODEM_CONTROL_DISABLE, 
            SCIFA_RTS_ACTIVE_TRIGGER_15, 
            1, 
            1, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_TX_DMA_MODE, 
            SCIFA_RX_DMA_MODE, 
            9, 
            10, 
            10, 
            9, 
            r_callback_scifa0_teie, 
            r_callback_scifa0_reie, 
            "dmac1_rs_txi0", 
            "dmac0_rs_rxi0", 
            SCIFA_READ_BLOCKING_MODE_ENABLE, 
            SCIFA_WRITE_BLOCKING_MODE_ENABLE, 
            1024, 
            1024, 
        }, 
        {
            &GPIO_SC_TABLE_scifa0[0], 
            sizeof(GPIO_SC_TABLE_scifa0)/sizeof(st_r_drv_gpio_sc_config_t), 
        }
    },
    { 1, 
        {
            SCIFA_MODE_ASYNC, 
            1000000, 
            SCIFA_CLK_SRC_INT_SCK_IN, 
            SCIFA_CLK_16X, 
            SCIFA_DATA_8BIT, 
            SCIFA_PARITY_OFF, 
            SCIFA_EVEN_PARITY, 
            SCIFA_STOPBITS_1, 
            SCIFA_NOISE_CANCEL_DISABLE, 
            SCIFA_LSB_FIRST, 
            SCIFA_LOOPBACK_DISABLE, 
            SCIFA_MODEM_CONTROL_DISABLE, 
            SCIFA_RTS_ACTIVE_TRIGGER_15, 
            1, 
            1, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_TX_DMA_MODE, 
            SCIFA_RX_DMA_MODE, 
            9, 
            10, 
            10, 
            9, 
            r_callback_scifa1_teie, 
            r_callback_scifa1_reie, 
            "dmac3_rs_txi1", 
            "dmac2_rs_rxi1", 
            SCIFA_READ_BLOCKING_MODE_ENABLE, 
            SCIFA_WRITE_BLOCKING_MODE_ENABLE, 
            1024, 
            1024, 
        }, 
        {
            &GPIO_SC_TABLE_scifa1[0], 
            sizeof(GPIO_SC_TABLE_scifa1)/sizeof(st_r_drv_gpio_sc_config_t), 
        }
    },
    { 2, 
        {
            SCIFA_MODE_ASYNC, 
            1000000, 
            SCIFA_CLK_SRC_INT_SCK_IN, 
            SCIFA_CLK_16X, 
            SCIFA_DATA_8BIT, 
            SCIFA_PARITY_OFF, 
            SCIFA_EVEN_PARITY, 
            SCIFA_STOPBITS_1, 
            SCIFA_NOISE_CANCEL_DISABLE, 
            SCIFA_LSB_FIRST, 
            SCIFA_LOOPBACK_DISABLE, 
            SCIFA_MODEM_CONTROL_DISABLE, 
            SCIFA_RTS_ACTIVE_TRIGGER_15, 
            1, 
            1, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_TX_DMA_MODE, 
            SCIFA_RX_INTERRUPT_MODE, 
            9, 
            10, 
            10, 
            9, 
            r_callback_scifa2_teie, 
            NULL, 
            "dmac5_rs_txi2", 
            NULL, 
            SCIFA_READ_BLOCKING_MODE_ENABLE, 
            SCIFA_WRITE_BLOCKING_MODE_ENABLE, 
            1024, 
            1024, 
        }, 
        {
            &GPIO_SC_TABLE_scifa2[0], 
            sizeof(GPIO_SC_TABLE_scifa2)/sizeof(st_r_drv_gpio_sc_config_t), 
        }
    },
    { 3, 
        {
            SCIFA_MODE_ASYNC, 
            1000000, 
            SCIFA_CLK_SRC_INT_SCK_IN, 
            SCIFA_CLK_16X, 
            SCIFA_DATA_8BIT, 
            SCIFA_PARITY_OFF, 
            SCIFA_EVEN_PARITY, 
            SCIFA_STOPBITS_1, 
            SCIFA_NOISE_CANCEL_DISABLE, 
            SCIFA_LSB_FIRST, 
            SCIFA_LOOPBACK_DISABLE, 
            SCIFA_MODEM_CONTROL_DISABLE, 
            SCIFA_RTS_ACTIVE_TRIGGER_15, 
            1, 
            1, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_TX_DMA_MODE, 
            SCIFA_RX_INTERRUPT_MODE, 
            9, 
            10, 
            10, 
            9, 
            r_callback_scifa3_teie, 
            NULL, 
            "dmac7_rs_txi3", 
            NULL, 
            SCIFA_READ_BLOCKING_MODE_ENABLE, 
            SCIFA_WRITE_BLOCKING_MODE_ENABLE, 
            1024, 
            1024, 
        }, 
        {
            &GPIO_SC_TABLE_scifa3[0], 
            sizeof(GPIO_SC_TABLE_scifa3)/sizeof(st_r_drv_gpio_sc_config_t), 
        }
    },
    { 4, 
        {
            SCIFA_MODE_ASYNC, 
            1000000, 
            SCIFA_CLK_SRC_INT_SCK_IN, 
            SCIFA_CLK_16X, 
            SCIFA_DATA_8BIT, 
            SCIFA_PARITY_OFF, 
            SCIFA_EVEN_PARITY, 
            SCIFA_STOPBITS_1, 
            SCIFA_NOISE_CANCEL_DISABLE, 
            SCIFA_LSB_FIRST, 
            SCIFA_LOOPBACK_DISABLE, 
            SCIFA_MODEM_CONTROL_DISABLE, 
            SCIFA_RTS_ACTIVE_TRIGGER_15, 
            1, 
            1, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_SPTR_INIT_HIGH, 
            SCIFA_TX_INTERRUPT_MODE, 
            SCIFA_RX_INTERRUPT_MODE, 
            9, 
            10, 
            10, 
            9, 
            NULL, 
            NULL, 
            "dmac9_rs_txi4", 
            NULL, 
            SCIFA_READ_BLOCKING_MODE_ENABLE, 
            SCIFA_WRITE_BLOCKING_MODE_ENABLE, 
            1024, 
            1024, 
        }, 
        {
            &GPIO_SC_TABLE_scifa4[0], 
            sizeof(GPIO_SC_TABLE_scifa4)/sizeof(st_r_drv_gpio_sc_config_t), 
        }
    },
    /* End of modification */
};


#endif /* SRC_RENESAS_CONFIG_R_SCIFA_DRV_SC_CFG_H_ */

/*******************************************************************************************************************//**
 End  Of File
 ***********************************************************************************************************************/