/*
 * \file: millisekundentimer.c
 *
 * \author: Christian Mödlhammer
 * \date: 12.03.2017
 *
 * \brief Implementiert einen Timer der alle 1 ms interruptgesteuert auslöst
 *
 * Das Programm wird basierend auf der bereits bestehenden Timerroutine aus dem Examples erstellt.
 * Der Timer postet über eine Queue aus dem Task heraus alle definierten Intervalle.
 * Diese Queues können in anderen Routinen verwendet werden indem dort diese Queues abgefragt werden.
 *
 */
#include "millisekundentimer.h"

// +++++++++++++++++++++ defines für Timer Routinen +++++++++++++++++++++++++++++++++++++
#define TIMER_INTR_SEL TIMER_INTR_LEVEL  /*!< Timer level interrupt */
#define TIMER_GROUP    TIMER_GROUP_0     /*!< Test on timer group 0 */
#define TIMER_DIVIDER   4	              /*!< Hardware timer clock divider */
#define TIMER_SCALE    (TIMER_BASE_CLK / TIMER_DIVIDER)  /*!< used to calculate counter value */
#define TIMER_INTERVAL1_SEC   (0.001)   /*!< test interval for timer 1 */


void timer_evt_task(void *arg)
{
	static uint64_t mscount50=0;
	double time;

	while(1) {
			xQueueReceive(timer_queue, &time, portMAX_DELAY);

	        xQueueSendToBack(ms_1_queue, &time, 0);

	        if (mscount50==49) {
	        	xQueueSendToBack(ms_50_queue, &time, 0);
	        	mscount50=0;
	        }
	        else{
	        	mscount50++;
	        }
	}
}

/*
 * @brief timer group0 ISR handler
 */
void IRAM_ATTR timer_group0_isr(void *para)
{
    int timer_idx = (int) para;
    double time=0;

        /*Timer1 is an example that will reload counter value*/
        TIMERG0.hw_timer[timer_idx].update = 1;
        TIMERG0.int_clr_timers.t1 = 1;
        xQueueSendFromISR(timer_queue, &time, NULL);
        /*For a auto-reload timer, we still need to set alarm_en bit if we want to enable alarm again.*/
        TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;
//    }
}


/*
 * \brief initialize timer group0 hardware timer1
 *
 */
void tg0_timer1_init()
{
    int timer_group = TIMER_GROUP_0;
    int timer_idx = TIMER_1;
    timer_config_t config;
    config.alarm_en = 1;
    config.auto_reload = 1;
    config.counter_dir = TIMER_COUNT_UP;
    config.divider = TIMER_DIVIDER;
    config.intr_type = TIMER_INTR_SEL;
    config.counter_en = TIMER_PAUSE;
    /*Configure timer*/
    timer_init(timer_group, timer_idx, &config);
    /*Stop timer counter*/
    timer_pause(timer_group, timer_idx);
    /*Load counter value */
    timer_set_counter_value(timer_group, timer_idx, 0x00000000ULL);
    /*Set alarm value*/
    timer_set_alarm_value(timer_group, timer_idx, TIMER_INTERVAL1_SEC * TIMER_SCALE);
    /*Enable timer interrupt*/
    timer_enable_intr(timer_group, timer_idx);
    /*Set ISR handler*/
    timer_isr_register(timer_group, timer_idx, timer_group0_isr, (void*) timer_idx, ESP_INTR_FLAG_IRAM, NULL);
    /*Start timer counter*/
    timer_start(timer_group, timer_idx);
}

/**
 * \ Setup of Millisekundentimer (Main routine)
 *
 * brief creates queues and task for the millisekundentimer
 */
void app_timer()
{
	timer_queue = xQueueCreate(10, sizeof(double));
    ms_1_queue = xQueueCreate(10, sizeof(double));
    ms_50_queue = xQueueCreate(10, sizeof(double));

    tg0_timer1_init();

    xTaskCreate(timer_evt_task, "timer_evt_task", 2048, NULL, 5, NULL);
}
