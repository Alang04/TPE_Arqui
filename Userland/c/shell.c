#include "shell.h"
#include "userlib.h"

#define SYSCALL_READ       0
#define SYSCALL_WRITE      1
#define SYSCALL_GET_TIME   2
#define STDIN  0
#define STDOUT 1
#define WELCOME "Bienvenido al MASS OS!\n"
#define BUFF_LENGTH 100

int main(){
    shellPrintString(WELCOME);
    shellNewline();
    shellPrintString("Escriba 'help' para listar comandos.\n");

    char buff[BUFF_LENGTH];
    while(1){
        shellPrintString("> ");
        shellReadLine(buff, BUFF_LENGTH);
        shellNewline();
        processLine(buff, 0);
    }

    return 0;
}

void shellReadLine(char * buffer, uint64_t max){
    char c;
    uint32_t idx = 0;

    while((c = getchar()) != '\n'){
        if(c == '\b'){
            if(idx > 0){
                idx--;
                shellPutchar('\b', STDOUT);
                //shellPutchar(' ', STDOUT);
                //shellPutchar('\b', STDOUT);
            }
        } else{
            buffer[idx++] = c;
            shellPutchar(c, STDOUT);
        }
    }

    buffer[idx] = 0;
}

void shellPrintString(char *str){
    if(str == 0){
          return;
    }
    for(uint32_t i = 0; str[i] != '\0'; i++){
        shellPutchar(str[i], STDOUT);
    }
}

void shellPutchar(char c, uint64_t fd){
    char backspace = '\b';
    char cursor = CURSOR;
    //sys_write(STDOUT, &backspace, 1); // borro el cursor
    sys_write(fd, &c, 1); // escribo el caracter
    //sys_write(STDOUT, &cursor, 1); // escribo el cursor
}

void shellNewline(){
    shellPutchar('\n', STDOUT);
}