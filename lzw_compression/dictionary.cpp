#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "dictionary.h"

void DictionaryReallocUp(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    int* dictionary_keys_tmp     = (int*)  realloc(dictionary->keys, dictionary->capacity * 2 * sizeof(int));
    char** dictionary_values_tmp = (char**)realloc(dictionary->values, dictionary->capacity * 2 * sizeof(char*));
    
    dictionary->capacity *= 2;

    assert(dictionary_keys_tmp   != nullptr);
    assert(dictionary_values_tmp != nullptr); 

    dictionary->keys   = dictionary_keys_tmp;
    dictionary->values = dictionary_values_tmp;
}

char* DictionaryGetValue(Dictionary* dictionary, int key)
{
    assert(dictionary != nullptr);

    bool is_elem_in_dict = false;

    for (size_t i = 0; i < dictionary->size; i++)
        if(dictionary->keys[i] == key)
            return dictionary->values[i];

    return nullptr;

}

int DictionaryGetKey(Dictionary* dictionary, char* value)
{
    assert(dictionary != nullptr);

    for (size_t i = 0; i < dictionary->size; i++)
        if(strcmp(dictionary->values[i], value) == 0)
            return dictionary->keys[i];

    return 0;

}

void DictionaryAdd(Dictionary* dictionary, int key, const char* value, size_t value_len)
{
    assert(dictionary != nullptr);

    dictionary->keys[dictionary->size]   = key;
    dictionary->values[dictionary->size] = _strdup(value);

    dictionary->size++;

    if(dictionary->size >= dictionary->capacity)
        DictionaryReallocUp(dictionary);

    dictionary->largest_word_size = __max(dictionary->largest_word_size, value_len);
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

void DictionaryPrint(Dictionary* dictionary, FILE* file_to_print)
{
    fprintf(file_to_print, "Dictionary:\n"
                           "Size: %zu\n"
                           "Capacity: %zu\n"
                           "{\n", 
                            dictionary->size, dictionary->capacity);

    for(size_t i = 0; i < dictionary->size; i ++)
        fprintf(file_to_print, "(%zu) Key: %d -> Value: %s\n", i, dictionary->keys[i], dictionary->values[i]);

    fprintf(file_to_print, "}\n\n");
}

void DictionaryDtor(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    free(dictionary->keys); 

    for (size_t i = 0; i < dictionary->size; i++)
        free(dictionary->values[i]);

    free(dictionary->values);
}