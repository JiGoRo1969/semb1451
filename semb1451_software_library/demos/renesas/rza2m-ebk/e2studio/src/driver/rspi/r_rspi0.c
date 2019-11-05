/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_rspi0.c
* Version      : 1.0.0
* Device(s)    : RZ/A2M
* Description  : This file implements device driver for r_rspi0.
* Creation Date: 2019-10-01
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_rspi.h"
#include "iodefine.h"
#include "r_stb_drv_api.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t *	gp_rspi0_tx_address;              /* RSPI0 transmit buffer address */
volatile uint16_t	g_rspi0_tx_count;                 /* RSPI0 transmit data number */
volatile uint8_t *	gp_rspi0_rx_address;              /* RSPI0 receive buffer address */
volatile uint16_t	g_rspi0_rx_count;                 /* RSPI0 receive data number */
volatile uint16_t	g_rspi0_rx_length;                /* RSPI0 receive data length */
volatile uint32_t	r_cnt_rx0 = 0;

void		r_rspi0_transmit_interrupt(void);
void		r_rspi0_receive_interrupt(void);
void		r_rspi0_callback_transmitend(void);
void		r_rspi0_callback_receiveend(void);
uint32_t	r_rspi0_completed_translation(void);

uint32_t	r_rspi0_completed_translation(void)
{
	return r_cnt_rx0;
}

/***********************************************************************************************************************
* Function Name: r_rspi0_callback_transmitend
* Description  : This function is a callback function when RSPI0 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void	r_rspi0_callback_transmitend(void)
{
}

/***********************************************************************************************************************
* Function Name: r_rspi0_callback_receiveend
* Description  : This function is a callback function when RSPI0 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void	r_rspi0_callback_receiveend(void)
{
	r_cnt_rx0++;
}

/***********************************************************************************************************************
* Function Name: r_rspi0_transmit_interrupt
* Description  : This function is SPTI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void	r_rspi0_transmit_interrupt(void)
{
	if (g_rspi0_tx_count > 0U)
	{
		/* Write data for transmission */
#if 1
#if defined(USE_RSPI_DATALEN_32bit)
		{
			uint32_u	t_dat;

			t_dat.b[3] = *gp_rspi0_tx_address++;
			t_dat.b[2] = *gp_rspi0_tx_address++;
			t_dat.b[1] = *gp_rspi0_tx_address++;
			t_dat.b[0] = *gp_rspi0_tx_address++;
			RSPI0.SPDR.LONG		= t_dat.l;
		}
		g_rspi0_tx_count	   -= 4;
#elif defined(USE_RSPI_DATALEN_16bit)
		{
			uint16_u	t_dat;

			t_dat.b[1] = *gp_rspi0_tx_address++;
			t_dat.b[0] = *gp_rspi0_tx_address++;
			RSPI0.SPDR.WORD.L	= t_dat.w;
		}
		g_rspi0_tx_count	   -= 2;
#else // USE_RSPI_DATALEN_8bit
		RSPI0.SPDR.BYTE.LL		= (*(uint8_t*)gp_rspi0_tx_address);
		gp_rspi0_tx_address++;
		g_rspi0_tx_count--;
#endif
#else
#if defined(USE_RSPI_DATALEN_32bit)
		RSPI0.SPDR.LONG			= 0U;
		g_rspi0_tx_count	   -= 4;
#elif	defined(USE_RSPI_DATALEN_16bit)
		RSPI0.SPDR.WORD.L		= 0U;
		g_rspi0_tx_count	   -= 2;
#else // USE_RSPI_DATALEN_8bit
		RSPI0.SPDR.BYTE.LL		= 0U;
		g_rspi0_tx_count--;
#endif
#endif
	}
	else
	{
		/* Disable transmit interrupt */
		RSPI0.SPCR.BIT.SPTIE	= 0U;
		r_rspi0_callback_transmitend();
	}
}

