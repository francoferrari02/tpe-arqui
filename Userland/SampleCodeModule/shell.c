#include <userLib.h>
#include <stdio.h>
#include <time.h>
#include <sys_calls.h>
#include <colores.h>
#include "snake.h"
#include "shell.h"

#define MAX_BUFFER 254
#define MAX_COMMANDS 10

char line[MAX_BUFFER+1] = {0}; 
char parameter[MAX_BUFFER+1] = {0};
char command[MAX_BUFFER+1] = {0};
int linePos = 0;
char lastc;
int userDefined=0;
char username[MAX_BUFFER+1];
const char * commands[] = {"undefined","help","time","clear","snake","inforeg","zerodiv","invopcode","sizeinc","sizedec"};

static const Color WHITE = {255,255,255};
static const Color RED = {0,0,255};
static const Color YELLOW = {120,255,255};
static const Color LIGHT_BLUE = {255, 180,180};



static void newLine();
static void printLine(char c);
static int checkLine();
static void cmd_undefined();
static void cmd_help();
static void cmd_time();
static void cmd_clear();
static void cmd_snake();
static void cmd_inforeg();
static void cmd_zeroDiv();
static void cmd_invOpcode();
static void cmd_charsizeplus();
static void cmd_charsizeminus();


static void (*commands_ptr[MAX_COMMANDS])() = {cmd_undefined, cmd_help, cmd_time, cmd_clear, cmd_snake, cmd_inforeg, cmd_zeroDiv,cmd_invOpcode,
											   cmd_charsizeplus,cmd_charsizeminus};


void setUsername(){
	for (int i = 0; command[i] != '\0' && i < MAX_BUFFER; i++) {
		username[i] = command[i];
	}
	username[MAX_BUFFER] = '\0';
}


void showCommands(){
	prints("\n- time -",MAX_BUFFER, YELLOW);
	prints("          Muestra hora actual", MAX_BUFFER, WHITE);  

	prints("\n- clear -",MAX_BUFFER, YELLOW);       
	prints("         Limpia la pantalla",MAX_BUFFER, WHITE);

	prints("\n- inforeg -",MAX_BUFFER, YELLOW );
	prints("       Muestra los valores de los registros",MAX_BUFFER, WHITE);

	prints("\n- snake -",MAX_BUFFER, YELLOW );
	prints("         Inicia el juego de snake",MAX_BUFFER, WHITE );

	prints("\n- zerodiv -", MAX_BUFFER, YELLOW );
	prints("       Testea la excepcion de division por cero",MAX_BUFFER, WHITE);

	prints("\n- invopcode -",MAX_BUFFER, YELLOW );
	prints("     Testea la excepcion de codigo de operacion invalido",MAX_BUFFER, WHITE );

	prints("\n- sizedec -", MAX_BUFFER, YELLOW );
	prints("       Disminuye el tamanio de letra",MAX_BUFFER, WHITE );   

	prints("\n- sizeinc -", MAX_BUFFER, YELLOW );
	prints("       Aumenta el tamanio de letra",MAX_BUFFER, WHITE );        
	
	prints("\n- help -",MAX_BUFFER, YELLOW );
	prints("          Muestra los comandos disponibles",MAX_BUFFER, WHITE );


	printc('\n', WHITE);
}											


static void printLine(char c){
	if (linePos < MAX_BUFFER && c != lastc){
		if (isChar(c) || c == ' ' ||isDigit(c)){
			line[linePos++] = c;
			printc(c, WHITE);
		} else if (c == '\b' && linePos > 0){
			printc(c, WHITE);
			line[--linePos] = 0;
		} else if (c == '\n'){
			newLine();
		}
	}
	lastc = c;
}


static void newLine(){
	int i = checkLine();
	(*commands_ptr[i])();
	for (int i = 0; line[i] != '\0' ; i++){
		line[i] = 0;
		command[i] = 0;
		parameter[i] = 0;
	}
	linePos = 0;
	if(i != 3 ){
		prints("\n$ ",9, WHITE);
		prints(username,MAX_BUFFER, WHITE);
		prints("> ",MAX_BUFFER, WHITE);
	} else {
		prints("$ ",9, WHITE);
		prints(username,MAX_BUFFER, WHITE);
		prints("> ",MAX_BUFFER, WHITE);
	}
}


//separa comando de parametro
static int checkLine(){
	int i = 0;
	int j = 0;
	int k = 0;
	
	for ( j = 0 ; j < linePos && line[j] != ' ' ; j ++){
		command[j] = line[j];
	}
	if (j < linePos){
		j++;
		while (j < linePos){
			parameter[k++] = line[j++];
		}
	}

	for (i = 1 ; i < MAX_COMMANDS ; i++ ){
		if (!userDefined){
			setUsername();
			userDefined = 1;
			return 1;
		}else if (strcmp(command,commands[i]) == 0){
				return i;
		}
	}
	return 0;
}


static void cmd_help(){
	prints("\n\nComandos Disponibles:\n",MAX_BUFFER, LIGHT_BLUE);
	showCommands();
}

static void cmd_undefined(){
	prints("\n\nError: ",MAX_BUFFER, RED);
	prints(command,MAX_BUFFER, WHITE);
	prints("\" No se reconoce un comando valido, para ver los comandos disponibles escribir: \"help\"\n",MAX_BUFFER, WHITE );
}

static void cmd_time(){
	display_time();
}


static void cmd_snake(){
	cmd_charsizeminus();
	if(!startSnake(charToInt(parameter))){
		prints("\nNo se detecto parametro valido: '1' o '2' jugadores.\n Ejemplo: snake 1",MAX_BUFFER, WHITE ); 
	}
	
}

static void cmd_clear(){
	clear_scr();
}

static void cmd_inforeg(){
	inforeg();
}

static void cmd_charsizeplus(){
	increaseScale();
}

static void cmd_charsizeminus(){
	decreaseScale();
}

static void cmd_invOpcode(){
	test_invopcode();
}

static void cmd_zeroDiv(){
	test_zerodiv();
}

void shell(){
	
	while(1){
		char c;
		c = getChar();
		printLine(c);
	};
}

void start(){
	prints("\nBienvenido a la Shell hecha por Franco y Mateo!\n\n",MAX_BUFFER, LIGHT_BLUE);

	NoteType SOmelody[] = {
		{784, 200},   
		{0, 50},
		{659, 200},   
		{0, 50},
		{523, 200},   
		{0, 50},
		{659, 150},  
		{784, 400},  
		{0, 100},
		{392, 500}    
	};

	playMusic(SOmelody, (sizeof(SOmelody) / sizeof(NoteType)));
	prints("Ingrese su nombre de usuario: ",MAX_BUFFER, WHITE);

}
