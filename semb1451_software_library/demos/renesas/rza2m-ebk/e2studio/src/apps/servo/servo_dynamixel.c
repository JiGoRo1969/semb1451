/******************************************************************************
 * File Name	 : servo_dynamixel.c
 * Version       : 1.0.0
 * Creation Date : 2019-11-01
 * Device(s)	 : RZ/A2M
 * Tool-Chain	 : e2Studio Ver 7.4.0
 *				 : GCC ARM Embedded 6.3.1.20170620
 * OS			 : FreeRTOS
 * H/W Platform  : SEMB1451/1452
 * Description	 : RZ/A2M Program to control dynamixel servos
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

//#define 	USE_DEBUG_PRINTF_INFORMATION
//#define 	USE_DEBUG_SCIFA1_WITH_SCIFA0

/******************************************************************************
 * Macro definitions (Register bit)
 *****************************************************************************/

/******************************************************************************
 * Macro definitions
 *****************************************************************************/

#define	D_OFFSET_ID					(11)
#define	D_CNT_SERVO					(5)

#define	D_DIV_SLOWSETHOME			(100)
#define	D_TIME_SLOWSETHOME			(50)

#define	DYNAINST_PING				(0x01)
#define	DYNAINST_READ_DATA			(0x02)
#define	DYNAINST_WRITE_DATA			(0x03)
#define	DYNAINST_REG_WRITE			(0x04)
#define	DYNAINST_ACTION				(0x05)
#define	DYNAINST_RESET				(0x06)
#define	DYNAINST_SYNC_WRITE			(0x83)

#define	DYNAPARA_ANGLE_FULLRANGE	(4096)
#define	DYNAPARA_ANGLE_HALFRANGE	(2048)	// == (DYNAPARA_ANGLE_FULLRANGE + 1) / 2)
#define	DYNAPARA_ANGLE_CENTER		(2047)	// == (DYNAPARA_ANGLE_FULLRANGE + 1) / 2 - 1)

#define	r_nop()		__asm("nop")

/******************************************************************************
 * Typedef definitions
 *****************************************************************************/

typedef	union {
	uint16_t	w;
	uint8_t		b[2];
} uint16_u;

/******************************************************************************
 * Global functions (Prototype definition)
 *****************************************************************************/

void		r_sample_servo_set_position(uint16_t, uint16_t);
void		r_sample_servo_set_angle(uint16_t, int32_t);
void		r_sample_servo_send(void);
uint16_t	r_sample_servo_read(uint8_t, uint8_t);
int			r_sample_get_current_positions(void);
void		r_sample_goto_home_position(void);

/******************************************************************************
 * Global values
 *****************************************************************************/

uint16_t	g_angle_init[D_CNT_SERVO];
uint16_u	g_angle_setval[D_CNT_SERVO];
uint16_t	g_angle_current[D_CNT_SERVO];

/******************************************************************************
 * Local functions
 *****************************************************************************/

static	uint16_t	calc_crc16(uint8_t *ptr, uint8_t len);

/******************************************************************************
 * Local values
 *****************************************************************************/

/******************************************************************************
 * Imported global functions
 *****************************************************************************/

/******************************************************************************
 * Imported global variables
 *****************************************************************************/

/***********************************************************************************************************************
 * Function Name: calc_crc16
 ***********************************************************************************************************************/
static
uint16_t	calc_crc16(
	uint8_t		*ptr,
	uint8_t		len
)
{
	uint16_t	crc;

	crc = 0;
	for(int i = 0; i < len; i++)
	{
		crc = crc ^ (uint16_t)(ptr[i] << 8);
		for(int j = 0; j < 8; j++)
		{
			if( crc & 0x8000 )
			{
				crc = (uint16_t)(crc << 1U) ^ 0x8005U;
			}
			else
			{
				crc = (uint16_t)(crc << 1U);
			}
		}
	}
	return crc;
}

