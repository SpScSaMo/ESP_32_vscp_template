#ifndef ESP32_CONFIG_H
#define ESP32_CONFIG_H

#include "esp_wifi_types.h"

/* +++++++++++++++++++++++++++ defines for ESP32 Thing +++++++++++++++++++++++++++++ */
#define THING_ID		"ESP32_123456"
#define CH1 			"Lightsensor"
#define CH2		 		"Lightbarrier"
#define CH3		 		"Lightswitch"
#define CH4		 		"Motorswitch"
#define WIFIROUTER		"???"           //ENTER ROUTER NAME
#define WIFIPASS		"???"           //ENTER ROUTER PASSWORD
//#define WIFIAUTH        WIFI_AUTH_WPA_PSK
#define IPDEAMON		"10.0.0.14" //not used
#define PORT_NUMBER 	9592	//not used

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "btdeamon20170517084951.azurewebsites.net"
#define WEB_PORT 80

/* +++++++++++++++++++++++++++ Global Queue and typedef for sensors +++++++++++++++++++++++++++++ */
xQueueHandle sensor_queue;

typedef struct messageparameters {
    char type[20];
    char measurementtype[10];
    uint32_t value;
}messageparameters;

#endif
