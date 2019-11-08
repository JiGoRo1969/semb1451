/***********************************************************************************************************************
 * File Name     : r_rspi.h
 * Version       : 1.0.0
 * Creation Date : 2019-11-01
 * Device(s)	 : RZ/A2M
 * Tool-Chain	 : e2Studio Ver 7.4.0
 *				 : GCC ARM Embedded 6.3.1.20170620
 * OS			 : FreeRTOS
 * H/W Platform  : SEMB1451/1452
 * Description  : General header file for RSPI peripheral.
 * Operation	 :
 * Limitations	 :
 ***********************************************************************************************************************/

#ifndef __R_RSPI_H__
#define __R_RSPI_H__

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "r_typedefs.h"
#include "iodefine.h"
#include "r_cpg_drv_api.h"
#include "r_ostm_drv_api.h"
#include "r_scifa_drv_api.h"
#include "r_gpio_drv_api.h"
#include "r_startup_config.h"
#include "compiler_settings.h"
#include "main.h"
#include "r_os_abstraction_api.h"
#include "r_task_priority.h"
#include "iodefine.h"

/******************************************************************************
 * Definition for Function Selection
 *****************************************************************************/

//#define 	USE_RSPI_SSL0
//#define 	USE_RSPI_SSL1

#if 1
#define 	D_RSPI_SSL_ACTIVE		(1U)
#define 	D_RSPI_SSL_INACTIVE		(0U)
#else
#define 	D_RSPI_SSL_ACTIVE		(0U)
#define 	D_RSPI_SSL_INACTIVE		(1U)
#endif

#if 0
#define 	USE_RSPI_DATALEN_32bit
#elif 0
#define 	USE_RSPI_DATALEN_16bit
#else
#define 	USE_RSPI_DATALEN_8bit
#endif


/******************************************************************************
 * Macro definitions (Register bit)
 *****************************************************************************/
/*
    RSPI Control Register (SPCR)
*/
/* RSPI Mode Select (SPMS) */
#define _00_RSPI_MODE_SPI                       (0x00U) /* SPI operation (four-wire method) */
#define _01_RSPI_MODE_CLOCK_SYNCHRONOUS         (0x01U) /* Clock synchronous operation (three-wire method) */
/* Communications Operating Mode Select (TXMD) */
#define _00_RSPI_FULL_DUPLEX_SYNCHRONOUS        (0x00U) /* Full-duplex synchronous serial communications */
#define _02_RSPI_TRANSMIT_ONLY                  (0x02U) /* Serial communications with transmit only operations */
/* Mode Fault Error Detection Enable (MODFEN) */
#define _00_RSPI_MODE_FAULT_DETECT_DISABLED     (0x00U) /* Disables the detection of mode fault error */
#define _04_RSPI_MODE_FAULT_DETECT_ENABLED      (0x04U) /* Enables the detection of mode fault error */
/* RSPI Master/Slave Mode Select (MSTR) */
#define _00_RSPI_SLAVE_MODE                     (0x00U) /* Slave mode */
#define _08_RSPI_MASTER_MODE                    (0x08U) /* Master mode */
/* RSPI Error Interrupt Enable (SPEIE) */
#define _00_RSPI_ERROR_INTERRUPT_DISABLED       (0x00U) /* Disables the generation of RSPI error interrupt */
#define _10_RSPI_ERROR_INTERRUPT_ENABLED        (0x10U) /* Enables the generation of RSPI error interrupt */
/* RSPI Transmit Interrupt Enable (SPTIE) */
#define _00_RSPI_TRANSMIT_INTERRUPT_DISABLED    (0x00U) /* Disables the generation of RSPI transmit interrupt */
#define _20_RSPI_TRANSMIT_INTERRUPT_ENABLED     (0x20U) /* Enables the generation of RSPI transmit interrupt */
/* RSPI Function Enable (SPE) */
#define _00_RSPI_FUNCTION_DISABLED              (0x00U) /* Disables the RSPI function */
#define _40_RSPI_FUNCTION_ENABLED               (0x40U) /* Enables the RSPI function */
/* RSPI Receive Interrupt Enable (SPRIE) */
#define _00_RSPI_RECEIVE_INTERRUPT_DISABLED     (0x00U) /* Disables the generation of RSPI receive interrupt */
#define _80_RSPI_RECEIVE_INTERRUPT_ENABLED      (0x80U) /* Enables the generation of RSPI receive interrupt */

