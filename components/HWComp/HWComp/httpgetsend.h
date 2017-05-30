#ifndef HTTPGETSEND_H
#define HTTPGETSEND_H


//******************************************************************
// INCLUDES - FOR http Connection
//******************************************************************

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/queue.h"
#include "errno.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"


/* +++++++++++++++++++++++++++ global queue +++++++++++++++++++++++++++++ */
xQueueHandle send_queue;

void http_get_task(void *pvParameters);
void app_httpgetsend();


#endif
