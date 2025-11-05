// API de solo consumo (render de texto/gráfico)
// Este header expone las primitivas que otros módulos del kernel pueden usar.
// Contrato general:
// - Coordenadas y dimensiones están en píxeles del modo actual.
// - Las funciones validan límites cuando aplica y solo alteran framebuffer/estado de cursor.
#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>

/* Screen / mode information */
// Retorna el ancho/alto de pantalla en píxeles.
uint16_t getScreenWidth(void);
uint16_t getScreenHeight(void);
// True si (x,y) cae dentro de la pantalla.
int validPosition(uint64_t x, uint64_t y);

/* Text size helpers */
// Ajusta/consulta el tamaño por defecto del texto (múltiplo entero del bitmap base).
void setDefaultTextSize(uint64_t size);
uint64_t getDefaultTextSize(void);
// Ajusta el tamaño de texto actual, con mínimo de 1.
void setTextSize(uint8_t size);
void increaseFontSize(void);
void decreaseFontSize(void);

/* Pixel and drawing primitives */
// Dibuja un pixel en (x,y) con color 0xRRGGBB.
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
// Rellena un rectángulo en (x,y) de tamaño (w,h) con color.
void fillRect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint32_t color);

/* Character / text drawing */
// Escribe un caracter con estilo actual y avanza el cursor. Soporta '\n' y '\b'.
void videoPutChar(uint8_t c, uint32_t color);
// Imprime una cadena completa en el color indicado (usa videoPutChar).
void videoPrint(const char *str, uint32_t color);
// Control del flujo de texto y pantalla.
void newLine(void);
void scroll(void);
void clearScreen(uint32_t color);

#endif