#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "communication.h"
#include "mpu6050.h"
#include "reg.h"
#include "config.h"

#define chip_add (0x68<<1)

extern MPU6050_REG_t* MPU6050_REG;

typedef struct{
	float gyro_val[3];
	float gyro_val_st[3];

	float accel_val[3];
	float accel_val_st[3];

	float gyro_val_st_res[3];
	float accel_val_st_res[3];

	float gyro_test[3];
	float accel_test[3];

	float gyro_val_ft[3];
	float accel_val_ft[3];

}SelfTest;

typedef union{
	uint8_t val;
	struct{
		uint8_t gyro  : 5;
		uint8_t accel : 3;
	};
}TestGyro_t;

typedef union{
	uint8_t val;
	struct{
		uint8_t accel_x  : 2;
		uint8_t accel_y  : 2;
		uint8_t accel_z  : 2;
		uint8_t reserved : 2;
	};
}TestAccel_t;

Status_t mpu6050_sampleRateDivision(uint8_t clock_div)
{
	//MPU6050_REG->SMPRT_DIV &= ~(0xFF << 0);

	MPU6050_REG->SMPRT_DIV |= ((clock_div-1) << BIT_SHIFT_0);

	return write_reg(chip_add, ADDR_SMPLRT_DIV, &MPU6050_REG->SMPRT_DIV);
}

//Accelerometer have to set 1 kHz and Gyroscope have to set 8 kHz if select the digital_filter_0 else have to set 1kHz
Status_t mpu6050_setDigitalFilter(DigitalFilter_t digital_filter)
{
	MPU6050_REG->CONFIG &= ~(RESET_3_BIT << BIT_SHIFT_0);

	MPU6050_REG->CONFIG |= (1 << digital_filter);

	return write_reg(chip_add, ADDR_CONFIG, &MPU6050_REG->CONFIG);
}

Status_t mpu6050_gyroConfig(SelfTest_t  st_on_off, ScaleRange_t fs_sel)
{
	if(st_on_off != self_test_on){
		MPU6050_REG->GYRO_CONFIG &= ~(1 << BIT_SHIFT_5);
		MPU6050_REG->GYRO_CONFIG &= ~(1 << BIT_SHIFT_6);
		MPU6050_REG->GYRO_CONFIG &= ~(1 << BIT_SHIFT_7);
	}else{
		MPU6050_REG->GYRO_CONFIG |= (1 << BIT_SHIFT_5);
		MPU6050_REG->GYRO_CONFIG |= (1 << BIT_SHIFT_6);
		MPU6050_REG->GYRO_CONFIG |= (1 << BIT_SHIFT_7);
	}

	MPU6050_REG->GYRO_CONFIG |= (fs_sel << BIT_SHIFT_3);

	return write_reg(chip_add, ADDR_GYRO_CONFIG, &MPU6050_REG->GYRO_CONFIG);
}

Status_t mpu6050_accelConfig(SelfTest_t st_on_off, ScaleRange_t afs_sel)
{//BURAYI SONRA D�ZELT
	if(st_on_off != self_test_on){
		MPU6050_REG->ACCEL_CONFIG &= ~(1 << BIT_SHIFT_5);
		MPU6050_REG->ACCEL_CONFIG &= ~(1 << BIT_SHIFT_6);
		MPU6050_REG->ACCEL_CONFIG &= ~(1 << BIT_SHIFT_7);
	}else{
		MPU6050_REG->ACCEL_CONFIG |= (1 << BIT_SHIFT_5);
		MPU6050_REG->ACCEL_CONFIG |= (1 << BIT_SHIFT_6);
		MPU6050_REG->ACCEL_CONFIG |= (1 << BIT_SHIFT_7);
	}

	MPU6050_REG->ACCEL_CONFIG |= (afs_sel << BIT_SHIFT_3);

	return write_reg(chip_add, ADDR_ACCEL_CONFIG, &MPU6050_REG->ACCEL_CONFIG);
}

Status_t mpu6050_intrInit(IntLevel_t int_level, IntOutputType_t output_type, On_Off_t int_latch_enable)
{
	if(int_level == int_level_low){
		MPU6050_REG->INT_PIN_CFG |= (1 << BIT_SHIFT_7);
	}else{
		MPU6050_REG->INT_PIN_CFG &= ~(1 << BIT_SHIFT_7);
	}
	if(output_type == int_pin_opendrain){
		MPU6050_REG->INT_PIN_CFG |= (1 << BIT_SHIFT_6);
	}else{
		MPU6050_REG->INT_PIN_CFG &= ~(1 << BIT_SHIFT_6);
	}
	if(int_pin_opendrain == ON){
		MPU6050_REG->INT_PIN_CFG |= (1 << BIT_SHIFT_5);
	}else{
		MPU6050_REG->INT_PIN_CFG &= ~(1 << BIT_SHIFT_5);
	}

	return write_reg(chip_add, ADDR_INT_PIN_CFG , &MPU6050_REG->INT_PIN_CFG);
}

