// Kernel/c/irqDispatcher.c
#include "../include/irqDispatcher.h"
#include "../include/keyboardDriver.h"
#include "../include/lib.h"             // Para outb

#define KEYBOARD_IRQ 1

void irqDispatcher(uint64_t irq){
    switch (irq){
        case KEYBOARD_IRQ:
            keyboard_handler();
            break;
    }

    // Enviamos "End of Interrupt" (EOI) al PIC para avisar que terminamos.
    outb(0x20, 0x20);
}