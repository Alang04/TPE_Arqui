#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>

void exceptionDispatcher(uint64_t exception, uint64_t error_code);

#endif