/***********************************************************************************************************************
* Function Name: r_rspi0_receive_interrupt
* Description  : This function is SPRI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void	r_rspi0_receive_interrupt(void)
{
	if (g_rspi0_rx_length > g_rspi0_rx_count)
	{
#if defined(USE_RSPI_DATALEN_32bit)
		{
			uint32_u	t_dat;

			t_dat.l = RSPI0.SPDR.LONG;
			*(uint8_t *)gp_rspi0_rx_address++ = t_dat.b[3];
			*(uint8_t *)gp_rspi0_rx_address++ = t_dat.b[2];
			*(uint8_t *)gp_rspi0_rx_address++ = t_dat.b[1];
			*(uint8_t *)gp_rspi0_rx_address++ = t_dat.b[0];
		}
		g_rspi0_rx_count	   += 4;
#elif defined(USE_RSPI_DATALEN_16bit)
		{
			uint16_u	t_dat;

			t_dat.w = RSPI0.SPDR.WORD.L;
			*(uint8_t *)gp_rspi0_rx_address++ = t_dat.b[1];
			*(uint8_t *)gp_rspi0_rx_address++ = t_dat.b[0];
		}
		g_rspi0_rx_count	   += 2;
#else // USE_RSPI_DATALEN_8bit
		*(uint8_t *)gp_rspi0_rx_address = RSPI0.SPDR.BYTE.LL;
		gp_rspi0_rx_address++;
		g_rspi0_rx_count++;
#endif
	}
	if (g_rspi0_rx_length <= g_rspi0_rx_count)
	{
		/* Disable receive interrupt */
		RSPI0.SPCR.BIT.SPRIE	= 0U;
		/* Disable RSPI function */
		RSPI0.SPCR.BIT.SPE		= 0U;
		r_rspi0_callback_receiveend();
	}
}

/***********************************************************************************************************************
* Function Name: r_rspi0_create
* Description  : This function initializes RSPI0
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void	r_rspi0_create(void)
{
	/* Disable RSPI interrupts */
	RSPI0.SPCR.BIT.SPTIE	= 0U;
	RSPI0.SPCR.BIT.SPRIE	= 0U;
	/* Cancel RSPI0 module stop state */
	R_STB_StartModule(MODULE_RSPI0);
	/* Disable RSPI function */
	RSPI0.SPCR.BIT.SPE		= 0U;
	/* Set control registers */
	RSPI0.SPCR.BIT.MSTR 	= 1U;	// Master Mode
	RSPI0.SSLP.BIT.SSL0P	= D_RSPI_SSL_ACTIVE;
	RSPI0.SPPCR.BYTE		= _20_RSPI_MOSI_FIXING_MOIFV_BIT
							| _10_RSPI_MOSI_LEVEL_HIGH;
	RSPI0.SPSCR.BYTE		= _00_RSPI_SEQUENCE_LENGTH_1;
	RSPI0.SPBR.BYTE			= 10 - 1;	// 21 = 200kbps, 41 = 100kbps, 82 = 50kbps
							// f(P1) / { 2 * (SPBR + 1) * 2 ^ SPCMD[3:2] }
							// 66MHz / { 2 * (0x3B + 1) * 2 ^ 3 }
#if	defined(USE_RSPI_DATALEN_32bit)
	RSPI0.SPDCR.BYTE		= _60_RSPI_ACCESS_LONG;
#elif defined(USE_RSPI_DATALEN_16bit)
	RSPI0.SPDCR.BYTE		= _40_RSPI_ACCESS_WORD;
#else	// USE_RSPI_DATALEN_8bit
	RSPI0.SPDCR.BYTE		= _20_RSPI_ACCESS_BYTE;
