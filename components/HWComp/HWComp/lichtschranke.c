/**
 * @file 		lichtschranke.c
 *
 * @brief 		GPIO Interrupt for light barrier.
 *
 * @details 	This Code implements the logic for the light barrier.
 * @details		There are two light barriers in serial. One of them is attached to GPIO 2,
 * @details		the other one is attached to GPIO 4. On both GPIOs there is an interrupt
 * @details		triggered on each edge of the signal, that means when someone interrupts the
 * @details		light barrier and also when the light barrier is left.
 *
 *
 * @author 		Christian Mödlhammer
 *

 * @version 	0.1
 * @date		07.03.2017
 *
 */

#include "lichtschranke.h"
#include "../../../main/config.h"

/**
 * @brief 	Task for calculating the light barrier logic out of the message queue
 *
 * @param	arg	 none, but needed according to specification
 *
 */

void ligthbarrier_task(void* arg)
{
	uint32_t some_irrelevant_value=0;
	uint32_t firstLightBarrier=0;
	uint32_t lastLightBarrier=0;
	uint32_t io_value_LB1=0;
	uint32_t io_value_LB2=0;
	uint32_t timecounter=0;
	uint32_t personen=0;
	uint32_t firstSet=0;
	uint32_t relaystatus=0;
    messageparameters mparalb;

	while (1){

		// this gives us a trigger every 50 milliseconds
		xQueueReceive(ms_50_queue, &some_irrelevant_value, portMAX_DELAY);

		// then we read the values of both light barriers
		io_value_LB1=gpio_get_level(GPIO_INPUT_IO_LB1);
		io_value_LB2=gpio_get_level(GPIO_INPUT_IO_LB2);

		// check whether a light barrier was interrupted
		if (((io_value_LB1==0)||(io_value_LB2==0))&&(firstSet==0)) {
			if (io_value_LB1==0) firstLightBarrier=GPIO_INPUT_IO_LB1;
			if (io_value_LB2==0) firstLightBarrier=GPIO_INPUT_IO_LB2;
			if (DEBUG==1) printf("firstLB: %d\n",firstLightBarrier);
			firstSet=1;
			while (firstSet==1){
				// this gives us a trigger every 50 milliseconds
				xQueueReceive(ms_50_queue, &some_irrelevant_value, portMAX_DELAY);

				// then we read the values of both light barriers
				io_value_LB1=gpio_get_level(GPIO_INPUT_IO_LB1);
				if (DEBUG==1) printf("io_value_LB1: %d\n",io_value_LB1);
				io_value_LB2=gpio_get_level(GPIO_INPUT_IO_LB2);
				if (DEBUG==1) printf("io_value_LB2: %d\n",io_value_LB2);

				if ((io_value_LB1==1)&&(io_value_LB2==0)) lastLightBarrier=GPIO_INPUT_IO_LB2;
				if ((io_value_LB1==0)&&(io_value_LB2==1)) lastLightBarrier=GPIO_INPUT_IO_LB1;

				if (DEBUG==1) printf("lastLB: %d\n",lastLightBarrier);

				if ((io_value_LB1==1)&&(io_value_LB2==1)) {

					timecounter=0;

					while (timecounter<20){
						//wait for 50ms signal
						xQueueReceive(ms_50_queue, &some_irrelevant_value, portMAX_DELAY);
						io_value_LB1=gpio_get_level(GPIO_INPUT_IO_LB1);
						io_value_LB2=gpio_get_level(GPIO_INPUT_IO_LB2);
						if ((io_value_LB1==0)||(io_value_LB2==0)) break;
						timecounter++;
					}

					if (DEBUG==1) printf("TC on exit: %d\n",timecounter);


					// here the counting is executed, after there was no change for 1 second
					if (timecounter==20){
						if ((firstLightBarrier==GPIO_INPUT_IO_LB1)&&(lastLightBarrier==GPIO_INPUT_IO_LB2)) personen++;
						if ((firstLightBarrier==GPIO_INPUT_IO_LB2)&&(lastLightBarrier==GPIO_INPUT_IO_LB1)){
							if (personen==0) personen=0;
							else personen--;
						}
						firstLightBarrier=0;
						lastLightBarrier=0;
						firstSet=0;
						printf("Personen im Raum: %d\n",personen);

						// Anzahl der Personen in die Queue stellen
			            strcpy(mparalb.type,CH2);
			            strcpy(mparalb.measurementtype,"per");
			            mparalb.value=personen;
			        	xQueueSendToBack(sensor_queue, &mparalb, 0);

						// für das Relay an die Queue senden!
						if (personen>1) relaystatus=LIGHT_ON;
						if (personen<1) relaystatus=LIGHT_OFF;
						xQueueSendToBack(relay_queue, &relaystatus, 0);
					}

				}

			}
		}


	}
}


/**
 * @brief 	Entry point of the light barrier logic
 *
 * @details 	Here the GPIO Pins are initialized, bound to the interrupt and
 * @details 	a task is created for the calculation logic.
 *
 */

void app_lichtschranke(void)
{

	gpio_config_t io_conf;

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //bit mask of the pins, use GPIO25/26 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-down mode
    io_conf.pull_down_en = 1;
    gpio_config(&io_conf);
    //start light barrier task

    //    • pvTaskCode – Pointer to the task function. In C programming, we can simply
	//    supply the name of a function or, as has been seen in some samples, the
	//    address of the name of the function. Apparently these equate to items that are
	//    close enough to be used interchangablly.
	//    • pcName – Debugging name of the task.
	//    • usStackDepth – Size of the stack for the task.
	//    • pvParameters – Parameters for the task instance. This may be NULL.
	//    • uxPriority – Priority of the task instance.
	//    • xTaskHandle – Reference to the newly created task instance. This may be
	//    passed in as NULL if no task handle is required to be returned.

    xTaskCreate(ligthbarrier_task, "ligthbarrier_task", 2048, NULL, 9, NULL);

}

