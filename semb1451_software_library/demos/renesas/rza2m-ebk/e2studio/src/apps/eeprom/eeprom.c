/******************************************************************************
 * File Name	: eeprom.c
 * Device(s)	: RZ/A2M
 * Tool-Chain	: e2Studio Ver 7.4.0
 *				: GCC ARM Embedded 6.3.1.20170620
 * OS			: FreeRTOS
 * H/W Platform : SEMB1451/1452
 * Description	: RZ/A2M Program to control EEPROM via RIIC3
 * Operation	:
 * Limitations	:
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include	<stdio.h>
#include	<string.h>
#include	<fcntl.h>
#include	<unistd.h>

#include 	"r_typedefs.h"
#include 	"r_devlink_wrapper.h"
#include 	"r_riic_drv_sc_cfg.h"
#include 	"r_riic_drv_api.h"
#include 	"r_os_abstraction_api.h"
#include 	"r_compiler_abstraction_api.h"
#include 	"r_riic.h"

/******************************************************************************
 * Definition for Function Selection
 *****************************************************************************/

/******************************************************************************
 * Macro definitions (Register bit)
 *****************************************************************************/

/******************************************************************************
 * Macro definitions
 *****************************************************************************/

#define 	D_RIIC_DUMMY_READ_BUF_SIZE	(1)
#define 	D_RIIC_DUMMY_READ_MAX_TIME	(100)

#define	EEPROM_POWER_ON()	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pJ0[1])
#define	EEPROM_POWER_OFF()	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pJ0[0])
#define	EEPROM_ENABLE_WP()	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pC3[1])
#define	EEPROM_DISABLE_WP()	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_pC3[0])

/******************************************************************************
 * Typedef definitions
 *****************************************************************************/

/******************************************************************************
 * Global functions (Prototype definition)
 *****************************************************************************/

int_t	r_eeprom_init(void);
int_t	r_eeprom_shutdown(void);
int_t	r_eeprom_read(uint32_t addr, void *data, uint32_t size);
int_t	r_eeprom_write(uint32_t addr, const void *data, uint32_t size);

/******************************************************************************
 * Global values
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/

static int_t	r_eeprom_ackpoll(void);

/******************************************************************************
 * Local values
 *****************************************************************************/

static int_t	gs_handle_eeprom;

