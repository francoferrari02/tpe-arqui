#include "my_time.h"

static unsigned long ticks = 0;
extern int _hlt();
int ellapsed=0;

void timer_handler() {
	ticks++;
    ellapsed += 55;
    print_5secs();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void print_5secs() {
    if (ticks % 90 == 0) {
        ncPrint("5 seconds have passed\n");
    }
}



void sleep(int millis){
	ellapsed = 0;
	while (ellapsed<millis)
	{
		_hlt();
	}
}

int ms_elapsed() {
    return ticks*5000/91;
}

void timer_wait(int delta) { 
	int initialTicks = ticks;
	while(ticks - initialTicks < delta) {
		_hlt();
        ncPrint("Waiting\n");
	}
}