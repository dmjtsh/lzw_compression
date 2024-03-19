#pragma once

const size_t DICTIONARY_START_CAPACITY = 100;

struct Dictionary
{
    int*    keys;
    char** values;

    size_t size;
    size_t capacity;

    size_t largest_word_size;
};

void DictionaryPrint(Dictionary* dictionary, FILE* file_to_print);

void  DictionaryAdd(Dictionary* dictionary, int key, const char* value, size_t value_len);

char* DictionaryGetValue(Dictionary* dictionary, int key);
int   DictionaryGetKey(Dictionary* dictionary, char* value);

void DictionaryCtor(Dictionary* dictionary);
void DictionaryDtor(Dictionary* dictionary);