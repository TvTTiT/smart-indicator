#include "wifi/wifi.h"

bool check_wifi_credentials() {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("wifi_config", NVS_READONLY, &nvs_handle);

    if (err != ESP_OK) {
        return false;
    }

    size_t ssid_len = sizeof(ssid);
    size_t password_len = sizeof(password);
    err = nvs_get_str(nvs_handle, "ssid", ssid, &ssid_len);

    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_get_str(nvs_handle, "password", password, &password_len);
    nvs_close(nvs_handle);

    return err == ESP_OK;
}


void app_main() {
    // Initialize NVS flash memory at the beginning of the application
    ESP_ERROR_CHECK(nvs_flash_init());

    // Check if Wi-Fi credentials are stored in NVS
    bool wifiStored = check_wifi_credentials();
    if (wifiStored) {
        wifi_connection(); // Connect to the user's home WiFi
    } else {
        access_point_initialize(); // Start the access point and server once
        while (1) {
            if (input_check()) {
                printf("Received WiFi credentials. Starting WiFi connection...\n");
                stop_server();
                vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
                wifi_connection(); // Connect to the user's home WiFi
                break;
            }

            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to periodically check for WiFi credentials
        }
    }
}
