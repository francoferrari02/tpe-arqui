#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>



void ncPrint(const char * string);
void ncPrintChar(char character );
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
//funciones hechas para ej 1 (imprimir string con colores definidos)
void ncPrintCharColor(char character, uint8_t color);
void ncPrintColor(const char * string, uint8_t color);

//funciones hechas para ej 2 (implementacion de driver de video para imprimir en pantalla)
typedef enum{
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHT_GREY, DARK_GREY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, 
    LIGHT_MAGENTA, LIGHT_BROWN, WHITE,
}Color;

//ej2
#define VIDEO_MEMORY ((uint8_t*)0xB8000)
#define WIDTH 80
#define HEIGHT 25

typedef struct{
    uint8_t *video; //puntero a la memoria de video
    uint32_t width; //ancho de pantalla
    uint32_t height;//alto de pantalla
    uint32_t cursorX; // Posición en X
    uint32_t cursorY; // Posición en Y
}VideoDriver;

void writeChar(VideoDriver *driver, char c, Color textColor, Color bgColor, uint32_t x, uint32_t y);
void writeString(VideoDriver *driver, const char *str, Color textColor, Color bgColor);
uint8_t combineColors(Color textColor, Color bgColor);

//funcion ej3
char * getTime();
void intToStr(int value, char *buffer);


#endif