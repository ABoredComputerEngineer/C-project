#include "game.h"
#include "settings.h"
extern settings gameSet;
extern stat gameStat;

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
	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 1",gameStat.p1.win,gameStat.p1.lose);
	NEWLINE;
	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 2",gameStat.p2.win,gameStat.p2.lose);
	NEWLINE;
	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Computer",gameStat.ai.win,gameStat.ai.lose);
	NEWLINE;
	printf("\n\n");
}

void displayHeading ( char *s ){
		NEWSCREEN;	
	NEWLINE;
	printf("%20s|  %s  |\n"," ",s);
		NEWLINE;
}


void displayError(enum errors err_type , char *s){

	if ( err_type == err_command ){
		printf("\n\nInvalid command !! Type \"view help\" to view complete list of commands. Type \"new\" to start a single player game.\n\n " );
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
	printf("ai : change options for the computer \n\name");
	printf("(options) is one of the following.... \n");
	printf("%-5s : change the name. Give a string in the (value). Use double quotes if you want to use space \n","name");
	printf("%-5s : change the sign used. The (value) is a single character. \n", "sign");
	printf("%-5s : change the difficulty. Only used with ai. The (value) is \'easy\' , \'medium\' or \'hard\'.\n","mode");
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

void displayFrontPage( ){
	int i;
	for ( i = 0; i<40; i++ )
		putchar('*');
	putchar('\n');
}