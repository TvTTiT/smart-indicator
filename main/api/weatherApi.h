#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_http_client.h>
#include "cJSON.h"
// Function to handle HTTP events
esp_err_t _http_event_handler(esp_http_client_event_t *evt);
// Task to handle HTTP request
void http_request_task(void *pvParameters);
void print_weather_data();
void parse_weather_data(const char *json_data);
#endif /* WEATHER_API_H */