/***********************************************************************************************************************
* Function Name: r_servo_set_position
* Arguments    : Servo ID = r_id, Servo Position = r_val
* Return Value : None
***********************************************************************************************************************/
void	r_sample_servo_set_position(
	uint16_t	r_id,
	uint16_t	r_val
)
{
	if(DYNAPARA_ANGLE_FULLRANGE <= r_val)
	{
		r_val = DYNAPARA_ANGLE_FULLRANGE - 1;
	}
	g_angle_setval[r_id].w = r_val;
}


/***********************************************************************************************************************
* Function Name: r_servo_set_angle
* Arguments    : Servo ID = r_id, Servo Angle = r_val
* Return Value : None
***********************************************************************************************************************/
void	r_sample_servo_set_angle(
	uint16_t	r_id,
	int32_t		r_val
)
{
	if(+DYNAPARA_ANGLE_HALFRANGE <= r_val)
	{
		r_val = +(DYNAPARA_ANGLE_HALFRANGE - 1);
	}
	if(-DYNAPARA_ANGLE_HALFRANGE >= r_val)
	{
		r_val = -(DYNAPARA_ANGLE_HALFRANGE - 1);
	}
	r_val += DYNAPARA_ANGLE_CENTER;
	g_angle_setval[r_id].w = (uint16_t)r_val;
}

