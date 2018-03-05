#include "game.h"
#include "ai.h"
#include "settings.h"
extern settings gameSet;
extern stat gameStat;

/* Initializes the board to the initial state by setting the entire board to empty*/
void initializeBoard(void){
	int i,j;
	for ( i = 0; i<BOARD_ROW ; i++ )
		for ( j = 0; j<BOARD_COL; j++ )
			gameBoard.mat[i][j] = ' ';
	for ( i = 0; i<BOARD; i++ )
		gameBoard.row[i] = ' ';
}


/* Checks if the game has ended or not */

int hasEnded( void ){
	int i;
	/*Checking if the game has ended in any of the row*/
	for ( i = 0; i<7 ; i+= 3 ){
		/*If one of cell of the row is empty , the game has not ended in that row*/
		if (  gameBoard.row[i+1] == ' ')
			continue;
		/* If all the values in the row are equal , The game has ended */
		if ( gameBoard.row[i] == gameBoard.row[i+1] && gameBoard.row[i+1] == gameBoard.row[i+2] ){
			return (gameBoard.row[i+1] == player1.sign)?PLAYER1:PLAYER2;
		}
	}
	/*Checking if the game has ended in any of the column*/
	for ( i = 0; i<3 ; i++ ){
		if ( gameBoard.row[i+3] == ' ')
			continue;
		if ( gameBoard.row[i] == gameBoard.row[i+3] && gameBoard.row[i+3] == gameBoard.row[i+6] )
			return (gameBoard.row[i+3] == player1.sign)?PLAYER1:PLAYER2;
	}
	/*Checking if the game has ended in any of the diagonals*/
	if ( gameBoard.row[4] != ' '){ // If the center square is empty then game has not ended in any of diagonals

		if ( gameBoard.row[0] == gameBoard.row[4] && gameBoard.row[4] == gameBoard.row[8] )
			return (gameBoard.row[4] == player1.sign)?PLAYER1:PLAYER2;
		if ( gameBoard.row[2] == gameBoard.row[4] && gameBoard.row[4] == gameBoard.row[6] )
			return (gameBoard.row[4] == player1.sign)?PLAYER1:PLAYER2;
	}

	/* If the game has not ended in  any of the rows columns or diagonals and there is still a empty box, It has not ended*/
	for ( i =0 ; i<9; i++ ){
		if ( gameBoard.row[i] == ' ')
			return FALSE;
	}

	//If there is no empty cell
	return TIE;
}



/* "Performs" the move of the player 1 or player2 by changing the values in the game board 
	x is the cell in which the symbol is to be placed 
	player is the player whose symbol is to be places ( Player 1 or Player 2)*/
void performMove( int x , int player ){

	gameBoard.row[x] = ( player == PLAYER1 )?player1.sign:player2.sign;

}


/* Checks if the move the user is trying to do is legal or not */
int isLegal( int x  ){
	/* If the character is not valid or the cell the user is trying to fill is not Empty*/
	if ( x == -1 || gameBoard.row[x] != ' ')
		return FALSE;
	else 
		return TRUE; 
}

void playGame(int mode){
	int winner;
	initializeBoard();
	printBoard();
	if ( mode == SINGLE ){

		player1 = (gameStat.gcSingle%2)?gameSet.ai:gameSet.p1;
		player2 = (gameStat.gcSingle%2)?gameSet.p1:gameSet.ai;
		player1.position = PLAYER1;
		player2.position = PLAYER2;
		winner = playSingle();
		if ( winner == PLAYER1 ){
			if ( player1.type == AI )
				gameStat.ai.win++;
			else 
				gameStat.ai.lose++;
		} else if ( winner == PLAYER2 ){
			if ( player2.type == AI )
				gameStat.ai.win++;
			else
				gameStat.ai.lose++;
		}
	} else {
		player1 = gameSet.p1;
		player2 = gameSet.p2;
		winner = playDouble();
		if ( winner == PLAYER1 ){
			gameStat.p1.win++;
			gameStat.p2.lose++;
		}
		else {
			gameStat.p2.win++;
			gameStat.p1.lose++;
		}
	}
	if ( winner == PLAYER1 ){
		printf("%s wins!!!\n",player1.name);
	} else if (winner == PLAYER2) {
		printf("%s wins!!!\n",player2.name);
	} else {
		printf("The game has been drawn\n");
	}
	writeSettings();
}

int playSingle( void ){
	player current;
	current = player1;
	ai = (player1.type == AI )?player1:player2;
	human = (player1.type == HUMAN )?player1:player2;
	int cell,rv;
	setDepth();
	while ( !(rv=hasEnded()) ){
		printf("%s\n",(current.type == AI)?"Ai turn":"Human turn");
		if ( current.type == AI )
			cell = aiMove();
		else {
			printf("%s's turn: ", current.name);
			cell = getMove();
		}
		performMove(cell, current.position);
		current = ( current.position == PLAYER1 )?player2:player1;		
		printBoard();
	}
	gameStat.gcSingle++;
	return rv;
}

int playDouble( void ){
	player current;

	int cell,rv;

	current = player1;

	while ( !(rv=hasEnded()) ){
		printf("%s's turn: ",current.name);
		cell = getMove();
		performMove(cell,current.position);
		current = ( current.position == PLAYER1 )?player2:player1;
		printBoard();
	}

	gameStat.gcDouble++;
	return rv;
}