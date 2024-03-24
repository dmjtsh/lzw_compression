#pragma once

#include "dictionary.h"

const size_t MAX_CODE_WORD_SIZE  = 1024;
const size_t MAX_DICTIONARY_SIZE = 65536-2; // last two nums reserved for clear and end_file_code

const KeyType CLEAR_CODE          = 65535;
const KeyType END_FILE_CODE       = 65534;

KeyType* CompressData(const char* file_name, size_t* compressed_data_size);
char*  DecompressData(const char* file_name, size_t* decompressded_data_size);