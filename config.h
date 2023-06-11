/*
 * config.h
 *
 *  Created on: Sep 9, 2022
 *      Author: ahmet
 */

#ifndef _CONFIG_MPU6050_H_
#define _CONFIG_MPU6050_H_

#include <stdint.h>

#include "config.h"
#include "mpu6050.h"

#define ON  					 	  1
#define OFF 					 	  0

#define MAX			 	  scale_range_4
#define MID			 	  scale_range_2
#define MIN 		  	  scale_range_1

#define SELF_TEST					OFF
#define DIGITAL_FILTER_ACTIVE   	ON
#define TEMP_ACTIVE					OFF
#define SLEEP_ACTIVE 				OFF
#define MEASURE_RANGE 				MIN


#define Default_Sample_Rate_Division 				   8
#define Default_Digital_Filter_Range	digital_filter_6


#endif /* SENSOR_DRIVERS_MPU6050_CONFIG_H_ */
