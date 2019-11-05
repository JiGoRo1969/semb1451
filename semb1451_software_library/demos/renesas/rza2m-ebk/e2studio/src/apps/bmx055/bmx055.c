/******************************************************************************
 * File Name	 : bmx055.c
 * Version       : 1.0.0
 * Creation Date : 2019-11-01
 * Device(s)	 : RZ/A2M
 * Tool-Chain	 : e2Studio Ver 7.4.0
 *				 : GCC ARM Embedded 6.3.1.20170620
 * OS			 : FreeRTOS
 * H/W Platform  : SEMB1451/1452
 * Description	 : RZ/A2M Program to control BMX055 via RIIC0
 * Operation	 :
 * Limitations	 :
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include	"r_typedefs.h"
#include	"r_devlink_wrapper.h"
#include	"r_riic_drv_sc_cfg.h"
#include	"r_riic_drv_api.h"
#include	"r_os_abstraction_api.h"
#include	"r_compiler_abstraction_api.h"
#include	"r_riic.h"

/******************************************************************************
 * Definition for Function Selection
 *****************************************************************************/

/******************************************************************************
 * Macro definitions (Register bit)
 *****************************************************************************/

/******************************************************************************
 * Macro definitions
 *****************************************************************************/

#define	BMX055_POWER_ON()	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p20[0])
#define	BMX055_POWER_OFF()	direct_control(g_handle_gpio, CTL_GPIO_PIN_WRITE, &gs_p20[1])

/******************************************************************************
 * Typedef definitions
 *****************************************************************************/

/******************************************************************************
 * Global functions (Prototype definition)
 *****************************************************************************/

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

static int_t	gs_bmx055_handle;
//
//	P20 : BMX055 Power Switch
//
static st_r_drv_gpio_pin_rw_t
	gs_p20[2] = {{GPIO_PORT_2_PIN_0, GPIO_LEVEL_LOW,  GPIO_SUCCESS},
				 {GPIO_PORT_2_PIN_0, GPIO_LEVEL_HIGH, GPIO_SUCCESS}};

/******************************************************************************
 * Imported global functions
 *****************************************************************************/

/******************************************************************************
 * Imported global variables
 *****************************************************************************/

extern int_t	g_handle_gpio;

