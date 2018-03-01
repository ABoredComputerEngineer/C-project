// #include "game.h"

typedef struct aiBoard {
	int score;
	int move;
} aiBoard;


aiBoard *newBoard( void );
void setDepth(void);
aiBoard getBestMove( player , int  );
int aiMove( void );
aiBoard getRandomMove( aiBoard *board, player current );
int evaluateBoard(void);
