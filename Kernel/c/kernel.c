#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <idtLoader.h>
#include <time.h>
#include <interrupts.h>
#include <keyboardDriver.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
static const uint64_t PageSize = 0x1000;
static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

int main(){
    _sti();

    EntryPoint userlandMain = (EntryPoint)sampleCodeModuleAddress;
	// Limpio pantalla y dibujo algo visible para validar VBE
	clearScreen(0x000000);
	printString("Kernel loaded!", 10, 10, 0xFFFFFF, 1);
	printRectangle(5, 5, 300, 60, 0x00FF00);
	// Texto con cursor por defecto
	setDefaultTextSize(1);
	putChar('\n', 0xFFFFFF);
    userlandMain();

	return 0;
}

void clearBSS(void * bssAddress, uint64_t bssSize){
	memset(bssAddress, 0, bssSize);
}

void * getStackBase(){
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8	
		- sizeof(uint64_t)
	);
}

void * initializeKernelBinary(){
	void * moduleAddresses[] ={sampleCodeModuleAddress, sampleDataModuleAddress};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	load_idt();
	return getStackBase();
}

