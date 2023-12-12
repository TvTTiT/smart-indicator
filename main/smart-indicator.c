#include "wifi/wifi.h"
#include "api/weatherApi.h"

void app_main() {
    // Initialize NVS flash memory at the beginning of the application
    ESP_ERROR_CHECK(nvs_flash_init());

    // Check if Wi-Fi credentials are stored in NVS
   // bool wifi_credentials = check_wifi_credentials();
   // if (wifi_credentials) {
   //     wifi_connection(); // Connect to the user's home WiFi
   //     vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
   // } 
    //    printf("%d\n", wifi_credentials);
    // Check Wi-Fi 
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
    bool wifi_connected = check_wifi_connection();
    while(!wifi_connected){
        printf("start AP.......\n");
        access_point_initialize(); // Start the access point and server once
        while(1){
         if (input_check()) {
            printf("Received WiFi credentials. Starting WiFi connection...\n");
            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
            wifi_connection(); // Connect to the user's home WiFi
            vTaskDelay(pdMS_TO_TICKS(1000));
            wifi_connected = check_wifi_connection();
            printf("%d check wifi connection/n",wifi_connected);
            if(wifi_connected){
                stop_server();
                printf("Closing server\n");
                break;
            }
         }
        }  
    }
    if(wifi_connected) {
        xTaskCreate(&http_request_task, "http_request_task", 8192, NULL, 5, NULL);
    }
}