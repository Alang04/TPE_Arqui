#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

#define STDOUT 1
#define STDERR 2
#define REGSBUFF 500
#define REDRAW_BUFF 4096
#define KB 1024
#define BM_BUFF 20

typedef struct{
    char character;
    uint64_t fd;
}RedrawStruct;

uint64_t sys_write(uint64_t fd, const char * buff, uint64_t count);
uint64_t sys_read(char * buff, uint64_t count);
uint64_t sys_registers(char * buff);
void sys_time(uint8_t * buff);
void sys_date(uint8_t * buff);
uint64_t print_string(char *str);
uint64_t putchar(char c);
uint64_t print_string(char *str);
char getchar();
void processLine(char * buff, uint32_t * history_len);

void help();
void clear();
void registers();
void divideByZero();
void printTime();
void printDate();
void playBeep();
uint8_t adjustHour(uint8_t hour, int offset);
void printTimeAndDate(uint8_t* buff, char separator);
void shellIncreaseFontSize();
void shellDecreaseFontSize();
void redrawFont();
void bmMEM();
void bmCPU();
void bmFPS();
void bmKEY();

#endif
