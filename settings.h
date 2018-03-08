
typedef struct settings {
	player p1; // information about player 1
	player p2; // information about player 2
	player ai; // information about computer
	int difficulty; // difficulty of the game
} settings;

typedef struct command {
	char name[6]; // Name of the command
	struct identifier *identifierList[5]; // List of the sub commands of the main commands
} command;


typedef struct identifier {
	char name[15]; // Name of the identifier
	struct option *optionList[4];  // List of the options of the identifier
} identifier;


typedef struct option{
	char name[50]; // Name of the option 
	char identifierType[15];	// Name of the identifier which the option is part of
	char value[100]; // Value of the option
} option;

typedef struct wl {
	short int win;
	short int lose;
} score;

typedef struct gs{
	score p1; // W/L of player 1
	score p2; // W/L of player 2
	score ai; // W/L of computer
	int gcSingle; // Game count of single player game
	int gcDouble; // Game count of double player game
} stat;


enum difficulty {EASY=1,MEDIUM,HARD};
enum tokens {COMMAND,IDENTIFIER,OPTION,VALUE};
enum commands {set,new,view};


// Matches a command and returns the index of the command in the command list

command *matchCommand(char [][100]);

/* The parse Command reads the command typed by the user and perfoms the necessary action */
void parseCommand(char [][100], int);

/*Reads the commands if the user types "set" as the initial command */
option *parseSet(command *,char [][100], int);

/*Reads the commands if the user types "new" as the initial command */
void parseNew(command *,char [][100], int);
/*Reads the commands if the user types "view" as the initial command */
void parseView(command *,char [][100], int);


void setOptions(option *); // Changes the game settings according to the command of the user
void applyDefault( void ); // Applies the default settings
void buildCommandTree(void); // Build the command tree for command line
void reset(void);