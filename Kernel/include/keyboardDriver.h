#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

void keyboard_handler(uint8_t keyPressed);
char getCharFromKeyboard();
void setCeroChar();
unsigned char getNotChar();

#endif