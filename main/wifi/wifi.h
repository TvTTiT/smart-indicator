#ifndef WIFI_H
#define WIFI_H

#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_http_server.h"
#include "nvs.h"


void access_point_initialize(void);
httpd_handle_t start_webserver(void);
void stop_webserver(httpd_handle_t *server);
void wifi_connection(void);
bool input_check(void);
void stop_server(void);
bool check_wifi_credentials();
bool check_wifi_connection();
void wifi_initialize(void);
void nvs_reset_all(void);
void url_decode(char* str);
char hex_to_char(const char* hex);
void wifi_stop();
#endif
