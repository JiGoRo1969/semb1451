/**********************************************************************************************************************
 * File Name	 : main.c
 * Version		 : 1.0.0
 * Creation Date : 2019-11-01
 * Device(s)	 : RZ/A2M
 * Tool-Chain	 : e2Studio Ver 7.4.0
 *				 : GCC ARM Embedded 6.3.1.20170620
 * OS			 : None
 * H/W Platform  : SEMB1451/1452
 * Description	 : RZ/A2M Sample Program - Main
 * Operation	 :
 * Limitations	 :
 *********************************************************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <r_fat.h>
#include <unistd.h>

#include "r_typedefs.h"
#include "iodefine.h"
#include "compiler_settings.h"
#include "main.h"
#include "r_cpg_drv_api.h"
#include "r_ostm_drv_api.h"
#include "r_scifa_drv_api.h"
#include "r_gpio_drv_api.h"
#include "r_startup_config.h"
#include "r_os_abstraction_api.h"
#include "r_task_priority.h"
#include "r_riic.h"
#include "r_rspi.h"
#include "scifa.h"
#include "ff.h"
#include "r_sdif.h"
#include "r_sdhi_simplified_drv_sc_cfg.h"
#include "command.h"


/******************************************************************************
 * Definition for Function Selection
 *****************************************************************************/

/******************************************************************************
 * Macro definitions (Register bit)
 *****************************************************************************/

/******************************************************************************
 * Macro definitions
 *****************************************************************************/

#define		D_MAX_CNT_TIMEOUT				(1000000)

/* fat sample channel */
#define 	FAT_PRV_CH				(0)
#define 	FAT_PRV_CH_NUM			(FF_VOLUMES)
#define 	FAT_PRV_RW_PROC_NUM		(10uL)
#define 	FAT_PRV_100MS			(100uL)
#define 	FAT_PRV_300MS			(300uL)
#define 	FAT_PRV_500MS			(500uL)
#define 	FAT_PRV_KEY_INPUT_CYCLE	(FAT_PRV_100MS)
#define 	FAT_PRV_QUEUE_GET_CYCLE	(FAT_PRV_100MS)

#define 	FAT_PRV_PATH_LEN			(20)
#define 	FAT_PRV_QUEUE_NUM		(10uL)
#define 	FAT_PRV_RW_BUFF_SIZE		(512u)
#define 	FAT_PRV_SEM_WAITTIME		(500uL)

#define 	FAT_PRV_CATT_NUM_LONG	(50uL)
#define 	FAT_PRV_CATT_NUM_SHORT	(2uL)

/******************************************************************************
 * Typedef definitions
 *****************************************************************************/

typedef void (*p_fatSampleFunc)(void);

typedef enum
{
	FAT_EVENT_POWER_ON,
	FAT_EVENT_CD_INSERT,
	FAT_EVENT_CD_REMOVE,
	FAT_EVENT_KEY_INPUT,
	FAT_EVENT_NUM
} e_fat_event_t;

typedef enum
{
	FAT_MODE_DEFAULT,
	FAT_MODE_CD_INSERTED,
	FAT_MODE_CD_REMOVED,
	FAT_MODE_NUM
} e_fat_mode_t;

typedef enum
{
	FAT_OK,
	FAT_NG
} e_fat_ret_t;

/******************************************************************************
 * Global functions (Prototype definition)
 *****************************************************************************/

int_t	os_console_task_t(void);
int_t	os_servo_task_t(void);
int_t	os_spi_task_t(void);
int_t	os_adc_task_t(void);
int_t	os_main_task_t(void);
int_t	main(void);

/******************************************************************************
 * Global values
 *****************************************************************************/

int_t	g_handle_gpio;

