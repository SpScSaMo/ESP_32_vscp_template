/*
 * webserver.h
 *
 *  Created on: May 25, 2017
 *      Author: Gerhard
 */

#ifndef MAIN_WEBSERVER_H_
#define MAIN_WEBSERVER_H_


//******************************************************************
// INCLUDES - FOR webserver
//******************************************************************


//#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "tcpip_adapter.h"

#include "lwip/API.h"
#include "esp_err.h"            //for error handling
#include "esp_intr_alloc.h"     //is used for interrupts


#include "lwip/err.h"
#include "string.h"

#include "cJSON.h"


#include "config.h"				/* this has the defines for all the sensors*/

/**************************************************/


#define delay_ms(ms) vTaskDelay((ms) / portTICK_RATE_MS)


/**************************************************/
char* json_unformatted;



/**************************************************/


// STATIC HTTP HEADER
const static char http_html_hdr[] =
"HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";


// STATIC HTTP PAGE
const static char http_index_hml[] = "<!DOCTYPE html>"
"<html>\n"
"<head>\n"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"  <style type=\"text/css\">\n"
"    html, body, iframe { margin: 0; padding: 0; height: 100%; }\n"
"    iframe { display: block; width: 100%; border: none; }\n"
"  </style>\n"
"<title>Better Than VSCP</title>\n"
"</head>\n"
"<body>\n"
"<h1>Hello World, from ESP32!</h1>\n"
"</body>\n"
"</html>\n";


/**
 * Funktion Prototyps
 */


static void http_server_netconn_serve(struct netconn *conn);
static void http_server(void *pvParameters);
static void generate_json();

#endif /* MAIN_WEBSERVER_H_ */
