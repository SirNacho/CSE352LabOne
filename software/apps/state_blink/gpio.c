#include "gpio.h"

typedef struct { 
    volatile uint32_t* CNF_PTR[32];
}gpio_peripheral_struct_t;

gpio_peripheral_struct_t* CONF_PTR = (uint32_t*)0x50000700;
volatile uint32_t* IN_PTR = (uint32_t*) 0x50000510;
volatile uint32_t* OUT_SET_PTR = (uint32_t*) 0x50000508;
volatile uint32_t* OUT_CLR_PTR = (uint32_t*) 0x5000050C;

// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
    //volatile uint32_t* cnf_reg_ptr = CONF_PTR + gpio_num;
    if (dir == OUTPUT)
    {
        CONF_PTR->CNF_PTR[gpio_num] = 1;
    }
    else
    {
        CONF_PTR->CNF_PTR[gpio_num] = 0;
    }
}

// Set gpio_num high
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {
    *OUT_SET_PTR = (1 << gpio_num);
}

// Set gpio_num low
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {
    *OUT_CLR_PTR = (1 << gpio_num);
}

// Inputs: 
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    // should return pin state
    return (*IN_PTR >> gpio_num) & 1;
}
