#include <stdint.h>
#include <naiveConsole.h>

typedef enum{
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHT_GREY, DARK_GREY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, 
    LIGHT_MAGENTA, LIGHT_BROWN, WHITE,
}Color;


#define VIDEO_MEMORY ((uint8_t*)0xB8000) //direccion donde comienza la memoria de video
#define WIDTH 80
#define HEIGHT 25

typedef struct{
    uint8_t *video; //puntero a la memoria de video
    uint32_t width; //ancho de pantalla
    uint32_t height;//alto de pantalla
    uint32_t cursorX; // Posición en X
    uint32_t cursorY; // Posición en Y
}VideoDriver;

void initVideoDriver(VideoDriver *driver);
void clearScreen(VideoDriver *driver);
uint8_t combineColors(Color textColor, Color bgColor);
void writeChar(VideoDriver *driver, char c, Color textColor, Color bgColor, uint32_t x, uint32_t y);
void writeString(VideoDriver *driver, const char *str, Color textColor, Color bgColor);
void scroll();


