#include "../include/videoDriver.h"
#include "../include/font.h"
#include <stdint.h>
#include "../include/defs.h"
#include <string.h>

typedef struct vbe_mode_info_structure{
    uint16_t attributes;
    uint8_t window_a;
    uint8_t window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t w_char;
    uint8_t y_char;
    uint8_t planes;
    uint8_t bpp;
    uint8_t banks;
    uint8_t memory_model;
    uint8_t bank_size;
    uint8_t image_pages;
    uint8_t reserved0;
    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;
    uint32_t framebuffer;
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;
    uint8_t reserved1[206];
} __attribute__ ((packed)) vbe_mode_info_t;

static vbe_mode_info_t * vbe_mode_info = (vbe_mode_info_t *) 0x5C00;

static int currentX = 0;
static int currentY = 0;
static const int bgColor = 0x000000;   // Negro
static uint64_t defaultTextSize = TEXT_SIZE; // Tamaño por defecto (ajustable)

// API utilitaria
uint16_t getScreenWidth(){ 
    return vbe_mode_info->width;
}

uint16_t getScreenHeight(){
    return vbe_mode_info->height;
}

// Permite ajustar el tamaño por defecto usado por putChar
void setDefaultTextSize(uint64_t size){ 
    defaultTextSize = (size == 0) ? (1) : (size);
}

uint64_t getDefaultTextSize(void){
    return defaultTextSize;
}

int validPosition(uint64_t x,  uint64_t y){
    return x < vbe_mode_info->width && y < vbe_mode_info->height;
}

/* FUNCIONES DEMODO TEXTO. */

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y){
    if(!validPosition(x, y)){
        return;
    }

    uint8_t *framebuffer = (uint8_t *)(uintptr_t)vbe_mode_info->framebuffer;
    uint32_t bpp = vbe_mode_info->bpp;

    uint32_t bytesPerPixel = (bpp + 7) / 8;
    uint64_t offset = y * (uint64_t)vbe_mode_info->pitch + x * bytesPerPixel;

    for(uint32_t i = 0; i < bytesPerPixel; i++){
        framebuffer[offset + i] = (hexColor >> (8 * i)) & 0xFF;
    }
}

void scroll(){
    uint64_t lineHeight = defaultTextSize * FONT_HEIGHT;
    uint8_t * framebuffer = (uint8_t *) vbe_mode_info->framebuffer;
    
    for(uint64_t srcY = lineHeight; srcY < vbe_mode_info->height; srcY++){
        uint64_t dstY = srcY - lineHeight;
        uint64_t srcOffset = srcY * vbe_mode_info->pitch;
        uint64_t dstOffset = dstY * vbe_mode_info->pitch;
        memcpy(framebuffer + dstOffset, framebuffer + srcOffset, vbe_mode_info->pitch);
    }

    uint64_t lastLineStart = vbe_mode_info->height - lineHeight;

    fillRectangle(0, lastLineStart, vbe_mode_info->width, vbe_mode_info->height, bgColor);
}

void newLine(){
    currentX = 0;

    uint64_t stepY = (uint64_t)defaultTextSize * FONT_HEIGHT;

    if(currentY + stepY < vbe_mode_info->height){

        currentY += stepY;
        fillRectangle(0, currentY, vbe_mode_info->width, currentY + stepY, bgColor);

    } else{

        scroll();
        currentY = vbe_mode_info->height - stepY;
    }
}

void videoPutChar(uint8_t c, uint32_t color){
    if(c == '\n' || c == '\r'){ 
        newLine();
        return;
    }

    
    if(c == '\b'){
        uint64_t stepX = FONT_WIDTH * defaultTextSize;
        uint64_t stepY = FONT_HEIGHT * defaultTextSize;
        if(currentX >= stepX){
            currentX -= stepX;
            fillRectangle(currentX, currentY, currentX + stepX, currentY + stepY, bgColor);
            updateCursor();
        }
        return;
    }

    drawChar((uint32_t)currentX, currentY, c, color, defaultTextSize);
    moveRight();
    updateCursor();
}

void videoPrint(const char *str, uint32_t color){
    if(str == 0){
        return;
    }
    
    for(unsigned int i = 0; str[i] != '\0'; i++){
        videoPutChar((uint8_t)str[i], color);
    }
}

void moveRight(){
    uint64_t stepX = (uint64_t)FONT_WIDTH * defaultTextSize;
    if(currentX + stepX < vbe_mode_info->width){
        currentX += stepX;
    } else{
        newLine();
    }
}

