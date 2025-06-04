#include "replay.h"
#include "ai.h"
#include "menu.h"
#include "datastructures/deque.h"
#include "datastructures/stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

// Createdir
#include <sys/stat.h>
#ifdef _WIN32
    #include <direct.h> // mkdir windows
#endif

// for directory storage if not exist
void createDirectory (const char *path) {
    struct stat st = {0}; // get information file
    if (stat(path, &st) == -1) { // -1 if not exist
        #ifdef _WIN32
            _mkdir(path);
        #else 
            mkdir(path, 0700);
        #endif
    }
}

boolean menuSave(char *filename) {
    const char* saveHeader = "Would you like to save replay this match\n";
    const char* saveItems[] = {
        "Yes\n",
        "No\n",
        NULL
    };
    const char* saveSelector = "Press Enter to Select...";

    // if not exist
    createDirectory ("storage");
    createDirectory ("storage/replay");

    while (1) {
        int result = menu(saveHeader, saveItems, saveSelector);
        if (result == 0) {

            while (1) {
                clearScreen();
                printf("Enter the file name (max 50 character): ");
                
                int count = 0;
                char input;

                while (1){
                    input = (char) nonBlockingInput();

                    // Enter pressed and at least one character, out
                    if (input == KEY_ENTER && count > 0) break;

                    if (input == BACKSPC) {
                        if (count > 0) {
                            filename[--count] = '\0';
                            printf("\b \b");
                        }
                        continue;
                    }

                    //check valid characters
                    if (count < 50 && 
                        ((input >= 'a' && input <= 'z') ||
                         (input >= 'A' && input <= 'Z') || 
                         (input >= '0' && input <= '9') ||
                         (input == ' '))) {
                        filename[count++] = input;
                        printf("%c", input);
                    }
                }

                printf("\n");
                filename[count] = '\0';
                break; // Valid input, exit loop
            }
            return true;
        } else if (result == 1) {
            return false;
        }
    }
}

void printReplay(const char *fileName){
    Deque replay = loadDeque(fileName);
    if (isDequeEmpty(&replay)){
        return;
    }

    address history[64] = {NULL};
    int pos = 0;
    address current = replay.head;
    history[0] = current; // Inisialisasi langkah pertama

    while (current != NULL){
        clearScreen();
        Activity act = history[pos]->info;
        printf("Step %d (%c):\n", pos+1, act.currentPlayer);
        printBoardArray(act.board, act.currentPlayer, &act.lastMove);
        printf("Use arrow left, arrow right, and ESC\n");

        int input = userInput();
        switch (input) {
            case RIGHT:{
                if (history[pos+1] != NULL) {
                pos++;
                current = history[pos];
                } else {
                // if never redo, next step
                    if (current->next != NULL) {
                        pos++;
                        current = current->next;
                        history[pos] = current;
                    }
                }
                break;
            }
            case LEFT:{
                if (pos > 0){
                pos--;
                current = history[pos];
                }
                break;
            case ESC:{
                return;
            }
            }
            default:{
                break;
            }
        }
    }
}

int countFiles (const char *directoryPath){
    DIR *d;
    struct dirent *dir;

    int countTotalFile = 0;
    d = opendir(directoryPath);
    if (d) {
        while ((dir = readdir(d)) != NULL){
            if (dir->d_name[0] == '.') continue;
            countTotalFile++;
        }
        closedir(d);
    } else {
        return 0;
    }
    return countTotalFile;
}

void selectReplay (){
    const char* menuReplayHeader = "Select replay you want to see\n";
    const char* replaySelector = "\nPress ENTER to select\n";
    const char* replayDir = "storage/replay";

    // if not exist
    createDirectory ("storage");
    createDirectory ("storage/replay");

    int countTotalFile = countFiles (replayDir);

    if (countTotalFile == 0) {
        printf("No replay files found!\n");
        return;
    }

    DIR *d;
    struct dirent *dir;

    char listItem[countTotalFile][64]; //50 + .txt
    const char *listptr[countTotalFile+1];
    int count = 0;

    d = opendir (replayDir);
    if (d){
        while ((dir = readdir (d)) != NULL){
            if (dir->d_name[0] == '.') continue;
            snprintf(listItem[count], 63, "%s\n", dir->d_name);
            listItem[count][63] = '\0';
            listptr[count] = listItem[count];
            count++;
        }
        closedir(d);
    } else {
        return;
    }

    listptr[count] = NULL;

    int selected = menu(menuReplayHeader, listptr, replaySelector);

    if (selected >= 0 && selected < count) {
        // Hapus newline sebelum membuat path
        char filename[64];
        strncpy(filename, listItem[selected], 63);
        filename[63] = '\0';
        char *newline = strchr(filename, '\n');
        if (newline) *newline = '\0';

        char path[128];
        snprintf(path, sizeof(path), "%s", filename);
        printReplay(path);
    }
}