#include "wifi/wifi.h"
#include "api/solarApi.h"
#include "hardware/led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "hardware/current_time.h"

// Function to handle the timer expiration
void timerCallback(TimerHandle_t xTimer) {
    printf("No input for 10 seconds. Restarting ESP32...\n");
    esp_restart();
}

void app_main() {
    // Initialize NVS flash memory at the beginning of the application
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // Initialize the LED strip
    led_strip_handle_t led_strip = configure_led();
    // Turn on defalut LEDS
    default_leds(led_strip);
    // Check if Wi-Fi credentials are stored in NVS
    bool wifi_credentials = check_wifi_credentials();
    if (wifi_credentials) {
        wifi_connection(); // Connect to the user's home WiFi
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
   } 
    // Check Wi-Fi 
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
    bool wifi_connected = check_wifi_connection();
    while(!wifi_connected){
        printf("start AP.......\n");
        access_point_initialize(); // Start the access point and server once
        TimerHandle_t timer = xTimerCreate("InputTimer", pdMS_TO_TICKS(30000), pdFALSE, 0, timerCallback);
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

    if (wifi_connected) {
        //get the api data
        xTaskCreate(&http_request_task, "http_request_task", 8192, NULL, 5, NULL);
        // Initialize SNTP for time synchronization
        initialize_sntp();
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
        //int current_hour = get_current_hour();// Get the current time
        int current_hour = 2;
        printf("current_hour %d \n", current_hour);
        turn_off_all_leds(led_strip);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to change LEDs
        display_current_time(led_strip,current_hour);
    }
}