void updateCursor(){
    if(!validPosition(currentX + (uint64_t)FONT_WIDTH * defaultTextSize - 1, currentY + (uint64_t)FONT_HEIGHT * defaultTextSize - 1)){
        newLine();
    }
}

void printString(const char *str, uint64_t x, uint64_t y, uint32_t color, uint64_t size){
    if(str == 0){
        return;
    }

    for(unsigned long int i = 0; str[i] != '\0'; i++){
        drawChar((uint32_t)(x + (uint64_t)FONT_WIDTH * size * i), (uint32_t)y, (uint8_t)str[i], color, size);
    }
}



/* MODO GRAFICO */

int abs(int x){ 
    return (x < 0) ? (-x) : (x);
}

void drawLine(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color){
    int64_t ix0 = (int64_t)x0, iy0 = (int64_t)y0, ix1 = (int64_t)x1, iy1 = (int64_t)y1;
    int64_t dx = abs((int)(ix1 - ix0));
    int64_t sx = (ix0 < ix1) ? (1) : (-1);
    int64_t dy = -abs((int)(iy1 - iy0));
    int64_t sy = (iy0 < iy1) ? (1) : (-1);
    int64_t err = dx + dy;

    while(1){
        putPixel(color, (uint64_t)ix0, (uint64_t)iy0);

        if((iy0 == iy1) && (ix0 == ix1)){
            break;
        }

        int64_t e2 = 2 * err;

        if(e2 >= dy){
            err += dy; ix0 += sx; 
        }

        if(e2 <= dx){
            err += dx; iy0 += sy;
        }
    }
}

void fillRectangle(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color) {
    if (x1 <= x0 || y1 <= y0){
        return;
    }

    if(!validPosition(x0, y0)){
        return;
    }
    
    if (x1 > vbe_mode_info->width) x1 = vbe_mode_info->width;
    if (y1 > vbe_mode_info->height) y1 = vbe_mode_info->height;

    uint32_t bytes_per_pixel = (vbe_mode_info->bpp + 7) / 8;
    uint8_t *framebuffer = (uint8_t *)(uintptr_t)vbe_mode_info->framebuffer;
    uint64_t pitch = vbe_mode_info->pitch;

    for(uint64_t y = y0; y < y1; y++){
        uint64_t row_offset = y * pitch + x0 * bytes_per_pixel;
        
        for(uint64_t x = x0; x < x1; x++){
            uint64_t off = row_offset + (x - x0) * bytes_per_pixel;
            
            for(uint32_t b = 0; b < bytes_per_pixel; b++){
                framebuffer[off + b] = (color >> (8 * b)) & 0xFF;
            }

        }
    }
}

void drawRectangle(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color) {
    // Normaliza coordenadas
    if(x1 < x0){ 
        uint64_t t = x0; x0 = x1; x1 = t;
    }

    if(y1 < y0){
        uint64_t t = y0; y0 = y1; y1 = t;
    }

    // Lados
    drawLine(x0, y0, x1, y0, color);
    drawLine(x1, y0, x1, y1, color);
    drawLine(x1, y1, x0, y1, color);
    drawLine(x0, y1, x0, y0, color);
}

void drawFilledRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color){
    fillRectangle(x, y, (uint64_t)x + width, (uint64_t)y + height, color);
}

void drawString(const char *str, uint64_t x, uint64_t y, uint32_t color, uint64_t size){
    if (str == 0){
        return;
    }

    for(unsigned int i = 0; str[i] != '\0'; i++){
        drawChar((uint32_t)(x + (uint64_t)FONT_WIDTH * size * i), (uint32_t)y, (uint8_t)str[i], color, size);
    }
}

void drawChar(uint32_t x, uint32_t y, uint8_t c, uint32_t color, uint64_t size){
    if (c >= 128){
        return;
    }

    for(int i = 0; i < FONT_HEIGHT; i++){

        uint8_t line = font[c][i];

        for(int j = 0; j < FONT_WIDTH; j++){

            if((line << j) & 0x80){

                for(uint64_t dy = 0; dy < size; dy++){

                    for(uint64_t dx = 0; dx < size; dx++){

                        putPixel(color, x + (uint64_t)j * size + dx, y + (uint64_t)i * size + dy);

                    }
                }
            }
        }
    }
}

void clearScreen(uint32_t color){
    drawFilledRect(0, 0, vbe_mode_info->width, vbe_mode_info->height, color);
}