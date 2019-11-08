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
/*******************************************************************************
* File Name    : r_fat.h
* Version      : 1.20
* Description  : FAT/exFAT File System Sample Program
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 16.03.2018 1.00     First Release
*         : 28.12.2018 1.02     Support for OS
*         : 29.05.2019 1.20     Correspond to internal coding rules
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "ff.h"
#include "ffconf.h"
#include "console.h"

#ifndef __R_FAT_H
#define __R_FAT_H

/******************************************************************************
Macro definitions
******************************************************************************/

#if (FF_FS_EXFAT == 1)
#define MAX_FILE_SIZE_STRING_LENGTH (20)
#else
#define MAX_FILE_SIZE_STRING_LENGTH (10)
#endif

/* sample_app_rw_buff */
#define RW_BUFF_SIZE    (512)

/* drive_str */
#define DRIVE_LEN       (3)                 /* (Single digit drive number) + ":" */

/* fs_type_str */
#define FS_TYPE_NUM     (FS_EXFAT + 1)      /* Number of filesystem type    */
#define FS_TYPE_LEN     (6)                 /* Filesystem type length       */

#define FS_SEM_WAITTIME (500uL)

/******************************************************************************
Variable Externs
******************************************************************************/
extern uint32_t g_fat_sem_access[FF_VOLUMES];
extern uint8_t  g_app_rw_buff[FF_VOLUMES][RW_BUFF_SIZE];
extern FATFS    g_fatfs[FF_VOLUMES];
extern FIL      g_fil[FF_VOLUMES];

/******************************************************************************
Functions Prototypes
******************************************************************************/

/* Function Name: FAT_Att */
/**************************************************************************//**
 * @fn            int16_t FAT_Att(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Connects a drive.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Att(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: FAT_Det */
/**************************************************************************//**
 * @fn            int16_t FAT_Det(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Disconnects a drive.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Det(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: FAT_Dir */
/**************************************************************************//**
 * @fn            int16_t FAT_Dir(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Displays a list of files and subdirectories in a directory.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Dir(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: FAT_Type */
/**************************************************************************//**
 * @fn            int16_t FAT_Type(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Displays the contents of a file.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Type(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: FAT_Write */
/**************************************************************************//**
 * @fn            int16_t FAT_Write(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Write a sample string.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Write(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: FAT_Create */
/**************************************************************************//**
 * @fn            int16_t FAT_Create(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Create new file.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Create(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: FAT_Del */
/**************************************************************************//**
 * @fn            int16_t FAT_Del(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Delete file.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Del(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: FAT_Mkdir */
/**************************************************************************//**
 * @fn            int16_t FAT_Mkdir(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Create new directory.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Mkdir(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: FAT_Rmdir */
/**************************************************************************//**
 * @fn            int16_t FAT_Rmdir(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Delete directory.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Rmdir(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: FAT_Exit */
/**************************************************************************//**
 * @fn            int16_t FAT_Exit(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
 * @brief         Exit fat sample mode.
 * @warning       .
 * @param [in]    iArgCount - The number of arguments in the argument list.
 * @param [in]    ppszArgument - The argument list.
 * @param [in]    pCom - Pointer to the command object.
 * @retval        CMD_OK for success.
 *****************************************************************************/
int16_t FAT_Exit(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Function Name: char_to_tchar */
/**************************************************************************//**
 * @fn            void    char_to_tchar(char_t *strIn, TCHAR *strOut)
 * @brief         Convert char string to TCHAR string.
 * @warning       .
 * @param [in]    char_t *strIn  : input string
 * @param [in]    TCHAR  *strOut : output string
 * @retval        none.
 *****************************************************************************/
void    char_to_tchar(char_t *strIn, TCHAR *strOut);

/* Function Name: tchar_to_char */
/**************************************************************************//**
 * @fn            void    tchar_to_char(TCHAR *strIn, char_t *strOut)
 * @brief         Convert TCHAR string to char string.
 * @warning       .
 * @param [in]    TCHAR *strIn   : input string
 * @param [in]    char_t *strOut : output string
 * @retval        none.
 *****************************************************************************/
void    tchar_to_char(TCHAR *strIn, char_t *strOut);

/* Function Name: get_side */
/**************************************************************************//**
 * @fn            int32_t get_side(uint8_t * name)
 * @brief         Get the drive number from the path name.
 * @warning       .
 * @param [in]    uint8_t *name     : path name.
 * @retval        drive number      : Success.
 * @retval        -1                : Error.
 *****************************************************************************/
int32_t get_side(uint8_t * name);

/* Function Name: make_file_size_str */
/**************************************************************************//**
 * @fn            void    make_file_size_str(uint8_t * new_str, uint8_t * str)
 * @brief         Create file size string.
 *                Example : 1234567 -> 1,234,567
 * @warning       .
 * @param [out]   uint8_t *new_str : output.
 * @param [in]    uint8_t *str     : input.
 * @retval        none.
 *****************************************************************************/
void    make_file_size_str(uint8_t * new_str, uint8_t * str);

/* Function Name: make_file_create_date_str */
/**************************************************************************//**
 * @fn            void    make_file_create_date_str(uint8_t * new_str, uint16_t date)
 * @brief         Create file create date string.
 * @warning       .
 * @param [out]   uint8_t *new_str : output.
 * @param [in]    uint16_t date    : input date.
 * @retval        none.
 *****************************************************************************/
void    make_file_create_date_str(uint8_t * new_str, uint16_t date);

/* Function Name: make_file_create_time_str */
/**************************************************************************//**
 * @fn            void    make_file_create_time_str(uint8_t * new_str, uint16_t time)
 * @brief         Create file create time string.
 * @warning       .
 * @param [out]   uint8_t *new_str : output.
 * @param [in]    uint16_t time    : input time.
 * @retval        none.
 *****************************************************************************/
void    make_file_create_time_str(uint8_t * new_str, uint16_t time);

#endif /* __R_FAT_H */

/* End of File */

