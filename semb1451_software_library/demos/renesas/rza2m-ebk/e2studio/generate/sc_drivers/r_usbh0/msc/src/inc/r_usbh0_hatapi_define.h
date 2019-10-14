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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usbh0_hatapi_define.h
* Description  : USB common extern header
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
***********************************************************************************************************************/

#ifndef R_USBH0_HATAPI_DEFINE_H
#define R_USBH0_HATAPI_DEFINE_H

/******************************************************************************
Constant macro definitions
******************************************************************************/
/* prevent allow key */
#define USBH0_MEDIA_UNLOCK              (0u)        /* Media unlock */
#define USBH0_MEDIA_LOCK                (1u)        /* Media Lock */

/* Peripheral Device Type (InquiryRequest) */
#define USBH0_PDT_DIRECT                (0x00u)
#define USBH0_PDT_SEQUENTIAL            (0x01u)
#define USBH0_PDT_WRITEONCE             (0x04u)
#define USBH0_PDT_CDROM                 (0x05u)
#define USBH0_PDT_OPTICAL               (0x07u)
#define USBH0_PDT_UNKNOWN               (0x1Fu)

/* Partision check */
#define USBH0_PBR_ADDR                  (0x01u)
#define USBH0_MBR_ADDR                  (0x02u)
#define USBH0_EMBR_ADDR                 (0x03u)
#define USBH0_BOOT_ERROR                (0x05u)
#define USBH0_BOOTRECORD_SIG            (0xAA55u)
#define USBH0_STARTDISK                 (0x80u)
#define USBH0_NOTSTARTDISK              (0x00u)
#define USBH0_NOPCODE                   (0x90u)
#define USBH0_JMPCODE1                  (0xEBu)
#define USBH0_JMPCODE2                  (0xE9u)

/* Partition type */
#define USBH0_PT_NONE                   (0x00u)
#define USBH0_PT_FAT12A                 (0x01u)
#define USBH0_PT_FAT16A                 (0x04u)
#define USBH0_PT_EPRTA                  (0x05u)
#define USBH0_PT_FAT16B                 (0x06u)
#define USBH0_PT_FAT32A                 (0x0Bu)
#define USBH0_PT_FAT32X                 (0x0Cu)
#define USBH0_PT_FAT16X                 (0x0Eu)
#define USBH0_PT_EPRTB                  (0x0Fu)
#define USBH0_PT_EPRTC                  (0x85u)

/*****************************************************************************
Typedef definitions
******************************************************************************/
/* MBR */
typedef struct
{
    uint8_t     jmp_code;
    uint8_t     jmp_addr;
    uint8_t     nop_code;
    uint8_t     bsr_code[443];
    uint8_t     partition_table[64];
    uint8_t     signature[2];
} st_usbh0_mbr_t;

/* PTBL */
typedef struct
{
    uint8_t     active_flag;
    uint8_t     start_head;
    uint8_t     start_cs[2];
    uint8_t     partition_type;
    uint8_t     stop_head;
    uint8_t     stop_cs[2];
    uint8_t     start_sector_num[4];
    uint8_t     partition_sect[4];
} st_usbh0_ptbl_t;

/* PBR */
typedef struct
{
    uint8_t     jmp_code;
    uint8_t     jmp_addr;
    uint8_t     nop_code;
    uint8_t     name[8];
    uint8_t     sector_size[2];
    uint8_t     cluster_size;
    uint8_t     reserved_sector[2];
    uint8_t     fatcount;
    uint8_t     root_dirtop[2];
    uint8_t     total_sector0[2];
    uint8_t     dfsmedia_type;
    uint8_t     fat_sector[2];
    uint8_t     track_sector[2];
    uint8_t     cylinder_sector[2];
    uint8_t     offset_sector[4];
    uint8_t     total_sector1[4];
    uint8_t     fat_sig_data[474];
    uint8_t     signature[2];
} st_usbh0_pbr_t;

/* FAT12 */
typedef struct
{
    uint8_t     drive_num;
    uint8_t     reserve;
    uint8_t     boot_sig;
    uint8_t     vol_sirial[4];
    uint8_t     vol_label[11];
    uint8_t     file_system_type[8];
} st_usbh0_fat1216_t;