/*
    RSPI Slave Select Polarity Register (SSLP)
*/
/* SSL0 Signal Polarity Setting (SSL0P) */
#define _00_RSPI_SSL0_POLARITY_LOW              (0x00U) /* SSL0 signal is active low */
#define _01_RSPI_SSL0_POLARITY_HIGH             (0x01U) /* SSL0 signal is active high */
/* SSL1 Signal Polarity Setting (SSL1P) */
#define _00_RSPI_SSL1_POLARITY_LOW              (0x00U) /* SSL1 signal is active low */
#define _02_RSPI_SSL1_POLARITY_HIGH             (0x02U) /* SSL1 signal is active high */
/* SSL2 Signal Polarity Setting (SSL2P) */
#define _00_RSPI_SSL2_POLARITY_LOW              (0x00U) /* SSL2 signal is active low */
#define _04_RSPI_SSL2_POLARITY_HIGH             (0x04U) /* SSL2 signal is active high */
/* SSL3 Signal Polarity Setting (SSL3P) */
#define _00_RSPI_SSL3_POLARITY_LOW              (0x00U) /* SSL3 signal is active low */
#define _08_RSPI_SSL3_POLARITY_HIGH             (0x08U) /* SSL3 signal is active high */

/*
    RSPI Pin Control Register (SPPCR)
*/
/* RSPI Loopback (SPLP) */
#define _00_RSPI_LOOPBACK_DISABLED              (0x00U) /* Normal mode */
#define _01_RSPI_LOOPBACK_ENABLED               (0x01U) /* Loopback mode (reversed transmit data = receive data) */
/* RSPI Loopback 2 (SPLP2) */
#define _00_RSPI_LOOPBACK2_DISABLED             (0x00U) /* Normal mode */
#define _02_RSPI_LOOPBACK2_ENABLED              (0x02U) /* Loopback mode (transmit data = receive data) */
/* MOSI Idle Fixed Value (MOIFV) */
#define _00_RSPI_MOSI_LEVEL_LOW                 (0x00U) /* Level output on MOSIA during idling corresponds to low */
#define _10_RSPI_MOSI_LEVEL_HIGH                (0x10U) /* Level output on MOSIA during idling corresponds to high */
/* MOSI Idle Value Fixing Enable (MOIFE) */
#define _00_RSPI_MOSI_FIXING_PREV_TRANSFER      (0x00U) /* MOSI output value equals final data from previous transfer */
#define _20_RSPI_MOSI_FIXING_MOIFV_BIT          (0x20U) /* MOSI output value equals the value set in the MOIFV bit */

/*
    RSPI Sequence Control Register (SPSCR)
*/
/* RSPI Sequence Length Specification (SPSLN[2:0]) */
#define _00_RSPI_SEQUENCE_LENGTH_1              (0x00U) /* 0 -> 0... */
#define _01_RSPI_SEQUENCE_LENGTH_2              (0x01U) /* 0 -> 1 -> 0... */
#define _02_RSPI_SEQUENCE_LENGTH_3              (0x02U) /* 0 -> 1 -> 2 -> 0... */
#define _03_RSPI_SEQUENCE_LENGTH_4              (0x03U) /* 0 -> 1 -> 2 -> 3 -> 0... */
#define _04_RSPI_SEQUENCE_LENGTH_5              (0x04U) /* 0 -> 1 -> 2 -> 3 -> 4 -> 0... */
#define _05_RSPI_SEQUENCE_LENGTH_6              (0x05U) /* 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 0... */
#define _06_RSPI_SEQUENCE_LENGTH_7              (0x06U) /* 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 0... */
#define _07_RSPI_SEQUENCE_LENGTH_8              (0x07U) /* 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 0... */

/*
    RSPI Data Control Register (SPDCR)
*/
/* RSPI Longword Access/Word Access Specification (SPLW) */ 
#define _20_RSPI_ACCESS_BYTE                    (0x20U) /* SPDR is accessed in  8bit */
#define _40_RSPI_ACCESS_WORD                    (0x40U) /* SPDR is accessed in 16bit */
#define _60_RSPI_ACCESS_LONG                    (0x60U) /* SPDR is accessed in 32bit */

/*
    RSPI Clock Delay Register (SPCKD)
*/
/* RSPCK Delay Setting (SCKDL[2:0]) */
#define _00_RSPI_RSPCK_DELAY_1                  (0x00U) /* 1 RSPCK */
#define _01_RSPI_RSPCK_DELAY_2                  (0x01U) /* 2 RSPCK */
#define _02_RSPI_RSPCK_DELAY_3                  (0x02U) /* 3 RSPCK */
#define _03_RSPI_RSPCK_DELAY_4                  (0x03U) /* 4 RSPCK */
#define _04_RSPI_RSPCK_DELAY_5                  (0x04U) /* 5 RSPCK */
#define _05_RSPI_RSPCK_DELAY_6                  (0x05U) /* 6 RSPCK */
#define _06_RSPI_RSPCK_DELAY_7                  (0x06U) /* 7 RSPCK */
#define _07_RSPI_RSPCK_DELAY_8                  (0x07U) /* 8 RSPCK */

