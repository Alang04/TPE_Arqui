#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H


#include <defs.h>
#include <stdint.h>

//void printPressedKey();
void writeBuff(unsigned char c);
void clearBuff();
uint64_t readBuff(char * out_buf, unsigned long maxLen);
void handlePressedKey();
uint64_t copyRegistersBuffer(char * copy);

#endif