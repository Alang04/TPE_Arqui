#include "../include/exceptions.h"
#include "../include/videoDriver.h"
#include "../include/keyboardDriver.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

Exception exceptionsArray[] = {&zeroDivision, 
	0, 0, 0, 0, 0, &invalidOpcode};

char * exceptionMessage[] = { "zeroDivision Exception!", "invalidOpcode Exception!"};

void zeroDivision(){
	excepHandler(exceptionMessage[0]);
}

void invalidOpcode(){
	excepHandler(exceptionMessage[1]);
}

void exceptionDispatcher(int exception){
	if (exception == ZERO_EXCEPTION_ID){
		zeroDivision();
	} else{
		if(exception == INVALID_OPCODE_ID){
			invalidOpcode();
		}
	}
}

void excepHandler(char * msg){
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