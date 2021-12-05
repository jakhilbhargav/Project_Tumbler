/*
 * GYRO source
 */

#include "gyro.h"

const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(GYRO_TWI_INSTANCE_ID);
uint8_t gyroData;

const nrf_drv_twi_t* gyro_getInstance()
{
    return &m_twi;
}

/*
 * Gyro task to fetch data
 */
void gyro_task(void* pvParameter)
{
    printf("\r\nGyro task started\r\n");

    ret_code_t err_code;

    while (true)
    {
        vTaskDelay(200);
        err_code = nrf_drv_twi_rx(&m_twi, GYRO_ADDR, &gyroData, sizeof(gyroData));
        APP_ERROR_CHECK(err_code);
    }
}
