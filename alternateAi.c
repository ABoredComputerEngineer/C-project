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
#define MAX_IDENTIFIERS 9
#define MAX_COMMANDS 6
#define IN 1
#define OUT 0


#define NEWLINE printf("--------------------------------------------------------------------\n")
#define NEWSCREEN printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n")
typedef struct game {
	char mat[BOARD_ROW][BOARD_COL];// The game board stored in matrix form
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

enum setting {name,ai_name,sign,mode}; // required for error handling

void displayChangeSettings(enum setting, char *, char *);
void displaySettings( void ); // Displays the game settings
void displayFrontPage( void );
void displayScoreCurrent( void );
void displayScoreLifeTime( void );
void displayCredits(void);

/* Declaration of the Global Variables. Almost required in all functions */
game gameBoard; // the game stored in 1D array
short int maxDepth; // controls the game difficulty
player player1,player2,ai,human,p1,p2; // Variables to store the player information


/* FROM SETTINGS.H */

typedef struct settings {
	player p1; // information about player 1
	player p2; // information about player 2
	player ai; // information about computer
	int difficulty; // difficulty of the game
} settings;

typedef struct command {
	char name[6]; // Name of the command
	struct identifier *identifierList[5]; // List of the sub commands of the main commands
} command;


typedef struct identifier {
	char name[15]; // Name of the identifier
	struct option *optionList[4];  // List of the options of the identifier
} identifier;


typedef struct option{
	char name[50]; // Name of the option 
	char identifierType[15];	// Name of the identifier which the option is part of
	char value[100]; // Value of the option
} option;

typedef struct wl {
	short int win;
	short int lose;
} score;

typedef struct gs{
	score p1; // W/L of player 1
	score p2; // W/L of player 2
	score ai; // W/L of computer
	int gcSingle; // Game count of single player game
	int gcDouble; // Game count of double player game
} stat;


enum difficulty {EASY=1,MEDIUM,HARD};
enum tokens {COMMAND,IDENTIFIER,OPTION,VALUE};
enum commands {set,new,view};


// Matches a command and returns the index of the command in the command list

command *matchCommand(char [][100]);

/* The parse Command reads the command typed by the user and perfoms the necessary action */
void parseCommand(char [][100], int);

/*Reads the commands if the user types "set" as the initial command */
option *parseSet(command *,char [][100], int);

/*Reads the commands if the user types "new" as the initial command */
void parseNew(command *,char [][100], int);
/*Reads the commands if the user types "view" as the initial command */
void parseView(command *,char [][100], int);


void setOptions(option *); // Changes the game settings according to the command of the user
void applyDefault( void ); // Applies the default settings
void buildCommandTree(void); // Build the command tree for command line
void reset(void);// Resets the game settings to default and all scores to 0
settings gameSet;
stat gameStat;
stat currentStat;
/* FROM AI.H */

typedef struct aiBoard {
	int score; // The 'score' of the board as seen by ai or the human
	int move; // The move which results in the score above
} aiBoard;


aiBoard *newBoard( void ); // dynamically create a new array aiBoard and fill with GARBAGE value 
void setDepth(void); // Sets the difficulty of the game
aiBoard getBestMove( player , int  ); // Returns the best possible of the human or the computer
int aiMove( void ); // Return the cell in which the ai should move for best possible outcome
aiBoard getRandomMove( aiBoard *board, player current ); // Returns a random move from a list of best possible moves
int evaluateBoard(void); // Evaluates the score of the current state of the game


/* END OF FUNCTION DECLARATIONS */

/* FUNCTIONS IN SETTINGS.C */

command *commandList;
identifier *identifierList; 
option *optionList;

void buildCommandTree(){
	int i = 0,j=0;
	commandList = malloc(sizeof(command)*MAX_COMMANDS);
	identifierList = malloc(sizeof(identifier)*MAX_IDENTIFIERS);
	optionList = malloc(sizeof(option)*MAX_OPTIONS);
	for ( i = 0; i<MAX_COMMANDS; i++ ){
		for ( j = 0; j<5; j++ )
			commandList[i].identifierList[j] = NULL;
	}
	for ( i = 0; i<MAX_IDENTIFIERS; i++ ){	
		for ( j = 0; j<4; j++ )
			identifierList[i].optionList[j] = NULL;
	}
	strcpy(commandList[0].name,"set");
	strcpy(commandList[1].name , "new");
	strcpy(commandList[2].name ,"view");
	strcpy(commandList[3].name , "exit");
	strcpy(commandList[4].name , "clear");
	strcpy(commandList[5].name , "reset");

	commandList[0].identifierList[0] = &identifierList[0]; // identiferList[0] is player 1
	commandList[0].identifierList[1] = &identifierList[1]; // identifierList[1] is player 2
	commandList[0].identifierList[2] = &identifierList[2]; // identifierList[2] is ai


	strcpy(identifierList[0].name ,"p1");
	identifierList[0].optionList[0] = &optionList[0]; // name option
	identifierList[0].optionList[1] = &optionList[1]; // Symbol option

	strcpy(identifierList[1].name ,"p2");
	identifierList[1].optionList[0] = &optionList[0]; // name option
	identifierList[1].optionList[1] = &optionList[1]; // Symbol option

	strcpy(identifierList[2].name , "ai");
	identifierList[2].optionList[0] = &optionList[0]; // name option
	identifierList[2].optionList[1] = &optionList[1] ;// Symbol option
	identifierList[2].optionList[2] = &optionList[2] ;// difficulty option for ai only


	commandList[1].identifierList[0] = &identifierList[3]; //game mode :: Single
	commandList[1].identifierList[1] = &identifierList[4]; //gameMode :: Multi
	strcpy(identifierList[3].name ,"1p");
	strcpy(identifierList[4].name , "2p");


	commandList[2].identifierList[0] = &identifierList[5]; // Settings
	commandList[2].identifierList[1] = &identifierList[6]; //Score
	commandList[2].identifierList[2] = &identifierList[7];
	commandList[2].identifierList[3] = &identifierList[8];
	strcpy(identifierList[5].name , "settings");
	strcpy(identifierList[6].name , "score");
	strcpy(identifierList[7].name , "credits");
	strcpy(identifierList[8].name , "help");


	strcpy(optionList[0].name , "name" );
	strcpy(optionList[1].name , "sign");
	strcpy(optionList[2].name , "mode");
	
	return;

}




command *matchCommand( char tokenList[][100] ){
	int i =0;
	for ( i = 0; i<MAX_COMMANDS ; i++ ){
		if ( strcmp(tokenList[COMMAND],commandList[i].name) == 0 )
			return &commandList[i];
	}
	return NULL;
}


void parseCommand(char tokenList[][100], int tokens){ // tokens is the number of tokens 
	command *current = NULL;
	option *opt = NULL;
	current = matchCommand(tokenList);

	if ( current == NULL ){
		displayError(err_command,"n");
		return;
	} else if ( strcmp(current->name,"set") == 0 ){  // If the command is to change the settings
		opt  = parseSet(current,tokenList,tokens);
		if ( opt != NULL ){
			setOptions(opt);
		}
	} else if ( strcmp(current->name,"new") == 0){ // If the command is to start a new game
		parseNew(current,tokenList,tokens);
	} else if ( strcmp( current->name , "view") == 0 ){ // if the command is to view scores or settings
		parseView(current,tokenList,tokens);

	} else if ( strcmp(current->name,"exit") == 0){
		free(commandList);
		free(optionList);
		free(identifierList);
		printf("\nThe stats for this session are::: \n");
		displayScoreCurrent();
		 printf("\nThanks for playing!!!!!\n\n\n");
		exit(1);
	} else if ( strcmp(current->name,"clear") == 0 ){
		NEWSCREEN;
	} else if ( strcmp(current->name, "reset") == 0 ){
		printf("\nEverything has been reset to default\n");
		reset();
	}
	else {
		displayError(err_command,"n");
	}
	return;

}



void setOptions(option *new ){

	if ( strcmp(new->name,"name") == 0 ){
		if ( strcmp(new->identifierType,"p1") == 0 ){
			displayChangeSettings(name,gameSet.p1.name,new->value);
			strcpy(gameSet.p1.name,new->value);
		} else if ( strcmp(new->identifierType,"p2") == 0) {
			displayChangeSettings(name,gameSet.p2.name,new->value);
			strcpy(gameSet.p2.name,new->value);
		} else {
			displayChangeSettings(ai_name,gameSet.ai.name,new->value);
			strcpy(gameSet.ai.name,new->value);
		}
	} else if ( strcmp(new->name, "sign") == 0 ){
		if ( strcmp(new->identifierType,"p1") == 0 ){
			displayChangeSettings(sign,&gameSet.p1.sign,(new->value));
			gameSet.p1.sign = new->value[0];
		} else if ( strcmp(new->identifierType,"p2") == 0 ){
			displayChangeSettings(sign,&gameSet.p2.sign,(new->value));
			gameSet.p2.sign = new->value[0];
		} else {
			displayChangeSettings(sign,&gameSet.ai.sign,(new->value));
			gameSet.ai.sign = new->value[0];
		}
	} else if ( strcmp(new->name, "mode") == 0 ){
		if ( strcmp(new->value,"easy") == 0 ){
			displayChangeSettings( mode, (gameSet.difficulty == EASY)?"easy":((gameSet.difficulty == MEDIUM)?"medium":"hard") , new->value);
			gameSet.difficulty = EASY;
		} else if ( strcmp(new->value , "medium") == 0 ){
			displayChangeSettings( mode, (gameSet.difficulty == EASY)?"easy":((gameSet.difficulty == MEDIUM)?"medium":"hard") , new->value);
			gameSet.difficulty = MEDIUM;
		} else if ( strcmp(new->value,"hard") == 0 ){
			displayChangeSettings( mode, (gameSet.difficulty == EASY)?"easy":((gameSet.difficulty == MEDIUM)?"medium":"hard") , new->value);
			gameSet.difficulty = HARD;
		} else {
			return;
		}
	}
	writeSettings();
}




void applyDefault( void ){
	strcpy(gameSet.p1.name,"Player 1");
	gameSet.p1.sign = '0';
	gameSet.p1.position = PLAYER1;
	gameSet.p1.type = HUMAN;
	strcpy( gameSet.p2.name,"Player 2");
	gameSet.p2.sign = 'X';
	gameSet.p2.position = PLAYER2;
	gameSet.p2.type = HUMAN;
	strcpy(gameSet.ai.name,"Marco Bot");
	gameSet.ai.sign = '+';
	gameSet.ai.type = AI;
	gameSet.ai.position = PLAYER2;
	gameSet.difficulty = EASY;
}


option *parseSet( command *current, char tokenList[][100] , int tokens ){
	int i;
	identifier *identify = NULL;
	option *opt = NULL;
		for ( i = 0; i<3 && current->identifierList[i] != NULL ; i++ ){
			if ( strcmp(current->identifierList[i]->name, tokenList[IDENTIFIER]) == 0 ){
				identify = current->identifierList[i];
				break;
			}
		} 
		if (identify == NULL || tokens < 4 ) {
			displayError(err_identifier,current->name);
			return NULL;
		}

		for ( i = 0; i<3 && identify->optionList[i] != NULL ; i++ ){
				if ( strcmp(identify->optionList[i]->name,tokenList[OPTION]) == 0 ){
					opt = identify->optionList[i];
					removeQuotes(tokenList[VALUE]);
					strcpy(opt->identifierType,identify->name);

					strcpy(opt->value,tokenList[VALUE]);
					return opt;
				}
		}
		
	return NULL;

}

void parseNew(command *current, char tokenList[][100], int tokens ){	
		if ( tokens > 2 ){
			displayError(err_identifier,current->name);
			return;
		}
		if ( tokens == 1 ){
			playGame(SINGLE);
		} else if ( strcmp(current->identifierList[0]->name, tokenList[IDENTIFIER]) == 0 ){
			playGame(SINGLE);
		} else if ( strcmp(current->identifierList[1]->name, tokenList[IDENTIFIER]) == 0 ) {
			playGame(DOUBLE);
		} else {
			displayError(err_identifier,current->name);
			return;
		}

}

void parseView( command *current, char tokenList[][100], int tokens ){
	identifier *identify = NULL;
	int i;
		if  (tokens != 2  ){
			displayError(err_identifier,current->name);
			return;
		}
		for ( i = 0; i<MAX_IDENTIFIERS && current->identifierList[i] != NULL ; i++ ){
			if ( strcmp(current->identifierList[i]->name, tokenList[IDENTIFIER]) == 0 ){
				identify = current->identifierList[i];
				break;
			}
		} 
		if ( identify == NULL ){
			displayError(err_identifier,current->name);
			return;
		}
		if ( strcmp(identify->name, "score") == 0 ){
			displayScore();
		} else if ( strcmp(identify->name, "settings") == 0){
			displaySettings();
		} else if ( strcmp(identify->name, "help") == 0 ){
			displayHelp();
		} else if ( strcmp(identify->name, "credits") == 0 ){
			displayCredits();
		} else {
			displayError(err_identifier,current->name);
			return;
		}

}

void reset( void ){
	applyDefault();
	memset((void *)&gameStat,0,sizeof(gameStat));
	memset((void *)&currentStat,0,sizeof(currentStat));
	writeSettings();
}


/* FUNCTIONS IN CORE.C */

/* Initializes the board to the initial state by setting the entire board to empty*/
void initializeBoard(void){
	int i;
	for ( i = 0; i<BOARD; i++ )
		gameBoard.row[i] = ' ';
}


/* Checks if the game has ended or not */

int hasEnded( void ){
	int i;
	/*Checking if the game has ended in any of the row*/	
	for ( i = 0; i<7 ; i+= 3 ){
		/*If one of cell of the row is empty , the game has not ended in that row*/
		if (  gameBoard.row[i+1] == ' ')
			continue;
		/* If all the values in the row are equal , The game has ended */
		if ( gameBoard.row[i] == gameBoard.row[i+1] && gameBoard.row[i+1] == gameBoard.row[i+2] ){
			return (gameBoard.row[i+1] == player1.sign)?PLAYER1:PLAYER2;
		}
	}
	/*Checking if the game has ended in any of the column*/
	for ( i = 0; i<3 ; i++ ){
		if ( gameBoard.row[i+3] == ' ')
			continue;
		if ( gameBoard.row[i] == gameBoard.row[i+3] && gameBoard.row[i+3] == gameBoard.row[i+6] )
			return (gameBoard.row[i+3] == player1.sign)?PLAYER1:PLAYER2;
	}
	/*Checking if the game has ended in any of the diagonals*/
	if ( gameBoard.row[4] != ' '){ // If the center square is empty then game has not ended in any of diagonals

		if ( gameBoard.row[0] == gameBoard.row[4] && gameBoard.row[4] == gameBoard.row[8] )
			return (gameBoard.row[4] == player1.sign)?PLAYER1:PLAYER2;
		if ( gameBoard.row[2] == gameBoard.row[4] && gameBoard.row[4] == gameBoard.row[6] )
			return (gameBoard.row[4] == player1.sign)?PLAYER1:PLAYER2;
	}

	/* If the game has not ended in  any of the rows columns or diagonals and there is still a empty box, It has not ended*/
	for ( i =0 ; i<9; i++ ){
		if ( gameBoard.row[i] == ' ')
			return FALSE;
	}

	//If there is no empty cell
	return TIE;
}



/* "Performs" the move of the player 1 or player2 by changing the values in the game board 
	x is the cell in which the symbol is to be placed 
	player is the player whose symbol is to be places ( Player 1 or Player 2)*/
void performMove( int x , int player ){

	gameBoard.row[x] = ( player == PLAYER1 )?player1.sign:player2.sign;

}


/* Checks if the move the user is trying to do is legal or not */
int isLegal( int x  ){
	/* If the character is not valid or the cell the user is trying to fill is not Empty*/
	if ( x == -1 || gameBoard.row[x] != ' ')
		return FALSE;
	else 
		return TRUE; 
}

void playGame(int mode){
	int winner;
	initializeBoard();
	printBoard();
	if ( mode == SINGLE ){

		player1 = (gameStat.gcSingle%2)?gameSet.ai:gameSet.p1;
		player2 = (gameStat.gcSingle%2)?gameSet.p1:gameSet.ai;
		player1.position = PLAYER1;
		player2.position = PLAYER2;
		winner = playSingle();
		if ( winner == PLAYER1 ){
			if ( player1.type == AI ){
				gameStat.ai.win++;
				currentStat.ai.win++;
				gameStat.p1.lose++;
				currentStat.p1.lose++;
			}
			else{
				currentStat.p1.win++;
				gameStat.p1.win++;
				currentStat.ai.lose++;
				gameStat.ai.lose++;
			} 
		} else if ( winner == PLAYER2 ){
			if ( player2.type == AI ){
				currentStat.ai.win++;
				gameStat.ai.win++;
				currentStat.p1.lose++;
				gameStat.p1.lose++;
			}
			else{
				currentStat.ai.lose++;
				gameStat.ai.lose++;
				currentStat.p1.win++;
				gameStat.p1.win++;
			}
		}
	} else {
		player1 = (gameStat.gcDouble%2)?gameSet.p2:gameSet.p1;
		player2 = (gameStat.gcDouble%2)?gameSet.p1:gameSet.p2;
		player1.position = PLAYER1;
		player2.position = PLAYER2;
		winner = playDouble();
		if ( winner == PLAYER1 ){
			currentStat.p1.win++;
			gameStat.p1.win++;
			currentStat.p2.lose++;
			gameStat.p2.lose++;
		}
		else {
			currentStat.p2.win++;
			gameStat.p2.win++;
			currentStat.p1.lose++;
			gameStat.p1.lose++;
		}
	}
	if ( winner == PLAYER1 ){
		printf("%s wins!!!\n",player1.name);
	} else if (winner == PLAYER2) {
		printf("%s wins!!!\n",player2.name);
	} else {
		printf("The game has been drawn\n");
	}
	writeSettings();
}

int playSingle( void ){
	player current;
	current = player1;
	ai = (player1.type == AI )?player1:player2;
	human = (player1.type == HUMAN )?player1:player2;
	int cell,rv;
	setDepth();
	while ( !(rv=hasEnded()) ){
		if ( current.type == AI )
			cell = aiMove();
		else {
			printf("%s's turn: ", current.name);
			cell = getMove();
		}
		performMove(cell, current.position);
		current = ( current.position == PLAYER1 )?player2:player1;		
		printBoard();
	}
	gameStat.gcSingle++;
	currentStat.gcSingle++;
	return rv;
}

int playDouble( void ){
	player current;

	int cell,rv;

	current = player1;

	while ( !(rv=hasEnded()) ){
		printf("%s's turn: ",current.name);
		cell = getMove();
		performMove(cell,current.position);
		current = ( current.position == PLAYER1 )?player2:player1;
		printBoard();
	}

	gameStat.gcDouble++;
	currentStat.gcDouble++;
	return rv;
}

/* FUNCTIONS IN AI.C */

aiBoard *newBoard( void ){
	int i;
	aiBoard *p = malloc( sizeof(aiBoard) * 9 );
	for ( i = 0; i< 9 ; i++ ){
		p[i].score = p[i].move = GARBAGE;
	}
	return p;
}

void setDepth(){

	if ( gameSet.difficulty == EASY || (gameSet.difficulty == MEDIUM  && (int)rand()%2)){
		maxDepth = 2;
	
	} else {
		if ( ai.position == PLAYER2 )
			maxDepth = 4;
		else 
			maxDepth = 5;
	}
}


int aiMove( void ){
	return getBestMove(ai , 0).move;
}


aiBoard getBestMove( player current , int depth ){
	int i,score,rv,x = 0;
	aiBoard *new = newBoard() ;
	aiBoard move;
	rv = hasEnded();
	if ( rv != 0 || depth == maxDepth){  // game has  Ended
		aiBoard x;
		x.move = GARBAGE;
		x.score = evaluateBoard();
		free( new );
		return x;
	}
	for ( i = 0; i<9 ; i++ ){
		if ( gameBoard.row[i] == ' ' ){
			performMove( i , current.position );
				// printBoard();
			if ( current.type == AI )
				score = getBestMove( human, depth + 1 ).score;
			 else
				score = getBestMove( ai , depth + 1 ).score;
			gameBoard.row[i] = ' ';
			new[x].score = score;
			new[x++].move = i;
		}
	}

	// for ( i = 0; new[i].score!=GARBAGE &&i<9; i++ )
	// 	printf("Score: %d\tMove:%d\t%s\n",new[i].score,new[i].move,(current.type==AI)?"AI":"Human");
	// putchar('\n');

	move = getRandomMove( new, current );
	free(new);
	return move;

}



aiBoard getRandomMove( aiBoard board[], player current){
	int bestScore;
	aiBoard bestMoveList[9];
	int i,k=0;

	if ( current.type == AI ){
		bestScore = -1000;
		for ( i = 0; i<9 && board[i].score != GARBAGE; i++ ){
			if ( board[i].score > bestScore )
				bestScore = board[i].score;
		}
	} else {
		bestScore = 1000;
		for ( i = 0; i<9 && board[i].score!=GARBAGE; i++ ){
			if ( board[i].score < bestScore )
				bestScore = board[i].score;
		}
	}

	for ( i = 0; i<9 && board[i].score != GARBAGE; i++ ){
		if ( bestScore == board[i].score )
			bestMoveList[k++] = board[i];
	}

	return bestMoveList[rand() % k];

}

int evaluateBoard( ){
	short int i,j,wins = 0,values[8],sumRow=0,k=0;

	for ( j = 0; j<7; j+= 3 ){
		for ( i = 0; i<3; i++){
			if ( gameBoard.row[i+j] == human.sign )
				sumRow--;
			else if ( gameBoard.row[i+j] == ai.sign )
				sumRow++;
		}
		values[k++] = sumRow;
		sumRow = 0;
	}
	for ( j = 0; j<3; j++ ){
		for ( i = j; i<9; i+=3 ){
			if ( gameBoard.row[i] == ai.sign )
				sumRow++;
			else if ( gameBoard.row[i] == human.sign)
				sumRow--;
		}
		values[k++] = sumRow;
		sumRow = 0;
	}
	for ( i = 0; i<9 ; i+= 4){
		if ( gameBoard.row[i] == ai.sign )
			sumRow++;
		else if (gameBoard.row[i] == human.sign )
			sumRow--;
	}
	values[k++] = sumRow;
	sumRow = 0;
	for ( i = 6; i>0; i-=2 ){
		if ( gameBoard.row[i] == ai.sign)
			sumRow++;
		else if (gameBoard.row[i] == human.sign )
			sumRow--;
	}

	values[k++] = sumRow;
	sumRow = 0;

	for ( i = 0; i<k; i++ ){
		if ( values[i] == 3 ){
			wins+=10;
			break;
		} else if ( values[i] == 2 )
			wins+=1;
		else if ( values[i] == -2 )
			wins-=1;
		else if ( values[i] == -3 ){
			wins-=10;
			break;
		}
	}
	return wins;

}

/* FUNCTIONS IN IO.C */
/* prints the board according to the row type of the game board*/

void printBoard( void ){
	NEWSCREEN;
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.row[6],gameBoard.row[7],gameBoard.row[8]);
	printf("\t\t         |        |         \n");
	printf("\t\t---------|--------|---------\n");
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.row[3],gameBoard.row[4],gameBoard.row[5]);
	printf("\t\t         |        |         \n");
	printf("\t\t---------|--------|---------\n"); 	
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.row[0],gameBoard.row[1],gameBoard.row[2]);
	printf("\t\t         |        |         \n"); 
	putchar('\n');
}

