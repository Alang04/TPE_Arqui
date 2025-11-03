#include "../include/videoDriver.h"
#include "../include/keyboardDriver.h"
#include "../include/naiveConsole.h"
#include "../include/defs.h"
#include <stdint.h>

//extern uint8_t getPressedKey();
extern volatile uint8_t pressed_key;

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
static uint8_t pressedKeys[LETTERS] = {0};

static char buff[BUFF_LENGTH];
static char registersBuff[REGISTERS_BUFFER_SIZE];
static int shift = 0;
static int caps = 0;
int buff_size = 0;
int start_index = 0;
int end_index = 0;

void writeBuff(unsigned char c){


    buff[end_index] = c;
    end_index = (end_index + 1) % BUFF_LENGTH;
    if(buff_size < BUFF_LENGTH){
        buff_size++;
    } else{
        start_index = (start_index + 1) % BUFF_LENGTH; 
    }
}

void clearBuff(){
    buff_size = 0;
    start_index = 0;
    end_index = 0;
}

uint8_t getFromBuffer(){
    if(buff_size == 0){
        return (uint8_t)-1;
    }

    buff_size--;
    uint8_t result = buff[start_index];
    start_index = (start_index + 1) % BUFF_LENGTH;

    return result;
}

uint64_t readKeyBuff(char * buff, uint64_t count){
    int i;
    for(i = 0; i < count && i < buff_size; i++){
        buff[i] = getFromBuffer();
    }

    return i;
}

void handlePressedKey(){
    uint8_t scancode = pressed_key;
    pressed_key = 0;

    if(scancode == BACKSPACE){
        if(buff_size == 0){
            return;
        }
        end_index = (end_index - 1 + BUFF_LENGTH) % BUFF_LENGTH;
        buff_size--;
        return;
    }
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
}

uint64_t copyRegistersBuffer(char * copy){
    return 0;
}