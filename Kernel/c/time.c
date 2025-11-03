#include "time.h"
#include "interrupts.h"

unsigned char getSeconds();
unsigned char getMinutes();
unsigned char getHour();
unsigned char getDayOfMonth();
unsigned char getMonth();
unsigned char getYear();

static unsigned long ticks = 0;

// Duerme aprox. 'ms' milisegundos usando ticks y HLT
void sleep(int ms){
	unsigned long start = ticks;

	unsigned long target = ms / 10;

	while((ticks - start) < target){
		_hlt();
	}
}

// Ticks transcurridos desde el arranque
unsigned long deltaTicks(){
	return ticks;
}

// Obtiene fecha en BCD: DD/MM/AA
void date(unsigned char *buff){
	buff[0] = getDayOfMonth();
	buff[1] = getMonth();
	buff[2] = getYear();
}

// Obtiene hora en BCD: HH:MM:SS
void time(unsigned char *buff){
	buff[0] = getHour();
	buff[1] = getMinutes();
	buff[2] = getSeconds();
}

// Incrementa el contador en cada interrupción del PIT
void timer_handler(){
	ticks++;
}

int ticks_elapsed(){
	return ticks;
}

int seconds_elapsed(){
	return ticks / 18;
}