/******************************************************************************
 * Function Name: r_sample_servo_init
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_sample_servo_init( void )
{
	uint16_u	sum;

	r_nop();
	//
	for(int i = 0; i < D_CNT_SERVO; i++)
	{
		g_angle_init[i] = DYNAPARA_ANGLE_CENTER;
	}
	//
#undef	T_LEN
#define	T_LEN	(24)
	//
	g_scifa_txd[ 0]	= 0xFF;							// preamble
	g_scifa_txd[ 1]	= 0xFF;							// preamble
	g_scifa_txd[ 2]	= 0xFD;							// preamble
	g_scifa_txd[ 3]	= 0x00;							// preamble
	g_scifa_txd[ 4]	= 0xFE;							// ID(fixed 254)
	g_scifa_txd[ 5]	= T_LEN - 7;					// Low(Length)
	g_scifa_txd[ 6]	= 0;							// High(Length)
	g_scifa_txd[ 7]	= DYNAINST_SYNC_WRITE;			// Instruction
	g_scifa_txd[ 8]	= 68;							// Low(Item)  = status return level
	g_scifa_txd[ 9]	= 0;							// High(Item) = status return level
	g_scifa_txd[10]	= 1;							// Low(Byte Size)
	g_scifa_txd[11]	= 0;							// High(Byte Size)
	g_scifa_txd[12]	= 0 + D_OFFSET_ID;				// ID
	g_scifa_txd[13]	= 1;							// 0=ping, 1=ping+read, 2=all
	g_scifa_txd[14]	= 1 + D_OFFSET_ID;				// ID
	g_scifa_txd[15]	= 1;							// 0=ping, 1=ping+read, 2=all
	g_scifa_txd[16]	= 2 + D_OFFSET_ID;				// ID
	g_scifa_txd[17]	= 1;							// 0=ping, 1=ping+read, 2=all
	g_scifa_txd[18]	= 3 + D_OFFSET_ID;				// ID
	g_scifa_txd[19]	= 1;							// 0=ping, 1=ping+read, 2=all
	g_scifa_txd[20]	= 4 + D_OFFSET_ID;				// ID
	g_scifa_txd[21]	= 1;							// 0=ping, 1=ping+read, 2=all
	sum.w = calc_crc16((uint8_t *)g_scifa_txd, T_LEN-2);	// Check SUM
	g_scifa_txd[T_LEN-2] = sum.b[0];
	g_scifa_txd[T_LEN-1] = sum.b[1];
	//
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	r_scifa_read(0, T_LEN);
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	//
	r_scifa_write(0, T_LEN);
	r_scifa_write(1, T_LEN);
	r_scifa_write(2, T_LEN);
	r_scifa_write(3, T_LEN);
	r_scifa_write(4, T_LEN);
	//
	r_scifa_waiting_write_completed(0);
	r_scifa_waiting_write_completed(1);
	r_scifa_waiting_write_completed(2);
	r_scifa_waiting_write_completed(3);
	r_scifa_waiting_write_completed(4);
	//
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	r_scifa_waiting_read_completed(0);
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
#ifdef	USE_DEBUG_PRINTF_INFORMATION
	fprintf(stdout, "init:%02lX", g_scifa_rxd[0][0]);
	for(int i = 1; i < T_LEN; i++)
	{
		fprintf(stdout, ",%02lX", g_scifa_rxd[0][i]);
	}
	fprintf(stdout, "\n");
#endif	// USE_DEBUG_PRINTF_INFORMATION
	//
	R_OS_TaskSleep(50);
	//
#undef	T_LEN
#define	T_LEN	(24)
	//
	g_scifa_txd[ 0]	= 0xFF;							// preamble
	g_scifa_txd[ 1]	= 0xFF;							// preamble
	g_scifa_txd[ 2]	= 0xFD;							// preamble
	g_scifa_txd[ 3]	= 0x00;							// preamble
	g_scifa_txd[ 4]	= 0xFE;							// ID(fixed 254)
	g_scifa_txd[ 5]	= T_LEN - 7;					// Low(Length)
	g_scifa_txd[ 6]	= 0;							// High(Length)
	g_scifa_txd[ 7]	= DYNAINST_SYNC_WRITE;			// Instruction
	g_scifa_txd[ 8]	= 64;							// Low(Item)  = Torque Enable
	g_scifa_txd[ 9]	= 0;							// High(Item) = Torque Enable
	g_scifa_txd[10]	= 1;							// Low(Byte Size)
	g_scifa_txd[11]	= 0;							// High(Byte Size)
	g_scifa_txd[12]	= 0 + D_OFFSET_ID;				// ID
	g_scifa_txd[13]	= 1;							// 1=enable, 0=disable
	g_scifa_txd[14]	= 1 + D_OFFSET_ID;				// ID
	g_scifa_txd[15]	= 1;							// 1=enable, 0=disable
	g_scifa_txd[16]	= 2 + D_OFFSET_ID;				// ID
	g_scifa_txd[17]	= 1;							// 1=enable, 0=disable
	g_scifa_txd[18]	= 3 + D_OFFSET_ID;				// ID
	g_scifa_txd[19]	= 1;							// 1=enable, 0=disable
	g_scifa_txd[20]	= 4 + D_OFFSET_ID;				// ID
	g_scifa_txd[21]	= 1;							// 1=enable, 0=disable
	sum.w = calc_crc16((uint8_t *)g_scifa_txd, T_LEN-2);	// Check SUM
	g_scifa_txd[T_LEN-2] = sum.b[0];
	g_scifa_txd[T_LEN-1] = sum.b[1];
	//
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	r_scifa_read(0, T_LEN);
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	//
	r_scifa_write(0, T_LEN+2);
	r_scifa_write(1, T_LEN+2);
	r_scifa_write(2, T_LEN+2);
	r_scifa_write(3, T_LEN+2);
	r_scifa_write(4, T_LEN+2);
	//
	r_scifa_waiting_write_completed(0);
	r_scifa_waiting_write_completed(1);
	r_scifa_waiting_write_completed(2);
	r_scifa_waiting_write_completed(3);
	r_scifa_waiting_write_completed(4);
	//
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	r_scifa_waiting_read_completed(0);
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
#ifdef	USE_DEBUG_PRINTF_INFORMATION
	fprintf(stdout, "init:%02lX", g_scifa_rxd[0][0]);
	for(int i = 1; i < T_LEN; i++)
	{
		fprintf(stdout, ",%02lX", g_scifa_rxd[0][i]);
	}
	fprintf(stdout, "\n");
#endif	// USE_DEBUG_PRINTF_INFORMATION
	//
	R_OS_TaskSleep(50);
}

/***********************************************************************************************************************
 * Function Name: r_sample_servo_send
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void	r_sample_servo_send( void )
{
	uint16_u	sum;

	r_nop();
	//
#undef	T_LEN
#define	T_LEN	(39)
	//
	g_scifa_txd[ 0]	= 0xFF;							// preamble
	g_scifa_txd[ 1]	= 0xFF;							// preamble
	g_scifa_txd[ 2]	= 0xFD;							// preamble
	g_scifa_txd[ 3]	= 0x00;							// preamble
	g_scifa_txd[ 4]	= 0xFE;							// ID(fixed 254)
	g_scifa_txd[ 5]	= T_LEN - 7;					// Low(Length)
	g_scifa_txd[ 6]	= 0;							// High(Length)
	g_scifa_txd[ 7]	= DYNAINST_SYNC_WRITE;			// Instruction
	g_scifa_txd[ 8]	= 116;							// Low(Item)  = Goal Position
	g_scifa_txd[ 9]	= 0;							// High(Item) = Goal Position
	g_scifa_txd[10]	= 4;							// Low(Byte Size)
	g_scifa_txd[11]	= 0;							// High(Byte Size)
	g_scifa_txd[12]	= 0 + D_OFFSET_ID;				// ID
	g_scifa_txd[13]	= g_angle_setval[0].b[0];		// low(angle)
	g_scifa_txd[14]	= g_angle_setval[0].b[1];		// high(angle)
	g_scifa_txd[15]	= 0;							//
	g_scifa_txd[16]	= 0;							//
	g_scifa_txd[17]	= 1 + D_OFFSET_ID;				// ID
	g_scifa_txd[18]	= g_angle_setval[1].b[0];		// low(angle)
	g_scifa_txd[19]	= g_angle_setval[1].b[1];		// high(angle)
	g_scifa_txd[20]	= 0;							//
	g_scifa_txd[21]	= 0;							//
	g_scifa_txd[22]	= 2 + D_OFFSET_ID;				// ID
	g_scifa_txd[23]	= g_angle_setval[2].b[0];		// low(angle)
	g_scifa_txd[24]	= g_angle_setval[2].b[1];		// high(angle)
	g_scifa_txd[25]	= 0;							//
	g_scifa_txd[26]	= 0;							//
	g_scifa_txd[27]	= 3 + D_OFFSET_ID;				// ID
	g_scifa_txd[28]	= g_angle_setval[3].b[0];		// low(angle)
	g_scifa_txd[29]	= g_angle_setval[3].b[1];		// high(angle)
	g_scifa_txd[30]	= 0;							//
	g_scifa_txd[31]	= 0;							//
	g_scifa_txd[32]	= 4 + D_OFFSET_ID;				// ID
	g_scifa_txd[33]	= g_angle_setval[4].b[0];		// low(angle)
	g_scifa_txd[34]	= g_angle_setval[4].b[1];		// high(angle)
	g_scifa_txd[35]	= 0;							//
	g_scifa_txd[36]	= 0;							//
	sum.w = calc_crc16((uint8_t *)g_scifa_txd, T_LEN-2);	// Check SUM
	g_scifa_txd[T_LEN-2] = sum.b[0];
	g_scifa_txd[T_LEN-1] = sum.b[1];
	//
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	r_scifa_read(0, T_LEN);
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	//
	r_scifa_write(0, T_LEN+2);
	r_scifa_write(1, T_LEN+2);
	r_scifa_write(2, T_LEN+2);
	r_scifa_write(3, T_LEN+2);
	r_scifa_write(4, T_LEN+2);
	//
	r_scifa_waiting_write_completed(0);
	r_scifa_waiting_write_completed(1);
	r_scifa_waiting_write_completed(2);
	r_scifa_waiting_write_completed(3);
	r_scifa_waiting_write_completed(4);
	//
#ifdef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	r_scifa_waiting_read_completed(0);
#endif	// USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
#ifdef	USE_DEBUG_PRINTF_INFORMATION
	fprintf(stdout,"send:%02lX", g_scifa_rxd[0][0]);
	for(int i = 1; i < T_LEN; i++)
	{
		fprintf(stdout,",%02lX", g_scifa_rxd[0][i]);
	}
	fprintf(stdout,"\n");
#endif	// USE_DEBUG_PRINTF_INFORMATION
	r_nop();
}

/***********************************************************************************************************************
 * Function Name : r_sample_servo_read
 * Arguments	 : id   = SERVO ID
 *				 : adrs = SERVO register address
 * Return Value	 : None
 ***********************************************************************************************************************/
