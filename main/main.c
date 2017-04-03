//##################################################################
//Projekt: VSCP Very Simple Control Protokoll implementation ESP32 #
//##################################################################
//
// This Project implements the VSCP over Ethernet on the ESP32 Thing
//
//
//Authors:  Gerhard Sabeditsch, Juergen Schoener, Juergen Spandl,
//          Christian Moedelhammer
//
//Version: 1.0
//
//##################################################################

//******************************************************************
// STANDARD INCLUDES - ESP32 - THING
//******************************************************************
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//******************************************************************
// SPECIAL INCLUDES - ESP32 - THING
//******************************************************************

#include "esp_err.h"            //for error handling
#include "esp_intr_alloc.h"     //is used for interrupts
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



//******************************************************************
// DEFINES - ESP32 - THING
//******************************************************************


//Everything for Timer Implementation
#define TIMER_INTR_SEL TIMER_INTR_LEVEL  /*!< Timer level interrupt */
#define TIMER_GROUP    TIMER_GROUP_0     /*!< Test on timer group 0 */
#define TIMER_DIVIDER   16               /*!< Hardware timer clock divider */
#define TIMER_SCALE    (TIMER_BASE_CLK / TIMER_DIVIDER)  /*!< used to calculate counter value */
#define TIMER_FINE_ADJ   (1.4*(TIMER_BASE_CLK / TIMER_DIVIDER)/1000000) /*!< used to compensate alarm value */
#define TIMER_INTERVAL0_SEC   (3.4179)   /*!< test interval for timer 0 */
#define TIMER_INTERVAL1_SEC   (5.78)   /*!< test interval for timer 1 */
#define TEST_WITHOUT_RELOAD   0   /*!< example of auto-reload mode */
#define TEST_WITH_RELOAD   1      /*!< example without auto-reload mode */
#define ESP_INTR_FLAG_DEFAULT 0
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//******************************************************************
// SPECIAL INCLUDES - ESP32 - VSCP Very Simple Control Protokoll
//******************************************************************
#include <string.h>
#include <stdlib.h>
#include "vscp_class.h"
#include "vscp_type.h"
#include "vscp_firmware.h"
#include "vscp_millisecond_timer.h"

//******************************************************************
// SPECIAL INCLUDES - ESP32 - HW Lichtschranke + Lichtrelay
//******************************************************************
#include "lichtschranke.h"
#include "lichtrelay.h"
#include "millisekundentimer.h"


//******************************************************************
// Prototypes - ESP32 - THING - Overview ;-)
//******************************************************************

esp_err_t event_handler(void *ctx, system_event_t *event);
void init_button0(void);

/* +++++++++++++++++++++++++++ śtruct for timevalue (for test porposes) +++++++++++++++++++++++++++++ */
struct timeval tv = { .tv_sec = 0, .tv_usec = 0 };   /* btw settimeofday() is helpfull here too*/



/* error Handling
 *
 */
esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

/* Button 0 ISR
 *
 * This is the ISR for Button 0 on falling edge
 *
 * \author: Christian Mödlhammer
 */
void IRAM_ATTR btn_isr_handler(void* arg)
{
	uint32_t sec, us;
	uint64_t ts,ts_init,ts_next;

	// Null für den Ausgangswert
	vscp_initbtncnt=0;

	if (gpio_get_level(GPIO_NUM_0)==0){

		gettimeofday(&tv, NULL);
			(sec) = tv.tv_sec;
			(us) = tv.tv_usec;
			(ts) = (uint64_t)(sec*1000000+us);
			ts_init=ts;
			ts_next=ts;
			while ((ts_next-ts_init)<250000){ // wait for 250 milliseconds
				gettimeofday(&tv, NULL);
				(sec) = tv.tv_sec;
				(us) = tv.tv_usec;
				(ts) = (uint64_t)(sec*1000000+us);
				ts_next=ts;
			}

			//if button pressed for more than 250 milliseconds
			if (gpio_get_level(GPIO_NUM_0)==0) vscp_initbtncnt=251;

	}

}

/*
 * Button init
 *
 * initiatlizes the button of the ESP 32 on GPIO 0 with an interrupt
 *
 * \author Christian M.
 *
 */
void init_button(void){

	gpio_set_pull_mode(GPIO_NUM_0,GPIO_PULLUP_ONLY);
	gpio_set_intr_type(GPIO_NUM_0, GPIO_INTR_ANYEDGE);
	gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_0, btn_isr_handler, (void*) GPIO_NUM_0);


}


/* app_main()
 *
 *  This Function is the Main Funktion of the Project
 *
 */
void app_main(void)
{


	// Start HW Components and millisecond timer
	app_timer(); // starting the timer with the queues 1 ms and 50 ms
	app_lichtschranke(); // starting the lightbarrier logic
    app_lichtrelay(); // starting the lightrelay logic


    //--VSCP------------------------//
    init_vscp_millisecond_timer();
    init_button(); // initializes Button and interrupt

    //

    //++++++++++++++++++++++++++++++
    
    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
            .ssid = "openHAB",
            .password = "openHABtest",
            .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );

    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);

    int level = 0;

    while (true) {
    	gpio_set_level(GPIO_NUM_5, level);
        level = !level;
        vTaskDelay(300 / portTICK_PERIOD_MS);
        printf("Value of vscp_initbtncnt: %d\n", vscp_initbtncnt);
    }
}



