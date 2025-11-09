#define _GNU_SOURCE // for strdup

#include "strings.h"

#include <stdio.h> // FIXME: remove
#include <stdlib.h>
#include <string.h>

size_t count_strings(char* const* strings)
{
    size_t count = 0;
    while (strings && strings[count]) ++count;
    return count;
}

bool append_string(char*** dest_strings, const char* src_string)
{
    if (!dest_strings || !src_string) return false;

    bool ret = true;
    if (!*dest_strings)
    {
        printf("Allocating new strings array: '%s'\n", src_string); // FIXME: remove
        *dest_strings = (char**)malloc(2 * sizeof(**dest_strings));
        if (*dest_strings)
        {
            (*dest_strings)[0] = strdup(src_string);
            (*dest_strings)[1] = NULL;  
        }
        else {
            ret = false;
        }
    } else {
        printf("added elem to array: '%s'\n", src_string); // FIXME: remove
        size_t count = count_strings(*dest_strings);
        char** new_array = (char**)realloc(*dest_strings, (count + 2) * sizeof(char*));
        if (new_array)
        {
            new_array[count] = strdup(src_string);
            new_array[count + 1] = NULL;
            *dest_strings = new_array;
        }
        else{
            ret = false;
        }
    }
    return ret;
}

void destroy_strings(char*** strings)
{
    if (!strings || !*strings) return;

    size_t count = count_strings(*strings);
    for (size_t i = 0; i < count; ++i)
    {
        free((*strings)[i]);
    }
    free(*strings);
    *strings = NULL;
}
