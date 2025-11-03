#ifndef _TIME_H_
#define _TIME_H_

void sleep(int ms);
unsigned long deltaTicks();
void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void date(unsigned char *buff);
void time(unsigned char *buff);

#endif
