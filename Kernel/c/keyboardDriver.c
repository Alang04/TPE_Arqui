#include "../include/keyboardDriver.h"
#include "../include/videoDriver.h"
#include "../include/lib.h"
#include <stdint.h>

static const char scancode_map[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

static char key_buffer = 0;

void keyboard_handler() {
    uint8_t scancode = inb(0x60);
    
    if (scancode < 0x80) { // Solo teclas "presionadas"
        char c = scancode_map[scancode];
        if (c != 0) {
            // Guardamos la tecla en el buffer
            key_buffer = c; 
        }
    }
}

char sys_read_keyboard() {
    if (key_buffer != 0) {
        char c = key_buffer;
        key_buffer = 0; // "Vaciamos" el buzón
        return c;
    }
    return 0; // No hay tecla
}