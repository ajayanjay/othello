#include "../../include/util/storage.h"

// Createdir
#include <sys/stat.h>
#include <unistd.h>
#include "dirent.h"
#include <stdlib.h>
#include <stdio.h>

// Creates directory if it doesn't exist
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

// Checks if file exists in specified directory path
boolean isFileExist (const char *path, const char *filename) {
    char fullPath[1024];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, filename);

    FILE *file = fopen(fullPath, "r");
    if (file) {
        fclose(file);
        return true;  // File exists
    }
    return false; // File does not exist
}

boolean isDirectoryExist (const char *path) {
    return stat(path, &(struct stat) {0}) == 0;
}

// Counts total number of files in directory excluding hidden files
int countFiles (const char *directoryPath){
    DIR *d;
    struct dirent *dir;

    int countTotalFile = 0;
    d = opendir(directoryPath);
    if (d) {
        while ((dir = readdir(d)) != NULL){
            if (dir->d_name[0] == '.') continue;
            countTotalFile++; // Increment count for file
        }
        closedir(d);
    } else {
        return 0;
    }
    return countTotalFile; // Return total count of files
}

// Removes specified file from system
int removeFile (const char *filePath) {
    return remove(filePath); // Delete file
}