#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "dictionary.h"

void DictionaryReallocUp(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    KeyType*   dictionary_keys_tmp   = (KeyType*)  realloc(dictionary->keys, dictionary->capacity * 2 * sizeof(KeyType));
    ValueType* dictionary_values_tmp = (ValueType*)realloc(dictionary->values, dictionary->capacity * 2 * sizeof(ValueType));
    
    dictionary->capacity *= 2;

    assert(dictionary_keys_tmp   != nullptr);
    assert(dictionary_values_tmp != nullptr); 

    dictionary->keys   = dictionary_keys_tmp;
    dictionary->values = dictionary_values_tmp;
}

ValueType* DictionaryGetValue(Dictionary* dictionary, KeyType key)
{
    assert(dictionary != nullptr);

    for (size_t i = 0; i < dictionary->size; i++)
        if(dictionary->keys[i] == key)
            return &dictionary->values[i];

    return nullptr;
}

bool DictionarySetKey(Dictionary* dictionary, ValueType* value, KeyType* key)
{
    assert(dictionary != nullptr);

    for (size_t i = 0; i < dictionary->size; i++)
    {
        if(value->length != dictionary->values[i].length)
            continue;

        if(memcmp(dictionary->values[i].byte_string, value->byte_string, value->length) == 0)
        {
            *key = dictionary->keys[i];
            return true;
        }
    }
    return false;
}

ValueType* DictionaryAdd(Dictionary* dictionary, KeyType key, ValueType* value)
{
    assert(dictionary != nullptr);

    dictionary->keys[dictionary->size]               = key;
    dictionary->values[dictionary->size].byte_string = (char*)calloc(value->length, sizeof(char));
    assert(dictionary->values[dictionary->size].byte_string != nullptr);
    memcpy(dictionary->values[dictionary->size].byte_string, value->byte_string, value->length);

    dictionary->values[dictionary->size].length = value->length;

    dictionary->size++;

    if(dictionary->size >= dictionary->capacity)
        DictionaryReallocUp(dictionary);

    return &dictionary->values[dictionary->size];
}

void DictionaryCtor(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    dictionary->keys   = (KeyType*)  calloc(DICTIONARY_START_CAPACITY, sizeof(KeyType));
    dictionary->values = (ValueType*)calloc(DICTIONARY_START_CAPACITY, sizeof(ValueType));

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

    for(size_t i = 0; i < dictionary->size; i++)
    {
        fprintf(file_to_print, "(%zu) Key: %d -> Value:", i, dictionary->keys[i]);
        for (size_t j = 0; j < dictionary->values[i].length; j++)
            fputc(dictionary->values[i].byte_string[j], file_to_print);
        fprintf(file_to_print, "\n");
    }

    fprintf(file_to_print, "}\n\n");
}

void DictionaryDtor(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    if(dictionary->keys && dictionary->values && dictionary->size > 0)
    {
        free(dictionary->keys); 

        // TODO:чекнуть на утечки
        for (size_t i = 0; i < dictionary->size; i++)
            free(dictionary->values[i].byte_string);

        free(dictionary->values);
    }
}