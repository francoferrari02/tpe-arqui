#include "keyboard.h"
#include "time.h"
#include <stdint.h>

unsigned char scanCode = 0;
static char retChar = 0;
static int shift = 0 ;
static int capsLock = 0;

static const char keyboardMap[256] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',//backspace,
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', //enter
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
        0, 0, 0, ' ', 0, 0 ,0,0,0,0,0,0,0, 0, 0, 0,
        0, 0 ,0,0,0,0,0,0,0,0,0,0,0
};


void keyboard_handler(uint8_t keyPressed) {
    scanCode = keyPressed;
    //shift presionado
    if (scanCode == 0x2A || scanCode == 0x36){
        shift = 1;
    }
    //shift liberado
    if (scanCode == 0xAA || scanCode == 0xB6) {
        shift = 0;
    }
    //Mayus
    if (scanCode == 0x3A) {
        capsLock = (capsLock+1)%2 ;
    }
}


char getCharFromKeyboard() {
    if (scanCode > 0x80 || scanCode == 0x0F){
        retChar = 0;
    } else {
        retChar = keyboardMap[scanCode];
    }
    if ( (retChar >= 'a' && retChar <= 'z') && (shift == 1 || capsLock == 1) ){
        return retChar - ('a'-'A');
    }
    return retChar;
}

void clearScanCode(){
    scanCode = 0;
}

unsigned char getScanCode(){
    return scanCode;
}