#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

#define STDOUT 1
#define STDERR 2
#define REGSBUFF 500
#define REDRAW_BUFF 4096
#define KB 1024
#define BM_BUFF 20
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define EIGHTH 50
#define QUARTER 100
#define HALF 200

typedef struct{
    char character;
    uint64_t fd;
}RedrawStruct;

// Redraw buffer API
void redraw_reset(void);
void redraw_append_char(char c, uint64_t fd);

uint64_t sys_write(uint64_t fd, const char * buff, uint64_t count);
uint64_t sys_read(char * buff, uint64_t count);
uint64_t sys_registers(char * buff);
void sys_time(uint8_t * buff);
void sys_date(uint8_t * buff);
void sys_increase_fontsize(void);
void sys_decrease_fontsize(void);
void sys_beep(uint32_t freq, uint64_t time);
uint64_t sys_ticks(void);
void sys_clear(void);
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
void invOp();
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
