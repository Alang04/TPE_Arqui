#include <exceptions.h>
#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0

static void zero_division();

void exceptionDispatcher(int exception){
    printString("EXCEPCION RECIBIDA");
    
    if(exception == ZERO_EXCEPTION_ID){
        zero_division();
    }
}

static void zero_division(){
	_sti();
	printString("Cannot Divide By Zero!");
}