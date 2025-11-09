#include "grep.h"

#define _GNU_SOURCE
#include <errno.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"
#include "utility.h"

static bool exists_newline(const char* line) {
  size_t len = strlen(line);
  return (len > 0 && line[len - 1] == '\n');
}

static void print_match(t_s21_option options, const char* filename,
                        const char* line, int line_num) {
  if (exists_option(options, OPTION_COUNT) ||
      exists_option(options, OPTION_FILES_WITH_MATCH)) {
    return;
  }

  if (!exists_option(options, OPTION_NO_FILENAME)) {
    printf("%s:", filename);
  }
  if (exists_option(options, OPTION_LINE_NUMBER)) {
    printf("%d:", line_num);
  }

  if (exists_newline(line)) {
    printf("%s", line);
  } else {
    printf("%s\n", line);
  }
}

static size_t find_part_mathes(const regex_t* regexes, size_t count_regex,
                               t_s21_option options, const char* filename,
                               const char* line, int line_num) {
  size_t match_count = 0;
  const char* pos = line;
  while (pos) {
    const char* min_pos = pos + strlen(line);
    size_t len_min_pos = 0;
    for (size_t i = 0; i < count_regex; ++i) {
      regmatch_t pmatch = {0};
      if (regexec(&regexes[i], pos, 1, &pmatch, 0) == 0) {
        const char* curr_pos = pos + pmatch.rm_so;
        len_min_pos = (curr_pos < min_pos)
                          ? (size_t)(pmatch.rm_eo - pmatch.rm_so)
                          : len_min_pos;
        min_pos = (curr_pos < min_pos) ? curr_pos : min_pos;
      }
    }

    bool matched = (min_pos < pos + strlen(line));
    if (exists_option(options, OPTION_INVERT)) matched = !matched;
    if (matched) {
      char* match_str = (char*)malloc(len_min_pos + 1);
      if (match_str) {
        strlcpy(match_str, min_pos, len_min_pos + 1);
        print_match(options, filename, match_str, line_num);
        free(match_str);
      }
      pos = min_pos + len_min_pos;
      ++match_count;
    } else {
      pos = NULL;
    }
  }
  return match_count;
}

static size_t find_mathes(const regex_t* regexes, size_t count_regex,
                          t_s21_option options, const char* filename,
                          const char* line, int line_num) {
  size_t match_count = 0;

  if (!exists_option(options, OPTION_ONLY_MATCHING)) {
    bool matched = false;
    for (size_t i = 0; i < count_regex && !matched; ++i) {
      matched |= regexec(&regexes[i], line, 0, NULL, 0) == 0;
    }
    if (exists_option(options, OPTION_INVERT)) matched = !matched;
    if (matched) {
      ++match_count;
      print_match(options, filename, line, line_num);
    }
  } else {
    match_count = find_part_mathes(regexes, count_regex, options, filename,
                                   line, line_num);
  }
  return match_count;
}

static void handle_file(const regex_t* regex, size_t count_regex,
                        t_s21_option options, const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file) {
    char* line = NULL;
    size_t len = 0;
    size_t line_num = 1, match_count = 0;
    while (getline(&line, &len, file) != -1) {
      match_count += (find_mathes(regex, count_regex, options, filename, line,
                                  line_num) > 0)
                         ? 1
                         : 0;
      line_num++;
    }

    if (exists_option(options, OPTION_FILES_WITH_MATCH)) {
      if (match_count > 0) printf("%s\n", filename);
    } else if (exists_option(options, OPTION_COUNT)) {
      if (!exists_option(options, OPTION_NO_FILENAME)) printf("%s:", filename);
      printf("%zu\n", match_count);
    }

    free(line);
    fclose(file);
  } else {
    S21_PRINT_ERROR("%s: %s", filename, strerror(errno));
    errno = 0;
  }
}

bool s21_grep(const t_parse_result* parse_result) {
  if (!parse_result) return false;

  bool ret = true;
  t_s21_option options = parse_result->options;
  int cflags = REG_NEWLINE;
  if (exists_option(options, OPTION_NO_MESSAGES)) S21_SUPRESS_ERRORS();
  if (exists_option(options, OPTION_IGNORE_CASE)) cflags |= REG_ICASE;
  if (count_strings(parse_result->filenames) == 1)
    options = add_option(options, OPTION_NO_FILENAME);

  const size_t count_patterns = count_strings(parse_result->patterns);
  regex_t regex[count_patterns];
  memset(regex, 0, sizeof(regex));

  for (size_t i = 0; ret && i < count_patterns; ++i) {
    if (regcomp(&regex[i], parse_result->patterns[i], cflags) != REG_NOERROR) {
      S21_PRINT_ERROR("invalid pattern: %s", parse_result->patterns[i]);
      ret = false;
    }
  }

  char** filenames = parse_result->filenames;
  while (ret && filenames && *filenames) {
    handle_file(regex, count_patterns, options, *filenames);
    ++filenames;
  }

  for (size_t i = 0; ret && i < count_patterns; ++i) {
    regfree(&regex[i]);
  }

  return ret;
}
