#include <sys_calls.h>
#include <userLib.h>
#include <colores.h>
static Color WHITE = {255,255,255};
static Color BLUE = {250,100,30};

#define DIFF 3

int getHours(){
	return sys_getHours();
}

int getMinutes(){
	return sys_getMinutes();
}

int getSeconds(){
	return sys_getSeconds();
}

void display_time(){
	int hours, minutes, seconds;

	hours = getHours();
	minutes = getMinutes();
	seconds = getSeconds();
	
	printc('\n', WHITE);
	printDec(hours - DIFF);
	printc(':', BLUE);
	printDec(minutes);
	printc(':', BLUE);
	printDec(seconds);
}
