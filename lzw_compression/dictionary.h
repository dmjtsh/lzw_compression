#pragma once

const size_t DICTIONARY_START_CAPACITY = 300;

struct ByteStrings
{
    char*  byte_string;
    size_t length;
};

typedef short       KeyType;
typedef ByteStrings ValueType;

struct Dictionary
{
    KeyType*   keys;
    ValueType* values;

    size_t size;
    size_t capacity;
};

void DictionaryPrint(Dictionary* dictionary, FILE* file_to_print);

ValueType* DictionaryAdd(Dictionary* dictionary, KeyType key, ValueType* value);

ValueType* DictionaryGetValue(Dictionary* dictionary, KeyType key);
bool       DictionarySetKey(Dictionary* dictionary, ValueType* value, KeyType* key);

void DictionaryCtor(Dictionary* dictionary);
void DictionaryDtor(Dictionary* dictionary);