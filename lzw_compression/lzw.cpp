#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "lzw.h"
#include "dictionary.h"

void InitializeDictionary(Dictionary* dictionary)
{
    assert(dictionary != nullptr);
   
    for (int i = 0; i < ALPHABET_SIZE; i++) 
    {
         char str[2] = { i, '\0' };
         DictionaryAdd(dictionary, i, str, 1);
    }

}

const char* CompressData(const char* data)
{
    assert(data != nullptr);

    Dictionary dictionary = {};
    DictionaryCtor(&dictionary);

    char* compressed_data = (char*) calloc(strlen(data), sizeof(char)); 
    assert(compressed_data != nullptr);

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

        if(str_size == dictionary.largest_word_size)
        {
            while (!(key = DictionaryGetKey(&dictionary, curr_str)))
            {
                curr_str[str_size] = '\0';
                str_size--;
                data_char_num--;
            }
            
            compressed_data[comp_data_char_num] = key;
            comp_data_char_num++;
                
            str_size = 0;
            curr_str[str_size] = '\0';
        }

     }

    DictionaryDtor(&dictionary);
}