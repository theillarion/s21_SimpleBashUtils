#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "grep.h"
#include "option.h"

static const char* short_opts = "e:f:ivclnhso";

t_s21_option parse_arguments(int argc, char** argv, char** pattern, char** filename_pattern, bool* ok) {
    t_s21_option options = OPTION_NONE;
    bool do_while = true;

    *pattern = NULL;
    *filename_pattern = NULL;
    if (ok) *ok = true;

    while (do_while) {
        int ret = getopt_long(argc, argv, short_opts, NULL, NULL);
        switch (ret) {
            case 'e':
                *pattern = optarg;
                options = add_option(options, OPTION_PATTERN);
                break;
            case 'i':
                options = add_option(options, OPTION_IGNORE_CASE);
                break;
            case 'v':
                options = add_option(options, OPTION_INVERT);
                break;
            case 'c':
                options = add_option(options, OPTION_COUNT);
                break;
            case 'l':
                options = add_option(options, OPTION_FILES_WITH_MATCH);
                break;
            case 'n':
                options = add_option(options, OPTION_LINE_NUMBER);
                break;
            case 'h':
                options = add_option(options, OPTION_NO_FILENAME);
                break;
            case 's':                                                                                                                                                                                                                                            │
                options = add_option(options, OPTION_NO_MESSAGES);                                                                                                                                                                                               │
                break;                                                                                                                                                                                                                                           │
            case 'f':                                                                                                                                                                                                                                            │
                options = add_option(options, OPTION_FILE_PATTERN);                                                                                                                                                                                              │
                break;                                                                                                                                                                                                                                           │
            case 'o':                                                                                                                                                                                                                                            │
                options = add_option(options, OPTION_ONLY_MATCHING);                                                                                                                                                                                             │
                break;   
            case '?':
                if (ok) *ok = false;
                break;
        }
    }
    // Если паттерн не задан через -e, берем первый не-опционный аргумент
    if (!*pattern && optind < argc) {
        *pattern = argv[optind++];
    }
    return options;
}

int main(int argc, char** argv) {
    bool ok = true;
    char* pattern = NULL;
    t_s21_option options = parse_arguments(argc, argv, &pattern, &ok);
    if (ok && pattern) {
        s21_grep(options, pattern, &argv[optind]);
    } else {
        fprintf(stderr, "Usage: %s [OPTIONS] PATTERN [FILE...]\n", argv[0]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
