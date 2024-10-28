#include "sound.h"

void startSound(uint32_t nFrequence){
    uint32_t Div;
    uint8_t tmp;

    //Set the PIT to the desired frequency
    Div = 1193180 / nFrequence;
    outSpeaker(0x43, 0xb6);
    outSpeaker(0x42, (uint8_t) (Div) );
    outSpeaker(0x42, (uint8_t) (Div >> 8));

    //And play the sound using the PC speaker
    tmp = inSpeaker(0x61);
    if (tmp != (tmp | 3)) {
        outSpeaker(0x61, tmp | 3);
    }
}

void stopSound(){
    uint8_t tmp = inSpeaker(0x61) & 0xFC;
    outSpeaker(0x61, tmp);
}