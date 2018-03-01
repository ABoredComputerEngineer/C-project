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
void applyDefault(void);
// int aiMove(void);
void newSettings(void);
void writeSettings(void);
void readSettings(void);
void newSettings(void);

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

typedef struct aiBoard {
	int score;
	int move;
} aiBoard;


aiBoard *newBoard( void );
void setDepth(void);
aiBoard getBestMove( player , int  );
int aiMove( void );
aiBoard getRandomMove( aiBoard *board, player current );
int evaluateBoard(void);


/* Declaration of the Global Variables */
game gameBoard;
// int turn = 1;
short int maxDepth;
player player1,player2,ai,human,p1,p2;
settings gameSet;
stat gameStat;



// #include "game.h"
// #include "ai.h"
/* prints the board according to the row type of the game board*/
void printBoard( ){
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.row[6],gameBoard.row[7],gameBoard.row[8]);
	printf("\t\t         |        |         \n");
	printf("\t\t---------|--------|---------\n");
	//printf("\t\t         |        |         \n");
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.row[3],gameBoard.row[4],gameBoard.row[5]);
	printf("\t\t         |        |         \n");
	printf("\t\t---------|--------|---------\n"); 	
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.row[0],gameBoard.row[1],gameBoard.row[2]);
	printf("\t\t         |        |         \n"); 
	// printf("\t\t         |        |         \n");
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
		NEWLINE;
	printf("Player 1 | \n");
	NEWLINE;
	printf("%-12s:::\t%-10s\n","Name",gameSet.p1.name);
	printf("%-12s:::\t%-4c\n","Sign",gameSet.p1.sign);
		NEWLINE;
	printf("Player 2 | \n");
	NEWLINE;
	printf("%-12s:::\t%-10s\n","Name",gameSet.p2.name);
	printf("%-12s:::\t%-4c\n","Sign",gameSet.p2.sign);
		NEWLINE;
	printf("Computer |\n");
	NEWLINE;
	printf("%-12s:::\t%-10s\n","Name",gameSet.ai.name);
	printf("%-12s:::\t%-4c\n","Sign",gameSet.ai.sign);
	printf("%-12s:::\t%-6s\n","Difficulty",(gameSet.difficulty == EASY )?"Easy":((gameSet.difficulty == MEDIUM)?"Medium":"Hard"));
	
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
	NEWLINE;
	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 1",gameStat.p1.win,gameStat.p1.lose);
	NEWLINE;
	// NEWLINE;
	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 2",gameStat.p2.win,gameStat.p2.lose);
	NEWLINE;
		// NEWLINE;
	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Computer",gameStat.ai.win,gameStat.ai.lose);
	NEWLINE;
}



// #include "game.h"
// #include "ai.h"

aiBoard *newBoard( void ){
	int i;
	aiBoard *p = malloc( sizeof(aiBoard) * 9 );
	for ( i = 0; i< 9 ; i++ ){
		p[i].score = p[i].move = GARBAGE;
	}
	return p;
}




/* Declarations and definitions of functions */

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





// int main( ){
// 	// delay( 5 );
// 	// int i, cell;
// 	// player current;
// 	char decision;
	
// 	printf("\nFirst Move or Second Move? ( 1 for first and 2 for 2nd).....\n");
// 	scanf("%c",&decision);
// 	if ( decision == '1' ){
// 		player1.type = HUMAN;
	
// 		player1.sign = '0';

// 		player2.type = AI;
// 		player2.position = PLAYER2;
// 		player2.sign = 'X';

// 	} else {
// 		player1.type = AI;
// 	player1.position = PLAYER1;
// 	player1.sign = '0';

// 	player2.type = HUMAN;

// 	player2.sign = 'X';

// 	}
// 	srand(time(NULL));
// 	initializeBoard();
// 	playGame(SINGLE);
// 	return 0;
// }



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
			// depth++;
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


