#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
    #define PATH "gameSet.dat"  // Windows
#elif defined(_WIN64)
   #define PATH "gameSet.dat" // Windows
#elif defined(__linux__)
    #define PATH "./gameSet.dat" 
#endif

#define BOARD 9
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
#define MAX_IDENTIFIERS 9
#define MAX_COMMANDS 6
#define IN 1
#define OUT 0


#define NEWLINE printf("--------------------------------------------------------------------\n")
#define NEWSCREEN printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")
typedef struct game {
	char row[BOARD]; //The game board stored in a single array
} game; 


typedef struct player {
	char name[100];
	short int type; // HUMAN if player is human, AI if player is computer
	short int position; // PLAYER1 if first player, PLAYER2 is 2nd player
	char sign; // sign of player the appears on board
} player;


/*Fucntions for core game*/
int ctoi( char c ); // Converts character to an integer
void initializeBoard(void); // empties the game board
void printBoard( void ); // prints the board

/* hasEnded() checks if game has Ended.
	returns PLAYER1 if player1 wins
	PLAYER2 if player2 wins
	TIE if game is tied
	FALSE if it has not ended */
int hasEnded( void );  

void performMove( int , int  ); // places the sign of given player (2nd argument) in the given cell ( 1st argument)
int isLegal( int x  ); // checks if the move performed is legal or not
int getMove( void ); // takes input from user and always return a valid move
void playGame(int); // initialized all game related variables before starting a game
int playSingle(void); // starts a single player game, returns the position of winner
int playDouble(void); // starts two player game, return the position of winner
void getString( char * ); // takes input of string using getchar() and stores in array
void displayScore( void ); // displayes the game score
void applyDefault(void); // applies the default settings

/*Functions for file handling*/
void newSettings(void);
void writeSettings(void);
void readSettings(void);


/* Functions for error handling and displaying */
enum errors {err_command,err_identifier,err_move};
void displayError(enum errors , char *);

/* Input Output functions */
void removeQuotes( char * ); // removes the quotes from a given string
// void getString(char *);
int stringSplit( char *, char [][100]); // splits as string into array of words, words with in quotes are taken as single word
void displayViewHelp(void);
void displaySetHelp(void);
void displayNewHelp(void);
void displayHelp(void);
void displayHeading(char *);
void displayCredits(void);
enum setting {name,ai_name,sign,mode}; // required for error handling

void displayChangeSettings(enum setting, char *, char *);
void displaySettings( void ); // Displays the game settings
void displayFrontPage( void );
void displayScoreCurrent( void );
void displayScoreLifeTime( void );


/* Declaration of the Global Variables. Almost required in all functions */
game gameBoard; // the game stored in 1D array
short int maxDepth; // controls the game difficulty
player player1,player2,ai,human,p1,p2; // Variables to store the player information
