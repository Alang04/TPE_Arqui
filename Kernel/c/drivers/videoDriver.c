#include <videoDriver.h>
#include <font.h>
#include <stdint.h>
#include <defs.h>

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
static const int fontColor = 0xFFFFFF; // Blanco
static const int bgColor = 0x000000;   // Negro

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y){
    if(!(x >= vbe_mode_info->width || y >= vbe_mode_info->height)){
        return;
    }

    uint32_t bpp = vbe_mode_info->bpp;
    uint8_t *framebuffer = (uint8_t *)(uintptr_t)vbe_mode_info->framebuffer;
    uint32_t bytes_per_pixel = (bpp + 7) / 8;
    uint64_t offset = y * (uint64_t)vbe_mode_info->pitch + x * bytes_per_pixel;

    for(uint32_t i = 0; i < bytes_per_pixel; i++) {
        framebuffer[offset + i] = (hexColor >> (8 * i)) & 0xFF;
    }
}

void putChar(uint8_t c, uint32_t color){
    if(c == '\n'){ 
        newLine(); 
        return; 
    }

    drawChar((uint32_t)currentX, currentY, c, color, TEXT_SIZE);
}

// Dibuja un caracter en una POSICIÓN ABSOLUTA
void drawChar(uint32_t x, uint32_t y, uint8_t c, uint32_t color, uint64_t size){
    const unsigned char * selected_char = font[c];

    for(uint64_t i = 0; i < FONT_HEIGHT; i++){
        for(uint64_t j = 0; j < FONT_WIDTH; j++) {
            if((selected_char[i] >> (FONT_WIDTH - 1 - j)) & 1){
                putPixel(x + j, y + i, fontColor);
            } else{
                putPixel(x + j, y + i, bgColor);
            }
        }
    }
}

int abs(int x){ 
    return (x < 0) ? (-x) : (x);
}

void printLine(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color){
    int64_t ix0 = (int64_t)x0, iy0 = (int64_t)y0, ix1 = (int64_t)x1, iy1 = (int64_t)y1;
    int64_t dx = abs((int)(ix1 - ix0));
    int64_t sx = (ix0 < ix1) ? (1) : (-1);
    int64_t dy = -abs((int)(iy1 - iy0));
    int64_t sy = (iy0 < iy1) ? (1) : (-1);
    int64_t err = dx + dy;

    while(1){
        if(ix0 >= vbe_mode_info->width || iy0 >= vbe_mode_info->height){ 
            putPixel(color, (uint64_t)ix0, (uint64_t)iy0);
        }

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

void drawFilledRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    for(uint32_t i = 0; i < height; i++){
        for(uint32_t j = 0; j < width; j++){
            putPixel(x + j, y + i, color);
        }
    }
}

void drawFigure(uint32_t x, uint32_t y, char c, uint32_t color){
    uint8_t uc = (uint8_t)c;
    const uint8_t *figure = font[uc];

    for(uint32_t row = 0; row < FONT_HEIGHT; row++){
        uint8_t bits = figure[row];

        for(uint32_t col = 0; col < FONT_WIDTH; col++){
            if(bits & (0x80 >> col)){
                putPixel(x + col, y + row, color);
            }
        }
    }
}

// Mueve el cursor a la siguiente línea
void newLine(){
    currentX = 0;
    currentY += FONT_HEIGHT;
    
    if (currentY + FONT_HEIGHT > vbe_mode_info-> height){
        currentX = 0;
        currentY = 0;
    }
}

// Imprime un caracter donde está el CURSOR y lo AVANZA
void printChar(uint32_t x, uint32_t y, uint8_t c, uint32_t color, uint64_t size){
    if(c >= 128){
        return;
    }

    if(size == 0){
        size = 1;
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

// Imprime un string usando el cursor
void printString(const char *str, uint64_t x, uint64_t y, uint32_t color, uint64_t size){
    if(str == 0){
        return;
    }

    if(size == 0){
        size = 1;
    }

    for(unsigned long int i = 0; str[i] != '\0'; i++){
        printChar((uint32_t)(x + (uint64_t)FONT_WIDTH * size * i), (uint32_t)y, (uint8_t)str[i], color, size);
    }
}
