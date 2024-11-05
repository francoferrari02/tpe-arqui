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



static Color WHITE = {255,255,255};
static Color RED = {0,0,255};
static Color ORANGE = {30,100,250};
static Color LIGHT_BLUE = {250,150,30};
static Color YELLOW = {5,230,250};
static Color GREEN = {60, 145, 55};


NoteType snakeSong[] = {       
        {440, 150},   
        {0, 30},
        {494, 150},   
        {0, 30},
        {523, 150},   
        {0, 30},
        {587, 150},   
        {0, 50},
        {392, 100},   
        {0, 20},
        {440, 100},  
        {0, 20},
        {349, 100},  
        {0, 20},
        {392, 100},   
        {523, 300}    
	};

NoteType snakeEatsSound[]={
    {660, 200},
    {0,50},
    {523, 200}
};

NoteType snakeDiesSound[]={
    {392, 200},
    {0, 58},
    {330, 200},
    {0, 50},
    {262, 300}
};


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
    int scoreX;
    int scoreY;
};



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
    player1->score = 0;
    player1->scoreX = 3;
    player1->scoreY = 2;
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
    player2->score = 0;
    player2->scoreX = 3;
    player2->scoreY = 3;
}


//generador de "comida" en lugares aleatorios
void generateDots(char game[HEIGHT][WIDTH], int *foodX, int *foodY) {
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

void direction(struct Player *player,char s1, char s2, char s3, char s4) {
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
void printScore(struct Player *player){
    fill_rect(player->scoreX* PIXELWIDTH,  player->scoreY* PIXELHEIGHT, PIXELWIDTH-20, PIXELHEIGHT-12, YELLOW);
    player->scoreX++;
}

//chequeo de colisiones y actualizacion de la serpiente
void situations(char game[HEIGHT][WIDTH], struct Player * player, char s1, char s2, char s3, char s4){
   if (player->directionToGo == s1) {
        player->actualY--;
    } else if (player->directionToGo == s2) {
        player->actualY++;
    } else if (player->directionToGo == s3) {
        player->actualX--;
    } else if (player->directionToGo == s4) {
        player->actualX++;
    }

    // Verifica si la serpiente se come a sí misma
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
    
    // Verifica si la serpiente murio
    if (!player->alive) {
        gameover = 1;
        player->playerColor = RED;
    }

    // Verifica si la serpiente come la comida
    if ((player->actualX == foodX && player->actualY == foodY)) {
        player->length++;
        player->score++;
        playMusic(snakeEatsSound, (sizeof(snakeEatsSound) / sizeof(NoteType)) );
        printScore(player);
        generateDots(game, &foodX, &foodY);
    }

    // Actualiza la posición de la serpiente
    if (player->alive) {
        game[player->actualY][player->actualX] = player->symbol;
        player->vecPos[0].i = player->actualY;
        player->vecPos[0].j = player->actualX;
    }
}


//UN JUGADOR___________________________________________________________________________________________________________________________________



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

    generateDots(game, &foodX, &foodY);
}


void paintGame(char game[HEIGHT][WIDTH], struct Player *player) {
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

void logic(char game[HEIGHT][WIDTH], struct Player *player, char s1, char s2, char s3, char s4) {
    situations(game,player,s1,s2,s3,s4);

    paintGame(game, player);
}

void snake1() {
    char game[HEIGHT][WIDTH];
    struct Player player;
    startGame(game, &player);
    gameover = 0;
    
    while (!gameover) {
        direction(&player,PLAYER1_UP,PLAYER1_DOWN,PLAYER1_LEFT,PLAYER1_RIGHT);
        updateSnakeLength(game, &player);
        logic(game, &player,PLAYER1_UP,PLAYER1_DOWN,PLAYER1_LEFT,PLAYER1_RIGHT);
        wait(100);
    }
    playMusic(snakeDiesSound, (sizeof(snakeDiesSound) / sizeof(NoteType)) );
    clear_scr();

    prints("\nGame Over, puntaje obtenido: ", MAX_BUFFER, WHITE);
    printDec(player.score);
    prints(". Presione espacio para continuar", MAX_BUFFER, WHITE);
    while(getChar() != ' ') {
        continue;
    }
}





//DOS JUGADORES___________________________________________________________________________________________________________________________________

struct Player player1;
struct Player player2;

void paintGame2(char game[HEIGHT][WIDTH], struct Player *player1, struct Player *player2) {
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


void startGame2(char game[HEIGHT][WIDTH], struct Player *player1, struct Player *player2) {
    setPlayer1(player1);
    setPlayer2(player2);
    game[player1->actualY][player1->actualX] = player1->symbol;
    game[player2->actualY][player2->actualX] = player2->symbol;

    int i, j;
    for (i = 4; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            game[i][j] = ' ';
        }
    }
    prints("Jugador1: ", MAX_BUFFER, WHITE);
    prints("\nJugador2: ", MAX_BUFFER, WHITE);

    generateDots(game, &foodX, &foodY);
}

void logic2(char game[HEIGHT][WIDTH], struct Player *player,char s1,char s2,char s3,char s4) {
    situations(game,player,s1,s2,s3,s4);
}

void snake2() {
    char game[HEIGHT][WIDTH];
    startGame2(game, &player1, &player2);
    gameover = 0;

    while (!gameover) {
        while (!gameover) {
            direction(&player1, PLAYER1_UP, PLAYER1_DOWN, PLAYER1_LEFT, PLAYER1_RIGHT);
            updateSnakeLength(game, &player1);
            logic2(game, &player1, PLAYER1_UP, PLAYER1_DOWN, PLAYER1_LEFT, PLAYER1_RIGHT);

            direction(&player2, PLAYER2_UP, PLAYER2_DOWN, PLAYER2_LEFT, PLAYER2_RIGHT);
            updateSnakeLength(game, &player2);
            logic2(game, &player2, PLAYER2_UP, PLAYER2_DOWN, PLAYER2_LEFT, PLAYER2_RIGHT);

            paintGame2(game, &player1, &player2);
            wait(100);
        }
        
        playMusic(snakeDiesSound, (sizeof(snakeDiesSound) / sizeof(NoteType)) );
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





// //--------------------------------------------------------------------------------------------------------------------------------

//inicia el juego
int startSnake(int option) {
    clear_scr();

    playMusic(snakeSong, (sizeof(snakeSong) / sizeof(NoteType)));
    
    if (option == 1) {
        prints("\nModo 1 jugador\n", MAX_BUFFER, WHITE);
        snake1();
    } else if (option == 2) {
        prints("\nModo 2 jugadores\n", MAX_BUFFER, WHITE);
        snake2();
    } else {
        return 0;
    }
    return 1;
}




 


