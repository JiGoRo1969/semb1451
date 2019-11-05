/******************************************************************************
 * File Name	 : scifa.c
 * Version       : 1.0.0
 * Creation Date : 2019-11-01
 * Device(s)	 : RZ/A2M
 * Tool-Chain	 : e2Studio Ver 7.4.0
 *				 : GCC ARM Embedded 6.3.1.20170620
 * OS			 : FreeRTOS
 * H/W Platform  : SEMB1451/1452
 * Description	 : RZ/A2M Program to control SCIFA functions
 * Operation	 :
 * Limitations	 :
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	"r_typedefs.h"
#include	"r_scifa_drv_api.h"
#include	"r_os_abstraction_api.h"
#include	"r_gpio_drv_api.h"
#include	"scifa.h"
#include	<math.h>

/******************************************************************************
 * Definition for Function Selection
 *****************************************************************************/

/******************************************************************************
 * Macro definitions (Register bit)
 *****************************************************************************/

/******************************************************************************
 * Macro definitions
 *****************************************************************************/

/******************************************************************************
 * Typedef definitions
 *****************************************************************************/

/******************************************************************************
 * Global functions (Prototype definition)
 *****************************************************************************/

void	r_callback_scifa0_reie(void);
void	r_callback_scifa1_reie(void);
void	r_callback_scifa0_teie(void);
void	r_callback_scifa1_teie(void);
void	r_callback_scifa2_teie(void);
void	r_callback_scifa3_teie(void);
void	r_callback_scifa4_teie(void);
void	r_scifa_write(uint8_t ch, uint16_t len);
void	r_scifa_waiting_write_completed(uint8_t ch);
void	r_scifa_read(uint8_t ch, uint16_t len);
void	r_scifa_waiting_read_completed(uint8_t ch);
void	r_scifa_init(void);

/******************************************************************************
 * Global values
 *****************************************************************************/

int_t		g_handle_scifa[5];
volatile uint8_t	g_scifa_txd[BUF_SIZE] __attribute__((aligned (32)));
volatile uint8_t	g_scifa_rxd[5][BUF_SIZE] __attribute__((aligned (32)));

/******************************************************************************
 * Local functions
 *****************************************************************************/

/******************************************************************************
 * Local values
 *****************************************************************************/

static volatile uint8_t	gs_flg_scifa0_rxd = 0;
static volatile uint8_t	gs_flg_scifa1_rxd = 0;
static volatile uint8_t	gs_flg_scifa0_txd = 0;
static volatile uint8_t	gs_flg_scifa1_txd = 0;
static volatile uint8_t	gs_flg_scifa2_txd = 0;
static volatile uint8_t	gs_flg_scifa3_txd = 0;
static volatile uint8_t	gs_flg_scifa4_txd = 0;

