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
* File Name    : r_usbh0_hstorage_driver.c
* Description  : USB Host Storage Driver API
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usbh0_hmsc_if.h"
#include "r_usbh0_hatapi_define.h"
#include "r_usbh0_hmsc_local.h"

#if (BSP_CFG_RTOS_USED == 1)
#include "r_os_abstraction_api.h"
#endif

/******************************************************************************
External variables and functions
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static usbh0_utr_cb_t   gs_usbh0_hmsc_command_result;
static uint16_t         gs_usbh0_hmsc_root_devaddr;
static uint16_t         gs_usbh0_hmsc_command_end_flag;

/******************************************************************************
Renesas Storage Driver API functions
******************************************************************************/


/******************************************************************************
Function Name   : R_USBH0_HmscStrgDriveTask
Description     : Storage drive task
Arguments       : none
Return value    : none
******************************************************************************/
void R_USBH0_HmscStrgDriveTask(void)
{
#if (BSP_CFG_RTOS_USED == 1)
	st_usbh0_utr_t  *p_mess;
	while(1)
	{
		USBH0_RTOS_RCV_MSG(USBH0_HSTRG_MSG, (usbh0_msg_t **)&p_mess);
		r_usbh0_hmsc_strg_drive_task();
		g_usbh0_msg_check &= ~(1<<USBH0_HSTRG_TSK);
	}
#else /* (BSP_CFG_RTOS_USED == 1) */
	r_usbh0_hmsc_strg_drive_task();
#endif /* (BSP_CFG_RTOS_USED == 1) */
}   /* eof R_USBH0_HmscStrgDriveTask() */

/******************************************************************************
Function Name   : r_usbh0_hmsc_strg_drive_task
Description     : Storage drive task
Arguments       : none
Return value    : none
******************************************************************************/
void r_usbh0_hmsc_strg_drive_task(void)
{
    st_usbh0_utr_t    *p_mess;
    usbh0_er_t        err;

    /* receive message */
    err = R_USBH0_RCV_MSG(USBH0_HSTRG_MBX, (usbh0_msg_t**)&p_mess);
    if (USBH0_OK != err)
    {
        return;
    }

    switch(p_mess->msginfo)
    {
        case USBH0_MSG_HMSC_STRG_DRIVE_SEARCH:
            usbh0_hmsc_strg_drive_search_act(p_mess);    /* Start Drive search */
        break;

        case USBH0_MSG_HMSC_STRG_DRIVE_SEARCH_END:
            g_usbh0_hmsc_strg_process = 0;
            p_mess->keyword = gs_usbh0_hmsc_root_devaddr;
            (gs_usbh0_hmsc_command_result)(p_mess);
        break;

        case USBH0_MSG_HMSC_STRG_USER_COMMAND:
            (gs_usbh0_hmsc_command_result)(p_mess);   /* R_USBH0_HmscStrgUserCommand Result */
        break;

        case USBH0_MSG_HMSC_STRG_RW_END:
            if (USBH0_HMSC_OK == p_mess->result)
            {
                gs_usbh0_hmsc_command_end_flag = USBH0_TRUE;
            }
            else
            {
                gs_usbh0_hmsc_command_end_flag = USBH0_ERROR;
            }

        break;

        default:
        break;
    }

    err = R_USBH0_REL_BLK(USBH0_HSTRG_MPL, p_mess);
    if( err != USBH0_OK )
    {
        USBH0_PRINTF0("### USB Strg Task rel_blk error\n");
    }
}  /* eof r_usbh0_hmsc_strg_drive_task() */


/******************************************************************************
Function Name   : R_USBH0_HmscStrgDriveSearch
Description     : Searches drive SndMsg
Arguments       : uint16_t addr    : Device Address
Return value    : uint16_t         : [DONE/ERROR]
******************************************************************************/
uint16_t R_USBH0_HmscStrgDriveSearch(uint16_t addr, usbh0_utr_cb_t complete)
{
    st_usbh0_utr_t    *p_blf;
    usbh0_er_t        err;

    gs_usbh0_hmsc_root_devaddr = addr;
    gs_usbh0_hmsc_command_result = complete;
    g_usbh0_hmsc_strg_drive_search_seq = USBH0_SEQ_0;

    /* Get mem pool blk */
    err = R_USBH0_PGET_BLK(USBH0_HSTRG_MPL, &p_blf);
    if (USBH0_OK == err)
    {
        p_blf->msginfo  = USBH0_MSG_HMSC_STRG_DRIVE_SEARCH;
        p_blf->keyword  = addr;
        p_blf->complete = complete;

        /* Send message */
        err = R_USBH0_SND_MSG(USBH0_HSTRG_MBX, (usbh0_msg_t*)p_blf);
        if(USBH0_OK != err)
        {
            err = R_USBH0_REL_BLK(USBH0_HSTRG_MPL,p_blf);
            USBH0_PRINTF0("### StrgDriveSearch function snd_msg error\n");
        }
    }
    else
    {
        USBH0_PRINTF0("### StrgDriveSearch function pget_blk error\n");
    }

    return err;
}   /* eof R_USBH0_HmscStrgDriveSearch() */

