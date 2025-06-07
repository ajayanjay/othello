#include "../include/replay.h"
#include "../include/util/menu.h"
#include "../include/util/storage.h"
#include "../include/ai/ai.h"
#include "../include/datastructure/stack.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int isAlphaOrNumberOrSpace(int c) {
    return ((c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9') || 
            (c == ' '));
}

boolean saveReplayMenu(Deque * dequeUndo) {

    createDirectory (REPLAY_DIR);

    const char* saveHeader = "Would you like to save replay this match\n";
    const char* saveItems[] = {
        "Yes\n",
        "No\n",
        NULL
    };
    const char* saveSelector = "Press Enter to Select...";

    while (1) {
        int result = menu(saveHeader, saveItems, saveSelector);
        if (result == 0) {

            while (1) {
                clearScreen();
                printf("Enter the file name (max 24 characters): ");
                
                char filename[25];
                inputLimitedString(filename, 1, 24, isAlphaOrNumberOrSpace, NULL);

                char fullPath[64];
                snprintf(fullPath, sizeof(fullPath), "gamedata/replays/%s", filename);

                if (isFileExist("gamedata/replays", filename)){
                    if (!overwriteReplayMenu()){
                        continue; //enter name again
                    }
                }

                //save file and exit
                saveDeque (dequeUndo, fullPath);
                break;
            }

            //Always save last game
            saveDeque (dequeUndo, "gamedata/replays/1LastGame");
            return true;
        } else if (result == 1) {
            saveDeque (dequeUndo, "gamedata/replays/1LastGame");
            return false;
        }
    }
}

boolean overwriteReplayMenu (){
    const char* overwriteHeader = "File already exists! Do you want to overwrite?\n";
    const char* overwriteItems[] = {
        "No, enter new name\n",
        "Yes, overwrite\n",
        NULL
    };
    const char* overwriteSelector = "Press enter to select\n";

    switch(menu(overwriteHeader, overwriteItems, overwriteSelector)) {
        case 0: return false;
        case 1: return true;
        default: return false;
    }
}

void printReplay(const char *fileName){
    Deque replay;
    loadDeque(&replay, fileName);
    if (isDequeEmpty(&replay)){
        return;
    }

    address history[64] = {NULL};
    int pos = 0;
    address current = replay.tail;
    history[0] = current; // Inisialisasi langkah pertama

    while (current != NULL){
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
                    if (current->prev != NULL) {
                        pos++;
                        current = current->prev;
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

void selectReplays(){
    const char* menuReplayHeader = "Select replay you want to see\n\n";
    const char* replaySelector = "\nPress ENTER to select\n";

    int countTotalFile = countFiles (REPLAY_DIR);

    if (countTotalFile == 0) {
        printf("No replay files found!\n");
        return;
    }

    DIR *d;
    struct dirent *dir;

    char listItem[countTotalFile][64];
    const char *listPtr[countTotalFile + 2];
    int count = 0;

    d = opendir (REPLAY_DIR);
    if (!d) return;

    while ((dir = readdir (d)) != NULL){
        if (dir->d_name[0] == '.') continue;
        snprintf(listItem[count], 261, "%s\n", dir->d_name);
        listItem[count][63] = '\0';
        listPtr[count] = listItem[count];
        count++;
    }
    closedir(d);

    listPtr[count] = "(Back To Main Menu)\n";
    listPtr[count+1] = NULL;

    while (1) {
        int selected = menu(menuReplayHeader, listPtr, replaySelector);

        if (selected == count){
            return; // case back
        }

        if (selected >= 0 && selected < count) {
            // Hapus newline sebelum membuat path
            char filename[64];
            strncpy(filename, listItem[selected], 63);
            filename[63] = '\0';
            char *newline = strchr(filename, '\n');
            if (newline) *newline = '\0';

            char path[128];
            snprintf(path, sizeof(path), "gamedata/replays/%s", filename);
            printReplay(path);
        }
    }
}