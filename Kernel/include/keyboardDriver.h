#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H

#include <defs.h>
#include <stdint.h>

//void printPressedKey();
void writeBuff(unsigned char c);
void clearBuff();
uint64_t readBuff(char * out_buf, unsigned long maxLen);
void handlePressedKey();
uint64_t copyRegistersBuffer(char * buff);
uint8_t getFromBuffer();
uint64_t readKeyBuff(char * buff, uint64_t count);
uint32_t intToHexa(uint64_t value, char * dest);
extern uint64_t regsArray[];

#endif