/* Gets the move the user wants to perform in the form of a string and returns the value of the cell
	in the board */

int getMove( void ){
	char in[100];
	while ( TRUE ){
		getString(in);
		if ( strlen(in) == 1 ){
			if ( isLegal(ctoi(in[0])) )
				return ctoi(in[0]);
		}
	printf("\nInvalid Move\n");
	putchar('>');
	}
	
}

/* ctoi converts the character to an integer if it is between 1  and 9*/
int ctoi( char c ){
	if ( c > '0' && c<='9' ) 
		return c - '0' - 1 ;
	else
		return -1;
}

void removeQuotes( char str[] ){
	char temp[100];
	int i,j=0;
	for ( i = 0; str[i] != '\0' ; i++ ){
		if ( str[i] != '\"')
			temp[j++] = str[i];
	}
	temp[j] = '\0';	
	strcpy(str,temp);
} 

void getString(char x[]) {
	char c;
	int i = 0;
	while ( (c=getchar()) != '\n' )
		x[i++] = c;
	x[i] = '\0';
}

void displaySettings( void ){
	displayHeading("Settings");
	printf("| Player 1 | \n");
	NEWLINE;
	printf("%-12s:::\t%-10s\n","Name",gameSet.p1.name);
	printf("%-12s:::\t%-4c\n","Sign",gameSet.p1.sign);
		NEWLINE;
	printf("| Player 2 | \n");
	NEWLINE;
	printf("%-12s:::\t%-10s\n","Name",gameSet.p2.name);
	printf("%-12s:::\t%-4c\n","Sign",gameSet.p2.sign);
		NEWLINE;
	printf("| Computer |\n");
	NEWLINE;
	printf("%-12s:::\t%-10s\n","Name",gameSet.ai.name);
	printf("%-12s:::\t%-4c\n","Sign",gameSet.ai.sign);
	printf("%-12s:::\t%-6s\n","Difficulty",(gameSet.difficulty == EASY )?"Easy":((gameSet.difficulty == MEDIUM)?"Medium":"Hard"));
	NEWLINE;
	printf("\n\n");
}


