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
    led_strip_handle_t mass_devices_led_strip_1 = configure_mass_devices_time_led_1();
    led_strip_handle_t mass_devices_led_strip_2 = configure_mass_devices_time_led_2();
    led_strip_handle_t light_devices_led_strip_1 = configure_light_devices_time_led_1();
    led_strip_handle_t light_devices_led_strip_2 = configure_light_devices_time_led_2();
    led_strip_handle_t leaf_led_strip = configure_leaf_led();
    // Turn on defalut LEDS
    default_leds(mass_devices_led_strip_1);
    default_leds(mass_devices_led_strip_2);
    default_leds(light_devices_led_strip_1);
    default_leds(light_devices_led_strip_2);
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
        turn_off_all_leds(mass_devices_led_strip_1);
        turn_off_all_leds(mass_devices_led_strip_2);
        turn_off_all_leds(light_devices_led_strip_1);
        turn_off_all_leds(light_devices_led_strip_2);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change color 
        // Turn to red
        display_no_wifi_conection(mass_devices_led_strip_1);
        display_no_wifi_conection(mass_devices_led_strip_2);
        display_no_wifi_conection(light_devices_led_strip_1);
        display_no_wifi_conection(light_devices_led_strip_2);

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
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        // Get the API data
        xTaskCreate(&http_request_task, "http_request_task", 8192, NULL, 5, NULL);

        // Display time for mass devices
        turn_off_all_leds(mass_devices_led_strip_1);
        turn_off_all_leds(mass_devices_led_strip_2);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        display_time_for_mass_devices(mass_devices_led_strip_1);
        display_time_for_mass_devices(mass_devices_led_strip_2);

        // Display time for light devices
        turn_off_all_leds(light_devices_led_strip_1);
        turn_off_all_leds(light_devices_led_strip_2);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        display_time_for_light_devices(light_devices_led_strip_1);
        display_time_for_light_devices(light_devices_led_strip_2);

        // The time gap in milliseconds
        int time_gap = get_time_gap_to_next_day();
        printf("Time gap until the next day: %d milliseconds \n", time_gap);
        vTaskDelay(pdMS_TO_TICKS(time_gap)); // Delay until the next hour before start looping
        //cleanning up 
        clean_solar_data();
        clean_accumulated_data();
        printf("data is cleaned \n");

    }

}