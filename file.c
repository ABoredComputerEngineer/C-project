#include "game.h"
#include "settings.h"
extern settings gameSet;
extern stat gameStat;

typedef struct {
	settings set;
	stat points;
} content;


void writeSettings(void){
	FILE *fp;
	content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen(PATH,"wb")) == NULL ){
		applyDefault();
		newSettings();
	} else {
		fwrite(&new,sizeof(content),1,fp);
		fclose(fp);
	}
}

void newSettings(void){
	FILE *fp;
		content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen(PATH,"wb")) == NULL ){
		printf("Error! Cant open game file");
		return;
	}
	fwrite(&new,sizeof(content),1,fp);
	fclose(fp);
}

void readSettings(void){
	FILE *fp;
	int x;
	content new;
	new.set = gameSet;
	new.points = gameStat;
	if ( (fp = fopen(PATH,"rb")) == NULL ){
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
