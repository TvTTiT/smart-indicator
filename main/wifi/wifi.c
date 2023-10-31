#include "wifi.h"

#define AP_SSID "Smart Indicator"
#define AP_PASSWORD "12345678"
#define AP_MAX_CONN 4
#define AP_CHANNEL 0
#define STA_SSID "TRY-ESP"
#define STA_PASSWORD "12345678"

static const char *html_welcome = "<html><body><h1>Welcome to Smart Indicator</h1></body></html";

static esp_err_t root_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html_welcome, strlen(html_welcome));
    return ESP_OK;
}

httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t root = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = root_handler,
            .user_ctx = NULL,
        };
        httpd_register_uri_handler(server, &root);
        return server;
    }

    printf("Error starting the server!\n");
    return NULL;
}

void stop_webserver(httpd_handle_t server) {
    httpd_stop(server);
}

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    // Your event handling code here
}

void wifi_initialize(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    wifi_config_t sta_config = {
        .sta = {
            .ssid = STA_SSID,
            .password = STA_PASSWORD,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));

    wifi_config_t ap_config = {
        .ap = {
            .ssid = AP_SSID,
            .password = AP_PASSWORD,
            .max_connection = AP_MAX_CONN,
            .channel = AP_CHANNEL,
            .ssid_hidden = 0,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config));

    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    httpd_handle_t server = start_webserver();
    if (server == NULL) {
        printf("Failed to start the HTTP server!\n");
    }
}
