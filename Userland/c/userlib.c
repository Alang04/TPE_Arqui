#include <stdint.h>
#include <stddef.h>
#include "../c/include/userlib.h"
#include "../c/include/shell.h"

static Command commands[] = {
    {"help", help},
    {"clear", clear},
    {"printTime", printTime},
    {"printDate", printDate},
    {"registers", registers},
    {"testDiv0", divideByZero},
    {"playBeep", playBeep},
    {"bmFPS", bmFPS},
    {"bmCPU", bmCPU},
    {"bmMEM", bmMEM},
    {"bmKEY", bmKEY},
    {0, 0},
};

RedrawStruct redrawBuffer[REDRAW_BUFF];
uint32_t redrawLength = 0;

/* Convierte un entero sin signo a cadena en la base indicada.
   - value: número a convertir.
   - dest: buffer destino (debe tener espacio suficiente), se escribe NUL-terminated.
   - base: base entre 2 y 16 (por ejemplo 10 para decimal, 16 para hex).
   Retorna la cantidad de caracteres escritos (sin incluir el '\0').
*/
uint64_t num_to_str(uint64_t value, char * dest, int base){
    if(!dest) return 0;
    if(base < 2 || base > 16) base = 10;

    char tmp[65];
    int pos = 0;

    if(value == 0){
        tmp[pos++] = '0';
    } else {
        while(value){
            int d = value % base;
            tmp[pos++] = (d < 10) ? ('0' + d) : ('A' + (d - 10));
            value /= base;
        }
    }

    /* volcar en orden correcto */
    for(int i = 0; i < pos; i++){
        dest[i] = tmp[pos - 1 - i];
    }
    dest[pos] = '\0';
    return (uint64_t)pos;
}

void bmCPU(){
    /*
     * bmCPU - simple CPU benchmark
     * - Runs a mix of integer and floating-point operations N times
     * - Measures elapsed ticks using sys_ticks() and prints total time
     *   and a rough operations-per-tick metric.
     *
     * Notes:
     * - N is kept reasonably large to get measurable tick counts.
     * - This is a coarse benchmark (no warming, no cycle-accurate timing).
     */
    const uint32_t N = 1000000u;
    uint64_t ticks = sys_ticks();

    uint64_t result = 0;
    double float_result = 0.0;

    for(uint32_t i = 0; i < N; ++i){
        result += (uint64_t)i * 7ull;
        result = result % 2147483647ull;

        float_result += (double)i * 3.14159;
        if ((i % 100000u) == 0 && i != 0) {
            float_result *= 0.5;
        }
    }

    uint64_t end_ticks = sys_ticks();
    uint64_t delta = end_ticks - ticks;

    shellPrintString("Tiempo: ");

    char timeBuff[32];
    num_to_str(delta, timeBuff, 10);
    shellPrintString(timeBuff);
    shellPrintString(" ticks\n");

    if(delta > 0){
        /* promote N to 64-bit before division to avoid surprises */
        uint64_t ops_per_tick = ((uint64_t)N) / delta;
        shellPrintString("Operaciones por tick: ");
        num_to_str(ops_per_tick, timeBuff, 10);
        shellPrintString(timeBuff);
        shellPrintString("\n");
    } else {
        shellPrintString("Elapsed ticks = 0, no se puede calcular ops/tick.\n");
    }

    return;
}

void bmFPS(){    
    /*
     * bmFPS - crude frame-rate benchmark
     * - Repeatedly clears the screen for ~3 seconds and counts iterations.
     * - Assumes sys_ticks() increments roughly 18 times per second (BIOS-like).
     * - Avoid printing inside the loop to not skew the results.
     */
    uint64_t ticks = sys_ticks();
    uint64_t count = 0;
    /* 18 ticks ~= 1 second on many systems that emulate BIOS ticks */
    uint64_t duration = 18 * 7; /* ~3 seconds */

    shellPrintString("Inicio de test.\n");
    /* busy loop that only clears the screen and increments counter */
    while((sys_ticks() - ticks) < duration){
        sys_clear();
        count++;
    }

    /* count iterations over ~7 seconds -> approximate frames per second */
    uint64_t fps = count / 7;
    shellPrintString("FPS: ");

    char fpsBuff[BM_BUFF];
    num_to_str(fps, fpsBuff, 10);
    shellPrintString(fpsBuff);
    shellPrintString("\n");
}

