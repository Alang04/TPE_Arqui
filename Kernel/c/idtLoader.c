#include <stdint.h>
#include <idtLoader.h>
#include <interrupts.h>
#include <defs.h>

#pragma pack(push)		/* Push de la alineación actual */
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
	uint16_t offset_l;
	uint16_t selector;
	uint8_t cero;
	uint8_t access;
	uint16_t offset_m;
	uint32_t offset_h;
	uint32_t other_cero;
} DESCR_INT;

#pragma pack(pop)		/* Reestablece la alinceación actual */

DESCR_INT * idt = (DESCR_INT *) 0;	// IDT de 255 entradas

extern void _irq00Handler(void);
extern void _irq01Handler(void);
extern void _irq02Handler(void);
extern void _irq03Handler(void);
extern void _irq04Handler(void);
extern void _irq05Handler(void);
extern void _exception0Handler(void);
extern void syscallIntRoutine(void);

static void setup_IDT_entry (int index, uint64_t offset, uint8_t access);

void load_idt(void) {
	_cli();

	// Setup exception interrupts
	setup_IDT_entry(0x00, (uint64_t)&_exception0Handler, ACS_INT);

	// Setup hardware interrupts
	setup_IDT_entry(0x20, (uint64_t)&_irq00Handler, ACS_INT);
	setup_IDT_entry(0x21, (uint64_t)&_irq01Handler, ACS_INT);
	setup_IDT_entry(0x22, (uint64_t)&_irq02Handler, ACS_INT);
	setup_IDT_entry(0x23, (uint64_t)&_irq03Handler, ACS_INT);
	setup_IDT_entry(0x24, (uint64_t)&_irq04Handler, ACS_INT);
	setup_IDT_entry(0x25, (uint64_t)&_irq05Handler, ACS_INT);
    
    // Setup syscall interrupt
	setup_IDT_entry(0x80, (uint64_t)&syscallIntRoutine, ACS_INT | ACS_DPL_3);
	
	// Enable timer tick and keyboard interruptions
	picMasterMask(0b11111100); 
	picSlaveMask(0b11111111);
	
	_sti();
}

static void setup_IDT_entry (int index, uint64_t offset, uint8_t access) {
	idt[index].offset_l = offset & 0xFFFF;
	idt[index].selector = 0x08;
	idt[index].cero = 0;
	idt[index].access = access;
	idt[index].offset_m = (offset >> 16) & 0xFFFF;
	idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
	idt[index].other_cero = (uint64_t) 0;
}
