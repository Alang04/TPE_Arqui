#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>

void putPixel(int x, int y, int color);
void drawChar(int x, int y, char character);
void printString(const char * str);
void newLine();                            
void printChar(char c);                     

#endif