/******************************************************************************
Function Name   : R_USBH0_HmscStrgDriveOpen
Description     : Releases drive
Arguments       : uint16_t *side   : Side pointer
                : uint16_t addr    : Device Address
Return value    : uint16_t         : [OK/ERROR]
******************************************************************************/
uint16_t R_USBH0_HmscStrgDriveOpen(uint16_t *side, uint16_t addr)
{
    uint16_t    err;

    err = R_USBH0_HmscAllocDrvNo(side, addr);
    if (USBH0_ERROR == err)
    {
        return USBH0_ERROR;
    }

    /* Set Pipe */
    g_usbh0_hmsc_in_pipe[*side]   = R_USBH0_HstdGetPipeID(addr, USBH0_EP_BULK, USBH0_EP_IN, 0xFF);
    g_usbh0_hmsc_out_pipe[*side]  = R_USBH0_HstdGetPipeID(addr, USBH0_EP_BULK, USBH0_EP_OUT, 0xFF);

    /* Set SubClass */
    g_usbh0_hmsc_sub_class[*side] = g_usbh0_hmsc_tmp_sub_class;

    return USBH0_OK;
}   /* eof R_USBH0_HmscStrgDriveClose() */

/******************************************************************************
Function Name   : R_USBH0_HmscStrgDriveClose
Description     : Releases drive
Arguments       : uint16_t side    : Side
Return value    : uint16_t         : [OK/ERROR]
******************************************************************************/
uint16_t R_USBH0_HmscStrgDriveClose(uint16_t side)
{
    uint16_t    result;

    /* Clear Pipe */
    g_usbh0_hmsc_out_pipe[side]   = 0;
    g_usbh0_hmsc_in_pipe[side]    = 0;
    R_USBH0_HstdClearPipe(g_usbh0_hmsc_drvno_tbl[side].devadr);

    result = R_USBH0_HmscFreeDrvNo(side);

    return result;
}   /* eof R_USBH0_HmscStrgDriveClose() */

/******************************************************************************
Function Name   : R_USBH0_HmscStrgReadSector
Description     : Releases drive
Arguments       : uint16_t side        : Side
                : uint8_t  *buff       : Buffer address
                : uint32_t secno       : Sector number
                : uint16_t seccnt      : Sector count
                : uint32_t trans_byte  : Trans byte
Return value    : uint16_t             : [DONE/ERROR]
******************************************************************************/
uint16_t R_USBH0_HmscStrgReadSector(uint16_t side, uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte)
{
    uint16_t    result;

    /* Device Status */
    result = R_USBH0_HmscGetDevSts(side);
    if (USBH0_TRUE != result)
    {
        USBH0_PRINTF1("### device det(R_USBH0_HmscStrgReadSector:side=%d)\n", side);
        return USBH0_ERROR;
    }

    g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_RW_END;
    gs_usbh0_hmsc_command_end_flag = USBH0_FALSE;
    result = R_USBH0_HmscRead10(side, p_buff, secno, seccnt, trans_byte);

    return result;
}   /* eof R_USBH0_HmscStrgReadSector() */

/******************************************************************************
Function Name   : R_USBH0_HmscStrgWriteSector
Description     : Writes sector information
Arguments       : uint16_t side        : Side
                : const uint8_t  *buff : Buffer address
                : uint32_t secno       : Sector number
                : uint16_t seccnt      : Sector count
                : uint32_t trans_byte  : Trans byte
Return value    : uint16_t             : [DONE/ERROR]
******************************************************************************/
uint16_t R_USBH0_HmscStrgWriteSector(uint16_t side, const uint8_t *p_buff, uint32_t secno,
                                    uint16_t seccnt, uint32_t trans_byte)
{
    uint16_t    result;

    /* Device Status */
    result = R_USBH0_HmscGetDevSts(side);
    if (USBH0_TRUE != result)
    {
        USBH0_PRINTF1("### device det(R_USBH0_HmscStrgWriteSector:side=%d)\n", side);
        return USBH0_ERROR;
    }

    g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_RW_END;
    gs_usbh0_hmsc_command_end_flag = USBH0_FALSE;
    result = R_USBH0_HmscWrite10(side, p_buff, secno, seccnt, trans_byte);

    return result;
}   /* eof R_USBH0_HmscStrgWriteSector() */

