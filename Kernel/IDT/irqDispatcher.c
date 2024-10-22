#include <time.h>
#include <stdint.h>

static void int_20();
//cambiar el switch por un arreglo de punteros a funcion

void irqDispatcher(uint64_t irq) {
	//cambiar el switch por un arreglo de punteros a funcion
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
	}
	return;
}



void int_20() {
	timer_handler();
}

void int_21(){
	uint8_t keyPressed = getCharFromKeyboard();
	keyboard_handler(keyPressed);
}
