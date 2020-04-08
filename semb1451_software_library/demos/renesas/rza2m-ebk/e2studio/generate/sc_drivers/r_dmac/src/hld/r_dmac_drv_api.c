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
 * @file         r_dmac_drv_api.c
 * @version      1.0
 * @description  DMAC high layer device driver
 ******************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 31.08.2018 1.00    First Release
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "compiler_settings.h"
#include "driver.h"
#include "r_dmac_drv_api.h"
#include "r_dmac_drv_link.h"    /* Link include to low layer driver */
#include "r_dmac_hld_prv.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
    #undef TRACE
    #define TRACE(x)
#endif

#define NOT_USED(p)                     ((void)(p))

/*****************************************************************************
 Function Prototypes
 ******************************************************************************/

/**
 * @brief      Open the DMAC device driver
 * @param[in]  p_stream Pointer to the file stream
 * @retval     DRV_SUCCESS: Success
 *             DRV_ERROR: Failure
 */
static int_t dmac_hld_open (st_stream_ptr_t pStream);

/**
 * @brief      Close the GPIO device driver
 * @param[in]  p_stream Pointer to the file stream
 * @retval     None
 */
static void dmac_hld_close (st_stream_ptr_t pStream);

/**
 * @brief          Handle custom controls for the DMAC device driver
 * @param[in]      p_stream: Pointer to the file stream
 * @param[in]      ctl_code: The custom control code
 * @param[in/out]  p_ctl_struct: Pointer to the custom control structure
 * @retval         DRV_SUCCESS: Success
 *                 DRV_ERROR:   Failure
 */
static int_t dmac_hld_control (st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct);

/**
 * @brief Provides build information even if driver fails to open\n
 *        version information is updated by the developer
 * @param[in]   stream_ptr: Pointer to the file stream
 * @param[out]  p_ver_info: version information
 * @retval      DRV_SUCCESS (never fails)
 */
static int_t dmac_hld_get_version (st_stream_ptr_t stream_ptr, st_ver_info_t *pVerInfo);

static int_t dmac_get_channel (st_stream_ptr_t stream_ptr);

/*****************************************************************************
 Constant Data
 ******************************************************************************/

static const st_drv_info_t s_hld_info =
{
    {
        ((R_DRV_DMAC_HLD_VERSION_MAJOR << 16) + R_DRV_DMAC_HLD_VERSION_MINOR)
    },
    R_DRV_DMAC_HLD_BUILD_NUM,
    R_DRV_DMAC_HLD_DRV_NAME
};

/******************************************************************************
 Private variables
 ******************************************************************************/

static st_dmac_internal_configuration_t s_dmac_drv_config =
{
    false  // hardware not initialised
};

/******************************************************************************
 Global variables
 ******************************************************************************/

/* Define the driver function table for this implementation */
const st_r_driver_t g_dmac_driver =
    {"DMAC Device Driver", dmac_hld_open, dmac_hld_close, no_dev_read, no_dev_write, dmac_hld_control,
                dmac_hld_get_version};

/******************************************************************************
 External global variables
 ******************************************************************************/

/*****************************************************************************
 Private Functions
 ******************************************************************************/

/**
 *             dmac_hld_open
 * @brief      Open the DMAC device driver
 * @param[in]  p_stream Pointer to the file stream
 * @retval     DRV_SUCCESS: Success
 *             DRV_ERROR: Failure
 */
static int_t dmac_hld_open (st_stream_ptr_t p_stream)
{
    int_t channel;
    uint_t sc_config_index;

    if (!s_dmac_drv_config.driver_initialised)
    {
        /* initialise LLD, hardware and interrupts */
        r_dmac_hld_prv_dma_init();

        /* initialise the open count for each channel */
        for (channel = 0; channel < DMAC_NUMBER_OF_CHANNELS; channel++)
        {
        	s_dmac_drv_config.channel_configuration[channel].open_count = 0;
        }

        s_dmac_drv_config.driver_initialised = true;
    }

    /* get channel associated with handle */
    channel = dmac_get_channel(p_stream);
    sc_config_index = p_stream->sc_config_index;

    if (DRV_ERROR == channel)
    {
        return DRV_ERROR;
    }

    /* check that we haven't exceeded the open count for this channel */
    if (s_dmac_drv_config.channel_configuration[channel].open_count >= R_CFG_DMAC_DRV_MAXIMUM_ACCESS_PRV)
    {
        return DRV_ERROR;
    }

    /* configure the channel according to Smart Configurator settings */
    if (r_dmac_hld_prv_open(sc_config_index) == DRV_ERROR)
    {
        return DRV_ERROR;
    }

    /* increase open count for this channel */
    s_dmac_drv_config.channel_configuration[channel].open_count++;

    /* return status */
    return (DRV_SUCCESS);
}
/*******************************************************************************
 End of function dmac_hld_open
 ******************************************************************************/

