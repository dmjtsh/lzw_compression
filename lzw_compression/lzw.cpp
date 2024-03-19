#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "lzw.h"



const char* CompressData(const char* data)
{
    assert(data != nullptr);

    Dictionary dictionary = {};
    DictionaryCtor(&dictionary);

    DictionaryDtor(&dictionary);
}