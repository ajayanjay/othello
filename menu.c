#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>
#else
	#include <termios.h>
	#include <unistd.h>
#endif

int menu(const char *menuHeader, const char **menuItems, const char *menuFooter)
{
	int cursor = 0, action;
	int n = getMenuItemsLength(menuItems);

	while (1) 
	{
		clearScreen();

		printf("%s", menuHeader);
		for (int i = 0; i < n; ++i) {
			if (i == cursor) printf("> %s", menuItems[i]);
			else printf("%s", menuItems[i]);
		}
		printf("%s", menuFooter);

		while ((action = menuInput(&cursor, 0, n - 1)) == UNNECESSARY_INPUT);

		if (action != CURSOR_MOVE) return cursor;

	}
}

int nonBlockingInput()
{
	#ifdef _WIN32
		return getch();
	#else
		struct termios oldt, newt;
		int ch;
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		ch = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		return ch;
	#endif
	return -1;
}

void inputUntilChar(char c)
{
	while (nonBlockingInput() != c);
}

void inputUntilEnter()
{
	while (nonBlockingInput() != KEY_ENTER);
}

Input userInput()
{
	switch (nonBlockingInput())
	{
		#ifdef _WIN32
			case 0: case 224:
				switch(nonBlockingInput()) 
				{
					case 72: return UP;
					case 75: return LEFT;  
					case 80: return DOWN;
					case 77: return RIGHT;
					default: return NUL;
				}
		#else
			case 27:
				switch(nonBlockingInput()) 
				{
					case 91:
						switch(nonBlockingInput()) 
						{
							case 65: return UP;
							case 68: return LEFT;  
							case 66: return DOWN;
							case 67: return RIGHT;
							default: return NUL;
						}
					default:
						return NUL;
				}
		#endif

		case KEY_ENTER:
			return ENTER;

		// WASD movement
		case 'w': case 'W':
			return UP;

		case 'a': case 'A':
			return LEFT;

		case 's': case 'S':
			return DOWN;

		case 'd': case 'D':
			return RIGHT;

		case 'z': case 'Z':
			return KEY_Z;
		
		case 'y': case 'Y':
			return KEY_Y;

		default: 
			return NUL;
	}
}

void clearScreen() {
	#ifdef _WIN32
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coordScreen = { 0, 0 };
		DWORD dwCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		/* Get the number of cells in the current buffer */
		GetConsoleScreenBufferInfo(hStdOut, &csbi);
		DWORD dwSize = (DWORD) (csbi.dwSize.X * csbi.dwSize.Y);

		/* Fill the entire buffer with spaces */
		FillConsoleOutputCharacter(hStdOut, ' ', dwSize, coordScreen, &dwCharsWritten);

		/* Move the cursor home */
		SetConsoleCursorPosition(hStdOut, coordScreen);
	#else
		// For other platforms, you can use ANSI escape codes
		system("clear");
	#endif
}

int getMenuItemsLength(const char **menuItems) 
{
	int count = -1;
	while (menuItems[++count] != NULL);
	return count;
}

int menuInput(int *item, int minSize, int maxSize)
{
	switch(userInput())
	{
		case UP: case LEFT:
			if (isAtFront(*item, minSize)) return UNNECESSARY_INPUT;
			return moveCursor(&*item, PREVIOUS);

		case DOWN: case RIGHT:
			if (isAtEnd(*item, maxSize)) return UNNECESSARY_INPUT;
			return moveCursor(&*item, NEXT);

		case ENTER:
			return ENTER;

		default:
			return UNNECESSARY_INPUT;
	}
}

int moveCursor(int *item, int direction)
{
	if (direction == NEXT)
		++(*item);
	else
		--(*item);

	return CURSOR_MOVE;
}

int isAtEnd(int item, int maxSize) {
	return item >= maxSize;
}

int isAtFront(int item, int minSize) {
	return item <= minSize;
}