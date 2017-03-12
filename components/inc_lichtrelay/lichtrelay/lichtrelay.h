#ifndef CHRISTIAN_LICHTRELAY_H
#define CHRISTIAN_LICHTRELAY_H


/* +++++++++++++++++++++++++++ general includes +++++++++++++++++++++++++++++ */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

/* +++++++++++++++++++++++++++ defines +++++++++++++++++++++++++++++ */
#define GPIO_OUTPUT 19
#define GPIO_OUTPUT_PIN_SEL (1<<GPIO_OUTPUT)
#define LIGHT_ON 0
#define LIGHT_OFF 1


/* +++++++++++++++++++++++++++ global queue +++++++++++++++++++++++++++++ */
//static xQueueHandle lr_evt_queue = NULL;


void lichtrelay_task(void* arg);
void app_lichtrelay();


#endif
