#include <stdint.h>
#include "communication.h"

#include "stm32f4xx_hal.h"
#define i2c_size_1byte 0x00000001U
extern I2C_HandleTypeDef hi2c1;

Status_t read_reg(uint8_t chip_add, uint8_t reg_add, uint8_t *data)
{
	char temp_state;	
	temp_state = HAL_I2C_Mem_Read( &hi2c1, chip_add, reg_add, i2c_size_1byte, data,i2c_size_1byte, 100 );
	
	if(temp_state == 0){
		return status_ok;
	}
	else if(temp_state ==1){
		return status_false;
	}
	else{
		return status_unexpected;
	}	
}

Status_t write_reg(uint8_t chip_add, uint8_t reg_add, uint8_t *data)
{
	char temp_state;	
	temp_state = HAL_I2C_Mem_Write(&hi2c1,chip_add, reg_add, i2c_size_1byte, data,i2c_size_1byte, 100);
	
	if(temp_state == 0){
		return status_ok;
	}
	else if(temp_state ==1){
		return status_false;
	}
	else{
		return status_unexpected;
	}	
}

Status_t work_test(uint8_t chip_add)
{
	char temp_state;
	
	temp_state = HAL_I2C_IsDeviceReady(&hi2c1, chip_add, 3, 100);	
	if(temp_state == 0){
		return status_ok;
	}
	else if(temp_state ==1){
		return status_false;
	}
	else{
		return status_unexpected;
	}
}
