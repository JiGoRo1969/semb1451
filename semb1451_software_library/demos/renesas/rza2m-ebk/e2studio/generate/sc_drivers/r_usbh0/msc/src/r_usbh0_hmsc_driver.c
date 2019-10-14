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
* File Name    : r_usbh0_hmsc_driver.c
* Description  : USB Host MSC BOT driver
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usbh0_hmsc_if.h"
#include "r_usbh0_hmsc_local.h"


/******************************************************************************
External variables and functions
******************************************************************************/
st_usbh0_hmsc_drive_management_t    g_usbh0_hmsc_drvno_tbl[USBH0_MAXSTRAGE];    /* Drive no. management table */
uint16_t                            g_usbh0_hmsc_in_pipe[USBH0_MAXSTRAGE];      /* Pipenumber */
uint16_t                            g_usbh0_hmsc_out_pipe[USBH0_MAXSTRAGE];     /* Pipenumber */
//st_usbh0_hmsc_cbw_t                 g_usbh0_hmsc_cbw[USBH0_MAXSTRAGE];          /* CBW headder */
//st_usbh0_hmsc_csw_t                 g_usbh0_hmsc_csw[USBH0_MAXSTRAGE];          /* CSW headder */
uint32_t                            g_usbh0_hmsc_cbw_tag_no[USBH0_MAXSTRAGE];   /* CBW tag number */
uint8_t                             g_usbh0_hmsc_sub_class[USBH0_MAXSTRAGE];
uint8_t                             g_usbh0_hmsc_tmp_sub_class;
//uint8_t                             *gp_usbh0_hmsc_device_table;
//uint8_t                             *gp_usbh0_hmsc_config_table;
//uint8_t                             *gp_usbh0_hmsc_interface_table;
uint16_t                            g_usbh0_hmsc_devaddr;
uint16_t                            g_usbh0_hmsc_init_seq = USBH0_SEQ_0;
st_usbh0_utr_t                      g_usbh0_hmsc_class_control;


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */
//static uint8_t          gs_usbh0_hmsc_class_data[USBH0_HMSC_CLSDATASIZE];
static st_usbh0_utr_t   gs_usbh0_hmsc_trans_data[USBH0_MAXSTRAGE];          /* Send data transfer message */
static st_usbh0_utr_t   gs_usbh0_hmsc_receive_data[USBH0_MAXSTRAGE];        /* Receive data transfer message */
static uint32_t         gs_usbh0_hmsc_trans_size;
static uint8_t          *gsp_usbh0_hmsc_buff;
static uint16_t         gs_usbh0_hmsc_process;
static uint16_t         gs_usbh0_hmsc_data_seq = USBH0_SEQ_0;
static uint16_t         gs_usbh0_hmsc_stall_err_seq = USBH0_SEQ_0;
static uint16_t         gs_usbh0_hmsc_csw_err_roop = USBH0_FALSE;

/* functions */
static uint16_t         usbh0_hmsc_data_act(st_usbh0_utr_t *p_mess);
static void             usbh0_hmsc_stall_err(st_usbh0_utr_t *p_mess);
static void             usbh0_hmsc_specified_path(st_usbh0_utr_t *p_mess);
static void             usbh0_hmsc_check_result(st_usbh0_utr_t *p_mess);
static void             usbh0_hmsc_command_result(uint16_t result);
static uint16_t         usbh0_hmsc_get_string_desc(uint16_t addr, uint16_t string, usbh0_utr_cb_t complete);
static uint16_t         usbh0_hmsc_send_cbw(uint16_t side);
static uint16_t         usbh0_hmsc_send_cbw_check(uint16_t side, uint16_t hmsc_retval);
static uint16_t         usbh0_hmsc_send_data(uint16_t side, uint8_t *p_buff, uint32_t size);
static uint16_t         usbh0_hmsc_send_data_check(uint16_t side, uint16_t hmsc_retval);
static uint16_t         usbh0_hmsc_get_data(uint16_t side, uint8_t *p_buff, uint32_t size);
static uint16_t         usbh0_hmsc_get_data_check(uint16_t side, uint16_t hmsc_retval);
static uint16_t         usbh0_hmsc_get_csw(uint16_t side);
static uint16_t         usbh0_hmsc_get_csw_check(uint16_t side, uint16_t hmsc_retval);
static usbh0_er_t       usbh0_hmsc_clear_stall(uint16_t pipe, usbh0_utr_cb_t complete);
static uint16_t         usbh0_hmsc_std_req_check(uint16_t result);


st_usbh0_hmsc_cbw_t     g_usbh0_hmsc_cbw[USBH0_MAXSTRAGE] __attribute__ ((section ("USB_CLASS_0")));    /* CBW headder */
st_usbh0_hmsc_csw_t     g_usbh0_hmsc_csw[USBH0_MAXSTRAGE] __attribute__ ((section ("USB_CLASS_0")));    /* CSW headder */
uint8_t                 *gp_usbh0_hmsc_device_table __attribute__ ((section ("USB_CLASS_0")));
uint8_t                 *gp_usbh0_hmsc_config_table __attribute__ ((section ("USB_CLASS_0")));
uint8_t                 *gp_usbh0_hmsc_interface_table __attribute__ ((section ("USB_CLASS_0")));
static uint8_t          *gsp_usbh0_hmsc_buff __attribute__ ((section ("USB_CLASS_0")));
static uint8_t          gs_usbh0_hmsc_class_data[USBH0_HMSC_CLSDATASIZE] __attribute__ ((section ("USB_CLASS_0")));


/******************************************************************************
Renesas Abstracted HMSC Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : usbh0_hmsc_task
Description     : USB HMSC Task
Arguments       : none
Return value    : none
******************************************************************************/
void usbh0_hmsc_task(void)
{
    st_usbh0_utr_t    *mess;
    usbh0_er_t        err;

    /* Receive message */
    err = R_USBH0_RCV_MSG(USBH0_HMSC_MBX, (usbh0_msg_t**)&mess);
    if(USBH0_OK != err)
    {
        return;
    }

    switch(mess->msginfo)
    {
        case USBH0_MSG_CLS_INIT:
            usbh0_hmsc_class_check(mess);
        break;

        case USBH0_MSG_HMSC_NO_DATA:
        case USBH0_MSG_HMSC_DATA_IN:
        case USBH0_MSG_HMSC_DATA_OUT:
            usbh0_hmsc_data_act(mess);
        break;

        case USBH0_MSG_HMSC_CBW_ERR:
        case USBH0_MSG_HMSC_CSW_PHASE_ERR:
            usbh0_hmsc_stall_err(mess);
        break;

        default:
        break;
    }

    err = R_USBH0_REL_BLK(USBH0_HMSC_MPL,mess);
    if(USBH0_OK != err)
    {
        USBH0_PRINTF0("### USB hmsc Task rel_blk error\n");
    }
}   /* eof usbh0_hmsc_task() */

