#include <shell.h>
#include <userlib.h>

#define SYSCALL_READ       0
#define SYSCALL_WRITE      1
#define SYSCALL_GET_TIME   2
#define STDIN  0
#define STDOUT 1

static void cmd_help(){
    shellPrintString("Comandos disponibles:\n");
    shellPrintString("help       - lista comandos\n");
    shellPrintString("time       - muestra hora\n");
    shellPrintString("div0       - provoca division por cero\n");
    shellPrintString("invop      - ejecuta instruccion invalida\n");
    shellPrintString("regs       - muestra registros (TODO)\n");
}

int main(){
    shellPrintString("Bienvenido! Escriba 'help' para listar comandos.\n");
    //char line[128];
    //while(1){
    //    print_prompt();
    //    read_line(line, sizeof(line));
    //    if (line[0] == 0) continue;
    //    if (strcmp(line, "help") == 0) { cmd_help(); continue; }
    //    if (strcmp(line, "time") == 0) { cmd_time(); continue; }
    //    if (strcmp(line, "div0") == 0) { cmd_div0(); continue; }
    //    if (strcmp(line, "invop") == 0) { cmd_invop(); continue; }
    //    if (strcmp(line, "regs") == 0) { printf("(TODO regs)\n"); continue; }
    //    if (strcmp(line, "fontsize +") == 0) { printf("(stub fontsize +)\n"); continue; }
    //    if (strcmp(line, "fontsize -") == 0) { printf("(stub fontsize -)\n"); continue; }
    //    printf("Comando no reconocido. Escriba 'help'.\n");
    //}
    return 0;
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