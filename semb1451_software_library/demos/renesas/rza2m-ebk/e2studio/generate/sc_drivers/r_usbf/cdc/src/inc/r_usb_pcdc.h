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
 * File Name    : r_usb_pcdc.h
 * Description  : USB PCDC Local Header
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 26.12.2014 1.10 RX71M is added
 *         : 30.09.2015 1.11 RX63N/RX631 is added.
 *         : 30.09.2016 1.20 RX65N/RX651 is added.
 *         : 30.09.2017 1.22 Delete functions prototypes
 *                           "usb_pcdc_ctrl_trans_idst",
 *                           "usb_pcdc_ctrl_trans_rdds",
 *                           "usb_pcdc_ctrl_trans_wrds",
 *                           "usb_pcdc_ctrl_trans_wrnd",
 *                           "usb_pcdc_ctrl_trans_rdss",
 *                           "usb_pcdc_ctrl_trans_wrss",
 *                           "cdc_configured",
 *                           "cdc_detach",
 *                           "cdc_default",
 *                           "cdc_suspend",
 *                           "cdc_resume",
 *                           "cdc_interface",
 *                           "cdc_detach_device",
 *                           "usb_pcdc_pipe_info",
 *                           "cdc_registration",
 *         : 31.03.2018 1.23 Supporting Smart Configurator
 ***********************************************************************************************************************/
#ifndef R_USB_PCDC_LOCAL_H
#define R_USB_PCDC_LOCAL_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/*****************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Exported global variables
 ******************************************************************************/

/******************************************************************************
 Exported global functions (to be accessed by other files)
******************************************************************************/
void usb_pcdc_data_trans (uint16_t pipe, uint32_t Size, uint8_t *table, usb_cb_t complete);


#endif  /* R_USB_PCDC_LOCAL_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/
