#include "grep.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <stdbool.h>

static void print_match(t_s21_option options, const char* filename, const char* line, int line_num) {
    if (!exists_option(options, OPTION_COUNT) && !exists_option(options, OPTION_FILES_WITH_MATCH)) {
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

void handle_file(const regex_t* regex, const char* filename, t_s21_option options)
{
    FILE* file = fopen(filename, "r");
    if (file)
    {
        char* line = NULL;
        size_t len = 0;
        int line_num = 1, match_count = 0;
        while (getline(&line, &len, file) != -1) {
            bool match = (regexec(regex, line, 0, NULL, 0) == 0);
            if (exists_option(options, OPTION_INVERT)) match = !match;
            if (match) {
                match_count++;
                print_match(options, filename, line, line_num);
            }
            line_num++;
        }

        if (match_count > 0 && exists_option(options, OPTION_FILES_WITH_MATCH)) {
            printf("%s\n", filename);
        }
        else if (match_count > 0 && exists_option(options, OPTION_COUNT)) {
            if (!exists_option(options, OPTION_NO_FILENAME)) printf("%s:", filename);
            printf("%d\n", match_count);
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

void s21_grep(t_s21_option options, const char* pattern, char** filenames) {
    regex_t regex;
    int cflags = REG_NOSUB | REG_NEWLINE;
    if (exists_option(options, OPTION_IGNORE_CASE)) cflags |= REG_ICASE;

    if (regcomp(&regex, pattern, cflags) == 0) {
        for (; filenames && *filenames; ++filenames)
        {
            handle_file(&regex, *filenames, options);
        }
        regfree(&regex);
    }
    else {
        S21_PRINT_ERROR("Invalid pattern: %s", pattern);
    }
}
