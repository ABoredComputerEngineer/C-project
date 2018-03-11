
#include "game.h"
#include "ai.h"
#include "settings.h"

extern settings gameSet;
extern stat gameStat;

aiBoard *newBoard( void ){
	int i;
	aiBoard *p = malloc( sizeof(aiBoard) * 9 );
	for ( i = 0; i< 9 ; i++ ){
		p[i].score = p[i].move = GARBAGE;
	}
	return p;
}




/* Declarations and definitions of functions */

void setDepth(){

	if ( gameSet.difficulty == EASY || (gameSet.difficulty == MEDIUM  && (int)rand()%2)){
		maxDepth = 2;
	
	} else {
		if ( ai.position == PLAYER2 )
			maxDepth = 4;
		else 
			maxDepth = 5;
	}
}





int aiMove( void ){
	return getBestMove(ai , 0).move;
}


aiBoard getBestMove( player current , int depth ){
	int i,score,rv,x = 0;
	aiBoard *new = newBoard() ;
	aiBoard move;
	rv = hasEnded();
	if ( rv != 0 || depth == maxDepth){  // game has  Ended
		aiBoard x;
		x.move = GARBAGE;
		x.score = evaluateBoard();
		free( new );
		return x;
	}
	for ( i = 0; i<9 ; i++ ){
		if ( gameBoard.row[i] == ' ' ){
			performMove( i , current.position );
			//printBoard();
			if ( current.type == AI )
				score = getBestMove( human, depth + 1 ).score;
			 else
				score = getBestMove( ai , depth + 1 ).score;
			gameBoard.row[i] = ' ';
			new[x].score = score;
			new[x++].move = i;
		}
	}
	
	move = getRandomMove( new, current );
	free(new);
	return move;

}



aiBoard getRandomMove( aiBoard board[], player current){
	int bestScore;
	aiBoard bestMoveList[9];
	int i,k=0;

	if ( current.type == AI ){
		bestScore = -1000;
		for ( i = 0; i<9 && board[i].score != GARBAGE; i++ ){
			if ( board[i].score > bestScore )
				bestScore = board[i].score;
		}
	} else {
		bestScore = 1000;
		for ( i = 0; i<9 && board[i].score!=GARBAGE; i++ ){
			if ( board[i].score < bestScore )
				bestScore = board[i].score;
		}
	}

	for ( i = 0; i<9 && board[i].score != GARBAGE; i++ ){
		if ( bestScore == board[i].score )
			bestMoveList[k++] = board[i];
	}

	return bestMoveList[rand() % k];

}

int evaluateBoard( ){
	short int i,j,wins = 0,values[8],sumRow=0,k=0;

	for ( j = 0; j<7; j+= 3 ){
		for ( i = 0; i<3; i++){
			if ( gameBoard.row[i+j] == human.sign )
				sumRow--;
			else if ( gameBoard.row[i+j] == ai.sign )
				sumRow++;
		}
		values[k++] = sumRow;
		sumRow = 0;
	}
	for ( j = 0; j<3; j++ ){
		for ( i = j; i<9; i+=3 ){
			if ( gameBoard.row[i] == ai.sign )
				sumRow++;
			else if ( gameBoard.row[i] == human.sign)
				sumRow--;
		}
		values[k++] = sumRow;
		sumRow = 0;
	}
	for ( i = 0; i<9 ; i+= 4){
		if ( gameBoard.row[i] == ai.sign )
			sumRow++;
		else if (gameBoard.row[i] == human.sign )
			sumRow--;
	}
	values[k++] = sumRow;
	sumRow = 0;
	for ( i = 6; i>0; i-=2 ){
		if ( gameBoard.row[i] == ai.sign)
			sumRow++;
		else if (gameBoard.row[i] == human.sign )
			sumRow--;
	}

	values[k++] = sumRow;
	sumRow = 0;

	for ( i = 0; i<k; i++ ){
		if ( values[i] == 3 ){
			wins+=10;
			break;
		} else if ( values[i] == 2 )
			wins+=1;
		else if ( values[i] == -2 )
			wins-=1;
		else if ( values[i] == -3 ){
			wins-=10;
			break;
		}
	}
	return wins;

}

