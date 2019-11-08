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
 * File Name    : r_dmac_drv_api.h
 * Device(s)    : RZ/A2M
 * Description  : Header file for DMAC Driver
*******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 31.08.2018 1.00 First Release
******************************************************************************/

#include "r_typedefs.h"
#include "r_devlink_wrapper.h"

#include "r_dmac_device_cfg.h"

#ifndef R_DMAC_INC_R_DMAC_DRV_API_H_
#define R_DMAC_INC_R_DMAC_DRV_API_H_

/******************************************************************************
 Macro definitions
 ******************************************************************************/

#define R_DRV_DMAC_HLD_DRV_NAME           ("HLD DMAC")

/** Major Version Number of API.
 * Updated by product owner */
#define R_DRV_DMAC_HLD_VERSION_MAJOR      (2)

/** Minor Version Number of API.
 * Updated by developer */
#define R_DRV_DMAC_HLD_VERSION_MINOR      (0)

/** Build Number of API.
 * Generated during customer release */
#define R_DRV_DMAC_HLD_BUILD_NUM          (0)

/** Unique ID. Assigned by requirements */
#define R_DRV_DMAC_HLD_UID                (59)

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

typedef enum
{
    CTL_DMAC_SOFTWARE_TRIGGER = (R_DRV_DMAC_HLD_UID << 16) + 0,/*!< Software trigger a DMA transfer (e.g. memory to memory) */
    CTL_DMAC_SET_CONFIGURATION,                                /*!< Configure a DMA transfer */
    CTL_DMAC_GET_CONFIGURATION,                                /*!< Get DMA channel configuration */
    CTL_DMAC_ENABLE,                                           /*!< Enable a DMA transfer */
    CTL_DMAC_DISABLE,                                          /*!< Disable a DMA transfer */
    CTL_DMAC_NEXT_TRANSFER,                                    /*!< Set data for following transfer */
} e_ctrl_code_dmac_t;

typedef enum
{
    DMAC_SUCCESS = DRV_SUCCESS,                                /*!< Success */
    DMAC_ERR_FAILED = -1,                                      /*!< Operation failed */
    DMAC_ERR_INVALID_SIZE = -2,                                /*!< Invalid transfer unit size setting */
    DMAC_ERR_INVALID_ADDRESS_TYPE = -3,                        /*!< Invalid address type setting */
    DMAC_ERR_INVALID_CFG = -4,                                 /*!< Invalid configuration */
    DMAC_ERR_INVALID_CHANNEL = -5                              /*!< Invalid channel */
} e_r_drv_dmac_err_t;

typedef enum
{
    DMA_DATA_SIZE_MIN = (-1),                                   /*!< Use the smallest DMA transfer unit size */
    DMA_DATA_SIZE_1 = 1,                                        /*!< 1 byte DMA transfer unit size */
    DMA_DATA_SIZE_2 = 2,                                        /*!< 2 byte DMA transfer unit size */
    DMA_DATA_SIZE_4 = 4,                                        /*!< 4 byte DMA transfer unit size */
    DMA_DATA_SIZE_8 = 8,                                        /*!< 8 byte DMA transfer unit size */
    DMA_DATA_SIZE_16 = 16,                                      /*!< 16 byte DMA transfer unit size */
    DMA_DATA_SIZE_32 = 32,                                      /*!< 32 byte DMA transfer unit size */
    DMA_DATA_SIZE_64 = 64,                                      /*!< 64 byte DMA transfer unit size */
    DMA_DATA_SIZE_128 = 128,                                    /*!< 128 byte DMA transfer unit size */
    DMA_DATA_SIZE_MAX = 0                                       /*!< Use the largest DMA transfer unit size */
} e_r_drv_dmac_data_size_t;

typedef enum
{
    DMA_ADDRESS_INCREMENT = 0,                                  /*!< Increment the address */
    DMA_ADDRESS_FIX = 1                                         /*!< Fix the address */
} e_r_drv_dmac_address_type_t;

typedef enum
{
    DMA_REQUEST_SOURCE,                                         /*!< Read DMA */
    DMA_REQUEST_DESTINATION,                                    /*!< Write DMA */
} e_r_drv_dmac_request_dirn_t;

typedef enum
{
    DMA_SINGLE_TRANSFER_MODE,                                   /*!< Single transfer mode */
    DMA_BLOCK_TRANSFER_MODE,                                    /*!< Block transfer mode */
} e_r_drv_dmac_transfer_mode_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
typedef struct
{
    e_r_drv_dmac_xfer_resource_t resource;                      /*!< DMA transfer resource */
    e_r_drv_dmac_data_size_t source_width;                      /*!< DMA transfer unit size (source) */
    e_r_drv_dmac_data_size_t destination_width;                 /*!< DMA transfer unit size (destination) */
    e_r_drv_dmac_address_type_t source_address_type;            /*!< DMA address type (source) */
    e_r_drv_dmac_address_type_t destination_address_type;       /*!< DMA address type (destination) */
    e_r_drv_dmac_request_dirn_t direction;                      /*!< DMA transfer direction */
    void (*p_dmaComplete)();                                    /*!< set callback function (DMA end interrupt) */
    void (*p_dmaError)();                                       /*!< set callback function (DMA error) */
    void *source_address;                                       /*!< Source Address */
    void *destination_address;                                  /*!< Destination Address */
    uint32_t count;                                             /*!< DMA Transfer Size */
} st_r_drv_dmac_channel_config_t;

typedef struct
{
    st_r_drv_dmac_channel_config_t config;                      /*!< channel configuration data */
    e_r_drv_dmac_err_t err;                                     /*!< error code (see e_r_drv_dmac_err_t) */
} st_r_drv_dmac_config_t;

typedef struct
{
    uint_t channel;                                             /*!< DMA channel */
    st_r_drv_dmac_channel_config_t config;                      /*!< channel configuration data */
} st_r_drv_dmac_sc_config_t;
#pragma GCC diagnostic pop

typedef struct
{
    void *source_address;                                       /*!< Source Address */
    void *destination_address;                                  /*!< Destination Address */
    uint32_t count;                                             /*!< DMA Transfer Size */
} st_r_drv_dmac_next_transfer_t;

/******************************************************************************
 Constant Data
 ******************************************************************************/

extern const st_r_driver_t g_dmac_driver;

#endif /* R_DMAC_INC_R_DMAC_DRV_API_H_ */
