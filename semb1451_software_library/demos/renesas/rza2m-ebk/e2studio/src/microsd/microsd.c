/*******************************************************************************
* File Name	   : microsd.c
* Device(s)	   : RZ/A2M
* Tool-Chain   : e2Studio Ver 7.4.0
*			   : GNU Arm Embedded Toolchain 6-2017-q2-update
* OS		   : None
* Operation	   :
* Limitations  :
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <r_fat.h>
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
#include "ff.h"
#include "r_sdif.h"
//#include "r_sdhi_drv_sc_cfg.h"
#include "r_sdhi_simplified_drv_sc_cfg.h"

/******************************************************************************
Macro definitions
******************************************************************************/

#define FAT_PRV_DEMO_CONSOLE

/* fat sample channel */
#define FAT_PRV_CH				(0)
#define FAT_PRV_CH_NUM			(FF_VOLUMES)
#define FAT_PRV_RW_PROC_NUM		(10uL)
#define FAT_PRV_100MS			(100uL)
#define FAT_PRV_300MS			(300uL)
#define FAT_PRV_500MS			(500uL)
#define FAT_PRV_KEY_INPUT_CYCLE	(FAT_PRV_100MS)
#define FAT_PRV_QUEUE_GET_CYCLE	(FAT_PRV_100MS)

#define FAT_PRV_PATH_LEN		(20)
#define FAT_PRV_QUEUE_NUM		(10uL)
#define FAT_PRV_RW_BUFF_SIZE	(512u)
#define FAT_PRV_SEM_WAITTIME	(500uL)

#define FAT_PRV_CATT_NUM_LONG	(50uL)
#define FAT_PRV_CATT_NUM_SHORT	(2uL)

/******************************************************************************
Typedef definitions
******************************************************************************/

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
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

extern int_t	g_handle_gpio;
extern st_r_drv_gpio_pin_rw_t	gs_p23[2], gs_p60[2];
extern st_r_drv_gpio_pin_rw_t	gs_p21;

/******************************************************************************
Private global variables and functions
******************************************************************************/


/* Cast to an appropriate type */

/* Terminal window escape sequences */

static TCHAR		s_fat_drive_path[FAT_PRV_PATH_LEN];
static TCHAR		s_fat_file_path[FAT_PRV_PATH_LEN];
static e_fat_mode_t	s_fat_mode[FAT_PRV_CH_NUM] = { FAT_MODE_DEFAULT };

extern void cmd_console(FILE* pIn, FILE *pOut, char_t *pszPrompt);

static e_r_drv_gpio_level_t fat_key_input(void);
static e_fat_ret_t fat_write_proc(void);
static e_fat_ret_t fat_read_proc(void);
static void fat_dummy_proc(void);
static void fat_power_on_proc(void);
static void fat_cd_insert_proc(void);
static void fat_cd_remove_proc(void);
static void fat_key_input_proc(void);

void	fat_led_off(void);
void	fat_led_on(void);
void	fat_led_error(void);
void	os_fat_key_input_task_t(void *params);
void	os_fat_main_task_t(void *params);

static const p_fatSampleFunc s_tbl_fat_proc[FAT_EVENT_NUM][FAT_MODE_NUM] =
{
	/* FAT_MODE_DEFAULT,	 FAT_MODE_CD_INSERTED, FAT_MODE_CD_REMOVED */
	{ fat_power_on_proc, fat_dummy_proc,	 fat_dummy_proc	   }, /* POWER_ON  */
	{ fat_dummy_proc,	fat_dummy_proc,		 fat_cd_insert_proc }, /* CD_INSERT */
	{ fat_dummy_proc,	fat_cd_remove_proc,	 fat_dummy_proc	   }, /* CD_REMOVE */
	{ fat_dummy_proc,	fat_key_input_proc,	 fat_dummy_proc	   }  /* KEY_INPUT */
};

uint32_t				s_fat_sem_key_input[FAT_PRV_CH_NUM]	= { 0uL };
e_fat_event_t			s_fat_isr_event[2];
p_os_msg_queue_handle_t	s_fat_queue_handle[FAT_PRV_CH_NUM];

