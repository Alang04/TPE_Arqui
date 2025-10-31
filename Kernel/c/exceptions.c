#include <exceptions.h>
#include <videoDriver.h>
#include <interrupts.h>
#include <stdint.h>

#define ZERO_EXCEPTION_ID 0

static void zero_division();

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

void exceptionDispatcher(int exception, uint64_t *stackPtr){
    printString("\n[EXCEPCION] ");
    if(exception == ZERO_EXCEPTION_ID){
        printString("Division por cero\n");
    } else if (exception == 6){
        printString("Codigo de operacion invalido\n");
    } else {
        printString("ID: ");
        printHex64_local(exception);
        printString("\n");
    }

    // Layout tras pushState (de menor a mayor dir):
    // [0]: r15, [1]: r14, [2]: r13, [3]: r12, [4]: r11, [5]: r10, [6]: r9, [7]: r8,
    // [8]: rsi, [9]: rdi, [10]: rbp, [11]: rdx, [12]: rcx, [13]: rbx, [14]: rax
    uint64_t *r = stackPtr;
    printString("RAX="); printHex64_local(r[14]); printString("  RBX="); printHex64_local(r[13]); printString("  RCX="); printHex64_local(r[12]); printString("  RDX="); printHex64_local(r[11]); printString("\n");
    printString("RSI="); printHex64_local(r[8]);  printString("  RDI="); printHex64_local(r[9]);  printString("  RBP="); printHex64_local(r[10]); printString("  RSP="); printHex64_local((uint64_t)stackPtr); printString("\n");
    printString("R8 ="); printHex64_local(r[7]);  printString("  R9 ="); printHex64_local(r[6]);  printString("  R10="); printHex64_local(r[5]);  printString("  R11="); printHex64_local(r[4]); printString("\n");
    printString("R12="); printHex64_local(r[3]);  printString("  R13="); printHex64_local(r[2]);  printString("  R14="); printHex64_local(r[1]);  printString("  R15="); printHex64_local(r[0]); printString("\n");

    // RIP está en el frame de IRETQ, justo después de nuestros 15 registros
    uint64_t rip = stackPtr[15];
    printString("RIP="); printHex64_local(rip); printString("\n");

    if(exception == ZERO_EXCEPTION_ID){
        zero_division();
    }
}

static void zero_division(){
	printString("Cannot Divide By Zero!");
}