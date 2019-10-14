/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : r_ether_drv_sc_cfg.h
 * Version      : 1.01
 * Description  : OS timer driver configuration header, part of the high layer driver.
 *                This file uses the same name for any low layer driver that is supported by a compatible high layer
 *                driver.
 *                File is included in the high layer driver to allow the low layer driver to be swapped at build time
 *                and share the same high layer code.
 *********************************************************************************************************************/

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
#include "r_typedefs.h"
#include "r_gpio_drv_api.h"
#include "r_gpio_drv_sc_cfg.h"

#ifndef SRC_RENESAS_CONFIG_R_ETHER_DRV_SC_CFG_H_
#define SRC_RENESAS_CONFIG_R_ETHER_DRV_SC_CFG_H_

/******************************************************************************
Macro definitions
******************************************************************************/
/* Use MMU or not. */
    #define ETHER_CFG_USE_MMU (1)

/* Use ETHER0 or not. */
    #define ETHER_CFG_CH0_EN    (1)

/* Use ETHER1 or not. */
    #define ETHER_CFG_CH1_EN    (1)

/* The number of Rx descriptors. */
    #define ETHER_CFG_EMAC_RX_DESCRIPTORS               (8)

/* The number of Tx descriptors. */
    #define ETHER_CFG_EMAC_TX_DESCRIPTORS               (8)

/* The size of the transmit buffer or receive buffer. */
    #define ETHER_CFG_BUFSIZE   (1536)

/* Wait time needed to access PHY after reset.(ms) */
    #define ETHER_CFG_PHY_WAIT  (150)

/******************************************************************************
Type definitions
******************************************************************************/
/**
 * @typedef st_r_drv_ether_sc_config_t Smart Configurator Interface
 */

typedef struct
{
    /* Enable or disable Ethernet channel 0. 1: Enable */
    int_t       ether_ch0_en;

    /* Enable or disable Ethernet channel 1. 1: Enable */
    int_t       ether_ch1_en;

    /* Select interface:
     *  0: MII  (Media Independent Interface)
     *  1: RMII (Reduced Media Independent Interface)
     */
    int_t       ether_mode;

    /* PHY-LSI address setting for ETHER0. */
    int_t       ether_ch0_phy_address;

    /* PHY-LSI address setting for ETHER1. */
    int_t       ether_ch1_phy_address;

    /* The loop count of software loop used for read or write in PHY-LSI. */
    int_t       ether_phy_mii_wait;

    /* The waiting time for reset completion of PHY-LSI. */
    int_t       ether_phy_delay_reset;

    /* Link status read from LMON bit of ETHERC PSR register. */
    int_t       ether_link_preset;

    /* LINKSTA signal for detect link status changes. */
    int_t       ether_use_linksta;

    /* Difference between phisycal address and virtual address of un-cached RAM.. */
    int_t       ether_mmu_uncached_diff;
} ether_config_t;

typedef struct
{
    ether_config_t                  config;
    st_r_drv_gpio_pin_init_table_t  pin;
} st_r_drv_ether_sc_config_t;

/******************************************************************************
Variable Externs
******************************************************************************/
#if defined(R_ETHER_RZA2_C__)
/**
 * @section R_SCIFA_SC_CFG_80_TABLE Smart Configurator settings table.
 */
const st_r_drv_ether_sc_config_t g_ether_sc_table[] =
{
    /* This code is auto-generated. Do not edit manually */
    { 
        {
            1, 
            0, 
            1, 
            7, 
            7, 
            8, 
            0x00020000, 
            0, 
            0, 
            0x02000000, 
        }, 
        {
            &GPIO_SC_TABLE_ether0[0], 
            sizeof(GPIO_SC_TABLE_ether0)/sizeof(st_r_drv_gpio_sc_config_t), 
        }
    },
    /* End of modification */
};
#else
extern const st_r_drv_ether_sc_config_t g_ether_sc_table[1];
#endif

#endif /* SRC_RENESAS_CONFIG_R_ETHER_DRV_SC_CFG_H_ */

/**********************************************************************************************************************
End  Of File
 *********************************************************************************************************************/
