#ifndef __REG__
#define __REG__

#include <stdint.h>

/* This section include in mpu6050 registers adress values*/
#define ADDR_SELF_TEST_X 			0x0D
#define ADDR_SELF_TEST_Y 			0x0E
#define ADDR_SELF_TEST_Z			0x0F
#define ADDR_SELF_TEST_A 			0x10
#define ADDR_SMPLRT_DIV 			0x19
#define ADDR_CONFIG			    	0x1A
#define ADDR_GYRO_CONFIG 			0x1B
#define ADDR_ACCEL_CONFIG			0x1C
/*---------------------------------*/
#define ADDR_FIFO_EN				0x23
#define ADDR_INT_PIN_CFG  			0x37
#define ADDR_INT_ENABLE 			0x38
#define ADDR_INT_STATUS 			0x3A
/*---------------------------------*/
#define ADDR_ACCEL_XOUT_H			0x3B
#define ADDR_ACCEL_XOUT_L			0x3C
#define ADDR_ACCEL_YOUT_H			0x3D
#define ADDR_ACCEL_YOUT_L			0x3E
#define ADDR_ACCEL_ZOUT_H			0x3F
#define ADDR_ACCEL_ZOUT_L			0x40
#define ADDR_TEMP_OUT_H				0x41
#define ADDR_TEMP_OUT_L 			0x42
#define ADDR_GYRO_XOUT_H 			0x43
#define ADDR_GYRO_XOUT_L 			0x44
#define ADDR_GYRO_YOUT_H 			0x45
#define ADDR_GYRO_YOUT_L 			0x46
#define ADDR_GYRO_ZOUT_H			0x47
#define ADDR_GYRO_ZOUT_L 			0x48
/*---------------------------------*/
#define ADDR_SIGNAL_PATH_RESET 		0x68
#define ADDR_USER_CTRL 				0x6A
#define ADDR_PWR_MGMT_1 			0x6B
#define ADDR_PWR_MGMT_2 			0x6C
#define ADDR_FIFO_COUNT_H 			0x72
#define ADDR_FIFO_COUNT_L 			0x73
#define ADDR_FIFO_R_W 				0x74
#define ADDR_WHO_AM_I 				0x75

/* ---------------------------------------------------------------------------------------------------------------- */

#define BIT_SHIFT_0 0
#define BIT_SHIFT_1 1
#define BIT_SHIFT_2 2
#define BIT_SHIFT_3 3
#define BIT_SHIFT_4 4
#define BIT_SHIFT_5 5
#define BIT_SHIFT_6 6
#define BIT_SHIFT_7 7
#define BIT_SHIFT_8 8
/* ---------------------------------------------------------------------------------------------------------------- */
/* This section include in mpu6050 registers*/ 

typedef struct MPU6050_R{
	uint8_t  	  SELF_TEST_X;
	uint8_t 	  SELF_TEST_Y;
	uint8_t 	  SELF_TEST_Z;
	uint8_t  	  SELF_TEST_A;
	uint8_t    		SMPRT_DIV;
	uint8_t 	       CONFIG;
	uint8_t  	  GYRO_CONFIG;
	uint8_t 	 ACCEL_CONFIG;
	uint8_t SIGNAL_PATH_RESET;
	uint8_t 	   PWR_MGMT_1;
	uint8_t 	   PWR_MGMT_2;
	uint8_t		  INT_PIN_CFG;
	uint8_t 	   INT_ENABLE;
	uint8_t		   INT_STATUS;
}MPU6050_REG_t;

#endif
