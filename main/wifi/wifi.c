#include "wifi.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "page.h"

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#define AP_SSID "Smart Indicator"
#define AP_MAX_CONN 4
#define AP_CHANNEL 0
#define MAX_CONNECTION_ATTEMPTS 10

char ssid[64] = "";
char password[64] = "";
bool input_received = false;
bool wifi_connected = false; 

static int connection_attempts = 0;

httpd_handle_t server = NULL; 

static esp_err_t root_handler(httpd_req_t *req) {
    httpd_resp_sendstr(req, html_welcome);
    return ESP_OK;
}

// Function to decode URL-encoded string in-place
void url_decode(char* str) {
    char* p = str;
    while (*p) {
        if (*p == '%' && *(p + 1) && *(p + 2)) {
            if (*(p + 1) == '2' && *(p + 2) == '0') {
                // Handle space encoded as '+'
                *p = ' ';
                memmove(p + 1, p + 3, strlen(p + 3) + 1);
            } else {
                *p = hex_to_char(p + 1);
                memmove(p + 1, p + 3, strlen(p + 3) + 1);
            }
        } else if (*p == '+') {
            // Replace '+' with space
            *p = ' ';
        }
        p++;
    }
}

// Helper function to convert a hex string to a char
char hex_to_char(const char* hex) {
    char result = 0;
    for (int i = 0; i < 2; ++i) {
        char c = hex[i];
        result = result << 4;
        if (c >= '0' && c <= '9') {
            result += c - '0';
        } else if (c >= 'A' && c <= 'F') {
            result += c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            result += c - 'a' + 10;
        }
    }
    return result;
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

    // URL decode the received data
    url_decode(buffer);

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
    input_received = true; 
    return ESP_OK;
}

httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    server = NULL; // Initialize the global 'server' variable to NULL

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

void stop_webserver(httpd_handle_t *server) {
    if (*server != NULL) {
        httpd_stop(*server);
        *server = NULL; // Set the pointer to NULL
    }
}

void access_point_initialize(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
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

    server = start_webserver();
    if (server == NULL) {
        printf("Failed to start the HTTP server!\n");
    }
}

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_id == WIFI_EVENT_STA_START) {
        printf("WIFI CONNECTING....\n");
        wifi_connected = false;
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
    } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
        printf("WiFi CONNECTED\n"); 
        wifi_connected = true;
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
        // Save SSID and password to NVS when the connection is successful
        nvs_handle_t nvs_handle;
        esp_err_t err = nvs_open("wifi_config", NVS_READWRITE, &nvs_handle);
        if (err == ESP_OK) {
            err = nvs_set_str(nvs_handle, "ssid", ssid);
            if (err == ESP_OK) {
                err = nvs_set_str(nvs_handle, "password", password);
            }
            nvs_commit(nvs_handle);
            nvs_close(nvs_handle);
            ESP_LOGE("NVS", "SUCCESS");
        }

        if (err != ESP_OK) {
            ESP_LOGE("NVS", "Error saving Wi-Fi credentials to NVS");
        }

        // Reset connection attempts on successful connection
        connection_attempts = 0;
    } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        printf("WiFi lost connection\n");
        wifi_connected = false;
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
        
        // Check the connection attempts counter
        if (++connection_attempts >= MAX_CONNECTION_ATTEMPTS) {
            printf("Exceeded maximum connection attempts. Restarting...\n");
            esp_restart();
        } else {
            // Attempt to reconnect
            esp_wifi_connect();
        }
    } else if (event_id == IP_EVENT_STA_GOT_IP) {
        printf("Wifi got IP...\n\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to connect to Wi-Fi
    }
}

void wifi_initialize(){
    esp_netif_init(); // Network interface initialization
    esp_netif_create_default_wifi_sta(); // Create default WiFi station interface
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL); // Register WiFi event handler
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL); // Register IP event handler

}

void wifi_connection() {
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT(); // Initialize WiFi

    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = "",
            .password = "",
        }
    };

    strcpy((char *)wifi_configuration.sta.ssid, ssid);
    strcpy((char *)wifi_configuration.sta.password, password);

    ESP_ERROR_CHECK(esp_wifi_init(&wifi_initiation)); // Initialize WiFi
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // Set WiFi mode to station
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration)); // Set WiFi configuration
    ESP_ERROR_CHECK(esp_wifi_start()); // Start WiFi
    esp_wifi_connect(); // Connect to WiFi

    printf("Wi-Fi initialization finished. SSID:%s  password:%s\n", wifi_configuration.sta.ssid, wifi_configuration.sta.password);

}

void wifi_stop() {
    ESP_ERROR_CHECK(esp_wifi_stop());    // Stop WiFi
    ESP_ERROR_CHECK(esp_wifi_deinit());  // Deinitialize WiFi
}

bool input_check (){
    return input_received; 
}

void stop_server() {
    vTaskDelay(pdMS_TO_TICKS(1000)); 
    stop_webserver(&server);
    if (server == NULL) {
        printf("The server is closed.\n");
        esp_wifi_stop();
        esp_wifi_set_mode(WIFI_MODE_STA); // Set Wi-Fi mode to Station mode

        // Optionally, you can deinitialize the NVS storage
        ESP_ERROR_CHECK(nvs_flash_deinit());
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    } else {
        printf("The server is not closed.\n");
    }

}


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

bool check_wifi_connection(){
    return wifi_connected; 
}

void nvs_reset_all() {
    esp_err_t err = nvs_flash_init();

    if (err == ESP_OK) {
        err = nvs_flash_erase();
        if (err == ESP_OK) {
            ESP_LOGI("NVS", "NVS erased successfully");
        } else {
            ESP_LOGE("NVS", "Error erasing NVS: %s", esp_err_to_name(err));
        }
        nvs_flash_deinit();
    } else {
        ESP_LOGE("NVS", "Error initializing NVS: %s", esp_err_to_name(err));
    }
}
