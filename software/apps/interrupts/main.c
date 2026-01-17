#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "buckler.h"
#include "gpio.h"

#include "software_interrupt.h"

uint32_t LED_0_PIN = 25;
uint32_t BUTTON_0_PIN = 28;

void SWI1_EGU1_IRQHandler(void) {
   NRF_EGU1->EVENTS_TRIGGERED[0]=0;
   // rest of the code follows here
   printf("Software Interrupt Recieved\n");
   nrf_delay_ms(4000);

}

// Step B: Write an IRQ Handler/ISR for the Interrupt
void GPIO_IRQHandler(void) {
   // Clear the Events register
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    printf("Interrupt Recieved\n");
    gpio_set(LED_0_PIN);
    nrf_delay_ms(400);
    gpio_clear(LED_0_PIN);
}

int main(void) {
    
  // initialize RTT library
  /*error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");*/

  gpio_config(LED_0_PIN, OUTPUT);
  gpio_config(BUTTON_0_PIN, INPUT);

  gpio_set(LED_0_PIN);
  //software_interrupt_init();

  // Initialize LED and BUTTON pins on the Buckler board
  //............


  // Steps for Setting up Interrupt
  // 1. configure the interrupt

  NRF_GPIOTE->CONFIG[0] = 0;
  NRF_GPIOTE->CONFIG[0] |= (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);
  NRF_GPIOTE->CONFIG[0] |= (BUTTON_0_PIN << GPIOTE_CONFIG_PSEL_Pos);
  NRF_GPIOTE->CONFIG[0] |= (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos);

  // 2. enable interrupt -- NRF_GPIOTE->...

  NRF_GPIOTE->INTENSET = (GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos);

  // 3. enable IRQ on NVIC  -- NVIC_EnableIRQ(...)

  NVIC_EnableIRQ(GPIOTE_IRQn);
  //NVIC_SetPriority(GPIOTE_IRQn, 0);
  //NVIC_SetPriority(SWI1_EGU1_IRQn, 1);

    while (1) {
        // Main application 
        printf("Looping\n");
        //nrf_delay_ms(100);
        //software_interrupt_generate();
    }
}

