#include <stdint.h>
#include "../include/game.h"
#include "../include/config.h"
#include "../include/types.h"

struct TronGame {
    // Dimensiones y grilla (buffer estático 2D: [y][x])
    uint8_t cells[TRON_GRID_H][TRON_GRID_W]; // valores de Cell

    // Ritmo/tiempo
    uint64_t last_step_ticks;  // referencia del último step (0 si sin iniciar)
    int step_count;            // steps en la ronda actual

    // Modo/estado
    GameMode mode;             // MODE_ONE_PLAYER o MODE_TWO_PLAYERS
    GameState state;           // PLAYING/PAUSED/GAME_OVER
    Winner winner;             // resultado si terminó la ronda

    // Puntajes acumulados
    int score_p1;
    int score_p2;

    // Jugadores
    PlayerState p1;
    PlayerState p2;
};

// Helpers internos (no exportados)
static inline int in_bounds(const struct TronGame *g, int x, int y){
    return (x >= 0 && x < TRON_GRID_W && y >= 0 && y < TRON_GRID_H);
}

static void clear_grid(struct TronGame *g){
    for(int y = 0; y < TRON_GRID_H; y++){
        for(int x = 0; x < TRON_GRID_W; x++){
            g->cells[y][x] = CELL_EMPTY;
        }
    }
}

static void place_initial_positions(struct TronGame *g){
    // P1 a la izquierda, P2 a la derecha, ambos en mitad de altura
    g->p1.x = TRON_GRID_W / 4;
    g->p1.y = TRON_GRID_H / 2;
    g->p1.dir = DIR_RIGHT;
    g->p1.pending_dir = g->p1.dir;
    g->p1.alive = 1;

    g->p2.x = (3 * TRON_GRID_W) / 4;
    g->p2.y = (TRON_GRID_H) / 2;
    g->p2.dir = DIR_LEFT;
    g->p2.pending_dir = g->p2.dir;
    g->p2.alive = 1;
}

TronGame * game_create(GameMode mode){
    // TODO: reservar y configurar estado inicial
    TronGame *g = malloc(sizeof(TronGame));
    if (!g) {
        return 0; // fallo de asignación
    }

    //Inicializo los valores por defecto del juego
    g->mode = mode;
    g->last_step_ticks = 0;
    g->step_count = 0;
    g->winner = WIN_NONE;
    g-> state = STATE_PLAYING;

    g->score_p1 = 0;
    g->score_p2 = 0;
    return g;
}

void game_destroy(TronGame *g){
    (void)g;
    // TODO: liberar memoria si se alocó
}

void game_reset_round(TronGame *g){
    (void)g;
    // TODO: resetear posiciones, direcciones y trazas
}

void game_enable_ai(TronGame *g, int enabled, int difficulty){
    (void)g; (void)enabled; (void)difficulty;
    // TODO: habilitar IA y almacenar dificultad
}

void game_queue_turn(TronGame *g, int player_id, Direction dir){
    (void)g; (void)player_id; (void)dir;
    // TODO: encolar giro válido para próximo step
}

int game_update(TronGame *g, uint64_t now_ticks){
    (void)g; (void)now_ticks;
    // TODO: avanzar step si corresponde según ticks
    return 0;
}

int game_is_over(const TronGame *g){
    (void)g;
    // TODO: indicar si terminó la ronda
    return 0;
}

Winner game_winner(const TronGame *g){
    (void)g;
    // TODO: retornar ganador si hubo
    return WIN_NONE;
}

void game_get_snapshot(const TronGame *g, GameSnapshot *out){
    (void)g; (void)out;
    // TODO: volcar snapshot de lectura para render/IA
}
