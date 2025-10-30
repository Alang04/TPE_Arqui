#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
int timer_flag_observed();
uint64_t timer_flag_value();

#endif