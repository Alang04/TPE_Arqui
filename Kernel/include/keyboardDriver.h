#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H

#include <stdint.h>

void printPressedKey();
uint64_t sys_read_keyboard();
void readLineVBE(char *buffer, unsigned long maxLen, uint32_t *x, uint32_t y, uint32_t color);
char readKeyAsciiBlockingVBE(uint32_t *x, uint32_t y, uint32_t color);
void keyboard_irq_producer(void);

#endif