static st_r_drv_gpio_pin_rw_t
	gs_pA7[2] = {{GPIO_PORT_A_PIN_7, GPIO_LEVEL_LOW,  GPIO_SUCCESS}, {GPIO_PORT_A_PIN_7, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
	gs_p72[2] = {{GPIO_PORT_7_PIN_2, GPIO_LEVEL_LOW,  GPIO_SUCCESS}, {GPIO_PORT_7_PIN_2, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
#ifdef CONNECT_SERVO_TO_SCIFA2
	gs_pF3[2] = {{GPIO_PORT_F_PIN_3, GPIO_LEVEL_LOW,  GPIO_SUCCESS}, {GPIO_PORT_F_PIN_3, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
#endif	// CONNECT_SERVO_TO_SCIFA2
#ifdef CONNECT_SERVO_TO_SCIFA3
	gs_pF0[2] = {{GPIO_PORT_F_PIN_0, GPIO_LEVEL_LOW,  GPIO_SUCCESS}, {GPIO_PORT_F_PIN_0, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
#endif	// CONNECT_SERVO_TO_SCIFA3
#ifdef CONNECT_SERVO_TO_SCIFA4
	gs_pA2[2] = {{GPIO_PORT_A_PIN_3, GPIO_LEVEL_LOW,  GPIO_SUCCESS}, {GPIO_PORT_A_PIN_3, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
#endif	// CONNECT_SERVO_TO_SCIFA4
	gs_pB0[2] = {{GPIO_PORT_B_PIN_0, GPIO_LEVEL_LOW,  GPIO_SUCCESS}, {GPIO_PORT_B_PIN_0, GPIO_LEVEL_HIGH, GPIO_SUCCESS}};

/******************************************************************************
 * Imported global functions
 *****************************************************************************/

/******************************************************************************
 * Imported global variables
 *****************************************************************************/

extern	int_t	g_handle_gpio;

/******************************************************************************
 * Function Name: r_callback_scifa0_reie
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_callback_scifa0_reie(void)
{
	gs_flg_scifa0_rxd = 1;
}
/******************************************************************************
 * End of function r_callback_scifa0_reie
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_callback_scifa1_reie
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_callback_scifa1_reie(void)
{
	gs_flg_scifa1_rxd = 1;
}
/******************************************************************************
 * End of function r_callback_scifa1_reie
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_callback_scifa0_teie
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_callback_scifa0_teie(void)
{
	gs_flg_scifa0_txd = 1;
	SET_DIR0(D_DIR_READ);
}
/******************************************************************************
 * End of function r_callback_scifa0_teie
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_callback_scifa1_teie
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_callback_scifa1_teie(void)
{
	gs_flg_scifa1_txd = 1;
	SET_DIR1(D_DIR_READ);
}
/******************************************************************************
 * End of function r_callback_scifa1_teie
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_callback_scifa2_teie
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_callback_scifa2_teie(void)
{
	gs_flg_scifa2_txd = 1;
}
/******************************************************************************
 * End of function r_callback_scifa2_teie
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_callback_scifa3_teie
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_callback_scifa3_teie(void)
{
	gs_flg_scifa3_txd = 1;
}
/******************************************************************************
 * End of function r_callback_scifa3_teie
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_callback_scifa4_teie
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_callback_scifa4_teie(void)
{
	gs_flg_scifa4_txd = 1;
}
/******************************************************************************
 * End of function r_callback_scifa4_teie
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_scifa_write
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_scifa_write(uint8_t ch, uint16_t len)
{
	switch(ch)
	{
#ifdef CONNECT_SERVO_TO_SCIFA0
	case 0:
		SET_DIR0(D_DIR_WRITE);
		gs_flg_scifa0_txd = 0;
		write(g_handle_scifa[0], (uint8_t *)g_scifa_txd, len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA0
#ifdef CONNECT_SERVO_TO_SCIFA1
	case 1:
		SET_DIR1(D_DIR_WRITE);
		gs_flg_scifa1_txd = 0;
		write(g_handle_scifa[1], (uint8_t *)g_scifa_txd, len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA1
#ifdef CONNECT_SERVO_TO_SCIFA2
	case 2:
		gs_flg_scifa2_txd = 0;
		write(g_handle_scifa[2], (uint8_t *)g_scifa_txd, len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA2
#ifdef CONNECT_SERVO_TO_SCIFA3
	case 3:
		gs_flg_scifa3_txd = 0;
		write(g_handle_scifa[3], (uint8_t *)g_scifa_txd, len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA3
#ifdef CONNECT_SERVO_TO_SCIFA4
	case 4:
		gs_flg_scifa4_txd = 0;
		write(g_handle_scifa[4], (uint8_t *)g_scifa_txd, len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA4
	}
}
/******************************************************************************
 * End of function r_scifa_write
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_scifa_waiting_write_completed
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_scifa_waiting_write_completed(uint8_t ch)
{
	switch(ch)
	{
#ifdef CONNECT_SERVO_TO_SCIFA0
	case 0:
		while(0 == gs_flg_scifa0_txd)
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA0
#ifdef CONNECT_SERVO_TO_SCIFA1
	case 1:
		while(0 == gs_flg_scifa1_txd)
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA1
#ifdef CONNECT_SERVO_TO_SCIFA2
	case 2:
		while(0 == gs_flg_scifa2_txd)
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA2
#ifdef CONNECT_SERVO_TO_SCIFA3
	case 3:
		while(0 == gs_flg_scifa3_txd)
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA3
#ifdef CONNECT_SERVO_TO_SCIFA4
	case 4:
		while(0 == gs_flg_scifa4_txd)
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA4
	}
}
/******************************************************************************
 * End of function r_scifa_waiting_write_completed
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_scifa_read
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_scifa_read(uint8_t ch, uint16_t len)
{
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
#define	CONNECT_SERVO_TO_SCIFA0		// tempolary only in r_scifa_read()
#undef	CONNECT_SERVO_TO_SCIFA1		// tempolary only in r_scifa_read()
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	switch(ch)
	{
#ifdef CONNECT_SERVO_TO_SCIFA0
	case 0:
		gs_flg_scifa0_rxd = 0;
		read(g_handle_scifa[0], (uint8_t *)g_scifa_rxd[0], len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA0
#ifdef CONNECT_SERVO_TO_SCIFA1
	case 1:
		gs_flg_scifa1_rxd = 0;
		read(g_handle_scifa[1], (uint8_t *)g_scifa_rxd[1], len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA1
#ifdef CONNECT_SERVO_TO_SCIFA2
	case 2:
	//	gs_flg_scifa2_rxd = 0;
		read(g_handle_scifa[2], (uint8_t *)g_scifa_rxd[2], len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA2
#ifdef CONNECT_SERVO_TO_SCIFA3
	case 3:
	//	gs_flg_scifa3_rxd = 0;
		read(g_handle_scifa[3], (uint8_t *)g_scifa_rxd[3], len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA3
#ifdef CONNECT_SERVO_TO_SCIFA4
	case 4:
	//	gs_flg_scifa4_rxd = 0;
		read(g_handle_scifa[4], (uint8_t *)g_scifa_rxd[4], len);
		break;
#endif	// CONNECT_SERVO_TO_SCIFA4
	}
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
#undef	CONNECT_SERVO_TO_SCIFA0		// tempolary only in r_scifa_read()
#define	CONNECT_SERVO_TO_SCIFA1		// tempolary only in r_scifa_read()
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
}
/******************************************************************************
 * End of function r_scifa_read
 *****************************************************************************/


/******************************************************************************
 * Function Name: r_scifa_waiting_read_completed
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_scifa_waiting_read_completed(uint8_t ch)
{
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
#define	CONNECT_SERVO_TO_SCIFA0		// tempolary only in r_scifa_waiting_read_completed()
#undef	CONNECT_SERVO_TO_SCIFA1		// tempolary only in r_scifa_waiting_read_completed()
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	switch(ch)
	{
#ifdef CONNECT_SERVO_TO_SCIFA0
	case 0:
		while(0 == gs_flg_scifa0_rxd)
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA0
#ifdef CONNECT_SERVO_TO_SCIFA1
	case 1:
#if 1
		for(volatile uint32_t i = 0; (i < D_LOOPLIMIT) && (0 == gs_flg_scifa1_rxd); i++)
#else
		while(0 == gs_flg_scifa1_rxd)
#endif
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA1
#ifdef CONNECT_SERVO_TO_SCIFA2
	case 2:
	//	while(0 == gs_flg_scifa2_rxd)
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA2
#ifdef CONNECT_SERVO_TO_SCIFA3
	case 3:
	//	while(0 == gs_flg_scifa3_rxd)
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA3
#ifdef CONNECT_SERVO_TO_SCIFA4
	case 4:
	//	while(0 == gs_flg_scifa4_rxd)
		{
			r_nop();
		}
		break;
#endif	// CONNECT_SERVO_TO_SCIFA4
	}
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
#undef	CONNECT_SERVO_TO_SCIFA0		// tempolary only in r_scifa_waiting_read_completed()
#define	CONNECT_SERVO_TO_SCIFA1		// tempolary only in r_scifa_waiting_read_completed()
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
}
/******************************************************************************
 * End of function r_scifa_waiting_read_completed
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_scifa_init
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_scifa_init(void)
{
	static scifa_config_t	gs_set_cfg[5];

#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
#define	CONNECT_SERVO_TO_SCIFA0		// tempolary only in r_scifa_init()
//#define	CONNECT_SERVO_TO_SCIFA4		// tempolary only in r_scifa_init()
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	//
	// SCIF0 SEL0
	//
#ifdef CONNECT_SERVO_TO_SCIFA0
	SET_DIR0(D_DIR_WRITE);
	g_handle_scifa[0] = open("\\\\.\\scifa0", O_RDWR);
	if(0 > g_handle_scifa[0])
	{
		while(1) r_nop();
	}
	control(g_handle_scifa[0], CTL_SCIFA_GET_CONFIGURATION, &gs_set_cfg[0]);
	control(g_handle_scifa[0], CTL_SCIFA_SET_CONFIGURATION, &gs_set_cfg[0]);
#endif	// CONNECT_SERVO_TO_SCIFA0
	//
	// SCIF1 SEL1
	//
#ifdef CONNECT_SERVO_TO_SCIFA1
	SET_DIR1(D_DIR_WRITE);
	g_handle_scifa[1] = open("\\\\.\\scifa1", O_RDWR);
	if(0 > g_handle_scifa[1])
	{
		while(1) r_nop();
	}
	control(g_handle_scifa[1], CTL_SCIFA_GET_CONFIGURATION, &gs_set_cfg[1]);
	control(g_handle_scifa[1], CTL_SCIFA_SET_CONFIGURATION, &gs_set_cfg[1]);
#endif	// CONNECT_SERVO_TO_SCIFA0
	//
	// SCIF2 SEL2
	//
#ifdef CONNECT_SERVO_TO_SCIFA2
#if 1
	SET_DUPLEX2(D_HALF_DUPLEX);
#else
	SET_DUPLEX2(D_FULL_DUPLEX);
#endif
	g_handle_scifa[2] = open("\\\\.\\scifa2", O_RDWR);
	if(0 > g_handle_scifa[2])
	{
		while(1) r_nop();
	}
	control(g_handle_scifa[2], CTL_SCIFA_GET_CONFIGURATION, &gs_set_cfg[2]);
	control(g_handle_scifa[2], CTL_SCIFA_SET_CONFIGURATION, &gs_set_cfg[2]);
#endif
	//
	// SCIF3 SEL3
	//
#ifdef CONNECT_SERVO_TO_SCIFA3
#if 1
	SET_DUPLEX3(D_HALF_DUPLEX);
#else
	SET_DUPLEX3(D_FULL_DUPLEX);
#endif
	g_handle_scifa[3] = open("\\\\.\\scifa3", O_RDWR);
	if(0 > g_handle_scifa[3])
	{
		while(1) r_nop();
	}
	control(g_handle_scifa[3], CTL_SCIFA_GET_CONFIGURATION, &gs_set_cfg[3]);
	control(g_handle_scifa[3], CTL_SCIFA_SET_CONFIGURATION, &gs_set_cfg[3]);
#endif
	//
	// SCIF4 SEL4
	//
#ifdef CONNECT_SERVO_TO_SCIFA4
#if 1
	SET_DUPLEX4(D_HALF_DUPLEX);
#else
	SET_DUPLEX4(D_FULL_DUPLEX);
#endif
	g_handle_scifa[4] = open("\\\\.\\scifa4", O_RDWR);
	if(0 > g_handle_scifa[4])
	{
		while(1) r_nop();
	}
	control(g_handle_scifa[4], CTL_SCIFA_GET_CONFIGURATION, &gs_set_cfg[4]);
	control(g_handle_scifa[4], CTL_SCIFA_SET_CONFIGURATION, &gs_set_cfg[4]);
#endif
	//
	//	Power Gate(PB0) ON(=1) to use SERVO MOTOR
	//
    direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pB0[1]);
	//
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	SET_DIR0(D_DIR_READ);
#undef	CONNECT_SERVO_TO_SCIFA0		// tempolary only in r_scifa_init()
//#undef	CONNECT_SERVO_TO_SCIFA4		// tempolary only in r_scifa_init()
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
}
/******************************************************************************
 * End of function r_ss_init_port
 *****************************************************************************/

/***********************************************************************************************************************
 * EOF
 ***********************************************************************************************************************/