int stringSplit( char in[], char out[][100]){
	int i,quote = OUT,k = 0,state = OUT,wordCount = 0;

	for ( i = 0; in[i] != '\0'; i++ ){
		if ( in[i] == ' ' && quote == OUT){
			out[wordCount - 1][k] = '\0';

			state = OUT;
		} else if (state == OUT) {
			wordCount++;
			k = 0;
			state = IN;

		}

		if ( in[i] == '\"')
			quote = !quote;

		if ( state == IN ){
			out[wordCount-1][k++] = in[i];
		}

	}
	out[wordCount-1][k] = '\0';
	return wordCount ;
}

void displayScore( void ){
	displayHeading("ScoreCard");
	printf("| Current Stats | \n");
	displayScoreCurrent();
	printf("| Lifetime Stats | \n");
	displayScoreLifeTime();
}

void displayHeading ( char *s ){
		NEWSCREEN;	
	NEWLINE;
	printf("%20s|  %s  |\n"," ",s);
		NEWLINE;
}


void displayError(enum errors err_type , char *s){

	if ( err_type == err_command ){
		printf("\n\nInvalid command !! Type \"view help\" to view complete list of commands.\nType \"new\" to start a single player game or \"new 2p\" to start a two player game.\n\n" );
	} else if ( err_type == err_identifier ){
		if ( strcmp(s,"new") == 0 ){
			printf("\nInvalid command!! Use new command as:: \n");
			displayNewHelp();
		} else if ( strcmp(s,"view") == 0 ){
			printf("\nInvalid command!! Use 'view' command as:: \n");
			displayViewHelp();
		} else if ( strcmp(s,"set") == 0 ){
			printf("\nInvalid command!! Use 'set' command as:: \n");
			displaySetHelp();
		}
	} else if ( err_type == err_move ){
		printf("\nInvalid Move!!!\n");
	}
}


