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

/* +++++++++++++++++++++++++++ general includes +++++++++++++++++++++++++++++ */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "lichtschranke.h"
#include "lichtrelay.h"
#include "time.h"
#include <sys/time.h>
#include <inttypes.h>


/* +++++++++++++++++++++++++++ defines +++++++++++++++++++++++++++++ */
#define __STDC_FORMAT_MACROS  // define for use of uint64_t in printf
#define GPIO_INPUT_IO_0     25
#define GPIO_INPUT_IO_1     26
#define GPIO_INPUT_PIN_SEL  ((1<<GPIO_INPUT_IO_0) | (1<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

/* +++++++++++++++++++++++++++ global queue +++++++++++++++++++++++++++++ */
static xQueueHandle gpio_evt_queue = NULL;

/* +++++++++++++++++++++++++++ śtruct for timevalue +++++++++++++++++++++++++++++ */
struct timeval tv = { .tv_sec = 0, .tv_usec = 0 };   /* btw settimeofday() is helpfull here too*/

/**
 * @brief 	Interrupt Service routine for GPIO 2 or 4 trigger on rising or falling edge
 *
 * @param	arg	 GPIO Port that triggered an interrupt
 *
 */

void IRAM_ATTR lb_isr_handler(void* arg)
{
	uint32_t gpio_num = (uint32_t) arg;
	uint32_t sec, us;
	uint64_t ts,ts_init,ts_next;

	gettimeofday(&tv, NULL);
	(sec) = tv.tv_sec;
	(us) = tv.tv_usec;
	(ts) = (uint64_t)(sec*1000000+us);
	ts_init=ts;
	ts_next=ts;
	while ((ts_next-ts_init)<20000){ // wait for 20 milliseconds
		gettimeofday(&tv, NULL);
		(sec) = tv.tv_sec;
		(us) = tv.tv_usec;
		(ts) = (uint64_t)(sec*1000000+us);
		ts_next=ts;
	}

	xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL); // post to queue
}


/**
 * @brief 	Task for calculating the light barrier logic out of the message queue
 *
 * @param	arg	 none, but needed according to specification
 *
 */

void ligthbarrier_task(void* arg)
{
    uint32_t io_num;
	static uint32_t personen=0;
    uint32_t firstLB, lastLB, firstLev, lastLev;

	// START the following code is for test purposes
    uint32_t sec, us;
	uint64_t lgts, actts;
	// END the following code is for test purposes
	(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY));
    for(;;) {
    	printf("Anfang: GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));

        firstLB=io_num;
            firstLev=gpio_get_level(io_num);
            if ((firstLB==GPIO_INPUT_IO_0)&&(firstLev==0)){
            	lgts=0;
            	while(1){
            		(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY));
                	printf("in While: GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));

            		gettimeofday(&tv, NULL);
            		(sec) = tv.tv_sec;
            		(us) = tv.tv_usec;
		       		(actts) = (uint64_t)(sec*1000000+us);
				    if ((lgts!=0)&&((actts-lgts)>500000)) break;
			        (lgts) = (uint64_t)(sec*1000000+us);
		            lastLB=io_num;
		            lastLev=gpio_get_level(io_num);
		    	}
            	if ((lastLB==GPIO_INPUT_IO_1)&&(lastLev==1)) printf("Personen: %d\n", ++personen);

            }
            if ((firstLB==GPIO_INPUT_IO_1)&&(firstLev==0)){
            	lgts=0;
            	while(1){
            		(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY));
                	printf("in While: GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));

            		gettimeofday(&tv, NULL);
		            (sec) = tv.tv_sec;
				    (us) = tv.tv_usec;
		       		(actts) = (uint64_t)(sec*1000000+us);
				    if ((lgts!=0)&&((actts-lgts)>500000)) break;
			        (lgts) = (uint64_t)(sec*1000000+us);
		            lastLB=io_num;
		            lastLev=gpio_get_level(io_num);
            	}
            	if ((lastLB==GPIO_INPUT_IO_0)&&(lastLev==1)) printf("Personen: %d\n", --personen);


  //      	printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
    		// START the following code is for test purposes
//            gettimeofday(&tv, NULL);
//    		(sec) = tv.tv_sec;
//    		(us) = tv.tv_usec;
//    		(ts) = (uint64_t)(sec*1000000+us);
//            printf("Timestamp: %" PRIu64 "\n", ts);
    		// END the following code is for test purposes

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

void app_lichtschranke()
{
    gpio_config_t io_conf;

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    //bit mask of the pins, use GPIO2/4 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-down mode
    io_conf.pull_down_en = 1;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
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

    xTaskCreate(ligthbarrier_task, "ligthbarrier_task", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, lb_isr_handler, (void*) GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, lb_isr_handler, (void*) GPIO_INPUT_IO_1);
}

