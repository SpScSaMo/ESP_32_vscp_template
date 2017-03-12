/*
 * millisekundentimer.h
 *
 *  Created on: Mar 13, 2017
 *      Author: christian
 */

#ifndef MAIN_MILLISEKUNDENTIMER_H_
#define MAIN_MILLISEKUNDENTIMER_H_

// includes
#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "time.h"
#include <sys/time.h>
#include <inttypes.h>


// functions
void timer_evt_task(void *arg);
void IRAM_ATTR timer_group0_isr(void *para);
void tg0_timer1_init();
void app_timer();

#endif /* MAIN_MILLISEKUNDENTIMER_H_ */
