#include "game.h"
#include "settings.h"
extern settings gameSet;
extern stat gameStat;

// #include "ai.h"
/* prints the board according to the row type of the game board*/
void displayHeading(char *);
void printBoard( ){
	// NEWSCREEN;
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
	// NEWSCREEN;	
	// NEWLINE;
	// printf("%20s|  %s  |\n"," ","Settings");
	// 	NEWLINE;
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
	// NEWLINE;
	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 1",gameStat.p1.win,gameStat.p1.lose);
	NEWLINE;
	// NEWLINE;
	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 2",gameStat.p2.win,gameStat.p2.lose);
	NEWLINE;
		// NEWLINE;
	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Computer",gameStat.ai.win,gameStat.ai.lose);
	// NEWLINE;
		NEWLINE;
	printf("\n\n");
}

void displayHeading ( char *s ){
		NEWSCREEN;	
	NEWLINE;
	printf("%20s|  %s  |\n"," ",s);
		NEWLINE;
}