/******************************************************************************
Function Name   : usbh0_hmsc_classcheck
Description     : check class HMSC SEQ process message
Arguments       : void
Return value    : none
******************************************************************************/
void usbh0_hmsc_classcheck(void)
{
    st_usbh0_utr_t    *p_blf;
    usbh0_er_t        err;

    /* Get mem pool blk */
    err = R_USBH0_PGET_BLK(USBH0_HMSC_MPL, &p_blf);
    if (USBH0_OK == err)
    {
        p_blf->msginfo = USBH0_MSG_CLS_INIT;
        p_blf->result = USBH0_CTRL_END;

        /* Send message */
        err = R_USBH0_SND_MSG(USBH0_HMSC_MBX, (usbh0_msg_t*)p_blf);
        if (USBH0_OK != err)
        {
            err = R_USBH0_REL_BLK(USBH0_HMSC_MPL,p_blf);
            USBH0_PRINTF0("### ClassCheck function snd_msg error\n");
        }
    }
    else
    {
        USBH0_PRINTF0("### ClassCheck function pget_blk error\n");
    }
}    /* eof r_usbh0_hmsc_classcheck() */

/******************************************************************************
Function Name   : usbh0_hmsc_class_check
Description     : check class
Arguments       : st_usbh0_utr_t *mess : message
Return value    : none
******************************************************************************/
void usbh0_hmsc_class_check(st_usbh0_utr_t *p_mess)
{
    uint16_t    retval;
    uint16_t    result;

    result = USBH0_OK;
    switch(g_usbh0_hmsc_init_seq)
    {
        case USBH0_SEQ_0:
            /* Descriptor check */

            /* Check Interface Descriptor (deviceclass) */
            if (USBH0_IFCLS_MAS != gp_usbh0_hmsc_interface_table[USBH0_IF_B_INTERFACECLASS])
            {
                USBH0_PRINTF1("### Interface deviceclass is %x , not support.\n", gp_usbh0_hmsc_interface_table[5]);
                result = USBH0_ERROR;
            }

            /* Check Interface Descriptor (subclass) */
            g_usbh0_hmsc_tmp_sub_class = gp_usbh0_hmsc_interface_table[USBH0_IF_B_INTERFACESUBCLASS];
            if (USBH0_ATAPI == g_usbh0_hmsc_tmp_sub_class)
            {
                USBH0_PRINTF0("    Interface subclass  : SFF-8070i\n");
            }
            else if (USBH0_SCSI == g_usbh0_hmsc_tmp_sub_class)
            {
                USBH0_PRINTF0("    Interface subclass  : SCSI transparent command set\n");
            }
            else if (USBH0_ATAPI_MMC5 == g_usbh0_hmsc_tmp_sub_class)
            {
                USBH0_PRINTF0("    Interface subclass  : ATAPI command set\n");
            }
            else
            {
                USBH0_PRINTF1("### Interface subclass is %x , not support.\n", gp_usbh0_hmsc_interface_table[6]);

                /* Unknown Command */
                result = USBH0_ERROR;
            }

            /* Check Interface Descriptor (protocol) */
            if (USBH0_BOTP == gp_usbh0_hmsc_interface_table[USBH0_IF_B_INTERFACEPROTOCOL])
            {
                USBH0_PRINTF0("    Interface protocol   : BOT \n");
            }
            else
            {
                USBH0_PRINTF1("### Interface protocol is %x , not support.\n", gp_usbh0_hmsc_interface_table[7]);
                result = USBH0_ERROR;
            }

            /* Check Endpoint number */
            if (gp_usbh0_hmsc_interface_table[USBH0_IF_B_NUMENDPOINTS] < 2)
            {
                USBH0_PRINTF1("### Endpoint number is %x , less than 2.\n", gp_usbh0_hmsc_interface_table[4]);
                result = USBH0_ERROR;
            }

            /* Send GetDescriptor(Stirng) */
            if (USBH0_ERROR != result)
            {
/*                retval = usbh0_hmsc_get_string_desc(g_usbh0_hmsc_devaddr, 0, usbh0_hmsc_class_check);*/
                retval = usbh0_hmsc_get_string_desc(g_usbh0_hmsc_devaddr, 0, usbh0_hmsc_classcheck);
                g_usbh0_hmsc_init_seq++;
            }
        break;

        case USBH0_SEQ_1:
            retval = usbh0_hmsc_std_req_check(p_mess->result);
            if (USBH0_ERROR == retval)
            {
                result = USBH0_ERROR;
            }
            else
            {
                /* Send GetDescriptor(Stirng) */
/*
            	retval = usbh0_hmsc_get_string_desc(g_usbh0_hmsc_devaddr, (uint16_t)gp_usbh0_hmsc_device_table[15],
                                                usbh0_hmsc_class_check);
*/
                retval = usbh0_hmsc_get_string_desc(g_usbh0_hmsc_devaddr, (uint16_t)gp_usbh0_hmsc_device_table[15],
                                                usbh0_hmsc_classcheck);
                g_usbh0_hmsc_init_seq++;
            }
        break;

        case USBH0_SEQ_2:
            retval = usbh0_hmsc_std_req_check(p_mess->result);
            if (USBH0_ERROR == retval)
            {
                result = USBH0_ERROR;
            }

            if (USBH0_ERROR != result)
            {
                /* Return to MGR */
                R_USBH0_HstdReturnEnuMGR(retval);
                g_usbh0_hmsc_init_seq = USBH0_SEQ_0;
            }
        break;

        default:
            result = USBH0_ERROR;
        break;
    }

    if (USBH0_ERROR == result)
    {
        /* Return to MGR */
        R_USBH0_HstdReturnEnuMGR(USBH0_ERROR);
        g_usbh0_hmsc_init_seq = USBH0_SEQ_0;
    }
}   /* eof usbh0_hmsc_class_check() */