Status_t mpu6050_intEnable(On_Off_t fifo_owflow, On_Off_t data_ready)
{
	if(data_ready == 1){
		MPU6050_REG->INT_ENABLE |= (1<< BIT_SHIFT_0);
	}else{
		MPU6050_REG->INT_ENABLE &= ~(1<< BIT_SHIFT_0);
	}
	if(fifo_owflow == 1){
		MPU6050_REG->INT_ENABLE |= (1<< BIT_SHIFT_4);
	}else{
		MPU6050_REG->INT_ENABLE &= ~(1<< BIT_SHIFT_4);
	}

	return write_reg(chip_add, ADDR_INT_ENABLE, &MPU6050_REG->INT_ENABLE);
}

Status_t mpu6050_readIntStatus(uint8_t* int_status_result)
{
	Status_t status;

	status  = read_reg(chip_add, ADDR_INT_STATUS, (int_status_result+0));
	status += read_reg(chip_add, ADDR_INT_STATUS, (int_status_result+1));

	return status ? status_false : status_ok;
}

Status_t mpu6050_getGyroValues(uint8_t buff[6])
{
	Status_t status;

	for(int i=0; i<6; ++i)
	{
		status += read_reg(chip_add, ADDR_GYRO_XOUT_H + i, &buff[i]);
	}

	return status ? status_false : status_ok;
}

Status_t mpu6050_getAccelValues(uint8_t buff[6])
{
	Status_t status;

	for(int i=0; i<6; ++i)
	{
		status += read_reg(chip_add, ADDR_ACCEL_XOUT_H + i, &buff[i]);
	}

	return status ? status_false : status_ok;
}

Status_t mpu6050_getTempratureValue(uint8_t* buff)
{
	Status_t status;

	for(int i=0; i<2; ++i)
	{
		status += read_reg(chip_add, ADDR_TEMP_OUT_H + i, (buff + i));
	}

	return status ? status_false : status_ok;
}

Status_t mpu6050_signalReset(SignalReset_t reset_signal)
{
	//cancel the all measurement reset command
	MPU6050_REG->SIGNAL_PATH_RESET &= ~(RESET_3_BIT << BIT_SHIFT_0);

	MPU6050_REG->SIGNAL_PATH_RESET |= (1<<reset_signal);

	return write_reg(chip_add, ADDR_SIGNAL_PATH_RESET, &MPU6050_REG->SIGNAL_PATH_RESET);
}

Status_t mpu6050_setClockSource(ClockSelect_t clock_source)
{
	MPU6050_REG->PWR_MGMT_1 &= ~(RESET_3_BIT << BIT_SHIFT_0);

	MPU6050_REG->PWR_MGMT_1 |= (clock_source << BIT_SHIFT_0);

	return write_reg(chip_add, ADDR_PWR_MGMT_1, &MPU6050_REG->PWR_MGMT_1);
}

Status_t mpu6050_tempEnable(On_Off_t on_off)
{
	if(on_off == OFF){
		MPU6050_REG->PWR_MGMT_1 &= ~(1 << BIT_SHIFT_3);
	}else{
		MPU6050_REG->PWR_MGMT_1 |=  (1 << BIT_SHIFT_3);
	}

	return write_reg(chip_add, ADDR_PWR_MGMT_1, &MPU6050_REG->PWR_MGMT_1);
}

Status_t mpu6050_sleepMode(On_Off_t on_off)
{
	if(on_off == OFF){
		MPU6050_REG->PWR_MGMT_1 &= ~(1 << BIT_SHIFT_6);
	}else{
		MPU6050_REG->PWR_MGMT_1 |=  (1 << BIT_SHIFT_6);
	}

	return write_reg(chip_add, ADDR_PWR_MGMT_1, &MPU6050_REG->PWR_MGMT_1);
}

Status_t mpu6050_deviceReset(On_Off_t on_off)
{
	if(on_off == OFF){
		MPU6050_REG->PWR_MGMT_1 &= ~(1 << BIT_SHIFT_7);
	}else{
		MPU6050_REG->PWR_MGMT_1 |=  (1 << BIT_SHIFT_7);
	}

	return write_reg(chip_add, ADDR_PWR_MGMT_1, &MPU6050_REG->PWR_MGMT_1);
}

Status_t mpu6050_whoAmI(uint8_t* buff)
{
	return read_reg(chip_add, ADDR_WHO_AM_I, buff);
}