void displaySetHelp(void){
	printf("set (player_type) (options) (value) \n\n");
	printf("(player_type) is one of the following... \n");
	printf("p1 : change options for player 1\n");
	printf("p2 : change options for player 2\n");
	printf("ai : change options for the computer \n\n");
	printf("(options) is one of the following.... \n");
	printf("%-5s : change the name. Give a string in the (value). \n%8cUse double quotes if you want to use space \n","name",' ');
	printf("%-5s : change the sign used. The (value) \n%8cis a single character. \n", "sign",' ');
	printf("%-5s : change the difficulty. Only used with ai.\n%8cThe (value) is \'easy\' , \'medium\' or \'hard\'.\n","mode",' ');
	NEWLINE;
}

void displayViewHelp(void){
	printf("view (options) \n");
	printf("The valid values for (options) are : \n");
	printf("%-9s : Display the complete list of commands\n", "help");
	printf("%-9s : Display the current game settings\n", "settings");
	printf("%-9s : Display the total game score\n", "score");
	printf("%-9s : View the game credits\n", "credits");
	NEWLINE;
}

void displayNewHelp(void){
	printf("new (game_mode) \n");
	printf("(game_mode) is one of the following... \n");
	printf("1p : Starts a single player\n");
	printf("2p : Starts a two player game\n");
	printf("A blank will automatically start a single player game\n");
	NEWLINE;
}

