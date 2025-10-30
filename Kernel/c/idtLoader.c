#include <stdint.h>
#include <idtLoader.h>
#include <interrupts.h>
#include <defs.h>
#include <lib.h>
#include <videoDriver.h>

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

#define IDT_ENTRIES 256

static DESCR_INT idt[IDT_ENTRIES] __attribute__((aligned(0x10)));

typedef struct {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) IDTR;

static IDTR idtr = { sizeof(idt) - 1, (uint64_t)idt };

static inline uint8_t inb_local(uint16_t port) {
	uint8_t value;
	__asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

static inline void outb_local(uint16_t port, uint8_t value) {
	__asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static void remapPIC(void);
static void printHex64_local(uint64_t value);

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

	IDTR currentIdtr;
	__asm__ volatile ("sidt %0" : "=m"(currentIdtr));

	uint32_t bytesToCopy = currentIdtr.limit + 1;
	if (bytesToCopy > sizeof(idt)) {
		bytesToCopy = sizeof(idt);
	}
	memcpy(idt, (void*)currentIdtr.base, bytesToCopy);
	if (bytesToCopy < sizeof(idt)) {
		memset((uint8_t*)idt + bytesToCopy, 0, sizeof(idt) - bytesToCopy);
	}

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
	printString("IDT[0x80] handler: ");
	printHex64_local((uint64_t)&syscallIntRoutine);
	printString("\n");
	
	__asm__ volatile ("lidt %0" : : "m"(idtr));
	printString("IDT cargada.\n");

	remapPIC();

	// Enable timer tick and keyboard interruptions
	picMasterMask(0b11111100); 
	picSlaveMask(0b11111111);
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

static void remapPIC(void) {
	const uint8_t PIC_MASTER_OFFSET = 0x20;
	const uint8_t PIC_SLAVE_OFFSET = 0x28;
	uint8_t masterMask = inb_local(0x21);
	uint8_t slaveMask = inb_local(0xA1);

	outb_local(0x20, 0x10 | 0x01);
	outb_local(0xA0, 0x10 | 0x01);
	outb_local(0x21, PIC_MASTER_OFFSET);
	outb_local(0xA1, PIC_SLAVE_OFFSET);
	outb_local(0x21, 0x04);
	outb_local(0xA1, 0x02);
	outb_local(0x21, 0x01);
	outb_local(0xA1, 0x01);

	outb_local(0x21, masterMask);
	outb_local(0xA1, slaveMask);
}

static void printHex64_local(uint64_t value) {
	char buffer[19];
	buffer[0] = '0';
	buffer[1] = 'x';
	for (int i = 0; i < 16; i++) {
		uint8_t nibble = (value >> (60 - i * 4)) & 0xF;
		buffer[2 + i] = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
	}
	buffer[18] = '\0';
	printString(buffer);
}
