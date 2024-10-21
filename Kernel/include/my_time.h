#ifndef _TIME_H_
#define _TIME_H_

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void print_5secs();

void sleep(int millis);

int ms_elapsed(); 

void timer_wait(int delta); 

#endif
