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


//funcion ej3
char * getTime();
void intToStr(int value, char *buffer);


#endif