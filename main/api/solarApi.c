#include "solarApi.h"
#include "cJSON.h"

#define TAG "solarApi"
// Solar API
#define URL "http://api.forecast.solar/D7KgKB343sgm7ftz/estimate/watts/52.3299/6.1125/37/0/0.4"

#define MAX_DATA_POINTS 17

typedef struct {
    char timestamp[20];
    int value;
} SolarData;

SolarData solarData[MAX_DATA_POINTS];

char accumulatedData[4096] = "";  // Adjust the size based on your expected response size
int dataCount = 0;
void parse_solar_data(const char *json_data) {
    cJSON *root = cJSON_Parse(json_data);
    if (root != NULL) {
        cJSON *result = cJSON_GetObjectItem(root, "result");
        if (result != NULL) {
            cJSON *timestamp, *value;
            cJSON_ArrayForEach(timestamp, result) {
                value = cJSON_GetObjectItem(result, timestamp->string);
                if (value != NULL && dataCount < MAX_DATA_POINTS) {
                    strncpy(solarData[dataCount].timestamp, timestamp->string, sizeof(solarData[dataCount].timestamp));
                    solarData[dataCount].value = value->valueint;
                    dataCount++;
                }
            }
        }

        cJSON_Delete(root); // Free cJSON objects
    }
}
esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
       case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            strncat(accumulatedData, (const char *)evt->data, evt->data_len);
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
             // Parse JSON data
            parse_solar_data(accumulatedData);
            print_solar_data();
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT");
            break;
    }
    return ESP_OK;
}
void http_request_task(void *pvParameters) {
    esp_http_client_config_t config = {
        .url = URL,
        .event_handler = _http_event_handler,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP Status = %lld, content_length = %lld",
                (int64_t)esp_http_client_get_status_code(client),
                (int64_t)esp_http_client_get_content_length(client));

    } else {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    vTaskDelete(NULL);
}
// Function to print the solar data
void print_solar_data() {
    for (int i = 0; i < dataCount; i++) {
        ESP_LOGI(TAG, "Timestamp: %s, Value: %d", solarData[i].timestamp, solarData[i].value);
    }
}