static Status_t mpu6050_getTestValue(uint8_t buff[6])
{
	Status_t status;
	TestGyro_t gyro_test[3];
	uint8_t temp_val;
	TestAccel_t  accel_test;

	for(int i=0; i<3; ++i){
		status += read_reg(chip_add, ADDR_SELF_TEST_X, &(temp_val));
		gyro_test[i].val = temp_val;
		buff[i] = gyro_test[i].gyro;
	}
	status += read_reg(chip_add, ADDR_SELF_TEST_X, &accel_test.val);

	buff[3] = (gyro_test[0].accel << BIT_SHIFT_2) | (accel_test.accel_x << BIT_SHIFT_0);
	buff[4] = (gyro_test[1].accel << BIT_SHIFT_2) | (accel_test.accel_y << BIT_SHIFT_0);
	buff[5] = (gyro_test[2].accel << BIT_SHIFT_2) | (accel_test.accel_z << BIT_SHIFT_0);

	return status ? status_false : status_ok;
}

Status_t mpu6050_selfTest(SelfTestResult_t* self_test_result)
{
	Status_t status;

	SelfTest* self_test;
	self_test = (SelfTest*)calloc(1,sizeof(SelfTest));
	uint8_t  gyro_buff[6], accel_buff[6], test_buff[6];
	//Digital filter enabled low pass 10 Hz
	//mpu6050_setDigitalFilter(digital_filter_0);

	//Some init configuration for get the gyro and accelerometer value
	status = mpu6050_setClockSource(internal);
	status += mpu6050_sampleRateDivision(8);

	/* ---------------------------------------------------------*/
	// Gyro and Accelerometer Result With self Test
	status += mpu6050_gyroConfig(self_test_on, scale_range_1);
	status += mpu6050_accelConfig(self_test_on, scale_range_1);

	status += mpu6050_getGyroValues(gyro_buff);
	status += mpu6050_getAccelValues(accel_buff);

	for(int i=0; i<3; ++i){
		self_test->gyro_val[i]  = (gyro_buff[2*i] << BIT_SHIFT_8) | (gyro_buff[2*i +1]);
		self_test->accel_val[i] = (accel_buff[2*i] << BIT_SHIFT_8) | (accel_buff[2*i +1]);
	}

	/* ---------------------------------------------------------*/
	// Gyro and Accelerometer Result Without self Test
	status += mpu6050_gyroConfig(self_test_off, scale_range_1);
	status += mpu6050_accelConfig(self_test_off, scale_range_1);

	status += mpu6050_getGyroValues(gyro_buff);
	status += mpu6050_getAccelValues(accel_buff);

	for(int i=0; i<3; ++i){
		self_test->gyro_val_st[i]  = (gyro_buff[2*i] << BIT_SHIFT_8) | (gyro_buff[2*i +1]);
		self_test->accel_val_st[i] = (accel_buff[2*i] << BIT_SHIFT_8) | (accel_buff[2*i +1]);
	}

	/* ---------------------------------------------------------*/
	// Calculate the Self Test Response
	for(int i=0; i<3; ++i){
		self_test->gyro_val_st_res[i] = ((self_test->gyro_val_st[i]) / (131)) - ((self_test->gyro_val[i]) / (131));
		self_test->accel_val_st_res[i] = ((self_test->accel_val_st[i]) / (16384)) -((self_test->accel_val[i]) / (16384));
	}

	/* ---------------------------------------------------------*/
	// Calculate the Factory Trim Value
	status += mpu6050_getTestValue(test_buff);

	for(int i=0; i<3; ++i){
		if(test_buff[i] != 0){
			self_test->gyro_val_ft[i] = 3275*( pow(1.046,(self_test->gyro_test[i]-1)) );
		}else{
			self_test->gyro_val_ft[i] = 0;
		}
		if(test_buff[i+3] != 0){
			self_test->accel_val_ft[i] = 1392.64*(pow(2.705882, (self_test->accel_test[i]-1)/30) );
		}else{
			self_test->accel_val_ft[i] = 0;
		}
	}

	self_test->gyro_val_ft[1] = self_test->gyro_val_ft[1] * -1;

	self_test_result->gyro_x_result = (self_test->gyro_val_st_res[0] - self_test->gyro_val_ft[0]) / self_test->gyro_val_ft[0];
	self_test_result->gyro_y_result = (self_test->gyro_val_st_res[1] - self_test->gyro_val_ft[1]) / self_test->gyro_val_ft[1];
	self_test_result->gyro_z_result = (self_test->gyro_val_st_res[2] - self_test->gyro_val_ft[2]) / self_test->gyro_val_ft[2];

	self_test_result->accel_x_result = (self_test->accel_val_st_res[0] - self_test->accel_val_ft[0]) / self_test->accel_val_ft[0];
	self_test_result->accel_y_result = (self_test->accel_val_st_res[1] - self_test->accel_val_ft[1]) / self_test->accel_val_ft[1];
	self_test_result->accel_z_result = (self_test->accel_val_st_res[2] - self_test->accel_val_ft[2]) / self_test->accel_val_ft[2];

	free(self_test);

	return status ? status_false : status_ok;
}

