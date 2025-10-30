#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <idtLoader.h>
#include <timer.h>
#include <interrupts.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void printHex64(uint64_t value);

typedef int (*EntryPoint)();

int main(){
    	printString("Kernel listo. Cargando IDT...\n");
    	load_idt();

	printString("Module[0] qword: ");
	printHex64(*(uint64_t*)sampleCodeModuleAddress);
	printString("\n");
	printString("Module[0] text preview: ");
	printString((const char*)0x401000);
	printString("\n");

	printString("Habilitando interrupciones (STI)...\n");
	_sti();
	printString("STI ejecutado.\n");

	EntryPoint userlandMain = (EntryPoint)sampleCodeModuleAddress;
	printString("Saltando a User Space (0x400000)...\n\n");
	userlandMain();
    
    	printString("\nFin de la ejecucion.");
    	while(1);
    
    	return 0;
}

void clearBSS(void * bssAddress, uint64_t bssSize){
	memset(bssAddress, 0, bssSize);
}

void * getStackBase(){
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8			//The size of the stack itself, 32KiB
		- sizeof(uint64_t)		//Begin at the top of the stack
	);
}

void * initializeKernelBinary(){
	
	void * moduleAddresses[] ={
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

static void printHex64(uint64_t value) {
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