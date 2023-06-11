#ifndef __FUNCTION_LAYER__
#define __FUNCTION_LAYER__

#include <stdbool.h>
#include "communication.h"
#include "mpu6050.h"


#define RESET_3_BIT       7
#define RESET_BYTES_1  0xff

#define INT_LATCH_DISABLE 0
#define INT_LATCH_ENABLE  1

typedef bool On_Off_t;

typedef enum{
	scale_range_1 = 0,// for accel 250 °/s   	|| 	  for gyro ± 2g
	scale_range_2,	  // for accel 500 °/s	    || 	  for gyro ± 4g
	scale_range_3,    // for accel 1000 °/s     || 	  for gyro ± 8g
	scale_range_4     // for accel 2000 °/s     ||    for gyro ± 16g
}ScaleRange_t;

typedef enum{
	self_test_on = 0,
	self_test_off
}SelfTest_t;

typedef enum{
	temp_reset = 1,
	accel_reset = 2,
	gyro_reset = 4,
	temp_and_accel_reset = 3,
	accel_and_gyro_reset = 6,
	temp_and_gyro_reset = 5,
	reset_all = 7
}SignalReset_t;

typedef enum{
	internal = 0,
	gyro_referance_x,
	gyro_referance_y,
	gyro_referance_z,
	external_32_khz,
	external_19_mhz,
	keep_timing_reset = 7
}ClockSelect_t;

typedef enum{
	digital_filter_0,
	digital_filter_1,
	digital_filter_2,
	digital_filter_3,
	digital_filter_4,
	digital_filter_5,
	digital_filter_6,
	digital_filter_7
}DigitalFilter_t;

typedef enum{
	int_level_high = 0,
	int_level_low
}IntLevel_t;

typedef enum{
	int_pin_pushpull = 0,
	int_pin_opendrain
}IntOutputType_t;



typedef struct{
	float  gyro_x_result;
	float  gyro_y_result;
	float  gyro_z_result;
	float accel_x_result;
	float accel_y_result;
	float accel_z_result;
}SelfTestResult_t;

Status_t mpu6050_sampleRateDivision(uint8_t clock_div);

Status_t mpu6050_setDigitalFilter(DigitalFilter_t digital_filter);

Status_t mpu6050_gyroConfig(SelfTest_t st_on_off, ScaleRange_t fs_sel);

Status_t mpu6050_accelConfig(SelfTest_t st_on_off, ScaleRange_t afs_sel);

Status_t mpu6050_intrInit(IntLevel_t int_level, IntOutputType_t output_type, On_Off_t int_latch_enable);

Status_t mpu6050_intEnable(On_Off_t fifo_owflow, On_Off_t data_ready);

Status_t mpu6050_readIntStatus(uint8_t* int_status_result);

Status_t mpu6050_getGyroValues(uint8_t buff[6]);

Status_t mpu6050_getAccelValues(uint8_t buff[6]);

Status_t mpu6050_getTempratureValue(uint8_t* buff);

Status_t mpu6050_signalReset(SignalReset_t reset_signal);

Status_t mpu6050_setClockSource(ClockSelect_t clock_source);

Status_t mpu6050_tempEnable(On_Off_t on_off);

Status_t mpu6050_sleepMode(On_Off_t on_off);

Status_t mpu6050_deviceReset(On_Off_t on_off);

Status_t mpu6050_whoAmI(uint8_t *buff);

Status_t mpu6050_selfTest(SelfTestResult_t* self_test_result);

#endif
