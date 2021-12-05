/*
 * Tumbler's application source
 */

#include "init.h"
#include "gyro.h"
#include "control.h"

int main(void)
{
    App_Init();

    // Create RTOS tasks
    xTaskCreate(gyro_task,     "GYRO",    configMINIMAL_STACK_SIZE + 200, NULL, 2, NULL);
    xTaskCreate(control_task,  "CONTROL", configMINIMAL_STACK_SIZE + 200, NULL, 2, NULL);

    /* Activate deep sleep mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* Start FreeRTOS scheduler. */
    printf("\r\nStarting RTOS scheduler...\r\n");
    vTaskStartScheduler();

    while (true)
    {
        /* FreeRTOS should not be here... FreeRTOS goes back to the start of stack
         * in vTaskStartScheduler function. */
    }
}
