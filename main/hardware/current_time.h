#ifndef CURRENT_TIME_H
#define CURRENT_TIME_H
#include "led_strip.h"

void initialize_sntp(void);
int get_current_hour(void);
int get_time_gap_to_next_hour(void);
bool is_sntp_initialized(void);

#endif  // CURRENT_TIME_H