/******************************************************************************
Function Name   : usbh0_hmsc_set_rw_cbw
Description     : CBW parameter initialization for the READ10/WRITE10 command
Arguments       : uint16_t command :
                : uint32_t secno :
                : uint16_t seccnt :
                : uint32_t trans_byte :
                : uint16_t side :
Return value    : none
******************************************************************************/
void usbh0_hmsc_set_rw_cbw(uint16_t command, uint32_t secno, uint16_t seccnt, uint32_t trans_byte, uint16_t side)
{
    /* CBW parameter set */
    g_usbh0_hmsc_cbw[side].dcbw_tag                       = g_usbh0_hmsc_cbw_tag_no[side];
    g_usbh0_hmsc_cbw[side].dcbw_dtl_lo                    = (uint8_t)trans_byte;
    g_usbh0_hmsc_cbw[side].dcbw_dtl_ml                    = (uint8_t)(trans_byte >> 8);
    g_usbh0_hmsc_cbw[side].dcbw_dtl_mh                    = (uint8_t)(trans_byte >> 16);
    g_usbh0_hmsc_cbw[side].dcbw_dtl_hi                    = (uint8_t)(trans_byte >> 24);
    g_usbh0_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 0;
    g_usbh0_hmsc_cbw[side].bmcbw_flags.reserved7          = 0;
    g_usbh0_hmsc_cbw[side].bcbw_lun.bcbw_lun              = 0;    /* Support LUN0 Only */
    g_usbh0_hmsc_cbw[side].bcbw_lun.reserved4             = 0;
    g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 0;
    g_usbh0_hmsc_cbw[side].bcbw_cb_length.reserved3       = 0;

    /* ATAPI_COMMAND */
    g_usbh0_hmsc_cbw[side].cbw_cb[0] = (uint8_t)command;

    /* LUN */
    g_usbh0_hmsc_cbw[side].cbw_cb[1] = 0x00;

    /* Sector address */
    g_usbh0_hmsc_cbw[side].cbw_cb[2] = (uint8_t)(secno >> 24);
    g_usbh0_hmsc_cbw[side].cbw_cb[3] = (uint8_t)(secno >> 16);
    g_usbh0_hmsc_cbw[side].cbw_cb[4] = (uint8_t)(secno >> 8);
    g_usbh0_hmsc_cbw[side].cbw_cb[5] = (uint8_t)secno;

    /* Reserved */
    g_usbh0_hmsc_cbw[side].cbw_cb[6] = 0x00;

    /* Sector length */
    g_usbh0_hmsc_cbw[side].cbw_cb[7] = (uint8_t)(seccnt >> 8);

    /* Block address */
    g_usbh0_hmsc_cbw[side].cbw_cb[8] = (uint8_t)seccnt;

    /* Control data */
    g_usbh0_hmsc_cbw[side].cbw_cb[9] = (uint8_t)0x00;

    /* ATAPI command check */
    switch (command)
    {
        case USBH0_ATAPI_TEST_UNIT_READY:
        case USBH0_ATAPI_REQUEST_SENSE:
        case USBH0_ATAPI_INQUIRY:
        case USBH0_ATAPI_MODE_SELECT6:
        case USBH0_ATAPI_MODE_SENSE6:
        case USBH0_ATAPI_START_STOP_UNIT:
        case USBH0_ATAPI_PREVENT_ALLOW:
        case USBH0_ATAPI_READ_FORMAT_CAPACITY:
        case USBH0_ATAPI_READ_CAPACITY:
            USBH0_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        /* Initialized READ CBW TAG */
        case USBH0_ATAPI_READ10:
            g_usbh0_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;   /* 10bytes */
        break;

        /* Initialized WRITE CBW TAG */
        case USBH0_ATAPI_WRITE10:
            g_usbh0_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 0;
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;   /* 10bytes */
        break;

        case USBH0_ATAPI_SEEK:
        case USBH0_ATAPI_WRITE_AND_VERIFY:
        case USBH0_ATAPI_VERIFY10:
        case USBH0_ATAPI_MODE_SELECT10:
        case USBH0_ATAPI_MODE_SENSE10:
        default:
            USBH0_PRINTF0("### Non-mounted command demand generating !\n");
        break;
    }

    if (USBH0_ATAPI == g_usbh0_hmsc_sub_class[side])
    {
        g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length = USBH0_MSC_CBWCB_LENGTH;  /* 12bytes */
    }
}   /* eof usbh0_hmsc_set_rw_cbw() */

