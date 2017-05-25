/**
 * \file 		jalousie.c
 *
 * \brief 		GPIO pin for switching the relay for the jalousie.

 *
 * \author 		Christian Mödlhammer
 *
 * \version 	0.1
 * \date		25.05.2017
 *
 */



#include "jalousie.h"

/**
 * \brief 	Task for the control logic of the Jalousie
 *
 * \details triggered by jalousie_queue and the values 0 for off, 2 for up and 3 for down
 *
 * @param	arg	 none, but needed according to specification
 *
 */

void jalousie_task(void* arg)
{
	uint32_t UpOrDown=99; // this is the default value
	uint32_t some_irrelevant_value=0;
	uint32_t timecount;

	//initially turn off
	gpio_set_level(GPIO_OUTPUT_POWER, GPIO_OFF);
	gpio_set_level(GPIO_OUTPUT_UP, GPIO_OFF);
	gpio_set_level(GPIO_OUTPUT_DOWN, GPIO_OFF);

	while (1) {
		if (xQueueReceive(jalousie_queue, &UpOrDown, 0)==pdTRUE){
			printf("Received %d from jalousie_queue\n",UpOrDown); //for debugging
		}


		if (UpOrDown==JALOUSIE_UP) {
			gpio_set_level(GPIO_OUTPUT_UP, GPIO_ON);
			gpio_set_level(GPIO_OUTPUT_DOWN, GPIO_OFF);
			gpio_set_level(GPIO_OUTPUT_POWER, GPIO_ON);

		}
		if (UpOrDown==JALOUSIE_DOWN) {
			gpio_set_level(GPIO_OUTPUT_UP, GPIO_OFF);
			gpio_set_level(GPIO_OUTPUT_DOWN, GPIO_ON);
			gpio_set_level(GPIO_OUTPUT_POWER, GPIO_ON);

		}

	xQueueReceive(ms_50_queue, &some_irrelevant_value, portMAX_DELAY);
	timecount+=50; // 50 ms werden zu timecount addiert
	if (UpOrDown==JALOUSIE_OFF) timecount=TIMEOUT; // damit wird ausgeschaltet
	if (timecount>=TIMEOUT) {  // nach definierter Zeit in ms wird abgeschaltet
		gpio_set_level(GPIO_OUTPUT_POWER, GPIO_OFF);
		UpOrDown=99;
		timecount=0;
	}
    }
}



/**
 * \brief 	Entry point of the jalousie control
 *
 * \details 	Here the GPIO Pins are initialized
 * \details 	a task is created for the control logic.
 *
 */

void app_jalousie()
{


	gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL_JAL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 1;
    //configure GPIO with the given settings
    gpio_config(&io_conf);


    //create a queue to handle gpio event from isr
    jalousie_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(jalousie_task, "jalousie_task", 2048, NULL, 9, NULL);

}

