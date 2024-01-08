#include "led.h"
#include"../api/solarApi.h"
#include <math.h>
/* Pint Map

    P0 = 7h = 19h
    P1 = 8h = 20h
    P2 = 9h = 21h
    P3 = 10h = 22h
    P4 = 11h = 23h
    P5 = 12h = 24h = 0h
    P6 = 13h = 1h
    P7 = 14h = 2h
    P8 = 15h = 3h
    P9 = 16h = 4h
    P10 = 17h = 5h
    P11 = 18h = 6h
*/

int red = 70;
int green = 70;
int blue = 0;

led_strip_handle_t configure_current_time_led(void)
{
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = 16,   // GPIO 16
        .max_leds = 12,        // Set the number of LEDs to 12
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

led_strip_handle_t configure_energy_production_time_led(void)
{
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = 17,   // GPIO 17
        .max_leds = 12,        // Set the number of LEDs to 12
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

led_strip_handle_t configure_mass_devices_time_led(void)
{
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = 18,   // GPIO 18
        .max_leds = 12,        // Set the number of LEDs to 12
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

led_strip_handle_t configure_light_devices_time_led(void)
{
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = 19,   // GPIO 19
        .max_leds = 12,        // Set the number of LEDs to 12
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
    for (int i = 0; i < 12; i++) {
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, 50, 50, 50));  // Turn on all LEDs
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
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, red, green, blue));
        break;
    case 1:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, red, green, blue));
        break;
    case 2:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, red, green, blue));
        break;
    case 3:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, red, green, blue));
        break;
    case 4:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, red, green, blue));
        break;
    case 5:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, red, green, blue));
        break;
    case 6:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 11, red, green, blue));
        break;
    case 7:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, red, green, blue));
        break;
    case 8:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, red, green, blue));
        break;
    case 9:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, red, green, blue));
        break;
    case 10:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, red, green, blue));
        break;
    case 11:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, red, green, blue));
        break;
    case 12:
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, red, green, blue));
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

void display_time_for_mass_devices(led_strip_handle_t led_strip){
    for(int i = 0; i < MAX_DATA_POINTS; i++){
        int hour = atoi(solarData[i].timestamp + 11);
        int avgValue = get_avg_value();
        int value = solarData[i].value;
        if(hour > 12){
            hour -= 12;
        }
        if(value >= avgValue){
            //Display hour
            switch (hour)
            {
            case 0:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, red, green, blue));
                break;
            case 1:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, red, green, blue));
                break;
            case 2:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, red, green, blue));
                break;
            case 3:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, red, green, blue));
                break;
            case 4:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, red, green, blue));
                break;
            case 5:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, red, green, blue));
                break;
            case 6:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 11, red, green, blue));
                break;
            case 7:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, red, green, blue));
                break;
            case 8:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, red, green, blue));
                break;
            case 9:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, red, green, blue));
                break;
            case 10:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, red, green, blue));
                break;
            case 11:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, red, green, blue));
                break;
            case 12:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, red, green, blue));
                break;
            } 
        }
       
    }
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
    printf("display_time_for_mass_devices completed\n");
}

void display_time_for_light_devices(led_strip_handle_t led_strip){
     for(int i = 0; i < MAX_DATA_POINTS; i++){
        int hour = atoi(solarData[i].timestamp + 11);
        int avgValue = get_avg_value() * 0.5;
        int value = solarData[i].value;
        if(hour > 12){
            hour -= 12;
        }
        if(value >= avgValue){
            //Display hour
            switch (hour)
            {
            case 0:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, red, green, blue));
                break;
            case 1:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, red, green, blue));
                break;
            case 2:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, red, green, blue));
                break;
            case 3:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, red, green, blue));
                break;
            case 4:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, red, green, blue));
                break;
            case 5:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, red, green, blue));
                break;
            case 6:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 11, red, green, blue));
                break;
            case 7:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, red, green, blue));
                break;
            case 8:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, red, green, blue));
                break;
            case 9:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, red, green, blue));
                break;
            case 10:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, red, green, blue));
                break;
            case 11:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, red, green, blue));
                break;
            case 12:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, red, green, blue));
                break;
            } 
        }
       
    }
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
    printf("display_time_for_light_devices completed\n");
}

void display_energy_production(led_strip_handle_t led_strip){
    int preValue = 0;
    int preHour = 0;
   for(int i = 0; i < MAX_DATA_POINTS; i++){
        int red_color = red;
        int green_color = green;
        int blue_color = blue;

        int hour = atoi(solarData[i].timestamp + 11);
        int value = solarData[i].value;


        if(hour > 12){
            hour -= 12;
        }

        if(i >= 1){
            preValue = solarData[i - 1].value;
            preHour = atoi(solarData[i - 1].timestamp + 11);
        }
        
        

        if(value > 0)
        {
            if(value > preValue &&  hour > preHour && preValue != 0 && preHour != 0){
                red_color += 10;
                green_color += 10;
            }

            if(value < preValue && hour < preHour){
                red_color -= 10;
                green_color -= 10;
            }
            
            printf("red %d green %d \n", red_color, green_color);

            //Display hour
            switch (hour)
            {
            case 0:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, red_color, green_color, blue_color));
                break;
            case 1:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 6, red_color, green_color, blue_color));
                break;
            case 2:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 7, red_color, green_color, blue_color));
                break;
            case 3:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 8, red_color, green_color, blue_color));
                break;
            case 4:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 9, red_color, green_color, blue_color));
                break;
            case 5:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 10, red_color, green_color, blue_color));
                break;
            case 6:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 11, red_color, green_color, blue_color));
                break;
            case 7:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 0, red_color, green_color, blue_color));
                break;
            case 8:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 1, red_color, green_color, blue_color));
                break;
            case 9:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 2, red_color, green_color, blue_color));
                break;
            case 10:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 3, red, green, blue));
                break;
            case 11:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 4, red, green, blue));
                break;
            case 12:
                ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, 5, red, green, blue));
                break;
            } 
        }
       
    }
    ESP_ERROR_CHECK(led_strip_refresh(led_strip));
    printf("display_energy_production completed\n");
}
