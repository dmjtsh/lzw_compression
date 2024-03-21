#pragma once

const size_t DICTIONARY_START_CAPACITY = 300;

struct Dictionary
{
    short* keys;
    char** values;

    size_t size;
    size_t capacity;
};

void DictionaryPrint(Dictionary* dictionary, FILE* file_to_print);

void DictionaryAdd(Dictionary* dictionary, short key, const char* value, size_t value_len);

char* DictionaryGetValue(Dictionary* dictionary, short key);
short DictionaryGetKey(Dictionary* dictionary, char* value);

void DictionaryCtor(Dictionary* dictionary);
void DictionaryDtor(Dictionary* dictionary);