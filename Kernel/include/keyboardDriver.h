#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H

#include <stdint.h>

void printPressedKey();
void readLineVBE(char *buffer, unsigned long maxLen, uint32_t *x, uint32_t y, uint32_t color);
char readKeyAsciiBlockingVBE(uint32_t *x, uint32_t y, uint32_t color);
void keyboard_irq_producer(void);
void write(unsigned char c);
void erase();
void clear();
uint64_t copyRegistersBuffer(char * copy);

#endif