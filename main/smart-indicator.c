#include "wifi/wifi.h"
#include "api/solarApi.h"
#include "hardware/led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "hardware/current_time.h"

// Function to handle the timer expiration
void timerCallback(TimerHandle_t xTimer) {
    printf("No input. Restarting ESP32...\n");
    esp_restart();
}

void wifi_initialize() {
    esp_netif_init(); // Network interface initialization
    esp_netif_create_default_wifi_sta(); // Create default WiFi station interface

}

void app_main() {
    // Initialize NVS flash memory at the beginning of the application
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // Initialize the LED strip
    led_strip_handle_t current_time_led_strip = configure_current_time_led();
    led_strip_handle_t energy_production_time_led_strip = configure_energy_production_time_led();
    led_strip_handle_t mass_devices_led_strip = configure_mass_devices_time_led();
    led_strip_handle_t light_devices_led_strip = configure_light_devices_time_led();
    // Turn on defalut LEDS
    default_leds(current_time_led_strip);
    default_leds(energy_production_time_led_strip);
    default_leds(mass_devices_led_strip);
    default_leds(light_devices_led_strip);
    // Check if Wi-Fi credentials are stored in NVS
    bool wifi_credentials = check_wifi_credentials();
    if (wifi_credentials) {
        wifi_initialize();
        wifi_connection(); // Connect to the user's home WiFi
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
   } 
    // Check Wi-Fi 
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
    bool wifi_connected = check_wifi_connection();
    while(!wifi_connected){
        printf("start AP.......\n");
        access_point_initialize(); // Start the access point and server once
        TimerHandle_t timer = xTimerCreate("InputTimer", pdMS_TO_TICKS(5 * 60 * 1000), pdFALSE, 0, timerCallback);
        xTimerStart(timer, 0);

        while (1) {
            if (input_check()) {
                // Reset the timer if there's input
                xTimerReset(timer, 0);
                printf("Received WiFi credentials. Starting WiFi connection...\n");
                vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
                wifi_connection(); // Connect to the user's home WiFi
                vTaskDelay(pdMS_TO_TICKS(1000));
                wifi_connected = check_wifi_connection();
                if (wifi_connected) {
                    stop_server();
                    printf("Closing server\n");
                    break;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        xTimerStop(timer, 0);
        xTimerDelete(timer, 0);
    }
    // Initialize SNTP for time synchronization
    initialize_sntp();
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for SNTP
    // Processing after wifi connection 
    while (wifi_connected) {
        // Get the API data
        xTaskCreate(&http_request_task, "http_request_task", 8192, NULL, 5, NULL);

        // Get the current time
        int current_hour = get_current_hour();
        printf("current_hour %d \n", current_hour);

        // Turn off all LEDs
        turn_off_all_leds(current_time_led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs

        // Display current time
        display_current_time(current_time_led_strip, current_hour);

        // Display time for energy production
        int energy_production_start_hour = get_start_time_for_energy_production();
        int energy_production_end_hour = get_end_time_for_energy_production();
        turn_off_all_leds(energy_production_time_led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        display_energy_production(energy_production_time_led_strip, energy_production_start_hour, energy_production_end_hour);
        
        // Display time for mass devices
        int mass_devices_start_hour = get_start_time_to_use_mass_devices();
        int mass_devices_end_hour = get_end_time_to_use_mass_devices();
        turn_off_all_leds(mass_devices_led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        display_time_for_mass_devices(mass_devices_led_strip, mass_devices_start_hour, mass_devices_end_hour);

        // Display time for light devices
        int light_devices_start_hour = get_start_time_to_use_light_devices();
        int light_devices_end_hour = get_end_time_to_use_light_devices();
        turn_off_all_leds(light_devices_led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        display_time_for_light_devices(light_devices_led_strip, light_devices_start_hour, light_devices_end_hour);

        vTaskDelay(pdMS_TO_TICKS(5 * 6 * 1000)); // Delay before start looping
        //clean memory
        cleanSolarData();
        cleanAccumulatedData();
        printf("cleaned");
    }

}