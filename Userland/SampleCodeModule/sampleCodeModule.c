#include <stdint.h>

static inline uint64_t _int80(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
    register uint64_t rax __asm__("rax") = a;
    register uint64_t rbx __asm__("rbx") = b;
    register uint64_t rcx __asm__("rcx") = c;
    register uint64_t rdx __asm__("rdx") = d;
    __asm__ __volatile__(
        "int $0x80"
        : "+a" (rax)
        : "b" (rbx), "c" (rcx), "d" (rdx)
        : "memory", "cc"
    );
    return rax;
}

#define SYSCALL_READ       0
#define SYSCALL_WRITE      1
#define SYSCALL_GET_TIME   2

#define STDIN  0
#define STDOUT 1

static int strlen(const char * str) {
    int i = 0;
    while (str[i]) i++;
    return i;
}

static int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

static void printf(const char * str) {
    _int80(SYSCALL_WRITE, STDOUT, (uint64_t)str, strlen(str));
}

static void putchar(char c) {
    _int80(SYSCALL_WRITE, STDOUT, (uint64_t)&c, 1);
}

static char getchar() {
    char c = 0;
    while (c == 0) {
        _int80(SYSCALL_WRITE, STDOUT, (uint64_t)"[usr] before read\n", 18);
        c = _int80(SYSCALL_READ, STDIN, 0, 0);
        if (c) {
            char msg[24] = "[usr] after read: X\n";
            msg[18] = c ? c : '?';
            _int80(SYSCALL_WRITE, STDOUT, (uint64_t)msg, 21);
        }
    }
    return c;
}

static void print_prompt() {
    printf("> ");
}

static void get_time(uint8_t buf[3]) {
    _int80(SYSCALL_GET_TIME, 0, (uint64_t)buf, 0);
}

static int bcd_to_bin(uint8_t b) {
    return (b & 0x0F) + ((b >> 4) & 0x0F) * 10;
}

static void read_line(char *buf, int maxlen) {
    int pos = 0;
    while (1) {
        char c = getchar();
        if (c == '\r') c = '\n';
        if (c == '\n') {
            putchar('\n');
            break;
        }
        if (c == '\b') {
            if (pos > 0) {
                pos--;
                // borrar del display
                putchar('\b');
            }
            continue;
        }
        if (pos < maxlen - 1 && (unsigned char)c >= 32) {
            buf[pos++] = c;
            putchar(c);
        }
    }
    buf[pos] = 0;
}

static void cmd_help() {
    printf("Comandos disponibles:\n");
    printf("  help       - lista comandos\n");
    printf("  time       - muestra hora\n");
    printf("  div0       - provoca division por cero\n");
    printf("  invop      - ejecuta instruccion invalida\n");
    printf("  regs       - muestra registros (TODO)\n");
    printf("  fontsize + - cambia tamano de fuente (stub)\n");
}

static void cmd_time() {
    uint8_t t[3];
    get_time(t);
    int hh = bcd_to_bin(t[0]);
    int mm = bcd_to_bin(t[1]);
    int ss = bcd_to_bin(t[2]);
    char out[32];
    // formato HH:MM:SS
    out[0] = '0' + (hh/10); out[1] = '0' + (hh%10); out[2] = ':';
    out[3] = '0' + (mm/10); out[4] = '0' + (mm%10); out[5] = ':';
    out[6] = '0' + (ss/10); out[7] = '0' + (ss%10); out[8] = '\n'; out[9] = 0;
    printf(out);
}

static void cmd_div0() {
    // Genera #DE
    __asm__ __volatile__(
        "xor %%rdx, %%rdx\n\t"
        "mov $1, %%rax\n\t"
        "div %%rdx\n\t"
        : : : "rax", "rdx"
    );
}

static void cmd_invop() {
    // Genera #UD
    __asm__ __volatile__("ud2");
}

int main() {
    printf("Bienvenido! Escriba 'help' para listar comandos.\n");
    char line[128];
    while (1) {
        print_prompt();
        read_line(line, sizeof(line));
        if (line[0] == 0) continue;
        if (strcmp(line, "help") == 0) { cmd_help(); continue; }
        if (strcmp(line, "time") == 0) { cmd_time(); continue; }
        if (strcmp(line, "div0") == 0) { cmd_div0(); continue; }
        if (strcmp(line, "invop") == 0) { cmd_invop(); continue; }
        if (strcmp(line, "regs") == 0) { printf("(TODO regs)\n"); continue; }
        if (strcmp(line, "fontsize +") == 0) { printf("(stub fontsize +)\n"); continue; }
        if (strcmp(line, "fontsize -") == 0) { printf("(stub fontsize -)\n"); continue; }
        printf("Comando no reconocido. Escriba 'help'.\n");
    }
    return 0;
}