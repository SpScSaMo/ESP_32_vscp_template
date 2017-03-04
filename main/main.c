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
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//******************************************************************
// SPECIAL INCLUDES - ESP32 - VSCP Very Simple Control Protokoll
//******************************************************************
#include <string.h>
#include <stdlib.h>
#include "vscp_class.h"
#include "vscp_type.h"
#include "vscp_firmware.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/* init_millisecond_clock
 * # The firmware code rely on a millisecond clock.
 * # First add this to your code and make sure that it works.
 *
 * #vscp_timer and vscp_configtimer is the important things here. 
 * #They are both defined by the firmware code and should be increased 
 * #by on every millisecond, so that is what we do in our code.
 *
 *
 * http://www.vscp.org/docs/vscpfirmware/doku.php?id=a_millisecond_clock
 */
init_millisecond_clock(void) {
    
    /*
    // Clock
    if ( INTCONbits.TMR0IF ) { // If a Timer0 Interrupt, Then...
        // Reload value for 1 ms reolution
        WriteTimer0(TIMER0_RELOAD_VALUE);
        vscp_timer++;
        vscp_configtimer++;
        measurement_clock++;
        timeout_clock++;
        ...
    */
    
    
}


esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}



/* app_main()
 *
 *  This Function is the Main Funktion of the Project
 *
 */
void app_main(void)
{
    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
            .ssid = "access_point_name",
            .password = "password",
            .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    int level = 0;
    while (true) {
        gpio_set_level(GPIO_NUM_4, level);
        level = !level;
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