/******************************************************************************
Function Name   : usbh0_hmsc_set_els_cbw
Description     : CBW parameter initialization for other commands
Arguments       : uint8_t *cbwcb :
                : uint32_t trans_byte :
                : uint16_t side :
Return value    : none
******************************************************************************/
void usbh0_hmsc_set_els_cbw(uint8_t *p_cbwcb, uint32_t trans_byte, uint16_t side)
{
    uint8_t     i;

    /* CBW parameter set */
    g_usbh0_hmsc_cbw[side].dcbw_tag       = g_usbh0_hmsc_cbw_tag_no[side];
    g_usbh0_hmsc_cbw[side].dcbw_dtl_lo    = (uint8_t)trans_byte;
    g_usbh0_hmsc_cbw[side].dcbw_dtl_ml    = (uint8_t)(trans_byte >> 8);
    g_usbh0_hmsc_cbw[side].dcbw_dtl_mh    = (uint8_t)(trans_byte >> 16);
    g_usbh0_hmsc_cbw[side].dcbw_dtl_hi    = (uint8_t)(trans_byte >> 24);

    /* Receive */
    g_usbh0_hmsc_cbw[side].bmcbw_flags.cbw_dir        = 0;
    g_usbh0_hmsc_cbw[side].bmcbw_flags.reserved7      = 0;
    g_usbh0_hmsc_cbw[side].bcbw_lun.bcbw_lun          = 0;    /*** Support LUN0 ONLY ***/
    g_usbh0_hmsc_cbw[side].bcbw_lun.reserved4         = 0;
    g_usbh0_hmsc_cbw[side].bcbw_cb_length.reserved3   = 0;

    for (i = 0; i < 12; i++)
    {
        g_usbh0_hmsc_cbw[side].cbw_cb[i] = p_cbwcb[i];
    }

    /* ATAPI command check */
    switch (p_cbwcb[0])
    {
        case USBH0_ATAPI_TEST_UNIT_READY:
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH0_ATAPI_REQUEST_SENSE:
            g_usbh0_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH0_ATAPI_FORMAT_UNIT:
            USBH0_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        case USBH0_ATAPI_INQUIRY:
            g_usbh0_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH0_ATAPI_MODE_SELECT6:
        case USBH0_ATAPI_MODE_SENSE6:
        break;

        case USBH0_ATAPI_START_STOP_UNIT:
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH0_ATAPI_PREVENT_ALLOW:
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 6;
        break;

        case USBH0_ATAPI_READ_FORMAT_CAPACITY:
            g_usbh0_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;
        break;

        case USBH0_ATAPI_READ_CAPACITY:
            g_usbh0_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;
        break;

        case USBH0_ATAPI_READ10:
        case USBH0_ATAPI_WRITE10:
            USBH0_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        case USBH0_ATAPI_SEEK:
        case USBH0_ATAPI_WRITE_AND_VERIFY:
        case USBH0_ATAPI_VERIFY10:
            USBH0_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        case USBH0_ATAPI_MODE_SELECT10:
            USBH0_PRINTF0("### Non-mounted command demand generating !\n");
        break;

        case USBH0_ATAPI_MODE_SENSE10:
            g_usbh0_hmsc_cbw[side].bmcbw_flags.cbw_dir            = 1;
            g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length  = 10;
        break;

        default:
            USBH0_PRINTF0("### Non-mounted command demand generating !\n");
        break;
    }

    if (USBH0_ATAPI == g_usbh0_hmsc_sub_class[side])
    {
        g_usbh0_hmsc_cbw[side].bcbw_cb_length.bcbw_cb_length = USBH0_MSC_CBWCB_LENGTH;  /* 12bytes */
    }
}   /* eof usbh0_hmsc_set_els_cbw() */

/******************************************************************************
Function Name   : usbh0_hmsc_clr_data
Description     : data clear
Arguments       : uint16_t len :
                : uint8_t *buf :
Return value    : none
******************************************************************************/
void usbh0_hmsc_clr_data(uint16_t len, uint8_t *p_buf)
{
    uint16_t    i;

    for (i = 0; i < len; ++i)
    {
        *p_buf++ = 0x00;
    }
}   /* eof usbh0_hmsc_clr_data() */

/******************************************************************************
Function Name   : usbh0_hmsc_no_data
Description     : HMSC No data
Arguments       : uint16_t side :
Return value    : uint16_t :
******************************************************************************/
uint16_t usbh0_hmsc_no_data(uint16_t side)
{
    st_usbh0_utr_t    mess;

    mess.keyword = side;
    gs_usbh0_hmsc_process = USBH0_MSG_HMSC_NO_DATA;

    usbh0_hmsc_specified_path(&mess);

    gs_usbh0_hmsc_data_seq = USBH0_SEQ_0;

    return USBH0_OK;
}   /* eof usbh0_hmsc_no_data() */

/******************************************************************************
Function Name   : usbh0_hmsc_data_in
Description     : HMSC Data In
Arguments       : uint16_t side :
                : uint8_t *buff :
                : uint32_t size :
Return value    : uint16_t :
******************************************************************************/
uint16_t usbh0_hmsc_data_in(uint16_t side, uint8_t *p_buff, uint32_t size)
{
    st_usbh0_utr_t    mess;

    mess.keyword = side;
    gsp_usbh0_hmsc_buff = p_buff;
    gs_usbh0_hmsc_trans_size = size;
    gs_usbh0_hmsc_process = USBH0_MSG_HMSC_DATA_IN;

    usbh0_hmsc_specified_path(&mess);

    gs_usbh0_hmsc_data_seq = USBH0_SEQ_0;

    return USBH0_OK;
}   /* eof usbh0_hmsc_data_in() */

/******************************************************************************
Function Name   : usbh0_hmsc_data_out
Description     : HMSC Data Out
Arguments       : uint16_t side :
                : uint8_t *buff :
                : uint32_t size :
Return value    : uint16_t :
******************************************************************************/
uint16_t usbh0_hmsc_data_out(uint16_t side, const uint8_t *p_buff, uint32_t size)
{
    st_usbh0_utr_t    mess;

    mess.keyword = side;
    gsp_usbh0_hmsc_buff = (uint8_t *)p_buff;
    gs_usbh0_hmsc_trans_size = size;
    gs_usbh0_hmsc_process = USBH0_MSG_HMSC_DATA_OUT;

    usbh0_hmsc_specified_path(&mess);

    gs_usbh0_hmsc_data_seq = USBH0_SEQ_0;

    return USBH0_OK;
}   /* eof usbh0_hmsc_data_out() */

