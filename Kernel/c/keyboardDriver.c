#include "../include/keyboardDriver.h"
#include "../include/videoDriver.h"
#include "../include/lib.h"             // Para usar inb()
#include <stdint.h>

// Mapa de scancodes (teclas "presionadas") a caracteres ASCII
// para un layout de teclado US. 0 significa "no imprimible".
static const char scancode_map[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

void keyboard_handler() {
    uint8_t scancode = inb(0x60); // Lee el scancode del puerto del teclado

    // Si el bit más alto es 1, es una tecla "liberada" (la ignoramos)
    if (scancode < 0x80) {
        char c = scancode_map[scancode];
        if (c != 0) {
            printChar(c); 
        }
    }
}