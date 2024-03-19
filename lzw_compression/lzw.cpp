#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "lzw.h"
#include "dictionary.h"

void InitializeDictionary(Dictionary* dictionary)
{
    assert(dictionary != nullptr);
   
    // DEBUG
    for (int i = 0; i < 4; i++)
    {
        char str[2] = { 'a'+i, '\0' };
        DictionaryAdd(dictionary, dictionary->size+1, str, 1);
    }

    /*
    for (int i = 0; i < ALPHABET_SIZE; i++) 
    {
         char str[2] = { i, '\0' };
         DictionaryAdd(dictionary, i, str, 1);
    }
    */

}

const char* CompressData(const char* data)
{
    assert(data != nullptr);

    Dictionary dictionary = {};
    DictionaryCtor(&dictionary);
    
    // DEBUG
    DictionaryPrint(&dictionary, stdout);

    InitializeDictionary(&dictionary);
    // DEBUG
    DictionaryPrint(&dictionary, stdout);

    char* compressed_data = (char*) calloc(strlen(data), sizeof(char)); 
    assert(compressed_data != nullptr);

    size_t data_len = strlen(data);
    size_t comp_data_char_num = 0;
    size_t data_char_num = 0;
    size_t str_size = 0;

    char  curr_str[MAX_CODE_WORD_SIZE] = {};

    int key = 0;
    while (data[data_char_num] != '\0')
    {
        curr_str[str_size] = data[data_char_num]; 
        str_size++;
        data_char_num++;
        
        if(data_char_num == 10)
            printf("");

        if(str_size == dictionary.largest_word_size || data_char_num >= data_len)
        {
            // CHECK THE BIGGEST WORD IN DICT
            while (!(key = DictionaryGetKey(&dictionary, curr_str)))
            {
                str_size--;
                data_char_num--;
                curr_str[str_size] = '\0';
            }
            
            compressed_data[comp_data_char_num] = key;
            comp_data_char_num++;
            
            // ADDING NEW WORD TO DICTIONARY

            curr_str[str_size]   = data[data_char_num];
            curr_str[str_size+1] = '\0';

            DictionaryAdd(&dictionary, dictionary.size+1, curr_str, str_size+1);
            // DEBUG
            DictionaryPrint(&dictionary, stdout);
            printf("Code String: %s\n", compressed_data);

            str_size = 0;
            curr_str[str_size] = '\0';
        }

     }

    printf("Code String: %s\n", compressed_data);

    DictionaryDtor(&dictionary);

    return compressed_data;
}