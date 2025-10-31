#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>

void putPixel(int x, int y, int color);
void drawChar(int x, int y, char character);
void printString(const char * str);
void newLine();                            
void printChar(char c);      
void sys_write_stdout(const char * str, int count);   
void drawFilledRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void drawGlyph8x16(uint32_t x, uint32_t y, char c, uint32_t color);

#endif