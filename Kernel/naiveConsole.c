#include <naiveConsole.h>
#include <stdint.h>  // Para usar uint32_t
#include <stdbool.h> // Para usar bool

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

extern int getSeconds();
extern int getMinutes();
extern int getHour();




void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character)
{
	*currentVideo = character; 
	currentVideo += 2;
}

void ncNewline(){
	do{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}


// Función para convertir un entero a string
void intToStr(int value, char *buffer) {
    bool isNegative = false; // Para manejar números negativos
    int i = 0;

    // Manejar el caso de 0
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0'; // Terminar la cadena
        return;
    }

    // Manejar números negativos
    if (value < 0) {
        isNegative = true;
        value = -value; // Hacer el valor positivo
    }

    // Convertir el número a string
    while (value > 0) {
        buffer[i++] = (value % 10) + '0'; // Obtener el dígito menos significativo
        value /= 10;                       // Dividir el valor por 10
    }

    if (isNegative) {
        buffer[i++] = '-'; // Agregar el signo negativo
    }

    // Terminar la cadena
    buffer[i] = '\0';

    // Invertir la cadena para tener el orden correcto
    int start = (isNegative) ? 1 : 0; // Iniciar después del signo si es negativo
    int end = i - 1;                  // Último índice válido
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}


char * getTime() {
    static char timeString[9]; // Buffer estático para "HH:MM:SS"
    char hourStr[3];           // Buffer para horas
    char minStr[3];            // Buffer para minutos
    char secStr[3];            // Buffer para segundos

    // Obtener los valores desde las funciones Assembly
    int hour = getHour(); 
    int minutes = getMinutes();
    int seconds = getSeconds();

    // Convertir cada valor a cadena
    intToStr(hour, hourStr);   // Convertir horas
    intToStr(minutes, minStr);  // Convertir minutos
    intToStr(seconds, secStr);  // Convertir segundos

    // Formatear "HH:MM:SS" en timeString
    timeString[0] = hourStr[0];
    timeString[1] = (hourStr[1] != '\0') ? hourStr[1] : '0'; // Manejar el caso de un solo dígito
    timeString[2] = ':';
    timeString[3] = minStr[0];
    timeString[4] = (minStr[1] != '\0') ? minStr[1] : '0'; // Manejar el caso de un solo dígito
    timeString[5] = ':';
    timeString[6] = secStr[0];
    timeString[7] = (secStr[1] != '\0') ? secStr[1] : '0'; // Manejar el caso de un solo dígito
    timeString[8] = '\0'; // Terminador de cadena

	return timeString;
}




void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer); 
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
