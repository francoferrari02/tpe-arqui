#include <stdio.h>
#include <videoDriver.h>



// Inicializa el driver de video
void initVideoDriver(VideoDriver *driver) {
    driver->video = VIDEO_MEMORY;
    driver->width = WIDTH;
    driver->height = HEIGHT;
	driver->cursorX=0;
	driver->cursorY=0;
}

// Limpia la pantalla
void clearScreen(VideoDriver *driver) {
    for (int i = 0; i < driver->width * driver->height; i++) {
        driver->video[i * 2] = ' ';       // Espacio en blanco
        driver->video[i * 2 + 1] = 0x07;  // Color blanco
    }
}

// Combina el color de texto y el color de fondo
uint8_t combineColors(Color textColor, Color bgColor) {
    return (bgColor << 4) | (textColor & 0x0F);
}

// Escribe un carácter en la posición (x, y) con los colores de texto y fondo especificados
void writeChar(VideoDriver *driver, char c, Color textColor, Color bgColor, uint32_t x, uint32_t y) {
    if (x < driver->width && y < driver->height) {
        uint32_t index = (y * driver->width + x) * 2;
        driver->video[index] = c;       
        driver->video[index + 1] = combineColors(textColor, bgColor); 
    }
}


// Escribe una cadena en la posición actual del cursor con los colores de texto y fondo especificados
void writeString(VideoDriver *driver, const char *str, Color textColor, Color bgColor) {
    while (*str) {
        writeChar(driver, *str, textColor, bgColor, driver->cursorX, driver->cursorY);
        driver->cursorX++; // Mueve el cursor a la derecha

        // Si alcanzamos el final de la línea, avanzamos a la siguiente fila
        if (driver->cursorX >= driver->width) {
            driver->cursorX = 0; 
            driver->cursorY++;    
        }

        // Si alcanzamos el final de la pantalla, podemos hacer scroll o reiniciar
        if (driver->cursorY >= driver->height) {
            scroll();
            driver->cursorY = driver->height - 1; // Nos quedamos en la última fila

        }
        str++; // Avanza al siguiente carácter de la cadena
    }
    // Actualiza la posición del cursor
	driver->cursorY++;
	driver->cursorX=0;
}

// Hace scroll en la pantalla cuando se llegó al final
void scroll(){
    for (int i = 0; i < WIDTH * (HEIGHT - 1) * 2; i++) {
        VIDEO_MEMORY[i] = VIDEO_MEMORY[i + WIDTH * 2];
    }
    for (int i = WIDTH * (HEIGHT - 1) * 2; i < WIDTH * HEIGHT * 2; i++) {
        VIDEO_MEMORY[i] = 0;
    }
}