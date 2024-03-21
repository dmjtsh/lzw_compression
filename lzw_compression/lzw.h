#pragma once

const size_t MAX_CODE_WORD_SIZE  = 1024;
const size_t MAX_DICTIONARY_SIZE = 65536;

short* CompressData(const char* file_name, size_t* compressed_data_size);