/**
 *        dmac_get_channel
 * @brief Acquires the channel number from a given stream_ptr
 *
 * This function safely returns a channel number (0, 1, 2, ...) if it is
 * supported by the driver, otherwise it returns -1
 *
 * @param stream_ptr Pointer to the file stream
 *
 * @retval  N Where N is the current channel.
 * @retval -1 DRV_ERROR
 */
static int_t dmac_get_channel (st_stream_ptr_t stream_ptr)
{
    int_t ret_value = DRV_ERROR;
    int_t channel_id;

    /* if valid stream pointer */
    if (NULL != stream_ptr)
    {
        /* get channel associated with configuration  */
        channel_id = R_DMAC_GetChannel((uint_t) (stream_ptr->sc_config_index));

        if (DRV_ERROR != channel_id)
        {
            /* verify that the channel is supported */
            /* look for channel in the array of available channels from low level driver */
            /* (e_channel_id_t). This is a bit field where the power of 2 is the available channel */
            if (((1 << channel_id) <= R_CH15))
            {
                /* set channel as bit field where value is 2 << channel */
                uint_t channel_bit_value = (1u << channel_id);

                /* get bit field of supported channels */
                uint_t supported_channels = DMAC_CFG_LLD_SUPPORTED_CHANNELS;

                if (0 != (channel_bit_value & supported_channels))
                {
                    /* channel is supported */
                    ret_value = channel_id;
                }
                else
                {
                    /* channel is not supported */
                    ret_value = DRV_ERROR;
                }
            }
        }
    }

    /* return status as error or channel number  */
    return (ret_value);
}
/******************************************************************************
 * End of Function dmac_get_channel
 ******************************************************************************/

/**
 *             dmac_hld_close
 * @brief      Close the DMAC device driver
 * @param[in]  p_stream Pointer to the file stream
 * @retval     None
 */
static void dmac_hld_close (st_stream_ptr_t p_stream)
{
    int_t channel;
    uint_t sc_config_index;

    sc_config_index = p_stream->sc_config_index;

    /* get channel associated with handle */
    channel = dmac_get_channel(p_stream);

    if (s_dmac_drv_config.channel_configuration[channel].open_count > 0)
    {
    	s_dmac_drv_config.channel_configuration[channel].open_count--;
    }

    if (0 == s_dmac_drv_config.channel_configuration[channel].open_count)
    {
        R_DMAC_CloseChannel(sc_config_index);
    }

    /* un-initialise the hardware if all channels closed? */
    /* R_DMAC_HWUninitialise(); */
}
/***********************************************************************************************************************
 End of function dmac_hld_close
 **********************************************************************************************************************/

/**
 *                 dmac_hld_control
 * @brief          Handle custom controls for the DMAC device driver
 * @param[in]      p_stream: Pointer to the file stream
 * @param[in]      ctl_code: The custom control code
 * @param[in/out]  p_ctl_struct: Pointer to the custom control structure
 * @retval         DRV_SUCCESS: Success
 *                 DRV_ERROR:   Failure
 */
