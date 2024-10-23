#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <videoDriver.h>

static void int_20();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
	}
	return;
}

void int_20() {
	dv_prints("Tick",RED ,BLACK );	
	dv_prints("Tick",ORANGE ,BLACK );	
	dv_prints("Tick",YELLOW ,BLACK );	
	dv_prints("Tick",GREEN ,BLACK );	
	dv_prints("Tick",BLUE ,BLACK );	
	dv_prints("Tick",PURPLE ,BLACK );	
	
	ncNewline();
	timer_handler();
}
