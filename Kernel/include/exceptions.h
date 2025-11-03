#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

void exceptionDispatcher(int exception);
void excepHandler(char * msg);
void zeroDivision();
void invalidOpcode();
typedef void (*Exception)();

#endif