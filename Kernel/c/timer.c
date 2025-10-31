#include <time.h>
#include <stdint.h>

static unsigned long ticks = 0;
static int flagReported = 0;
static uint64_t observedFlagValue = 0;

void timer_handler(){
	ticks++;
	if (!flagReported) {
		volatile uint64_t *sharedFlag = (uint64_t *)0x600000;
		uint64_t value = *sharedFlag;
		if (value == 0x1BADB002 || value == 0x2BADB002) {
			observedFlagValue = value;
			flagReported = 1;
		}
	}
}

int seconds_elapsed(){
	return ticks / 18;
}

int ticks_elapsed(){
	return ticks;
}

int timer_flag_observed(){
	return flagReported;
}

uint64_t timer_flag_value(){
	return observedFlagValue;
}