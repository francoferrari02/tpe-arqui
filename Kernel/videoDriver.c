#include <videoDriver.h>
#include <stdint.h>
#include <font.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
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
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;



void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

Color RED = {30, 30, 255};
Color WHITE = {255, 255, 255};
Color BLACK = {0, 0, 0};
Color GREEN = {0, 255, 0};
Color BLUE = {0, 0, 255};
Color YELLOW = {255, 255, 0};
Color CYAN = {0, 255, 255};
Color MAGENTA = {255, 0, 255};
Color ORANGE = {255, 165, 0};
Color PURPLE = {128, 0, 128};
Color BROWN = {165, 42, 42};
Color GRAY = {128, 128, 128};
Color LIGHT_GRAY = {211, 211, 211};
Color DARK_GRAY = {169, 169, 169};
Color PINK = {255, 192, 203};


/* draws a char in screen in the given coordinates, then sets the coordinates where the next char should be drawn */
static void drawChar (int x, int y, unsigned char c,Color fntColor, Color bgColor);
/* Moves everything up one line */
static void scrollUp ();
/* get a number in dec from another base */
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
/* Gets the pixel pointer in screen from the (x,y) coordinate*/
static uint32_t* getPixelPtr(uint16_t x, uint16_t y);



//tendria que modificar esto para que pueda cambiar el tamanio de la letra
// Ajusta los valores predeterminados
const uint16_t CHAR_WIDTH = 9;
const uint16_t CHAR_HEIGHT = 16;

//////////////////////////////////////////////////////////////////////////////////


// Establecer un factor de escala predeterminado
uint8_t pixelScale = 1;

// Aumentar el factor de escala para aumentar el tamaño de un carácter
void increasePixelScale() {
    if (pixelScale < 5) {
        pixelScale++;
    }
}

// Disminuir el factor de escala para reducir el tamaño de un carácter
void decreasePixelScale() {
    if (pixelScale > 1) {
        pixelScale--;
    }
}

// Obtener el ancho real de un carácter según el factor de escala actual
uint16_t getRealCharWidth() {
    return CHAR_WIDTH * pixelScale;
}

// Obtener el alto real de un carácter según el factor de escala actual
uint16_t getRealCharHeight() {
    return CHAR_HEIGHT * pixelScale;
}

//////////////////////////////////////////////////////////////////////////////////
/* screen coordinates where the next char will be written */
uint16_t cursorX = 0;
uint16_t cursorY = 0;

static char buffer[64] = { '0' };


void dv_prints(const char *str, Color fnt, Color bgd){
    for (int i = 0 ; str[i] != '\0'; i++ ){
        dv_print(str[i], fnt, bgd);
    }
}


void dv_print(const char c, Color fnt, Color bgd){
    switch (c) {
        case '\n':
            dv_newline();
        break;
        case '\b':
            dv_backspace(fnt, bgd);
        break;
        case '\0':
            /* nada, no imprime nada */
        break;
        default:
            drawChar(cursorX, cursorY , c , fnt , bgd);
        break;
    }
}


void dv_newline(){
    cursorX = 0;
    cursorY += CHAR_HEIGHT* pixelScale;

    if (cursorY + CHAR_HEIGHT*pixelScale > VBE_mode_info->height){
        cursorY -= CHAR_HEIGHT*pixelScale;
        scrollUp();
    }
}


void dv_backspace(Color fnt, Color bgd){
    if (cursorX >= CHAR_WIDTH*pixelScale){
        cursorX -= CHAR_WIDTH*pixelScale;
    } else {
        cursorX = 0;
    }
    drawChar(cursorX, cursorY , ' ' , fnt , bgd);
    cursorX -= CHAR_WIDTH*pixelScale;
}


void dv_clear (Color color) {
    Color* pixel = (Color*) ((uint64_t)VBE_mode_info->framebuffer);

    //recorro todos los pixeles de la pantalla y los pongo del color que quiero
    for (uint32_t len = (uint32_t)VBE_mode_info->width * VBE_mode_info->height; len; len--, pixel++){
        *pixel = color;
    }

    //seteo los cursores en el inicio (arriba a la izquierda)
    cursorX = 0;
    cursorY = 0;
}


