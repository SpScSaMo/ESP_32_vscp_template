#ifndef GERHARD_MILLISECOND_TIMER_H
#define GERHARD_MILLISECOND_TIMER_H

// includes
#include "../../components/HWComp/HWComp/millisekundentimer.h"


//Globale Variablen
extern volatile uint16_t vscp_timer;
extern volatile uint16_t vscp_configtimer;
volatile uint16_t measurement_clock;
volatile uint16_t timeout_clock;

//Functions
void task_millisecond_clock(void* arg);

void init_vscp_millisecond_timer(void);

#endif