/******************************************************************************
* Function Name: fat_key_input
* Description  : Get the key input value.
* Arguments	   : none
* Return Value : the key input value
******************************************************************************/
static e_r_drv_gpio_level_t fat_key_input(void)
{
	return 1;
}
/*******************************************************************************
 End of function fat_key_input
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_write_proc
* Description  : Fat sample write process
* Arguments	   : none
* Return Value : FAT_OK for success
******************************************************************************/
static e_fat_ret_t fat_write_proc(void)
{
	e_fat_ret_t fat_ret;
	bool_t		chk_sem;
	FRESULT		chk_fr;
	UINT		bw;

#if (FF_FS_EXFAT == 0)
	const char_t	   p_write_buff[] = "Renesas FAT sample.";
#else
	const char_t	   p_write_buff[] = "Renesas FAT/exFAT sample.";
#endif

	fat_ret = FAT_NG;
	chk_sem		   = R_OS_SemaphoreWait(&g_fat_sem_access[FAT_PRV_CH], FAT_PRV_SEM_WAITTIME);
	if (true == chk_sem)
	{
		sprintf(s_fat_file_path, "%d:\\renesas.txt", FAT_PRV_CH);
		chk_fr = f_open(&g_fil[FAT_PRV_CH], s_fat_file_path, (FA_OPEN_APPEND | FA_WRITE));
		if (FR_OK == chk_fr)
		{
			memcpy(&g_app_rw_buff[FAT_PRV_CH][0], &p_write_buff[0], sizeof(p_write_buff));
			chk_fr = f_write(&g_fil[FAT_PRV_CH]
							,&g_app_rw_buff[FAT_PRV_CH][0]
							,sizeof(p_write_buff)
							,&bw);
			if (FR_OK == chk_fr)
			{
				fat_ret = FAT_OK;
			}

			/* Cast to an appropriate type */
			(void)f_close(&g_fil[FAT_PRV_CH]);
		}
		R_OS_SemaphoreRelease(&g_fat_sem_access[FAT_PRV_CH]);
	}
	return fat_ret;
}
/*******************************************************************************
 End of function fat_write_proc
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_read_proc
* Description  : Fat sample read process
* Arguments	   : none
* Return Value : FAT_OK for success
******************************************************************************/
static e_fat_ret_t fat_read_proc(void)
{
	e_fat_ret_t fat_ret;
	bool_t			   chk_sem;
	FRESULT			   chk_fr;
	UINT			   br;

	/* uint32_t			  cnt; */

	fat_ret = FAT_NG;
	chk_sem		   = R_OS_SemaphoreWait(&g_fat_sem_access[FAT_PRV_CH], FAT_PRV_SEM_WAITTIME);
	if (true == chk_sem)
	{
		sprintf(s_fat_file_path, "%d:\\renesas.txt", FAT_PRV_CH);
		chk_fr = f_open(&g_fil[FAT_PRV_CH], s_fat_file_path, FA_READ);
		if (FR_OK == chk_fr)
		{
			while (true)
			{
				chk_fr = f_read(&g_fil[FAT_PRV_CH]
							   ,&g_app_rw_buff[FAT_PRV_CH][0]
							   ,FAT_PRV_RW_BUFF_SIZE
							   ,&br);
				if (FR_OK != chk_fr)			/* Read error */
				{
					break;
				}
				else if (0 == br)			/* eof */
				{
					fat_ret = FAT_OK;
					break;
				}
				else
				{
					/* DO NOTHING */
					;

					/* for (cnt = 0; cnt < br; cnt++) */
					/* { */
					/*	   printf("%02X ", g_app_rw_buff[FAT_PRV_CH][cnt]); */

					/*	   if (15 == (cnt % 16)) */
					/*	   { */
					/*		   printf("\r\n"); */
					/*	   } */
					/* } */
					/* printf("\r\n"); */
				}
			}

			/* Cast to an appropriate type */
			(void)f_close(&g_fil[FAT_PRV_CH]);
		}
		R_OS_SemaphoreRelease(&g_fat_sem_access[FAT_PRV_CH]);
	}
	return fat_ret;
}
/*******************************************************************************
 End of function fat_read_proc
 ******************************************************************************/

