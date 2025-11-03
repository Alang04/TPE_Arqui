#include "syscallDispatcher.h"
#include "videoDriver.h"
#include "keyboardDriver.h"
#include "lib.h"
#include "defs.h"
#include "time.h"
#include "sound.h"

void sys_increase_fontsize(){
    increaseFontSize();
}
void sys_decrease_fontsize(){
    decreaseFontSize();
}

uint64_t sys_write(uint64_t fd, const char * buff, uint64_t count){
    uint32_t color = 0xFFFFFF;

    for(int i = 0; i < count; i++){
        videoPutChar(buff[i], color);
    }

    return count;
}

uint64_t sys_read(char * buff, uint64_t count){
   return readKeyBuff(buff, count);
}

void sys_date(uint8_t * buff){
    date(buff);
}

void sys_time(uint8_t * buff){
    time(buff);
}

uint64_t sys_registers(char * buff){
    return copyRegistersBuffer(buff);
}

void sys_beep(uint32_t freq, uint64_t time){
    beep(freq, time);
}

uint64_t sys_ticks(){
    return deltaTicks();
}

void * syscalls[CANT_SYS] = {&sys_registers, &sys_time, &sys_date, &sys_read, &sys_write, &sys_increase_fontsize, &sys_decrease_fontsize, &sys_beep, &sys_ticks};