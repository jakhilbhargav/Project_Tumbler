/*
 * Application Init source
 */

#include "init.h"
#include "nrf_drv_clock.h"
#include "app_uart.h"
#include "nrf_uart.h"
#include "nrf_uarte.h"
#include "gyro.h"

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
        {
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
                //data_handler(m_sample);
            }

            break;
        }

        default:
            break;
    }
}

void App_Init()
{
    // Clock Init
    ret_code_t err_code;

    /* Initialize clock driver for better time accuracy in FREERTOS */
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    // LED Init for debug
    bsp_board_init(BSP_INIT_LEDS);

    // UART Init for printf
    const app_uart_comm_params_t comm_params =
    {
        RX_PIN_NUMBER,
        TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        APP_UART_FLOW_CONTROL_ENABLED,
        false,
        NRF_UARTE_BAUDRATE_115200
    };

    APP_UART_FIFO_INIT(&comm_params,
                        UART_RX_BUF_SIZE,
                        UART_TX_BUF_SIZE,
                        uart_error_handle,
                        APP_IRQ_PRIORITY_LOWEST,
                        err_code);

    APP_ERROR_CHECK(err_code);

    // TWIM init for gyro
    const nrf_drv_twi_config_t twi_gyro_config = {
       .scl                = ARDUINO_SCL_PIN,
       .sda                = ARDUINO_SDA_PIN,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    const nrf_drv_twi_t* pGyroInstance = gyro_getInstance();
    err_code = nrf_drv_twi_init(pGyroInstance, &twi_gyro_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(pGyroInstance);

    // Set I2C slave address
    uint8_t data[2];
    err_code = nrf_drv_twi_tx(pGyroInstance, GYRO_ADDR, data, sizeof(data), false);
    APP_ERROR_CHECK(err_code);

    printf("\r\nInit completed.\r\n");
}
