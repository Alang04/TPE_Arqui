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
    volatile uint64_t *sharedFlag = (uint64_t *)0x600000;
    *sharedFlag = 0x1BADB002;
        c = _int80(SYSCALL_READ, STDIN, 0, 0);
    }
    return c;
}

int main() {
    volatile char *video = (char *)0xB8000;
    video[0] = 'U';
    video[1] = 0x0F;

    volatile uint64_t *sharedFlag = (uint64_t *)0x600000;
    *sharedFlag = 0xFEEDBEEFCAFEBEEFULL;

    return 0; // Temporarily return to kernel for debugging
}