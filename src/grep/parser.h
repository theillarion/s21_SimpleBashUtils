#ifndef S21_PARSER_H
#define S21_PARSER_H

#include <stdbool.h>
#include "option.h"

typedef struct parse_result {
    bool ok;
    t_s21_option options;
    char** patterns;
    char** filenames;
}   t_parse_result;

t_parse_result s21_init_parse_result();
void s21_destroy_parse_result(t_parse_result* result);

t_parse_result s21_parse_arguments(int argc, char** argv);


#endif // S21_PARSER_H