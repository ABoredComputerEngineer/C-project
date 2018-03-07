
typedef struct settings {
	player p1;
	player p2;
	player ai;
	int difficulty;
	int win;
	int loss;
} settings;

typedef struct command {
	char name[6];
	struct identifier *identifierList[5];
} command;


typedef struct identifier {
	char name[15];
	struct option *optionList[4];
} identifier;


typedef struct option{
	char name[50];
	char identifierType[15];
	char value[100];
	// struct option *next;
} option;

typedef struct wl {
	short int win;
	short int lose;
} score;

typedef struct gs{
	score p1;
	score p2;
	score ai;
	int gcSingle;
	int gcDouble;
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
/*Reads the commands if the user types "exit" as the initial command */
void parseExit(command *,char [][100], int);



void setOptions(option *); // Changes the game settings according to the command of the user
void applyDefault( void ); // Applies the default settings
void applySettings( void );
void displaySettings( void ); // Displays the game settings

