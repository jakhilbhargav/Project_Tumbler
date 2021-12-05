/*
 * Control source
 */

#include "control.h"

void control_task(void* pvParameter)
{
    printf("\r\nControl task started\r\n");

    while (1)
    {
        vTaskDelay(200);
    }
}
