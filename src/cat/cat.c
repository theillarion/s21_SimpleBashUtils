#include "cat.h"

#define _GNU_SOURCE
#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const char* short_opt_str = "bnsvETet";
static const struct option long_opt_str[] = {{"number-nonblank", 0, NULL, 'b'},
                                             {"number", 0, NULL, 'n'},
                                             {"squeeze-blank", 0, NULL, 's'}};

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

static void handle_file(t_s21_option options, const char* filename) {
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
  }
}

t_s21_option s21_parse_arguments(int argc, char** argv, bool* ok) {
  t_s21_option options = OPTION_NONE;
  bool do_while = true;

  if (ok) *ok = true;
  while (do_while) {
    int ret = getopt_long(argc, argv, short_opt_str, long_opt_str, NULL);
    if (ret == 'b')
      options = add_option(options, OPTION_NUMBER_NONBLANK);
    else if (ret == 'n')
      options = add_option(options, OPTION_NUMBER_ALL);
    else if (ret == 's')
      options = add_option(options, OPTION_SQUEEZE_BLANKS);
    else if (ret == 'v')
      options = add_option(options, OPTION_SHOW_NONPRINTING);
    else if (ret == 'E')
      options = add_option(options, OPTION_SHOW_ENDS);
    else if (ret == 'T')
      options = add_option(options, OPTION_SHOW_TABS);
    else if (ret == 'e')
      options = add_option(options, OPTION_SHOW_NONPRINTING | OPTION_SHOW_ENDS);
    else if (ret == 't')
      options = add_option(options, OPTION_SHOW_NONPRINTING | OPTION_SHOW_TABS);
    else if (ret == '?') {
      if (ok) *ok = false;
      do_while = false;
    } else if (ret == -1) {
      do_while = false;
    } else {
      if (ok) *ok = false;
      do_while = false;
    }
  }
  return options;
}

void s21_cat(t_s21_option options, char** filenames) {
  if (!filenames) {
    return;
  }

  while (*filenames) {
    handle_file(options, *filenames);
    ++filenames;
  }
}