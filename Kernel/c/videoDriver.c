#include "../include/videoDriver.h"
#include "../include/font.h"

#define FONT_WIDTH 8
#define FONT_HEIGHT 16

struct vbe_mode_info_structure{
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
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
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

// Función para dibujar un píxel
void putPixel(int x, int y, int color) {
    // Calculamos la posición lineal del píxel en la memoria
    char *screen = (char *) (uint64_t) VBE_mode_info->framebuffer;
    int offset = (x + y * VBE_mode_info->width) * (VBE_mode_info->bpp / 8);

    // Escribimos los componentes del color (Azul, Verde, Rojo)
    screen[offset] = color & 0xFF;         		// Blue
    screen[offset + 1] = (color >> 8) & 0xFF;  	// Green
    screen[offset + 2] = (color >> 16) & 0xFF; 	// Red
}

// Nuevas variables para manejar la posición del cursor
static int currentX = 0;
static int currentY = 0;

void drawChar(int x, int y, char character, int fontColor, int bgColor) {
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

void printString(const char * str) {
    int fontColor = 0xFFFFFF; 	// Blanco
    int bgColor = 0x000000;   	// Negro

    while (*str){
        drawChar(currentX, currentY, *str, fontColor, bgColor);
        currentX += FONT_WIDTH;
        str++;
    }
}
