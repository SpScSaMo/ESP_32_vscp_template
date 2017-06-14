#ifndef ESP32_CONFIG_H
#define ESP32_CONFIG_H

#include "esp_wifi_types.h"
#include "freertos/event_groups.h"

/* +++++++++++++++++++++++++++ defines for ESP32 Thing for communication +++++++++++++++++++++++++++++ */
#define THING_ID		"ESP32_123456"
#define CHANNEL1		"1"
#define CHANNELTYPE1 	CHANNELTYPE_NUMBER  //"Lightsensor"
#define CHANNEL2		"2"
#define CHANNELTYPE2	CHANNELTYPE_NUMBER  //"Lightbarrier"
#define CHANNEL3		"3"
#define CHANNELTYPE3	CHANNELTYPE_SWITCH  //"Lightswitch"
#define CHANNEL4		"4"
#define CHANNELTYPE4	CHANNELTYPE_ROLLERSHUTTER  //"Motorswitch"
#define WIFIROUTER		"XXX"           //ENTER ROUTER NAME Gerhard Phone
#define WIFIPASS		"XXX"           //ENTER ROUTER PASSWORD test1234

#define COMMANDMEASSURETYPE_UNDEFINED  	"0"
#define COMMANDMEASSURETYPE_COUNT		"1"
#define COMMANDMEASSURETYPE_LENGTH		"2"
#define COMMANDMEASSURETYPE_MASS		"3"
#define COMMANDMEASSURETYPE_TIME		"4"
#define COMMANDMEASSURETYPE_TEMPERATURE	"5"
#define COMMANDMEASSURETYPE_BRIGHTNESS	"6"


#define COMMANDTYPE_OnOffType 				"0"
#define COMMANDTYPE_IncreaseDecreaseType 	"1"
#define COMMANDTYPE_NextPreviousType		"2"
#define COMMANDTYPE_OpenClosedType			"3"
#define COMMANDTYPE_PlayPauseType			"4"
#define COMMANDTYPE_RewindFastforwardType	"5"
#define COMMANDTYPE_StopMoveType			"6"
#define COMMANDTYPE_UpDownType				"7"
#define COMMANDTYPE_Percent 				"8"
#define COMMANDTYPE_Hsb						"9"

#define OnOffType_Off 	"0"
#define OnOffType_On  	"1"

#define UpDownType_Up 	"0"
#define UpDownType_Down "1"

#define CHANNELTYPE_NUMBER			"0"
#define CHANNELTYPE_SWITCH			"1"
#define CHANNELTYPE_COLOR			"2"
#define CHANNELTYPE_CONTACT			"3"
#define CHANNELTYPE_DATETIME    	"4"
#define CHANNELTYPE_DIMMER      	"5"
#define CHANNELTYPE_PLAYER      	"6"
#define CHANNELTYPE_ROLLERSHUTTER  	"7"
#define CHANNELTYPE_TEXT        	"8"


/* Constants that aren't configurable in menuconfig */
#define DEAMON_SERVER "btdeamon20170517084951.azurewebsites.net"
#define DEAMON_PORT "80"

#define WEBSERVER_PORT 88

/* ++++++++++++++++++++++++++++++++ other defines +++++++++++++++++++++++++++++++++++++++++*/

//#define DEBUG(...) ESP_LOGD(TAG,__VA_ARGS__);
#define INFO(...) ESP_LOGI(TAG,__VA_ARGS__);
#define ERROR(...) ESP_LOGE(TAG,__VA_ARGS__);
#define delay_ms(ms) vTaskDelay((ms) / portTICK_RATE_MS)

/* +++++++++++++++++++++++++++ Global Queue and typedef for sensors +++++++++++++++++++++++++++++ */
xQueueHandle sensor_queue;

typedef struct messageparameters {
    char channelId[3];
    char commandType[3];
    uint32_t value;
}messageparameters;

/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t wifi_event_group;

static const char *TAG = "example"; // for logging of http requests


/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
int STA_CONNECTED_BIT;



#endif
