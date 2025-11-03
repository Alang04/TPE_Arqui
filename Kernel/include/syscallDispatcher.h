#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>
#include "defs.h"

extern void * syscalls[CANT_SYS];

uint64_t sys_write(uint64_t fd, const char * buff, uint64_t count);
uint64_t sys_read(char * buff, uint64_t count);
uint64_t sys_registers(char * buff);
void sys_time(uint8_t * buff);
void sys_date(uint8_t * buff);
void sys_decrease_fontsize();
void sys_increase_fontsize();
void sys_beep(uint32_t freq, uint64_t time);
uint64_t sys_ticks();

#endif