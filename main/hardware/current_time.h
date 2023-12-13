#ifndef CURRENT_TIME_H
#define CURRENT_TIME_H
#include "led_strip.h"

void initialize_sntp(void);
int get_current_hour(void);
#endif  // CURRENT_TIME_H
