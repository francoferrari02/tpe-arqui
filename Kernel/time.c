#include <time.h>
#include "include/naiveConsole.h"
#include "include/videoDriver.h"

static unsigned long ticks = 0;
int ellapsed = 0; 

void timer_handler() {
	ticks++;
	ellapsed += 55;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

int ms_elapsed() {
    return ticks*5000/91;
}

void wait(int millis){
	ellapsed = 0;
	while (ellapsed<millis)
	{
		_hlt();
	}
}

void timer_wait(int delta) { 
	int initialTicks = ticks;
	while(ticks - initialTicks < delta) {
		_hlt();
	}
}
