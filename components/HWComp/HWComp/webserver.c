/*
 * webserver.c
 *
 *  Created on: May 25, 2017
 *      Author: Gerhard
 */


#include "webserver.h"

static void http_server_netconn_serve(struct netconn *conn)
{
		struct netbuf *inbuf;
		char *buf;
		u16_t buflen;
		err_t err;
		
		/* Read the data from the port, blocking if nothing yet there.
		 We assume the request (the part we care about) is in one netbuf */
		err = netconn_recv(conn, &inbuf);
		
		if (err == ERR_OK) {
				netbuf_data(inbuf, (void**)&buf, &buflen);
				
				// strncpy(_mBuffer, buf, buflen);
				
				/* Is this an HTTP GET command? (only check the first 5 chars, since
				 there are other formats for GET, and we're keeping it very simple )*/
				printf("buffer = %s \n", buf);
				if (buflen>=5 &&
						buf[0]=='G' &&
						buf[1]=='E' &&
						buf[2]=='T' &&
						buf[3]==' ' &&
						buf[4]=='/' ) {
						printf("buf[5] = %c\n", buf[5]);
						/* Send the HTML header
						 * subtract 1 from the size, since we dont send the \0 in the string
						 * NETCONN_NOCOPY: our data is const static, so no need to copy it
						 */
						
						netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
						
						if(buf[5]=='h') {
								/* Send our HTML page */
								netconn_write(conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY);
						}
						else if(buf[5]=='l') {
								/* Send our HTML page */
								netconn_write(conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY);
						}
						else if(buf[5]=='j') {
								netconn_write(conn, json_unformatted, strlen(json_unformatted), NETCONN_NOCOPY);
						}
						else {
								netconn_write(conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY);
						}
				}
				
		}
		/* Close the connection (server closes in HTTP) */
		netconn_close(conn);
		
		/* Delete the buffer (netconn_recv gives us ownership,
		 so we have to make sure to deallocate the buffer) */
		netbuf_delete(inbuf);
}

static void http_server(void *pvParameters)
{
		struct netconn *conn, *newconn;
		err_t err;
		conn = netconn_new(NETCONN_TCP);
		netconn_bind(conn, NULL, 80);
		netconn_listen(conn);
		do {
				err = netconn_accept(conn, &newconn);
				if (err == ERR_OK) {
						http_server_netconn_serve(newconn);
						netconn_delete(newconn);
				}
		} while(err == ERR_OK);
		netconn_close(conn);
		netconn_delete(conn);
}


static void generate_json() {
		cJSON *root, *info, *d;
		root = cJSON_CreateObject();
		
		cJSON_AddItemToObject(root, "d", d = cJSON_CreateObject());
		cJSON_AddItemToObject(root, "info", info = cJSON_CreateObject());
		
		cJSON_AddStringToObject(d, "myName", "CMMC-ESP32-NANO");
		cJSON_AddNumberToObject(d, "temperature", 30.100);
		cJSON_AddNumberToObject(d, "humidity", 70.123);
		
		cJSON_AddStringToObject(info, "ssid", "dummy");
		//cJSON_AddNumberToObject(info, "heap", system_get_free_heap_size());
		//cJSON_AddStringToObject(info, "sdk", system_get_sdk_version());
		//cJSON_AddNumberToObject(info, "time", system_get_time());
		
		while (1) {
				//cJSON_ReplaceItemInObject(info, "heap",
				//													cJSON_CreateNumber(system_get_free_heap_size()));
				//cJSON_ReplaceItemInObject(info, "time",
				//													cJSON_CreateNumber(system_get_time()));
				//cJSON_ReplaceItemInObject(info, "sdk",
				//													cJSON_CreateString(system_get_sdk_version()));
				
				json_unformatted = cJSON_PrintUnformatted(root);
				printf("[len = %d]  ", strlen(json_unformatted));
				
				for (int var = 0; var < strlen(json_unformatted); ++var) {
						putc(json_unformatted[var], stdout);
				}
				
				printf("\n");
				fflush(stdout);
				delay_ms(2000);
				free(json_unformatted);
		}
}
