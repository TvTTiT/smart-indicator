#include "wifi/wifi.h"

void app_main() {
    wifi_initialize();

    while (1) {
        // Your application logic
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
