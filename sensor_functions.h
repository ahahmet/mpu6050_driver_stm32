/*
 * sensor_app.h
 *
 *  Created on: Sep 9, 2022
 *      Author: ahmet
 */
#ifndef __SENSOR__
#define __SENSOR__ 

#include <stdint.h>
#include "communication.h"

typedef struct{
	double gyro_x;
	double gyro_y;
	double gyro_z;
	float accel_x;
	float accel_y;
	float accel_z;

	int16_t gyro_x_r;
	int16_t gyro_y_r;
	int16_t gyro_z_r;
	int16_t accel_x_r;
	int16_t accel_y_r;
	int16_t accel_z_r;
}MeasureValues_t;

Status_t sensor_init(void);

Status_t measure_data(MeasureValues_t* measure_values);

#endif