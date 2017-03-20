#include "vscp_millisecond_timer.h"
//#include "vscp_firmware.h"

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
void task_millisecond_clock(void* arg) {
	
	uint32_t some_irrelevant_value = NULL;
	
	while (1){

		// this gives us a trigger every millisecond     -> This should block the while loop for a millisecond
		xQueueReceive(ms_1_queue, &some_irrelevant_value, portMAX_DELAY);

		vscp_timer++;
		vscp_configtimer++;
		measurement_clock++;
		timeout_clock++;
	
	}

}


void init_vscp_millisecond_timer(void) {

	
	//Thats for the VSCP - Timer must count up Timer Variables all the time
	xTaskCreate(task_millisecond_clock, "task_millisecond_clock", 2048, NULL, 5, NULL);
	
	
}