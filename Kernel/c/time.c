#include "time.h"

unsigned char getSeconds();
unsigned char getMinutes();
unsigned char getHour();
unsigned char getDayOfMonth();
unsigned char getMonth();
unsigned char getYear();

static unsigned long ticks = 0;

void sleep(int ms){
	unsigned long start = ticks;

	unsigned long target = ms / 10;

	while((ticks - start) < target){
		_hlt();
	}
}

unsigned long deltaTicks(){
	return ticks;
}

void date(unsigned char *buff){
	buff[0] = getDayOfMonth();
	buff[1] = getMonth();
	buff[2] = getYear();
}

void time(unsigned char *buff){
	buff[0] = getHour();
	buff[1] = getMinutes();
	buff[2] = getSeconds();
}

void timer_handler(){
	ticks++;
}

int ticks_elapsed(){
	return ticks;
}

int seconds_elapsed(){
	return ticks / 18;
}