/*
    RSPI Slave Select Negation Delay Register (SSLND)
*/
/* SSL Negation Delay Setting (SLNDL[2:0]) */
#define _00_RSPI_SSL_NEGATION_DELAY_1           (0x00U) /* 1 RSPCK */
#define _01_RSPI_SSL_NEGATION_DELAY_2           (0x01U) /* 2 RSPCK */
#define _02_RSPI_SSL_NEGATION_DELAY_3           (0x02U) /* 3 RSPCK */
#define _03_RSPI_SSL_NEGATION_DELAY_4           (0x03U) /* 4 RSPCK */
#define _04_RSPI_SSL_NEGATION_DELAY_5           (0x04U) /* 5 RSPCK */
#define _05_RSPI_SSL_NEGATION_DELAY_6           (0x05U) /* 6 RSPCK */
#define _06_RSPI_SSL_NEGATION_DELAY_7           (0x06U) /* 7 RSPCK */
#define _07_RSPI_SSL_NEGATION_DELAY_8           (0x07U) /* 8 RSPCK */

/*
    RSPI Next-Access Delay Register (SPND)
*/
/* RSPI Next-Access Delay Setting (SPNDL[2:0]) */
#define _00_RSPI_NEXT_ACCESS_DELAY_1            (0x00U) /* 1 RSPCK + 2 PCLK */
#define _01_RSPI_NEXT_ACCESS_DELAY_2            (0x01U) /* 2 RSPCK + 2 PCLK */
#define _02_RSPI_NEXT_ACCESS_DELAY_3            (0x02U) /* 3 RSPCK + 2 PCLK */
#define _03_RSPI_NEXT_ACCESS_DELAY_4            (0x03U) /* 4 RSPCK + 2 PCLK */
#define _04_RSPI_NEXT_ACCESS_DELAY_5            (0x04U) /* 5 RSPCK + 2 PCLK */
#define _05_RSPI_NEXT_ACCESS_DELAY_6            (0x05U) /* 6 RSPCK + 2 PCLK */
#define _06_RSPI_NEXT_ACCESS_DELAY_7            (0x06U) /* 7 RSPCK + 2 PCLK */
#define _07_RSPI_NEXT_ACCESS_DELAY_8            (0x07U) /* 8 RSPCK + 2 PCLK */

/*
    RSPI Command Registers 0 to 7 (SPCMD0 to SPCMD7)
*/
/* RSPCK Phase Setting (CPHA) */
#define _0000_RSPI_RSPCK_SAMPLING_ODD           (0x0000U) /* Data sampling on odd edge, data variation on even edge */
#define _0001_RSPI_RSPCK_SAMPLING_EVEN          (0x0001U) /* Data variation on odd edge, data sampling on even edge */
/* RSPCK Polarity Setting (CPOL) */
#define _0000_RSPI_RSPCK_POLARITY_LOW           (0x0000U) /* RSPCK is low when idle */
#define _0002_RSPI_RSPCK_POLARITY_HIGH          (0x0002U) /* RSPCK is high when idle */
/* Bit Rate Division Setting (BRDV[1:0]) */
#define _0000_RSPI_BASE_BITRATE_1               (0x0000U) /* These bits select the base bit rate */
#define _0004_RSPI_BASE_BITRATE_2               (0x0004U) /* These bits select the base bit rate divided by 2 */
#define _0008_RSPI_BASE_BITRATE_4               (0x0008U) /* These bits select the base bit rate divided by 4 */
#define _000C_RSPI_BASE_BITRATE_8               (0x000CU) /* These bits select the base bit rate divided by 8 */
/* SSL Signal Assertion Setting (SSLA[2:0]) */
#define _0000_RSPI_SIGNAL_ASSERT_SSL0           (0x0000U) /* SSL0 */
#define _0010_RSPI_SIGNAL_ASSERT_SSL1           (0x0010U) /* SSL1 */
#define _0020_RSPI_SIGNAL_ASSERT_SSL2           (0x0020U) /* SSL2 */
#define _0030_RSPI_SIGNAL_ASSERT_SSL3           (0x0030U) /* SSL3 */
/* SSL Signal Level Keeping (SSLKP) */
#define _0000_RSPI_SSL_KEEP_DISABLE             (0x0000U) /* Negates all SSL signals upon completion of transfer */
#define _0080_RSPI_SSL_KEEP_ENABLE              (0x0080U) /* Keep SSL level from end of transfer till next access */
/* RSPI Data Length Setting (SPB[3:0]) */
#define _0400_RSPI_DATA_LENGTH_BITS_8           (0x0400U) /* 8 bits */
#define _0700_RSPI_DATA_LENGTH_BITS_8           (0x0700U) /* 8 bits */
#define _0F00_RSPI_DATA_LENGTH_BITS_16          (0x0F00U) /* 16 bits */
#define _0200_RSPI_DATA_LENGTH_BITS_32          (0x0200U) /* 32 bits */
#define _0300_RSPI_DATA_LENGTH_BITS_32          (0x0300U) /* 32 bits */
/* RSPI LSB First (LSBF) */
#define _0000_RSPI_MSB_FIRST                    (0x0000U) /* MSB first */
#define _1000_RSPI_LSB_FIRST                    (0x1000U) /* LSB first */
/* RSPI Next-Access Delay Enable (SPNDEN) */
#define _0000_RSPI_NEXT_ACCESS_DELAY_DISABLE    (0x0000U) /* Next-access delay of 1 RSPCK + 2 PCLK */
#define _2000_RSPI_NEXT_ACCESS_DELAY_ENABLE     (0x2000U) /* Next-access delay equal to setting of SPND register */
/* SSL Negation Delay Setting Enable (SLNDEN) */
#define _0000_RSPI_NEGATION_DELAY_DISABLE       (0x0000U) /* SSL negation delay of 1 RSPCK */
#define _4000_RSPI_NEGATION_DELAY_ENABLE        (0x4000U) /* SSL negation delay equal to setting of SSLND register */
/* RSPCK Delay Setting Enable (SCKDEN) */
#define _0000_RSPI_RSPCK_DELAY_DISABLE          (0x0000U) /* RSPCK delay of 1 RSPCK */
#define _8000_RSPI_RSPCK_DELAY_ENABLE           (0x8000U) /* RSPCK delay equal to setting of the SPCKD register */

