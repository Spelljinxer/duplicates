//  CITS2002 Project 2 2021
//  Name(s):             Hidden for Privacy.
//  Student number(s):  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>
#include <sys/stat.h>


#define CHECK_ALLOC(p) if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }

#define SIZE MAXPATHLEN //size of the hash

//----------------------------------------(HASH.C FUNCTIONS)---------------------------------------------------------
typedef struct hash_item hash_item;
 
struct hash_item
{
    char* key;
    char* value;
};
 
typedef struct HashTable HashTable;
 
struct HashTable 
{
    hash_item** items;
    int size;
    int count;
};

extern unsigned long get_hash(char* str);
 
extern hash_item* new_item(char* key, char* value);
extern HashTable* create_table(int size);
 
extern void free_item(hash_item* item);
extern void free_table(HashTable* table);
 
extern void table_put(HashTable* table, char* key, char* value);
extern char* table_get(HashTable* table, char* key);

//---------------------------------------(SUPPORT.C FUNCTIONS)------------------------------------------------------------

extern int file_exists(const char *path, const char *filename);
extern int file_or_dir(const char *path);

extern void usageInvalidDirectory();
extern void usageCommandLine();
extern void usageArguments(char* arg);
extern void check_valid_file(char* dir);


//---------------------------------------(DIRECTORY.C FUNCTIONS)------------------------------------------------------------

extern int number_of_files(HashTable* table, char *dir);
extern int size_of_files(HashTable *table, char *dir);
extern int unique_files(HashTable *table, char* dir);
extern int minimum_size(HashTable *table,  char*dir);
extern void list_duplicates(HashTable *table, char *dir);
extern void f_filename(HashTable *ht, char* filenm);
extern void h_hash(HashTable *ht, char* sha2hash);
