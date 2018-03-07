#include "game.h"
#include "settings.h"



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

// void main(){
// 	displayError(err_identifier,"set");
// }


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
	// printf("The acceptable (value) are... \n");
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
		printf("\nTapaiko naam '%s' bata '%s' ma badaliyeko xa. Dhanyabad. \n",prev,current);
	} else if ( set == sign ){
		printf("\nTapaiko sign %c bata %c ma change vayeko xa. Dhanyabad.\n",prev[0],current[0]);
	} else if ( set == mode ){
		if ( (strcmp(prev,"easy") == 0) && (strcmp(current,"easy") != 0 ) ){
			printf("\nTapailai jitna garo hunexa.\n");
		} else {
			printf("\nTapailai jitna sajilo hunexa. \n");
		}
	} else if ( set == ai_name ){
		printf("\nComputer ko naam '%s' bata '%s' ma change vayeko xa.\n",prev,current);
	}
}

void displayFrontPage( ){
	int i;
	for ( i = 0; i<40; i++ )
		putchar('*');
	putchar('\n');
}