
#ifndef __I2C_LAYER__
#define __I2C_LAYER__

#include <stdint.h>
#include "communication.h"


typedef enum Status{
	status_ok = 0,
	status_false = 1,
	status_unexpected = 2
}Status_t;

Status_t read_reg(uint8_t chip_add, uint8_t reg_add, uint8_t *data);

Status_t write_reg(uint8_t chip_add, uint8_t reg_add, uint8_t *data);

Status_t work_test(uint8_t chip_add);

#endif 