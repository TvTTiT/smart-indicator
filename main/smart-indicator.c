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

void app_main() {
    //nvs_reset_all();
    // Initialize NVS flash memory at the beginning of the application
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // Initialize the LED strip
    led_strip_handle_t current_time_led_strip = configure_current_time_led();
    led_strip_handle_t energy_production_time_led_strip = configure_energy_production_time_led();
    led_strip_handle_t mass_devices_led_strip = configure_mass_devices_time_led();
    led_strip_handle_t light_devices_led_strip = configure_light_devices_time_led();
    led_strip_handle_t leaf_led_strip = configure_leaf_led();
    // Turn on defalut LEDS
    default_leds(current_time_led_strip);
    default_leds(energy_production_time_led_strip);
    default_leds(mass_devices_led_strip);
    default_leds(light_devices_led_strip);
    display_leaf_led(leaf_led_strip);
    // Check if Wi-Fi credentials are stored in NVS
    bool wifi_credentials = check_wifi_credentials();
    wifi_initialize();
    if (wifi_credentials) {
        wifi_connection(); // Connect to the user's home WiFi
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
   } 
    // Check Wi-Fi 
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
    bool wifi_connected = check_wifi_connection();
    while(!wifi_connected){
         // Turn off all LEDs
        turn_off_all_leds(current_time_led_strip);
        turn_off_all_leds(energy_production_time_led_strip);
        turn_off_all_leds(mass_devices_led_strip);
        turn_off_all_leds(light_devices_led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change color 
        // Turn to red
        display_no_wifi_conection(current_time_led_strip);
        display_no_wifi_conection(energy_production_time_led_strip);
        display_no_wifi_conection(mass_devices_led_strip);
        display_no_wifi_conection(light_devices_led_strip);

        printf("start AP.......\n");
        access_point_initialize(); // Start the access point and server once
        //call back to check for wifi_connected 
        TimerHandle_t timer = xTimerCreate("InputTimer", pdMS_TO_TICKS(3 * 60 * 1000), pdFALSE, 0, timerCallback);
        xTimerStart(timer, 0);
        //loop for starting server
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
    //check for sntp
    if (!is_sntp_initialized()) {
        printf(" SNTP is not initialized. Restarting ESP32...\n");
        esp_restart();
    }  
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for SNTP
    
    // Processing after wifi connection 
    while (wifi_connected) {
        // Get the current time
        int current_hour = get_current_hour();
        printf("current_hour %d \n", current_hour);

        // Turn off all LEDs
        turn_off_all_leds(current_time_led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        // Display current time
        display_current_time(current_time_led_strip, current_hour);

        // Get the API data
        xTaskCreate(&http_request_task, "http_request_task", 8192, NULL, 5, NULL);

        // Display time for energy production
        turn_off_all_leds(energy_production_time_led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        display_energy_production(energy_production_time_led_strip);
        
        // Display time for mass devices
        turn_off_all_leds(mass_devices_led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        display_time_for_mass_devices(mass_devices_led_strip);

        // Display time for light devices
        turn_off_all_leds(light_devices_led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        display_time_for_light_devices(light_devices_led_strip);

        // The time gap in milliseconds
        int time_gap = get_time_gap_to_next_hour();
        printf("Time gap until the next hour: %d milliseconds \n", time_gap);
        vTaskDelay(pdMS_TO_TICKS(time_gap)); // Delay until the next hour before start looping
        //cleanning up 
        cleanSolarData();
        cleanAccumulatedData();
        printf("data is cleaned \n");

    }

}