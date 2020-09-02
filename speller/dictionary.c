// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int dic_size = 0;

// Number of buckets in hash table
const unsigned int N = 200000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int n = strlen(word);
    char word_copy[LENGTH + 1];
    for (int i = 0; i < n; i++)
    {
        word_copy[i] = tolower(word[i]);
    }
    word_copy[n] = '\0';
    // hash word to get hash value
    int location = hash(word_copy);
    //access linked list at that index in the hash table
    node *cursor = table[location];
    while (cursor != NULL)
    {
        //see if word is in linked list
        if (strcasecmp(cursor->word, word_copy) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //* credits to...*https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *dic = fopen(dictionary, "r");
    if (dic == NULL)
    {
        return false;
    }
    //read strings from file one at a time
    char tmp[LENGTH + 1];
    while ((fscanf(dic, "%s", tmp) != EOF))
    {
        size();
        //create a new node for each word
        node *new_word = malloc(sizeof(node));
        if (new_word == NULL)
        {
            unload();
            return false;
        }
        strcpy(new_word->word, tmp);
        new_word->next = NULL;
        //hash word to obtain a hash value
        int location = hash(new_word->word);
        node *test = table[location];
        //insert node into hash table at that location

        if (test == NULL)
        {

            table[location] = new_word;
        }
        else
        {
            new_word->next = table[location];
            table[location] = new_word;
        }
    }
    fclose(dic);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // adds a count each time a word is added to hash
    dic_size = dic_size + 1;
    //outputs - 1 to account for the final running when size is actually needed
    return (dic_size - 1);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int y = 0; y < N; y++)
    {
        node *cursor = table[y];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
