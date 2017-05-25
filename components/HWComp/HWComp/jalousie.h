#ifndef CHRISTIAN_JALOUSIE_H
#define CHRISTIAN_JALOUSIE_H


/* +++++++++++++++++++++++++++ general includes +++++++++++++++++++++++++++++ */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "millisekundentimer.h"

/* +++++++++++++++++++++++++++ defines +++++++++++++++++++++++++++++ */
#define GPIO_OUTPUT_UP 12
#define GPIO_OUTPUT_DOWN 13
#define GPIO_OUTPUT_POWER 14
#define GPIO_OUTPUT_PIN_SEL_JAL ((1<<GPIO_OUTPUT_UP)|(1<<GPIO_OUTPUT_DOWN)|(1<<GPIO_OUTPUT_POWER))

#define GPIO_OFF	1
#define GPIO_ON		0

#define JALOUSIE_OFF	0
#define JALOUSIE_ON		1
#define JALOUSIE_UP		2
#define JALOUSIE_DOWN	3

#define TIMEOUT	5000 // turn automatically of after this amount of milliseconds

/* +++++++++++++++++++++++++++ global queue +++++++++++++++++++++++++++++ */
xQueueHandle jalousie_queue;

/*
 * \brief Task for Jalousie
 *
 * This function writes to the defined GPIO port as it receives a post in the jalousie_queue
 *
 * */
void jalousie_task(void* arg);

/*
 * \brief Main routine for Relay
 *
 * This function initializes the necessary resources for the GPIO write
 *
 * */

void app_jalousie();


#endif
