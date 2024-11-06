#ifndef _VIDEO_DRIVER_H_
#define _VIDEO_DRIVER_H_


#include <stdint.h>
#include "../../Userland/SampleCodeModule/include/colores.h"




//////////////
void increasePixelScale();


void decreasePixelScale();
uint16_t getRealCharWidth();

uint16_t getRealCharHeight();
//////////////





void VDprints(const char *str, Color fnt);


void VDprint(char c, Color fnt);


void VDNewline();


void VDBackspace(Color fnt, Color bgd);

void VDclear (Color color);


void VDprintDec(uint64_t value, Color fnt, Color bgd);
void VDprintHex(uint64_t value, Color fnt, Color bgd);
void VDprintBin(uint64_t value, Color fnt, Color bgd);
void VDprintBase(uint64_t value, uint32_t base, Color fnt, Color bgd);


void VDFillArea (int x, int y, int x2, int y2, Color color);


void VDSetPixel(uint16_t x, uint16_t y, Color color);


uint16_t DVGetwidth(void);
uint16_t DVGetHeight(void);
uint32_t DVGetFrameBuffer(void);
uint8_t DVGetPixelWidth(void);


#endif