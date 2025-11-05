// API de solo consumo (tiempo, fecha y ticks del sistema)
// Contrato:
// - sleep(ms) bloquea de forma cooperativa usando ticks (resolución ~10ms).
// - deltaTicks() retorna ticks desde el arranque.
// - date/time escriben en BCD: [DD,MM,AA] y [HH,MM,SS].
#ifndef _TIME_H_
#define _TIME_H_

void sleep(int ms);
unsigned long deltaTicks(void);
void timer_handler(void);
void date(unsigned char *buff);
void time(unsigned char *buff);

#endif