// #include "game.h"
// #include "ai.h"
/* Initializes the board to the initial state by setting the entire board to empty*/
void initializeBoard(void){
	int i,j;
	for ( i = 0; i<BOARD_ROW ; i++ )
		for ( j = 0; j<BOARD_COL; j++ )
			gameBoard.mat[i][j] = ' ';
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
			if ( player1.type == AI )
				gameStat.ai.win++;
			else 
				gameStat.ai.lose++;
		} else if ( winner == PLAYER2 ){
			if ( player2.type == AI )
				gameStat.ai.win++;
			else
				gameStat.ai.lose++;
		}
	} else {
		player1 = gameSet.p1;
		player2 = gameSet.p2;
		winner = playDouble();
		if ( winner == PLAYER1 ){
			gameStat.p1.win++;
			gameStat.p2.lose++;
		}
		else {
			gameStat.p2.win++;
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
		printf("%s\n",(current.type == AI)?"Ai turn":"Human turn");
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
	return rv;
}

int playDouble( void ){
	player current;

	int cell,rv;

	current = player1;

	while ( !(rv=hasEnded()) ){
		cell = getMove();
		performMove(cell,current.position);
		current = ( current.position == PLAYER1 )?player2:player1;
		printBoard();
	}

	gameStat.gcDouble++;
	return rv;
}


// #include "game.h"
// #include <assert.h>
// settings newSet;
typedef struct {
	settings set;
	stat points;
} content;


// void displayScore( void ){
// 	NEWLINE;
// 	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 1",gameStat.p1.win,gameStat.p1.lose);
// 	NEWLINE;
// 	// NEWLINE;
// 	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 2",gameStat.p2.win,gameStat.p2.lose);
// 	NEWLINE;
// 		// NEWLINE;
// 	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Computer",gameStat.ai.win,gameStat.ai.lose);
// 	NEWLINE;
// }

// void applyDefault( void ){
// 	strcpy(gameSet.p1.name,"Player 1");
// 	gameSet.p1.sign = '0';
// 	gameSet.p1.position = PLAYER1;
// 	gameSet.p1.type = HUMAN;
// 	strcpy( gameSet.p2.name,"Player 2");
// 	gameSet.p2.sign = 'X';
// 	gameSet.p2.position = PLAYER2;
// 	gameSet.p2.type = HUMAN;
// 	strcpy(gameSet.ai.name,"Marco Bot");
// 	gameSet.ai.sign = '+';
// 	gameSet.ai.type = AI;
// 	gameSet.ai.position = PLAYER2;
// 	gameSet.difficulty = EASY;
// }

void writeSettings(void){
	FILE *fp;
	content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen("./gameSet.dat","wb")) == NULL ){
		applyDefault();
		newSettings();
	} else {
		fwrite(&new,sizeof(content),1,fp);
		fclose(fp);
	}
}

// void displaySettings( settings *current ){
// 		NEWLINE;
// 	printf("Player 1 | \n");
// 	NEWLINE;
// 	printf("%-12s:::\t%-10s\n","Name",current->p1.name);
// 	printf("%-12s:::\t%-4c\n","Sign",current->p1.sign);
// 		NEWLINE;
// 	printf("Player 2 | \n");
// 	NEWLINE;
// 	printf("%-12s:::\t%-10s\n","Name",current->p2.name);
// 	printf("%-12s:::\t%-4c\n","Sign",current->p2.sign);
// 		NEWLINE;
// 	printf("Computer |\n");
// 	NEWLINE;
// 	printf("%-12s:::\t%-10s\n","Name",current->ai.name);
// 	printf("%-12s:::\t%-4c\n","Sign",current->ai.sign);
// 	printf("%-12s:::\t%-6s\n","Difficulty",(current->difficulty == EASY )?"Easy":((current->difficulty == MEDIUM)?"Medium":"Hard"));
	
// }

void newSettings(void){
	FILE *fp;
		content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen("./gameSet.dat","wb")) == NULL ){
		printf("Error! Cant open game file");
		return;
	}
	fwrite(&new,sizeof(content),1,fp);
	// fwrite(&gameStat,sizeof(gameStat),1,fp);
	fclose(fp);
}

