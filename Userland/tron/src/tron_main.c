#include <stdint.h>
#include "../include/tron_main.h"
#include "../include/game.h"
#include "../include/input.h"
#include "../include/render_text.h"
#include "../include/ai.h"
#include "../../c/include/userlib.h" // sys_* wrappers

int tron_main(void){
    // TODO: menú mínimo (1P vs IA / 2P) y configuración básica
    // TODO: crear instancia de juego y configurar velocidad
    // TODO: loop principal: input → (IA si corresponde) → update → render
    // Por ahora devolvemos al shell sin ejecutar nada.
    sys_write(STDOUT, "Tron: placeholder. En construcción.\n", 34);
    return 0;
}
