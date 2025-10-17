#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>

void putPixel(int x, int y, int color);
void drawChar(int x, int y, char character, int fontColor, int bgColor);
void printString(const char * str);

#endif