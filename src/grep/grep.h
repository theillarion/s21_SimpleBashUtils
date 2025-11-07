#ifndef S21_GREP_H
#define S21_GREP_H

#include "option.h"

#define S21_PRINT_ERROR(STR, ...) \
    fprintf(stderr, "s21_grep: " STR "\n", ##__VA_ARGS__)

void s21_grep(t_s21_option options, const char* pattern, char** filenames);


#endif // S21_GREP_H
