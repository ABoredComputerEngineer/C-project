#include "game.h"

#define MAX_OPTIONS 3
#define MAX_IDENTIFIERS 7
#define MAX_COMMANDS 3
#define IN 1
#define OUT 0


// Splits a string removing whitespaces and stores them in an array of strings
int stringSplit(char [], char [][100] );

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
	char value[100];
	// struct option *next;
} option;

enum difficulty {EASY=1,MEDIUM,HARD};

	command commandList[MAX_COMMANDS];
	identifier identifierList[MAX_IDENTIFIERS];
	option optionList[MAX_OPTIONS];
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

	commandList[0].identifierList[0] = &identifierList[0]; // identiferList[0] is player 1
	commandList[0].identifierList[1] = &identifierList[1]; // identifierList[1] is player 2
	commandList[0].identifierList[2] = &identifierList[2]; // identifierList[2] is ai
	commandList[0].exOption = &optionList[2];


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
	buildCommandTree();
	// for ( i = 0; i<3; i++){
		// printf("%s\t",commandList[0].identifierList[2]->optionList[2]->name);
	// }
	// putchar('\n');

	char string[100];
	char out[10][100];
	putchar('>');
	scanf("%[^\n]",string);
	k = stringSplit(string,out);
		// printf("%s\n",string);
	for ( i = 0; i<k; i++ )
		printf("%s\n",out[i]);
	return 0;
}

int stringSplit( char in[], char out[][100]){
	int i,j,k = 0,state = OUT,wordCount = 0;

	for ( i = 0; in[i] != '\0'; i++ ){
		// putchar(in[i]);
		if ( in[i] == ' ' ){
			state = OUT;
		} else if (state == OUT) {
			if ( k != 0 ){
				out[wordCount][k++] = '\0';	
				wordCount++;
				k = 0;
			}
			state = IN;

		}

		if ( state == IN ){
			out[wordCount][k++] = in[i];
		}

	}
	out[wordCount][k] = '\0';
	return wordCount + 1;
}




