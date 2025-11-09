#ifndef S21_UTILITY_H
#define S21_UTILITY_H

#include <stdio.h>

#define S21_PRINT_ERROR(STR, ...) fprintf(stderr, "s21_grep: " STR "\n", ##__VA_ARGS__)
#define S21_SUPRESS_ERRORS() freopen("/dev/null", "w", stderr)

#endif // S21_UTILITY_H