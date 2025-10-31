#include <keyboardDriver.h>
#include <lib.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <stdint.h>

char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // backspace
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     // enter
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/', 0, '*',
    0,' ', // space bar
    // ... (resto omitido)
};

static volatile char kbuf[256];
static volatile unsigned khead = 0;
static volatile unsigned ktail = 0;

static inline void kb_enqueue(char c) {
    unsigned next = (khead + 1) & 0xFF;
    if (next != ktail) { // only enqueue if buffer not full
        kbuf[khead] = c;
        khead = next;
    }
}

static inline int kb_dequeue(char *out) {
    if (khead == ktail) return 0;
    *out = kbuf[ktail];
    ktail = (ktail + 1) & 0xFF;
    return 1;
}

void printPressedKey(){
    while(1){
        uint8_t scancode = getPressedKey();
        if (scancode & 0x80){
            continue; // ignore releases
        }
        if(scancode < 128){
            printChar(scancode_to_ascii[scancode]);
        }
    }
}

// Non-blocking: devuelve 0 si no hay tecla disponible
uint64_t sys_read_keyboard(){
    char c;
    if (kb_dequeue(&c))
        return (uint64_t)c;
    return 0;
}

// Lee una tecla y la dibuja en modo gráfico VBE
char readKeyAsciiBlockingVBE(uint32_t *x, uint32_t y, uint32_t color) {
    while (1){
        uint8_t sc = (uint8_t)getPressedKey();

        if (sc >= 128) continue;
        char ascii = scancode_to_ascii[sc];
        if (ascii != 0) {
            if((unsigned char)ascii >= 32){
                drawGlyph8x16(*x, y, ascii, color);
                *x += 8;
            }
            return ascii;
        }
    }
}

// Lee una línea de texto en modo gráfico VBE y la deja en buffer
void readLineVBE(char *buffer, unsigned long maxLen, uint32_t *x, uint32_t y, uint32_t color) {
    if (maxLen == 0) return;
    unsigned long pos = 0;
    buffer[0] = 0;
    for (;;) {
        char c = readKeyAsciiBlockingVBE(x, y, color);
        if (c == 0) continue;
        if (c == '\n' || c == '\r') {
            buffer[pos] = 0;
            return;
        }
        if (c == '\b') {
            if (pos > 0) {
                pos--;
                buffer[pos] = 0;
                *x -= 8;
                // Borra el último carácter dibujando un rectángulo negro
                drawFilledRect(*x, y, 8, 16, 0x000000);
            }
            continue;
        }
        if ((unsigned char)c < 32) continue;
        if (pos < maxLen - 1) {
            buffer[pos++] = c;
            buffer[pos] = 0;
        }
    }
}

// Productor: llamado desde IRQ1 (0x21)
void keyboard_irq_producer(void) {
    // Leer un scancode del puerto 0x60
    uint8_t sc;
    __asm__ __volatile__("inb $0x60, %0" : "=a"(sc));
    if (sc & 0x80) return; // release, ignorar
    if (sc < 128) {
        char c = scancode_to_ascii[sc];
        if (c != 0) {
            kb_enqueue(c);
        }
    }
}