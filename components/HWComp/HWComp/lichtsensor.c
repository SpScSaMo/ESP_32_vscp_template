/*
 * lichtsensor.c
 *
 *  Created on: Apr 14, 2017
 *      Author: christian
 */



#include "lichtsensor.h"


xSemaphoreHandle print_mux; // Semaphore to handle printf()!
uint8_t sensor_ch0_data_h, sensor_ch0_data_l, sensor_ch1_data_h, sensor_ch1_data_l;

/**
 * @brief Reads values from the sensor
 *
 */
esp_err_t i2c_master_sensor_read(i2c_port_t i2c_num)
{
    /* The following lines start the sensor */
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL2561_ADDR_SEL << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, TSL2561_REGISTER_CONTROL | TSL2561_COMMAND_BIT , ACK_CHECK_EN);
    i2c_master_write_byte(cmd, TSL2561_CONTROL_POWERON, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    int ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ret;
    }
//    xSemaphoreTake(print_mux, portMAX_DELAY);
//    printf("*I2C command with Sensor successful*Return=%d\n",ret);
//    xSemaphoreGive(print_mux);
//    vTaskDelay(1000 / portTICK_RATE_MS);


    /* Read the low byte channel 0 from sensor */
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL2561_ADDR_SEL << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, TSL2561_REGISTER_CHAN0_LOW | TSL2561_COMMAND_BIT, ACK_CHECK_EN);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL2561_ADDR_SEL << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &sensor_ch0_data_l, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }

    /* Read the high byte channel 0 from sensor */
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL2561_ADDR_SEL << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, TSL2561_REGISTER_CHAN0_HIGH | TSL2561_COMMAND_BIT, ACK_CHECK_EN);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL2561_ADDR_SEL << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &sensor_ch0_data_h, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }

    /* Read the low byte channel 1 from sensor */
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL2561_ADDR_SEL << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, TSL2561_REGISTER_CHAN1_LOW | TSL2561_COMMAND_BIT, ACK_CHECK_EN);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL2561_ADDR_SEL << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &sensor_ch1_data_l, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }


    /* Read the high byte channel 1 from sensor */
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL2561_ADDR_SEL << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, TSL2561_REGISTER_CHAN1_HIGH | TSL2561_COMMAND_BIT, ACK_CHECK_EN);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TSL2561_ADDR_SEL << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &sensor_ch1_data_h, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 2000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

/**
 * @brief i2c master initialization
 */
void i2c_master_init()
{
	int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

/*
 * Function:  calculateLux
 * --------------------
 * This method, adapted from the Adafruit arduino libraries,
 * takes the current channel0 and channel1 values and turns them into lux measurements.
 *
 * This method was originally in the datasheet, but MR. Townsend did a great job here.
 *
 * source: https://github.com/DinsFire64/TSL2561_ESP8266/blob/master/driver/tsl2561.c
 */

uint32_t calculateLux(uint16_t ch0, uint16_t ch1) {
	unsigned long chScale;
	unsigned long channel1;
	unsigned long channel0;

	chScale = (1 << TSL2561_LUX_CHSCALE);

	// Scale for gain (1x or 16x)
	//if (!_gain) chScale = chScale << 4;
	chScale = chScale << 4;

	// scale the channel values
	channel0 = (ch0 * chScale) >> TSL2561_LUX_CHSCALE;
	channel1 = (ch1 * chScale) >> TSL2561_LUX_CHSCALE;

	// find the ratio of the channel values (Channel1/Channel0)
	unsigned long ratio1 = 0;
	if (channel0 != 0)
		ratio1 = (channel1 << (TSL2561_LUX_RATIOSCALE + 1)) / channel0;

	// round the ratio value
	unsigned long ratio = (ratio1 + 1) >> 1;

	unsigned int b=0;
	unsigned int m=0;

	if ((ratio <= TSL2561_LUX_K1T)) {
		b = TSL2561_LUX_B1T;
		m = TSL2561_LUX_M1T;
	} else if (ratio <= TSL2561_LUX_K2T) {
		b = TSL2561_LUX_B2T;
		m = TSL2561_LUX_M2T;
	} else if (ratio <= TSL2561_LUX_K3T) {
		b = TSL2561_LUX_B3T;
		m = TSL2561_LUX_M3T;
	} else if (ratio <= TSL2561_LUX_K4T) {
		b = TSL2561_LUX_B4T;
		m = TSL2561_LUX_M4T;
	} else if (ratio <= TSL2561_LUX_K5T) {
		b = TSL2561_LUX_B5T;
		m = TSL2561_LUX_M5T;
	} else if (ratio <= TSL2561_LUX_K6T) {
		b = TSL2561_LUX_B6T;
		m = TSL2561_LUX_M6T;
	} else if (ratio <= TSL2561_LUX_K7T) {
		b = TSL2561_LUX_B7T;
		m = TSL2561_LUX_M7T;
	} else if (ratio > TSL2561_LUX_K8T) {
		b = TSL2561_LUX_B8T;
		m = TSL2561_LUX_M8T;
	}

	unsigned long temp;
	temp = ((channel0 * b) - (channel1 * m));

	// do not allow negative lux value --> Unsigned can never be less than 0
//	if (temp < 0)
//		temp = 0;

	// round lsb (2^(LUX_SCALE-1))
	temp += (1 << (TSL2561_LUX_LUXSCALE - 1));

	// strip off fractional portion
	uint32_t lux = temp >> TSL2561_LUX_LUXSCALE;

	// Signal I2C had no errors
	return lux;
}

void i2c_read_task(void* arg)
{
    int ret;
    uint32_t task_idx = (uint32_t) arg;
    uint32_t luxwert;

    while (1) {
        ret = i2c_master_sensor_read(I2C_MASTER_NUM);
        xSemaphoreTake(print_mux, portMAX_DELAY);

        if (ret == ESP_OK) {
//            printf("ch0:data_h: %02x\n", sensor_ch0_data_h);
//            printf("ch0:data_l: %02x\n", sensor_ch0_data_l);
//            printf("ch0:sensor val: %04x\n", (uint16_t)( sensor_ch0_data_h << 8 | sensor_ch0_data_l ));
//            printf("ch1:data_h: %02x\n", sensor_ch1_data_h);
//            printf("ch1:data_l: %02x\n", sensor_ch1_data_l);
//            printf("ch1:sensor val: %04x\n", (uint16_t)( sensor_ch1_data_h << 8 | sensor_ch1_data_l ));
            vTaskDelay( 2000 / portTICK_RATE_MS);
            luxwert=calculateLux((uint16_t)( sensor_ch0_data_h << 8 | sensor_ch0_data_l ),(uint16_t)( sensor_ch1_data_h << 8 | sensor_ch1_data_l ));
            printf("lux: %d\n", luxwert);
        	xQueueSendToBack(lichtsensor_queue, &luxwert, 0);
        } else {
            printf("No ack, sensor not connected...skip...\n");
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(( 1000 * ( task_idx + 1 ) ) / portTICK_RATE_MS);

    }
}


void app_lichtsensor(void)
{
	print_mux = xSemaphoreCreateMutex();

    i2c_master_init();
    //create a queue to handle gpio event from isr
    lichtsensor_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(i2c_read_task, "i2c_read_task", 1024 * 2, NULL, 10, NULL);
}


