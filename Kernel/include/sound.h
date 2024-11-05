#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

void speakerOut(int port, int value);

void wait(int duration);

int speakerIn(int port);

void startSpeaker(uint32_t freq);

void stopSpeaker();

void noise(uint32_t freq, uint64_t duration);

#endif /* SOUND_H */