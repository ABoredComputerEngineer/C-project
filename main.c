#include "game.h"
#include "ai.h"
#include "settings.h"
extern settings gameSet;
extern stat gameStat;

int main(){
	int k;
	buildCommandTree();
	printf("%s\n",PATH);
		// strcpy(commandList[3].name , "exit");
	// //printf("current command: %s\n",commandList[3].name);
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
		// //printf("tokens : %d\n",k);
		
		parseCommand(tokenList, k);
		// getchar();
		// //printf("%d\n",gameSet.difficulty);
		// displaySettings();
	}
}
