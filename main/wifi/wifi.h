#ifndef WIFI_H
#define WIFI_H

#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_http_server.h"
#include "nvs.h"

extern char ssid[64]; // Declare the ssid variable
extern char password[64]; // Declare the password variable

void access_point_initialize(void);
httpd_handle_t start_webserver(void);
void stop_webserver(httpd_handle_t *server);
void wifi_connection(void);
bool input_check(void);
void stop_server(void);

#endif
