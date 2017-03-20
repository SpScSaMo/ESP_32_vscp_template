#ifndef CHRISTIAN_LICHTRELAY_H
#define CHRISTIAN_LICHTRELAY_H


/* +++++++++++++++++++++++++++ defines +++++++++++++++++++++++++++++ */
#define GPIO_OUTPUT 19
#define GPIO_OUTPUT_PIN_SEL (1<<GPIO_OUTPUT)
#define LIGHT_ON 0
#define LIGHT_OFF 1


/* +++++++++++++++++++++++++++ global queue +++++++++++++++++++++++++++++ */
xQueueHandle relay_queue;

/*
 * \brief Task for Relay
 *
 * This function writes to the defined GPIO port as it receives a post in the relay queue
 *
 * */
void lichtrelay_task(void* arg);

/*
 * \brief Main routine for Relay
 *
 * This function initializes the necessary resources for the GPIO write
 *
 * */

void app_lichtrelay();


#endif