static void drawChar(int x, int y, unsigned char c, Color fntColor, Color bgColor) {
    //con estos indices recorro el caracter
    int cx, cy;
    //mascara de bits para saber que color imprimo a pantalla, si pertenece a caracter o a fondo
    int mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    //puntero a donde inician los datos de mi caracter, hago (c-32 pues los caracteres imprimibles arrancan en el 32), lo multiplico por 16 pues es lo que ocupa
    //un caracter en el bitmap
    const unsigned char *glyph = IBM_VGA_8x16_glyph_bitmap + 16 * (c - 32);

    // Chequeo que no sea el final de línea, ni el final de la pantalla
    if (cursorX >= VBE_mode_info->width) {
        cursorX = 0;
        if (cursorY + getRealCharHeight() > VBE_mode_info->height) {
            cursorY -= getRealCharHeight();
            scrollUp();
        } else {
            cursorY += getRealCharHeight();
        }
    }

    for (cy = 0; cy < 16; cy++) {
        for (cx = 0; cx < 8; cx++) {
            // Uso el factor de escala
            for (int i = 0; i < pixelScale; i++) {
                for (int j = 0; j < pixelScale; j++) {
                    dv_setPixel(cursorX + (8 - cx) * pixelScale + i, cursorY + cy * pixelScale + j, glyph[cy] & mask[cx] ? fntColor : bgColor);
                }
            }
        }
    }

    cursorX += getRealCharWidth();
}



static void scrollUp (){
    Color* pixel, *next;
    for (int i = 0 ; i < cursorY + CHAR_HEIGHT*pixelScale ; i++){
        for (int j = 0 ; j < VBE_mode_info->width ; j++){
            pixel = (Color *) getPixelPtr(j,i);
            next = (Color *) getPixelPtr(j,i+CHAR_HEIGHT*pixelScale);
            *pixel = *next;
        }
    }
}


void dv_printDec(uint64_t value, Color fnt, Color bgd){
    dv_printBase(value, 10,fnt,bgd);
}

void dv_printHex(uint64_t value, Color fnt, Color bgd){
    dv_printBase(value, 16,fnt,bgd);
}

void dv_printBin(uint64_t value, Color fnt, Color bgd){
    dv_printBase(value, 2,fnt,bgd);
}

void dv_printBase(uint64_t value, uint32_t base, Color fnt, Color bgd){
    uintToBase(value, buffer, base);
    for (int i = 0 ; buffer[i] != '\0' ; i++ ){
        dv_print(buffer[i], fnt, bgd);
    }
}


static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    //Calculate characters for each digit
    do{
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
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




//te paso una coordenada de la pantalla en (x,y) y te devuelvo la direccion de la pantalla que representa ese pixel
static uint32_t* getPixelPtr(uint16_t x, uint16_t y) {
    uint8_t pixelwidth = VBE_mode_info->bpp/8;     //la cantidad de bytes hasta el siguiente pixel a la derecha (bpp: BITS per px)
    uint16_t pixelHeight = VBE_mode_info->pitch;   //la cantidad de bytes hasta el pixel hacia abajo

    uintptr_t pixelPtr = (uintptr_t)(VBE_mode_info->framebuffer) + (x * pixelwidth) + (y * pixelHeight);
    return (uint32_t*)pixelPtr;
}



//pinto un pixel de la pantalla con el color que quiero, 
void dv_setPixel(uint16_t x, uint16_t y, Color color) {
    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height)
        return;

    Color* pixel = (Color*) getPixelPtr(x, y);
    *pixel = color;
}


//dibujo un cuadrado
void dv_fillRect (int x, int y, int w, int h, Color color){
    Color * pixel;

    for (int i = 0 ; i < h ; i++){
        pixel = (Color*) getPixelPtr(x,y+i);
        for (int j = 0 ; j < w ; j++, pixel++){
            *pixel = color;
        }
    }
}


uint16_t dv_getWidth(void) {
    return VBE_mode_info->width;
}

uint16_t dv_getHeight(void) {
    return VBE_mode_info->height;
}

uint32_t dv_getFrameBuffer(void) {
    return VBE_mode_info->framebuffer;
}

uint8_t dv_getPixelWidth(void){
    return VBE_mode_info->bpp;
}

uint16_t dv_getPitch(void){
    return VBE_mode_info->pitch;
}