/******************************************************************************
Function Name   : usbh0_hmsc_data_act
Description     : Send Data request
Arguments       : st_usbh0_utr_t *mess :
Return value    : uint16_t :
******************************************************************************/
static uint16_t usbh0_hmsc_data_act(st_usbh0_utr_t *p_mess)
{
    static uint16_t side;
    uint16_t        hmsc_retval;
    uint16_t        result;
    uint8_t         *p_buff;
    uint32_t        size;

    p_buff = gsp_usbh0_hmsc_buff;
    size = gs_usbh0_hmsc_trans_size;
    result = p_mess->result;

    switch (gs_usbh0_hmsc_data_seq)
    {
        case USBH0_SEQ_0:    /* CBW */
            side = p_mess->keyword;
            hmsc_retval = usbh0_hmsc_send_cbw(side);
            gs_usbh0_hmsc_data_seq++;
        break;

        case USBH0_SEQ_1:    /* Check CBW and Send Data */
            hmsc_retval = usbh0_hmsc_send_cbw_check(side, result);
            if (USBH0_HMSC_STALL == hmsc_retval)
            {
                gs_usbh0_hmsc_process = USBH0_MSG_HMSC_CBW_ERR;
                gs_usbh0_hmsc_stall_err_seq = USBH0_SEQ_0;
                p_mess->keyword = g_usbh0_hmsc_drvno_tbl[side].devadr;
                usbh0_hmsc_specified_path(p_mess);
                gs_usbh0_hmsc_data_seq = USBH0_SEQ_0;
            }
            else if (USBH0_HMSC_OK != hmsc_retval)
            {
                USBH0_PRINTF1("### Data : SendCBW error(drive:%d) \n", side);
                gs_usbh0_hmsc_data_seq = USBH0_SEQ_0;
                usbh0_hmsc_command_result(hmsc_retval);
            }
            else    /* Success */
            {
                switch (gs_usbh0_hmsc_process)
                {
                    case USBH0_MSG_HMSC_NO_DATA:
                        hmsc_retval = usbh0_hmsc_get_csw(side);
                        gs_usbh0_hmsc_data_seq = USBH0_SEQ_4;
                    break;

                    case USBH0_MSG_HMSC_DATA_OUT:
                        usbh0_hmsc_send_data(side, p_buff, size);
                        gs_usbh0_hmsc_data_seq++;
                    break;

                    case USBH0_MSG_HMSC_DATA_IN:
                        usbh0_hmsc_get_data(side, p_buff, size);
                        gs_usbh0_hmsc_data_seq++;
                    break;
                }
            }
        break;

        case USBH0_SEQ_2:   /* Check Data and Send CSW*/
            if (USBH0_MSG_HMSC_DATA_OUT == gs_usbh0_hmsc_process)
            {
                hmsc_retval = usbh0_hmsc_send_data_check(side, result);
            }
            else if (USBH0_MSG_HMSC_DATA_IN == gs_usbh0_hmsc_process)
            {
                hmsc_retval = usbh0_hmsc_get_data_check(side, result);
            }

            if (USBH0_HMSC_STALL == hmsc_retval)
            {
                if (USBH0_MSG_HMSC_DATA_OUT == gs_usbh0_hmsc_process)
                {
                    usbh0_hmsc_clear_stall(g_usbh0_hmsc_out_pipe[side], usbh0_hmsc_check_result);
                }
                else if (USBH0_MSG_HMSC_DATA_IN == gs_usbh0_hmsc_process)
                {
                    usbh0_hmsc_clear_stall(g_usbh0_hmsc_in_pipe[side], usbh0_hmsc_check_result);
                }

                gs_usbh0_hmsc_data_seq++;
            }
            else if (USBH0_HMSC_OK != hmsc_retval)
            {
                USBH0_PRINTF1("### Data : SendData error(drive:%d) \n", side);
                usbh0_hmsc_command_result(hmsc_retval);
                gs_usbh0_hmsc_data_seq = USBH0_SEQ_0;
            }
            else    /* Success */
            {
                hmsc_retval = usbh0_hmsc_get_csw(side);
                gs_usbh0_hmsc_data_seq = USBH0_SEQ_4;
            }
        break;

        case USBH0_SEQ_3:    /* Check ClearStall and Send CSW */
            usbh0_hmsc_std_req_check(result);
            usbh0_hmsc_get_csw(side);
            gs_usbh0_hmsc_data_seq++;
        break;

        case USBH0_SEQ_4:    /* Check CSW */
            hmsc_retval = usbh0_hmsc_get_csw_check(side, result);
            switch (hmsc_retval)
            {
                case USBH0_HMSC_OK:    /* Success */
                    if (USBH0_TRUE == gs_usbh0_hmsc_csw_err_roop)
                    {
                        gs_usbh0_hmsc_csw_err_roop = USBH0_FALSE;
                        hmsc_retval = USBH0_HMSC_CSW_ERR;
                    }
                    usbh0_hmsc_command_result(hmsc_retval);
                break;

                case USBH0_HMSC_CSW_ERR:
                    USBH0_PRINTF1("*** Data : CSW-NG(drive:%d) \n", side);
                    gs_usbh0_hmsc_csw_err_roop = USBH0_TRUE;
                    R_USBH0_HmscRequestSense(side, p_buff);
                break;

                case USBH0_HMSC_STALL:
                    USBH0_PRINTF1("*** Data : CSW-STALL(drive:%d) \n", side);
                    gs_usbh0_hmsc_process = USBH0_MSG_HMSC_CSW_PHASE_ERR;
                    gs_usbh0_hmsc_stall_err_seq = USBH0_SEQ_0;
                    p_mess->keyword = g_usbh0_hmsc_drvno_tbl[side].devadr;
                    usbh0_hmsc_specified_path(p_mess);
                break;

                case USBH0_MSC_CSW_PHASE_ERR:
                    USBH0_PRINTF1("*** Data : CSW-PhaseError(drive:%d) \n", side);
                    gs_usbh0_hmsc_process = USBH0_MSG_HMSC_CSW_PHASE_ERR;
                    gs_usbh0_hmsc_stall_err_seq = USBH0_SEQ_0;
                    p_mess->keyword = g_usbh0_hmsc_drvno_tbl[side].devadr;
                    usbh0_hmsc_specified_path(p_mess);
                break;

                default:
                    if (USBH0_TRUE == gs_usbh0_hmsc_csw_err_roop)
                    {
                        gs_usbh0_hmsc_csw_err_roop = USBH0_FALSE;
                        hmsc_retval = USBH0_HMSC_CSW_ERR;
                    }
                    USBH0_PRINTF1("### Data : GetCSW error(drive:%d) \n", side);
                    usbh0_hmsc_command_result(hmsc_retval);
                break;
            }

            gs_usbh0_hmsc_data_seq = USBH0_SEQ_0;
        break;

        default:
            gs_usbh0_hmsc_data_seq = USBH0_SEQ_0;
            usbh0_hmsc_command_result(hmsc_retval);
        break;
    }

    return hmsc_retval;   /* Data read error */
}   /* eof usbh0_hmsc_data_act() */

