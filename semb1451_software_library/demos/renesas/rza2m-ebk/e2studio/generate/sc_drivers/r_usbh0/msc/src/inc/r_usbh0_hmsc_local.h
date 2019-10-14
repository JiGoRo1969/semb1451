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
* File Name    : r_usbh0_hmsc_define.h
* Description  : USB Host MSC header file
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
***********************************************************************************************************************/

#ifndef R_USBH0_HMSC_LOCAL_H
#define R_USBH0_HMSC_LOCAL_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usbh0_basic_if.h"      /* type define */
#include "r_usbh0_basic_local.h"
#include "r_usbh0_hmsc_config.h"

/******************************************************************************
Enum define
******************************************************************************/
/* ERROR CODE */
enum 
{
    USBH0_HMSC_OK             = (uint16_t)0,
    USBH0_HMSC_STALL          = (uint16_t)-1,
    USBH0_HMSC_CBW_ERR        = (uint16_t)-2,     /* CBW error */
    USBH0_HMSC_DAT_RD_ERR     = (uint16_t)-3,     /* Data IN error */
    USBH0_HMSC_DAT_WR_ERR     = (uint16_t)-4,     /* Data OUT error */
    USBH0_HMSC_CSW_ERR        = (uint16_t)-5,     /* CSW error */
    USBH0_HMSC_CSW_PHASE_ERR  = (uint16_t)-6,     /* Phase error */
    USBH0_HMSC_SUBMIT_ERR     = (uint16_t)-7,     /* Submit error */
} e_usbh0_ghmsc_error_t;

/* CSW STATUS */
enum 
{
    USBH0_MSC_CSW_OK          = (uint16_t)0x00,
    USBH0_MSC_CSW_NG          = (uint16_t)0x01,
    USBH0_MSC_CSW_PHASE_ERR   = (uint16_t)0x02
} e_usbh0_gcmsc_cswsts_t;

/*****************************************************************************
Macro definitions
******************************************************************************/
/* CBW definitions */
#define USBH0_MSC_CBWLENGTH                   (31u)
#define USBH0_MSC_CBWCB_LENGTH                (12u)
#define USBH0_MSC_CBW_SIGNATURE               ((uint32_t)0x43425355)

/* CSW definitions */
#define USBH0_MSC_CSW_LENGTH                  (13u)
#define USBH0_MSC_CSW_SIGNATURE               ((uint32_t)0x53425355)

/* subClass code */
#define USBH0_ATAPI_MMC5                      ((uint8_t)0x02)
#define USBH0_ATAPI                           ((uint8_t)0x05)
#define USBH0_SCSI                            ((uint8_t)0x06)

/* Protocol code */
#define USBH0_BOTP                            ((uint8_t)0x50)

/* Message code */
#define USBH0_MSG_HMSC_NO_DATA                ((uint16_t)0x501)
#define USBH0_MSG_HMSC_DATA_IN                ((uint16_t)0x502)
#define USBH0_MSG_HMSC_DATA_OUT               ((uint16_t)0x503)
#define USBH0_MSG_HMSC_CSW_PHASE_ERR          ((uint16_t)0x505)
#define USBH0_MSG_HMSC_CBW_ERR                ((uint16_t)0x506)

#define USBH0_MSG_HMSC_STRG_DRIVE_SEARCH      ((uint16_t)0x601)
#define USBH0_MSG_HMSC_STRG_DRIVE_SEARCH_END  ((uint16_t)0x602)
#define USBH0_MSG_HMSC_STRG_USER_COMMAND      ((uint16_t)0x605)
#define USBH0_MSG_HMSC_STRG_RW_END            ((uint16_t)0x606)

/* Class Request Buffer Size */
#define USBH0_HMSC_CLSDATASIZE                (256u)

/* Host MSC Task */
#define USBH0_HMSC_TSK                       (USBH0_TID_6)           /* Task ID */
#define USBH0_HMSC_MBX                       (USBH0_HMSC_TSK)        /* Mailbox ID */
#define USBH0_HMSC_MPL                       (USBH0_HMSC_TSK)        /* Memorypool ID */
#define USBH0_HMSC_MSG                       (USBH0_HMSC_TSK)        /* Message ID */

/* Host STRG Task */
#define USBH0_HSTRG_TSK                      (USBH0_TID_7)           /* Task ID */
#define USBH0_HSTRG_MBX                      (USBH0_HSTRG_TSK)       /* Mailbox ID */
#define USBH0_HSTRG_MPL                      (USBH0_HSTRG_TSK)       /* Memorypool ID */
#define USBH0_HSTRG_MSG                      (USBH0_HSTRG_TSK)       /* Message ID */


