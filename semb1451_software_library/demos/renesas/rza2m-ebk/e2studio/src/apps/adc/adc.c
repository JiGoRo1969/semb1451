/******************************************************************************
 * File Name	 : adc.c
 * Version       : 1.0.0
 * Creation Date : 2019-11-01
 * Device(s)	 : RZ/A2M
 * Tool-Chain	 : e2Studio Ver 7.4.0
 *				 : GCC ARM Embedded 6.3.1.20170620
 * OS			 : FreeRTOS
 * H/W Platform  : SEMB1451/1452
 * Description	 : RZ/A2M Program to control adc functions
 * Operation	 :
 * Limitations	 :
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include	"r_typedefs.h"
#include	"iodefine.h"
#include	"r_os_abstraction_api.h"
#include	"r_adc_drv_api.h"
#include	"r_gpio_drv_api.h"
#include	"r_task_priority.h"

/******************************************************************************
 * Definition for Function Selection
 *****************************************************************************/

#define 	USE_VOLTAGE_MONITOR
//#define 	USE_JOYSTICK
#define 	USE_DISTANCE

//#define 	USE_DEBUG_PRINTF_INFORMATION

/******************************************************************************
 * Macro definitions (Register bit)
 *****************************************************************************/

/******************************************************************************
 * Macro definitions
 *****************************************************************************/

#define	D_RESOLUTION_ADC	(1 << 12)
#define	D_HALF_RESO_ADC		(D_RESOLUTION_ADC / 2)

#define D_VCC_A				(3.3f)
#define D_BAT_R_UP			(1000.0f)		// 1000k Ohm = R156
#define D_BAT_R_DOWN		(62.0f)			//   62k Ohm = R158
#define D_BAT_R_RATIO		((D_BAT_R_UP + D_BAT_R_DOWN) / D_BAT_R_DOWN)
#define D_ADC2BAT			(D_BAT_R_RATIO * D_VCC_A / D_RESOLUTION_ADC)

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

void		r_adc_init(void);

/******************************************************************************
 * Global values
 *****************************************************************************/

int_t		g_handle_adc = 0;

volatile uint16_t	g_adc_val[8];
volatile double		g_distance;
volatile double		g_battery;

/******************************************************************************
 * Local functions
 *****************************************************************************/

/******************************************************************************
 * Local values
 *****************************************************************************/

