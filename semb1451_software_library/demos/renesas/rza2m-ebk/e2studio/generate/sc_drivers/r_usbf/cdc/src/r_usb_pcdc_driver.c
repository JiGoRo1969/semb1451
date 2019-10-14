/***********************************************************************************************************************
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
 * Copyright (C) 2014(2018) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_pcdc_driver.c
 * Description  : USB PCDC Driver code
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 26.12.2014 1.10 RX71M is added
 *         : 30.09.2015 1.11 RX63N/RX631 is added.
 *         : 30.09.2016 1.20 RX65N/RX651 is added.
 *         : 30.09.2017 1.22 Changed function name "pcdc_read_complete"->"usb_pcdc_read_complete"
 *                           "pcdc_write_complete"->"usb_pcdc_write_complete"
 *         : 31.03.2018 1.23 Supporting Smart Configurator
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_pcdc_if.h"
#include "r_usb_pcdc.h"
#include "r_usb_bitdefine.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
extern uint16_t g_usb_peri_connected; /* Peri CDC application enable */

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/******************************************************************************
 Exported global variables
 ******************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
/* variables */
/* PCDC <--> USB massage */
//usb_utr_t g_usb_pcdc_mess[10] __attribute__ ((section ("USB_DATA_RAM")));
usb_utr_t g_usb_pcdc_mess[10] __attribute__ ((section ("USB_DATA_UNCACHE")));

/******************************************************************************
 Renesas Abstracted Peripheral Communications Devices Class Driver API functions
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_pcdc_data_trans
 Description     : Tranfer data from USB
 Arguments       : uint16_t pipe        ; Pipe No
                 : uint32_t size        ; Data Size
                 : uint8_t *table       ; Data address
                 : usb_cb_t complete    ; Callback function
 Return value    : none
 ******************************************************************************/
void usb_pcdc_data_trans (uint16_t pipe, uint32_t size, uint8_t * table, usb_cb_t complete)
{
    /* PIPE Transfer set */
    g_usb_pcdc_mess[pipe].keyword = pipe; /* Pipe No */
    g_usb_pcdc_mess[pipe].p_tranadr = table; /* Data address */
    g_usb_pcdc_mess[pipe].tranlen = size; /* Data Size */
    g_usb_pcdc_mess[pipe].complete = (usb_cb_t) complete; /* Callback function */

    usb_pstd_transfer_start(&g_usb_pcdc_mess[pipe]); /* USB Transfer Start */
}
/******************************************************************************
 End of function usb_pcdc_data_trans
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_pcdc_read_complete
 Description     : CallBack Function
 Arguments       : usb_utr_t    *mess   : Pointer to usb_utr_t structure
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 Return          : none
 ******************************************************************************/
void usb_pcdc_read_complete (usb_utr_t *mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    if ( USB_TRUE == g_usb_peri_connected)
    {
        /* Set Receive data length */
    	ctrl.size = g_usb_read_request_size[USB_CFG_USE_USBIP][mess->keyword] - mess->tranlen;
        ctrl.pipe = mess->keyword;  /* Pipe number setting */
        ctrl.type = USB_PCDC;       /* Device class setting  */
        switch (mess->status)
        {
            case USB_DATA_OK :
                ctrl.status = USB_SUCCESS;
            break;
            case USB_DATA_SHT :
                ctrl.status = USB_ERR_SHORT;
            break;
            case USB_DATA_OVR :
                ctrl.status = USB_ERR_OVER;
            break;
            case USB_DATA_ERR :
            default :
                ctrl.status = USB_ERR_NG;
            break;
        }
        ctrl.module = USB_CFG_USE_USBIP;
        usb_set_event(USB_STS_READ_COMPLETE, &ctrl);
    }
}
/******************************************************************************
 End of function usb_pcdc_read_complete
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_pcdc_write_complete
 Description     : CallBack Function
 Arguments       : usb_utr_t    *mess   : Pointer to usb_utr_t structure
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 Return          : none
 ******************************************************************************/
void usb_pcdc_write_complete (usb_utr_t *mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    if ( USB_TRUE == g_usb_peri_connected)
    {
        ctrl.pipe = mess->keyword; /* Pipe number setting */
        if (USB_CFG_PCDC_BULK_IN == ctrl.pipe)
        {
            ctrl.type = USB_PCDC; /* CDC Data class  */
        }

        /* USB_CFG_PCDC_INT_IN */
        else
        {
            ctrl.type = USB_PCDCC; /* CDC Control class  */
        }
        if (USB_DATA_NONE == mess->status)
        {
            ctrl.status = USB_SUCCESS;
        }
        else
        {
            ctrl.status = USB_ERR_NG;
        }
        ctrl.module = USB_CFG_USE_USBIP;
        usb_set_event(USB_STS_WRITE_COMPLETE, &ctrl);
    }
}
/******************************************************************************
 End of function usb_pcdc_write_complete
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
