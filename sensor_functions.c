/*
 * sensor_app.c
 *
 *  Created on: Sep 8, 2022
 *      Author: ahmet
 */

#include <stdlib.h>
#include "communication.h"
#include "mpu6050.h"
#include "config.h"
#include "sensor_functions.h"
#include "reg.h"


MPU6050_REG_t *MPU6050_REG;

static Status_t sensor_check()
{
	Status_t status;
	uint8_t  check;
	status = mpu6050_whoAmI(&check);

	if((status != status_ok) || (check != 0x68)) return status_false;

	return status_ok;
}

Status_t sensor_init(void)
{
	Status_t status;
	MPU6050_REG = (MPU6050_REG_t*)calloc(1,sizeof(MPU6050_REG_t));
	status = sensor_check();
	if(status != status_ok) return status_false;

	status += mpu6050_setClockSource(internal);
	
	//status += mpu6050_deviceReset(OFF);
	
	status += mpu6050_tempEnable(TEMP_ACTIVE);

	status += mpu6050_sleepMode(SLEEP_ACTIVE);

	status  = mpu6050_sampleRateDivision(Default_Sample_Rate_Division);

	status += mpu6050_gyroConfig(self_test_off, MEASURE_RANGE);

	status += mpu6050_accelConfig(self_test_off, MEASURE_RANGE);

	#if	DIGITAL_FILTER_ACTIVE == ON
		status += mpu6050_setDigitalFilter(Default_Digital_Filter_Range);
	#endif

	return status ? status_false : status_ok;
}

Status_t measure_data(MeasureValues_t* measure_values)
{
	Status_t status;

	uint8_t gyro_buff[6];
	uint8_t accel_buff[6];

	status  = mpu6050_getGyroValues(gyro_buff);
	status += mpu6050_getAccelValues(accel_buff);

	measure_values->gyro_x_r = (gyro_buff[0] << 8) | (gyro_buff[1]);
	measure_values->gyro_x   = measure_values->gyro_x_r / 131;
	measure_values->gyro_y_r = (gyro_buff[2] << 8) | (gyro_buff[3]);
	measure_values->gyro_y   = measure_values->gyro_y_r / 131;
	measure_values->gyro_z_r = (gyro_buff[4] << 8) | (gyro_buff[5]);
	measure_values->gyro_z   = measure_values->gyro_z_r / 131;

	measure_values->accel_x_r = ((accel_buff[0] << 8) | (accel_buff[1]));
	measure_values->accel_x   = (float)((measure_values->accel_x_r) / 16384);
	measure_values->accel_y_r = ((accel_buff[2] << 8) | (accel_buff[3]));
	measure_values->accel_y   = (float)((measure_values->accel_y_r) / 16384);
	measure_values->accel_z_r = ((accel_buff[4] << 8) | (accel_buff[5]));
	measure_values->accel_z   = (float)((measure_values->accel_z_r) / 16384);

	#if SELF_TEST == ON
		status += mpu6050_selfTest(self_test_result);
	#endif
	return status;
}
