#ifndef LED_H
#define LED_H

#include "led_strip.h"

#define LED_STRIP_RMT_RES_HZ (10 * 1000 * 1000)

led_strip_handle_t configure_led(void);
void default_leds(led_strip_handle_t led_strip);

#endif  // LED_H
