#include "game.h"
#include <assert.h>

#define MAX_OPTIONS 3
#define MAX_IDENTIFIERS 7
#define MAX_COMMANDS 4
#define IN 1
#define OUT 0


typedef struct command {
	char name[10];
	struct identifier *identifierList[3];
	struct option *exOption;
} command;


typedef struct identifier {
	char name[10];
	struct option *optionList[4];
} identifier;


typedef struct option{
	char name[10];
	char identifierType[10];
	char value[100];
	// struct option *next;
} option;

enum difficulty {EASY=1,MEDIUM,HARD};
enum tokens {COMMAND,IDENTIFIER,OPTION,VALUE};
enum commands {set,new,view};

// Splits a string removing whitespaces and stores them in an array of strings
int stringSplit(char [], char [][100] );

// Matches a command and returns the index of the command in the command list

command *matchCommand(char [][100]);

// Matches a identifier in the idenfier list of the given command index
identifier *matchIdentifier( command * , char [][100] );
void parseCommand(char [][100], int);
	command commandList[MAX_COMMANDS];
	identifier identifierList[MAX_IDENTIFIERS];
	option optionList[MAX_OPTIONS];
void buildCommandTree(){
	int i = 0,j=0;
	for ( i = 0; i<MAX_COMMANDS; i++ ){
		for ( j = 0; j<3; j++ )
			commandList[i].identifierList[j] = NULL;
		commandList[i].exOption = NULL;
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
	commandList[0].exOption = &optionList[2]; // equivaluent to set difficulty


	strcpy(identifierList[0].name ,"player1");
	identifierList[0].optionList[0] = &optionList[0]; // name option
	identifierList[0].optionList[1] = &optionList[1]; // Symbol option

	strcpy(identifierList[1].name ,"player2");
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
	strcpy(optionList[1].name , "symbol");
	strcpy(optionList[2].name , "difficulty");

}


int main(){
	int i,k;
	command *j;
	option *new;
	buildCommandTree();

	char command[100];
	char tokenList[10][100];
	while ( 1) {
		putchar('>');
		scanf("%[^\n]",command);
		getchar();
		k = stringSplit(command,tokenList);
		printf("tokens : %d\n",k);
		
		parseCommand(tokenList, k);
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

// identifier *matchIdentifier(command *current, char tokenList[][100]){
// 	int i;
// 	for ( i = 0 ; current->identifierList[i] != NULL ; i++ ){	
// 		if ( strcmp(current->identifierList[i]->name,tokenList[IDENTIFIER]) == 0 )
// 			return i;
// 	}

// 	if ( current->exOption != NULL ){ // The given command has an extra option

// 		if ( strcmp(current->exOption->name,tokenList[IDENTIFIER]) == 0 ) // If the second identifier is an option instead eg. "set difficulty"
// 			return 10;
// 	}
// 	return -1;
// }


void parseCommand(char tokenList[][100], int tokens){ // tokens is the number of tokens 
	int i;
	command *current = NULL;
	identifier *identify = NULL;
	option *opt = NULL;
	current = matchCommand(tokenList);

	if ( current == NULL ){
		return;
	} else if ( strcmp(current->name,"set") == 0 ){
		printf("\n%s\n",current->name);
		for ( i = 0; i<3 && current->identifierList[i] != NULL ; i++ ){
			if ( strcmp(current->identifierList[i]->name, tokenList[IDENTIFIER]) == 0 ){
				identify = current->identifierList[i];
			}
		}
		if (identify == NULL ) {
			printf("identifier Not found\n");
			return ;
		}

		for ( i = 0; i<4 && identify->optionList[i] != NULL ; i++ ){
				if ( strcmp(identify->optionList[i]->name,tokenList[OPTION]) == 0 ){
					opt = identify->optionList[i];
					strcpy(opt->identifierType,identify->name);
					strcpy(opt->value,tokenList[VALUE]);
					printf("Writing to options\n");
				}
		}
	} else if ( strcmp(current->name,"new") == 0){
		printf("\n%s\n",current->name);
		if ( tokens == 1 ){
			printf("\nStarting a new game \n");
		} else if ( strcmp(current->identifierList[0]->name, tokenList[IDENTIFIER]) == 0 ){
			printf("\nStarting a single player game\n");
		} else if ( strcmp(current->identifierList[1]->name, tokenList[IDENTIFIER]) == 0 ) {
			printf("\nStarting two player game\n");
		} else {
			printf("Command  not recognized\n");
			return;
		}
	} else if ( strcmp( current->name , "view") == 0 ){
		printf("\n%s\n",current->name);
		for ( i = 0; i<3 && current->identifierList[i] != NULL ; i++ ){
			if ( strcmp(current->identifierList[i]->name, tokenList[IDENTIFIER]) == 0 ){
				identify = current->identifierList[i];
			}
		} 
		if ( identify == NULL ){
			printf("\nInvlaid Command\n");
			return;
		}
		if ( strcmp(identify->name, "score") == 0 ){
			printf("\nDisplaying Score\n");
		} else if ( strcmp(identify->name, "settings") == 0){
			printf("\nDisplaying Settings\n");
		} else {
			printf("Command not recognized\n");
			return;
		}

	} else if ( strcmp(current->name,"exit") == 0){
		exit(1);
	}
	return;
	// for ( i = 0; i<3; i++ ){
	// 	if ( strcmp(tokenList[COMMAND],commandList[i].name) == 0 ){
	// 		current = &commandList[i];
	// 	}
	// }
	// if ( current == NULL ){
	// 	printf("Command Not found\n");
	// 	return NULL;
	// }

	// for ( i = 0; i<7 && current->identifierList[i] != NULL ; i++ ){
	// 	if ( strcmp(current->identifierList[i]->name, tokenList[IDENTIFIER]) == 0 ){
	// 		identify = current->identifierList[i];
	// 	}
	// }

	// if ( identify == NULL && current->exOption != NULL && ( strcmp(current->exOption->name,tokenList[IDENTIFIER]) == 0 ) ){
	// 		opt = current->exOption;
	// 		return NULL;
	// } else if (identify == NULL ) {
	// 	printf("identifier Not found\n");
	// 	return NULL;
	// }

	// for ( i = 0; i<4 && identify->optionList[i] != NULL ; i++ ){
	// 		if ( strcmp(identify->optionList[i]->name,tokenList[OPTION]) == 0 ){
	// 			opt = identify->optionList[i];
	// 			strcpy(opt->identifierType,identify->name);
	// 			strcpy(opt->value,tokenList[VALUE]);
	// 		}
	// }

	// return opt;

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