//
//	PJ0 : EEPROM Power Switch
//	PC3 : EEPROM Write Protect
//
static st_r_drv_gpio_pin_rw_t
	gs_pJ0[2] = {{GPIO_PORT_J_PIN_0, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
				 {GPIO_PORT_J_PIN_0, GPIO_LEVEL_HIGH, GPIO_SUCCESS}},
	gs_pC3[2] = {{GPIO_PORT_C_PIN_3, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
				 {GPIO_PORT_C_PIN_3, GPIO_LEVEL_HIGH, GPIO_SUCCESS}};

/******************************************************************************
 * Imported global functions
 *****************************************************************************/

/******************************************************************************
 * Imported global variables
 *****************************************************************************/

extern int_t	g_handle_gpio;

/******************************************************************************
 * Function Name: r_eeprom_init
 * Description	: Open RIIC driver to control EEPROM
 * Arguments	: None
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_eeprom_init(void)
{
	EEPROM_POWER_ON();
	EEPROM_ENABLE_WP();
	if(0 > (gs_handle_eeprom = open(D_EEPROM_RIIC_NAME, O_RDWR)))
	{
		return ERROR_FAILURE;
	}
	return NO_ERROR;
}
/******************************************************************************
 End of function r_eeprom_init
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_eeprom_shutdown
 * Description	: Close RIIC driver to shutdown EEPROM
 * Arguments	: None
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_eeprom_shutdown(void)
{
	if(0 > gs_handle_eeprom)
	{
		return ERROR_FAILURE;
	}
	close(gs_handle_eeprom);
	EEPROM_POWER_OFF();
	EEPROM_DISABLE_WP();
	return NO_ERROR;
}
/******************************************************************************
 * End of function r_eeprom_shutdown
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_eeprom_read
 * Description	: Read data from EEPROM memory
 * Arguments	: uint8_t addr - start address to read from EEPROM
 *				: const void *data - pointer of the buffer to store the data
 *				: size_t size - read data size
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_eeprom_read(uint32_t addr, void *data, uint32_t size)
{
	st_r_drv_riic_transfer_t	s_i2c;

	s_i2c.device_address	= D_EEPROM_DEV_ADDRESS;
	s_i2c.sub_address_type	= RIIC_SUB_ADDR_WIDTH_16_BITS;
	s_i2c.sub_address		= addr;
	s_i2c.number_of_bytes	= size;
	s_i2c.p_data_buffer		= (uint8_t *)data;
	if(DRV_ERROR == control(gs_handle_eeprom, CTL_RIIC_READ, &s_i2c))
	{
		return ERROR_FAILURE;
	}
	return NO_ERROR;
}
/******************************************************************************
 * End of function r_eeprom_read
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_eeprom_ackpoll
 * Description	: Polling ACK from EEPROM by dummy read
 * Arguments	: none
 * Return Value : int_t - error code
 *****************************************************************************/
static int_t	r_eeprom_ackpoll(void)
{
	int_t		error = ERROR_FAILURE;
	uint8_t		dummy_buf[D_RIIC_DUMMY_READ_BUF_SIZE];
	st_r_drv_riic_transfer_t s_i2c;

	s_i2c.device_address	= D_EEPROM_DEV_ADDRESS;
	s_i2c.sub_address_type	= RIIC_SUB_ADDR_WIDTH_16_BITS;
	s_i2c.sub_address		= 0;
	s_i2c.number_of_bytes	= D_RIIC_DUMMY_READ_BUF_SIZE;
	s_i2c.p_data_buffer		= dummy_buf;
	for(int_t cnt = 0; D_RIIC_DUMMY_READ_MAX_TIME < cnt; cnt++)
	{
		if(DRV_ERROR != control(gs_handle_eeprom, CTL_RIIC_READ, &s_i2c))
		{
			error = NO_ERROR;
			break;
		}
	}
	return error;
}
/******************************************************************************
 * End of function r_eeprom_ackpoll
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_eeprom_write
 * Description	: Write data to EEPROM
 * Arguments	: uint8_t addr - start address to write to EEPROM
 *				: const void *data - pointer to the buffer storing data
 *				: size_t size - write data size
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_eeprom_write(uint32_t addr, const void *data, uint32_t size)
{
	int_t		error = NO_ERROR;
	uint32_t	write_size;
	uint32_t	write_addr = addr;
	const uint8_t	*p_data;
	st_r_drv_riic_transfer_t	s_i2c;

	EEPROM_DISABLE_WP();
	p_data = (const uint8_t *)data;
	write_size = (D_EEPROM_PAGE_SIZE - (write_addr % D_EEPROM_PAGE_SIZE));
	if(write_size > size)
	{
		write_size = size;
	}
	while(0 < size)
	{
		s_i2c.device_address	= D_EEPROM_DEV_ADDRESS;
		s_i2c.sub_address_type	= RIIC_SUB_ADDR_WIDTH_16_BITS;
		s_i2c.sub_address		= write_addr;
		s_i2c.number_of_bytes	= write_size;
		s_i2c.p_data_buffer		= (uint8_t *)p_data;
		if(DRV_ERROR == control(gs_handle_eeprom, CTL_RIIC_WRITE, &s_i2c))
		{
			error = ERROR_FAILURE;
			break;
		}
		p_data		+= write_size;
		write_addr	+= write_size;
		size		-= write_size;
		if(ERROR_FAILURE == (error = r_eeprom_ackpoll()))
		{
			break;
		}
		write_size = (D_EEPROM_PAGE_SIZE < size) ? D_EEPROM_PAGE_SIZE : size;
	}
	EEPROM_ENABLE_WP();
	return error;
}
/*****************************************************************************
 * End of function r_eeprom_write
 *****************************************************************************/

/*****************************************************************************
 * EOF
 *****************************************************************************/
