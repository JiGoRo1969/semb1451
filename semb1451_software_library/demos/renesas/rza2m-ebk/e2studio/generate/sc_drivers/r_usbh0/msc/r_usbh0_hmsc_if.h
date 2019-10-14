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
* Copyright (C) 2014(2015) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usbh0_hmsc_if.h
* Description  : Interface file for USB HMSC API
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
***********************************************************************************************************************/

#ifndef R_USBH0_HMSC_IF_H
#define R_USBH0_HMSC_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usbh0_basic_if.h"
#include "r_usbh0_hmsc_config.h"

/*****************************************************************************
Enum definitions
******************************************************************************/
enum
{
    /*--- SFF-8070i command define ---*/
    USBH0_ATAPI_TEST_UNIT_READY         = 0x00u,
    USBH0_ATAPI_REQUEST_SENSE           = 0x03u,
    USBH0_ATAPI_FORMAT_UNIT             = 0x04u,
    USBH0_ATAPI_INQUIRY                 = 0x12u,
    USBH0_ATAPI_MODE_SELECT6            = 0x15u,
    USBH0_ATAPI_MODE_SENSE6             = 0x1Au,
    USBH0_ATAPI_START_STOP_UNIT         = 0x1Bu,
    USBH0_ATAPI_PREVENT_ALLOW           = 0x1Eu,
    USBH0_ATAPI_READ_FORMAT_CAPACITY    = 0x23u,
    USBH0_ATAPI_READ_CAPACITY           = 0x25u,
    USBH0_ATAPI_READ10                  = 0x28u,
    USBH0_ATAPI_WRITE10                 = 0x2Au,
    USBH0_ATAPI_SEEK                    = 0x2Bu,
    USBH0_ATAPI_WRITE_AND_VERIFY        = 0x2Eu,
    USBH0_ATAPI_VERIFY10                = 0x2Fu,
    USBH0_ATAPI_MODE_SELECT10           = 0x55u,
    USBH0_ATAPI_MODE_SENSE10            = 0x5Au,
};

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void        R_USBH0_HmscTask(void);
void        R_USBH0_HmscDriverStart(void);
void        R_USBH0_HmscClassCheck(uint16_t **pp_table);
uint16_t    R_USBH0_HmscRead10(uint16_t side, uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
uint16_t    R_USBH0_HmscWrite10(uint16_t side, const uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
uint16_t    R_USBH0_HmscTestUnit(uint16_t side);
uint16_t    R_USBH0_HmscRequestSense(uint16_t side, uint8_t *p_buff);
uint16_t    R_USBH0_HmscInquiry(uint16_t side, uint8_t *p_buff);
uint16_t    R_USBH0_HmscReadCapacity(uint16_t side, uint8_t *p_buff);
uint16_t    R_USBH0_HmscReadFormatCapacity(uint16_t side, uint8_t *p_buff);
usbh0_er_t  R_USBH0_HmscGetMaxUnit(uint16_t addr, uint8_t *p_buff, usbh0_utr_cb_t complete);
usbh0_er_t  R_USBH0_HmscMassStorageReset(uint16_t addr, usbh0_utr_cb_t complete);
uint16_t    R_USBH0_HmscPreventAllow(uint16_t side, uint8_t *p_buff);
uint16_t    R_USBH0_HmscModeSense10(uint16_t side, uint8_t *p_buff);
uint16_t    R_USBH0_HmscModeSelect6(uint16_t side, uint8_t *p_buff);
uint16_t    R_USBH0_HmscGetDevSts(uint16_t side);
uint16_t    R_USBH0_HmscAllocDrvNo(uint16_t *p_side, uint16_t devadr);
uint16_t    R_USBH0_HmscFreeDrvNo(uint16_t side);
uint16_t    R_USBH0_HmscRefDrvNo(uint16_t *p_side, uint16_t devadr);

void        R_USBH0_HmscStrgDriveTask(void);
uint16_t    R_USBH0_HmscStrgDriveSearch(uint16_t addr, usbh0_utr_cb_t complete);
uint16_t    R_USBH0_HmscStrgDriveOpen(uint16_t *p_side, uint16_t addr);
uint16_t    R_USBH0_HmscStrgDriveClose(uint16_t side);
uint16_t    R_USBH0_HmscStrgReadSector(uint16_t side, uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
uint16_t    R_USBH0_HmscStrgWriteSector(uint16_t side, const uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
uint16_t    R_USBH0_HmscStrgUserCommand(uint16_t side, uint16_t command, uint8_t *p_buff, usbh0_utr_cb_t complete);
uint16_t    R_USBH0_HmscStrgCheckEnd(void);

#endif /* R_USBH0_HMSC_IF_H */
