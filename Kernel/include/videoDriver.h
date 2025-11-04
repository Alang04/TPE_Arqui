// Header matching the implementations in Kernel/drivers/videoDriver.c
#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>

/* Screen / mode information */
uint16_t getScreenWidth(void);
uint16_t getScreenHeight(void);
int validPosition(uint64_t x, uint64_t y);

/* Text size helpers */
void setDefaultTextSize(uint64_t size);
uint64_t getDefaultTextSize(void);
void setTextSize(uint8_t size);
void increaseFontSize(void);
void decreaseFontSize(void);

/* Pixel and drawing primitives */
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void drawLine(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color);
void drawRectangle(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color);
void fillRectangle(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color);
void drawFilledRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);

/* Character / text drawing */
void videoPutChar(uint8_t c, uint32_t color);
void videoPrint(const char *str, uint32_t color);
void newLine(void);
void scroll(void);
void clearScreen(uint32_t color);

#endif