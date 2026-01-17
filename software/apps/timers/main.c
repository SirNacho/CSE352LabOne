//** Code: Hard Timers
//** Author: Umer Huzaifa
//** Comments: Keeping a separate app for the hardware timers and counters

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"

uint32_t PRESCALER = 0;

uint32_t secondsToTicks(uint8_t duration)
{
  uint32_t timer_frequency = 16000000 / (1 << PRESCALER);
  uint32_t ticks = ((uint32_t)duration * timer_frequency);
  return ticks;
}

void timer_init(uint8_t duration)
{
    NRF_TIMER4->TASKS_STOP = 0x01;
    NRF_TIMER4->TASKS_CLEAR = 0x01;

  // fill in this function to initialize a timer of your choice
  printf("Initializing timer with duration of %ld ticks\n", secondsToTicks(duration));
  // timer 0 is at 0x40008000
  // CC is at 0x540

  NRF_TIMER4->PRESCALER = PRESCALER;
  NRF_TIMER4->BITMODE = 3;
  NRF_TIMER4->MODE = 0;
  NRF_TIMER4->CC[3] = secondsToTicks(duration); 

  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;
}

uint32_t read_timer()
{
    NRF_TIMER4->TASKS_CAPTURE[3] = 0x01;
      return NRF_TIMER4->CC[3];
  // fill in this function for reading the timer value on calling this function
}

int main(void)
{
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Board initialized!\n");

  // You can use the NRF GPIO library to test your timers
  nrf_gpio_pin_dir_set(BUCKLER_LED0, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED2, NRF_GPIO_PIN_DIR_OUTPUT);

  uint8_t duration = 3; // or a time of your choice.
  timer_init(duration);

  nrf_delay_ms(duration * 1000);

  // loop forever
  while (1)
  {
    uint32_t elapsed = read_timer();
    printf("Timer 4 readings are: %ld \n", elapsed);
    if (elapsed >= duration) {
      break;
    }
  }
  NRF_TIMER4->TASKS_CLEAR = 1;
  printf("Timer over\n");
}