uint16_t	r_sample_servo_read(
	uint8_t		id,
	uint8_t		adrs
)
{
	uint16_u	sum;
	uint16_u	retval;

	r_nop();
	//
#undef	T_LEN
#define	T_LEN	(14)
	//
	g_scifa_txd[ 0]	= 0xFF;							// preamble
	g_scifa_txd[ 1]	= 0xFF;							// preamble
	g_scifa_txd[ 2]	= 0xFD;							// preamble
	g_scifa_txd[ 3]	= 0x00;							// preamble
	g_scifa_txd[ 4]	= (uint8_t)(id + D_OFFSET_ID);	// ID
	g_scifa_txd[ 5]	= T_LEN - 7;					// Low(Length)
	g_scifa_txd[ 6]	= 0;							// High(Length)
	g_scifa_txd[ 7]	= DYNAINST_READ_DATA;			// Instruction
	g_scifa_txd[ 8]	= adrs;							// Low(Item)
	g_scifa_txd[ 9]	= 0;							// High(Item)
	g_scifa_txd[10]	= 4;							// Low(Byte Size)
	g_scifa_txd[11]	= 0;							// High(Byte Size)
	sum.w = calc_crc16((uint8_t *)g_scifa_txd, T_LEN-2);	// Check SUM
	g_scifa_txd[T_LEN-2]	= sum.b[0];
	g_scifa_txd[T_LEN-1]	= sum.b[1];
	//
	r_scifa_read(0, T_LEN);
	r_scifa_write(0, T_LEN+2);
	r_scifa_read(1, T_LEN);
	r_scifa_write(1, T_LEN+2);
	r_scifa_write(2, T_LEN+2);
	r_scifa_write(3, T_LEN+2);
	r_scifa_write(4, T_LEN+2);
	//
	r_scifa_waiting_write_completed(0);
	r_scifa_waiting_write_completed(1);
	r_scifa_waiting_write_completed(2);
	r_scifa_waiting_write_completed(3);
	r_scifa_waiting_write_completed(4);
	//
	r_scifa_waiting_read_completed(0);
	r_scifa_waiting_read_completed(1);
#ifdef	USE_DEBUG_PRINTF_INFORMATION
	fprintf(stdout,"send:%02lX", g_scifa_rxd[0][0]);
	for(int i = 1; i < T_LEN; i++)
	{
		fprintf(stdout,",%02lX", g_scifa_rxd[0][i]);
	}
	fprintf(stdout,"\n");
#endif	// USE_DEBUG_PRINTF_INFORMATION
	//
	retval.w = 0;
	//
#ifdef CONNECT_SERVO_TO_SCIFA0
	if((0 != g_scifa_rxd[0][9]) || (0 != g_scifa_rxd[0][10]))
	{
		retval.b[0] = g_scifa_rxd[0][ 9];
		retval.b[1] = g_scifa_rxd[0][10];
	}
#endif
#ifdef CONNECT_SERVO_TO_SCIFA1
	if((0 != g_scifa_rxd[1][9]) || (0 != g_scifa_rxd[1][10]))
	{
		retval.b[0] = g_scifa_rxd[1][ 9];
		retval.b[1] = g_scifa_rxd[1][10];
	}
#endif
#ifdef CONNECT_SERVO_TO_SCIFA2
	g_scifa_rxd[2][T_LEN + 9] = g_scifa_rxd[2][T_LEN + 10] = 0;
	r_scifa_read(2, T_LEN + 15);
	if((0 != g_scifa_rxd[2][T_LEN + 9]) || (0 != g_scifa_rxd[2][T_LEN + 10]))
	{
		retval.b[0] = g_scifa_rxd[2][T_LEN +  9];
		retval.b[1] = g_scifa_rxd[2][T_LEN + 10];
	}
#endif
#ifdef CONNECT_SERVO_TO_SCIFA3
	g_scifa_rxd[3][T_LEN + 9] = g_scifa_rxd[3][T_LEN + 10] = 0;
	r_scifa_read(3, T_LEN + 15);
	if((0 != g_scifa_rxd[3][T_LEN + 9]) || (0 != g_scifa_rxd[3][T_LEN + 10]))
	{
		retval.b[0] = g_scifa_rxd[3][T_LEN +  9];
		retval.b[1] = g_scifa_rxd[3][T_LEN + 10];
	}
#endif
#ifdef CONNECT_SERVO_TO_SCIFA4
	g_scifa_rxd[4][T_LEN + 9] = g_scifa_rxd[4][T_LEN + 10] = 0;
	r_scifa_read(4, T_LEN + 15);
	if((0 != g_scifa_rxd[4][T_LEN + 9]) || (0 != g_scifa_rxd[4][T_LEN + 10]))
	{
		retval.b[0] = g_scifa_rxd[4][T_LEN +  9];
		retval.b[1] = g_scifa_rxd[4][T_LEN + 10];
	}
#endif
	r_nop();
	return retval.w;
}

