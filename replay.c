#include "replay.h"
#include "ai.h"
#include "menu.h"
#include "datastructures/deque.h"
#include "datastructures/stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

boolean menuSave(char *filename) {
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
            clearScreen();
            while (1) {
                printf("Enter the file name: ");
                fgets(filename, 100, stdin);
                filename[strcspn(filename, "\n")] = 0; // delete newline

                int len = strlen(filename);
                int valid = 1;
                for (int i = 0; i < len ; ++i) {
                    if (!((filename[i] >= 'a' && filename[i] <= 'z') ||
                        (filename[i] >= 'A' && filename[i] <= 'Z') || 
                        (filename[i] >= '0' && filename[i] <= '9'))) {
                        valid = 0;
                        break;
                    }
                }
                if (!valid || len == 0) {
                    printf("Terdapat huruf yang tidak diperbolehkan! Silakan ulangi input.\n");
                    continue;
                }
                break;
            }
            return true;
        } else if (result == 1) {
            return false;
        }
        // if input not valid, return to menu
    }
}


void printReplay(const char *fileName){
    Deque replay = loadDeque(fileName);
    if (isDequeEmpty(&replay)){
        return;
    }

    address history[60] = {NULL};
    int pos = 0;
    address current = replay.head;
    history[0] = current; // Inisialisasi langkah pertama

    while (current != NULL){
        Activity act = history[pos]->info;
        printf("Step %d (%c):\n", pos+1, act.currentPlayer);
        printBoardArray(act.board, act.currentPlayer, &act.lastMove);

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
            }
            default:{
                break;
            }
        }
    }
}

void selectReplay (){
    const char* menuReplayHeader = "Select replay you want to see\n";
    const char* replaySelector = "\nPress ENTER to select\n";

    DIR *d;
    struct dirent *dir;

    int countTotalFile = 0;
    d = opendir("storage/replay");
    if (d) {
        while ((dir = readdir(d)) != NULL){
            if (dir->d_name[0] == '.') continue;
            countTotalFile++;
        }
        closedir(d);
    } else {
        return;
    }

    char listItem[countTotalFile][256];
    const char *listptr[65];
    int count = 0;

    d = opendir ("storage/replay");
    if (d){
        while ((dir = readdir (d)) != NULL){
            if (dir->d_name[0] == '.') continue;
            snprintf(listItem[count], 255, "%s\n", dir->d_name);
            listItem[count][255] = '\0';
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
        char filename[256];
        strncpy(filename, listItem[selected], 255);
        filename[255] = '\0';
        char *newline = strchr(filename, '\n');
        if (newline) *newline = '\0';

        char path[256];
        snprintf(path, sizeof(path), "storage/replay/%s.txt", filename);
        printReplay(path);
        inputUntilEnter();
    }
}