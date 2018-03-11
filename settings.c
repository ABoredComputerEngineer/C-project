#include "game.h"
#include "ai.h"
#include "settings.h"
settings gameSet;
stat gameStat;
stat currentStat;

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