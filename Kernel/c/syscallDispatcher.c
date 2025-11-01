#include "../include/syscallDispatcher.h"
#include "../include/videoDriver.h"
#include "../include/keyboardDriver.h"
#include "../include/lib.h"
#include "../include/defs.h"

uint64_t sys_write(uint64_t fd, const char * buff, uint64_t count){
    uint32_t color = 0xFFFFFF;
   
    for(int i = 0; i < count; i++){
        putChar(buff[i], color);
    }

    return count;
}

uint64_t sys_read(char * buff, uint64_t count){
   return read(buff, count);
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