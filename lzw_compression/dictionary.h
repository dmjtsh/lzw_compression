#pragma once

struct Dictionary
{
    int*    keys;
    char** values;

    size_t size;
    size_t capacity;
};