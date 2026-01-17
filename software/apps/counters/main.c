
//** Code: Hard Timers (Lab 6)
//** Author: Umer Huzaifa
//** Comments: 
//** Implementing a timer and a counter. 
//** Counter is another timer that can be triggered as we like instead of the clock signals


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "gpio.h"

#include "buckler.h"

uint32_t PRESCALER = 0;
uint32_t BUTTON_0_PIN = 28;
uint32_t LED_0_PIN = 25;
#define count_threshold 10000000

uint32_t secondsToTicks(uint8_t duration)
{
  uint32_t timer_frequency = 16000000 / (1 << PRESCALER);
  uint32_t ticks = ((uint32_t)duration * timer_frequency);
  return ticks;
}


void timer_init(uint8_t duration){
    NRF_TIMER4->TASKS_STOP = 0x01;
    NRF_TIMER4->TASKS_CLEAR = 0x01;
  // fill in this function to initialize a timer of your choice
  uint32_t PRESCALER = 0;
  uint32_t timer_frequency = 16000000 / (1 << PRESCALER);
  uint32_t ticks = (uint32_t)(duration * timer_frequency);
  printf("Initializing timer with duration of %ld ticks\n", secondsToTicks(duration));
  // timer 0 is at 0x40008000
  // CC is at 0x540

  NRF_TIMER4->PRESCALER = PRESCALER;
  NRF_TIMER4->BITMODE = 3;
  NRF_TIMER4->MODE = 0;
  NRF_TIMER4->CC[3] = ticks; 

  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;

  // all the register settings for setting up TIMER4 for 3 seconds timing operation
  NRF_TIMER4->CC[2] = secondsToTicks(3);
  // Additional settings for declaring TIMER4 as an interrupt

}

void counter_init(uint8_t duration){
  NRF_TIMER3->TASKS_STOP = 0x01;
  NRF_TIMER3->TASKS_CLEAR = 0x01;
  // fill in this function to initialize a timer of your choice
  uint32_t PRESCALER = 0;
  uint32_t timer_frequency = 16000000 / (1 << PRESCALER);
  uint32_t ticks = (uint32_t)(duration * timer_frequency);
  // timer 0 is at 0x40008000
  // CC is at 0x540

  NRF_TIMER3->PRESCALER = PRESCALER;
  NRF_TIMER3->BITMODE = 3;
  NRF_TIMER3->MODE = 1;
  NRF_TIMER3->CC[2] = ticks; 
  NRF_TIMER3->TASKS_CLEAR = 0x01;
  NRF_TIMER3->TASKS_START = 0x01;
  NRF_TIMER3->TASKS_COUNT = 0x01;


  // Configure TIMER3 as a counter
}

uint32_t read_timer()
{
    NRF_TIMER4->TASKS_CAPTURE[3] = 0x01;
      return NRF_TIMER4->CC[3];
  // fill in this function for reading the timer value on calling this function
}

uint32_t read_counter(){
  // just like TIMER4, read the values of TIMER3
      NRF_TIMER3->TASKS_CAPTURE[3] = 0x01;
      return NRF_TIMER3->CC[3];

}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Board initialized!\n");

  // You can use the NRF GPIO library to test your timers
  gpio_config(BUTTON_0_PIN, INPUT);
  uint8_t duration = 3;

  nrf_delay_ms(duration * 1000);
  counter_init(duration);
  timer_init(duration);

  // loop forever
  while (1) {
    uint32_t elapsed = read_timer();
    uint32_t timer = read_timer();

    printf("Timer 4 readings are: %d \n", timer);
    printf("Counter (timer 3) readings are: %d \n", read_counter());

    // Place a check when counter needs to be incremented.
    if (NRF_TIMER4->EVENTS_COMPARE[2]) {
        NRF_TIMER3->TASKS_COUNT = 0x01;
        NRF_TIMER4->EVENTS_COMPARE[2] = 0;

        NRF_TIMER4->TASKS_STOP = 1;
        NRF_TIMER4->TASKS_CLEAR = 1;
        NRF_TIMER4->TASKS_START = 1;

    }
    if (elapsed >= duration) {
      break;
    }
    // 
    //nrf_delay_ms(1000);
    __WFI();
  }
  NRF_TIMER4->TASKS_CLEAR = 1;
  printf("Timer over\n");
}