/*****************************************************************************
Typedef definitions
******************************************************************************/
/* CBW Structure define.    */
typedef struct
{
    uint32_t    dcbw_signature;
    uint32_t    dcbw_tag;
    uint8_t     dcbw_dtl_lo;
    uint8_t     dcbw_dtl_ml;
    uint8_t     dcbw_dtl_mh;
    uint8_t     dcbw_dtl_hi;
    struct
    {
        uint8_t reserved7:7;
        uint8_t cbw_dir:1;
    } bmcbw_flags;
    struct
    {
        uint8_t bcbw_lun:4;
        uint8_t reserved4:4;
    } bcbw_lun;
    struct
    {
        uint8_t bcbw_cb_length:5;
        uint8_t reserved3:3;
    } bcbw_cb_length;
    uint8_t     cbw_cb[16];
} st_usbh0_hmsc_cbw_t;

/* CSW Structure define define. */
typedef struct
{
    uint32_t    dcsw_signature;
    uint32_t    dcsw_tag;
    uint8_t     dcsw_data_residue_lo;
    uint8_t     dcsw_data_residue_ml;
    uint8_t     dcsw_data_residue_mh;
    uint8_t     dcsw_data_residue_hi;
    uint8_t     bcsw_status;
} st_usbh0_hmsc_csw_t;

/* Structure for DRIVE No. */
typedef struct
{
    uint16_t    use_flag;          /* USE flag */
    uint16_t    devadr;            /* Device address */
} st_usbh0_hmsc_drive_management_t;

/*****************************************************************************
Public Valiables
******************************************************************************/
/* HMSC driver */
extern st_usbh0_utr_t                     g_usbh0_hmsc_class_control;
extern st_usbh0_hmsc_drive_management_t   g_usbh0_hmsc_drvno_tbl[USBH0_MAXSTRAGE];
extern uint16_t                         g_usbh0_hmsc_in_pipe[USBH0_MAXSTRAGE];
extern uint16_t                         g_usbh0_hmsc_out_pipe[USBH0_MAXSTRAGE];
extern st_usbh0_hmsc_cbw_t                g_usbh0_hmsc_cbw[USBH0_MAXSTRAGE];
extern st_usbh0_hmsc_csw_t                g_usbh0_hmsc_csw[USBH0_MAXSTRAGE];
extern uint32_t                         g_usbh0_hmsc_cbw_tag_no[USBH0_MAXSTRAGE];
extern uint8_t                          g_usbh0_hmsc_sub_class[USBH0_MAXSTRAGE];
extern uint8_t                          g_usbh0_hmsc_tmp_sub_class;
extern uint8_t                          *gp_usbh0_hmsc_device_table;
extern uint8_t                          *gp_usbh0_hmsc_config_table;
extern uint8_t                          *gp_usbh0_hmsc_interface_table;
extern uint16_t                         g_usbh0_hmsc_devaddr;
extern uint16_t                         g_usbh0_hmsc_init_seq;

/* Storage Driver */
extern uint16_t                         g_usbh0_hmsc_strg_process;
extern uint16_t                         g_usbh0_hmsc_strg_drive_search_seq;

/*****************************************************************************
Public Functions
******************************************************************************/
/* HMSC driver */
void        R_USBH0_HmscTask(void);
void        usbh0_hmsc_task(void);
void        R_USBH0_HmscStrgDriveTask(void);
void        r_usbh0_hmsc_strg_drive_task(void);
void        usbh0_hmsc_class_check(st_usbh0_utr_t *p_mess);
void        usbh0_hmsc_set_els_cbw(uint8_t *p_cbwcb, uint32_t trans_byte, uint16_t side);
void        usbh0_hmsc_set_rw_cbw(uint16_t command, uint32_t secno, uint16_t seccnt, uint32_t trans_byte, uint16_t side);
void        usbh0_hmsc_clr_data(uint16_t len, uint8_t *p_buff);
uint16_t    usbh0_hmsc_no_data(uint16_t side);
uint16_t    usbh0_hmsc_data_in(uint16_t side, uint8_t *p_buff, uint32_t size);
uint16_t    usbh0_hmsc_data_out(uint16_t side, const uint8_t *buff, uint32_t size);

/* Storage Driver */
void        usbh0_hmsc_strg_drive_search_act(st_usbh0_utr_t *p_mess);

#endif  /* R_USBH0_HMSC_LOCAL_H */
/******************************************************************************
End  Of File
******************************************************************************/
