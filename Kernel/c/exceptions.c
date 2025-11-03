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

void exceptionDispatcher(int exception){
	Exception ex = exceptionsArray[exception];

	if(ex != 0){
		ex();
	}
}

void exceptionHandler(char * msg){
	newLine();
	videoPrint(msg, 0xFFFFFF);
	newLine();
	videoPrint("Presiona ENTER para continuar...", 0xFFFFFF);

	_sti();
	int c;

	do{
		_hlt();
	}while((c = getFromBuffer()) != '\n');
}