/******************************************************************************
Function Name   : usbh0_hmsc_stall_err
Description     : HMSC Stall Error
Arguments       : st_usbh0_utr_t *mess :
Return value    : none
******************************************************************************/
static void usbh0_hmsc_stall_err(st_usbh0_utr_t *p_mess)
{
    static uint16_t devadr;
    uint16_t        side;
    uint16_t        result;

    result = p_mess->result;

    switch (gs_usbh0_hmsc_stall_err_seq)
    {
        case USBH0_SEQ_0:
            devadr = p_mess->keyword;
            R_USBH0_HmscMassStorageReset(devadr, usbh0_hmsc_check_result);
            gs_usbh0_hmsc_stall_err_seq++;
        break;

        case USBH0_SEQ_1:
            usbh0_hmsc_std_req_check(result);
            R_USBH0_HmscRefDrvNo(&side, devadr);
            usbh0_hmsc_clear_stall(g_usbh0_hmsc_out_pipe[side], usbh0_hmsc_check_result);
            gs_usbh0_hmsc_stall_err_seq++;
        break;

        case USBH0_SEQ_2:
            R_USBH0_HmscRefDrvNo(&side, devadr);
            usbh0_hmsc_clear_stall(g_usbh0_hmsc_in_pipe[side], usbh0_hmsc_check_result);
            gs_usbh0_hmsc_stall_err_seq++;
        break;

        case USBH0_SEQ_3:
            if (USBH0_MSG_HMSC_CSW_PHASE_ERR == p_mess->msginfo)
            {
                result = USBH0_HMSC_CSW_PHASE_ERR;
            }
            else
            {
                result = USBH0_HMSC_CBW_ERR;
            }

            usbh0_hmsc_command_result(result);
            gs_usbh0_hmsc_stall_err_seq = USBH0_SEQ_0;
        break;

        default:
            if (USBH0_MSG_HMSC_CSW_PHASE_ERR == p_mess->msginfo)
            {
                result = USBH0_HMSC_CSW_PHASE_ERR;
            }
            else
            {
                result = USBH0_HMSC_CBW_ERR;
            }
            usbh0_hmsc_command_result(result);
            gs_usbh0_hmsc_stall_err_seq = USBH0_SEQ_0;
        break;
    }
}   /* eof usbh0_hmsc_stall_err() */

/******************************************************************************
Function Name   : usbh0_hmsc_specified_path
Description     : Next Process Selector
Arguments       : st_usbh0_utr_t *mess :
Return value    : none
******************************************************************************/
static void usbh0_hmsc_specified_path(st_usbh0_utr_t *p_mess)
{
    st_usbh0_utr_t    *p_blf;
    usbh0_er_t        err;

    /* Get mem pool blk */
    err = R_USBH0_PGET_BLK(USBH0_HMSC_MPL, &p_blf);
    if (USBH0_OK == err)
    {
        p_blf->msginfo  = gs_usbh0_hmsc_process;
        p_blf->keyword  = p_mess->keyword;
        p_blf->result   = p_mess->result;

        /* Send message */
        err = R_USBH0_SND_MSG(USBH0_HMSC_MBX, (usbh0_msg_t*)p_blf);
        if (USBH0_OK != err)
        {
            err = R_USBH0_REL_BLK(USBH0_HMSC_MPL,p_blf);
            USBH0_PRINTF0("### SpecifiedPass function snd_msg error\n");
        }
    }
    else
    {
        USBH0_PRINTF0("### SpecifiedPass function pget_blk error\n");
    }
}   /* eof usbh0_hmsc_specified_path() */

/******************************************************************************
Function Name   : usbh0_hmsc_check_result
Description     : Hub class check result
Arguments       : st_usbh0_utr_t *mess :
Return value    : none
******************************************************************************/
static void usbh0_hmsc_check_result(st_usbh0_utr_t *p_mess)
{
    usbh0_hmsc_specified_path(p_mess);
}   /* eof usbh0_hmsc_check_result() */

/******************************************************************************
Function Name   : usbh0_hmsc_command_result
Description     : Hub class check result
Arguments       : uint16_t result     : Result
Return value    : none
******************************************************************************/
static void usbh0_hmsc_command_result(uint16_t result)
{
    st_usbh0_utr_t    *p_blf;
    usbh0_er_t        err;

    /* Get mem pool blk */
    err = R_USBH0_PGET_BLK(USBH0_HSTRG_MPL, &p_blf);
    if (USBH0_OK == err)
    {
        p_blf->msginfo  = g_usbh0_hmsc_strg_process;
        p_blf->result   = result;

        /* Send message */
        err = R_USBH0_SND_MSG(USBH0_HSTRG_MBX, (usbh0_msg_t*)p_blf);
        if (USBH0_OK != err)
        {
            err = R_USBH0_REL_BLK(USBH0_HSTRG_MPL, p_blf);
            USBH0_PRINTF0("### CheckResult function snd_msg error\n");
        }
    }
    else
    {
        USBH0_PRINTF0("### CheckResult function pget_blk error\n");
    }
}   /* eof usbh0_hmsc_command_result() */


/******************************************************************************
Function Name   : usbh0_hmsc_get_string_desc
Description     : Set GetDescriptor
Arguments       : uint16_t addr            : device address
                : uint16_t string          : descriptor index
                : usbh0_utr_cb_t complete    : callback function
Return value    : uint16_t                 : error info
******************************************************************************/
static uint16_t usbh0_hmsc_get_string_desc(uint16_t addr, uint16_t string, usbh0_utr_cb_t complete)
{
    static st_usbh0_setup_t   string_setup;
    uint16_t                i;

    if (0 == string)
    {
        string_setup.index  = (uint16_t)0x0000;
        string_setup.length = (uint16_t)0x0004;
    }
    else
    {
        /* Set LanguageID */
        string_setup.index  = (uint16_t)(gs_usbh0_hmsc_class_data[2]);
        string_setup.index |= (uint16_t)((uint16_t)(gs_usbh0_hmsc_class_data[3]) << 8);
        string_setup.length = (uint16_t)USBH0_HMSC_CLSDATASIZE;
    }

    string_setup.type       = (USBH0_GET_DESCRIPTOR | USBH0_DEV_TO_HOST | USBH0_STANDARD | USBH0_DEVICE);
    string_setup.value      = (uint16_t)(USBH0_STRING_DESCRIPTOR + string);
    string_setup.devaddr    = addr;

    for (i = 0; i < string_setup.length; i++)
    {
        gs_usbh0_hmsc_class_data[i] = (uint8_t)0xFF;
    }

    g_usbh0_hmsc_class_control.p_tranadr    = (void *)gs_usbh0_hmsc_class_data;
    g_usbh0_hmsc_class_control.complete   = complete;
    g_usbh0_hmsc_class_control.tranlen    = (uint32_t)string_setup.length;
    g_usbh0_hmsc_class_control.keyword    = USBH0_PIPE0;
    g_usbh0_hmsc_class_control.p_setup      = &string_setup;

    return (R_USBH0_HstdTransferStart(&g_usbh0_hmsc_class_control));
}   /* eof usbh0_hmsc_get_string_desc() */

