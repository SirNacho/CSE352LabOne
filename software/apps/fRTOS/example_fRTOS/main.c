#include<stdio.h>

#include "app_error.h"
#include "app_timer.h"/**/
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_clock.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
// #include "nordic_common.h"
#include "buckler.h"

#include "FreeRTOS.h"
#include "task.h"
//#include "queue.h"
//#include "timers.h"

//#include "supporting_functions.h"

void task_one(void *pvParameters) {
    // This loop runs FOREVER — tasks never return!
    for(;;) {
        printf("Task 1 Running!\n");
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void task_two(void *pvParameters) {
    for(;;) {
        printf("Task 2 Running!\n");  // Print to serial terminal
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {

    // Create tasks: (function, name, stack, params, priority, handle)
    xTaskCreate(task_one,   "Task 1",   256, NULL, 1, NULL);  // Priority 1
    xTaskCreate(task_two, "Task Two", 256, NULL, 1, NULL);  // Priority 1 (same)

    vTaskStartScheduler();  // Start FreeRTOS! (this line NEVER returns)
    return 0;  // We never get here, but the compiler likes it
}
