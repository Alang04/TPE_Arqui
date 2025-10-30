#include "../include/syscallDispatcher.h"
#include "../include/videoDriver.h"
#include "../include/keyboardDriver.h"

#define SYSCALL_WRITE 1
#define SYSCALL_READ  0

uint64_t int80Dispatcher(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx) {
    
    switch (rax) {
        case SYSCALL_READ:
            // rax=0 (read)
            // rbx=fd (file descriptor, 0 para stdin)
            
            if (rbx == 0) { // 0 = STDIN (teclado)
                printString("sys_read_keyboard\n");
                return sys_read_keyboard();
            }
            break;

        case SYSCALL_WRITE:
            if (rbx == 1) { // 1 = STDOUT
                printString("sys_write_stdout\n");
                sys_write_stdout((const char *)rcx, (int)rdx);
            }
            break;
    }
    return 0;
}