/* FAT32 */
typedef struct
{
    uint8_t     fat_sector[4];
    uint8_t     extended_flag[2];
    uint8_t     file_systemver[2];
    uint8_t     root_dir_cluster[4];
    uint8_t     fsinfo_sector[2];
    uint8_t     backup_boot_sector[2];
    uint8_t     reserve12[12];
    uint8_t     drive_num;
    uint8_t     reserve;
    uint8_t     boot_sig;
    uint8_t     vol_sirial[4];
    uint8_t     vol_label[11];
    uint8_t     file_system_type[8];
} st_usbh0_fat32_t;


/* Callback Message format define. */
typedef struct
{
    uint32_t    ar_rst;
    uint32_t    ul_size;
} st_usbh0_pmsc_cbm_t;

/* Command Descriptor Block format define. */
typedef union
{
    struct
    {
        uint8_t uc_OpCode;
        struct
        {
            uint8_t b_reserved:5;
            uint8_t b_LUN:3;
        }
        s_LUN;
        uint8_t uc_data;
    }
    s_usbh0_ptn0;
    struct
    {
        uint8_t uc_OpCode;
        struct
        {
            uint8_t b_immed:1;
            uint8_t b_reserved4:4;
            uint8_t b_LUN:3;
        }
        s_LUN;
        uint8_t uc_rsv2[2];
        uint8_t uc_Allocation;
        uint8_t uc_rsv1[1];
        uint8_t uc_rsv6[6];
    }
    s_usbh0_ptn12;
    struct
    {
        uint8_t uc_OpCode;
        struct
        {
            uint8_t b_Defect:3;
            uint8_t b_CmpList:1;
            uint8_t b_FmtData:1;
            uint8_t b_LUN:3;
        }
        s_LUN;
        uint8_t ul_LBA0;
        uint8_t ul_LBA1;
        uint8_t ul_LBA2;
        uint8_t ul_LBA3;
        uint8_t uc_rsv6[6];
    }
    s_usbh0_ptn378;
    struct
    {
        uint8_t uc_OpCode;
        struct
        {
            uint8_t b_SP:1;
            uint8_t b_ByteChk:1;
            uint8_t b_reserved2:2;
            uint8_t b_1:1;
            uint8_t b_LUN:3;
        }
        s_LUN;
        /* Logical block */
        uint8_t ul_LogicalBlock0;
        uint8_t ul_LogicalBlock1;
        uint8_t ul_LogicalBlock2;
        uint8_t ul_LogicalBlock3;
        uint8_t uc_rsv1[1];
        uint8_t us_Length_Hi;
        uint8_t us_Length_Lo;
        uint8_t uc_rsv3[3];
    }
    s_usbh0_ptn4569;
} st_usbh0_hmsc_cdb_t;

/*****************************************************************************
Enum definitions
******************************************************************************/
enum usbh0_gpmsc_atapiresult
{
    USBH0_ATAPI_SUCCESS                 = 0x11,

    /* Command receive events */
    USBH0_ATAPI_NO_DATA                 = 0x21,
    USBH0_ATAPI_A_SND_DATA              = 0x22,
    USBH0_ATAPI_A_RCV_DATA              = 0x23,
    USBH0_ATAPI_SND_DATAS               = 0x24,
    USBH0_ATAPI_RCV_DATAS               = 0x25,
    USBH0_ATAPI_NOT_SUPPORT             = 0x26,

    /* Complete events */
    USBH0_ATAPI_CMD_CONTINUE            = 0x31,
    USBH0_ATAPI_CMD_COMPLETE            = 0x32,
    USBH0_ATAPI_CMD_FAILED              = 0x33,

    /* ATAPI Start events */
    USBH0_ATAPI_READY                   = 0x41,

    /* respond error */
    USBH0_ATAPI_ERROR                   = 0x51,

    /* ERR CODE */
    USBH0_ATAPI_ERR_CODE_SEPARATER      = 0x100,
    USBH0_ATAPI_ERR_INVAL               = 0x61
} e_usbh0_gpmsc_atapiresult_t;

#endif  /* R_USBH0_HATAPI_DEFINE_H */
