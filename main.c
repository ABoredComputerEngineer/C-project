#include "game.h"
#include "ai.h"
#include "settings.h"

extern settings gameSet; // defined in settings.c
extern stat gameStat; // defined in settings.c


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
