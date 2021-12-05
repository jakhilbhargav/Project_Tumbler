/*
 * Gyro header
 */

#include "common.h"
#include "nrf_drv_twi.h"

#define GYRO_TWI_INSTANCE_ID    0
#define GYRO_ADDR               (0x22)

const nrf_drv_twi_t* gyro_getInstance();

void gyro_task (void* pvParameter);
