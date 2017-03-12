/**
 * @file 		lichtrelay.c
 *
 * @brief 		GPIO pin for switching the relay.
 *
 * @details 	This Code implements the logic for turning on and of a simple light
 * @details		with a relay attached to GPIO PIN 19. It is turned on when the Level is low.
 *
 *
 * @author 		Christian Mödlhammer
 *
 * @version 	0.1
 * @date		11.03.2017
 *
 */

/* +++++++++++++++++++++++++++ general includes +++++++++++++++++++++++++++++ */
#include "lichtrelay.h"



/**
 * @brief 	Task for calculating the light barrier logic out of the message queue
 *
 * @param	arg	 none, but needed according to specification
 *
 */

void lichtrelay_task(void* arg)
{
//	uint32_t OnOrOff;
//	uint32_t status=LIGHT_OFF;
	for(;;) {

//	        if (status==LIGHT_OFF) status=LIGHT_ON;
//	        if (status==LIGHT_ON) status=LIGHT_OFF;
//	        gpio_set_level(GPIO_OUTPUT, status);
//	        vTaskDelay(1000 / portTICK_RATE_MS);
//        if(xQueueReceive(lr_evt_queue, &OnOrOff, portMAX_DELAY)) {
//            gpio_set_level(GPIO_OUTPUT, OnOrOff);
//        }
    }
}



/**
 * @brief 	Entry point of the light barrier logic
 *
 * @details 	Here the GPIO Pins are initialized, bound to the interrupt and
 * @details 	a task is created for the calculation logic.
 *
 */

void app_lichtrelay()
{


	gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    //    lr_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    //xTaskCreate(lichtrelay_task, "lichtrelay_task", 1024, NULL, 11, NULL);


    //xQueueSendToBack(lr_evt_queue, &status, portMAX_DELAY);


}

