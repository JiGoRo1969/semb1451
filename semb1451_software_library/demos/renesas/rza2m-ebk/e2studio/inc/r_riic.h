/******************************************************************************
 * File Name	 : r_riic.h
 * Version       : 1.0.0
 * Creation Date : 2019-11-01
 * Device(s)	 : RZ/A2M
 * Tool-Chain	 : e2Studio Ver 7.4.0
 *				 : GCC ARM Embedded 6.3.1.20170620
 * OS			 : FreeRTOS
 * H/W Platform  : SEMB1451/1452
 * Description   : RZ/A2M Program to control RIIC functions
 * Operation	 :
 * Limitations	 :
 *****************************************************************************/

#ifndef __R_RIIC_H__
#define __R_RIIC_H__

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "r_typedefs.h"

/******************************************************************************
 * Definition for Function Selection
 *****************************************************************************/

/******************************************************************************
 * Macro definitions (Register bit)
 *****************************************************************************/

/******************************************************************************
 * Macro definitions
 *****************************************************************************/

//	The target EEPROM model is R1EX24512BTAS0I (Renesas)
#define 	D_EEPROM_RIIC_NAME			(DEVICE_INDENTIFIER "riic3")
#define 	D_EEPROM_DEV_ADDRESS		(0xA0)
#define 	D_EEPROM_MAX_SIZE			(2048)			/* 2Kbyte */
#define 	D_EEPROM_PAGE_SIZE			(128)

//	The target MEMS model is BMX055 (Bosch)
#define		D_BMX055_RIIC_NAME			(DEVICE_INDENTIFIER "riic0")
#define		D_BMX055_DEV_ADDRESS		(0x30)

#define 	ERROR_FAILURE				(-1)	// <Generic error code
#define		NO_ERROR					(0)		// <Success

/******************************************************************************
 * Typedef definitions
 *****************************************************************************/

/******************************************************************************
 * Global functions (Prototype definition)
 *****************************************************************************/

int_t	r_eeprom_init(void);
int_t	r_eeprom_shutdown(void);
int_t	r_eeprom_write(uint32_t addr, const void *data, uint32_t size);
int_t	r_eeprom_read(uint32_t addr, void *data, uint32_t size);

int_t	r_bmx055_init(void);
int_t	r_bmx055_shutdown(void);
int_t	r_bmx055_acc_read(uint32_t addr, void *data, uint32_t size);
int_t	r_bmx055_acc_write(uint32_t addr, const void *data, uint32_t size);
int_t	r_bmx055_gyr_read(uint32_t addr, void *data, uint32_t size);
int_t	r_bmx055_gyr_write(uint32_t addr, const void *data, uint32_t size);
int_t	r_bmx055_mag_read(uint32_t addr, void *data, uint32_t size);
int_t	r_bmx055_mag_write(uint32_t addr, const void *data, uint32_t size);

/******************************************************************************
 * Global values
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/

/******************************************************************************
 * Local values
 *****************************************************************************/

/******************************************************************************
 * Imported global functions
 *****************************************************************************/

/******************************************************************************
 * Imported global variables
 *****************************************************************************/

#endif	// __R_RIIC_H__

/*****************************************************************************
 * EOF
 *****************************************************************************/
