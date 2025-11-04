#include "tron.h"

typedef struct position {
    int x;
    int y;
} position_t;

static char tronTable[TABLE_LENGHT][TABLE_HEIGHT] = {0};

static char directions[][] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}}; 