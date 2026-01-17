// Analog accelerometer app
//
// Reads data from the ADXL327 analog accelerometer

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>  // needed for tilt angle calculation


#include "timestamp.h"

#define RAD2DEG 57.13 

// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2


float timestamp=0.0;



// callback for SAADC events
void saadc_callback (nrfx_saadc_evt_t const * p_event) {
  // don't care about adc callbacks
}

// sample a particular analog channel in blocking mode
 nrf_saadc_value_t sample_value (uint8_t channel) {
   nrf_saadc_value_t val;
   ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
   APP_ERROR_CHECK(error_code);
   return val;
 }

float tilt(float ax, float ay, float az, bool method)
{
  float result;

  if (method == true)
  {
    result = atan(ax/ay);
  }
  else
  {
    result = acos(az/(sqrt(pow(ax, 2) + pow(ay, 2) + pow(az, 2))));
  }

    return result * 180.0 / M_PI;
}


int main (void) {
//-------------------------------------------------------------//  
//----------------- MODIFY THE FOLLOWING CODE -----------------//
  ret_code_t error_code = NRF_SUCCESS;

  // // initialize RTT ENDS_INIT();

  // initialize analog to digital converter
  // ...
 // ...
  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  // initialize analog to digital converter
  nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
  saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
  error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
  APP_ERROR_CHECK(error_code);

  // initialize analog inputs
  // configure with 0 as input pin for now
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
  channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
  channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_X;
  error_code = nrfx_saadc_channel_init(X_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Y;
  error_code = nrfx_saadc_channel_init(Y_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Z;
  error_code = nrfx_saadc_channel_init(Z_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // initialization complete
  printf("Buckler initialized!\n");

  
  ////////////////////

  // calibration of the voltage output from the ADC

  // .. copy your work from the accelerometer lab

  // variables for sampling raw values from the ADC
  
  // defining tilt variables
  
  
  // Initialize the SD Card for logging data in TESTFILE.csv
  init_SDCard();

  // loop forever
  while (1) {
   

    // sample analog inputs
    nrf_saadc_value_t x_value = sample_value(X_CHANNEL);
    nrf_saadc_value_t y_value = sample_value(Y_CHANNEL);
    nrf_saadc_value_t z_value = sample_value(Z_CHANNEL);
   // convert the ADC output to voltage values
    float x_voltage_value = x_value * 0.00087890625;
    float y_voltage_value = y_value * 0.00087890625;
    float z_voltage_value = z_value * 0.00087890625;

   // calculation of acceleration from voltage output
   //-------------------------------------------------------------//    
   //---------- DONT MODIFY THE CODE BELOW THIS LINE -------------//
// display results
    //float ax, ay, az;
    float ax = (((x_value * 0.00087890625) - 1.5) / 420) * 1000;
    float ay = (((y_value * 0.00087890625) - 1.5) / 420) * 1000;
    float az = (((z_value * 0.00087890625) - 1.5) / 420) * 1000;
    float method1 = tilt(ax, ay, az, true);
    float method2 =  tilt(ax, ay, az, false);

    printf("a_x: %f\ta_y: %f\ta_z:%f\n", ax, ay, az);
    printf("method1: %f\nmethod2: %f\n", method1, method2);
   
     //code for logging data from the accelerometer    
      if (!gpio_read(BUCKLER_BUTTON0)) {
         
        timestamp = get_timestamp();      
        simple_logger_log("%f,%f,%f,%f,%f,%f\n", timestamp, ax, ay, az, method1, method2);
      
        printf("%f - Wrote line to SD card\n", timestamp);
        // Signal that lines were written
        
        // gpio_clear(BUCKLER_LED0);      
        nrf_delay_ms(100);
      }
      else
        gpio_set(BUCKLER_LED0);  
  }   
}