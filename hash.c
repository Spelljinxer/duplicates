//  CITS2002 Project 2 2021
//  Name(s):             Hidden for Privacy.
//  Student number(s):

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/param.h>
#include "duplicates.h"

/**
 * @brief Returns the specific hashcode from a given key or value
 * 
 * @param str the key/value being analyzed
 * @return the hasCode of @param str
 */
unsigned long get_hash(char* str) 
{
    unsigned long i = 0;
    for(int j = 0; str[j]; j++)
    {
        i += str[j];
    }
    return i % SIZE;
}
 
/**
 * @brief creates an item to be put into the hashtable
 * 
 * @param key which key we're putting it in
 * @param value the value we're putting at @param key
 * @return a new item to the hashtable
 */
hash_item* new_item(char* key, char* value) {
    // Creates a pointer to a new hash table item
    hash_item* item = (hash_item*) malloc (sizeof(hash_item));
    CHECK_ALLOC(item);
    item->key = (char*) malloc (strlen(key) + 1); 
    CHECK_ALLOC(item->key);
    item->value = (char*) malloc (strlen(value) + 1);  //dynamically allocate memory to the key and values
    CHECK_ALLOC(item->value);   //be sure to check that it is not null
     
    strcpy(item->key, key); //copy the input key to the hashtable
    strcpy(item->value, value); //copy the input value to the hastable
 
    return item;
}
 
/**
 * @brief Creates a new HashTable with a given size
 * 
 * @param size the size of the hashtable
 * @return a newly created Hashtable
 */
HashTable* create_table(int size) {
    // Creates a new HashTable
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    CHECK_ALLOC(table);
    table->size = size;
    table->count = 0;
    table->items = (hash_item**) calloc (table->size, sizeof(hash_item*));
    CHECK_ALLOC(table->items);
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;
 
    return table;
}

/**
 * @brief deallocate the memory on item
 * 
 * @param item to be freed
 */
void free_item(hash_item* item) {
    free(item->key);
    free(item->value);
    free(item);
}
 
/**
 * @brief deallocate the memory on table
 * 
 * @param table to be freed
 */
void free_table(HashTable* table) {
    
    for (int i=0; i<table->size; i++) 
    {
        hash_item* item = table->items[i]; 
        
        if (item != NULL)
        {
            free_item(item);
        }
    }
 
    free(table->items);
    free(table);
}
 
 /**
  * @brief Inserts an item into a table.
  * 
  * @param table to be inserted onto
  * @param key to be inserted onto the table
  * @param value to be inserted onto the key
  */
void table_put(HashTable* table, char* key, char* value) 
{
    hash_item* item = new_item(key, value);

    
    unsigned long index = get_hash(key); //find out what index to be inserted onto
 
    hash_item* current_item = table->items[index];
     
    if (current_item == NULL) //we want to insert onto something that is not null
    {
        if (table->count == table->size)  //if the current key does not exist then it must be full
        {
            free_item(item); // Remove the create item
            return;
        }
        
        table->items[index] = item; //insert directly onto the table
        table->count++;   //icrement the count to keep track of how many items
    }
 
    else 
    {
            if (strcmp(current_item->key, key) == 0) 
            {
                strcpy(table->items[index]->value, value);   //just replace the current value with new value
                return;
            }
    }
}

/**
 * @brief Retrieves the value from a given key
 * 
 * @param table to be analyzed
 * @param key where are we retrieving the value
 * @return a char* value that is the value of @param key, NULL otherwise
 */
char* table_get(HashTable* table, char* key) {
    int index = get_hash(key); //get the specific index of that key
    hash_item* item = table->items[index];
    
    if (item != NULL) //item must not be null otherwise we just retrive NULL
    {
        if (strcmp(item->key, key) == 0)
        {
            return item->value;
        }
    }
    return NULL;
}
 
