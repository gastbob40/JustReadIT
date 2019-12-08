#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 2000
#define LENGTH 45


typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}node;


char* dictionary =  "/home/damien.champeyroux/Downloads/dictionary.txt";
char* word = "incredible";


bool check(const char* word);

bool load(const char* dictionary);

node* hashtable[HASH_SIZE];

int hash_function(const char* word);

int number_of_words = 0;


int is_in_dictionary(char* word)
{
    bool loaded = load(dictionary);

    if (!loaded){
        printf("Fail dictionary loading");
        return 1;
    }
    return check(word);
}


bool check(const char* word)
{
    int word_length = strlen(word);
    char lower_word[LENGTH+1];
    for (int i = 0; i < word_length; i++)
    {
        if(isupper(word[i]))
            lower_word[i] = tolower(word[i]) ;
        else
            lower_word[i] = word[i];

    }
    lower_word[word_length] = '\0';
    int bucket = hash_function(lower_word);
    node* cursor = hashtable[bucket];
    while (cursor != NULL)
    {
        if (strcmp(lower_word, cursor->word) == 0)
            return true;
        cursor = cursor->next;
    }
    return false;
}


bool load(const char* dictionary)
{
    for(int i = 0; i < HASH_SIZE; i++)
        hashtable[i] = NULL;

    FILE* the_dictionary;
    the_dictionary = fopen(dictionary, "r");
    if (the_dictionary == NULL)
        return false;

    char buffer[LENGTH+1];
    while (fscanf(the_dictionary, "%s", buffer) > 0)
    {
        node* new_node = malloc(sizeof(node));
        new_node->next = NULL;
        strcpy(new_node->word, buffer);
        int bucket = hash_function(new_node->word);
        if (hashtable[bucket] == NULL)
            hashtable[bucket] = new_node;
        else
            new_node->next = hashtable[bucket];
        hashtable[bucket] = new_node;
        number_of_words++;
    }
    fclose(the_dictionary);
    return true;
}

int hash_function(const char* word)
{
    int sum = 0;
    int word_length = strlen(word);

    for (int i = 0; i < word_length; i++)
        sum += word[i];

    int bucket = sum % HASH_SIZE;
    return bucket;
}

//is_in_dictionary(word))