/******************************************************************************
Function Name   : usbh0_hmsc_send_cbw
Description     : Send CBW
Arguments       : uint16_t side           : Drive Number
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh0_hmsc_send_cbw(uint16_t side)
{
    usbh0_er_t    err;

    /* Set CBW TAG usbh0_hmsc_CbwTagCount()*/
    g_usbh0_hmsc_cbw_tag_no[side]++;
    if ((uint16_t)0 == g_usbh0_hmsc_cbw_tag_no[side])
    {
        g_usbh0_hmsc_cbw_tag_no[side] = (uint16_t)1;
    }

    gs_usbh0_hmsc_trans_data[side].keyword    = g_usbh0_hmsc_out_pipe[side];    /* Device number */
    gs_usbh0_hmsc_trans_data[side].p_tranadr    = (void*)&g_usbh0_hmsc_cbw[side]; /* Transfer data address */
    gs_usbh0_hmsc_trans_data[side].tranlen    = (uint32_t)USBH0_MSC_CBWLENGTH;  /* Transfer data length */
    gs_usbh0_hmsc_trans_data[side].complete   = &usbh0_hmsc_check_result;        /* Call Back Function Info */

    err = R_USBH0_HstdTransferStart(&gs_usbh0_hmsc_trans_data[side]);
    if (USBH0_OK != err)
    {
        USBH0_PRINTF1("### Mass Storage Device Class submit error(drive:%d) !\n", side);
        return USBH0_HMSC_SUBMIT_ERR;
    }

    return err;
}   /* eof usbh0_hmsc_send_cbw() */

/******************************************************************************
Function Name   : usbh0_hmsc_send_cbw_check
Description     : Check send CBW
Arguments       : uint16_t side           : Drive Number
                : uint16_t result         : Transfer Result
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh0_hmsc_send_cbw_check(uint16_t side, uint16_t result)
{
    switch (result)
    {
        case USBH0_DATA_NONE:     /* Send CBW */
            return USBH0_HMSC_OK;
        break;

        case USBH0_DATA_STALL:    /* Stall */
            USBH0_PRINTF1("*** CBW Transfer STALL(drive:%d) !\n", side);
            return USBH0_HMSC_STALL;;
        break;

        case USBH0_DATA_ERR:
            USBH0_PRINTF1("### CBW Transfer ERROR(drive:%d) !\n", side);
        break;

        default:
            USBH0_PRINTF1("### CBW Transfer error(drive:%d) !\n", side);
        break;
    }

    return USBH0_HMSC_CBW_ERR;
}   /* eof usbh0_hmsc_send_cbw_check() */

/******************************************************************************
Function Name   : usbh0_hmsc_get_data
Description     : Receive Data request
Arguments       : uint16_t side           : Drive Number
                : uint8_t *buff           : Receive Data Buffer Address
                : uint32_t size           : Receive Data Size
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh0_hmsc_get_data(uint16_t side, uint8_t *p_buff, uint32_t size)
{
    usbh0_er_t    err;

    gs_usbh0_hmsc_receive_data[side].keyword  = g_usbh0_hmsc_in_pipe[side]; /* Device number */
    gs_usbh0_hmsc_receive_data[side].p_tranadr  = (void*)p_buff;            /* Transfer data address */
    gs_usbh0_hmsc_receive_data[side].tranlen  = size;                     /* Transfer data length */
    gs_usbh0_hmsc_receive_data[side].complete = &usbh0_hmsc_check_result;    /* Call Back Function Info */

    err = R_USBH0_HstdTransferStart(&gs_usbh0_hmsc_receive_data[side]);
    if (USBH0_OK != err)
    {
        USBH0_PRINTF1("### Mass Storage Device Class submit error(drive:%d) !\n", side);
        return USBH0_HMSC_SUBMIT_ERR;
    }

    return err;
}   /* eof usbh0_hmsc_get_data() */


/******************************************************************************
Function Name   : usbh0_hmsc_get_data_check
Description     : Check Get Data
Arguments       : uint16_t side           : Drive Number
                : uint16_t result         : Transfer Result
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh0_hmsc_get_data_check(uint16_t side, uint16_t result)
{
    switch (result)
    {
        case USBH0_DATA_SHT:
            /* Continue */
        case USBH0_DATA_OK:
            return USBH0_HMSC_OK;
        break;

        case USBH0_DATA_STALL:
            USBH0_PRINTF1("*** GetData STALL(drive:%d) !\n", side);
            return USBH0_HMSC_STALL;
        break;

        case USBH0_DATA_ERR:
            USBH0_PRINTF1("### GetData ERROR(drive:%d) !\n", side);
        break;

        case USBH0_DATA_OVR:
            USBH0_PRINTF1("### GetData over(drive:%d) !\n", side);
        break;

        default:
            USBH0_PRINTF1("### GetData error(drive:%d) !\n", side);
        break;
    }

    return USBH0_HMSC_DAT_RD_ERR;
}   /* eof usbh0_hmsc_get_data_check() */

/******************************************************************************
Function Name   : usbh0_hmsc_send_data
Description     : Send Pipe Data
Arguments       : uint16_t side           : Drive Number
                : uint8_t  *buff          : Data Info Address
                : uint32_t size           : Data Size
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh0_hmsc_send_data(uint16_t side, uint8_t *p_buff, uint32_t size)
{
    usbh0_er_t    err;

    gs_usbh0_hmsc_trans_data[side].keyword    = g_usbh0_hmsc_out_pipe[side];    /* Device number */
    gs_usbh0_hmsc_trans_data[side].p_tranadr  = (void*)p_buff;                /* Transfer data address */
    gs_usbh0_hmsc_trans_data[side].tranlen    = size;                         /* Transfer data length */
    gs_usbh0_hmsc_trans_data[side].complete   = &usbh0_hmsc_check_result;        /* Call Back Function Info */

    err = R_USBH0_HstdTransferStart(&gs_usbh0_hmsc_trans_data[side]);
    if (USBH0_OK != err)
    {
        USBH0_PRINTF1("### Mass Storage Device Class submit error(drive:%d) !\n", side);
        return USBH0_HMSC_SUBMIT_ERR;
    }

    return err;
}   /* eof usbh0_hmsc_send_data() */

