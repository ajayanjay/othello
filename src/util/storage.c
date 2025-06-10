#include "../../include/util/storage.h"
#include "dirent.h"
#include <stdlib.h>
#include <stdio.h>

// Createdir
#include <sys/stat.h>
#include <unistd.h>


// for directory storage if not exist
void createDirectory (const char *path) {
    struct stat st = {0}; // get information file
    if (stat(path, &st) == -1) { // -1 if not exist
        #ifdef _WIN32
            mkdir(path);
        #else 
            mkdir(path, 0700);
        #endif
    }
}

int removeDirectory (const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1)
        return 0;
    
    
    #ifdef _WIN32
        return rmdir(path);
    #else 
        return rmdir(path);
    #endif
}

boolean isFileExist (const char *path, const char *filename) {
    char fullPath[1024];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, filename);

    FILE *file = fopen(fullPath, "r");
    if (file) {
        fclose(file);
        return true;  // File exists
    }
    return false;
}

boolean isDirectoryExist (const char *path) {
    return stat(path, &(struct stat) {0}) == 0;
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