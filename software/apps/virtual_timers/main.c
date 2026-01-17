// Virtual timers
//
// Uses a single hardware timer to create an unlimited supply of virtual
//  software timers

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
#include "virtual_timer.h"
#include "virtual_timer_linked_list.h"

void led0_toggle() {
    nrf_gpio_pin_toggle(BUCKLER_LED0);
}

void led1_toggle() {
    nrf_gpio_pin_toggle(BUCKLER_LED1);
}

void led2_toggle() {
    nrf_gpio_pin_toggle(BUCKLER_LED2);
}

int main(void) {
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

  // Don't forget to initialize your timer library
  virtual_timer_init();
  nrf_delay_ms(30);

  //timer_start(1000, NULL, false);

  // Setup some timers and see what happens
  virtual_timer_start_repeated(1000000, &led0_toggle);
  virtual_timer_start_repeated(2000000, &led1_toggle);


  node_t* node_One = (node_t*)malloc(sizeof(node_t));
  node_One->timer_value =  10000;
  node_One->ID = 1;
  node_One->cbFunc = led0_toggle;
  node_One->period = 2000000;

  node_t* node_Two = (node_t*)malloc(sizeof(node_t));
  node_Two->timer_value = 10000;
  node_Two->ID = 2;
  node_Two->cbFunc = led1_toggle;
  node_Two->period = 5000000;

  node_t* node_Three = (node_t*)malloc(sizeof(node_t));
  node_Three->timer_value = 10000;
  node_Three->ID = 3;
  node_Three->cbFunc = led2_toggle;
  node_Three->period = 10000000;


  node_One->next = node_Two;
  node_Two->next = node_Three;
  node_Three->next = NULL;
  node_t* list_head = list_get_first();

  list_insert_sorted(node_One);
  list_insert_sorted(node_Two);
  list_insert_sorted(node_Three);

  if (list_head == node_One) 
  {
    NRF_TIMER4->CC[0] = 10000;
  }

  list_print(list_head);

  


  // loop forever
  while (1) {

/*    virtual_timer_start(2000000, NULL, false);
    virtual_timer_start(5000000, NULL, false);
    virtual_timer_start(10000000, NULL, false);*/

    nrf_delay_ms(10);
  }
}

