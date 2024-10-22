
#include "../include/keyboardDriver.h"
#include "../include/my_time.h"
#include <stdint.h>

static int shift = 0 ;
static int capsLock = 0;
unsigned char pressedChar = 0;
static char retChar = 0;


static const char lowKeyMap[] = {

      0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9',  '0', '-', '=',
    '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',  '[', ']',
    '\n',    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
      0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0, '*',
      0,  ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,
      0,    0,   38,   0, '-',   37,   0,   39, '+',   0,   40,   0,    0,   0,
      0,    0,   0,   0,   0,   0,   0,   0,  0,    0,   0,   0,    0,   0,

};

static const char upKeyMap[] = {

      0,   27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
   '\n',    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
      0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0, '*',
      0, ' ',    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,    0,   0, '-',   0,   0,   0, '+',   0,   0,   0,   0,   0,
      0,   0,    0,   0,   0,   0

};

static char * keyMap[] = {lowKeyMap, upKeyMap};


void keyboard_handler(uint8_t keyPressed) {
    pressedChar = keyPressed;

    //shift pressed
    if (pressedChar == 0x2A || pressedChar == 0x36){
        shift = 1;
    }
    //shift not pressed
    if (pressedChar == 0xAA || pressedChar == 0xB6) {
        shift = 0;
    }
    //capsLock
    if (pressedChar == 0x3A) {
        capsLock = (capsLock+1)%2;
    }
}


char getCharFromKeyboard() {
    //soltar tecla
    if (pressedChar > 0x80 || pressedChar == 0x0F){
        retChar = 0;
    }else if (pressedChar == 0x48 || pressedChar == 0x50){
        retChar = pressedChar;
    } 
    else {
        retChar = keyMap[shift][pressedChar];
    }
    return retChar;
}

void setCeroChar(){
    pressedChar = 0;
}

unsigned char getPressedChar(){
    return pressedChar;
}