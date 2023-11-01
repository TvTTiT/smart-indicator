#include "wifi.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "page.h"

#define AP_SSID "Smart Indicator"
#define AP_MAX_CONN 4
#define AP_CHANNEL 0

static esp_err_t root_handler(httpd_req_t *req) {
    httpd_resp_sendstr(req, html_welcome);
    return ESP_OK;
}

static esp_err_t connect_handler(httpd_req_t *req) {
    char buffer[1024];
    int ret;
    int received = 0;

    while (1) {
        ret = httpd_req_recv(req, buffer + received, sizeof(buffer) - received);
        if (ret <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                httpd_resp_send_408(req);
            }
            break;
        }
        received += ret;
    }
    buffer[received] = '\0';

    // Parse the received data to extract SSID and password
    char ssid[64] = "";
    char password[64] = "";
    if (httpd_query_key_value(buffer, "ssid", ssid, sizeof(ssid)) == ESP_OK &&
        httpd_query_key_value(buffer, "password", password, sizeof(password)) == ESP_OK) {
        // Print SSID and password to console
        ESP_LOGI("WiFi", "Received SSID: %s, Password: %s", ssid, password);
    }

    // Send a "Success" response to the user
    const char *success_response = 
        "<html>"
        "<body>"
        "<h1>Success! Form Data Received</h1>"
        "</body>"
        "</html>";

    httpd_resp_send(req, success_response, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 50;

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t root = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = root_handler,
            .user_ctx = NULL,
        };
        httpd_register_uri_handler(server, &root);

        httpd_uri_t connect = {
            .uri = "/connect",
            .method = HTTP_POST,
            .handler = connect_handler,
            .user_ctx = NULL,
        };
        httpd_register_uri_handler(server, &connect);

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
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    wifi_config_t ap_config = {
        .ap = {
            .ssid = AP_SSID,
            .max_connection = AP_MAX_CONN,
            .channel = AP_CHANNEL,
            .ssid_hidden = 0,
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config));

    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("WiFi", "ESP32 started in AP mode");

    httpd_handle_t server = start_webserver();
    if (server == NULL) {
        printf("Failed to start the HTTP server!\n");
    }
}