/******************************************************************************
* Function Name: os_fat_key_input_task_t
* Description  : Fat sample key input task
* Arguments	   : void *params : no used
* Return Value : none
******************************************************************************/
void	os_fat_key_input_task_t(void *params)
{
	e_r_drv_gpio_level_t key_input_row;
	e_r_drv_gpio_level_t key_input_old;
	uint32_t			 key_input_cnt;
	e_fat_event_t key_event;
	bool_t				 chk_sem;
	bool_t				 chk_queue;

	/* Cast to an appropriate type */
	UNUSED_PARAM(params);

	key_input_row = GPIO_LEVEL_SC_DEFAULT;
	key_input_old = GPIO_LEVEL_SC_DEFAULT;
	key_input_cnt = 0;

	while (true)
	{
		key_input_row = fat_key_input();
		if (key_input_row == key_input_old)
		{
			if ((GPIO_LEVEL_LOW == key_input_row)
			 && (key_input_cnt < FAT_PRV_CATT_NUM_LONG))
			{
				key_input_cnt++;
				if (key_input_cnt >= FAT_PRV_CATT_NUM_LONG)
				{
					key_input_cnt = FAT_PRV_CATT_NUM_LONG;
					chk_sem = R_OS_SemaphoreWait(&s_fat_sem_key_input[FAT_PRV_CH]
												,FAT_PRV_SEM_WAITTIME);
					if (true == chk_sem)
					{
						key_event = FAT_EVENT_KEY_INPUT; /* long press */
						chk_queue = R_OS_MessageQueuePut(s_fat_queue_handle[FAT_PRV_CH], &key_event);
						if (true != chk_queue)
						{
							fat_led_error();
						}
						R_OS_SemaphoreRelease(&s_fat_sem_key_input[FAT_PRV_CH]);
					}
				}
			}
		}
		else
		{
			if ((key_input_cnt >= FAT_PRV_CATT_NUM_SHORT)
			 && (key_input_cnt <  FAT_PRV_CATT_NUM_LONG ))
			{
				chk_sem = R_OS_SemaphoreWait(&s_fat_sem_key_input[FAT_PRV_CH], FAT_PRV_SEM_WAITTIME);
				if (true == chk_sem)
				{
					key_event = FAT_EVENT_KEY_INPUT; /* short press */
					chk_queue = R_OS_MessageQueuePut(s_fat_queue_handle[FAT_PRV_CH], &key_event);
					if (true != chk_queue)
					{
						fat_led_error();
					}
					R_OS_SemaphoreRelease(&s_fat_sem_key_input[FAT_PRV_CH]);
				}
			}
			key_input_cnt = 0;
			key_input_old = key_input_row;
		}
		R_OS_TaskSleep(FAT_PRV_KEY_INPUT_CYCLE);
	}
}
/*******************************************************************************
 End of function os_fat_key_input_task_t
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_led_off
* Description  : led off
*			   : LED1(Yellowish-green) : OFF
*			   : LED1(Red			 ) : OFF
* Arguments	   : none
* Return Value : none
******************************************************************************/
void	fat_led_off(void)
{
	/* Cast to an appropriate type */
	(void)direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p23[1]);

	/* Cast to an appropriate type */
	(void)direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p60[1]);
}
/*******************************************************************************
 End of function fat_led_off
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_led_on
* Description  : led on(normal)
*			   : LED1(Yellowish-green) : ON
*			   : LED1(Red			 ) : OFF
* Arguments	   : none
* Return Value : none
******************************************************************************/
void	fat_led_on(void)
{
	/* Cast to an appropriate type */
	(void)direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p23[0]);

	/* Cast to an appropriate type */
	(void)direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p60[1]);
}
/*******************************************************************************
 End of function fat_led_on
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_led_error
* Description  : led on(error)
*			   : LED1(Yellowish-green) : OFF
*			   : LED1(Red			 ) : ON
* Arguments	   : none
* Return Value : none
******************************************************************************/
void	fat_led_error(void)
{
	/* Cast to an appropriate type */
	(void)direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p23[1]);

	/* Cast to an appropriate type */
	(void)direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p60[0]);
}
/*******************************************************************************
 End of function fat_led_error
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_cd_int_cb_function
* Description  : Callback interrupt function for card detection
* Arguments	   : int32_t sd_port : channel no (0 or 1)
*			   : int32_t cd		 : card detect information
* Return Value : success : SD_OK
*			   : fail	 : SD_ERR
******************************************************************************/
int32_t fat_cd_int_cb_function(int32_t sd_port, int32_t cd)
{
	bool_t	chk_queue;

	if (0 != cd)
	{
		s_fat_isr_event[0] = FAT_EVENT_CD_INSERT;
		if (FAT_PRV_CH == sd_port)
		{
			chk_queue = R_OS_MessageQueuePut(s_fat_queue_handle[sd_port], &s_fat_isr_event[0]);
			if (false == chk_queue)
			{
				/* DO NOTHING */
				;
			}
		}
	}
	else
	{
		s_fat_isr_event[1] = FAT_EVENT_CD_REMOVE;
		if (FAT_PRV_CH == sd_port)
		{
			chk_queue = R_OS_MessageQueuePut(s_fat_queue_handle[sd_port], &s_fat_isr_event[1]);
			if (false == chk_queue)
			{
				/* DO NOTHING */
				;
			}
		}
	}
	return SD_OK;
}
/*******************************************************************************
 End of function fat_cd_int_cb_function
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_dummy_proc
* Description  : Fat sample dummy process
* Arguments	   : none
* Return Value : none
******************************************************************************/
static void fat_dummy_proc(void)
{
	fat_led_error();
}
/*******************************************************************************
 End of function fat_dummy_proc
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_power_on_proc
* Description  : Fat sample power on process
* Arguments	   : none
* Return Value : none
******************************************************************************/
static void fat_power_on_proc(void)
{
	FRESULT chk_fr;

	/*--- initialize ---*/
	memset(&g_fatfs[FAT_PRV_CH], 0, sizeof(FATFS));

	fat_led_off();

	/*--- mount ---*/
	sprintf(s_fat_drive_path, "%d:", FAT_PRV_CH);
	chk_fr = f_mount(&g_fatfs[FAT_PRV_CH], s_fat_drive_path, 1);
	if (FR_OK == chk_fr)
	{
		/* update fat sample mode */
		s_fat_mode[FAT_PRV_CH] = FAT_MODE_CD_INSERTED;
	}
	else
	{
		/* FR_NO_FILESYSTEM is not care */
		/* update fat sample mode */
		s_fat_mode[FAT_PRV_CH] = FAT_MODE_CD_REMOVED;
		fat_led_error();
	}
}
/*******************************************************************************
 End of function fat_power_on_proc
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_cd_insert_proc
* Description  : Fat sample card detection is inserted process
* Arguments	   : none
* Return Value : none
******************************************************************************/
static void fat_cd_insert_proc(void)
{
	FRESULT chk_fr;

	/*--- initialize ---*/
	memset(&g_fatfs[FAT_PRV_CH], 0, sizeof(FATFS));

	fat_led_off();

	/*--- mount ---*/
	sprintf(s_fat_drive_path, "%d:", FAT_PRV_CH);
	chk_fr = f_mount(&g_fatfs[FAT_PRV_CH], s_fat_drive_path, 1);
	if (FR_OK != chk_fr)
	{
		fat_led_error();
	}

	/* update fat sample mode */
	s_fat_mode[FAT_PRV_CH] = FAT_MODE_CD_INSERTED;
}
/*******************************************************************************
 End of function fat_cd_insert_proc
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_cd_remove_proc
* Description  : Fat sample card detection is removed process
* Arguments	   : none
* Return Value : none
******************************************************************************/
static void fat_cd_remove_proc(void)
{
	FRESULT chk_fr;

	/*--- initialize ---*/
	memset(&g_fatfs[FAT_PRV_CH], 0, sizeof(FATFS));

	/* fat_led_off(); */

	/*--- unmount ---*/
	sprintf(s_fat_drive_path, "%d:", FAT_PRV_CH);
	chk_fr = f_unmount(s_fat_drive_path);
	if (FR_OK != chk_fr)
	{
		fat_led_error();
	}

	/* update fat sample mode */
	s_fat_mode[FAT_PRV_CH] = FAT_MODE_CD_REMOVED;
}
/*******************************************************************************
 End of function fat_cd_remove_proc
 ******************************************************************************/

