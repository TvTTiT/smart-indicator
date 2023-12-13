#include "led.h"
#include <math.h>

/* Pint Map

    P0 = 7h = 19h
    P2 = 8h = 20h
    P4 = 9h = 21h
    P6 = 10h = 22h
    P8 = 11h = 23h
    P10 = 12h = 24h = 0h
    P12 = 13h = 1h
    P14 = 14h = 2h
    P16 = 15h = 3h
    P18 = 16h = 4h
    P20 = 17h = 5h
    P22 = 18h = 6h
*/


led_strip_handle_t configure_led(void)
{
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = 16,   // GPIO 16
        .max_leds = 24,        // Set the number of LEDs to 24
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal
    };

    // LED strip backend configuration: RMT
    led_strip_rmt_config_t rmt_config = {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
        .rmt_channel = 0,
#else
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
        .flags.with_dma = false,               // DMA feature is available on ESP target like ESP32-S3
#endif
    };

    // LED Strip object handle
    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    printf( "Created LED strip object with RMT backend");
    return led_strip;
}

void default_leds(led_strip_handle_t led_strip) {
    for (int i = 0; i < 24; i++) {
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, 5, 5, 5));  // Turn on all LEDs
    }
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));  // Refresh the LED strip to send data
    printf("default_leds is on \n");
}

void display_current_time(led_strip_handle_t led_strip, int hour) {
    if(hour > 12){
        hour -= 12;
    }
    //Display hour
    switch (hour)
    {
    case 0:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 11, 5, 5, 5));
        break;
    case 1:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 12, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 13, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 14, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 15, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 16, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 17, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 18, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 19, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 20, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 21, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 22, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 23, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 2:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 14, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 15, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 16, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 17, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 18, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 19, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 20, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 21, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 22, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 23, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 3:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 16, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 17, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 18, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 19, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 20, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 21, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 22, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 23, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 4:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 18, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 19, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 20, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 21, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 22, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 23, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 5:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 20, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 21, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 22, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 23, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 6:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 22, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 23, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 7:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 8:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 9:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 10:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        break;
    case 11:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
    case 12:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, 5, 5, 5));
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 11, 5, 5, 5));
        break;
        
    }

    // Refresh the LED strip to send data
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
}

void turn_off_all_leds(led_strip_handle_t led_strip){
    // Turn off all LEDs
    ESP_ERROR_CHECK(led_strip_clear(led_strip));
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));  // Refresh the LED strip to send data
    printf("default_leds is off \n");
}
