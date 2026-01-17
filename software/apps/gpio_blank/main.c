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

typedef struct { 
  uint32_t* cnf0Ptr;
  uint32_t* cnf1Ptr;
  uint32_t* cnf2Ptr;
  uint32_t* cnf3Ptr;
  uint32_t* cnf4Ptr;
  uint32_t* cnf5Ptr;
  uint32_t* cnf6Ptr;
  uint32_t* cnf7Ptr;
  uint32_t* cnf8Ptr;
  uint32_t* cnf9Ptr;
  uint32_t* cnf10Ptr;
  uint32_t* cnf11Ptr;
  uint32_t* cnf12Ptr;
  uint32_t* cnf13Ptr;
  uint32_t* cnf14Ptr;
  uint32_t* cnf15Ptr;
  uint32_t* cnf16Ptr;
  uint32_t* cnf17Ptr;
  uint32_t* cnf18Ptr;
  uint32_t* cnf19Ptr;
  uint32_t* cnf20Ptr;
  uint32_t* cnf21Ptr;
  uint32_t* cnf22Ptr;
  uint32_t* cnf23Ptr;
  uint32_t* cnf24Ptr;
  uint32_t* cnf25Ptr;
  uint32_t* cnf26Ptr;
  uint32_t* cnf27Ptr;
  uint32_t* cnf28Ptr;
  uint32_t* cnf29Ptr;
  uint32_t* cnf30Ptr;
  uint32_t* cnf31Ptr;
}gpio_peripheral_struct;


gpio_peripheral_struct* CONF_PTR = (uint32_t*)0x50000700;

uint32_t* IN_PTR = (uint32_t*) 0x50000510;

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  uint32_t* outSetPtr = (uint32_t *) 0x50000508;
  uint32_t* outClrPtr = (uint32_t *) 0x5000050C;
  uint32_t* cnfPtr = (uint32_t *) 0x50000700;
  
  //printf("%X\n%X\n%X\n", outSetPtr, outClrPtr, cnfPtr);

  uint32_t* gpio23Ptr = (uint32_t *) 0x5000075C;
  uint32_t* gpio24Ptr = (uint32_t *) 0x50000760;
  uint32_t* gpio25Ptr = (uint32_t *) 0x50000764;

  *gpio23Ptr = 1;
  *gpio24Ptr = 1;
  *gpio25Ptr = 1;

  printf("Conf_ptr: %p \n", (uint32_t*)CONF_PTR + 22);
  printf("Conf_ptr: %p \n", (uint32_t*)CONF_PTR + 28);

  uint32_t* button_conf_ptr = (uint32_t*)CONF_PTR + 22;
  uint32_t* switch_conf_ptr = (uint32_t*)CONF_PTR + 28;

  *button_conf_ptr = 0;
  *switch_conf_ptr = 0;

  // loop forever
  while(1){
    printf("%d %d\n", (*IN_PTR >> 22) & 1, (*IN_PTR >> 28) & 1);


    //This section is for question 7
    /*
    nrf_delay_ms(500);
    *outSetPtr = 1 << 23;
    nrf_delay_ms(500); 
    *outSetPtr = 1 << 24; 
    nrf_delay_ms(500);
    *outSetPtr = 1 << 25; 
    nrf_delay_ms(500);
    *outClrPtr = 1 << 23;
    nrf_delay_ms(500);
    *outClrPtr = 1 << 24; 
    nrf_delay_ms(500);
    *outClrPtr = 1 << 25;
    nrf_delay_ms(500);
    */
  }
}
