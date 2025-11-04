#include <stdint.h>
#include "../include/render_text.h"

void render_text_init(int grid_w, int grid_h){
    (void)grid_w; (void)grid_h;
    // TODO: preparar buffers de texto si fuera necesario
}

void render_text_begin(void){
    // TODO: preparar frame (opcional)
}

void render_text_draw(const GameSnapshot *snap){
    (void)snap;
    // TODO: dibujar HUD y una representación ASCII de la grilla
}

void render_text_end(void){
    // TODO: flush si fuese necesario
}

void render_text_clear(void){
    // TODO: limpiar pantalla con sys_clear
}
