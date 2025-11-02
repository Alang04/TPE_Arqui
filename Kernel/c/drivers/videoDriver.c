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
static uint64_t defaultTextSize = TEXT_SIZE; // Tamaño por defecto (ajustable)

// API utilitaria
uint16_t getScreenWidth(void){ 
    return vbe_mode_info->width;
}

uint16_t getScreenHeight(void){
    return vbe_mode_info->height;
}

// Permite ajustar el tamaño por defecto usado por putChar
void setDefaultTextSize(uint64_t size){ 
    defaultTextSize = (size == 0) ? 1 : size;
}

uint64_t getDefaultTextSize(void){
    return defaultTextSize;
}

/* MODO TEXTO. */

void newLine(){
    currentX = 0;
    currentY += FONT_HEIGHT;
    
    if (currentY + FONT_HEIGHT > vbe_mode_info->height){
        currentX = 0;
        currentY = 0;
    }
}

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

/* MODO GRAFICO */

void drawFilledRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    for(uint32_t i = 0; i < height; i++){
        for(uint32_t j = 0; j < width; j++){
            putPixel(color, x + j, y + i);
        }
    }
}

void printLine(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color){
    int64_t ix0 = (int64_t)x0, iy0 = (int64_t)y0, ix1 = (int64_t)x1, iy1 = (int64_t)y1;
    int64_t dx = abs((int)(ix1 - ix0));
    int64_t sx = (ix0 < ix1) ? (1) : (-1);
    int64_t dy = -abs((int)(iy1 - iy0));
    int64_t sy = (iy0 < iy1) ? (1) : (-1);
    int64_t err = dx + dy;

    while(1){
        // Dibuja el punto actual (si está fuera, putPixel lo descarta)
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

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y){
    // Salir si está fuera de pantalla
    if(x >= vbe_mode_info->width || y >= vbe_mode_info->height){
        return;
    }

    uint32_t bpp = vbe_mode_info->bpp;
    uint8_t *framebuffer = (uint8_t *)(uintptr_t)vbe_mode_info->framebuffer;
    uint32_t bytes_per_pixel = (bpp + 7) / 8; // 24/32 bpp -> 3/4 bytes
    uint64_t offset = y * (uint64_t)vbe_mode_info->pitch + x * bytes_per_pixel;

    // Escritura en orden little-endian de los bytes del color
    for(uint32_t i = 0; i < bytes_per_pixel; i++) {
        framebuffer[offset + i] = (hexColor >> (8 * i)) & 0xFF;
    }
}

void putChar(uint8_t c, uint32_t color){
    if(c == '\n'){ 
        newLine(); 
        return; 
    }

    drawChar((uint32_t)currentX, (uint32_t)currentY, c, color, defaultTextSize);
    // Avanza el cursor en X según el tamaño actual
    currentX += FONT_WIDTH * (int)defaultTextSize;
    // Si se pasa del ancho, salto de línea
    if (currentX + (int)(FONT_WIDTH * defaultTextSize) > (int)vbe_mode_info->width) {
        newLine();
    }
}

// Dibuja un caracter en una POSICIÓN ABSOLUTA
void drawChar(uint32_t x, uint32_t y, uint8_t c, uint32_t color, uint64_t size){
    if (c >= 128) { return; }
    if (size == 0) { size = 1; }

    const unsigned char * selected_char = font[c];

    for(uint64_t i = 0; i < FONT_HEIGHT; i++){
        for(uint64_t j = 0; j < FONT_WIDTH; j++) {
            uint8_t bit = (selected_char[i] >> (FONT_WIDTH - 1 - j)) & 1;
            for(uint64_t dy = 0; dy < size; dy++){
                for(uint64_t dx = 0; dx < size; dx++){
                    uint32_t px = x + j*size + dx;
                    uint32_t py = y + i*size + dy;
                    if (bit) {
                        putPixel(color, px, py);
                    } else {
                        putPixel(bgColor, px, py);
                    }
                }
            }
        }
    }
}

int abs(int x){ 
    return (x < 0) ? (-x) : (x);
}

// Adaptadores respecto a las funciones declaradas en el header
void helperFilledRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    drawFilledRect(x, y, width, height, color);
}

void printFillRectangle(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color) {
    if (x1 < x0) { uint64_t t = x0; x0 = x1; x1 = t; }
    if (y1 < y0) { uint64_t t = y0; y0 = y1; y1 = t; }
    drawFilledRect((uint32_t)x0, (uint32_t)y0, (uint32_t)(x1 - x0 + 1), (uint32_t)(y1 - y0 + 1), color);
}

void printRectangle(uint64_t x0, uint64_t y0, uint64_t x1, uint64_t y1, uint32_t color) {
    // Normaliza coordenadas
    if (x1 < x0) { uint64_t t = x0; x0 = x1; x1 = t; }
    if (y1 < y0) { uint64_t t = y0; y0 = y1; y1 = t; }
    // Lados
    printLine(x0, y0, x1, y0, color);
    printLine(x1, y0, x1, y1, color);
    printLine(x1, y1, x0, y1, color);
    printLine(x0, y1, x0, y0, color);
}

void drawFigure(uint32_t x, uint32_t y, char c, uint32_t color){
    uint8_t uc = (uint8_t)c;
    const uint8_t *figure = font[uc];

    for(uint32_t row = 0; row < FONT_HEIGHT; row++){
        uint8_t bits = figure[row];

        for(uint32_t col = 0; col < FONT_WIDTH; col++){
            if(bits & (0x80 >> col)){
                putPixel(color, x + col, y + row);
            }
        }
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
            } else {
                // Opcionalmente pintar fondo del carácter
                for(uint64_t dy = 0; dy < size; dy++){
                    for(uint64_t dx = 0; dx < size; dx++){
                        putPixel(bgColor, x + (uint64_t)j * size + dx, y + (uint64_t)i * size + dy);
                    }
                }
            }
        }
    }
}

// Limpia la pantalla con el color de fondo
void clearScreen(uint32_t color) {
    drawFilledRect(0, 0, vbe_mode_info->width, vbe_mode_info->height, color);
}
