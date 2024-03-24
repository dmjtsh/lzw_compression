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

    KeyType key = 0;
    
    ValueType buffer = {};
    buffer.length = 1;
    buffer.byte_string = (char*)&key;

    for (key = CHAR_MIN; key <= CHAR_MAX; key++) 
         DictionaryAdd(dictionary, dictionary->size, &buffer);

    // ADDING CLEAR AND END_FILE CODES
    buffer.length = 2;

    key = CLEAR_CODE;
    DictionaryAdd(dictionary, key, &buffer);

    key = END_FILE_CODE;
    DictionaryAdd(dictionary, key, &buffer);
}

void ClearDictionary(Dictionary* dictionary)
{
    assert(dictionary != nullptr);

    DictionaryDtor(dictionary);
    DictionaryCtor(dictionary);
}

KeyType* CompressData(const char* file_name, size_t* compressed_data_size)
{
    assert(file_name            != nullptr);
    assert(compressed_data_size != nullptr);

    size_t data_size          = 0;
    size_t comp_data_char_num = 0;
    size_t data_char_num      = 0;
    const char* data          = GetFileText(file_name, &data_size);
                                                      // +2 CLEAR CODE && END_FILE
    KeyType* compressed_data = (KeyType*)calloc(data_size+2, sizeof(KeyType));
    assert(compressed_data != nullptr);

    Dictionary dictionary = {};
    DictionaryCtor(&dictionary);

    // WRITING CLEAR CODE FIRSTLY
    InitializeDictionary(&dictionary);
    compressed_data[comp_data_char_num] = CLEAR_CODE;
    comp_data_char_num++;

    ValueType buffer = {};
    char      buffer_str[MAX_CODE_WORD_SIZE] = "";
    buffer.byte_string = buffer_str;
    buffer.length      = 0;

    KeyType key = 0;
    while (data_char_num < data_size)
    {
        if (dictionary.size >= MAX_DICTIONARY_SIZE)
        {
            ClearDictionary(&dictionary);
            InitializeDictionary(&dictionary);

            compressed_data[comp_data_char_num] = CLEAR_CODE;
            comp_data_char_num++;
        }

        buffer.byte_string[buffer.length] = data[data_char_num];
        buffer.length++;
        data_char_num++;

        while(data_char_num <= data_size && (DictionarySetKey(&dictionary, &buffer, &key)))
        {
            compressed_data[comp_data_char_num] = key;

            buffer.byte_string[buffer.length] = data[data_char_num];
            buffer.length++;
            data_char_num++;
        }

        data_char_num--;
        comp_data_char_num++;
        
        // ADDING NEW WORD TO DICTIONARY
        DictionaryAdd(&dictionary, dictionary.size, &buffer);

        // RESETING THE BUFFER 
        buffer.length = 0;
    }

    compressed_data[comp_data_char_num] = END_FILE_CODE;
    comp_data_char_num++;
    
    DictionaryDtor(&dictionary);

    *compressed_data_size = comp_data_char_num;

    return compressed_data;
}

char* DecompressData(const char* file_name, size_t* decompressded_data_size)
{
    assert(file_name               != nullptr);
    assert(decompressded_data_size != nullptr);

    Dictionary dictionary = {};
    DictionaryCtor(&dictionary);

    InitializeDictionary(&dictionary);

    size_t data_size            = 0;
    size_t decomp_data_char_num = 0;
    size_t data_key_num         = 0;
    KeyType* data               = (KeyType*)GetFileText(file_name, &data_size);

    //fix
    char* decompressed_data = (char*)calloc(data_size*4, sizeof(char));
    assert(decompressed_data != nullptr);

    ValueType buffer = {};
    char      buffer_str[MAX_CODE_WORD_SIZE] = "";
    buffer.byte_string = buffer_str;
    buffer.length      = 0;
    
    ValueType* value          = nullptr;
    ValueType* previous_value = nullptr;

    KeyType key = data[data_key_num];
    data_key_num++;
    while(key != END_FILE_CODE)
    {
        if (key == CLEAR_CODE)
        {
            ClearDictionary(&dictionary);
            InitializeDictionary(&dictionary);

            key = data[data_key_num];
            data_key_num++;

            if(key == END_FILE_CODE)
                break;
            
            value = DictionaryGetValue(&dictionary, key);

            memcpy(decompressed_data+decomp_data_char_num, value->byte_string, value->length);
            decomp_data_char_num += value->length;
            
            previous_value = value;
        }
        else
        {
            if(value = DictionaryGetValue(&dictionary, key))
            {
                memcpy(decompressed_data+decomp_data_char_num, value->byte_string, value->length);
                decomp_data_char_num += value->length;

                // ADD STRING TO TABLE
                memcpy(buffer.byte_string, previous_value->byte_string, previous_value->length);
                buffer.byte_string[previous_value->length] = value->byte_string[0];
                buffer.length = previous_value->length + 1;
                
                DictionaryAdd(&dictionary, dictionary.size, &buffer);

                previous_value = value;
            }
            else
            {            
                // ADD STRING TO TABLE
                memcpy(buffer.byte_string, previous_value->byte_string, previous_value->length);
                buffer.byte_string[previous_value->length] = previous_value->byte_string[0];
                buffer.length = previous_value->length + 1;
                
                DictionaryAdd(&dictionary, dictionary.size, &buffer);

                memcpy(decompressed_data+decomp_data_char_num, buffer.byte_string, buffer.length);
                decomp_data_char_num += buffer.length;
            }
        }
        key = data[data_key_num];
        data_key_num++;
    }

    *decompressded_data_size = decomp_data_char_num; 

    return decompressed_data;
}