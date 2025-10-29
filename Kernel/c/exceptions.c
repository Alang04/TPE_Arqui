#include <exceptions.h>
#include <videoDriver.h> // Para imprimir el error

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
	exceptionMsg("Cannot Divide By Zero!");
}