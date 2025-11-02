#include <videoDriver.h>
#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <defs.h>

extern uint8_t getPressedKey();

char kbd_min[KBD_LENGTH] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // backspace
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     //tab y enter
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/', 0, '*',
    0,' ',
};

char kbd_mayus[KBD_LENGTH] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', // backspace
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',     // tab y enter
    0,'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?', 0, '*',
    0,' ',
};

char * kbd_manager[] = {kbd_min, kbd_mayus};

static char buff[BUFF_LENGTH];
//static char registersBuffer[REGISTERS_BUFFER_SIZE];
static int shift = 0;
static int caps = 0;
int buff_size = 0;
int start_index = 0;
int end_index = 0;

void writeBuff(unsigned char c){
    buff[end_index] = c;
    end_index = (end_index + 1) % BUFF_LENGTH;
    buff_size = (buff_size + 1) % BUFF_LENGTH;
    printString("Entre a writeBuff", 0, 0, 0xFFFFFF, 1);      
    videoPutChar(c, 0xFFFFFF);
}

void clearBuff(){
    buff_size = 0;
    start_index = 0;
    end_index = 0;
}

uint64_t readBuff(char * out_buf, unsigned long maxLen){
    unsigned long i = 0;
    while(i < maxLen && buff_size > 0){
        char c = buff[start_index];

        buff_size--;
        start_index = (start_index + 1) % BUFF_LENGTH;

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
        } else{
            //if((unsigned char)c < 32) continue;
            out_buf[i] = c;
            i++;
            ncPrintChar(c);
        }
        printString("Entre a readBuff", 0, 0, 0xFFFFFF, 1);      
        videoPutChar(c, 0xFFFFFF);
    }
    out_buf[i] = 0;
    return i;
}


void handlePressedKey(){
    uint8_t scancode = getPressedKey();

    if(scancode == L_ARROW || scancode == R_ARROW || scancode == UP_ARROW || scancode == DOWN_ARROW || scancode == 0 || scancode > BREAK_CODE){
        return;
    } else if(scancode == L_SHIFT || scancode == R_SHIFT){
        shift = 1;
    } else if(scancode == (L_SHIFT | BREAK_CODE) || scancode == (R_SHIFT | BREAK_CODE)){
        shift = 0;
    } else if(scancode == CAPS_LOCK){
        caps = !caps;
    } else if(!(scancode & BREAK_CODE)){
        writeBuff(kbd_manager[(shift + caps) % 2][scancode]);
    }

    printString("Entre a handlePressedKey", 0, 0, 0xFFFFFF, 1);      
    videoPutChar(scancode, 0xFFFFFF);
}

uint64_t copyRegistersBuffer(char * copy){
    return 0;
}