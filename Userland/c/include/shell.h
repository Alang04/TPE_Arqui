#include <stdint.h>

#define CURSOR '_'

typedef void (*Runnable)(void);

typedef struct Command{
     char* name;
     Runnable function;
}Command;

void shellPrintString(char *str);
void shellPutchar(char c, uint64_t fd);
void shellNewline();
void shellReadLine(char * buffer, uint64_t max);
void shellPutchar(char c, uint64_t fd);