#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

uint64_t sys_write(uint64_t fd, const char * buff, uint64_t count);
uint64_t sys_read(char * buff, uint64_t count);
uint64_t sys_registers(char * buff);
void sys_time(uint8_t * buff);
void sys_date(uint8_t * buff);

#endif
