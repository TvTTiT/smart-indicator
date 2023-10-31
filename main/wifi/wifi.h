#ifndef WIFI_H
#define WIFI_H

#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_http_server.h"

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_initialize(void);

#endif
