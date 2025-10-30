#include <keyboardDriver.h>
#include <lib.h>
#include <naiveConsole.h>
#include <videoDriver.h>

char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // backspace
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     // enter
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/', 0, '*',
    0,' ', // space bar
    // ... (resto omitido)
};

void printPressedKey(){
    while(1){
        uint8_t scancode = getPressedKey();
        if(scancode < 128){
            ncPrintChar(scancode_to_ascii[scancode]);
            printString("Presionaste una tecla\n");
        }
    }
}

// Syscall-friendly keyboard read: returns the ASCII character
// corresponding to the next pressed key (0 if none).
uint64_t sys_read_keyboard(){
    char c = 0;
    char sc = getPressedKey(); // blocks until a scancode is available
    if((unsigned char)sc < 128){
        c = scancode_to_ascii[(unsigned char)sc];
        printString("Entre a sys_read_keyboard\n");
    }
    return (uint64_t)c;
}