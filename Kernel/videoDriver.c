#include "videoDriver.h"
#include "font.h"
#include "time.h"
#include "../Userland/SampleCodeModule/include/colores.h"


struct vbe_mode_info_structure {
    uint16_t attributes;        // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;           // deprecated
    uint8_t window_b;           // deprecated
    uint16_t granularity;       // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;      // deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;             // number of bytes per horizontal line
    uint16_t width;             // width in pixels
    uint16_t height;            // height in pixels
    uint8_t w_char;             // unused...
    uint8_t y_char;             // ...
    uint8_t planes;
    uint8_t bpp;                // bits per pixel in this mode
    uint8_t banks;              // deprecated; total number of banks in this mode
    uint8_t memory_model;   
    uint8_t bank_size;          // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;           // physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;   // size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__ ((packed));         // no alinea en memoria

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

static Color BLACK = {0,0,0};


/* draws a char in screen in the given coordinates, then sets the coordinates where the next char should be drawn */
static void PrintChar (int x, int y, unsigned char c,Color fntColor, Color bgColor);
/* Moves everything up one line */
static void VDScroll ();
/* get a number in dec from another base */
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
/* Gets the pixel pointer in screen from the (x,y) coordinate*/
static uint32_t* getPixelPtr(uint16_t x, uint16_t y);




const uint16_t CHAR_WIDTH = 9;
const uint16_t CHAR_HEIGHT = 16;

//////////////////////////////////////////////////////////////////////////////////



uint8_t pixelScale = 1;

void increasePixelScale() {
    if (pixelScale < 5) {
        pixelScale++;
    }
}


void decreasePixelScale() {
    if (pixelScale > 1) {
        pixelScale--;
    }
}


uint16_t getRealCharWidth() {
    return CHAR_WIDTH * pixelScale;
}


uint16_t getRealCharHeight() {
    return CHAR_HEIGHT * pixelScale;
}

//////////////////////////////////////////////////////////////////////////////////
/* screen coordinates where the next char will be written */
uint16_t cursorX = 0;
uint16_t cursorY = 0;

static char buffer[64] = { '0' };


void VDprints(const char *str, Color fnt){
    for (int i = 0 ; str[i] != '\0'; i++ ){
        VDprint(str[i], fnt);
    }
}


void VDprint(const char c, Color fnt){
    switch (c) {
        case '\n':
            VDNewline();
        break;
        case '\b':
            VDBackspace(fnt, BLACK);
        break;
        case '\0':
            /* nada, no imprime nada */
        break;
        default:
            PrintChar(cursorX, cursorY , c , fnt , BLACK);
        break;
    }
}


void VDNewline(){
    cursorX = 0;
    cursorY += CHAR_HEIGHT* pixelScale;

    if (cursorY + CHAR_HEIGHT*pixelScale > VBE_mode_info->height){
        cursorY -= CHAR_HEIGHT*pixelScale;
        VDScroll();
    }
}


void VDBackspace(Color fnt, Color bgd){
    if (cursorX >= CHAR_WIDTH*pixelScale){
        cursorX -= CHAR_WIDTH*pixelScale;
    } else {
        cursorX = 0;
    }
    PrintChar(cursorX, cursorY , ' ' , fnt , bgd);
    cursorX -= CHAR_WIDTH*pixelScale;
}


void VDclear (Color color) {
    Color* pixel = (Color*) ((uint64_t)VBE_mode_info->framebuffer);

    
    for (uint32_t len = (uint32_t)VBE_mode_info->width * VBE_mode_info->height; len; len--, pixel++){
        *pixel = color;
    }

    
    cursorX = 0;
    cursorY = 0;
}


static void PrintChar(int x, int y, unsigned char c, Color fntColor, Color bgColor) {
    
    int cx, cy;
    
    int mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    const unsigned char *glyph = IBM_VGA_8x16_glyph_bitmap + 16 * (c - 32);

    
    if (cursorX >= VBE_mode_info->width) {
        cursorX = 0;
        if (cursorY + getRealCharHeight() > VBE_mode_info->height) {
            cursorY -= getRealCharHeight();
            VDScroll();
        } else {
            cursorY += getRealCharHeight();
        }
    }

    for (cy = 0; cy < 16; cy++) {
        for (cx = 0; cx < 8; cx++) {
            // Uso el factor de escala
            for (int i = 0; i < pixelScale; i++) {
                for (int j = 0; j < pixelScale; j++) {
                    VDSetPixel(cursorX + (8 - cx) * pixelScale + i, cursorY + cy * pixelScale + j, glyph[cy] & mask[cx] ? fntColor : bgColor);
                }
            }
        }
    }

    cursorX += getRealCharWidth();
}



static void VDScroll (){
    Color* pixel, *next;
    for (int i = 0 ; i < cursorY + CHAR_HEIGHT*pixelScale ; i++){
        for (int j = 0 ; j < VBE_mode_info->width ; j++){
            pixel = (Color *) getPixelPtr(j,i);
            next = (Color *) getPixelPtr(j,i+CHAR_HEIGHT*pixelScale);
            *pixel = *next;
        }
    }
}


void VDprintDec(uint64_t value, Color fnt, Color bgd){
    VDprintBase(value, 10,fnt,bgd);
}

void VDprintHex(uint64_t value, Color fnt, Color bgd){
    VDprintBase(value, 16,fnt,bgd);
}

void VDprintBin(uint64_t value, Color fnt, Color bgd){
    VDprintBase(value, 2,fnt,bgd);
}

void VDprintBase(uint64_t value, uint32_t base, Color fnt, Color bgd){
    uintToBase(value, buffer, base);
    for (int i = 0 ; buffer[i] != '\0' ; i++ ){
        VDprint(buffer[i], fnt);
    }
}


static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    
    do{
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }while (value /= base);

    
    *p = 0;

    
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2){
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    return digits;
}





static uint32_t* getPixelPtr(uint16_t x, uint16_t y) {
    uint8_t pixelwidth = VBE_mode_info->bpp/8;     
    uint16_t pixelHeight = VBE_mode_info->pitch;   
    uintptr_t pixelPtr = (uintptr_t)(VBE_mode_info->framebuffer) + (x * pixelwidth) + (y * pixelHeight);
    return (uint32_t*)pixelPtr;
}



 
void VDSetPixel(uint16_t x, uint16_t y, Color color) {
    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height)
        return;

    Color* pixel = (Color*) getPixelPtr(x, y);
    *pixel = color;
}



void VDFillArea (int x, int y, int w, int h, Color color){
    Color * pixel;

    for (int i = 0 ; i < h ; i++){
        pixel = (Color*) getPixelPtr(x,y+i);
        for (int j = 0 ; j < w ; j++, pixel++){
            *pixel = color;
        }
    }
}


uint16_t DVGetwidth(void) {
    return VBE_mode_info->width;
}

uint16_t DVGetHeight(void) {
    return VBE_mode_info->height;
}

uint32_t DVGetFrameBuffer(void) {
    return VBE_mode_info->framebuffer;
}

uint8_t DVGetPixelWidth(void){
    return VBE_mode_info->bpp;
}

uint16_t DVGetPitch(void){
    return VBE_mode_info->pitch;
}