#include "../include/replay.h"
#include "../include/util/menu.h"
#include "../include/util/storage.h"
#include "../include/ai/ai.h"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

// Validates if character is alphanumeric or space to avoid bug
int isAlphaOrNumberOrSpace(int c) {
    return ((c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9') || 
            (c == ' '));
}

// Displays menu for saving game replay with filename input
boolean saveReplayMenu(Deque * dequeUndo) {

    createDirectory (REPLAY_DIR);

    const char* saveHeader = "Would you like to save replay this match\n\n";
    const char* saveItems[] = {
        "Yes\n",
        "No\n\n",
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
        } else {
            saveDeque (dequeUndo, "gamedata/replays/1LastGame");
            return false;
        }
    }
}

// Shows confirmation menu when attempting to overwrite existing replay file
boolean overwriteReplayMenu (){
    const char* overwriteHeader = "File already exists! Do you want to overwrite?\n";
    const char* overwriteItems[] = {
        "No, enter new name\n",
        "Yes, overwrite\n",
        NULL
    };
    const char* overwriteSelector = "Press enter to select\n";

    switch(menu(overwriteHeader, overwriteItems, overwriteSelector)) {
        case 0: return false; // no, new name
        case 1: return true; // overwrite
        default: return false;
    }
}

// Displays saved replay step by step with navigation controls
void printReplay(const char *fileName){
    Deque replay;
    if (!loadDeque(&replay, fileName) || isDequeEmpty(&replay)){
        return;
    }

    address current = replay.head;
    int pos = 1;

    while (1){
        clearScreen();

        Activity act = current->info;

        if (current->next == NULL)  printf("      End of Game.");
        else                        printf("      Step %d (%c):", pos, act.currentPlayer);

        printBoardArray(act.board, act.currentPlayer, &act.lastMove);
        printf("\n"
           "       Controls:\n"
           " [LEFT] (Previous Step)\n"
           " [RIGHT] (Next Step)\n"
           " [ESCAPE] (Back To Replay Menu)\n\n"
        );
        boolean unnecessaryInput = true;
        while (unnecessaryInput) switch (userInput()) {
            case INPUT_RIGHT:
                if (current->next != NULL) {
                    current = current->next;
                    pos++;
                    unnecessaryInput = false;
                }
                break;

            case INPUT_LEFT:
                if (current->prev != NULL) {
                    current = current->prev;
                    pos--;
                    unnecessaryInput = false;
                }
                break;

            case INPUT_ESCAPE:
                freeDeque(&replay);
                return;
            
            default:
                break;
            
        }
    }
}

// Main menu interface for replay system with play and delete options
void replayMainMenu() {
    const char* replayMenuHeader = "Replay Menu\n\n";
    const char* replayMenuItems[] = {
        "Play Replay\n",
        "Delete Replay\n\n",

        "Back to Main Menu\n",
        NULL
    };
    const char* replayMenuSelector = "\nPress ENTER to select\n";

    while (1) {
        int selected = menu(replayMenuHeader, replayMenuItems, replayMenuSelector);
        
        switch (selected) {
            case 0: // Play Replay
                selectReplays(0);
                break;
                
            case 1: // Delete Replay
                selectReplays(1);
                break;
                
            case 2: // Back to Main Menu
                return;
                
            default:
                return;
        }
    }
}

// Displays list of available replays for selection
void selectReplays(int action){
    const char* menuReplayHeader;
    switch (action) {
        case 0: // Play replay
            menuReplayHeader = "Select replay you want to play\n\n";
            break;
        case 1: // Delete replay
            menuReplayHeader = "Select replay you want to delete\n\n";
            break;
        default:
            return;
    }
    const char* replaySelector = "\nPress ENTER to select\n";

    while (1) {
        // Refresh file list runtime
        int countTotalFile = countFiles (REPLAY_DIR); // Get total count of files in replay directory

        if (countTotalFile == 0) { // No replay files found
            clearScreen();
            printf("No replay files found!\n");
            printf("Press any key to continue...\n");
            inputUntilEnter();
            return;
        }

        DIR *d;
        struct dirent *dir;

        char listItem[countTotalFile][64]; // Array to gathering file names
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

        listPtr[count] = "(Back)\n"; // Add back option
        listPtr[count+1] = NULL;

        int selected = menu(menuReplayHeader, listPtr, replaySelector); // Get selected item from menu

        if (selected == count){
            return; // case back
        }

        if (selected >= 0 && selected < count) {
            // Delete newline before path
            char filename[64];
            strncpy(filename, listItem[selected], 63);
            filename[63] = '\0';
            char *newline = strchr(filename, '\n');
            if (newline) *newline = '\0';
            char path[128];

            switch (action) {
                case 0: // Play replay
                    snprintf(path, sizeof(path), "%s/%s", REPLAY_DIR, filename);
                    printReplay(path);
                    break; // back
                case 1: // Delete replay
                    if (confirmDeleteReplay()) {
                        deleteReplay(filename);
                    }
                    break; // back
                default:
                    return;
            }
        } else {
            return;
        }
    }
}

// Deletes specified replay file from filesystem
void deleteReplay(const char *filename) {
    clearScreen();
    if (filename == NULL || strlen(filename) == 0) {
        return;
    }
    char path[128];
    snprintf(path, sizeof(path), "%s/%s", REPLAY_DIR, filename);
    
    if (removeFile(path) == 0) { // Success delete 
        printf("Replay deleted successfully!\n");
    } else {
        printf("Failed to delete replay file!\n");
    }
    
    printf("Press any key to continue...\n");
    inputUntilEnter();
}

// Shows confirmation dialog before permanently deleting a replay file
boolean confirmDeleteReplay(){
    const char* confirmDeleteHeader = "Are you sure you want to delete this replay?\n\n";
    const char* saveItems[] = {
        "No\n",
        "Yes\n\n",
        NULL
    };
    const char* confirmDeleteSelector = "Press ENTER to confirm\n";
    int selected = menu(confirmDeleteHeader, saveItems, confirmDeleteSelector);
    switch (selected) {
        case 0: //no
            return false; // Do nothing
        case 1: //yes
            return true; // Delete replay file
        default:
            return false;
    }
    return false;
}