/***********************************************************************************************************************
 * Function Name: r_sample_servo_get_positions
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
int		r_sample_servo_get_positions(void)
{
	int		flg_connect = 0;

	for(int i = 0; i < D_CNT_SERVO; i++)
	{
		g_angle_current[i] = r_sample_servo_read(i, 132);
		if(0 != g_angle_current[i])
		{
			flg_connect++;
		}
	}
	return flg_connect;
}

/***********************************************************************************************************************
 * Function Name: r_sample_servo_goto_home_position
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void	r_sample_servo_goto_home_position(void)
{
#ifndef	USE_DEBUG_LOOPBACK_FROM_SCIFA1_TO_SCIFA0
	if(0 < r_sample_servo_get_positions())
	{
		for(int j = 0; j < D_DIV_SLOWSETHOME; j++)
		{
			for(int i = 0; i < D_CNT_SERVO; i++)
			{
				uint32_t	tmpval;

				tmpval  = (uint32_t)g_angle_current[i] * (uint32_t)(D_DIV_SLOWSETHOME - j);
				tmpval += (uint32_t)g_angle_init[i] * j;
				tmpval /= D_DIV_SLOWSETHOME;
				g_angle_setval[i].w = (uint16_t)tmpval;
			}
			r_sample_servo_send();
			R_OS_TaskSleep(D_TIME_SLOWSETHOME);
		}
	}
#endif
	//
	//	set initialize angle of servos
	//
	r_sample_servo_set_position(0, g_angle_init[0]);
	r_sample_servo_set_position(1, g_angle_init[1]);
	r_sample_servo_set_position(2, g_angle_init[2]);
	r_sample_servo_set_position(3, g_angle_init[3]);
	r_sample_servo_set_position(4, g_angle_init[4]);
	r_sample_servo_send();
}

/***********************************************************************************************************************
 * EOF
 ***********************************************************************************************************************/