#endif
	RSPI0.SPCKD.BYTE		= _00_RSPI_RSPCK_DELAY_1;
	RSPI0.SSLND.BYTE		= _00_RSPI_SSL_NEGATION_DELAY_1;
	RSPI0.SPND.BYTE			= _00_RSPI_NEXT_ACCESS_DELAY_1;
	RSPI0.SPCMD0.WORD		= _0001_RSPI_RSPCK_SAMPLING_EVEN
							| _0002_RSPI_RSPCK_POLARITY_HIGH
							| _000C_RSPI_BASE_BITRATE_8
							| _0000_RSPI_SIGNAL_ASSERT_SSL0
							| _0000_RSPI_SSL_KEEP_DISABLE
#if defined(USE_RSPI_DATALEN_32bit)
							| _0300_RSPI_DATA_LENGTH_BITS_32
#elif defined(USE_RSPI_DATALEN_16bit)
							| _0F00_RSPI_DATA_LENGTH_BITS_16
#else // USE_RSPI_DATALEN_8bit
							| _0700_RSPI_DATA_LENGTH_BITS_8
#endif
							| _0000_RSPI_MSB_FIRST
							| _0000_RSPI_NEXT_ACCESS_DELAY_DISABLE
							| _0000_RSPI_NEGATION_DELAY_DISABLE
							| _0000_RSPI_RSPCK_DELAY_DISABLE;
	/* Set GTIOC0X pin */
	GPIO.PWPR.BIT.B0WI		= 0U;
	__asm("nop");
	GPIO.PWPR.BIT.PFSWE		= 1U;
	__asm("nop");
	/* Set RSPCKA(PG0) pin */
	PORTG.PMR.BIT.PMR0		= 0U;
	GPIO.PG0PFS.BIT.PSEL	= 3U;
	PORTG.PODR.BIT.PODR0	= 0U;
	PORTG.PDR.BIT.PDR0		= 0U;
	PORTG.PMR.BIT.PMR0		= 1U;
	/* Set MOSIA(PG1) pin */
	PORTG.PMR.BIT.PMR1		= 0U;
	GPIO.PG1PFS.BIT.PSEL	= 3U;
	PORTG.PODR.BIT.PODR1	= 0U;
	PORTG.PDR.BIT.PDR1		= 0U;
	PORTG.PMR.BIT.PMR1		= 1U;
	/* Set MISOA(PG2) pin */
	PORTG.PMR.BIT.PMR2		= 0U;
	GPIO.PG2PFS.BIT.PSEL	= 3U;
	PORTG.PODR.BIT.PODR2	= 0U;
	PORTG.PDR.BIT.PDR2		= 0U;
	PORTG.PMR.BIT.PMR2		= 1U;
#ifdef	USE_RSPI_SSL0
	/* Set SSL0(PG3) pin */
	PORTG.PMR.BIT.PMR3		= 0U;
	GPIO.PG3PFS.BIT.PSEL	= 3U;
	PORTG.PODR.BIT.PODR3	= 0U;
	PORTG.PDR.BIT.PDR3		= 0U;
	PORTG.PMR.BIT.PMR3		= 1U;
#else	// !USE_RSPI_SSL0
	/* Set CS_GPIO(PG3) pin */
	PORTG.PMR.BIT.PMR3		= 0U;
	GPIO.PG3PFS.BIT.PSEL	= 0U;
	PORTG.PODR.BIT.PODR3	= D_RSPI_SSL_INACTIVE;
	PORTG.PDR.BIT.PDR3		= 3U;
	PORTG.PMR.BIT.PMR3		= 0U;
#endif	// USE_RSPI_SSL0
}

/***********************************************************************************************************************
* Function Name: r_rspi0_start
* Description  : This function starts the RSPI0 module operation
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void	r_rspi0_start(void)
{
	R_INTC_RegistIntFunc(INTC_ID_RSPI_SPTI0, (void *)r_rspi0_transmit_interrupt);
	R_INTC_Enable(INTC_ID_RSPI_SPTI0);
	R_INTC_SetPriority(INTC_ID_RSPI_SPTI0, 10);
	//
	R_INTC_RegistIntFunc(INTC_ID_RSPI_SPRI0, (void *)r_rspi0_receive_interrupt);
	R_INTC_Enable(INTC_ID_RSPI_SPRI0);
	R_INTC_SetPriority(INTC_ID_RSPI_SPRI0, 10);
	/* Enable RSPI interrupts */
	RSPI0.SPCR.BIT.SPTIE	= 1U;
	RSPI0.SPCR.BIT.SPRIE	= 1U;
	/* Enable RSPI function */
	RSPI0.SPCR.BIT.SPE		= 1U;
}

