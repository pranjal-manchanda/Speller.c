// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

//Define node structure with word and pointer to next node
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//Default hash size (can be manipulated to adjust performance/memory usage)
#define HASH_SIZE 5381

//Array of nodes to create a chained hash table
node *hashTable[HASH_SIZE];

//Global variable to keep track of size of file
unsigned int word_count = 0;

//Function to return a hash value
//Using the djb2 hash function by Dan Bernstein
unsigned int hashFunc(char *str)
{
    unsigned long hash = 5381;
    int c = 0;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % HASH_SIZE;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int len = strlen(word);
    char lower_case[len + 1];

    //Convert string to all lower case
    for (int i = 0; i < len; i++)
    {
        lower_case[i] = tolower(word[i]);
    }
    lower_case[len] = '\0';

    //Get index from hash function
    unsigned int index = hashFunc(lower_case);

    //create traversal pointer at the index from hash function
    node *trav = hashTable[index];

    //Traverse the node untill we reach null pointer
    while (trav != NULL)
    {
        //if words match
        if (strcmp(trav->word, lower_case) == 0)
        {
            return true;
        }
        //else move onto next word
        else
        {
            trav = trav->next;
        }
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //Set array to all NULL values
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashTable[i] = NULL;
    }

    //Open file for reading and check if it openend properly
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        printf("File could not be opened");
        return false;
    }

    while (true)
    {
        //Dynamically allocate memory for new node
        node *new_node = malloc(sizeof(node));

        //If new node could not be created, exit
        if (!new_node)
        {
            return false;
        }

        //Add values to new node
        fscanf(file, "%s", new_node->word);
        new_node->next = NULL;

        //If end of file is reached
        if (feof(file))
        {
            free(new_node);
            break;
            //free node and exit
        }
        word_count++;//Increment word count after each successful addition

        //Get index from hash function
        unsigned int index = hashFunc(new_node->word);

        node *check = hashTable[index];

        //If index location is empty, insert new node
        if (check == NULL)
        {
            hashTable[index] = new_node;
        }

        //Else new node points to the current index location and becomes the index location itself
        else
        {
            new_node->next = hashTable[index];
            hashTable[index] = new_node;
        }

    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count; //Global variable updated in load function
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //Free all nodes
    for (int i = 0; i < HASH_SIZE; i++)
    {
        node *trav = hashTable[i];
        while (trav != NULL)
        {
            node *temp = trav;
            trav = trav->next;
            free(temp);
        }
    }
    return true;
}