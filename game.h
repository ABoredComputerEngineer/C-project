#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOARD 9
#define BOARD_ROW 3
#define BOARD_COL 3
#define PLAYER1 1
#define PLAYER2 2
#define TIE 3
#define TRUE 1
#define FALSE 0 
#define GARBAGE 322
#define AI 1
#define HUMAN 0
#define SINGLE 1
#define DOUBLE 2

#define MAX_OPTIONS 3
#define MAX_IDENTIFIERS 10
#define MAX_COMMANDS 4
#define IN 1
#define OUT 0


#define NEWLINE printf("------------------------------------------------------\n")
typedef struct game {
	char mat[BOARD_ROW][BOARD_COL];// The game board stored in matrix form
	char row[BOARD]; //The game board stored in a single array
} game; 


typedef struct player {
	char name[100];
	short int type;
	short int position;
	char sign;
} player;

typedef struct settings {
	player p1;
	player p2;
	player ai;
	int difficulty;
	int win;
	int loss;
} settings;


// aiBoard *newBoard( void );
// void setDepth( void );
int ctoi( char c );
void initializeBoard(void);
void printBoard( void );
int hasEnded( void );
void performMove( int , int  );
int isLegal( int x  );
int getMove( void );
void playGame(int);
int playSingle(void);
int playDouble(void);
void getString( char * );
void displayScore( void );
// int aiMove(void);


typedef struct command {
	char name[6];
	struct identifier *identifierList[3];
} command;


typedef struct identifier {
	char name[15];
	struct option *optionList[4];
} identifier;


typedef struct option{
	char name[50];
	char identifierType[15];
	char value[100];
	// struct option *next;
} option;

typedef struct wl {
	short int win;
	short int lose;
} score;

typedef struct gs{
	score p1;
	score p2;
	score ai;
	int gcSingle;
	int gcDouble;
} stat;


enum difficulty {EASY=1,MEDIUM,HARD};
enum tokens {COMMAND,IDENTIFIER,OPTION,VALUE};
enum commands {set,new,view};

/* Declaration of the Global Variables */
game gameBoard;
// int turn = 1;
short int maxDepth;
player player1,player2,ai,human,p1,p2;
settings gameSet;
stat gameStat;