/******************************************************************************
Function Name   : usbh0_hmsc_send_data_check
Description     : Check Send Data
Arguments       : uint16_t side           : Drive Number
                : uint16_t result         : Transfer Result
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh0_hmsc_send_data_check(uint16_t side, uint16_t result)
{
    switch (result)
    {
        case USBH0_DATA_NONE:
            return USBH0_HMSC_OK;
        break;

        case USBH0_DATA_STALL:
            USBH0_PRINTF1("*** SendData STALL(drive:%d) !\n", side);
            return USBH0_HMSC_STALL;
        break;

        case USBH0_DATA_ERR:
            USBH0_PRINTF1("### SendData ERROR(drive:%d) !\n", side);
        break;

        default:
            USBH0_PRINTF1("### SendData error(drive:%d) !\n", side);
        break;
    }

    return USBH0_HMSC_DAT_WR_ERR;
}   /* eof usbh0_hmsc_send_data_check() */

/******************************************************************************
Function Name   : usbh0_hmsc_get_csw
Description     : Receive CSW
Arguments       : uint16_t side           : Drive Number
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh0_hmsc_get_csw(uint16_t side)
{
    usbh0_er_t    err;

    gs_usbh0_hmsc_receive_data[side].keyword  = g_usbh0_hmsc_in_pipe[side];     /* Device number */
    gs_usbh0_hmsc_receive_data[side].p_tranadr  = (void*)&g_usbh0_hmsc_csw[side]; /* Transfer data address */
    gs_usbh0_hmsc_receive_data[side].tranlen  = (uint32_t)USBH0_MSC_CSW_LENGTH; /* Transfer data length */
    gs_usbh0_hmsc_receive_data[side].complete = &usbh0_hmsc_check_result;        /* Call Back Function Info */

    err = R_USBH0_HstdTransferStart(&gs_usbh0_hmsc_receive_data[side]);
    if (USBH0_OK != err)
    {
        USBH0_PRINTF1("### Mass Storage Device Class submit error(drive:%d) !\n", side);
        return USBH0_HMSC_SUBMIT_ERR;
    }

    return err;
}   /* eof usbh0_hmsc_get_csw() */

/******************************************************************************
Function Name   : usbh0_hmsc_get_csw_check
Description     : Check Receive CSW
Arguments       : uint16_t side           : Drive Number
                : uint16_t result         : Transfer Result
Return value    : uint16_t                : Error Code
******************************************************************************/
static uint16_t usbh0_hmsc_get_csw_check(uint16_t side, uint16_t result)
{
    switch (result)
    {
        case USBH0_DATA_SHT:
            /* Continue */
        case USBH0_DATA_OK:
            /* CSW Check */
            if (USBH0_MSC_CSW_SIGNATURE != g_usbh0_hmsc_csw[side].dcsw_signature)
            {
                USBH0_PRINTF2("### CSW signature error 0x%08x:SIGN=0x%08x.\n",
                                g_usbh0_hmsc_csw[side].dcsw_signature, USBH0_MSC_CSW_SIGNATURE);
                return USBH0_HMSC_CSW_ERR;
            }

            if (g_usbh0_hmsc_csw[side].dcsw_tag != g_usbh0_hmsc_cbw[side].dcbw_tag)
            {
                USBH0_PRINTF2("### CSW Tag error 0x%08x:CBWTAG=0x%08x.\n",
                                g_usbh0_hmsc_csw[side].dcsw_tag, g_usbh0_hmsc_cbw[side].dcbw_tag);
                return USBH0_HMSC_CSW_ERR;
            }

            switch (g_usbh0_hmsc_csw[side].bcsw_status)
            {
                case USBH0_MSC_CSW_OK:
                    return USBH0_HMSC_OK;
                break;

                case USBH0_MSC_CSW_NG:
                    return USBH0_HMSC_CSW_ERR;
                break;

                case USBH0_MSC_CSW_PHASE_ERR:
                    return USBH0_HMSC_CSW_PHASE_ERR;
                break;

                default:
                    USBH0_PRINTF1("### CSW status error 0x%2x.\n", g_usbh0_hmsc_csw[side].bcsw_status);
                    return USBH0_HMSC_CSW_ERR;
                break;
            }
        break;

        case USBH0_DATA_STALL:
            /* Stall */
            USBH0_PRINTF1("*** GetCSW Transfer STALL(drive:%d) !\n", side);
            return USBH0_HMSC_STALL;;
        break;

        case USBH0_DATA_ERR:
            USBH0_PRINTF1("### GetCSW Transfer ERROR(drive:%d) !\n", side);
        break;

        case USBH0_DATA_OVR:
            USBH0_PRINTF1("### GetCSW receive over(drive:%d) !\n", side);
        break;

        default:
            USBH0_PRINTF1("### GetCSW Transfer error(drive:%d) !\n", side);
        break;
    }

    return USBH0_HMSC_CSW_ERR;
}   /* eof usbh0_hmsc_get_csw_check() */

/******************************************************************************
Function Name   : usbh0_hmsc_clear_stall
Description     : Clear Stall
Arguments       : uint16_t     pipe     : PipeID
                : usbh0_utr_cb_t complete : Callback function
Return value    : uint16_t
******************************************************************************/
static usbh0_er_t usbh0_hmsc_clear_stall(uint16_t pipe, usbh0_utr_cb_t complete)
{
    usbh0_er_t    err;

    err = R_USBH0_HstdChangeDeviceState(complete, USBH0_DO_CLR_STALL, pipe);

    return err;
}   /* eof usbh0_hmsc_clear_stall() */

/******************************************************************************
Function Name   : usbh0_hmsc_std_req_check
Description     : Sample Standard Request Check
Arguments       : uint16_t result        : Transfer Result
Return value    : uint16_t               : error info
******************************************************************************/
static uint16_t usbh0_hmsc_std_req_check(uint16_t result)
{
    if (USBH0_DATA_STALL == result)
    {
        USBH0_PRINTF0("*** Standard Request STALL !\n");
        return USBH0_ERROR;
    }
    else if (USBH0_CTRL_END != result)
    {
        USBH0_PRINTF0("*** Standard Request error !\n");
        return USBH0_ERROR;
    }
    else
    {
        return USBH0_OK;
    }
}   /* eof usbh0_hmsc_std_req_check() */

/******************************************************************************
End  Of File
******************************************************************************/
