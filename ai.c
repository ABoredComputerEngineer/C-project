#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOARD 9
#define BOARD_ROW 3
#define BOARD_COL 3
#define PLAYER1 0
#define PLAYER2 1
#define P1_WIN 1
#define P2_WIN 2
#define TIE 3
#define TRUE 1
#define FALSE 0 
#define GARBAGE 322


typedef struct game {
	char mat[BOARD_ROW][BOARD_COL];// The game board stored in matrix form
	char row[BOARD]; //The game board stored in a single array
} game; 

typedef struct aiBoard {
	int score;
	int move;
} aiBoard;

aiBoard *newBoard( void ){
	int i;
	aiBoard *p = malloc( sizeof(aiBoard) * 9 );
	for ( i = 0; i< 9 ; i++ ){
		p[i].score = p[i].move = GARBAGE;
	}
	return p;
}

// int evaluateBoard( ){
// 	int i,j, evalAI,evalHuman, k = 0;
// 	char AI = '0', human = '+';

// 	/* Now the  game is stored in a 3x3 matrix*/
// 	for ( i = 0; i<3; i++ ){
// 		for ( j = 0; j<3; j++ )
// 			gameBoard.mat[i][j] = gameBoard.row[k++];
// 	}

// 	for ( i = 0; i< 3; i++ ){
// 		for ( j = 0; j<3; j++ ){

// 		}
// 	}

// }
/* Declaration of the Global Variables */
game gameBoard;
int turn = 1;
const int aiPlayer = 1;
const int humanPlayer = 0;
const int maxDepth = 2;


/* Declarations and definitions of functions */


/* ctoi converts the character to an integer if it is between 1  and 9*/
int ctoi( char c ){
	if ( c > '0' && c<='9' ) 
		return c - '0' - 1 ;
	else
		return -1;
}

/* Initializes the board to the initial state by setting the entire board to empty*/
void initializeBoard(void){
	int i,j;
	for ( i = 0; i<BOARD_ROW ; i++ )
		for ( j = 0; j<BOARD_COL; j++ )
			gameBoard.mat[i][j] = ' ';
	for ( i = 0; i<BOARD; i++ )
		gameBoard.row[i] = ' ';
}


/* prints the board according to the row type of the game board*/
void printBoard( ){
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.row[6],gameBoard.row[7],gameBoard.row[8]);
	printf("\t\t         |        |         \n");
	printf("\t\t---------|--------|---------\n");
	//printf("\t\t         |        |         \n");
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.row[3],gameBoard.row[4],gameBoard.row[5]);
	printf("\t\t         |        |         \n");
	printf("\t\t---------|--------|---------\n"); 	
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.row[0],gameBoard.row[1],gameBoard.row[2]);
	printf("\t\t         |        |         \n"); 
	// printf("\t\t         |        |         \n");
	putchar('\n');
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
			return (gameBoard.row[i+1] == '0')?P1_WIN:P2_WIN;
		}
	}
	/*Checking if the game has ended in any of the column*/
	for ( i = 0; i<3 ; i++ ){
		if ( gameBoard.row[i+3] == ' ')
			continue;
		if ( gameBoard.row[i] == gameBoard.row[i+3] && gameBoard.row[i+3] == gameBoard.row[i+6] )
			return (gameBoard.row[i+3] == '0')?P1_WIN:P2_WIN;
	}
	/*Checking if the game has ended in any of the diagonals*/
	if ( gameBoard.row[4] != ' '){

		if ( gameBoard.row[0] == gameBoard.row[4] && gameBoard.row[4] == gameBoard.row[8] )
			return (gameBoard.row[4] == '0')?P1_WIN:P2_WIN;
		if ( gameBoard.row[2] == gameBoard.row[4] && gameBoard.row[4] == gameBoard.row[6] )
			return (gameBoard.row[4] == '0')?P1_WIN:P2_WIN;
	}

	for ( i =0 ; i<9; i++ ){
		if ( gameBoard.row[i] == ' ')
			return FALSE;
	}
	return TIE;
	/*If it has not ended */
	// return FALSE;
}

void performMove( int x , int player ){
	/* Changes the value in the game board accoriding to the player, player 1 is 0 
	and player 2 is + */
	gameBoard.row[x] = ( player == aiPlayer )?'0':'+';;

}


