#include <keyboardDriver.h>
#include <lib.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <stdint.h>
#include <defs.h>

char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // backspace
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     //tab y enter
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/', 0, '*',
    0,' ', // espacio
    // ... (resto omitido)
};

static char buff[BUFF_LENGTH];
static char registersBuffer[REGISTERS_BUFFER_SIZE];
int buff_size = 0;
int start_index = 0;
int end_index = 0;

void write(unsigned char c){
    if(buff_size < BUFF_LENGTH){
        buff[end_index] = c;
        end_index = (end_index + 1) % BUFF_LENGTH;
        buff_size++;
    }
}

void erase(){
    if(buff_size > 0){
        start_index = (start_index + 1) % BUFF_LENGTH;
        buff_size--;
    }
}

void clear(){
    buff_size = 0;
    start_index = 0;
    end_index = 0;
}

void read(char * out_buf, unsigned long maxLen){
    unsigned long i = 0;
    while(i < maxLen){
        while(buff_size == 0); // espera activa

        char c = buff[start_index];
        erase();

        if(c == '\n' || c == '\r'){
            out_buf[i] = 0;
            return;
        }
        if(c == '\b'){
            if(i > 0){
                i--;
                // borrar del display
                ncPrintChar('\b');
            }
            continue;
        }
        if((unsigned char)c < 32) continue;

        out_buf[i] = c;
        i++;
        ncPrintChar(c);
    }
    out_buf[i] = 0;
}

void printPressedKey(){
    while(1){
        uint8_t scancode = getPressedKey();
        if(scancode & 0x80){
            continue; // ignora la liberacion de teclas 
        }
        if(scancode < 128){
            //printChar(scancode_to_ascii[scancode]);
        }
    }
}



// Lee una tecla y la dibuja en modo gráfico VBE
char readKeyAsciiBlockingVBE(uint32_t *x, uint32_t y, uint32_t color) {
    while (1){
        uint8_t sc = (uint8_t)getPressedKey();

        if (sc >= 128) continue;
        char ascii = scancode_to_ascii[sc];
        if (ascii != 0) {
            if((unsigned char)ascii >= 32){
                drawFigure(*x, y, ascii, color);
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
                // helperFilledRect(*x, y, 8, 16, 0x000000);
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

void handlePressedKey(){
    uint8_t scancode = getPressedKey();
    write(scancode_to_ascii[scancode]);
    return;
}

uint64_t copyRegistersBuffer(char * copy){
    // IMPLEMENTAR
}