#include "game.h"
// #include <assert.h>
#include "settings.h"
extern settings gameSet;
extern stat gameStat;

// settings newSet;
typedef struct {
	settings set;
	stat points;
} content;


// void displayScore( void ){
// 	NEWLINE;
// 	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 1",gameStat.p1.win,gameStat.p1.lose);
// 	NEWLINE;
// 	// NEWLINE;
// 	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Player 2",gameStat.p2.win,gameStat.p2.lose);
// 	NEWLINE;
// 		// NEWLINE;
// 	printf("%-20s\tWin:%-4d\tLoss:%-4d\n","Computer",gameStat.ai.win,gameStat.ai.lose);
// 	NEWLINE;
// }

// void applyDefault( void ){
// 	strcpy(gameSet.p1.name,"Player 1");
// 	gameSet.p1.sign = '0';
// 	gameSet.p1.position = PLAYER1;
// 	gameSet.p1.type = HUMAN;
// 	strcpy( gameSet.p2.name,"Player 2");
// 	gameSet.p2.sign = 'X';
// 	gameSet.p2.position = PLAYER2;
// 	gameSet.p2.type = HUMAN;
// 	strcpy(gameSet.ai.name,"Marco Bot");
// 	gameSet.ai.sign = '+';
// 	gameSet.ai.type = AI;
// 	gameSet.ai.position = PLAYER2;
// 	gameSet.difficulty = EASY;
// }

void writeSettings(void){
	FILE *fp;
	content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen("./gameSet.dat","wb")) == NULL ){
		applyDefault();
		newSettings();
	} else {
		fwrite(&new,sizeof(content),1,fp);
		fclose(fp);
	}
}

// void displaySettings( settings *current ){
// 		NEWLINE;
// 	printf("Player 1 | \n");
// 	NEWLINE;
// 	printf("%-12s:::\t%-10s\n","Name",current->p1.name);
// 	printf("%-12s:::\t%-4c\n","Sign",current->p1.sign);
// 		NEWLINE;
// 	printf("Player 2 | \n");
// 	NEWLINE;
// 	printf("%-12s:::\t%-10s\n","Name",current->p2.name);
// 	printf("%-12s:::\t%-4c\n","Sign",current->p2.sign);
// 		NEWLINE;
// 	printf("Computer |\n");
// 	NEWLINE;
// 	printf("%-12s:::\t%-10s\n","Name",current->ai.name);
// 	printf("%-12s:::\t%-4c\n","Sign",current->ai.sign);
// 	printf("%-12s:::\t%-6s\n","Difficulty",(current->difficulty == EASY )?"Easy":((current->difficulty == MEDIUM)?"Medium":"Hard"));
	
// }

void newSettings(void){
	FILE *fp;
		content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen("./gameSet.dat","wb")) == NULL ){
		printf("Error! Cant open game file");
		return;
	}
	fwrite(&new,sizeof(content),1,fp);
	// fwrite(&gameStat,sizeof(gameStat),1,fp);
	fclose(fp);
}

void readSettings(void){
	FILE *fp;
	int x;
	content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen("./gameSet.dat","rb")) == NULL ){
		applyDefault();
		newSettings(); 
	} else {
		x = (int)fread(&new,sizeof(content),1,fp);
		if ( x == 0){
			printf("Error Occurred\n");
					applyDefault();
			newSettings(); 
		}
		gameSet = new.set;
		gameStat = new.points;
		fclose(fp);
	}
}

// void main(){
// 	readSettings();
// 	displaySettings(&gameSet);
// 	// gameSet.ai.sign = 'Y';
// 	// gameSet.difficulty = HARD;
// 	// gameStat.p1.win = 100;
// 	// gameStat.ai.win = 100;
// 	writeSettings();
// 	displayScore();
// 	// readSettings();


// }