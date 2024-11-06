#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "lib.h"

void keyboard_handler(uint8_t keyPressed);


char getCharFromKeyboard();


unsigned char getScanCode();


void clearScanCode();



#endif