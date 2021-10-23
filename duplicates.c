//  CITS2002 Project 2 2021
//  Name(s):             Reiden Rufin (22986337), Nathan Eden (22960674)
//  Student number(s):   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <time.h>
#include <getopt.h>

#include "duplicates.h"
#include "strSHA2.c"

#define OPTLIST "aAf:h:lq"

bool hidden = false;

/**
 * @brief populates a hashtable with filename as key and it's respective strSHA2 as value
 * 
 * @param table the table to store the directory files
 * @param dir the directory to be analyzed
 */
void dir_list(HashTable *table, char *dir) 
{
    check_valid_file(dir); //checks if a valid directory input 
    DIR *dirp = opendir(dir); 
    struct dirent *dp;
    
    while( (dp = readdir(dirp)) != NULL) 
    {
        if(hidden == false) //when -a is not used 
        {
            if(dp->d_name[0] == '.') //checks for hidden files 
            {
                continue;
            }
            char pathname[MAXPATHLEN];
            sprintf(pathname, "%s/%s", dir, dp->d_name); //gets relative pathname
            if(file_or_dir(pathname) == 0) //checks if we are reading a file or a subdirectory 
            {
                dir_list(table, pathname); //recursively call, to keep checking if its a readable file
            }
            else
            {
                table_put(table, pathname, strSHA2(pathname)); //enters filename and its strSHA2 into hash table
            }

        }
        else //functionally the same except we include hidden files 
        {
            char pathname[MAXPATHLEN];
            sprintf(pathname, "%s/%s", dir, dp->d_name);
            int length = strlen(dp->d_name);
            if(length <= 2)
            {
                continue;
            }
            if(file_or_dir(pathname) == 0)
            {
                dir_list(table, pathname);
            }
            else
            {
                table_put(table, pathname, strSHA2(pathname)); 
            }
        }
    }
    closedir(dirp); //close directory 
}

/**
 * @brief Generates a hashtable that only consists of the unique files
 * 
 * @param table the table we're building onto
 * @param dir the directory containing files to be put into
 */
void unique_hash_table(HashTable *table, char *dir)
{
    check_valid_file(dir); 
    
    DIR *dirp = opendir(dir);
    struct dirent *dp;
    
    while( (dp = readdir(dirp)) != NULL)
    {
        if(hidden == false)
        {
            if(dp->d_name[0] == '.') //as hidden is false, we skip all the '.' starting files
            {
                continue;
            }
            char pathname[MAXPATHLEN];
            sprintf(pathname, "%s/%s", dir, dp->d_name);
            if(file_or_dir(pathname) == 0) //Checks if current pathname is a file or a directory
            {
                unique_hash_table(table, pathname);  //If directory, recurisvely call to read that directory
            }
            else
            {
                table_put(table, strSHA2(pathname), pathname); //As our keys are unique, the strSHA2 value will also be unique in,
                                                               // in the final hashtable
            }

        }
        else 
        {
            //similar to hidden = true, except we dont skip over hidden files
            char pathname[MAXPATHLEN];
            sprintf(pathname, "%s/%s", dir, dp->d_name);
            int length = strlen(dp->d_name); 
            if(length <= 2)
            {
                continue;  //Skip the '.' and '..' directories in the dir
            }
            if(file_or_dir(pathname) == 0) 
            {
                unique_hash_table(table, pathname); 
            }
            else
            {
                table_put(table, strSHA2(pathname), pathname);
            }
        }
    }
    closedir(dirp);
}

//-----------------------------------------------------(MAIN)------------------------------------------------------------------------------------
int main(int argc, char*argv[])
{
    if(argc <= 1) 
    {
        usageArguments(argv[0]);  //Prints the correct usage of the program
    }

    char*directory = argv[argc-1]; //The directory be read, is the last one inputted
    HashTable* ht = create_table(SIZE);  //Create our HashTable to store filenames and their strSHA2 values
    HashTable *unqe = create_table(SIZE); //Create our unique HashTables only storing unique files
    unique_hash_table(unqe, directory);  //Populate the unique hashtable
    

    if(argc == 2) //if no other arguments have been given
    {
        dir_list(ht, directory);
        printf("%d\n", number_of_files(ht, directory)); //prints total number of files found
        printf("%d\n", size_of_files(ht, directory)); //prints total size (in bytes) of all files found
        printf("%d\n", unique_files(unqe, directory)); //prints total number of unique files
        printf("%d\n", minimum_size(unqe, directory)); //prints minimum number of total size (subtract duplicate sizes)
        return 0;
    }

    int opt;
    char *filenm = NULL;
    char* sha2hash = NULL;
    opterr = 0;

    bool read = false;
    while((opt = getopt(argc, argv, OPTLIST)) != -1)
    {
        
        if (opt == 'a') //Allows for hidden and configuration files to be considered by duplicates
        {
            hidden = true; 
        }
        else if(opt == 'A') 
        {
            exit(EXIT_FAILURE); //EXIT_FAILURE as program only attempts basic version
        }
        else if(opt == 'f') //Performs the f_filename function if called -l
        {
            read = true;
            if(read)
            {
                dir_list(ht, directory);
                filenm = strdup(optarg); //The filenm becomes the inputted filename after -f filename
        
                if(file_exists(directory, filenm) == 0) 
                {
                    f_filename(ht, filenm);
                    
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    exit(EXIT_FAILURE);
                }
            }
        }
        else if (opt == 'h') //Performs the h_hash function if called -h
        {
            read = true;
            if(read)
            {
                dir_list(ht, directory);
                sha2hash = strdup(optarg);
                h_hash(ht, sha2hash);
            }
        }
        else if(opt == 'l') //Performs the list_duplicate function if called -l
        {
            read = true;
            if(read)
            {
              dir_list(ht, directory);
              list_duplicates(ht, directory);  
            }
        }
        else if(opt == 'q') //Exits quietly, if theere are no duplicates
        {             
            if(minimum_size(ht, directory) == size_of_files(unqe, directory)) //storage is minimized = no duplicates
            {
                exit(EXIT_SUCCESS);
            }
            else
            {
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            usageCommandLine(); //Prints the correct way to use the commands if an invalid command is given
        }
    }
    dir_list(ht, directory); //Fill the table with key/values of the directory
    
    if(hidden == true && argc == 3) //if -a has been called and only the directory is given with no other commands
    {
        HashTable* uniqe_hidden = create_table(SIZE);
        unique_hash_table(uniqe_hidden, directory);
        printf("%d\n", number_of_files(ht, directory)); //prints total number of files found
        printf("%d\n", size_of_files(ht, directory)); //prints total size (in bytes) of all files found
        printf("%d\n", unique_files(uniqe_hidden, directory)); //prints total number of unique files
        printf("%d\n", minimum_size(uniqe_hidden, directory)); //prints minimum number of total size (subtract duplicate sizes)
        return 0;
    }
    
    read = true;
    printf("\n");
    return 0;
}