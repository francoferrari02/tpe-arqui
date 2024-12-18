#include <sys_calls.h>
#include <stdio.h>
#include <exceptionTest.h>
#include <colores.h>
#include "include/userLib.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

static char buffer[64] = { '0' };

int scr_height;
int scr_width;


static Color WHITE = {255,255,255};



static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

void printc (char c, Color fnt){
	sys_write(STDOUT, c, fnt);
}

void prints (const char * str, int lenght, Color fnt){
	for (int i = 0 ; i < lenght && str[i] != 0 ; i++){
		printc(str[i], fnt);
	}
}

char getChar(){
	char c;
	sys_read(0,&c);
	return c;
}


int isChar(char c){
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
		return 1;
	}
	return 0;
}

int isDigit(char c){
		if (c >= '0' && c <= '9'){
		return 1;
	}
	return 0;
}

int strlen(const char *str){
	int i = 0;
	while (str[i] != 0){
		i++;
	}

	return i;
}

int strcmp(const char *str1, const char *str2){
	int i = 0;
	int ret = 0;

	while (str1[i] != 0 && str2[i] != 0){
		if (str1[i] - str2[i] != 0){
			return str1[i] - str2[i];
		}
		i++;
	}

	if (str1[i] != 0 || str2[i] != 0){
		return str1[i] - str2[i];
	}

	return ret;
}

uint64_t charToInt(char* str){
	uint64_t ret = 0;

	if (!isDigit(str[0])){
		ret += str[0] - '0';
		return -1;
	}

	for (int i = 0 ; str[i] != 0 ; i++ ){
		if (!isDigit(str[i])){
			return -1;
		}
		ret *= 10;
		ret += str[i] - '0';
	}
	return ret;
}

void printBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    for (int i = 0 ; buffer[i] != '\0' ; i++ ){
        printc(buffer[i], WHITE);
    }
}

void printDec(uint64_t value){
    printBase(value, (uint32_t) 10);
}

void printHex(uint64_t value){
    printBase(value, (uint32_t) 16);
}

void printBin(uint64_t value){
    printBase(value, (uint32_t) 2);
}




static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    
    do{
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }while (value /= base);

    *p = 0;

    
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2){
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    return digits;
}


void clear_scr(){
	sys_clear();
}


int get_scrWidht(){
	return scr_width;
}


int get_scrHeight(){
	return scr_height;
}


static void set_screSize(){
	scr_width = sys_scrWidth();
	scr_height = sys_scrHeight();
}


void fill_rect(int x, int y, int x2, int y2, Color color){
	set_screSize();
	if ( (x >= 0 && x+x2 < scr_width) && ((y >= 0 && y+y2 < scr_height)) ){
		sys_fillRect  (x, y, x2, y2, color);
	}
}

void wait(uint64_t ms){
	sys_wait(ms);
}


static const char* register_names[17] = {
    "RIP", "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15"
};


static void uint64ToHex(uint64_t n, char buf[16]) {
    int i=15;
    do {
        int digit = n % 16;
        buf[i] = (digit < 10 ? '0' : ('A' - 10)) + digit;
        n /= 16;
    } while(i-- != 0);
}


void inforeg(){
	char hexbuf[19];
    hexbuf[0] = '0';
    hexbuf[1] = 'x';
    hexbuf[18] = '\0';

	uint64_t registers[17];
	int i = sys_inforeg(registers);

	printc('\n', WHITE);	
	if (i == 1){
		for (int i = 0; i < 17; i++) {
        	prints(register_names[i],100, WHITE);
        	prints(": ",100, WHITE);
        	uint64ToHex(registers[i], hexbuf+2);
        	prints(hexbuf,100, WHITE);
        	if (i % 4 == 3)
            printc('\n', WHITE);
        else
            prints("   ",100, WHITE);
        }


    } else {
		prints("\nTodavia no hay un snapshot de los registros, presione SHIFT + S para capturar.\n",100, WHITE);
	}
}

void playSound(uint32_t freq, uint32_t length){
	sys_playSound(freq, length);
}


void playMusic(NoteType *melody, int length)
{
	for (int i = 0; i < length; i++)
	{
		playSound(melody[i].tone, melody[i].length);
	}
}


void test_invopcode(){
	ex_invopcode();
}

void test_zerodiv(){
	ex_zerodiv();
}

void increaseScale(){
	sys_pixelPlus();
}

void decreaseScale(){
	sys_pixelMinus();
}

int print_mem(uint64_t mem){
	return sys_printmem(mem);
}