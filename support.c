//  CITS2002 Project 2 2021
//  Name(s):             Hidden for Privacy.
//  Student number(s):

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "duplicates.h"

/**
 * @brief Checks if the given file exists in the path
 * 
 * @param path the pathname where we are checking
 * @param filename the filename that we're checking if exists
 * @return 0 if it does exist, 1 otherwise
 */
int file_exists(const char *path, const char *filename)
{
    char *file= malloc(strlen(path) + strlen(filename) + 1);
    CHECK_ALLOC(file);
    strcpy(file, path);

    strcat(file, filename);
    int exists = access(file, F_OK) == 0;
    free(file);

    return exists;
}

/**
 * @brief Checks whether the given path is a path or a file
 * 
 * @param path the path being analyzed
 * @return 0 if it is a path, 1 if it is a file
 */
int file_or_dir(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void usageInvalidDirectory()
{
    perror("Please use a valid directory");
    exit(EXIT_FAILURE);
}
void usageCommandLine()
{
    printf("Invalid Arguments were given. Accepted arguments: {-a, -A, -f filename, -h hash, -l, -q}\n");
    exit(EXIT_FAILURE);
}
void usageArguments(char* arg)
{
    fprintf(stderr, "Please follow the Correct Usage: %s DirectoryName\n", arg);
    exit(EXIT_FAILURE);
}
/**
 * @brief Checks if the given file can be read
 * Catches early termination
 * @param dir to be analyzed
 */
void check_valid_file(char* dir)
{
    DIR *dirp = opendir(dir);

    if(dirp == NULL)
    {
        usageInvalidDirectory();
    }
}
