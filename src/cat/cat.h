#ifndef S21_CAT_H
#define S21_CAT_H

#define _GNU_SOURCE
#include <stdio.h>

#include "option.h"

#define S21_PRINT_ERROR(STR, ...) \
  fprintf(stderr, "s21_cat: " STR, ##__VA_ARGS__)

t_s21_option s21_parse_arguments(int argc, char** argv, bool* ok);

void s21_cat(t_s21_option options, char** filenames);

#endif  // S21_CAT_H