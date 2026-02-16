#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"
static uint8_t LEDS[2] = {BUCKLER_LED0, BUCKLER_LED1};
// TASK 1: Blink an LED every 500ms
void led_task(void *pvParameters) {
    // This loop runs FOREVER — tasks never return!
    for(;;) {
        nrf_gpio_pin_toggle(LEDS[0]);   // Flip the LED on/off
        vTaskDelay(pdMS_TO_TICKS(500));  // Sleep 500ms (lets other tasks run)
    }
}

// TASK 2: Print a message every second
void ledTwo_task(void *pvParameters) {
    for(;;) {
        nrf_gpio_pin_toggle(LEDS[1]);
        vTaskDelay(pdMS_TO_TICKS(1000));   // Sleep 1000ms
    }
}
void prep_hardware() {
    ret_code_t error_code = NRF_SUCCESS;

    // initialize GPIO driver
    if (!nrfx_gpiote_is_init()) {
      error_code = nrfx_gpiote_init();
    }
    APP_ERROR_CHECK(error_code);

    // configure leds
    // manually-controlled (simple) output, initially set
    nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
    for (int i=0; i<2; i++) {
      error_code = nrfx_gpiote_out_init(LEDS[i], &out_config);
      APP_ERROR_CHECK(error_code);
    }
}
int main(void) {
    prep_hardware();  // Initialize GPIO, UART, etc.

    // Create tasks: (function, name, stack, params, priority, handle)
    xTaskCreate(ledTwo_task,   "LED",   256, NULL, 1, NULL);  // Priority 1
    xTaskCreate(led_task, "LED2", 256, NULL, 1, NULL);  // Priority 1 (same)

    vTaskStartScheduler();  // Start FreeRTOS! (this line NEVER returns)
    return 0;  // We never get here, but the compiler likes it
}
