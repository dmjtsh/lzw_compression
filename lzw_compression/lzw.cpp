#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "lzw.h"
#include "dictionary.h"
#include "DimasLIB/DimasUtilities/utilities.h"

void InitializeDictionary(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    for (char i = CHAR_MIN; i < CHAR_MAX; i++) 
    {
         char str[2] = { i, '\0' };
         DictionaryAdd(dictionary, dictionary->size + 1, str, 1);
    }
}

void ClearDictionary(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    DictionaryDtor(dictionary);
    DictionaryCtor(dictionary);
}

short* CompressData(const char* file_name, size_t* compressed_data_size)
{
    assert(file_name            != nullptr);
    assert(compressed_data_size != nullptr);

    Dictionary dictionary = {};
    DictionaryCtor(&dictionary);

    InitializeDictionary(&dictionary);
    DictionaryPrint(&dictionary, stdout);

    const char* data          = GetFileText(file_name);
    size_t data_size          = GetFileSize(file_name);
    size_t comp_data_char_num = 0;
    size_t data_char_num      = 0;
    size_t str_size           = 0;

    short* compressed_data = (short*)calloc(data_size, sizeof(short)); 
    assert(compressed_data != nullptr);

    char  buffer_str[MAX_CODE_WORD_SIZE] = {};
    short key = 0;
    
    while (data_char_num < data_size)
    {
        if (dictionary.size >= MAX_DICTIONARY_SIZE)
        {
            ClearDictionary(&dictionary);
            InitializeDictionary(&dictionary);
        }

        buffer_str[str_size] = data[data_char_num]; 
        buffer_str[str_size+1] = '\0';

        while(data_char_num < data_size && (key = DictionaryGetKey(&dictionary, buffer_str)))
        {
            compressed_data[comp_data_char_num] = key;

            str_size++;
            data_char_num++;

            if(data_char_num == 1023)
            printf("");

            buffer_str[str_size] = data[data_char_num];
            buffer_str[str_size+1] = '\0';
        }

        comp_data_char_num++;
        
        // ADDING NEW WORD TO DICTIONARY

        DictionaryAdd(&dictionary, dictionary.size+1, buffer_str, str_size+1);

        // RESETING THE BUFFER
        str_size = 0;
        buffer_str[str_size] = '\0';
    }

    DictionaryDtor(&dictionary);

    *compressed_data_size = comp_data_char_num;

    return compressed_data;
}