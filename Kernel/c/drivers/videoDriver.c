#include <videoDriver.h>
#include <font.h>
#include <stdint.h>

typedef struct vbe_mode_info_structure {
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

void putPixel(int x, int y, int color) {
    char *screen = (char *) (uint64_t) vbe_mode_info->framebuffer;
    int bytesPerPixel = vbe_mode_info->bpp / 8;
    int offset = (x + y * vbe_mode_info->width) * bytesPerPixel;

    screen[offset]     = color & 0xFF;           // Blue
    screen[offset + 1] = (color >> 8) & 0xFF;  // Green
    screen[offset + 2] = (color >> 16) & 0xFF; // Red
}

static int currentX = 0;
static int currentY = 0;
static const int fontColor = 0xFFFFFF; // Blanco
static const int bgColor = 0x000000;   // Negro

#define FONT_WIDTH 8
#define FONT_HEIGHT 16


// Dibuja un caracter en una POSICIÓN ABSOLUTA
void drawChar(int x, int y, char character) {
    const uint8_t * selected_char = font_map[(uint8_t)character];
    for (int i = 0; i < FONT_HEIGHT; i++) {
        for (int j = 0; j < FONT_WIDTH; j++) {
            if ((selected_char[i] >> (FONT_WIDTH - 1 - j)) & 1)
                putPixel(x + j, y + i, fontColor);
            else
                putPixel(x + j, y + i, bgColor);
        }
    }
}

// Mueve el cursor a la siguiente línea
void newLine(){
    currentX = 0;
    currentY += FONT_HEIGHT;
    
    // Aquí faltaría la lógica de 'scroll'
    if (currentY + FONT_HEIGHT > vbe_mode_info->height) {
        // Por ahora, simplemente reseteamos a la cima
        currentX = 0;
        currentY = 0;
        // (En el futuro aquí se debería mover toda la pantalla hacia arriba)
    }
}

// Imprime un caracter donde está el CURSOR y lo AVANZA
void printChar(char c) {
    if (c == '\n') { // Salto de línea
        newLine();
    } else if (c == '\b') { // Backspace (borrar)
        if (currentX > 0) {
            currentX -= FONT_WIDTH;
            drawChar(currentX, currentY, ' '); // Dibuja un espacio en blanco para "borrar"
        }
    } else {
        // Dibuja el caracter en la posición actual
        drawChar(currentX, currentY, c);
        // Avanza el cursor
        currentX += FONT_WIDTH;

        // Si nos pasamos del borde, saltamos de línea
        if (currentX + FONT_WIDTH > vbe_mode_info->width) {
            newLine();
        }
    }
}

// Imprime un string usando el cursor
void printString(const char * str) {
    while (*str) {
        printChar(*str);
        str++;
    }
}

void sys_write_stdout(const char * str, int count){
    for(int i = 0; i < count; i++) {
        printChar(str[i]);
    }
}