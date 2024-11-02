#include "snake.h"
#include <userLib.h>
#include <colores.h>
#include "sys_calls.h"


static unsigned long int next = 1;

int rand() {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}


#define WIDTH 40
#define HEIGHT 40
#define MAXDIM 100



static Color BLACK = {0,0,0};
static Color WHITE = {255,255,255};
static Color RED = {0,0,255};
static Color ORANGE = {30,100,250};
static Color LIGHT_BLUE = {250,150,30};
static Color YELLOW = {5,230,250};

//map colors
static Color GREEN = {60, 145, 55};
// static Color BROWN = {50, 60, 100};
// static Color TREE_GREEN = {5, 65, 30};


#define PIXELWIDTH (get_scrWidht() / WIDTH)
#define PIXELHEIGHT (get_scrHeight() / HEIGHT)
#define MAX_BUFFER 254

#define PLAYER1_UP 'w'
#define PLAYER1_DOWN 's'
#define PLAYER1_LEFT 'a'
#define PLAYER1_RIGHT 'd'

#define PLAYER2_UP 'i'
#define PLAYER2_DOWN 'k'
#define PLAYER2_LEFT 'j'
#define PLAYER2_RIGHT 'l'

int score1Y = 2;
int score1X = 3;
int score2y = 3;
int score2X = 3;

int gameover;
int foodX, foodY;

struct Position {
    int i;
    int j;
};

struct Player {
    int playerNumber;
    int actualX;
    int actualY;
    int directionToGo;
    int alive;
    int score;
    char symbol;
    Color playerColor;
    int length;
    struct Position vecPos[MAXDIM];
};

NoteType snakeSong[] = {
		{622, 300}, // D#5
		{0, 25},
		{466, 300}, // A#4
		{0, 50},
		{415, 450}, // G#4
		{0, 25},
		{622, 250}, // D#5
		{466, 900}	// A#4
	};





void generateFood(char game[HEIGHT][WIDTH], int *foodX, int *foodY) {
    
    do {
        *foodX = rand() % (WIDTH-2) + 1;
        *foodY = rand() % (HEIGHT-2) + 1;
    } while (game[*foodY][*foodX] != ' ');

    game[*foodY][*foodX] = '*';
   

}

void clearCell(char game[HEIGHT][WIDTH], int i, int j) {
    game[i][j] = ' ';
}

void updateSnakeLength(char game[HEIGHT][WIDTH], struct Player *player) {
    if (player->length >= 1) {
        
        clearCell(game, player->vecPos[player->length - 1].i, player->vecPos[player->length - 1].j);
        for (int i = player->length - 1; i > 0; i--) {
            player->vecPos[i] = player->vecPos[i - 1];
        }
    }
}

void input(struct Player *player,char s1, char s2, char s3, char s4) {
    char ch;
    ch = getChar();

    if (ch == s1 && player->directionToGo != s2) {
        player->directionToGo = s1;
    } else if (ch == s2 && player->directionToGo != s1) {
        player->directionToGo = s2;
    } else if (ch == s3 && player->directionToGo != s4) {
        player->directionToGo = s3;
    } else if (ch == s4 && player->directionToGo != s3) {
        player->directionToGo = s4;
    }
}

//impresion del puntaje de cada snake
void printScore1(){
    fill_rect(score1X* PIXELWIDTH,  score1Y* PIXELHEIGHT, PIXELWIDTH-20, PIXELHEIGHT-12, YELLOW);
    score1X++;
}
void printScore2(){
    fill_rect(score2X* PIXELWIDTH,  score2y* PIXELHEIGHT, PIXELWIDTH-20, PIXELHEIGHT-12, YELLOW);
    score2X++;
}

