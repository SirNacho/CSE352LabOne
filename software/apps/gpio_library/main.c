#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrf_delay.h"

#include "buckler.h"
#include "gpio.h"

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  uint32_t LED_0_PIN = 25;
  uint32_t LED_1_PIN = 24;

  uint32_t SWITCH_0_PIN = 22;
  uint32_t BUTTON_0_PIN = 28;

  gpio_config(LED_0_PIN, OUTPUT);
  gpio_config(LED_1_PIN, OUTPUT);

  gpio_config(SWITCH_0_PIN, INPUT);
  gpio_config(BUTTON_0_PIN, INPUT);

  gpio_set(LED_0_PIN);
  gpio_set(LED_1_PIN);


  // loop forever
  while(1){

    
    bool button_pressed = gpio_read(BUTTON_0_PIN);
    if (button_pressed == true)
    {
      gpio_set(LED_0_PIN);
    }
    else
    {
      gpio_clear(LED_0_PIN);
    }

    bool switch_on_high = gpio_read(SWITCH_0_PIN);
    if (switch_on_high == true)
    {
      gpio_set(LED_1_PIN);
    }
    else
    {
      gpio_clear(LED_1_PIN);
    }
    
    //nrf_delay_ms(500);
  }
}
