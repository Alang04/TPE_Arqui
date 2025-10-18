#include "../include/idt.h"
#include <stdint.h>
#include "../include/lib.h" // Para memset

// --- Declaramos los "enchufes" de ensamblador con los NOMBRES CORRECTOS ---
extern void _irq0Handler();    // <-- Corregido
extern void _irq1Handler();    // <-- Corregido
extern void _exception0Handler();  // <-- Corregido
extern void _exception6Handler();  // <-- Corregido
extern void _int80Handler();

// --- Definición de la Estructura de la IDT ---

typedef struct {
    uint16_t    offset_l; // (0-15)
    uint16_t    selector;
    uint8_t     ist;
    uint8_t     type_attr;
    uint16_t    offset_m; // (16-31)
    uint32_t    offset_h; // (32-63)
    uint32_t    zero;
} IDTEntry_t;

// La tabla IDT (256 entradas)
static IDTEntry_t idt[256];

// El puntero IDTR que se le da al CPU
static struct {
    uint16_t  limit;
    uint64_t  base;
} __attribute__((packed)) idtr = { sizeof(idt) - 1, (uint64_t)idt };

// Función para setear una entrada en la IDT
static void setup_IDT_entry(int index, uint64_t offset) {
    idt[index].offset_l = offset & 0xFFFF;
    idt[index].selector = 0x08; // KERNEL_CS
    idt[index].ist = 0;
    idt[index].type_attr = 0x8E; // 0b10001110 (Presente, Ring 0, Interrupt Gate)
    idt[index].offset_m = (offset >> 16) & 0xFFFF;
    idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
    idt[index].zero = 0;
}

// Función principal: Carga la IDT
void load_idt() {
    memset(idt, 0, sizeof(idt)); // Limpiamos la tabla
    
    // Excepciones
    setup_IDT_entry(0x00, (uint64_t)&_exception0Handler); 
    setup_IDT_entry(0x06, (uint64_t)&_exception6Handler);

    // Hardware (IRQs)
    setup_IDT_entry(0x20, (uint64_t)&_irq0Handler); // 0x20 es el Timer
    setup_IDT_entry(0x21, (uint64_t)&_irq1Handler); // 0x21 es el Teclado

    // Syscall
    setup_IDT_entry(0x80, (uint64_t)&_int80Handler);
    // Cambiamos el DPL (Ring 3) para que User Space pueda llamarla
    idt[0x80].type_attr = 0xEE; // 0b11101110

    // --- INICIALIZACION DEL PIC (PIC Remap) ---

     // ICW1: Inicia la secuencia de inicialización
     outb(0x20, 0x11);
     outb(0xA0, 0x11);

     // ICW2: Remapea los IRQs. PIC1 (0-7) a 0x20-0x27. PIC2 (8-15) a 0x28-0x2F.
     outb(0x21, 0x20);
     outb(0xA1, 0x28);

     // ICW3: Configuración Maestro/Esclavo
     outb(0x21, 0x04); // Le dice al Maestro que tiene un esclavo en IRQ2
     outb(0xA1, 0x02); // Le dice al Esclavo su "identidad" (en cascada)

     // ICW4: Modo 8086
     outb(0x21, 0x01);
     outb(0xA1, 0x01);

     // OCW1: deshabilitar todas las interrupciones...
     outb(0x21, 0xFF);
     outb(0xA1, 0xFF);

     // HabilitarSOLO Teclado (IRQ 1) y Timer (IRQ 0)
     outb(0x21, 0xFC); // 1111 1100 -> Habilita IRQ 0 y 1

     // Cargamos la IDT en el registro del CPU
     __asm__ __volatile__("lidt %0" : : "m"(idtr));
}