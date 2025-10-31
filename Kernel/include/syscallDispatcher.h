#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>

// Función que llamará el wrapper de ensamblador de int 80h
uint64_t int80Dispatcher(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx);

#endif