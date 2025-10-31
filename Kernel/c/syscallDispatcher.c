#include "../include/syscallDispatcher.h"
#include "../include/videoDriver.h"
#include "../include/keyboardDriver.h"
#include "../include/lib.h"

#define SYSCALL_WRITE     1
#define SYSCALL_READ      0
#define SYSCALL_GET_TIME  2

uint64_t int80Dispatcher(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx) {
    // DEBUG: entrada a syscall
    printString("[int80 enter]\n");

    switch(rax){
        case SYSCALL_READ:
            // rax=0 (read)
            // rbx=fd (file descriptor, 0 para stdin)
            
            if (rbx == 0) { // 0 = STDIN (teclado)
                printString("[int80 READ] waiting...\n");
                uint64_t c = sys_read_keyboard();
                printString("[int80 READ] done\n");
                return c;
            }
            break;

        case SYSCALL_WRITE:
            if (rbx == 1) { // 1 = STDOUT
                sys_write_stdout((const char *)rcx, (int)rdx);
                printString("[int80 WRITE] done\n");
                return rdx; // devolver cantidad de bytes escritos
            }
            break;

        case SYSCALL_GET_TIME: {
            // rcx = puntero a buffer de 3 bytes (hh, mm, ss)
            uint8_t *buf = (uint8_t *)rcx;
            if (buf != 0) {
                getTime(buf);
                printString("[int80 TIME] done\n");
                return 0;
            }
            break;
        }
    }
    printString("[int80 exit]\n");
    return 0;
}