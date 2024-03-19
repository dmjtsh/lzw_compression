#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "lzw.h"

int main()
{   
    char str1[200] = "aaaaaabacaa";

    char str2[1000] = {};
    strcpy(str2, CompressData(str1));

    printf("%s", str2);

    return 0;
}