// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

//////////////////////////////////////////////////////
int wordHash;
int counter = 0;
bool loaded = false;
/////////////////////////////////////////////////////

// size of hashtable
#define SIZE 65536

//create nodes for linked list
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

//create hashtable
node *hashtable[SIZE] = {NULL};

//create hash function
int hash (const char* word)
{
    unsigned int hash = 0;

    for(int i=0, n=strlen(word); i<n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }

    return hash % SIZE;
}

///////////////////////////////////////////////////////
// Returns true if word is in dictionary else false///
bool check(const char *word)
{
    //create temp variable that stores lower case version of word
    char temp[LENGTH + 1];
    int len = strlen(word);


    for (int i = 0; i < len; i++)
    {
        temp[i] = tolower(word[i]);
    }

    temp[len] = '\0';

    //hash temp and point head to it
    wordHash = hash(temp);

    //if hashtable is null at the index node return false
    if(hashtable[wordHash] == NULL)
    {
        return false;
    }

    node *cursor = hashtable[wordHash];

    //go through nodes, if word is spelt correctly return true
    while (cursor != NULL)
    {
        if(strcmp(temp, cursor->word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }
    return false;
}

///////////////////////////////////////////////////////////
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary
    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // create array for word to be stored
    char word[LENGTH + 1];

    //scan through dictionary and load each word into hash table
    while (fscanf(dict, "%s", word) != EOF)
    {
        // allocate memory for newWord
        node *new_node = malloc(sizeof(node));

        if (new_node == NULL)
        {
            printf("could not malloc a new node.\n");
            fclose(dict);
            return false;
        }

        //put word in new_node
        strcpy(new_node->word, word);

        counter++;

        //Hash word
        wordHash = hash(word);

        if(hashtable[wordHash] == NULL)
        {
            hashtable[wordHash] = new_node;
            new_node->next = NULL;
        }

        else
        {
            new_node->next = hashtable[wordHash];
            hashtable[wordHash] = new_node;
        }


    }
    //close dictionary
    fclose(dict);
    loaded = true;
    return true;
}

////////////////////////////////////////////////////////////
// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if(loaded)
    {
        return counter;
    }

    else
    {
       return 0;
    }
}

/////////////////////////////////////////////////////////////
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0; i < SIZE; i++)
    {
        node *unload = hashtable[i];
        while(unload != NULL)
        {
            node *temp = unload;
           unload = unload->next;
            free(temp);
        }
    }

    loaded = false;
    return true;
}
