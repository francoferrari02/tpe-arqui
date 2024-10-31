#include "sound.h"
#include <stdint.h>
#include "time.h"

void startSpeaker(uint32_t freq){

    uint32_t Div;
    uint8_t tmp;

    if(freq==0){
        stopSpeaker();
        return;
    }

    //Set the PIT to the desired frequency
    Div = 1193180 / freq;
    speakerOut(0x43, 0xb6);
    speakerOut(0x42, (uint8_t) (Div) );
    speakerOut(0x42, (uint8_t) (Div >> 8));

    //And play the sound using the PC speaker
    tmp = speakerIn(0x61);
    if (tmp != (tmp | 3)) {
        speakerOut(0x61, tmp | 3);
    }
}

void stopSpeaker(){
    uint8_t tmp = speakerIn(0x61) & 0xFC;
    speakerOut(0x61, tmp);
}

void noise(uint32_t freq, uint64_t duration)
{
    startSpeaker(freq);
    wait(duration);
    stopSpeaker();
}