void bmMEM(){
    /*
     * bmMEM - simple memory benchmark
     * - Fills a 4KB buffer many times, computes a checksum and does copies.
     * - Measures elapsed ticks and reports operations per tick.
     *
     * Notes:
     * - Make sure the operations count is calculated using 64-bit to avoid
     *   intermediate overflow on 32-bit platforms.
     */
    char buffer[4 * KB];
    uint64_t ticks = sys_ticks();

    for(int iteration = 0; iteration < 10000; iteration++){
        for(int i = 0; i < 4 * KB; i++){
            buffer[i] = (i + iteration) % 256;
        }

        /* use 64-bit checksum to avoid truncation issues */
        volatile uint64_t checksum = 0;
        for(int i = 0; i < 4 * KB; i++){
            checksum += (unsigned char)buffer[i];
            checksum = checksum % 1000000ULL;
        }

        for(int i = 0; i < 2 * KB; i++){
            buffer[i + 2 * KB] = buffer[i];
        }
    }

    uint64_t finalTicks = sys_ticks();
    uint64_t delta = finalTicks - ticks;

    shellPrintString("Tiempo: ");

    char buff[BM_BUFF];
    num_to_str(delta, buff, 10);
    shellPrintString(buff);
    shellPrintString(" ticks\n");

    if(delta > 0){
        /* compute operations using 64-bit arithmetic to be safe */
        uint64_t operations = (uint64_t)10000 * (uint64_t)(4 * KB) * 3ULL;
        uint64_t operationsPerCycle = operations / delta;
        shellPrintString("Operaciones por tick: ");
        num_to_str(operationsPerCycle, buff, 10);
        shellPrintString(buff);
        shellPrintString("\n");
    }

    /* flush redraw buffer to their file descriptors (same behavior as before) */
    uint64_t current = redrawBuffer[0].fd;
    uint32_t idx = 0;

    for(uint32_t i = 0; i < redrawLength; i++){
        if(redrawBuffer[i].fd != current || idx >= sizeof(buffer) - 1){

            if(idx > 0){
                sys_write(current, buffer, idx);
                idx = 0;
            }
            current = redrawBuffer[i].fd;
        }
        buffer[idx++] = redrawBuffer[i].character;
    }

    if(idx > 0){
        sys_write(current, buffer, idx);
    }
}

void bmKEY(){
    shellPrintString("Presione cualquier tecla: \n");
    uint64_t ticks = sys_ticks();
    getchar();

    uint64_t finalTicks = sys_ticks();
    uint64_t delta = finalTicks - ticks;
    shellPrintString("Tiempo: ");
    char buff[BM_BUFF];

    num_to_str(delta, buff, 10);
    shellPrintString(buff);
    shellPrintString(" ticks\n");
}

void playBeep(){
    sys_beep(440, 200);
    sys_beep(0,50);
    sys_beep(300, 100);
    sys_beep(0,50);
    sys_beep(400, 100);
    sys_beep(0,50);
}

void redrawFont(){
    sys_clear(); 

    if(redrawLength == 0){
        return;
    } 

    char buffer[REDRAW_BUFF]; 

    uint64_t current = redrawBuffer[0].fd;
    uint32_t idx = 0;

    for(uint32_t i = 0; i < redrawLength; i++){
        if(redrawBuffer[i].fd != current || idx >= sizeof(buffer) - 1){
            if(idx > 0){
                sys_write(current, buffer, idx);
                idx = 0;
            }
            current = redrawBuffer[i].fd;
        }
        buffer[idx++] = redrawBuffer[i].character;
    }
    
    if(idx > 0){
        sys_write(current, buffer, idx);
    }
}

void shellIncreaseFontSize(){
    sys_increase_fontsize(); 
    redrawFont();
}

void shellDecreaseFontSize(){ 
    sys_decrease_fontsize(); 
    redrawFont();
}

void help(){
    shellPrintString("Comandos disponibles: \n");
    shellPrintString("help      ->   muestra la lista de comandos.\n");
    shellPrintString("clear     ->   limpia la pantalla.\n");
    shellPrintString("printTime ->   imprime la hora actual.\n");
    shellPrintString("printDate ->   imprime la fecha actual.\n");
    shellPrintString("registers ->   imprime registros.\n");
    shellPrintString("testDiv0  ->   division por cero.\n");
    shellPrintString("invOp     ->   instruccion invalida.\n");  /* falta hacer */
    shellPrintString("playBeep  ->   reproduce un beep.\n");
    shellPrintString("bmFPS     ->   benchmark de FPS.\n");
    shellPrintString("bmCPU     ->   benchmark de CPU.\n");
    shellPrintString("bmMEM     ->   benchmark de MEM.\n");
    shellPrintString("bmKEY     ->   benchmark de teclado.\n");
}
void clear(){
    sys_clear();
}

void divideByZero(){
    clear();
    
    int x = 1;
    int y = 0;
    int z;
    z = x / y; // dispara #DE
    (void)z;   // evitar warning de variable no usada (si no se dispara la excepción)
}

void registers(){
    char buffer[REGSBUFF];

    if(sys_registers(buffer)){
        shellPrintString(buffer);
    } else{
        shellPrintString("Presione CTRL para guardar los registros.\n");
    }
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