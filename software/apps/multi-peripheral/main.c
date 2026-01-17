// Display app
//
// Write messages to a Newhaven OLED display over SPI

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrf_drv_spi.h"
#include "nrf_twi_mngr.h"

#include "buckler.h"
#include "display.h"
#include "lsm9ds1.h"

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized\n");

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize spi master(controller)
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

    // initialize LSM9DS1 driver
  lsm9ds1_init(&twi_mngr_instance);
  printf("lsm9ds1 initialized\n");

  lsm9ds1_start_gyro_integration();

  printf("Testing IMU \n");

  // initialize display driver
  display_init(&spi_instance);
  printf("Display initialized\n");
  nrf_delay_ms(1000);

  // Write test numbers in a loop
  unsigned int i = 0;
  while(1) {
    // get measurements
    lsm9ds1_measurement_t acc_measurement = lsm9ds1_read_accelerometer();
    lsm9ds1_measurement_t gyr_measurement = lsm9ds1_read_gyro_integration();
    lsm9ds1_measurement_t mag_measurement = lsm9ds1_read_magnetometer();

/*
    float tilt_value = acos(acc_measurement.z_axis/(sqrt(pow(acc_measurement.x_axis, 2) + pow(acc_measurement.y_axis, 2) + pow(acc_measurement.z_axis, 2))));
    tilt_value = tilt_value * 180.0 / M_PI;

    char tilttopbuf [16] = {0};
    char tiltbotbuf [16] = {0};

    if (tilt_value >= 45 || tilt_value <= -45) 
    {
      snprintf(tilttopbuf, 16, "!!! DANGER OF");
      snprintf(tiltbotbuf, 16, "OVERTURN!!!");
    }
    else
    {
      snprintf(tilttopbuf, 16, "Tilt Angle:");
      snprintf(tiltbotbuf, 16, "%f", tilt_value);
    }

    display_write(tilttopbuf, 0);
    display_write(tiltbotbuf, 1);
*/
    
    char acctopbuf[16] = {0};
    char accbotbuf[16] = {0};

    snprintf(acctopbuf, 16, "Ax: %0.3f Ay:", 0);
    snprintf(accbotbuf, 16, "%0.3f Az: %0.3f", 0, 0);
    display_write(acctopbuf, 0);
    display_write(accbotbuf, 1);
    
    nrf_delay_ms(500);

    char gyrtopbuf[16] = {0};
    char gyrbotbuf[16] = {0};
    snprintf(gyrtopbuf, 16, "Gx: %0.3f Gy:", 0);
    snprintf(gyrbotbuf, 16, "%0.3f Gz: %0.3f", 0, 0);
    display_write(gyrtopbuf, 0);
    display_write(gyrbotbuf, 1);
    nrf_delay_ms(500);
    

    i += 7;
    nrf_delay_ms(10);
  }
}