static st_r_drv_gpio_pin_rw_t
	gs_pH2[2] = {{GPIO_PORT_H_PIN_2, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
				 {GPIO_PORT_H_PIN_2, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
	gs_pB1[2] = {{GPIO_PORT_B_PIN_1, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
				 {GPIO_PORT_B_PIN_1, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
	gs_pB2[2] = {{GPIO_PORT_B_PIN_2, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
				 {GPIO_PORT_B_PIN_2, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
	gs_pB3[2] = {{GPIO_PORT_B_PIN_3, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
				 {GPIO_PORT_B_PIN_3, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
	gs_pB4[2] = {{GPIO_PORT_B_PIN_4, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
				 {GPIO_PORT_B_PIN_4, GPIO_LEVEL_HIGH, GPIO_SUCCESS}};

/******************************************************************************
 * Imported global functions
 *****************************************************************************/

/******************************************************************************
 * Imported global variables
 *****************************************************************************/

extern	int_t	g_handle_gpio;

/******************************************************************************
 * Function Name: os_adc_task_t
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_adc_gets(void)
{
	//
	//	software trigger for A/D converter
	//
	control(g_handle_adc, CTL_ADC_SOFTWARE_TRIGGER, NULL);
	//
	//	read A/D values
	//
#if defined(USE_VOLTAGE_MONITOR) && defined(USE_DISTANCE)
	read(g_handle_adc, (uint_t *)&g_adc_val[0], 4);
#elif defined(USE_VOLTAGE_MONITOR)
	read(g_handle_adc, &g_adc_val[0], 2);
#elif defined(USE_DISTANCE)
	read(g_handle_adc, &g_adc_val[1], 2);
#endif
	//
	//	translation from A/D values to physical values
	//
#ifdef	USE_VOLTAGE_MONITOR
	g_battery = D_ADC2BAT * (double)g_adc_val[0];
#ifdef	USE_DEBUG_PRINTF_INFORMATION
	fprintf(stdout, "BM(%5.2lf[V](%4d)),", g_battery, g_adc_val[0]);
#endif	// USE_DEBUG_PRINTF_INFORMATION
#endif	// USE_VOLTAGE_MONITOR
#ifdef	USE_DISTANCE
	g_distance = -0.238f * (double)g_adc_val[1] + 689.060f;
#ifdef	USE_DEBUG_PRINTF_INFORMATION
	fprintf(stdout, "DI(%5.2lf[mm](%4d)),", g_distance, g_adc_val[1]);
#endif	// USE_DEBUG_PRINTF_INFORMATION
#endif	// USE_DISTANCE

#ifdef	USE_DEBUG_PRINTF_INFORMATION
	fprintf(stdout, "\n");
#endif	// USE_DEBUG_PRINTF_INFORMATION
}

/******************************************************************************
 * Function Name: r_adc_init
 * Description  : This is an initialization for ADC
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void	r_adc_init(void)
{
    static st_r_drv_adc_config_t	adc_config;

	if(0 > (g_handle_adc = open(DEVICE_INDENTIFIER "adc0", O_RDWR)))
	{
		g_distance		= 0.0f;
		g_battery		= 0.0f;
		g_handle_adc	= -1;
		return;
	}
	adc_config.config.alignment				= ADC_ALIGN_RIGHT;
	adc_config.config.interrupt_priority	= INTC_PRIORITY_20;
	adc_config.config.resolution			= ADC_RESOLUTION_12;
	adc_config.config.scan_mode				= ADC_SCAN_MODE_SINGLE;
	adc_config.config.p_adcCallback			= NULL;
	//
	adc_config.config.channels[ADC_CHANNEL_AN000].sample_time = 0;
	adc_config.config.channels[ADC_CHANNEL_AN001].sample_time = 0;
#ifdef	USE_VOLTAGE_MONITOR
	adc_config.config.channels[ADC_CHANNEL_AN002].sample_time = 50;
#else	// !USE_VOLTAGE_MONITOR
	adc_config.config.channels[ADC_CHANNEL_AN002].sample_time = 0;
#endif	// USE_VOLTAGE_MONITOR
	adc_config.config.channels[ADC_CHANNEL_AN003].sample_time = 0;
#ifdef	USE_JOYSTICK
	adc_config.config.channels[ADC_CHANNEL_AN004].sample_time = 50;
	adc_config.config.channels[ADC_CHANNEL_AN005].sample_time = 50;
#else	// !USE_JOYSTICK
	adc_config.config.channels[ADC_CHANNEL_AN004].sample_time = 0;
	adc_config.config.channels[ADC_CHANNEL_AN005].sample_time = 0;
#endif	// USE_JOYSTICK
#ifdef	USE_DISTANCE
	adc_config.config.channels[ADC_CHANNEL_AN006].sample_time = 50;
#else	// !USE_DISTANCE
	adc_config.config.channels[ADC_CHANNEL_AN006].sample_time = 0;
#endif	// USE_DISTANCE
	adc_config.config.channels[ADC_CHANNEL_AN007].sample_time = 0;
	adc_config.config.channels[ADC_CHANNEL_AN000].trigger_source = ADC_TRIGGER_DISABLED;
	adc_config.config.channels[ADC_CHANNEL_AN001].trigger_source = ADC_TRIGGER_DISABLED;
#ifdef	USE_VOLTAGE_MONITOR
	adc_config.config.channels[ADC_CHANNEL_AN002].trigger_source = ADC_TRIGGER_NORMAL;
#else	// !USE_VOLTAGE_MONITOR
	adc_config.config.channels[ADC_CHANNEL_AN002].trigger_source = ADC_TRIGGER_DISABLED;
#endif	// USE_VOLTAGE_MONITOR
	adc_config.config.channels[ADC_CHANNEL_AN003].trigger_source = ADC_TRIGGER_DISABLED;
#ifdef	USE_JOYSTICK
	adc_config.config.channels[ADC_CHANNEL_AN004].trigger_source = ADC_TRIGGER_NORMAL;
	adc_config.config.channels[ADC_CHANNEL_AN005].trigger_source = ADC_TRIGGER_NORMAL;
#else	// !USE_JOYSTICK
	adc_config.config.channels[ADC_CHANNEL_AN004].trigger_source = ADC_TRIGGER_DISABLED;
	adc_config.config.channels[ADC_CHANNEL_AN005].trigger_source = ADC_TRIGGER_DISABLED;
#endif	// USE_JOYSTICK
#ifdef	USE_DISTANCE
	adc_config.config.channels[ADC_CHANNEL_AN006].trigger_source = ADC_TRIGGER_NORMAL;
#else	// !USE_DISTANCE
	adc_config.config.channels[ADC_CHANNEL_AN006].trigger_source = ADC_TRIGGER_DISABLED;
#endif	// USE_DISTANCE
	adc_config.config.channels[ADC_CHANNEL_AN007].trigger_source = ADC_TRIGGER_DISABLED;
	//
	if(DRV_SUCCESS != control(g_handle_adc, CTL_ADC_SET_CONFIGURATION, &adc_config))
	{
		g_distance		= 0.0f;
		g_battery		= 0.0f;
		g_handle_adc	= -1;
		return;
	}
	//
	// ADC2 Battery Monitoring Selector
	//
#ifdef	USE_VOLTAGE_MONITOR
	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pH2[1]);
#else	// !USE_VOLTAGE_MONITOR
	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pH2[0]);
#endif	// USE_VOLTAGE_MONITOR
	//
	// ADC4 SEL(CN22-#4)
	//
#ifdef	USE_JOYSTICK
	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pB1[1]);
#else	// !USE_JOYSTICK
	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pB1[0]);
#endif	// USE_JOYSTICK
	//
	// ADC5 SEL(CN23-#4)
	//
#ifdef	USE_JOYSTICK
	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pB2[1]);
#else	// !USE_JOYSTICK
	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pB2[0]);
#endif	// USE_JOYSTICK
	//
	// ADC6 SEL(CN24-#4)
	//
#ifdef	USE_DISTANCE
	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pB3[1]);
#else	// !USE_DISTANCE
	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pB3[0]);
#endif	// USE_DISTANCE
	//
	// ADC7 SEL(CN25-#4)
	//
	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pB4[0]);
}

/*****************************************************************************
 * EOF
 *****************************************************************************/
