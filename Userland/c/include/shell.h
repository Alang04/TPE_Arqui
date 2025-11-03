#include <stdint.h>

#define CURSOR '_'
#define SYSCALL_READ       0
#define SYSCALL_WRITE      1
#define SYSCALL_GET_TIME   2
#define STDIN  0
#define STDOUT 1
#define WELCOME "Bienvenido al MASS OS!\n"
#define BUFF_LENGTH 100

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