static int_t dmac_hld_control (st_stream_ptr_t p_stream, uint32_t ctl_code, void *p_ctl_struct)
{
    int_t ret_value = DRV_ERROR;
    e_r_drv_dmac_err_t dmac_err;
    st_r_drv_dmac_config_t *p_configuration;
    st_r_drv_dmac_next_transfer_t *p_next_transfer;
    int_t channel;
    uint_t sc_config_index;

    sc_config_index = p_stream->sc_config_index;

    /* get channel associated with handle */
    channel = dmac_get_channel(p_stream);

    if (channel < 0)
    {
        return DRV_ERROR;
    }

    /* fail if the channel is not open */
    if (0 == s_dmac_drv_config.channel_configuration[channel].open_count)
    {
        return DRV_ERROR;
    }

    switch ((e_ctrl_code_dmac_t) ctl_code)
    {
        /* start a DMA transfer */
        case CTL_DMAC_SOFTWARE_TRIGGER:
        {
            if (R_DMAC_SoftwareTrigger(sc_config_index) == DMAC_SUCCESS)
            {
                ret_value = DRV_SUCCESS;
            }
            else
            {
                ret_value = DRV_ERROR;
            }
            break;
        }

        case CTL_DMAC_SET_CONFIGURATION:
        {
            if (NULL != p_ctl_struct)
            {
                /* assign new pointer for readability */
                p_configuration = (st_r_drv_dmac_config_t *) p_ctl_struct;

                R_DMAC_SetDmaCompleteCallback(sc_config_index, p_configuration->config.p_dmaComplete);
                R_DMAC_SetDmaErrorCallback(sc_config_index, p_configuration->config.p_dmaError);

                dmac_err = R_DMAC_ConfigureChannel(sc_config_index, &p_configuration->config, 0);
                p_configuration->err = dmac_err;

                if (DMAC_SUCCESS == dmac_err)
                {
                    ret_value = DRV_SUCCESS;
                }
                else
                {
                    ret_value = DRV_ERROR;
                }
            }
            break;
        }

        case CTL_DMAC_GET_CONFIGURATION:
        {
            if (NULL != p_ctl_struct)
            {
                /* assign new pointer for readability */
                p_configuration = (st_r_drv_dmac_config_t *) p_ctl_struct;

                R_DMAC_GetDmaCompleteCallback(sc_config_index, &(p_configuration->config.p_dmaComplete));
                R_DMAC_GetDmaErrorCallback(sc_config_index, &(p_configuration->config.p_dmaError));

                dmac_err = R_DMAC_GetChannelConfiguration(sc_config_index, &p_configuration->config);
                p_configuration->err = dmac_err;

                if (DMAC_SUCCESS == dmac_err)
                {
                    ret_value = DRV_SUCCESS;
                }
                else
                {
                    ret_value = DRV_ERROR;
                }
            }
            break;
        }

        case CTL_DMAC_ENABLE:
        {
            if (r_dmac_hld_prv_dma_enable(sc_config_index) == DMAC_SUCCESS)
            {
                ret_value = DRV_SUCCESS;
            }
            else
            {
                ret_value = DRV_ERROR;
            }
            break;
        }

        case CTL_DMAC_DISABLE:
        {
            if (NULL != p_ctl_struct)
            {
                if (r_dmac_hld_prv_dma_disable(sc_config_index, (uint32_t *) p_ctl_struct) == DMAC_SUCCESS)
                {
                    ret_value = DRV_SUCCESS;
                }
                else
                {
                    ret_value = DRV_ERROR;
                }
            }
            break;
        }

        case CTL_DMAC_NEXT_TRANSFER:
        {
            if (NULL != p_ctl_struct)
            {
                /* assign new pointer for readability */
                p_next_transfer = (st_r_drv_dmac_next_transfer_t *) p_ctl_struct;

                dmac_err = R_DMAC_SetNextTransfer(sc_config_index, p_next_transfer->source_address,
                        p_next_transfer->destination_address, p_next_transfer->count);

                if (DMAC_SUCCESS == dmac_err)
                {
                    ret_value = DRV_SUCCESS;
                }
                else
                {
                    ret_value = DRV_ERROR;
                }
            }
            break;
        }

        case CTL_DMAC_GET_TRANSFER_BYTE_COUNT:
        {
            if (NULL != p_ctl_struct)
            {
                /* assign new pointer for readability */
                uint32_t * p_crtb_value = (uint32_t *) p_ctl_struct;

                dmac_err = R_DMAC_GetCrtbRegisterValue(sc_config_index, p_crtb_value);

                if (DMAC_SUCCESS == dmac_err)
                {
                    ret_value = DRV_SUCCESS;
                }
                else
                {
                    ret_value = DRV_ERROR;
                }
            }
            break;
        }

        default:
        {
            TRACE(("DMAC Driver: Unknown control code\r\n"));
            break;
        }
    }

    return (ret_value);
}
/***********************************************************************************************************************
 End of function dmac_hld_control
 **********************************************************************************************************************/

/**
 *              dmac_hld_get_version
 * @brief       Provides build information even if driver fails to open\n
 *              version information is updated by the developer
 * @param[in]   stream_ptr: Pointer to the file stream
 * @param[out]  p_ver_info: version information
 * @retval      DRV_SUCCESS (never fails)
 */
static int_t dmac_hld_get_version (st_stream_ptr_t stream_ptr, st_ver_info_t *p_ver_info)
{
    /* get driver version */
    NOT_USED(stream_ptr);

    p_ver_info->hld.version.sub.major = s_hld_info.version.sub.major;
    p_ver_info->hld.version.sub.minor = s_hld_info.version.sub.minor;
    p_ver_info->hld.build = s_hld_info.build;
    p_ver_info->hld.p_szdriver_name = s_hld_info.p_szdriver_name;

    /* Obtain version information from Low layer Driver */
    R_DMAC_GetVersion( &p_ver_info->lld);

    return (DRV_SUCCESS);
}
/***********************************************************************************************************************
 End of function dmac_hld_get_version
 **********************************************************************************************************************/

/******************************************************************************
 End Of File
 ******************************************************************************/