/***********************************************************************************************************************
* Function Name: r_rspi0_Send_Receive
* Description  : This function sends and receives RSPI0 data
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DMAC/DTC)
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer (not used when data is handled by DMAC/DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

void	r_rspi0_send_receive(
	void *		tx_buf,
	void *		rx_buf,
	uint16_t	len
)
{
	if(1U > len)
	{
		return;
	}
	/* Initialize the global counters */
	gp_rspi0_tx_address		= tx_buf;
	g_rspi0_tx_count		= len;
	gp_rspi0_rx_address		= rx_buf;
	g_rspi0_rx_length		= len;
	g_rspi0_rx_count		= 0U;
	/* Enable transmit interrupt */
	RSPI0.SPCR.BIT.SPTIE	= 1U;
	/* Enable receive interrupt */
	RSPI0.SPCR.BIT.SPRIE	= 1U;
	/* Enable RSPI function */
	RSPI0.SPCR.BIT.SPE		= 1U;
	//
	r_cnt_rx0				= 0U;
	//
#if defined(USE_RSPI_DATALEN_32bit)
	{
		uint32_u	t_dat;

		t_dat.b[3] = *gp_rspi0_tx_address++;
		t_dat.b[2] = *gp_rspi0_tx_address++;
		t_dat.b[1] = *gp_rspi0_tx_address++;
		t_dat.b[0] = *gp_rspi0_tx_address++;
		RSPI0.SPDR.LONG		= t_dat.l;
	}
	g_rspi0_tx_count	   -= 4;
#elif defined(USE_RSPI_DATALEN_16bit)
	{
		uint16_u	t_dat;

		t_dat.b[1] = *gp_rspi0_tx_address++;
		t_dat.b[0] = *gp_rspi0_tx_address++;
		RSPI0.SPDR.WORD.L	= t_dat.w;
	}
	g_rspi0_tx_count	   -= 2;
#else // USE_RSPI_DATALEN_8bit
	RSPI0.SPDR.BYTE.LL		= (*(uint8_t*)gp_rspi0_tx_address);
	gp_rspi0_tx_address++;
	g_rspi0_tx_count--;
#endif
}

void	r_rspi0_zerosend_receive(
	void *		rx_buf,
	uint16_t	len
)
{
	g_rspi0_tx_count		= len;
	gp_rspi0_rx_address		= rx_buf;
	g_rspi0_rx_length		= len;
	g_rspi0_rx_count		= 0U;
	/* Enable transmit interrupt */
	RSPI0.SPCR.BIT.SPTIE	= 1U;
	/* Enable receive interrupt */
	RSPI0.SPCR.BIT.SPRIE	= 1U;
	/* Enable RSPI function */
	RSPI0.SPCR.BIT.SPE		= 1U;
	//
	r_cnt_rx0				= 0U;
	//
#if defined(USE_RSPI_DATALEN_32bit)
	RSPI0.SPDR.LONG			= 0U;
	g_rspi0_tx_count	   -= 4;
#elif defined(USE_RSPI_DATALEN_16bit)
	RSPI0.SPDR.WORD.L		= 0U;
	g_rspi0_tx_count	   -= 2;
#else // USE_RSPI_DATALEN_8bit
	RSPI0.SPDR.BYTE.LL		= 0U;
	g_rspi0_tx_count--;
#endif
}

/***********************************************************************************************************************
 * EOF
 ***********************************************************************************************************************/
