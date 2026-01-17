#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_pwm.h"

#define PWM_INSTANCE    NRF_PWM0  // Use PWM0 instance
#define PWM_PIN1        23        // GPIO pin for PWM channel 1
#define PWM_PIN2        24        // GPIO pin for PWM channel 2
#define PWM_PIN3        25        // GPIO pin for PWM channel 3

#define PWM_FREQUENCY   1000      // PWM frequency in Hz
#define PWM_TOP_VALUE   16000     // PWM period (16000 ticks @ 1 MHz)

// Define duty cycles (values from 0 to PWM_TOP_VALUE)

uint16_t pwm_seq[4] = {(PWM_TOP_VALUE * 0.1), (PWM_TOP_VALUE * 0.25), (PWM_TOP_VALUE * 0.5), (PWM_TOP_VALUE * 0.75)};

//uint16_t pwm_seq_values[] = {
    //...,  // Fraction of PWM_TOP_VALUE -- the value to go to COMP0 register
    //(PWM_TOP_VALUE * 0.1)
    //...,  // Fraction of PWM_TOP_VALUE -- the value to go to COMP1 register
    //...,  // Fraction of PWM_TOP_VALUE -- the value to go to COMP2 register
//};


// Function to initialize PWM
void pwm_init(void) {
    // Configure PWM pins, 
    NRF_PWM0->PSEL.OUT[0] = (PWM_PIN1 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);
    NRF_PWM0->PSEL.OUT[1] = (PWM_PIN2 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);
     NRF_PWM0->PSEL.OUT[2] = (PWM_PIN3 << PWM_PSEL_OUT_PIN_Pos) | (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);
    NRF_PWM0->ENABLE = (1 << PWM_ENABLE_ENABLE_Pos);
    NRF_PWM0->MODE = (0 << PWM_MODE_UPDOWN_Pos);
    NRF_PWM0->PRESCALER = (0 << PWM_PRESCALER_PRESCALER_Pos);
    NRF_PWM0->COUNTERTOP = (PWM_TOP_VALUE << PWM_COUNTERTOP_COUNTERTOP_Pos); //1 msec
    NRF_PWM0->LOOP = (0 << PWM_LOOP_CNT_Pos);
    NRF_PWM0->DECODER = (2 << PWM_DECODER_LOAD_Pos) | (0 << PWM_DECODER_MODE_Pos);
    NRF_PWM0->SEQ[0].PTR = ((uint32_t)(pwm_seq) << PWM_SEQ_PTR_PTR_Pos);
    NRF_PWM0->SEQ[0].CNT = ((sizeof(pwm_seq) / sizeof(uint16_t)) << PWM_SEQ_CNT_CNT_Pos);
    NRF_PWM0->SEQ[0].REFRESH = 0;
    NRF_PWM0->SEQ[0].ENDDELAY = 0;
    NRF_PWM0->TASKS_SEQSTART[0] = 1;
    NRF_PWM0->ENABLE = 1;
    
    
}

int main(void) {
    pwm_init();

    while (1) {
        printf(PWM_PSEL_OUT_PIN_Pos);
        // Infinite loop, PWM runs independently
    }
}
