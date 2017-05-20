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
#include "config.h"				/* this has the defines for all the sensors
								 and actors of the esp32_Thing */
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
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//******************************************************************
// SPECIAL INCLUDES - ESP32 - VSCP Very Simple Control Protokoll
//******************************************************************
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vscp_class.h"
#include "vscp_type.h"
#include "vscp_firmware.h"
#include "vscp_millisecond_timer.h"

//******************************************************************
// SPECIAL INCLUDES - ESP32 - HW Lichtschranke + Lichtrelay
//******************************************************************
#include "lichtschranke.h"
#include "lichtsensor.h"
#include "lichtrelay.h"
#include "millisekundentimer.h"


//******************************************************************
// INCLUDES - FOR TCP Connection
//******************************************************************
//#include <stdio.h>
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <math.h>
#include <sys/time.h>
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "errno.h"
#include "string.h"
//#include "error.h"




static const char *TAG = "CLIENTE";

//#define DEBUG(...) ESP_LOGD(TAG,__VA_ARGS__);
#define INFO(...) ESP_LOGI(TAG,__VA_ARGS__);
#define ERROR(...) ESP_LOGE(TAG,__VA_ARGS__);

#define delay_ms(ms) vTaskDelay((ms) / portTICK_RATE_MS)



#define LEN_DATA 512

volatile char mydata[LEN_DATA];

//volatile char mydata[] = "TestData:12345";//datos de este dispositivo

static EventGroupHandle_t wifi_event_group; //manejo de banderas de conexiones

const int STA_CONNECTED_BIT = BIT0;// bandera de conexion sta


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//******************************************************************
// Prototypes - ESP32 - THING - Overview ;-)
//******************************************************************


void IRAM_ATTR btn0_isr_handler(void* arg);
void init_button0(void);

/* +++++++++++++++++++++++++++ śtruct for timevalue (for test porposes) +++++++++++++++++++++++++++++ */
struct timeval tv = { .tv_sec = 0, .tv_usec = 0 };   /* btw settimeofday() is helpfull here too*/



/**
 * WIFI Event Handler, falls die Verbindung abreist oder so ;-)
 */
static esp_err_t event_handler(void *ctx, system_event_t *event){
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        printf("SYSTEM_EVENT_STA_START\n");
		esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
    	printf("SYSTEM_EVENT_STA_GOT_IP\n");
    	xEventGroupSetBits(wifi_event_group, STA_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
    	printf("SYSTEM_EVENT_STA_DISCONNECTED\n");
    	esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, STA_CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

/* Button 0 ISR
 *
 * This is the ISR for Button 0 on falling edge
 *
 * \author: Christian Mödlhammer
 */
void IRAM_ATTR btn0_isr_handler(void* arg)
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

/**
 * builds a char array out of a string
 */
void stringToSendChar(char text[]){

	for(int i=0;i<LEN_DATA;i++){
		mydata[i]=text[i];
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
void init_button0(void){

	gpio_set_pull_mode(GPIO_NUM_0,GPIO_PULLUP_ONLY);
	gpio_set_intr_type(GPIO_NUM_0, GPIO_INTR_ANYEDGE);
	gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_0, btn0_isr_handler, (void*) GPIO_NUM_0);

}

/**
 * initializes the WIFI and the connection to the in config.h specified Access Point
 *
 *
 */
static void initialise_wifi(void){

    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t sta_config = {
        .sta = {
            .ssid = WIFIROUTER,
            .password = WIFIPASS,
			.bssid_set = 0
        }
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}




/**
 *	Socket öffnen und Daten senden
 */
static void task_socket(void *someirrelevantdata){
  struct timeval  tv1, tv2;
  uint32_t luxW;
  char text[LEN_DATA];

  while(1){
    gettimeofday(&tv1, NULL);
    xEventGroupWaitBits(
              wifi_event_group,   /* The event group being tested. */
              STA_CONNECTED_BIT, /* The bits within the event group to wait for. */
              false,        /* BIT_0 & BIT_4 should be cleared before returning. */
              true,       /* Don't wait for both bits, either bit will do. */
              portMAX_DELAY );
  	int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
  	   ERROR("SOCKET: %d ERRno:%d", sock, errno);
    }else{
    	struct sockaddr_in serverAddress;
    	serverAddress.sin_family = AF_INET;
    	inet_pton(AF_INET, IPDEAMON, &serverAddress.sin_addr.s_addr);
    	serverAddress.sin_port = htons(PORT_NUMBER);

    	int rs = connect(sock, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in));
      if(rs<0){
    	   ERROR("Verbindungsfehler: %d ERRno:%d", sock, errno);
         close(sock);
      }else{
    	xQueueReceive(lichtsensor_queue, &luxW, portMAX_DELAY);
    	//printf("Luxreceived: %d\n",luxW);
    	strcpy(text,"GET /endpoint/test?lux=");
    	char tmp[sizeof(uint32_t)];
    	sprintf(tmp,"%d", luxW);
    	strcat(text, tmp);
    	strcat(text,"  HTTP/1.1\n");
    	stringToSendChar(text);
    	rs = write(sock,(char*)mydata,LEN_DATA);
        close(sock);
        if(rs<0){
      	   ERROR("Schreibfehler: %d ERRno:%d", sock, errno);
        }else{
          // si todo esto para esperar de forma chiva solo cuando todo sale bien:D
          do{
            delay_ms(5);
            gettimeofday(&tv2,NULL);
          }while(
            ((tv2.tv_usec - tv1.tv_usec)/1000 +
            (tv2.tv_sec - tv1.tv_sec)*1000)<1000
          );
        }
      }
    }
  }
}



/* app_main()
 *
 *  This Function is the Main Funktion of the Project
 *
 */
void app_main(void)
{
	nvs_flash_init(); //Initialize NVS flash storage with layout given in the partition table

	// Start HW Components and millisecond timer
	app_lichtsensor(); // starting light sensor
	app_timer(); // starting the timer with the queues 1 ms and 50 ms
	app_lichtschranke(); // starting the light barrier logic
	app_lichtrelay(); // starting the light relay logic

	// Start WIFI connection
	initialise_wifi();
    xTaskCreate(&task_socket, "socket", 2048  , NULL, 5, NULL);

    //
    
    //--VSCP------------------------//
    init_vscp_millisecond_timer();
    init_button0(); // initializes Button and interrupt

    

    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);
    int level = 0;
    while (true) {
    		gpio_set_level(GPIO_NUM_5, level);
    	        level = !level;
    	        vTaskDelay(300 / portTICK_PERIOD_MS);

 //       printf("Value of vscp_initbtncnt: %d\n", vscp_initbtncnt);
    }
}

