#ifndef S21_UTILITY_H
#define S21_UTILITY_H

#include <stdio.h>

#define S21_PRINT_ERROR(STR, ...) fprintf(stderr, "s21_cat: " STR "\n", ##__VA_ARGS__)

#endif // S21_UTILITY_H