void displayHelp(void){
	displayHeading("Help");
	printf("Use the following commands to do perform the operations\n");
	NEWLINE;
	printf("| 'new' Command | Start a new game.\n");
	NEWLINE;
	displayNewHelp();
	NEWLINE;
	printf("| 'view' Command | View the game settings,score, help etc.\n");
	NEWLINE;
	displayViewHelp();
	NEWLINE;
	printf("| 'set' Command | Change the game settings\n");
	NEWLINE;
	displaySetHelp();
	NEWLINE;
	printf("| 'exit' Command | Exit the game\n");
	NEWLINE;
	printf("| 'reset' Command | \n");
	NEWLINE;
	printf("Resets the game settings to default and all scores to 0\n");
	NEWLINE;
}

void displayChangeSettings(enum setting set, char *prev, char *current){
	if ( set == name ){
		printf("\nName changed from \"%s\" to \"%s\".\n",prev,current);
	} else if ( set == sign ){
		printf("\nName changed from \"%c\" to \"%c\".\n\n",prev[0],current[0]);
	} else if ( set == mode ){
		if ( (strcmp(prev,"easy") == 0) && (strcmp(current,"easy") != 0 ) ){
			printf("\nDifficulty has been decreased.\n");
		} else {
			printf("\nDifficulty has increased. \n");
		}
	} else if ( set == ai_name ){
		printf("\nComputer name changed form \"%s\" to \"%s\".\n",prev,current);
	}
}

