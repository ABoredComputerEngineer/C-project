
typedef struct aiBoard {
	int score; // The 'score' of the board as seen by ai or the human
	int move; // The move which results in the score above
} aiBoard;


aiBoard *newBoard( void ); // dynamically create a new array aiBoard and fill with GARBAGE value 
void setDepth(void); // Sets the difficulty of the game
aiBoard getBestMove( player , int  ); // Returns the best possible of the human or the computer
int aiMove( void ); // Return the cell in which the ai should move for best possible outcome
aiBoard getRandomMove( aiBoard *board, player current ); // Returns a random move from a list of best possible moves
int evaluateBoard(void); // Evaluates the score of the current state of the game
