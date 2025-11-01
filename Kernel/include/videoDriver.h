#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>

uint16_t getScreenHeight(void);
uint16_t getScreenWidth(void);
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void printChar(uint32_t x, uint32_t y, uint8_t c, uint32_t color, uint64_t size);
void printString(const char *str, uint64_t x, uint64_t y, uint32_t color, uint64_t size);
void printLine(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color);
void printRectangle(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color);
void printFillRectangle(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color);
void helperFilledRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void putChar(uint8_t c, uint32_t color);
void drawChar(uint32_t x, uint32_t y, uint8_t c, uint32_t color, uint64_t size);
int abs(int x);
void drawFigure(uint32_t x, uint32_t y, char c, uint32_t color);
void newLine();

#endif