/******************************************************************************
Function Name   : R_USBH0_HmscStrgCheckEnd
Description     : Check Read/Write end
Arguments       : void
Return value    : uint16_t             : [USBH0_TRUE/USBH0_FALSE/USBH0_ERROR]
******************************************************************************/
uint16_t R_USBH0_HmscStrgCheckEnd(void)
{
    uint16_t    end_flag;

    end_flag = gs_usbh0_hmsc_command_end_flag;
    gs_usbh0_hmsc_command_end_flag = USBH0_FALSE;

    return end_flag;
}   /* eof R_USBH0_HmscStrgCheckEnd() */

/******************************************************************************
Function Name   : R_USBH0_HmscStrgUserCommand
Description     : USB Mass Storage Command
Arguments       : uint16_t side        : Side
                : uint16_t command     : Command
                : uint8_t  *buff       : Buffer address
                : usbh0_utr_cb_t complete    : callback info
Return value    : uint16_t             : [DONE/ERROR]
******************************************************************************/
uint16_t R_USBH0_HmscStrgUserCommand(uint16_t side, uint16_t command, uint8_t *p_buff, usbh0_utr_cb_t complete)
{
    uint16_t    result;

    /* Device Status */
    result = R_USBH0_HmscGetDevSts(side);
    if (USBH0_TRUE != result)
    {
        USBH0_PRINTF1("### device det(StrgUserCommand:side=%d)\n", side);
        return USBH0_ERROR;
    }

    switch (command)
    {
        case USBH0_ATAPI_TEST_UNIT_READY:
            g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_USER_COMMAND;
            gs_usbh0_hmsc_command_result = complete;

            /* Test unit */
            result = R_USBH0_HmscTestUnit(side);
        break;

        case USBH0_ATAPI_REQUEST_SENSE:
            g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_USER_COMMAND;
            gs_usbh0_hmsc_command_result = complete;

            /*Request sense */
            result = R_USBH0_HmscRequestSense(side, p_buff);
        break;

        case USBH0_ATAPI_INQUIRY:
            g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_USER_COMMAND;
            gs_usbh0_hmsc_command_result = complete;

            /* Inquiry */
            result = R_USBH0_HmscInquiry(side, p_buff);
        break;

        case USBH0_ATAPI_MODE_SELECT6:
            g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_USER_COMMAND;
            gs_usbh0_hmsc_command_result = complete;

            /* Mode select6 */
            result = R_USBH0_HmscModeSelect6(side, p_buff);
        break;

        case USBH0_ATAPI_PREVENT_ALLOW:
            g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_USER_COMMAND;
            gs_usbh0_hmsc_command_result = complete;

            /* Prevent allow */
            result = R_USBH0_HmscPreventAllow(side, p_buff);
        break;

        case USBH0_ATAPI_READ_FORMAT_CAPACITY:
            g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_USER_COMMAND;
            gs_usbh0_hmsc_command_result = complete;

            /* Read format capacity */
            result = R_USBH0_HmscReadFormatCapacity(side, p_buff);
        break;

        case USBH0_ATAPI_READ_CAPACITY:
            g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_USER_COMMAND;
            gs_usbh0_hmsc_command_result = complete;

            /* Read capacity */
            result = R_USBH0_HmscReadCapacity(side, p_buff);
        break;

        case USBH0_ATAPI_MODE_SENSE10:
            g_usbh0_hmsc_strg_process = USBH0_MSG_HMSC_STRG_USER_COMMAND;
            gs_usbh0_hmsc_command_result = complete;

            /* Mode sense10 */
            result = R_USBH0_HmscModeSense10(side, p_buff);
        break;

        case USBH0_ATAPI_FORMAT_UNIT:
        case USBH0_ATAPI_MODE_SENSE6:
        case USBH0_ATAPI_START_STOP_UNIT:
        case USBH0_ATAPI_READ10:
        case USBH0_ATAPI_WRITE10:
        case USBH0_ATAPI_SEEK:
        case USBH0_ATAPI_WRITE_AND_VERIFY:
        case USBH0_ATAPI_VERIFY10:
        case USBH0_ATAPI_MODE_SELECT10:
            result = USBH0_ERROR;
        break;

        default:
            return USBH0_ERROR;
        break;
    }

    if (USBH0_HMSC_OK == result)
    {
        return USBH0_OK;
    }
    else
    {
        return USBH0_ERROR;
    }
}   /* eof R_USBH0_HmscStrgUserCommand() */


/******************************************************************************
End  Of File
******************************************************************************/
