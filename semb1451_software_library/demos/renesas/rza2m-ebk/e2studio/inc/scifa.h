/******************************************************************************
 * File Name	 : scifa.h
 * Version       : 1.0.0
 * Creation Date : 2019-11-01
 * Device(s)	 : RZ/A2M
 * Tool-Chain	 : e2Studio Ver 7.4.0
 *				 : GCC ARM Embedded 6.3.1.20170620
 * OS			 : FreeRTOS
 * H/W Platform  : SEMB1451/1452
 * Description	 : RZ/A2M Program to control SCIFAn functions
 * Operation	 :
 * Limitations	 :
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

/******************************************************************************
 * Definition for Function Selection
 *****************************************************************************/

//#define 	USE_DEBUG_PRINTF_INFORMATION
//#define	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0

//#define 	CONNECT_SERVO_TO_SCIFA0
#define 	CONNECT_SERVO_TO_SCIFA1
//#define 	CONNECT_SERVO_TO_SCIFA2
//#define 	CONNECT_SERVO_TO_SCIFA3
//#define 	CONNECT_SERVO_TO_SCIFA4

#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
#undef 		CONNECT_SERVO_TO_SCIFA0
#define 	CONNECT_SERVO_TO_SCIFA1
#define 	USE_DEBUG_PRINTF_INFORMATION
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0

/******************************************************************************
 * Macro definitions (Register bit)
 *****************************************************************************/

/******************************************************************************
 * Macro definitions
 *****************************************************************************/

#define	BUF_SIZE		(64)

#define	D_DIR_WRITE		(1)
#define	D_DIR_READ		(0)
#define	D_FULL_DUPLEX	(1)
#define	D_HALF_DUPLEX	(0)

#define	SET_DIR0(X)		{direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pA7[(X)]);}
#define	SET_DIR1(X)		{direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p72[(X)]);}
#define	SET_DUPLEX2(X)	{direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pF3[(X)]);}
#define	SET_DUPLEX3(X)	{direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pF0[(X)]);}
#define	SET_DUPLEX4(X)	{direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pA2[(X)]);}

#define	r_nop()			__asm("nop")

#define	D_LOOPLIMIT		(528*1000*10)

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

extern	int_t				g_handle_scifa[5];
extern	volatile uint8_t	g_scifa_txd[BUF_SIZE];
extern	volatile uint8_t	g_scifa_rxd[5][BUF_SIZE];

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

/*****************************************************************************
 * EOF
 *****************************************************************************/
