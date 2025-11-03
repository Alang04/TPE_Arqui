#include "../include/exceptions.h"
#include "../include/videoDriver.h"
#include "../include/keyboardDriver.h"
#include "../include/interrupts.h"

Exception exceptionsArray[] = 
	{&zeroDivision, 
	0, 
	0, 
	0, 
	0, 
	0, 
	&invalidOpcode};

char * exceptionMessage[] = { "zeroDivision Exception!", "invalidOpcode Exception!"};

void zeroDivision(){
	exceptionHandler(exceptionMessage[0]);
}

void invalidOpcode(){
	exceptionHandler(exceptionMessage[1]);
}

// Llama al handler de la excepción si existe
void exceptionDispatcher(int exception){
	Exception ex = exceptionsArray[exception];

	if(ex != 0){
		ex();
	}
}

// Mensaje de error y espera a ENTER para continuar
void exceptionHandler(char * msg){
	newLine();
	videoPrint(msg, 0xFFFFFF);
	newLine();
	videoPrint("Presiona ENTER para continuar...", 0xFFFFFF);
	newLine();

	_sti();
	int c;

	do{
		_hlt();
	}while((c = getFromBuffer()) != '\n');

	newLine();
}