void displayFrontPage( void ){
	int i;

	for ( i = 0; i<80; i++ )
		putchar('*');
	printf("\n");
	printf("%-79c%c\n",'*','*');
	printf("*%25c%s%26c\n",' ',"A simple game of Tic-Tac-Toe",'*');
	printf("%-79c%c\n",'*','*');
	
	for ( i = 0; i<80; i++ )
		putchar('*');
	printf("\n");
	printf("%-79c%c\n",'*','*');
	
	printf("*%19c%s%20c\n",' ',"Type 'new' to start a single player game",'*');
	printf("*%19c%s%20c\n",' ',"Type 'new 2p' to start a two player game",'*');
	
	printf("*%14c%s%15c\n",' ',"Type 'view help' to view complete list of commands",'*');
	printf("*%16c%s%16c\n",' ',"Or 'view settings' to view the current settings",'*');
	printf("*%31c%s%30c\n",' ',"Enjoy the game!!!!",'*');
	printf("%-79c%c\n",'*','*');
	printf("%-79c%c\n",'*','*');
	for ( i = 0; i<80; i++ )
		putchar('*');
	printf("\n");
	
}

void displayScoreCurrent( void ){
	NEWLINE;
	printf("%-20s\tWin: %-4d\tLoss: %-4d\n","Player 1",currentStat.p1.win,currentStat.p1.lose);
	NEWLINE;
	printf("%-20s\tWin: %-4d\tLoss: %-4d\n","Player 2",currentStat.p2.win,currentStat.p2.lose);
	NEWLINE;
	printf("%-20s\tWin: %-4d\tLoss: %-4d\n","Computer",currentStat.ai.win,currentStat.ai.lose);
	NEWLINE;
	printf("%-20s\t%s: %-4d\t%s: %-4d\n","Games Played","Single",currentStat.gcSingle,"Two Player",currentStat.gcDouble);
	NEWLINE;
}

