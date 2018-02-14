#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD 9
#define BOARD_ROW 3
#define BOARD_COL 3
#define PLAYER1 0
#define PLAYER2 1
#define TRUE 1
#define FALSE 0 

void delay(double dly){
    /* save start clock tick */
    const clock_t start = clock();

    clock_t current;
    do{
        /* get current clock tick */
        current = clock();

        /* break loop when the requested number of seconds have elapsed */
    }while((double)(current-start)/CLOCKS_PER_SEC < dly);
}


typedef struct game {
	char mat[BOARD_ROW][BOARD_COL];
	char row[BOARD]; 
} game; 

game gameBoard;

typedef struct move {
	int x;
	int y;
} move;

int ctoi( char c ){
	if ( c > '0' && c<='9' ) 
		return c - '0';
	else
		return -1;
}

void initializeBoard(void){
	int i,j;
	for ( i = 0; i<BOARD_ROW ; i++ )
		for ( j = 0; j<BOARD_COL; j++ )
			gameBoard.mat[i][j] = ' ';
	for ( i = 0; i<BOARD; i++ )
		gameBoard.row[i] = ' ';
}
void printBoard( ){
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.mat[2][0],gameBoard.mat[2][1],gameBoard.mat[2][2]);
	printf("\t\t         |        |         \n");
	printf("\t\t---------|--------|---------\n");
	//printf("\t\t         |        |         \n");
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.mat[1][0],gameBoard.mat[1][1],gameBoard.mat[1][2]);
	printf("\t\t         |        |         \n");
	printf("\t\t---------|--------|---------\n"); 	
	printf("\t\t         |        |         \n");
	printf("\t\t    %c    |    %c   |    %c     \n",gameBoard.mat[0][0],gameBoard.mat[0][1],gameBoard.mat[0][2]);
	printf("\t\t         |        |         \n"); 
	// printf("\t\t         |        |         \n");
}

int hasEnded( void ){
	int i,j;
	// CHECK FOR ROWS
	for ( i = 0; i<BOARD_ROW; i++ )
		for ( j = 0; j<BOARD_COL-1; j++ ){
			if ( gameBoard.mat[i][j] != gameBoard.mat[i][j+1] )
				return TRUE;
		}
	// CHECK FOR COLUMS
	for  ( j = 0; j<BOARD_COL ; j++ )
		for( i = 0; i<BOARD_ROW-1; i++ )
			if ( gameBoard.mat[i+1][j] != gameBoard.mat[i][j] )
				return TRUE;
	// CHECK FOR DIAGONALS
	for ( i = 0; i<BOARD_ROW; i++ )
		if ( gameBoard.mat[i][i] != gameBoard.mat[i+1][i+1] )
			return TRUE;
	if ( (gameBoard.mat[2][0] != gameBoard.mat[1][1] ) || ( gameBoard.mat[0][2] != gameBoard.mat[1][1] ) )
		return TRUE;

	return FALSE;

}

void performMove( move *a , int player ){
	char sym;
	sym = ( player )?'0':'+';
	gameBoard.mat[a->y][a->x] = sym;

}

int isLegal( int x  ){
	if ( gameBoard.row[x-1] != ' ' )
		return FALSE;
	else 
		return TRUE; 
}


int getMove( void ){
	int x;
	while ( TRUE ){
		x = ctoi(getchar());
		if (  x != -1  && isLegal(x) )
			return x;
			// printf("\nInvalid move\n");
	}
}


int main( ){
	// delay( 5 );
	int player = PLAYER1, cell;
	move newMove;
	initializeBoard();
	printBoard();

	while ( !hasEnded() ){
		cell = getMove();
		newMove.x = (cell - 1 ) % 3;
		newMove.y = (cell - 1 )/3;
		performMove(&newMove, player);
		printBoard();
		player = !player;
	}

	printf("You won");
	return 0;
}
