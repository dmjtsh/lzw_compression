#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <ctype.h>
#include <sys\stat.h>

#include "utilities.h"

void SetErrorBit(unsigned* error, int error_bit)
{
    assert(error != NULL);
    *error |= error_bit;
}

void UnsetErrorBit(unsigned* error, int error_bit)
{
    assert(error != NULL);
    *error &= ~error_bit;
}

int GetFileSize(const char* file_name)
{
    struct stat st = {};

    int ret = stat(file_name, &st);
    assert(ret == 0);

    return st.st_size;
}

char* GetFileText(const char* file_name)
{
    if(!file_name)
        return NULL;
    int file_size = GetFileSize(file_name);

    FILE* file = NULL;
    file = fopen(file_name, "rb"); // open
    if (!file)
        return NULL;

    char* text = (char*)calloc(file_size + 1, sizeof(char));
    if (!text)
        return NULL;
    
    size_t n_chars = fread(text, sizeof(char), file_size, file); // read

	text[n_chars] = '\0';

    fclose(file); // close
    return text;
}

char* GetFileText(const char* file_name, size_t* n_chars)
{
    if(!file_name || !n_chars)
        return NULL;
    int file_size = GetFileSize(file_name);

    FILE* file = NULL;
    file = fopen(file_name, "rb"); // open
    if (!file)
        return NULL;

    char* text = (char*)calloc(file_size + 1, sizeof(char));
    if (!text)
        return NULL;
    
    *n_chars = fread(text, sizeof(char), file_size, file); // read

	text[*n_chars] = '\0';

    fclose(file); // close
    return text;
}

void CleanStr(char* str)
{
	size_t curr_ch_num = 0;
	
	while(str[curr_ch_num] != '\0')
	{
		str[curr_ch_num] = '\0';
		curr_ch_num++;
	}
}

int CountTextStrings(const char* text)
{
    if (!text)
        return -1;
    int text_size = strlen(text);

    size_t indents_num = 0;

    for (const char* current_p = text; current_p - text <= text_size; current_p++)
        if (*current_p == '\n')
            indents_num++;

    return indents_num;
}

size_t BordersRand(size_t left, size_t right)
{
    static bool is_seed_set = 0;
    if (!is_seed_set)
    {
        time_t t;
        srand((unsigned)time(&t));
        is_seed_set = 1;
    }
    return (left + rand() % (right - left));
}

bool IsDoubleNumsEqual(double num1, double num2)
{
    return fabs(num1 - num2) < EPSILON;
}

bool IsDoubleInt(double num)
{
	return num - (int)num < EPSILON;
}

double StrToNum(const char* str, size_t str_len, bool* is_str_num)
{
	*is_str_num = true;
	bool is_negative = false;
	size_t char_num = 0;
	double final_num = 0;
	if (str[0] == '-')
	{
		is_negative = true;
		char_num = 1;
	}

	for (;char_num < str_len; char_num++)
	{
		if(!isdigit(str[char_num]))
		{
			*is_str_num = false;
			break;
		}
		final_num += (str[char_num] - '0') * pow(10, str_len-char_num-1);  
	}
	if(is_negative)
		return -final_num;
	return final_num;
}

bool IsCharLetter(char ch)
{
	return (65  <= ch && ch <= 90)  || (97   <= ch && ch <= 122) ||
		   (128 <= ch && ch <= 175) || ( 224 <= ch && ch <= 241) ||
		   ( -64 <= ch && ch <= -1);
}

bool IsCharDigit(char ch)
{
	return '0' <= ch && ch <= '9';
}