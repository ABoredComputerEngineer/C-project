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
#define MAX_IDENTIFIERS 11
#define MAX_COMMANDS 20
#define IN 1
#define OUT 0


#define NEWLINE printf("------------------------------------------------------------\n")
#define NEWSCREEN printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")
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
void applyDefault(void);
// int aiMove(void);
void newSettings(void);
void writeSettings(void);
void readSettings(void);
void newSettings(void);



enum errors {err_command,err_identifier,err_move};
void displayError(enum errors , char *);
void displayViewHelp(void);
void displaySetHelp(void);
void displayNewHelp(void);
void displayHelp(void);
void displayHeading(char *);
enum setting {name,ai_name,sign,mode};
void displayChangeSettings(enum setting, char *prev, char *new);
/* Declaration of the Global Variables */
game gameBoard;
// int turn = 1;
short int maxDepth;
player player1,player2,ai,human,p1,p2;
