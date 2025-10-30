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

static void putchar(char c) {
    _int80(SYSCALL_WRITE, STDOUT, (uint64_t)&c, 1);
}

char read() {
    char c = 0;
    while (c == 0) {
        c = _int80(SYSCALL_READ, STDIN, 0, 0);
    }
    return c;
}

int main() {
    printf("Bienvenido a userland!\n");
    printf("Presione teclas (q para salir).\n");

    while (1){
        char c = read();

        if(c == 'q' || c == 'Q'){
            printf("Saliendo de userland...\n");
            break;
        }

        printf("Tecla presionada: ");
        putchar(c);
        printf("\n");
    }

    return 0;
}