/* Checks if the move the user is trying to do is legal or not */
int isLegal( int x  ){
	/* If the character is not valid or the cell the user is trying to fill is not Empty*/
	if ( x == -1 || gameBoard.row[x] != ' ')
		return FALSE;
	else 
		return TRUE; 
}

/* Gets the move the user wants to perform in the form of a string and returns the value of the cell
	in the board */

int getMove( void ){
	char in[100];
	while ( TRUE ){
		scanf("%s",in);
		if ( strlen(in) == 1 ){
			if ( isLegal(ctoi(in[0])) )
				return ctoi(in[0]);
		//	else 
		//		printf("\nInvalid Move\n");
		}// else {
		//	printf("\nInvalid Move\n");
		//}
	printf("\nInvalid Move\n");
	}
	
}
aiBoard getBestMove( int player, int depth );
int aiMove( ){
	return getBestMove(aiPlayer , 0).move;
}

int main( ){
	// delay( 5 );
	int i, cell, player = humanPlayer ;
	// move newMove;
	initializeBoard();
	printBoard();
	// // gameBoard.row = {'0','0','+',' ','0','+',' ',' ','+'};
	// gameBoard.row[0] = '0';
	// gameBoard.row[1] = '0';
	// gameBoard.row[2] = '+';
	// gameBoard.row[3] = ' ';
	// gameBoard.row[4] = '0';
	// gameBoard.row[5] = '+';
	// gameBoard.row[8] = '+';
	// printBoard();
	// printf("%d\n",hasEnded());
	while ( !hasEnded() ){
		if ( player == aiPlayer ){
			cell = aiMove();
		} else {

			cell = getMove();
		}

		// newMove.x = (cell - 1 ) % 3;
		// newMove.y = (cell - 1 )/3;
		performMove(cell, player);	
		printBoard();
		turn++;
		// player = !player;
		player = ( player == aiPlayer )?humanPlayer:aiPlayer;
	}

	// for ( i = 0; i<9; i++ ){
	// 	cell = aiMove();
	// 	printf("%d\n",cell+1);
	// 	performMove(cell,aiPlayer);
	// }
	// printf("You won , %d\n",hasEnded());
	return 0;
}



aiBoard getBestMove( int player , int depth ){
	int i,score,rv,x = 0;
	aiBoard *new = newBoard() ;
	aiBoard min = { 1000, GARBAGE},max = { -1000, GARBAGE };
	rv = hasEnded();
	// printf("%d\n",rv);
	if ( rv != 0 || depth == maxDepth){  // game has  Ended
		aiBoard x;
		x.move = GARBAGE;
		if ( rv == P1_WIN )
			x.score = 10;
		else if ( rv == P2_WIN )
			x.score = -10;
		else 
			x.score = 0;
		free( new );
		return x;
	}
	for ( i = 0; i<9 ; i++ ){
		if ( gameBoard.row[i] == ' ' ){
			performMove( i , player );
			printBoard();
			// depth++;
			if ( player == aiPlayer )
				score = getBestMove( humanPlayer, depth + 1 ).score;
			 else
				score = getBestMove( aiPlayer , depth + 1 ).score;
			gameBoard.row[i] = ' ';
			new[x].score = score;
			new[x++].move = i;
		}

	}

	for ( i = 0; new[i].score!=GARBAGE &&i<9; i++ )
		printf("Score: %d\tMove:%d\t%s\n",new[i].score,new[i].move,(player==aiPlayer)?"AI":"Human");
	putchar('\n');



	if ( player == aiPlayer ){
		for ( i = 0; new[i].score != GARBAGE && i<9; i++ ){
			if ( max.score < new[i].score )
				max = new[i];
		}
		free(new);
		// printf("Max Score:%d \t Move:%d\n",max.score,max.move);
		return max;
	} else {
		for ( i = 0; new[i].score!= GARBAGE && i<9; i++ )
			if ( min.score>new[i].score )
				min = new[i];
		free(new);
		// printf("Min Score:%d \t Move:%d\n",min.score,min.move);
		// printf("%d\n",min.score);
		return min;
	}

}



