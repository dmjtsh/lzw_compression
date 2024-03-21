#pragma once

const size_t MAX_CODE_WORD_SIZE  = 256;
const size_t MAX_DICTIONARY_SIZE = 4096;

const size_t ALPHABET_SIZE = 256;

short* CompressData(const char* file_name, size_t* compressed_data_size);