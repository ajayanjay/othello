#include "replay.h"
#include "menu.h"
#include "storage.h"
#include "ai.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

boolean menuSave(Deque *dequeundo) {
    const char* saveHeader = "Would you like to save replay this match\n";
    const char* saveItems[] = {
        "Yes\n",
        "No\n",
        NULL
    };
    const char* saveSelector = "Press Enter to Select...";

    createDirectory ("replay");
    while (1) {
        int result = menu(saveHeader, saveItems, saveSelector);
        if (result == 0) {

            while (1) {
                clearScreen();
                printf("Enter the file name (max 50 character): ");
                
                char filename[51];
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

                char fullPath[256];
                snprintf(fullPath, sizeof(fullPath), "replay/%s", filename);

                if (isFileExist("replay", filename)){
                    if (!menuoverwrite()){
                        continue; //enter name again
                    }
                }

                //save file and exit
                saveDeque (dequeundo, fullPath);
                break;
            }

            //Always save last game
            saveDeque (dequeundo, "replay/1LastGame");
            return true;
        } else if (result == 1) {
            saveDeque (dequeundo, "replay/1LastGame");
            return false;
        }
    }
}

boolean menuoverwrite (){
    const char* overwriteHeader = "File already exists! Do you want to overwrite?\n";
    const char* overwriteItems[] = {
        "No, enter new name\n",
        "Yes, overwrite\n",
        NULL
    };
    const char* overwriteSelector = "Press enter to select\n";

    int overwriteResult = menu (overwriteHeader, overwriteItems, overwriteSelector);

    if (overwriteResult == 1){
        return true;
    } else {
        return false;
    }
}

void printReplay(const char *fileName){
    Deque replay;
    loadDeque(&replay, fileName);
    if (isDequeEmpty(&replay)){
        return;
    }

    // pre-build: Load all steps from head to tail
    address history[64] = {NULL};
    int totalSteps = 0;
    address current = replay.head;  // start from head
    while (current != NULL && totalSteps < 64) {
        history[totalSteps] = current;
        current = current->next;
        totalSteps++;
    }
    
    if (totalSteps == 0) {
        printf("No valid steps found!\n");
        return;
    }
    
    // REVERSE: because head is the last step, reverse sequence
    address updatedHistory[64] = {NULL};
    for (int i = 0; i < totalSteps; i++) {
        updatedHistory[i] = history[totalSteps - 1 - i];
    }
    
    int pos = 0;
    boolean keepPlaying = true;
    
    while (keepPlaying) {
        if (pos < 0 || pos >= totalSteps) {
            break;
        }
        
        clearScreen();
        Activity act = updatedHistory[pos]->info;
        printf("Step %d of %d (%c):\n", pos+1, totalSteps, act.currentPlayer);
        printBoardArray(act.board, act.currentPlayer, &act.lastMove);
        printf("Use arrow left, arrow right, and ESC\n");

        int input = userInput();
        switch (input) {
            case RIGHT:
                if (pos < totalSteps - 1) {
                    pos++;
                }
                break;
                
            case LEFT:
                if (pos > 0) {
                    pos--;
                }
                break;
                
            case ESC:
                keepPlaying = false;
                break;
                
            default:
                break;
        }
    }
}

void selectReplay (){
    const char* menuReplayHeader = "Select replay you want to see\n";
    const char* replaySelector = "\nPress ENTER to select\n";
    const char* replayDir = "replay";

    int countTotalFile = countFiles (replayDir);

    if (countTotalFile == 0) {
        printf("No replay files found!\n");
        return;
    }

    DIR *d;
    struct dirent *dir;

    char listItem[countTotalFile][64];
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

    listptr[count] = "Back";
    listptr[count+1] = NULL;

    int selected = menu(menuReplayHeader, listptr, replaySelector);

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
        snprintf(path, sizeof(path), "replay/%s", filename);
        printReplay(path);
    }
}