/******************************************************************************
 * Function Name: r_bmx055_init
 * Description	: Open RIIC driver to control BMX055
 * Arguments	: None
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_bmx055_init(void)
{
	BMX055_POWER_ON();
	if(0 > (gs_bmx055_handle = open(D_BMX055_RIIC_NAME, O_RDWR)))
	{
		return ERROR_FAILURE;
	}
	return NO_ERROR;
}
/******************************************************************************
 * End of function r_bmx055_init
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_bmx055_shutdown
 * Description	: Close RIIC driver to shutdown BMX055
 * Arguments	: None
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_bmx055_shutdown(void)
{
	if(0 > gs_bmx055_handle)
	{
		return ERROR_FAILURE;
	}
	close(gs_bmx055_handle);
	BMX055_POWER_OFF();
	return NO_ERROR;
}
/******************************************************************************
 * End of function r_bmx055_shutdown
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_bmx055_acc_read
 * Description	: Read data from BMX055 accelerometer register
 * Arguments	: uint8_t addr - start address of BMX055 register
 *				: const void *data - pointer of the buffer to store the data
 *				: size_t size - read data size
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_bmx055_acc_read(uint32_t addr, void *data, uint32_t size)
{
	st_r_drv_riic_transfer_t	s_i2c;

	s_i2c.device_address	= 0x18 * 2;
	s_i2c.sub_address_type	= RIIC_SUB_ADDR_WIDTH_8_BITS;
	s_i2c.sub_address		= addr;
	s_i2c.number_of_bytes	= size;
	s_i2c.p_data_buffer		= (uint8_t *)data;
	if(DRV_ERROR == control(gs_bmx055_handle, CTL_RIIC_READ, &s_i2c))
	{
		return ERROR_FAILURE;
	}
	return NO_ERROR;
}
/******************************************************************************
 * End of function r_bmx055_acc_read
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_bmx055_acc_write
 * Description	: Write data to BMX055 accelerometer registers
 * Arguments	: uint8_t addr - start address of BMX055
 *				: const void *data - pointer to the buffer storing data
 *				: size_t size - write data size
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_bmx055_acc_write(uint32_t addr, const void *data, uint32_t size)
{
	st_r_drv_riic_transfer_t	s_i2c;

	s_i2c.device_address	= 0x18 * 2;
	s_i2c.sub_address_type	= RIIC_SUB_ADDR_WIDTH_8_BITS;
	s_i2c.sub_address		= addr;
	s_i2c.number_of_bytes	= size;
	s_i2c.p_data_buffer		= (uint8_t *)data;
	//
	if(DRV_ERROR == control(gs_bmx055_handle, CTL_RIIC_WRITE, &s_i2c))
	{
		return ERROR_FAILURE;
	}
	return NO_ERROR;
}
/*****************************************************************************
 * End of function r_bmx055_acc_write
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_bmx055_gyr_read
 * Description	: Read data from BMX055 gyroscope register
 * Arguments	: uint8_t addr - start address of BMX055 register
 *				: const void *data - pointer of the buffer to store the data
 *				: size_t size - read data size
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_bmx055_gyr_read(uint32_t addr, void *data, uint32_t size)
{
	st_r_drv_riic_transfer_t	s_i2c;

	s_i2c.device_address	= 0x68 * 2;
	s_i2c.sub_address_type	= RIIC_SUB_ADDR_WIDTH_8_BITS;
	s_i2c.sub_address		= addr;
	s_i2c.number_of_bytes	= size;
	s_i2c.p_data_buffer		= (uint8_t *)data;
	if(DRV_ERROR == control(gs_bmx055_handle, CTL_RIIC_READ, &s_i2c))
	{
		return ERROR_FAILURE;
	}
	return NO_ERROR;
}
/******************************************************************************
 * End of function r_bmx055_gyr_read
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_bmx055_gyr_write
 * Description	: Write data to BMX055 gyroscope registers
 * Arguments	: uint8_t addr - start address of BMX055
 *				: const void *data - pointer to the buffer storing data
 *				: size_t size - write data size
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_bmx055_gyr_write(uint32_t addr, const void *data, uint32_t size)
{
	st_r_drv_riic_transfer_t	s_i2c;

	s_i2c.device_address	= 0x68 * 2;
	s_i2c.sub_address_type	= RIIC_SUB_ADDR_WIDTH_8_BITS;
	s_i2c.sub_address		= addr;
	s_i2c.number_of_bytes	= size;
	s_i2c.p_data_buffer		= (uint8_t *)data;
	//
	if(DRV_ERROR == control(gs_bmx055_handle, CTL_RIIC_WRITE, &s_i2c))
	{
		return ERROR_FAILURE;
	}
	return NO_ERROR;
}
/*****************************************************************************
 * End of function r_bmx055_gyr_write
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_bmx055_mag_read
 * Description	: Read data from BMX055 magnet register
 * Arguments	: uint8_t addr - start address of BMX055 register
 *				: const void *data - pointer of the buffer to store the data
 *				: size_t size - read data size
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_bmx055_mag_read(uint32_t addr, void *data, uint32_t size)
{
	st_r_drv_riic_transfer_t	s_i2c;

	s_i2c.device_address	= 0x10 * 2;
	s_i2c.sub_address_type	= RIIC_SUB_ADDR_WIDTH_8_BITS;
	s_i2c.sub_address		= addr;
	s_i2c.number_of_bytes	= size;
	s_i2c.p_data_buffer		= (uint8_t *)data;
	if(DRV_ERROR == control(gs_bmx055_handle, CTL_RIIC_READ, &s_i2c))
	{
		return ERROR_FAILURE;
	}
	return NO_ERROR;
}
/******************************************************************************
 * End of function r_bmx055_mag_read
 *****************************************************************************/

/******************************************************************************
 * Function Name: r_bmx055_mag_write
 * Description	: Write data to BMX055 magnet registers
 * Arguments	: uint8_t addr - start address of BMX055
 *				: const void *data - pointer to the buffer storing data
 *				: size_t size - write data size
 * Return Value : int_t - error code
 *****************************************************************************/
int_t	r_bmx055_mag_write(uint32_t addr, const void *data, uint32_t size)
{
	st_r_drv_riic_transfer_t	s_i2c;

	s_i2c.device_address	= 0x10 * 2;
	s_i2c.sub_address_type	= RIIC_SUB_ADDR_WIDTH_8_BITS;
	s_i2c.sub_address		= addr;
	s_i2c.number_of_bytes	= size;
	s_i2c.p_data_buffer		= (uint8_t *)data;
	//
	if(DRV_ERROR == control(gs_bmx055_handle, CTL_RIIC_WRITE, &s_i2c))
	{
		return ERROR_FAILURE;
	}
	return NO_ERROR;
}
/*****************************************************************************
 * End of function r_bmx055_mag_write
 *****************************************************************************/

/*****************************************************************************
 * EOF
 *****************************************************************************/