//chequeo de colisiones y actualizacion de la serpiente
void inLogic(char game[HEIGHT][WIDTH], struct Player * player, char s1, char s2, char s3, char s4){
   if (player->directionToGo == s1) {
        player->actualY--;
    } else if (player->directionToGo == s2) {
        player->actualY++;
    } else if (player->directionToGo == s3) {
        player->actualX--;
    } else if (player->directionToGo == s4) {
        player->actualX++;
    }

    // Verificar si la serpiente se come a sí misma
    for (int i = 0; i < player->length; i++) {
        if (player->actualX == player->vecPos[i].j && player->actualY == player->vecPos[i].i) {
            player->alive = 0;
            break;
        }
    }

    // Verifica si la serpiente chocó con el borde de la pantalla
    if (player->actualX < 0 || player->actualX >= WIDTH || player->actualY < 4 || player->actualY >= HEIGHT) {
        player->alive = 0;
        
    }

    if (!player->alive) {
        gameover = 1;
        player->playerColor = RED;
    }

    if ((player->actualX == foodX && player->actualY == foodY) && player->playerNumber == 1) {
        player->length++;
        player->score++;
        printScore1();
        
        generateFood(game, &foodX, &foodY);
    } else {
        if ((player->actualX == foodX && player->actualY == foodY) && player->playerNumber == 2) {
            player->length++;
            player->score++;
            printScore2();
            
            generateFood(game, &foodX, &foodY);
        }
    }

    if (player->alive) {
        game[player->actualY][player->actualX] = player->symbol;
        player->vecPos[0].i = player->actualY;
        player->vecPos[0].j = player->actualX;
    }
}


struct Player player1;
struct Player player2;


//inicializa snake1
void setPlayer1(struct Player *player1) {
    player1->playerNumber = 1;
    player1->actualX = WIDTH / 4;
    player1->actualY = HEIGHT / 2;
    player1->directionToGo = PLAYER1_UP;
    player1->alive = 1;
    player1->symbol = '#';
    player1->playerColor = LIGHT_BLUE;
    player1->length = 2;
}

//inicializa snake2
void setPlayer2(struct Player *player2) {
    player2->playerNumber = 2;
    player2->actualX = 3 * WIDTH / 4;
    player2->actualY = HEIGHT / 2;
    player2->directionToGo = PLAYER2_UP;
    player2->alive = 1;
    player2->symbol = '@';
    player2->playerColor = ORANGE;
    player2->length = 2;
}


//UN JUGADOR___________________________________________________________________________________________________________________________________

void drawBoard(char game[HEIGHT][WIDTH], struct Player *player) {
    Color currentColor;
    int i, j;
    for (i = 4; i < HEIGHT ; i++) {
        for (j =0; j <= WIDTH; j++) {
            if (game[i][j] == ' ') {
                currentColor = GREEN;
            } else if (game[i][j] == player->symbol) {
                currentColor = player->playerColor;
            } else if (game[i][j] == '*'){
                currentColor = YELLOW;
            }
            fill_rect(j * PIXELWIDTH, i * PIXELHEIGHT, PIXELWIDTH, PIXELHEIGHT, currentColor);
        }
    }
}

void startGame(char game[HEIGHT][WIDTH], struct Player *player) {
    setPlayer1(player);

    game[player->actualY][player->actualX] = player->symbol;

    int i, j;
    for (i = 4; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            game[i][j] = ' ';
        }
    }
    prints("Puntaje: ", MAX_BUFFER, WHITE);

    generateFood(game, &foodX, &foodY);
    

}


void logic(char game[HEIGHT][WIDTH], struct Player *player, char s1, char s2, char s3, char s4) {
    inLogic(game,player,s1,s2,s3,s4);

    drawBoard(game, player);
}



void snakeGame() {
    char game[HEIGHT][WIDTH];
    struct Player player;
    startGame(game, &player);
    gameover = 0;
    

    while (!gameover) {
        input(&player,PLAYER1_UP,PLAYER1_DOWN,PLAYER1_LEFT,PLAYER1_RIGHT);
        updateSnakeLength(game, &player);
        logic(game, &player,PLAYER1_UP,PLAYER1_DOWN,PLAYER1_LEFT,PLAYER1_RIGHT);
        wait(100);

    }
    clear_scr();
    prints("\nGame Over, puntaje obtenido: ", MAX_BUFFER, WHITE);
    printDec(player.score);
    prints(". Presione espacio para continuar", MAX_BUFFER, WHITE);
    while(getChar() != ' ') {
        continue;
    }
   
}