void displayScoreLifeTime( void ){
	NEWLINE;
	printf("%-20s\tWin: %-4d\tLoss: %-4d\n","Player 1",gameStat.p1.win,gameStat.p1.lose);
	NEWLINE;
	printf("%-20s\tWin: %-4d\tLoss: %-4d\n","Player 2",gameStat.p2.win,gameStat.p2.lose);
	NEWLINE;
	printf("%-20s\tWin: %-4d\tLoss: %-4d\n","Computer",gameStat.ai.win,gameStat.ai.lose);
	NEWLINE;
	printf("%-20s\t%s: %-4d\t%s: %-4d\n","Games Played","Single",gameStat.gcSingle,"Two Player",gameStat.gcDouble);
	NEWLINE;
	printf("\n\n");
}

void displayCredits(void){
	printf("\n\n");	
	NEWLINE;
	printf("\nThe game is developed by:: \n");
	printf("%-20s%-20s\n","Amrit Pun","074BEX404");
	printf("%-20s%-20s\n","Asim Maharjan","074BEX408");
	printf("%-20s%-20s\n","Pratik Budathoki","074BEX429");
	printf("%-20s%-20s\n","Shiva Ram Godar","074BEX442");
	NEWLINE;
	printf("\n\n");
}

/* FUNCTIONS IN FILE.C */
typedef struct {
	settings set;
	stat points;
} content;


