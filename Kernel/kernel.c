#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <my_time.h>
#include <idtLoader.h>


extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();



void clearBSS(void * bssAddress, uint64_t bssSize)
{
    memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
    return (void*)(
        (uint64_t)&endOfKernel
        + PageSize * 8              //The size of the stack itself, 32KiB
        - sizeof(uint64_t)          //Begin at the top of the stack
    );
}

void * initializeKernelBinary()
{
    char buffer[10];

    ncPrint("[x64BareBones]");
    ncNewline();

    ncPrint("CPU Vendor:");
    ncPrint(cpuVendor(buffer));
    ncNewline();

    ncPrint("[Loading modules]");
    ncNewline();
    void * moduleAddresses[] = {
        sampleCodeModuleAddress,
        sampleDataModuleAddress
    };

    loadModules(&endOfKernelBinary, moduleAddresses);
    ncPrint("[Done]");
    ncNewline();
    ncNewline();

    ncPrint("[Initializing kernel's binary]");
    ncNewline();

    clearBSS(&bss, &endOfKernel - &bss);

    ncPrint("  text: 0x");
    ncPrintHex((uint64_t)&text);
    ncNewline();
    ncPrint("  rodata: 0x");
    ncPrintHex((uint64_t)&rodata);
    ncNewline();
    ncPrint("  data: 0x");
    ncPrintHex((uint64_t)&data);
    ncNewline();
    ncPrint("  bss: 0x");
    ncPrintHex((uint64_t)&bss);
    ncNewline();

    ncPrint("[Done]");
    ncNewline();
    ncNewline();
    return getStackBase();
}

int main()
{   
	 //load_idt();

	
    ncPrint("[Kernel Main]");
    ncNewline();
    ncPrint("  Sample code module at 0x");
    ncPrintHex((uint64_t)sampleCodeModuleAddress);
    ncNewline();
    ncPrint("  Calling the sample code module returned: ");
    ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
    ncNewline();
    ncNewline();

    ncPrint("  Sample data module at 0x");
    ncPrintHex((uint64_t)sampleDataModuleAddress);
    ncNewline();
    ncPrint("  Sample data module contents: ");
    ncPrint((char*)sampleDataModuleAddress);
    ncNewline();

    ncPrint("[Finished]");

    //ncPrintColor("Arquitectura de Computadoras", 0x7A); (ej1)

    //ej2
    VideoDriver videoDriver;
    initVideoDriver(&videoDriver);

    // Usar el driver de video para escribir en pantalla con el color de letra verde y fondo blanco
    writeString(&videoDriver, "Arquitectura de Computadoras", GREEN, BLACK);
    writeString(&videoDriver, "Hooolaa en azuull", BLUE, BLUE);
    writeString(&videoDriver, "Hooolaa en rojooooo", RED, BLACK);
    writeString(&videoDriver, "Hooolaa en marroooon", LIGHT_BROWN, BLACK);
    writeString(&videoDriver, "Hooolaa en griiiiis", LIGHT_GREY, BLACK);
    writeString(&videoDriver, "Hooolaa en azuull fondo rooojoooo", BLUE, RED);
    writeString(&videoDriver, "HOLA HOLA HOLA HOLA HOLA", LIGHT_MAGENTA, BLUE);
    
    writeString(&videoDriver, getTime(), LIGHT_BLUE, BLACK);


    //extern char * waitForKeyPress();  // Declaración de la función en ensamblador

    //char *keyPressed = waitForKeyPress();
    //writeString(&videoDriver, "tecla tocada:", LIGHT_MAGENTA, BLUE);
    //writeString(&videoDriver, keyPressed, LIGHT_MAGENTA, BLUE);
   
	// while(1) {
	// 	print_5secs();
	// 	ncNewline();
	// }
   


    return 0;
}

