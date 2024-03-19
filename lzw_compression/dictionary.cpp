#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dictionary.h"

void DictionaryReallocUp(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    int* dictionary_keys_tmp     = (int*)  realloc(dictionary->keys, dictionary->capacity * 2 * sizeof(int));
    char** dictionary_values_tmp = (char**)realloc(dictionary->keys, dictionary->capacity * 2 * sizeof(char*));
    
    dictionary->capacity *= 2;

    assert(dictionary_keys_tmp   != nullptr);
    assert(dictionary_values_tmp != nullptr); 

    dictionary->keys   = dictionary_keys_tmp;
    dictionary->values = dictionary_values_tmp;

}

void DictionaryCtor(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    dictionary->keys   = (int*)  calloc(DICTIONARY_START_CAPACITY, sizeof(int));
    dictionary->values = (char**)calloc(DICTIONARY_START_CAPACITY, sizeof(char*));

    dictionary->capacity = DICTIONARY_START_CAPACITY;
    dictionary->size     = 0;

    assert(dictionary->keys   != nullptr);
    assert(dictionary->values != nullptr);
}

bool DictionaryCheck(Dictionary* dictionary, int key)
{
    assert(dictionary != nullptr);

    bool is_elem_in_dict = false;

    for (size_t i = 0; i < dictionary->size; i++)
        if(dictionary->keys[i] == key)
            is_elem_in_dict = true;

    return is_elem_in_dict;

}

void DictionaryAdd(Dictionary* dictionary, int key, const char* value)
{
    assert(dictionary != nullptr);

    dictionary->keys[dictionary->size]   = key;
    dictionary->values[dictionary->size] = strdup(value);

    if(dictionary->size > DICTIONARY_START_CAPACITY)
        DictionaryReallocUp(dictionary);
}

void DictionaryDtor(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    free(dictionary->keys); 

    for (size_t i = 0; i < dictionary->size; i++)
        free(dictionary->values[i]);

    free(dictionary->values);
}