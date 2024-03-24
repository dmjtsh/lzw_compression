#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "lzw.h"

int main(int argc, const char** argv)
{   
    FILE* compressed_file   = fopen("compressed.bin" , "wb");
    FILE* decompressed_file = fopen((argc > 2) ? argv[1] : "decompressed.bin", "wb");

    size_t   compressed_data_size = 0;
    KeyType* compressed_data = CompressData((argc > 1) ? argv[0] : "TESTS/source1.txt", &compressed_data_size);
    fwrite(compressed_data, sizeof(KeyType), compressed_data_size, compressed_file);
    fclose(compressed_file);

    size_t decompressed_data_size = 0;
    char*  decompressed_data = DecompressData("compressed.bin", &decompressed_data_size);
    fwrite(decompressed_data, sizeof(char), decompressed_data_size, decompressed_file);
    fclose(decompressed_file);

    return 0;
}