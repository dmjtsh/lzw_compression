#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "DimasLIB/DimasUtilities/utilities.h"
#include "lzw.h"

int main(int argc, const char** argv)
{   
    FILE* compressed_file = fopen("compressed.txt" , "w");

    size_t compressed_data_size = 0;
    short* compressed_data = CompressData((argc > 1) ? argv[0] : "source.txt", &compressed_data_size);

    fwrite(compressed_data, sizeof(short), compressed_data_size, compressed_file);

    return 0;
}