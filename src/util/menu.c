#include "../../include/util/menu.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>
#else
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
#endif

// All functions in this file were authored by Azzar

int menu(const char *menuHeader, const char **menuItems, const char *menuFooter)
{
	int cursor = 0, action;
	int n = getMenuItemsLength(menuItems);

	char buffer[64 * (n + 2)];
	int offset = 0;

	while (1) 
	{
		clearScreen();
		buffer[0] = 0; offset = 0;

		offset += sprintf(buffer + offset, "%s", menuHeader);
		int i;
		for (i = 0; i < n; ++i) {
			if (i == cursor) offset += sprintf(buffer + offset ,"> %s", menuItems[i]);
			else offset += sprintf(buffer + offset, "%s", menuItems[i]);
		}
		offset += sprintf(buffer + offset, "%s", menuFooter);

		printf("%s", buffer);

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
					case 72: return INPUT_UP;
					case 75: return INPUT_LEFT;  
					case 80: return INPUT_DOWN;
					case 77: return INPUT_RIGHT;
					default: return NUL;
				}

			case KEY_ESC:
				return INPUT_ESCAPE;
		#else
			case 27:
				if (keyboardHit()) {
					switch(nonBlockingInput()) 
					{
						case 91:
							switch(nonBlockingInput()) 
							{
								case 65: return INPUT_UP;
								case 68: return INPUT_LEFT;  
								case 66: return INPUT_DOWN;
								case 67: return INPUT_RIGHT;
								default: return NUL;
							}
						default:
							return NUL;
					}
				} else
					return INPUT_ESCAPE;
		#endif

		case KEY_ENTER:
			return INPUT_ENTER;

		// WASD movement
		case 'w': case 'W':
			return INPUT_UP;

		case 'a': case 'A':
			return INPUT_LEFT;

		case 's': case 'S':
			return INPUT_DOWN;

		case 'd': case 'D':
			return INPUT_RIGHT;

		case 'z': case 'Z':
			return INPUT_Z;
		
		case 'y': case 'Y':
			return INPUT_Y;

		default: 
			return NUL;
	}
}

void inputLimitedString(char * buffer, int minSize, int maxSize, int (*isCharAllowed)(int), int (*modifyChar)(int)) {
	int count = 0;
	int input;

	while (1) {
		input = nonBlockingInput();

		if (input == KEY_ENTER && count >= minSize) break;

		if (input == KEY_BACKSPACE) {
			if (count > 0) {
				buffer[--count] = '\0';
				printf("\b \b");
			}
			continue;
		}

		if (count < maxSize && (isCharAllowed ? isCharAllowed(input) : 1)) {
			buffer[count] = modifyChar ? modifyChar(input) : input;
			printf("%c", buffer[count]);
			count++;
		}
	}

	buffer[count] = '\0';
	printf("\n");
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

int keyboardHit() {
	#ifdef _WIN32
		return kbhit();
	#else
		struct termios oldt, newt;
		int ch;
		int oldf;

		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

		ch = getchar();

		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		fcntl(STDIN_FILENO, F_SETFL, oldf);

		if (ch != EOF) {
			ungetc(ch, stdin);
			return 1;
		}

		return 0;
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
		case INPUT_UP: case INPUT_LEFT:
			if (isAtFront(*item, minSize)) return UNNECESSARY_INPUT;
			return moveCursor(&*item, PREVIOUS);

		case INPUT_DOWN: case INPUT_RIGHT:
			if (isAtEnd(*item, maxSize)) return UNNECESSARY_INPUT;
			return moveCursor(&*item, NEXT);

		case INPUT_ENTER:
			return INPUT_ENTER;

		case INPUT_ESCAPE:
			*item = maxSize;
			return INPUT_ENTER;

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