void writeSettings(void){
	FILE *fp;
	content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen(PATH,"wb")) != NULL ){
		fwrite(&new,sizeof(content),1,fp);
		fclose(fp);
	}
}

void newSettings(void){
	FILE *fp;
	content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen(PATH,"wb")) == NULL ){
		printf("Error! Cant open game file");
		return;
	}
	fwrite(&new,sizeof(content),1,fp);
	fclose(fp);
}

void readSettings(void){
	FILE *fp;
	int x;
	content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen(PATH,"rb")) == NULL ){
		// applyDefault();
		newSettings(); 
	} else {
		x = (int)fread(&new,sizeof(content),1,fp);
		if ( x == 0){
			printf("Error Occurred\n");
					applyDefault();
			newSettings(); 
		}
		gameSet = new.set;
		gameStat = new.points;
		fclose(fp);
	}
}

/* FUNCTIONS IN MAIN.C */

int main(){
	int k;
	buildCommandTree();
	char command[100];
	char tokenList[10][100];
	applyDefault();
	readSettings();
	srand(time(NULL));
	displayFrontPage();
	while ( 1) {
		putchar('>');
		getString(command);
		k = stringSplit(command,tokenList);
		parseCommand(tokenList, k);
		/* Resets the token list i.e. sets all characters of the 2d array to null '\0' */
		memset(tokenList,'\0',sizeof(char)*10*100); 
	}
	return 0;
}
