//  CITS2002 Project 2 2021
//  Name(s):            Hidden for Privacy.
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

/**
 * @brief Returns the number of files in a directory
 * 
 * @param table As we're storing all files into a hashtable, simply use it's count
 * @param dir directory we're storing
 * @return the total number of files by getting the count of @param table
 */
int number_of_files(HashTable* table, char *dir) 
{
  check_valid_file(dir);
  return table->count;
}

/**
 * @brief Returns the total size of all files in a directory
 * 
 * @param table to store the directory
 * @param dir to be analyzed
 * @return total size of of all files
 */
int size_of_files(HashTable *table, char *dir)
{
  check_valid_file(dir);
  int size = 0;
  int index = 0;
  while(index < table->size) //while we're still in the table
  {
      if(table->items[index]) //Check if that current index possesses an item from table 
      {
          char*key = table->items[index]->key; //Get the key (filename) from the table
          struct stat st;
          stat(key, &st);
          int fileSize = st.st_size; //We want to declare here as we want each size of key
          size += fileSize; //Increment the size
      }
      index++;
  }
  return size;
}

/**
 * @brief Returns the total number of unique files (non-duplicates, duplicates counted once)
 *          
 * @param table the unique hash table containing only unique files
 * @param dir directory to be stored
 * @return the total count of the unique hash table = number of unique files
 */
int unique_files(HashTable *table, char* dir)
{
    check_valid_file(dir);
    return table->count;
}

/**
 * @brief returns the minimum size of a directory (original size - duplicate sizes)
 *          - Similarly to size_of_files, but instead we only use the unique hashtable
 * @param table the unique hashtable
 * @param dir to be stored
 * @return total size of all the uniques which, does not have duplicates
 */
int minimum_size(HashTable *table,  char*dir)
{
    check_valid_file(dir);
    int size = 0;
    int index = 0;
    while(index < table->size) //While we're still in the table
    {
        if(table->items[index]) //check if there is an item now
        {
            char*value = table->items[index]->value; //get the value (in this case, our filename)
            struct stat st;
            stat(value, &st);
            int fileSize = st.st_size; //gets each size of each filename
            size += fileSize; //adds that size onto our total size
        }
        index++;
    }
    return size;
}

/**
 * @brief Algorithm that lists all the possible duplicates
 *          - Each line consists of the relative pathnames of two or more files that are duplicates
 *          - Duplicate files are separated by a tab character
 *          - Activated when -l is called
 * @param table the table that stored all our filenames
 * @param dir directory to be analyzed
 */
void list_duplicates(HashTable *table, char *dir)
{
    bool dup[table->size];  //boolean array to check if duplicated
    bool visited[table->size]; //boolean array to keep track of visited
    for (int i = 0; i<=table->size;++i) //initialise bool arrays
    { 
        visited[i] = false;
        dup[i] = true;
    }

    for (int a = 0; a < table->size; ++a) //visiteds if a file has duplicates
    { 
        int dupCount = 0; //keeps track of how many duplicates a file has 
        if(table->items[a])
        {
            char*str = table->items[a]->value;
            for (int b = 0; b < table->size; ++b)
            {
                if(table->items[b])
                {
                    char*str2 = table->items[b] -> value;
                    if(strcmp(str, str2) == 0) //increments dupCount when a duplicate file is found 
                    { 
                        dupCount++;
                    }
                }
            }
        }
        if (dupCount == 1) //if a file doesn't have duplicates dupCount == 1 
        { 
            dup[a] = false; //marks a file to not have any duplicates 
        }
    }

    for(int i = 0; i < table->size; ++i)
    { 
        if(table->items[i] && visited[i] == false && dup[i] != false) //if a file exists, hasn't been seen and has duplicates
        { 
            char*currentFileName = table->items[i]->key; 
            char*currentValue = table->items[i]->value;
            printf("%s\t", currentFileName);
            visited[i] = true;
            for(int j = 0; j<= table->size; ++j)
            {
                if (table->items[j])
                {
                    char*currentFileName2 = table->items[j] -> key; //saves filename
                    char*currentValue2 = table->items[j] -> value; //saves hash value
                    if (strcmp(currentValue, currentValue2) == 0 && visited[j] == false) //finds duplicates and prints them
                    { 
                        printf("%s\t", currentFileName2);
                        visited[j] = true;
                    }
                }
            }
            printf("\n"); //prints the new line when all duplicates have been printed
        }
    }
}

/**
 * @brief Finds and lists, one per line, the relative pathnames of all files whose SHA2 hash matches
 *           - Name of indicated file is not listed
 *           - Activated when -f filename is called
 * @param ht the table where we stored our directory read
 * @param filenm the filename
 */
void f_filename(HashTable *table, char* filenm)
{
    char*hash = table_get(table, filenm); //Get the strSHA2 hash of the inputted filename
    int index = 0;
    while(index < table->size)  //While we're still in our table
    {
        if(table->items[index])  //if we found an item
        {
            char*currentFileName = table->items[index]->key; //get the filename
            char*currentValue = table->items[index]->value; //get the strSHA2 hash
            
            //if the two hashes are the same, and, if the filenames are different, then compare
            if(strcmp(hash,currentValue) == 0 && (strcmp(currentFileName, filenm) != 0)) 
            {
                printf("%s\n", currentFileName); 
            }
        }
        index++;
    }
}

/**
 * @brief Find and list, one per line, the relative pathnames of all files with the hash
 * 
 * @param ht the table we're getting our hashes from
 * @param sha2hash the hash of to be compared to
 */
void h_hash(HashTable *ht, char* sha2hash)
{
    int index = 0;
    int match = 0;  //Variable to check if a hash has been matched
    while (index < ht->size) 
    {
        if (ht->items[index]) //If we have found an item in our table
        {
            char*currentFileName = ht->items[index] -> key; //Retrieve the filename
            char*currentValue = ht->items[index] -> value; //Retrieve the value (hash)
            if (strcmp(sha2hash, currentValue) == 0) 
            {
                printf("%s\n", currentFileName); //if both hashes are the same, print out the filename
                match++;  //if we have found something that matches, 
            }
        }
        index++;
    }
    if (match != 0)
    {
        exit(EXIT_SUCCESS); //EXIT_SUCCESS if we have found something that matches
    }
    else
    {
        exit(EXIT_FAILURE); //EXIT_FAILURE if we have not found anything
    }
}
