#ifndef menu_h
#define menu_h

#define UNNECESSARY_INPUT NUL
#define CURSOR_MOVE 32

#define PREVIOUS UP
#define NEXT DOWN

#define KEY_ESC 27

#ifdef _WIN32
	#define KEY_ENTER 13
	#define BACKSPC 8
#else
	#define KEY_ENTER 10
	#define BACKSPC 127
#endif

typedef enum input {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ENTER,
	KEY_Z,
	KEY_Y,
	NUL,
	ESC
} Input ;

int menu(const char* menuHeader, const char* menuItems[], const char* menuFooter);
// both requires NULL at the end of element at menuItems.
/* Usage Example

	menuHeader = 	"\nWelcome to X\n\n";
	menuItems[] = {
					"Play Game\n", 
					"Continue\n",
					"Settings\n",
					"How To Play\n\n",

					"Exit\n\n", NULL
	};

	menuFooter =	"Enter Your Choice: "

	switch (menu(menuHeader, menuItems, menuFooter)) {
	
		case 0: playGame();  break;
		case 1: continue();  break;
		case 2: settings();	 break;
		case 3: howToPlay(); break;
		case 4: return 0;

	}
*/

// input tanpa harus enter dulu.
int nonBlockingInput();

// biarin user input apapun, berhenti kalo char c di input.
void inputUntilChar(char c);

// biarin user input apapun, berhenti kalo enter di input.
void inputUntilEnter();

// konversi input dari user ke enum input_e.
Input userInput();

// input string dengan batasan karakter yang diizinkan dan modifikasi karakter.
void inputLimitedString(char * buffer, int minSize, int maxSize, int (*isCharAllowed)(int), int (*modifyChar)(int));

// bersihkan layar
void clearScreen();

// kbhit.
int keyboardHit();

// INSIDER FUNCTIONS. should not to be used outside this file.

// move cursor to next or previous item. or execute action. or not at all.
int menuInput(int *item, int minSize, int maxSize);

// move cursor to next or previous item.
int moveCursor(int *item, int direction);

// get the length of menuItems.
int getMenuItemsLength(const char* menuItems[]);

// check if the cursor is at the end of the menu.
int isAtEnd(int item, int maxSize);

// check if the cursor is at the front of the menu.
int isAtFront(int item, int minSize);

#endif