/*
    RSPI Data Control Register 2 (SPDCR2)
*/
/* Byte Swap (BYSW) */
#define _00_RSPI_BYTESWAP_DISABLED              (0x00U) /* Disable Byte Swap function */
#define _01_RSPI_BYTESWAP_ENABLED               (0x01U) /* Enable Byte Swap function */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_RSPI_PRIORITY_LEVEL0                (0x00U) /* Level 0 (interrupt disabled) */
#define _01_RSPI_PRIORITY_LEVEL1                (0x01U) /* Level 1 */
#define _02_RSPI_PRIORITY_LEVEL2                (0x02U) /* Level 2 */
#define _03_RSPI_PRIORITY_LEVEL3                (0x03U) /* Level 3 */
#define _04_RSPI_PRIORITY_LEVEL4                (0x04U) /* Level 4 */
#define _05_RSPI_PRIORITY_LEVEL5                (0x05U) /* Level 5 */
#define _06_RSPI_PRIORITY_LEVEL6                (0x06U) /* Level 6 */
#define _07_RSPI_PRIORITY_LEVEL7                (0x07U) /* Level 7 */
#define _08_RSPI_PRIORITY_LEVEL8                (0x08U) /* Level 8 */
#define _09_RSPI_PRIORITY_LEVEL9                (0x09U) /* Level 9 */
#define _0A_RSPI_PRIORITY_LEVEL10               (0x0AU) /* Level 10 */
#define _0B_RSPI_PRIORITY_LEVEL11               (0x0BU) /* Level 11 */
#define _0C_RSPI_PRIORITY_LEVEL12               (0x0CU) /* Level 12 */
#define _0D_RSPI_PRIORITY_LEVEL13               (0x0DU) /* Level 13 */
#define _0E_RSPI_PRIORITY_LEVEL14               (0x0EU) /* Level 14 */
#define _0F_RSPI_PRIORITY_LEVEL15               (0x0FU) /* Level 15 (highest) */

/******************************************************************************
 * Macro definitions
 *****************************************************************************/

/******************************************************************************
 * Typedef definitions
 *****************************************************************************/

typedef	unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned long	uint32_t;

typedef	union {
	uint32_t	l;
	uint16_t	w[2];
	uint8_t		b[4];
} uint32_u;

typedef	union {
	uint16_t	w;
	uint8_t		b[2];
} uint16_u;

/******************************************************************************
 * Global functions (Prototype definition)
 *****************************************************************************/

void		r_rspi0_create(void);
void		r_rspi0_start(void);
void		r_rspi0_send_receive(void *, void *, uint16_t);
void		r_rspi0_zerosend_receive(void *, uint16_t);
uint32_t	r_rspi0_completed_translation(void);

void		r_rspi1_create(void);
void		r_rspi1_start(void);
void		r_rspi1_send_receive(void *, void *, uint16_t);
void		r_rspi1_zerosend_receive(void *, uint16_t);
uint32_t	r_rspi1_completed_translation(void);

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

#endif	// __R_RSPI_H__

/***********************************************************************************************************************
 * EOF
 ***********************************************************************************************************************/
