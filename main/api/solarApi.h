#ifndef SOLAR_API_H
#define SOLAR_API_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_http_client.h>
#include "cJSON.h"

#define MAX_DATA_POINTS 18
#define MAX_TIME_TO_USE 2

// Function to handle HTTP events
esp_err_t _http_event_handler(esp_http_client_event_t *evt);
// Task to handle HTTP request
void http_request_task(void *pvParameters);
void print_solar_data();
void set_avg_value();
void parse_solar_data(const char *json_data);
int get_avg_value();
void set_energy_production_time();
void set_time_to_use_mass_devices();
void set_time_to_use_light_devices();
int get_start_time_to_use_mass_devices();
int get_end_time_to_use_mass_devices();
int get_start_time_to_use_light_devices();
int get_end_time_to_use_light_devices();
int get_start_time_for_energy_production();
int get_end_time_for_energy_production();
void set_energy_production_time();
#endif /* SOLAR_API_H */
