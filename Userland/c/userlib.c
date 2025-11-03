#include <stdint.h>
#include <stddef.h>
#include "../c/include/userlib.h"
#include "../c/include/shell.h"

static Command commands[] = {
    {"help", help},
    {"clear", clear},
    {"printTime", printTime},
    {"printDate", printDate},
    {0, 0},
};

void help(){
    shellPrintString("Comandos disponibles: \n");
    shellPrintString("help    ->   muestra la lista de comandos.\n");
    shellPrintString("time    ->   imprime la hora.\n");
    shellPrintString("div0    ->   division por cero.\n");
    shellPrintString("invop   ->   instruccion invalida.\n");
    shellPrintString("regs    ->   vuelca registros.\n");
}

void clear(){
    sys_clear();
}

uint8_t adjustHour(uint8_t hour, int offset){
    int decimalHour = ((hour >> 4) * 10) + (hour & 0x0F);
    decimalHour += offset;

     // Ajustar para que esté en el rango 0-23
    if (decimalHour < 0){
        decimalHour += 24;
    }else{
          if(decimalHour >= 24){
            decimalHour -= 24;
          }
    }

     return ((decimalHour / 10) << 4) | (decimalHour % 10);
}

void printTimeAndDate(uint8_t* buff, char separator){
    char outBuff[10];

    for(int i = 0; i < 3; i++){
        int value = ((buff[i] >> 4) & 0x0F) * 10 + (buff[i] & 0x0F);
        outBuff[3 * i] = (char)('0' + (value / 10));
        outBuff[3 * i + 1] = (char)('0' + (value % 10));

        if(i < 2){
            outBuff[3 * i + 2] = separator;
        }
    }

    outBuff[8] = '\n';
    outBuff[9] = 0;

    shellPrintString(outBuff);
}

void printTime(){
    uint8_t timeBuff[3];
    sys_time(timeBuff);
    timeBuff[0] = adjustHour(timeBuff[0], -3);
    printTimeAndDate(timeBuff, ':');
}

void printDate(){
    uint8_t timeBuff[3];
    uint8_t dateBuff[3];

    sys_time(timeBuff);
    sys_date(dateBuff);

    int hour = ((timeBuff[0] >> 4) * 10) + (timeBuff[0] & 0x0F);

    if(hour < 3){
        int day = ((dateBuff[0] >> 4) * 10) + (dateBuff[0] & 0x0F);
        day--;
        
        if(day <= 0){
            day = 30;
            int month = ((dateBuff[1] >> 4) * 10) + (dateBuff[1] & 0x0F);
            month--;
            
            if(month <= 0){
               month = 12;
               int year = ((dateBuff[2] >> 4) * 10) + (dateBuff[2] & 0x0F);
               year--;
               dateBuff[2] = ((year / 10) << 4) | (year % 10);
            }

            dateBuff[1] = ((month / 10) << 4) | (month % 10);
        }

        dateBuff[0] = ((day / 10) << 4) | (day % 10);
    }

    printTimeAndDate(dateBuff, '/');
}

// Implementaciones mínimas de string para entorno freestanding
size_t strlen(const char *s){
    size_t n = 0;
    if(s == 0) return 0;
    while(s[n] != '\0') n++;
    return n;
}

int strcmp(const char *a, const char *b){
    if(a == 0 && b == 0){
          return 0;
    }

    if(a == 0){
          return -1;
    }

    if(b == 0){
          return 1;
    }

    while(*a && (*a == *b)){
        a++; 
        b++;
    }

    return (unsigned char)*a - (unsigned char)*b;
}

uint64_t putchar(char c){
    char buff[1];
    buff[0] = c;

    return sys_write(STDOUT, buff, 1);
}

char getchar(){
    char c;
    while(sys_read(&c, 1) == 0)
        ;
    return c;
}

void processLine(char * buff, uint32_t * history_len){
    if(strlen(buff) == 0){
        return;
    }

    for(int i = 0; commands[i].name != 0; i++){
        if(strcmp(buff, commands[i].name) == 0){
            commands[i].function();
            return;
        }
    }

     shellPrintString("Comando no reconocido! Escriba 'help' para ver los comandos disponibles.\n");
}