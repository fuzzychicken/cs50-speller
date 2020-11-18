// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1985;

// Hash table
node *table[N];

int wordcount = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // use hash function on word to determine index
    unsigned int index = hash(word);

    // point cursor to the first element of the linked list at the index
    node *cursor = table[index];

    // traverse linked list
    while (cursor != NULL) {
        if (strcasecmp(cursor->word, word) == 0) {
            return true;
        }
        else {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
// Got hash function from: https://stackoverflow.com/questions/7666509/hash-function-for-string
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++)) {
        hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */
    }

    return (hash % 1985);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *inputfile = fopen(dictionary, "r");
    if (inputfile == NULL) {
        return false;
    }

    char buffer[LENGTH+1];


    //initialize everything in table to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    bool result = true;
    // read strings from file
    while(fscanf(inputfile, "%s", buffer) == 1) {

        // create a new node
        node *n_node = malloc(sizeof(node));
        wordcount++;
        if (n_node == NULL) {
            result = false;
            break;
        }
        else {
            // copy string into node
            strcpy(n_node->word, buffer);
            n_node->next = NULL;

            // determining which index of linked lists to insert word into
            int index = hash(buffer);

            //insert node into hash table
            if (table[index] == NULL) {
                table[index] = n_node;
            }
            else {
                n_node->next = table[index];
                table[index] = n_node;
            }
        }

    }
    fclose(inputfile);
    return result;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++) {
        node *cursor = table[i];
        node *tmp = cursor;

        while (cursor != NULL) {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }

    }
    return true;
}