void readSettings(void){
	FILE *fp;
	int x;
	content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen("./gameSet.dat","rb")) == NULL ){
		applyDefault();
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

// void main(){
// 	readSettings();
// 	displaySettings(&gameSet);
// 	// gameSet.ai.sign = 'Y';
// 	// gameSet.difficulty = HARD;
// 	// gameStat.p1.win = 100;
// 	// gameStat.ai.win = 100;
// 	writeSettings();
// 	displayScore();
// 	// readSettings();


// }

// #include "game.h"
// #include "ai.h"

// Splits a string removing whitespaces and stores them in an array of strings
int stringSplit(char [], char [][100] );
/* Removes double quotes from a string*/
void removeQuotes( char *);
void getString( char * );
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
/*Reads the commands if the user types "exit" as the initial command */
void parseExit(command *,char [][100], int);



void setOptions(option *); // Changes the game settings according to the command of the user
void applyDefault( void ); // Applies the default settings
void applySettings( void );
void displaySettings( void ); // Displays the game settings

command commandList[MAX_COMMANDS]; // The list of valid commands
identifier identifierList[MAX_IDENTIFIERS]; // The list of valid identifiers ( p1 ( player 1) , p2(player2) , ai  etc..)
option optionList[MAX_OPTIONS]; // The list of options like name, sign etc.

void buildCommandTree(){
	int i = 0,j=0;
	for ( i = 0; i<MAX_COMMANDS; i++ ){
		for ( j = 0; j<3; j++ )
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
	// printf("current command: %s\n",commandList[3].name);

	commandList[0].identifierList[0] = &identifierList[0]; // identiferList[0] is player 1
	commandList[0].identifierList[1] = &identifierList[1]; // identifierList[1] is player 2
	commandList[0].identifierList[2] = &identifierList[2]; // identifierList[2] is ai
	// commandList[0].exOption = &optionList[2]; // equivaluent to set difficulty


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

}


int main(){
	int k;
	buildCommandTree();
		strcpy(commandList[3].name , "exit");
	// printf("current command: %s\n",commandList[3].name);
	// gameSet.p1 = &player1;
	// gameSet.p2 = &player2;
	gameSet.difficulty = EASY;
	char command[100];
	char tokenList[10][100];
	readSettings();
	// applyDefault();
	// gameStat.gcSingle = 0;
	srand(time(NULL));
	while ( 1) {
		putchar('>');
		// scanf("%[^\n]",command);
		// gets(command);
		getString(command);
		// getchar();
		k = stringSplit(command,tokenList);
		// printf("tokens : %d\n",k);
		
		parseCommand(tokenList, k);
		// getchar();
		// printf("%d\n",gameSet.difficulty);
		// displaySettings();
	}
}


command *matchCommand( char tokenList[][100] ){
	int i =0;
	for ( i = 0; i<MAX_COMMANDS ; i++ ){
		// printf("%s\t%s\n",tokenList[COMMAND],commandList[i].name);
		if ( strcmp(tokenList[COMMAND],commandList[i].name) == 0 )
			return &commandList[i];
	}
	return NULL;
}


void parseCommand(char tokenList[][100], int tokens){ // tokens is the number of tokens 
	// int i;
	command *current = NULL;
	// identifier *identify = NULL;
	option *opt = NULL;
	current = matchCommand(tokenList);
	// printf("\n%s\n",tokenList[0]);

	if ( current == NULL ){
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
		printf("Exiting\n");
		exit(1);
	} else {
		printf("Invaid command\n");
	}
	return;

}



void setOptions(option *new ){

	if ( strcmp(new->name,"name") == 0 ){
		if ( strcmp(new->identifierType,"p1") == 0 ){
			strcpy(gameSet.p1.name,new->value);
		} else if ( strcmp(new->identifierType,"p2") == 0) {
			strcpy(gameSet.p2.name,new->value);
		} else {
			strcpy(gameSet.ai.name,new->value);
		}
	} else if ( strcmp(new->name, "sign") == 0 ){
		if ( strcmp(new->identifierType,"p1") == 0 ){
			gameSet.p1.sign = new->value[0];
		} else if ( strcmp(new->identifierType,"p2") == 0 ){
			gameSet.p2.sign = new->value[0];
		} else {
			gameSet.ai.sign = new->value[0];
		}
	} else if ( strcmp(new->name, "mode") == 0 ){
		if ( strcmp(new->value,"easy") == 0 ){
			gameSet.difficulty = EASY;
		} else if ( strcmp(new->value , "medium") == 0 ){
			gameSet.difficulty = MEDIUM;
		} else if ( strcmp(new->value,"hard") == 0 ){
			gameSet.difficulty = HARD;
		} else {
			printf("\nValue not recognized\n");
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
	printf("\n%s\n",current->name);
		if ( tokens != 4 ){
			printf("\nInvalid arguments\n");
			return NULL;
		}
		for ( i = 0; i<3 && current->identifierList[i] != NULL ; i++ ){
			if ( strcmp(current->identifierList[i]->name, tokenList[IDENTIFIER]) == 0 ){
				identify = current->identifierList[i];
				break;
			}
		} 
		if (identify == NULL ) {
			printf("identifier Not found\n");
			return NULL;
		}

		for ( i = 0; i<3 && identify->optionList[i] != NULL ; i++ ){
			// printf("\n%s \t %s \t %s \n",tokenList[OPTION],identify->optionList[i]->name,identify->name	);
				if ( strcmp(identify->optionList[i]->name,tokenList[OPTION]) == 0 ){
					opt = identify->optionList[i];
					removeQuotes(tokenList[VALUE]);
					strcpy(opt->identifierType,identify->name);

					strcpy(opt->value,tokenList[VALUE]);
					printf("Writing to options \t %s\n",opt->value);
					// setOptions(opt);
					return opt;
				}
		}
		printf("Invalid options \n");
		return NULL;

}

void parseNew(command *current, char tokenList[][100], int tokens ){
	printf("\n%s\n",current->name);	
		if ( tokens > 2 ){
			printf("Invalid number of arguments\n");
			return;
		}
		if ( tokens == 1 ){
			playGame(SINGLE);
			// printf("\nStarting a new game \n");
		} else if ( strcmp(current->identifierList[0]->name, tokenList[IDENTIFIER]) == 0 ){
			playGame(SINGLE);
			// printf("\nStarting a single player game\n");
		} else if ( strcmp(current->identifierList[1]->name, tokenList[IDENTIFIER]) == 0 ) {
			playGame(DOUBLE);
			printf("\nStarting two player game\n");
		} else {
			printf("Command  not recognized\n");
			return;
		}

}

void parseView( command *current, char tokenList[][100], int tokens ){
	identifier *identify = NULL;
	int i;
	printf("\n%s\n",current->name);
		if  (tokens != 2  ){
			printf("Invalid number of arguments\n");
			return;
		}
		for ( i = 0; i<MAX_IDENTIFIERS && current->identifierList[i] != NULL ; i++ ){
			if ( strcmp(current->identifierList[i]->name, tokenList[IDENTIFIER]) == 0 ){
				identify = current->identifierList[i];
				break;
			}
		} 
		if ( identify == NULL ){
			printf("\nInvlaid Command\n");
			return;
		}
		if ( strcmp(identify->name, "score") == 0 ){
			printf("\nDisplaying Score\n");
			displayScore();
		} else if ( strcmp(identify->name, "settings") == 0){
			// printf("\nDisplaying Settings\n");
			displaySettings();
		} else if ( strcmp(identify->name, "help") == 0 ){
			printf("Printing Help\n");
		} else if ( strcmp(identify->name, "credits") == 0 ){
			printf("Printing Credits\n");
		} else {
			printf("Command not recognized\n");
			return;
		}

}


