#include "parser.h"

#define _GNU_SOURCE  // for getline
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"
#include "utility.h"

static t_parse_result s21_init_parse_result() {
  t_parse_result result = {
      .ok = true, .options = OPTION_NONE, .patterns = NULL, .filenames = NULL};
  return result;
}

void s21_destroy_parse_result(t_parse_result* result) {
  if (!result) return;

  destroy_strings(&result->patterns);
  memset((void*)result, 0, sizeof(*result));
}

static const char* short_opts = "e:f:ivclnhso";

static bool handle_filename_patterns(char*** patterns, const char* filename) {
  if (!patterns || !filename) return false;

  bool ret = true;
  FILE* file = fopen(filename, "r");
  if (file) {
    char* line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != EOF) {
      const size_t line_len = strlen(line);
      if (line_len > 0 && line[line_len - 1] == '\n') {
        line[line_len - 1] = '\0';
      }
      append_string(patterns, line);
    }
    free(line);
    fclose(file);
  } else {
    S21_PRINT_ERROR("%s: %s", filename, strerror(errno));
    errno = 0;
    ret = false;
  }
  return ret;
}

t_parse_result s21_parse_arguments(int argc, char** argv) {
  t_parse_result result = s21_init_parse_result();
  bool do_while = true;

  while (do_while && result.ok) {
    int ret = getopt_long(argc, argv, short_opts, NULL, NULL);
    if (ret == 'e') {
      result.ok = append_string(&result.patterns, optarg);
    } else if (ret == 'i') {
      result.options = add_option(result.options, OPTION_IGNORE_CASE);
    } else if (ret == 'v') {
      result.options = add_option(result.options, OPTION_INVERT);
    } else if (ret == 'c') {
      result.options = add_option(result.options, OPTION_COUNT);
    } else if (ret == 'l') {
      result.options = add_option(result.options, OPTION_FILES_WITH_MATCH);
    } else if (ret == 'n') {
      result.options = add_option(result.options, OPTION_LINE_NUMBER);
    } else if (ret == 'h') {
      result.options = add_option(result.options, OPTION_NO_FILENAME);
    } else if (ret == 's') {
      result.options = add_option(result.options, OPTION_NO_MESSAGES);
    } else if (ret == 'f') {
      result.ok = handle_filename_patterns(&result.patterns, optarg);
    } else if (ret == 'o') {
      result.options = add_option(result.options, OPTION_ONLY_MATCHING);
    } else if (ret == '?') {
      result.ok = false;
    } else if (ret == -1) {
      do_while = false;
    } else {
      result.ok = false;
    }
  }
  // Если паттерн не задан через -e или -f, берем первый не-опционный аргумент
  if (count_strings(result.patterns) == 0) {
    result.ok = (optind < argc)
                    ? append_string(&result.patterns, argv[optind++])
                    : false;
  }
  if (optind < argc) {
    result.filenames = &argv[optind];
  } else {
    result.ok = false;
  }

  return result;
}
