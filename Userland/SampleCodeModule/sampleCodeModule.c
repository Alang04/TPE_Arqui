#include <stdint.h>

static uint64_t _int80(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx) {
    uint64_t ret;
    __asm__ __volatile__(
        "int $0x80"
        : "=a" (ret)
        : "a" (rax), "b" (rbx), "c" (rcx), "d" (rdx)
        : "memory"
    );
    return ret;
}

#define SYSCALL_WRITE 1
#define SYSCALL_READ  0
#define STDOUT 1
#define STDIN  0

static int strlen(const char * str) {
    int i = 0;
    while (str[i]) i++;
    return i;
}

void printf(const char * str) {
    _int80(SYSCALL_WRITE, STDOUT, (uint64_t)str, strlen(str));
}

char read() {
    char c = 0;
    // Hacemos polling (preguntamos en bucle) hasta que la syscall
    // nos devuelva un caracter que no sea 0
    while(c == 0) {
        c = _int80(SYSCALL_READ, STDIN, 0, 0);
    }
    return c;
}

int main() {
    
    printf("Hola desde User space\n");

    char c;
    char buffer[2] = {0}; // Un buffer para imprimir "c" como un string

    while(1) {
        // 1. Pedimos una tecla al Kernel
        c = read();
        
        // 2. Convertimos la tecla a un string
        buffer[0] = c;
        buffer[1] = 0;
        
        // 3. Le pedimos al Kernel que la imprima
        printf(buffer);
    }
    
    return 0;
}