st_r_drv_gpio_pin_rw_t	gs_p76[2] = {{GPIO_PORT_7_PIN_6, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
									 {GPIO_PORT_7_PIN_6, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
						gs_pA6[2] = {{GPIO_PORT_A_PIN_6, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
									 {GPIO_PORT_A_PIN_6, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
						gs_p23[2] = {{GPIO_PORT_2_PIN_3, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
									 {GPIO_PORT_2_PIN_3, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
						gs_p60[2] = {{GPIO_PORT_6_PIN_0, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
									 {GPIO_PORT_6_PIN_0, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
						gs_p21	   = { GPIO_PORT_2_PIN_1, GPIO_LEVEL_SC_DEFAULT, GPIO_SUCCESS };

/******************************************************************************
 * Local functions
 *****************************************************************************/

/******************************************************************************
 * Local values
 *****************************************************************************/

/******************************************************************************
 * Imported global functions
 *****************************************************************************/

//extern	void	r_scifa_init(void);
extern	void	r_sample_servo_init(void);
extern	void	r_sample_servo_goto_home_position(void);
extern	void	r_adc_gets(void);
extern	void	r_adc_init(void);
extern	void	os_fat_key_input_task_t(void *params);
extern	void	os_fat_main_task_t(void *params);
extern	void	fat_led_off(void);
extern	void	fat_led_on(void);
extern	void	fat_led_error(void);

/******************************************************************************
 * Imported global variables
 *****************************************************************************/

//extern e_fat_event_t	s_fat_isr_event[];
extern p_os_msg_queue_handle_t	s_fat_queue_handle[];
extern uint32_t					s_fat_sem_key_input[];

/******************************************************************************
* Function Name: os_console_task_t
* Description  : console task
* Arguments	   : none
* Return Value : 0
******************************************************************************/
int_t	os_console_task_t(void)
{
	//
	//	initialization for each devices
	//
	r_eeprom_init();
	r_bmx055_init();
	//
	//
	//
	printf("\r\nSEMB-1451/1452 software library - simple console\r\n\r\n");
	while(1)
	{
		/* ==== Receive command, activate sample software ==== */
		cmd_console(stdin, stdout, "$ ");
		R_OS_TaskSleep(500);
	}
	return 0;
}
/*******************************************************************************
 * End of function os_console_task_t
 ******************************************************************************/

/******************************************************************************
* Function Name: os_servo_task_t
* Description  : servo control task
* Arguments    : none
* Return Value : 0
******************************************************************************/
int_t	os_servo_task_t(void)
{
    //
    // Initialization for ServoMotor
    //
	r_scifa_init();
	R_OS_TaskSleep(500);
    r_sample_servo_init();
	R_OS_TaskSleep(500);
    r_sample_servo_goto_home_position();
    //
	while(1)
	{
        R_OS_TaskSleep(1000);
	}
	return 0;
}
/*******************************************************************************
 * End of function os_servo_task_t
 ******************************************************************************/

/******************************************************************************
* Function Name: os_spi_task_t
* Description  : SPI control task
* Arguments    : none
* Return Value : 0
******************************************************************************/
int_t	os_spi_task_t(void)
{
    //
    // Initialization for RSPI I/F
    //
	r_rspi0_create();
	r_rspi1_create();
    //
	r_rspi0_start();
	r_rspi1_start();
	//
	while(1)
	{
		static uint8_t	gs_spi0_txbuf[BUF_SIZE];
		static uint8_t	gs_spi0_rxbuf[BUF_SIZE];
		static uint8_t	gs_spi1_txbuf[BUF_SIZE];
		static uint8_t	gs_spi1_rxbuf[BUF_SIZE];
		static uint16_t	cnt = 0;

		sprintf(gs_spi0_txbuf, "RSPI0 : test messages %5d\n", cnt);
#ifndef	USE_RSPI_SSL0
		PORTG.PODR.BIT.PODR3	= D_RSPI_SSL_ACTIVE;
#endif	// !USE_RSPI_SSL0
		r_rspi0_send_receive(gs_spi0_txbuf, gs_spi0_rxbuf, BUF_SIZE);
		for(uint32_t t_cnt_timeout = 0; t_cnt_timeout < D_MAX_CNT_TIMEOUT; t_cnt_timeout++)
		{
			if(0 != r_rspi0_completed_translation())
			{
				break;
			}
			r_nop();
		}
#ifndef	USE_RSPI_SSL0
		PORTG.PODR.BIT.PODR3	= D_RSPI_SSL_INACTIVE;
#endif	// !USE_RSPI_SSL0
		sprintf(gs_spi1_txbuf, "RSPI1 : test messages %5d\n", cnt);
#ifndef	USE_RSPI_SSL1
		PORTG.PODR.BIT.PODR7	= D_RSPI_SSL_ACTIVE;
#endif	// !USE_RSPI_SSL1
		r_rspi1_send_receive(gs_spi1_txbuf, gs_spi1_rxbuf, BUF_SIZE);
		for(uint32_t t_cnt_timeout = 0; t_cnt_timeout < D_MAX_CNT_TIMEOUT; t_cnt_timeout++)
		{
			if(0 != r_rspi1_completed_translation())
			{
				break;
			}
			r_nop();
		}
#ifndef	USE_RSPI_SSL1
		PORTG.PODR.BIT.PODR7	= D_RSPI_SSL_INACTIVE;
#endif	// !USE_RSPI_SSL1
		cnt++;
        R_OS_TaskSleep(1000);
	}
	return 0;
}
/*******************************************************************************
 * End of function os_spi_task_t
 ******************************************************************************/

/******************************************************************************
* Function Name: os_adc_task_t
* Description  : ADC control task
* Arguments    : none
* Return Value : 0
******************************************************************************/
int_t	os_adc_task_t(void)
{
    //
    // Initialization for ADC I/F
    //
	r_adc_init();
    //
	while(1)
	{
        R_OS_TaskSleep(100);
		r_adc_gets();
	}
	return 0;
}
/*******************************************************************************
 * End of function os_adc_task_t
 ******************************************************************************/

/**********************************************************************************************************************
 * Function Name: os_main_task_t
 * Description  : FreeRTOS main task called by R_OS_KernelInit()
 *              : FreeRTOS is now configured and R_OS_Abstraction calls
 *              : can be used.
 *              : From this point forward no longer use direct_xxx calls.
 *              : For example
 *              : in place of   direct_open("ostm2", O_RDWR);
 *              : use           open(DEVICE_INDENTIFIER "ostm2", O_RDWR);
 *              :
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/
int_t	os_main_task_t(void)
{
	static e_fat_event_t		event;
	static bool_t					chk_queue, chk_sem;
	static uint32_t		gs_led_pattern;      /* LED lighting/turning off */

	if(0 > (g_handle_gpio = open(DEVICE_INDENTIFIER "gpio", O_RDWR)))
	{
		while (1)
		{
			R_COMPILER_Nop();
		}
	}
	//
	//	Create a task to run the console
	//
	R_OS_TaskCreate("CONSOLE", (os_task_code_t)os_console_task_t, NULL,
		R_OS_ABSTRACTION_DEFAULT_STACK_SIZE, TASK_CONSOLE_TASK_PRI);
    //
	//	Create a task to control the servos
	//
	R_OS_TaskCreate("SERVO", (os_task_code_t)os_servo_task_t, NULL,
		R_OS_ABSTRACTION_DEFAULT_STACK_SIZE, TASK_SERVO_TASK_PRI);
    //
	//	Create a task to control the spi interfaces
	//
	R_OS_TaskCreate("SPI", (os_task_code_t)os_spi_task_t, NULL,
		R_OS_ABSTRACTION_DEFAULT_STACK_SIZE, TASK_SPI_TASK_PRI);
    //
	//	Create a task to control the adc interfaces
	//
	R_OS_TaskCreate("ADC", (os_task_code_t)os_adc_task_t, NULL,
		R_OS_ABSTRACTION_DEFAULT_STACK_SIZE, TASK_ADC_TASK_PRI);
	//
	//
	//
	/* --- Create message queue start --- */
	chk_queue = R_OS_MessageQueueCreate(&s_fat_queue_handle[FAT_PRV_CH], FAT_PRV_QUEUE_NUM);
	if(true == chk_queue)
	{
		event = FAT_EVENT_POWER_ON;
		chk_queue = R_OS_MessageQueuePut(s_fat_queue_handle[FAT_PRV_CH], &event);
	}
	if (false == chk_queue)
	{
		fat_led_error();
		while(true)
		{
		}
	}
	/* --- Create message queue end --- */
	/* --- Create semaphore start --- */
	if(0uL == g_fat_sem_access[FAT_PRV_CH])
	{
		chk_sem = R_OS_SemaphoreCreate(&g_fat_sem_access[FAT_PRV_CH], 1uL);
		if(false == chk_sem)
		{
			fat_led_error();
			while(true)
			{
			}
		}
	}
	if(0uL == s_fat_sem_key_input[FAT_PRV_CH])
	{
		chk_sem = R_OS_SemaphoreCreate(&s_fat_sem_key_input[FAT_PRV_CH], 1uL);
		if(false == chk_sem)
		{
			fat_led_error();
			while(true)
			{
			}
		}
	}
	/* --- Create semaphore end --- */
	/* --- Create a task to run the fat sample --- */
	/* Cast to an appropriate type */
	R_OS_TaskCreate("Fat sample key input", os_fat_key_input_task_t, NULL,
			R_OS_ABSTRACTION_DEFAULT_STACK_SIZE, TASK_FAT_KEY_INPUT_TASK_PRI);
	/* Cast to an appropriate type */
	R_OS_TaskCreate("Fat sample main", os_fat_main_task_t, NULL,
			R_OS_ABSTRACTION_DEFAULT_STACK_SIZE, TASK_FAT_MAIN_TASK_PRI);
	//
	//	Start blinking LEDs as heart-beat
	//
	gs_led_pattern = 0;
	while (1)
	{
		gs_led_pattern ^= 1;
		if( 0 == gs_led_pattern )
		{
			direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p76[0]);
			direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pA6[1]);
		}
		else
		{
			direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p76[1]);
			direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pA6[0]);
		}
		if(1)
		{
			direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p23[1]);
		}
		else
		{
			direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p23[0]);
		}
		if(1)
		{
			direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p60[1]);
		}
		else
		{
			direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p60[0]);
		}
		R_OS_TaskSleep(500);
	}
	return 0;
}
/**********************************************************************************************************************
 * End of function os_main_task_t
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: main
 * Description  : C Entry point
 *              : opens and configures cpg driver
 *              : starts the freertos kernel
 * Arguments    : none
 * Return Value : 0
 *********************************************************************************************************************/
int_t	main(void)
{
    int_t cpg_handle;

    /* configure any drivers that are required before the Kernel initializes */

    /* Initialize the devlink layer */
    R_DEVLINK_Init();

    /* Initialize CPG */
    cpg_handle = direct_open("cpg", 0);

    if (cpg_handle < 0)
    {
        /* stop execute */
        while (1)
        {
            R_COMPILER_Nop();
        }
    }

    /* Can close handle if no need to change clock after here */
    direct_close(cpg_handle);

    /* Start FreeRTOS */
    /* R_OS_InitKernel should never return */
    R_OS_KernelInit();

    return (0);
}
/**********************************************************************************************************************
 * End of function main
 *********************************************************************************************************************/

/*****************************************************************************
 * EOF
 *****************************************************************************/
