#ifndef S21_STRINGS_H
#define S21_STRINGS_H

#include <stddef.h>
#include <stdbool.h>

size_t count_strings(char* const* strings);
bool append_string(char*** dest_strings, const char* src_string);
void destroy_strings(char*** dest_strings);

#endif // S21_STRINGS_H