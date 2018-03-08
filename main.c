#include "game.h"
#include "ai.h"
#include "settings.h"
extern settings gameSet;
extern stat gameStat;

int main(){
	int k;
	buildCommandTree();
	char command[100];
	char tokenList[10][100];
	readSettings();
	srand(time(NULL));
	while ( 1) {
		putchar('>');
		getString(command);
		k = stringSplit(command,tokenList);
		
		parseCommand(tokenList, k);
	}
	return 0;
}
