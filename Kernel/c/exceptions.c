#include <exceptions.h>
#include <videoDriver.h> // Para imprimir el error

// Imprime un mensaje y se congela
void exceptionDispatcher(uint64_t exception, uint64_t error_code) {
    printString("EXCEPCION RECIBIDA");
    newLine();
    
    if (exception == 0x00) {
        printString("ERROR: Division por cero");
    } else if (exception == 0x06) {
        printString("ERROR: Opcode Invalido");
    } else {
        printString("Excepcion");
    }
    
    while(1);
}