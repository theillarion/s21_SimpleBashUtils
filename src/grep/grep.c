#include "grep.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <stdbool.h>
#include "strings.h"
#include "utility.h"

static void print_match(t_s21_option options, const char* filename, const char* line, int line_num)
{
    if (exists_option(options, OPTION_COUNT) || exists_option(options, OPTION_FILES_WITH_MATCH)) {
        return;
    }

    if (!exists_option(options, OPTION_NO_FILENAME))
    {
        printf("%s:", filename);
    }
    if (exists_option(options, OPTION_LINE_NUMBER))
    {
        printf("%d:", line_num);
    }
    printf("%s", line);
}

static size_t find_mathes(const regex_t* regex, t_s21_option options, const char* filename, const char* line, int line_num)
{
    size_t match_count = 0;

    if (!exists_option(options, OPTION_ONLY_MATCHING)) {
        bool match = regexec(regex, line, 0, NULL, 0) == 0;
        if (exists_option(options, OPTION_INVERT)) match = !match;
        if (match)
        {
            ++match_count;
            print_match(options, filename, line, line_num);
        }
    }
    else if (!exists_option(options, OPTION_INVERT)) {
        const char* pos = line;
        regmatch_t pmatch = {0};
        while (pos && regexec(regex, pos, 1, &pmatch, 0) == 0) {
            size_t match_len = pmatch.rm_eo - pmatch.rm_so;
            char* match_str = (match_len > 0) ? (char*)malloc(match_len + 2) : NULL;
            if (match_str) {
                strncpy(match_str, pos + pmatch.rm_so, match_len);
                match_str[match_len] = '\n';
                match_str[match_len + 1] = '\0';

                print_match(options, filename, match_str, line_num);
                free(match_str);
                ++match_count;
            }
            pos += pmatch.rm_eo;
        }
    }
    return match_count;
}

static void handle_file(const regex_t* regex, t_s21_option options, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file)
    {
        char* line = NULL;
        size_t len = 0;
        size_t line_num = 1, match_count = 0;
        while (getline(&line, &len, file) != -1) {
            match_count += (find_mathes(regex, options, filename, line, line_num) > 0) ? 1 : 0;
            line_num++;
        }

        if (exists_option(options, OPTION_FILES_WITH_MATCH)) {
            if (match_count > 0) printf("%s\n", filename);
        }
        else if (exists_option(options, OPTION_COUNT)) {
            if (!exists_option(options, OPTION_NO_FILENAME)) printf("%s:", filename);
            printf("%zu\n", match_count);
        }
        
        free(line);
        fclose(file);
    }
    else
    {
        S21_PRINT_ERROR("%s: %s", filename, strerror(errno));
        errno = 0;
    }
}

bool s21_grep(const t_parse_result* parse_result) {
    if (!parse_result) return false;

    bool ret = true;
    t_s21_option options = parse_result->options;
    regex_t regex;
    int cflags = REG_NEWLINE;
    if (exists_option(options, OPTION_NO_MESSAGES)) S21_SUPRESS_ERRORS();
    if (exists_option(options, OPTION_IGNORE_CASE)) cflags |= REG_ICASE;
    if (count_strings(parse_result->filenames) == 1) options = add_option(options, OPTION_NO_FILENAME);

    char** patterns = parse_result->patterns;
    while (patterns && *patterns && ret)
    {
        if (regcomp(&regex, *patterns, cflags) == REG_NOERROR) {
            char** filenames = parse_result->filenames;
            while (filenames && *filenames)
            {
                handle_file(&regex, options, *filenames);
                ++filenames;
            }
            regfree(&regex);
        }
        else {
            S21_PRINT_ERROR("invalid pattern: %s", *patterns);
            ret = false;
        }
        ++patterns;
    }

    return ret;
}
