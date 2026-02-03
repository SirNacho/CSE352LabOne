// Finite State Machine for 
// Blinking LED at the press of a button

#include <math.h>
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
#include "nrf_drv_spi.h"

#include "buckler.h"
#include "display.h"
#include "kobukiActuator.h"
#include "kobukiSensorPoll.h"
#include "kobukiSensorTypes.h"
#include "kobukiUtilities.h"
#include "mpu9250.h"

#include "gpio.h"
#include "states.h"


// intialize statechart variables
  // if needed


state current_state;

void print_state(){

  printf("CURRENT STATE: %d \n", current_state);
}
void initialize_hardware(){

  gpio_config(BUCKLER_LED0, true);
  gpio_config(BUCKLER_LED1, true);
  gpio_config(BUCKLER_LED2, true);


  gpio_set(BUCKLER_LED0);
  gpio_set(BUCKLER_LED1);
  gpio_set(BUCKLER_LED2);

  gpio_config(BUCKLER_BUTTON0, false);

  // any additional robot related initialization can be placed here as well

}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // initialize LEDs

  initialize_hardware();
  
  // initialize the state variable

  current_state = INIT; 
 
  // Initialize SPI for display

    nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(1);
    nrf_drv_spi_config_t spi_config = {
        .sck_pin = BUCKLER_LCD_SCLK,
        .mosi_pin = BUCKLER_LCD_MOSI,
        .miso_pin = BUCKLER_LCD_MISO,
        .ss_pin = BUCKLER_LCD_CS,
        .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
        .orc = 0,
        .frequency = NRF_DRV_SPI_FREQ_4M,
        .mode = NRF_DRV_SPI_MODE_2,
        .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
    };
  error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  APP_ERROR_CHECK(error_code); 
  display_init(&spi_instance);
  printf("Display initialized\n");
  
  // loop forever, running state machine
  while (1) {
    // delay before continuing
    // Note: removing this delay will make responses quicker, but will result
    //  in printf's in this loop breaking JTAG
    nrf_delay_ms(50);
    print_state();

    // iterate statechart
    switch(current_state){


      case INIT:
        // move on to testing the button status
        current_state = OFF;
        break;
      case ON:
        printf("ON");
        gpio_clear(BUCKLER_LED0);
        
        if (gpio_read(BUCKLER_BUTTON0))
          {current_state = BLINK;            
            
          }
        break;
      case OFF:
        printf("OFF");
        gpio_set(BUCKLER_LED0);
        
        if (!gpio_read(BUCKLER_BUTTON0))
          {current_state = ON;          
          
          }
        break;
      case BLINK:
        printf("BLINK");
        while (1) {
          gpio_set(BUCKLER_LED0);
          nrf_delay_ms(500);
          gpio_clear(BUCKLER_LED0);

          if (!gpio_read(BUCKLER_BUTTON0))
          {current_state = OFF;          
          
          }
          break;
        }
        break;
      default:
        current_state = OFF;

    }
      char buf[2][16];
      snprintf(buf[0], sizeof(buf[0]), "State: %d\n", current_state);
      display_write(buf[0], 0);
    
  }
}
