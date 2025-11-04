#include "../include/ai.h"

int ai_decide_next_dir(const GameSnapshot *snap, int player_id, int difficulty, Direction *out_dir){
    (void)snap; (void)player_id; (void)difficulty; (void)out_dir;
    // TODO: IA básica "safe-first": evita chocar, elige izquierda/derecha cuando el frente no es seguro
    return 0;
}
