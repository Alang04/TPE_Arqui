#include <syscallDispatcher.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <lib.h>
#include <defs.h>
#include <time.h>

uint64_t sys_write(uint64_t fd, const char * buff, uint64_t count){
    uint32_t color = 0xFFFFFF;
    printString("Entre a sys_write", 0, 0, 0xFFFFFF, 1);
   
    for(int i = 0; i < count; i++){
        putChar(buff[i], color);
    }

    return count;
}

uint64_t sys_read(char * buff, uint64_t count){
    printString("Entre a sys_read", 0, 0, 0xFFFFFF, 1);
   return readBuff(buff, count);
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

void * syscalls[CANT_SYS] = {&sys_registers, &sys_time, &sys_date, &sys_read, &sys_write};