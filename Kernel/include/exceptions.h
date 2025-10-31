#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>

void exceptionDispatcher(int exception, uint64_t *stackPtr);

#endif