/******************************************************************************
* Function Name: fat_key_input_proc
* Description  : Fat sample key input process
* Arguments	   : none
* Return Value : none
******************************************************************************/
static void fat_key_input_proc(void)
{
	e_fat_ret_t chk_proc;
	uint32_t		   cnt;
	bool_t			   chk_sem;

	/* bool_t			  chk_queue; */

	chk_proc = FAT_NG;
	chk_sem	 = R_OS_SemaphoreWait(&s_fat_sem_key_input[FAT_PRV_CH], FAT_PRV_SEM_WAITTIME);
	if (true == chk_sem)
	{
		for (cnt = 0; cnt < FAT_PRV_RW_PROC_NUM; cnt++)
		{
			/*--- 1. Writing a string to the file. ---*/
			fat_led_on();
			chk_proc = fat_write_proc();
			if (FAT_OK != chk_proc)
			{
				break;
			}

			/*--- 2. Reading a string from the file. ---*/
			chk_proc = fat_read_proc();
			if (FAT_OK != chk_proc)
			{
				break;
			}
			R_OS_TaskSleep(FAT_PRV_300MS);

			/*--- 3. No access to the file. ---*/
			fat_led_off();
			R_OS_TaskSleep(FAT_PRV_500MS);
		}

		if (FAT_OK != chk_proc)
		{
			fat_led_error();

			/* clear a message queue */
			/* chk_queue = R_OS_MessageQueueClear(&s_fat_queue_handle[FAT_PRV_CH]); */
			/* if (true == chk_queue) */
			/* { */
			/*	   printf("queue clear ok\r\n"); */
			/* } */
			/* else */
			/* { */
			/*	   printf("queue clear ng\r\n"); */
			/* } */
		}
		R_OS_SemaphoreRelease(&s_fat_sem_key_input[FAT_PRV_CH]);
	}
}
/*******************************************************************************
 End of function fat_key_input_proc
 ******************************************************************************/

