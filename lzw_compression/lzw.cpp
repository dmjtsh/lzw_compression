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

#define BUFFER_SET_NEXT_SYMBOL()                            \
do                                                          \
{                                                           \
   buffer.byte_string[buffer.length] = data[data_char_num]; \
   buffer.length++;                                         \
   data_char_num++;                                         \
} while(0)                                                  
    
#define INIT_DICT()                                         \
do                                                          \
{                                                           \
    InitializeDictionary(&dictionary);                      \
    compressed_data[comp_data_char_num] = CLEAR_CODE;       \
    comp_data_char_num++;                                   \
} while(0)

KeyType* CompressData(const char* file_name, size_t* compressed_data_size)
{
    assert(file_name            != nullptr);
    assert(compressed_data_size != nullptr);

    size_t data_size     = 0;
    size_t data_char_num = 0;
    const char* data     = GetFileText(file_name, &data_size);
                                                      // +2 CLEAR CODE && END_FILE
    KeyType* compressed_data  = (KeyType*)calloc(data_size+2, sizeof(KeyType));
    size_t comp_data_char_num = 0;
    assert(compressed_data != nullptr);

    Dictionary dictionary = {};
    DictionaryCtor(&dictionary);
    INIT_DICT();

    ValueType buffer                         = {};
    char      buffer_str[MAX_CODE_WORD_SIZE] = "";
    buffer.byte_string                       = buffer_str;
    buffer.length                            = 0;

    KeyType key = 0;

    while (data_char_num < data_size)
    {
        if (dictionary.size >= MAX_DICTIONARY_SIZE)
        {
            ClearDictionary(&dictionary);
            INIT_DICT();
        }

        BUFFER_SET_NEXT_SYMBOL();

        while(data_char_num <= data_size && (DictionarySetKey(&dictionary, &buffer, &key)) && key != CLEAR_CODE && key != END_FILE_CODE)
        {
            compressed_data[comp_data_char_num] = key;

            if (key == END_FILE_CODE)
                printf("");

            BUFFER_SET_NEXT_SYMBOL();
        }

        // SETTING CORRECT DATA CHAR NUM
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

#undef BUFFER_SET_NEXT_SYMBOL
#undef INIT_DICT

#define DECOMP_DATA_COPY_MEM(bytes_to_copy, bytes_len)                        \
do                                                                            \
{                                                                             \
    memcpy(decompressed_data+decomp_data_char_num, bytes_to_copy, bytes_len); \
    decomp_data_char_num += bytes_len;                                        \
} while(0)

#define SET_KEY()              \
do                             \
{                              \
    key = data[data_key_num];  \
    data_key_num++;            \
} while(0)

char* DecompressData(const char* file_name, size_t* decompressded_data_size)
{
    assert(file_name               != nullptr);
    assert(decompressded_data_size != nullptr);

    Dictionary dictionary = {};

    size_t data_size = 0;
    KeyType* data    = (KeyType*)GetFileText(file_name, &data_size);

    size_t decomp_data_char_num = 0;
    char*  decompressed_data    = (char*)calloc(data_size, sizeof(char));
    assert(decompressed_data != nullptr);

    ValueType buffer                         = {};
    char      buffer_str[MAX_CODE_WORD_SIZE] = "";
    buffer.byte_string                       = buffer_str;
    buffer.length                            = 0; 
    
    ValueType* value          = nullptr;
    ValueType* previous_value = nullptr;

    KeyType key         = 0;
    size_t data_key_num = 0;
    SET_KEY();

    while(key != END_FILE_CODE)
    {
        if (decomp_data_char_num > data_size)
        {
            char* tmp = (char*)realloc(decompressed_data, data_size*2);
            assert(tmp != nullptr);

            decompressed_data = tmp;
            data_size *= 2;
        }

        if (key == CLEAR_CODE)
        {
            ClearDictionary(&dictionary);
            InitializeDictionary(&dictionary);
            
            SET_KEY();

            if(key == END_FILE_CODE)
                break;
            
            value = DictionaryGetValue(&dictionary, key);

            DECOMP_DATA_COPY_MEM(value->byte_string, value->length);

            previous_value = value;
        }
        else
        {
            if(value = DictionaryGetValue(&dictionary, key))
            {
                DECOMP_DATA_COPY_MEM(value->byte_string, value->length);

                // ADD NEW STRING TO TABLE
                memcpy(buffer.byte_string, previous_value->byte_string, previous_value->length);
                buffer.byte_string[previous_value->length] = value->byte_string[0];
                buffer.length = previous_value->length + 1;

                DictionaryAdd(&dictionary, dictionary.size, &buffer);

                previous_value = value;
            }
            else
            {            
                // ADD NEW STRING TO TABLE
                memcpy(buffer.byte_string, previous_value->byte_string, previous_value->length);
                buffer.byte_string[previous_value->length] = previous_value->byte_string[0];
                buffer.length = previous_value->length + 1;
                
                previous_value = DictionaryAdd(&dictionary, dictionary.size, &buffer);

                DECOMP_DATA_COPY_MEM(buffer.byte_string, buffer.length);
            }
        }

        SET_KEY();
    }

    *decompressded_data_size = decomp_data_char_num; 

    return decompressed_data;
}

#undef DECOMP_DATA_COPY_MEM
#undef SET_KEY