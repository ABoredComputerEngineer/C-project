#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOARD 9
#define BOARD_ROW 3
#define BOARD_COL 3
#define PLAYER1 1
#define PLAYER2 2
#define P1_WIN 1
#define P2_WIN 2
#define TIE 3
#define TRUE 1
#define FALSE 0 
#define GARBAGE 322
#define AI 1
#define HUMAN 0

typedef struct game {
	char mat[BOARD_ROW][BOARD_COL];// The game board stored in matrix form
	char row[BOARD]; //The game board stored in a single array
} game; 

typedef struct aiBoard {
	int score;
	int move;
} aiBoard;

typedef struct player {
	short int type;
	short int position;
	char sign;
} player;


aiBoard *newBoard( void ){
	int i;
	aiBoard *p = malloc( sizeof(aiBoard) * 9 );
	for ( i = 0; i< 9 ; i++ ){
		p[i].score = p[i].move = GARBAGE;
	}
	return p;
}


/* Declaration of the Global Variables */
game gameBoard;
int turn = 1;
// const int aiPlayer = 2;
// const int humanPlayer = 1;
short int maxDepth;
player player1,player2,ai,human;

/* Declarations and definitions of functions */

void setDepth(){
	if ( ai.position == PLAYER2 )
		maxDepth = 4;
	else 
		maxDepth = 5;
}
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
	if ( gameBoard.row[4] != ' '){

		if ( gameBoard.row[0] == gameBoard.row[4] && gameBoard.row[4] == gameBoard.row[8] )
			return (gameBoard.row[4] == player1.sign)?PLAYER1:PLAYER2;
		if ( gameBoard.row[2] == gameBoard.row[4] && gameBoard.row[4] == gameBoard.row[6] )
			return (gameBoard.row[4] == player1.sign)?PLAYER1:PLAYER2;
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
aiBoard getBestMove( player current, int depth );
int aiMove(  ){
	return getBestMove(ai , 0).move;
}

aiBoard getRandomMove( aiBoard *board, player current );
int evaluateBoard(void);
int main( ){
	// delay( 5 );
	int i, cell;
	player current;
	char decision;
	
	printf("\nFirst Move or Second Move? ( 1 for first and 2 for 2nd).....\n");
	scanf("%c",&decision);
	if ( decision == '1' ){
		player1.type = HUMAN;
		player1.position = PLAYER1;
		player1.sign = '0';

		player2.type = AI;
		player2.position = PLAYER2;
		player2.sign = 'X';

	} else {
		player1.type = AI;
	player1.position = PLAYER1;
	player1.sign = '0';

	player2.type = HUMAN;
	player2.position = PLAYER2;
	player2.sign = 'X';

	}
	ai = ( player1.type == AI )?player1:player2;
	human = ( player1.type == HUMAN )?player1:player2;
	setDepth();
	current = player1;
	srand(time(NULL));
	initializeBoard();
	printBoard();
	while ( !hasEnded() ){
		if ( current.type == AI ){
			cell = aiMove();
		} else {

			cell = getMove();
		}
		performMove(cell, current.position);	
		printBoard();
		turn++;
		current = ( current.position == PLAYER1 )?player2:player1;
	}
	return 0;
}



aiBoard getBestMove( player current , int depth ){
	int i,score,rv,x = 0;
	aiBoard *new = newBoard() ;
	aiBoard move;
	rv = hasEnded();
	if ( rv != 0 || depth == maxDepth){  // game has  Ended
		aiBoard x;
		x.move = GARBAGE;
		// if ( rv == PLAYER1 )
		// 	x.score = -10;
		// else if ( rv == PLAYER2 )
		// 	x.score = 10;
		// else 
		// 	x.score = 0;
		x.score = evaluateBoard();
		free( new );
		return x;
	}
	for ( i = 0; i<9 ; i++ ){
		if ( gameBoard.row[i] == ' ' ){
			performMove( i , current.position );
			printBoard();
			// depth++;
			if ( current.type == AI )
				score = getBestMove( human, depth + 1 ).score;
			 else
				score = getBestMove( ai , depth + 1 ).score;
			gameBoard.row[i] = ' ';
			new[x].score = score;
			new[x++].move = i;
		}

	}

	for ( i = 0; new[i].score!=GARBAGE &&i<9; i++ )
		printf("Score: %d\tMove:%d\t%s\n",new[i].score,new[i].move,(current.type==AI)?"AI":"Human");
	putchar('\n');


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
	short int row,col,i,j,wins = 0,values[8],sumRow=0,sumCol = 0,k=0;
	// player ai,human;


	// /* Now the  game is stored in a 3x3 matrix*/
	// for ( row = 0; row<3; row++ ){
	// 	for ( col = 0; col<3; col++ )
	// 		gameBoard.mat[row][col] = gameBoard.row[k++];
	// }
	// k = 0;
	// //Evaluate the rows and columns
	// for ( row = 0; row < 3; row++ ){
	// 	for ( col = 0; col < 3; col++ ){
	// 		if ( gameBoard.mat[row][col] == ai.sign )
	// 			sumRow+=1;
	// 		else if ( gameBoard.mat[row][col] == human.sign )
	// 			sumRow -= 1;
	// 		if ( gameBoard.mat[col][row] == ai.sign )
	// 			sumCol += 1;
	// 		else if ( gameBoard.mat[col][row] == human.sign )
	// 			sumCol-=1;
	// 	}
	// 	values[k++] = sumRow;
	// 	values[k++] = sumCol;
	// 	sumCol = 0;
	// 	sumRow = 0;
	// }

	// //Evaluate the diagonals
	// for  ( row = 0; row<3; row++ ){
	// 	if ( gameBoard.mat[row][row] == ai.sign )
	// 		sumRow+=1;
	// 	else if ( gameBoard.mat[row][row] == human.sign )
	// 		sumRow-=1;
	// }
	// sumRow = 0;
	// values[k++] = sumRow;
	// for ( row = 0; row<3 ; row++ ){
	// 	if ( gameBoard.mat[row][2-row] == ai.sign )
	// 		sumRow+=1;
	// 	else if ( gameBoard.mat[row][2-row] == human.sign )
	// 		sumRow-=1; 
	// }
	// values[k++] = sumRow;
	// quickSort( values, 0, k-1 );

	for ( j = 0; j<7; j+= 3 ){
		for ( i = 0; i<3; i++){
			if ( gameBoard.row[i+j] == human.sign )
				sumRow--;
			else if ( gameBoard.row[i+j] == ai.sign )
				sumRow++;
		// printf("%d\t",sumRow);
		}
	// printf("\n");
		values[k++] = sumRow;
		sumRow = 0;
	}
		// printf("\n");
	for ( j = 0; j<3; j++ ){
		for ( i = j; i<9; i+=3 ){
			if ( gameBoard.row[i] == ai.sign )
				sumRow++;
			else if ( gameBoard.row[i] == human.sign)
				sumRow--;
			// printf("%d\t",sumRow);
		}
		// printf("\n");
		values[k++] = sumRow;
		sumRow = 0;
	}
	// printf("\n");
	for ( i = 0; i<9 ; i+= 4){
		if ( gameBoard.row[i] == ai.sign )
			sumRow++;
		else if (gameBoard.row[i] == human.sign )
			sumRow--;
		// printf("%d\t",sumRow);
	}
	// printf("\n");
	values[k++] = sumRow;
	sumRow = 0;
	// printf("\n");
	for ( i = 6; i>0; i-=2 ){
		if ( gameBoard.row[i] == ai.sign)
			sumRow++;
		else if (gameBoard.row[i] == human.sign )
			sumRow--;
		// printf("%d\t",sumRow);
	}
	// printf("\n");
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

