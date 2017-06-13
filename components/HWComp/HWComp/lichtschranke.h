#ifndef CHRISTIAN_LICHTSCHRANKE_H
#define CHRISTIAN_LICHTSCHRANKE_H

/* +++++++++++++++++++++++++++ general includes +++++++++++++++++++++++++++++ */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "lichtrelay.h"
#include "millisekundentimer.h"
#include "time.h"
#include <sys/time.h>
#include <inttypes.h>


/* +++++++++++++++++++++++++++ defines +++++++++++++++++++++++++++++ */
#define __STDC_FORMAT_MACROS  // define for use of uint64_t in printf
#define GPIO_INPUT_IO_LB1     25
#define GPIO_INPUT_IO_LB2     26
#define GPIO_INPUT_PIN_SEL  ((1<<GPIO_INPUT_IO_LB1) | (1<<GPIO_INPUT_IO_LB2))
#define ESP_INTR_FLAG_DEFAULT 0
#define DEBUG 0

// ACHTUNG: unbedingt die Masse der Lichtschranke auch mit dem ESP32 verbinden, sonst gibt es Funktionsprobleme!


/* +++++++++++++++++++++++++++ message queue +++++++++++++++++++++++++++++ */
//xQueueHandle lightbarrier_queue; //sends the current count of people on change

/*
 * \brief Task with logic for light barrier
 *
 *	This implements the logic for the light barrier
 *	For test purposes it posts to a relay_Queue. This will be removed when the VSCP is
 *	working and the trigger is given over those messages
 *
 */
void lightbarrier_task(void* arg);

/*
 * \brief config gpio and start light barrier task
 *
 *	This is the Main routine for the light barrier. It configs the defined
 *	GPIOs as Input and starts the light barrier task.
 *
 */
void app_lichtschranke();

#endif
