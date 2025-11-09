#ifndef S21_PARSER_H
#define S21_PARSER_H

#include <stdbool.h>
#include "option.h"

t_s21_option s21_parse_arguments(int argc, char** argv, bool* ok);

#endif // S21_PARSER_H