//DOS JUGADORES___________________________________________________________________________________________________________________________________

void drawBoard2(char game[HEIGHT][WIDTH], struct Player *player1, struct Player *player2) {
    Color currentColor;
    int i, j;
    for (i = 4; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (game[i][j] == ' ') {
                currentColor = GREEN;
            } else if (game[i][j] == player1->symbol) {
                currentColor = player1->playerColor;
            } else if (game[i][j] == player2->symbol) {
                currentColor = player2->playerColor;
            } else if (game[i][j] == '*') {
                currentColor = YELLOW;
            }
            fill_rect(j * PIXELWIDTH, i * PIXELHEIGHT, PIXELWIDTH , PIXELHEIGHT , currentColor);
        }
    }
}

void startGame2Players(char game[HEIGHT][WIDTH], struct Player *player1, struct Player *player2) {
    setPlayer1(player1);
    setPlayer2(player2);

    game[player1->actualY][player1->actualX] = player1->symbol;
    game[player2->actualY][player2->actualX] = player2->symbol;

    // Inicializa el tablero y la comida, asegurándote de que las serpientes no se superpongan con la comida inicialmente
    int i, j;
    for (i = 4; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            game[i][j] = ' ';
        }
    }
    prints("Jugador1: ", MAX_BUFFER, WHITE);
    prints("\nJugador2: ", MAX_BUFFER, WHITE);

    generateFood(game, &foodX, &foodY);
}


void logic2(char game[HEIGHT][WIDTH], struct Player *player,char s1,char s2,char s3,char s4) {
    inLogic(game,player,s1,s2,s3,s4);
}



void snakeGame2Players() {
    char game[HEIGHT][WIDTH];
    startGame2Players(game, &player1, &player2);
    gameover = 0;

    while (!gameover) {
        while (!gameover) {

    
        input(&player1, PLAYER1_UP, PLAYER1_DOWN, PLAYER1_LEFT, PLAYER1_RIGHT);
        updateSnakeLength(game, &player1);
        logic2(game, &player1, PLAYER1_UP, PLAYER1_DOWN, PLAYER1_LEFT, PLAYER1_RIGHT);

        input(&player2, PLAYER2_UP, PLAYER2_DOWN, PLAYER2_LEFT, PLAYER2_RIGHT);
        updateSnakeLength(game, &player2);
        logic2(game, &player2, PLAYER2_UP, PLAYER2_DOWN, PLAYER2_LEFT, PLAYER2_RIGHT);

        drawBoard2(game, &player1, &player2);
        wait(100);


    }
    fill_rect(0, 0, get_scrWidht() / 2, get_scrHeight() / 8, BLACK);
    clear_scr();
    prints("\nGame Over, puntaje obtenido: ", MAX_BUFFER, WHITE);
    prints("\nJugador 1: ", MAX_BUFFER, WHITE);
    printDec(player1.score);
    prints("\nJugador 2: ", MAX_BUFFER, WHITE);
    printDec(player2.score);
    prints(". \nPresione espacio para continuar.", MAX_BUFFER, WHITE);
    while(getChar() != ' ') {
         continue;
    }

    
}
}


//--------------------------------------------------------------------------------------------------------------------------------

//inicia el juego
int startSnake(int option) {
    clear_scr();

    playMusic(snakeSong, (sizeof(snakeSong) / sizeof(NoteType)));

    if (option == 1) {
        prints("\nModo 1 jugador\n", MAX_BUFFER, WHITE);
        snakeGame();
    } else if (option == 2) {
        prints("\nModo 2 jugadores\n", MAX_BUFFER, WHITE);
        snakeGame2Players();
    } else {
        return 0;
    }
    return 1;
}