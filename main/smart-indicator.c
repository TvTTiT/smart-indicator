#include "wifi/wifi.h"

void app_main() {
    nvs_flash_init();
    access_point_initialize(); // Start the access point and server once

    while(1){
        if(input_check()){
            printf("Received WiFi credentials. Starting WiFi connection...\n");
            wifi_connected_handler();
            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to wifi
            wifi_connection(); // Connect to the user's home WiFi
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to periodically check for WiFi credentials
    }
        printf("out side while loop\n");
}

