#ifndef _VIDEO_DRIVER_H_
#define _VIDEO_DRIVER_H_


#include <stdint.h>
#include "../../Userland/SampleCodeModule/include/colores.h"




//////////////
void increasePixelScale();

// Disminuir el factor de escala para reducir el tamaño de un carácter
void decreasePixelScale();
// Obtener el ancho real de un carácter según el factor de escala actual
uint16_t getRealCharWidth();

// Obtener el alto real de un carácter según el factor de escala actual
uint16_t getRealCharHeight();
//////////////




/* Writes a string in screen*/
void dv_prints(const char *str, Color fnt);

/* Writes a char in screen, given a font Color and Background Color*/
void dv_print(char c, Color fnt);

/* Jumps to the next line */
void dv_newline();

/* Erases the last char from the line, can't go up a line */
void dv_backspace(Color fnt, Color bgd);

/* Clears screen with a given color */
void dv_clear (Color color);

/* prints in different bases */
void dv_printDec(uint64_t value, Color fnt, Color bgd);
void dv_printHex(uint64_t value, Color fnt, Color bgd);
void dv_printBin(uint64_t value, Color fnt, Color bgd);
void dv_printBase(uint64_t value, uint32_t base, Color fnt, Color bgd);

/* Given topLeft point draws a (color) rectangle */
void dv_fillRect (int x, int y, int x2, int y2, Color color);

/* Plots a single pixel of a given color, if valid */
void dv_setPixel(uint16_t x, uint16_t y, Color color);

/* Getters for screen */
uint16_t dv_getWidth(void);
uint16_t dv_getHeight(void);
uint32_t dv_getFrameBuffer(void);
uint8_t dv_getPixelWidth(void);


#endif