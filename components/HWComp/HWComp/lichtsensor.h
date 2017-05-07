/*
 * lichtsensor.h
 *
 *  Created on: Apr 14, 2017
 *      Author: christian
 */

#ifndef MAIN_LICHTSENSOR_H_
#define MAIN_LICHTSENSOR_H_

/****************  INCLUDES for the light sensor and I2C ********************/
#include <stdio.h>
#include <stdlib.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "sdkconfig.h"


/**********************  Defines ********************************/

#define I2C_MASTER_SCL_IO    32    /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO    33    /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM 		 I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ   100000     /*!< I2C master clock frequency */

#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */


#define TSL2561_ADDR_L			  0x29 //GND
#define TSL2561_ADDR_F			  0x39 //FLOATING
#define TSL2561_ADDR_H 			  0x49 //VCC
#define TSL2561_ADDR_SEL		  TSL2561_ADDR_F //Selected Address

#define TSL2561_COMMAND_BIT       0x80    // Must be 1
#define TSL2561_CLEAR_BIT         0x40    // Clears any pending interrupt (write 1 to clear)
#define TSL2561_WORD_BIT          0x20    // 1 = read/write word (rather than byte)
#define TSL2561_BLOCK_BIT         0x10    // 1 = using block read/write


//Registers
#define TSL2561_REGISTER_CONTROL			0x00
#define TSL2561_REGISTER_TIMING            	0x01
#define TSL2561_REGISTER_THRESHHOLDL_LOW  	0x02
#define TSL2561_REGISTER_THRESHHOLDL_HIGH 	0x03
#define TSL2561_REGISTER_THRESHHOLDH_LOW   	0x04
#define TSL2561_REGISTER_THRESHHOLDH_HIGH  	0x05
#define TSL2561_REGISTER_INTERRUPT         	0x06
#define TSL2561_REGISTER_CRC               	0x08
#define TSL2561_REGISTER_ID            	    0x0A
#define TSL2561_REGISTER_CHAN0_LOW         	0x0C
#define TSL2561_REGISTER_CHAN0_HIGH        	0x0D
#define TSL2561_REGISTER_CHAN1_LOW         	0x0E
#define TSL2561_REGISTER_CHAN1_HIGH        	0x0F

#define TSL2561_CONTROL_POWERON   0x03
#define TSL2561_CONTROL_POWEROFF  0x00

#define TSL2561_INTEG_13_7MS	  0x00
#define TSL2561_INTEG_101MS		  0x01
#define TSL2561_INTEG_402MS		  0x02
#define TSL2561_INTEG_MANUAL	  0x03


//Constants from Adafruit libraries.

#define TSL2561_LUX_LUXSCALE      (14)      // Scale by 2^14
#define TSL2561_LUX_RATIOSCALE    (9)       // Scale ratio by 2^9
#define TSL2561_LUX_CHSCALE       (10)      // Scale channel values by 2^10
#define TSL2561_LUX_CHSCALE_TINT0 (0x7517)  // 322/11 * 2^TSL2561_LUX_CHSCALE
#define TSL2561_LUX_CHSCALE_TINT1 (0x0FE7)  // 322/81 * 2^TSL2561_LUX_CHSCALE

#define TSL2561_LUX_K1T           (0x0040)  // 0.125 * 2^RATIO_SCALE
#define TSL2561_LUX_B1T           (0x01f2)  // 0.0304 * 2^LUX_SCALE
#define TSL2561_LUX_M1T           (0x01be)  // 0.0272 * 2^LUX_SCALE
#define TSL2561_LUX_K2T           (0x0080)  // 0.250 * 2^RATIO_SCALE
#define TSL2561_LUX_B2T           (0x0214)  // 0.0325 * 2^LUX_SCALE
#define TSL2561_LUX_M2T           (0x02d1)  // 0.0440 * 2^LUX_SCALE
#define TSL2561_LUX_K3T           (0x00c0)  // 0.375 * 2^RATIO_SCALE
#define TSL2561_LUX_B3T           (0x023f)  // 0.0351 * 2^LUX_SCALE
#define TSL2561_LUX_M3T           (0x037b)  // 0.0544 * 2^LUX_SCALE
#define TSL2561_LUX_K4T           (0x0100)  // 0.50 * 2^RATIO_SCALE
#define TSL2561_LUX_B4T           (0x0270)  // 0.0381 * 2^LUX_SCALE
#define TSL2561_LUX_M4T           (0x03fe)  // 0.0624 * 2^LUX_SCALE
#define TSL2561_LUX_K5T           (0x0138)  // 0.61 * 2^RATIO_SCALE
#define TSL2561_LUX_B5T           (0x016f)  // 0.0224 * 2^LUX_SCALE
#define TSL2561_LUX_M5T           (0x01fc)  // 0.0310 * 2^LUX_SCALE
#define TSL2561_LUX_K6T           (0x019a)  // 0.80 * 2^RATIO_SCALE
#define TSL2561_LUX_B6T           (0x00d2)  // 0.0128 * 2^LUX_SCALE
#define TSL2561_LUX_M6T           (0x00fb)  // 0.0153 * 2^LUX_SCALE
#define TSL2561_LUX_K7T           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B7T           (0x0018)  // 0.00146 * 2^LUX_SCALE
#define TSL2561_LUX_M7T           (0x0012)  // 0.00112 * 2^LUX_SCALE
#define TSL2561_LUX_K8T           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B8T           (0x0000)  // 0.000 * 2^LUX_SCALE
#define TSL2561_LUX_M8T 		  (0x0000) // 0.000 * 2^LUX_SCALE


/****************  Prototypes **********************************************/



esp_err_t i2c_master_sensor_read(i2c_port_t i2c_num);
void i2c_master_init();
void i2c_read_task(void* arg);
uint32_t calculateLux(uint16_t ch0, uint16_t ch1);
void app_lichtsensor(void); // The main task for I2C light sensor TSL2561



#endif /* MAIN_LICHTSENSOR_H_ */
