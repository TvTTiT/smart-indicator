#ifndef LED_H
#define LED_H

#include "led_strip.h"

#define LED_STRIP_RMT_RES_HZ (10 * 1000 * 1000)

led_strip_handle_t configure_mass_devices_time_led_1(void);
led_strip_handle_t configure_light_devices_time_led_1(void);
led_strip_handle_t configure_mass_devices_time_led_2(void);
led_strip_handle_t configure_light_devices_time_led_2(void);
led_strip_handle_t configure_leaf_led(void);
void default_leds(led_strip_handle_t led_strip);
void turn_off_all_leds(led_strip_handle_t led_strip);
void display_time_for_mass_devices(led_strip_handle_t led_strip);
void display_time_for_light_devices(led_strip_handle_t led_strip);
void display_no_wifi_conection(led_strip_handle_t led_strip);
void display_leaf_led(led_strip_handle_t led_strip);
#endif  // LED_H
