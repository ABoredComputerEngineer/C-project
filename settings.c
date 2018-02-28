#include "game.h"


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
	strcpy(identifierList[5].name , "settings");
	strcpy(identifierList[6].name , "score");

	strcpy(optionList[0].name , "name" );
	strcpy(optionList[1].name , "sign");
	strcpy(optionList[2].name , "mode");

}


int main(){
	int k;
	buildCommandTree();
	// gameSet.p1 = &player1;
	// gameSet.p2 = &player2;
	gameSet.difficulty = EASY;
	char command[100];
	char tokenList[10][100];
	applyDefault();
	gameStat.gcSingle = 0;
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
	for ( i = 0; i<MAX_COMMANDS; i++ ){
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
		exit(1);
	} else {
		printf("Invaid command\n");
	}
	return;

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
}


void displaySettings( void ){
	printf("\nPlayer 1::: \n");
	NEWLINE;
	printf("%-8s:::%20s\n","Name",gameSet.p1.name);
	printf("%-8s:::%4c\n","Sign",gameSet.p1.sign);
	printf("\nPlayer 2::: \n");
	NEWLINE;
	printf("%-8s:::%20s\n","Name",gameSet.p2.name);
	printf("%-8s:::%4c\n","Sign",gameSet.p2.sign);
	printf("\nMarco Bot\n");
	NEWLINE;
	printf("%-8s:::%20s\n","Name",gameSet.ai.name);
	printf("%-8s:::%4c\n","Name",gameSet.ai.sign);
	printf("%-8s:::%6s\n","Name",(gameSet.difficulty == EASY )?"Easy":((gameSet.difficulty == MEDIUM)?"Medium":"Hard"));
	
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
		for ( i = 0; i<3 && current->identifierList[i] != NULL ; i++ ){
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
		} else if ( strcmp(identify->name, "settings") == 0){
			// printf("\nDisplaying Settings\n");
			displaySettings();
		} else {
			printf("Command not recognized\n");
			return;
		}

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