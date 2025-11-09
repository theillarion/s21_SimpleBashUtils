#include "cat.h"

#define _GNU_SOURCE
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"

static void print_char(unsigned char symbol, t_s21_option options) {
  if (symbol == '\n' && exists_option(options, OPTION_SHOW_ENDS)) {
    printf("$\n");
  } else if (symbol == '\t' && exists_option(options, OPTION_SHOW_TABS)) {
    printf("^I");
  } else if ((symbol < 32 && symbol != '\n' && symbol != '\t') ||
             symbol == 127) {
    if (exists_option(options, OPTION_SHOW_NONPRINTING)) {
      char new_symbol = (symbol == 127) ? '?' : (symbol + 64);
      printf("^%c", new_symbol);
    } else {
      printf("%c", symbol);
    }
  } else if (symbol >= 128) {
    if (exists_option(options, OPTION_SHOW_NONPRINTING)) {
      printf("M-");
      symbol -= 128;
      if ((symbol < 32) || symbol == 127) {
        char new_symbol = (symbol == 127) ? '?' : (symbol + 64);
        printf("^%c", new_symbol);
      } else {
        printf("%c", symbol);
      }
    } else {
      printf("%c", symbol);
    }
  } else {
    printf("%c", symbol);
  }
}

static bool need_number(size_t prev_symbol, size_t curr_symbol,
                        t_s21_option options) {
  return prev_symbol == '\n' &&
         ((exists_option(options, OPTION_NUMBER_NONBLANK) &&
           curr_symbol != '\n') ||
          (!exists_option(options, OPTION_NUMBER_NONBLANK) &&
           exists_option(options, OPTION_NUMBER_ALL)));
}

static bool handle_file(t_s21_option options, const char* filename) {
  int ret = true;
  FILE* file = fopen(filename, "rb");
  if (file) {
    int curr_symbol, prev_symbol = '\n';
    size_t line_num = 1;
    int blank_count = 0;
    while ((curr_symbol = fgetc(file)) != EOF) {
      if (exists_option(options, OPTION_SQUEEZE_BLANKS) &&
          prev_symbol == '\n' && curr_symbol == '\n') {
        blank_count++;
      } else {
        blank_count = 0;
      }
      if (blank_count >= 2) {
        continue;
      }
      if (need_number(prev_symbol, curr_symbol, options)) {
        printf("%6zu\t", line_num++);
      }
      print_char((unsigned char)curr_symbol, options);
      prev_symbol = curr_symbol;
    }
    fclose(file);
  } else {
    S21_PRINT_ERROR("%s", strerror(errno));
    errno = 0;
    ret = false;
  }
  return ret;
}

bool s21_cat(t_s21_option options, char** filenames) {
  if (!filenames) {
    return false;
  }

  bool ret = true;
  while (*filenames && ret) {
    ret = handle_file(options, *filenames);
    ++filenames;
  }
  return ret;
}