/******************************************************************************
* Function Name: os_fat_main_task_t
* Description  : Fat sample main task
* Arguments	   : void *params : no used
* Return Value : none
******************************************************************************/
void	os_fat_main_task_t(void *params)
{
	bool_t				 chk_queue;
	e_fat_event_t *p_receive_event;

	/* Cast to an appropriate type */
	UNUSED_PARAM(params);

	while (true)
	{
		/* Cast to an appropriate type */
		chk_queue = R_OS_MessageQueueGet(s_fat_queue_handle[FAT_PRV_CH], (p_os_msg_t *)&p_receive_event,
										FAT_PRV_QUEUE_GET_CYCLE, false);
		if (true == chk_queue)
		{
			if (((*p_receive_event)					  < FAT_EVENT_NUM)
			 && (s_fat_mode[FAT_PRV_CH] < FAT_MODE_NUM ))
			{
				/* Cast to an appropriate type */
				if (NULL != s_tbl_fat_proc[*p_receive_event][s_fat_mode[FAT_PRV_CH]])
				{
					(*s_tbl_fat_proc[*p_receive_event][s_fat_mode[FAT_PRV_CH]])();
				}
			}
			else
			{
				fat_led_error();
			}
		}
	}
}
/*******************************************************************************
 End of function os_fat_main_task_t
 ******************************************************************************/

/* End of File */
