
#include <stdlib.h>

#include "grep.h"
#include "parser.h"
#include "utility.h"

int main(int argc, char** argv) {
  int exit_code = EXIT_SUCCESS;
  t_parse_result parse_result = s21_parse_arguments(argc, argv);

  if (parse_result.ok) {
    exit_code = (s21_grep(&parse_result)) ? EXIT_SUCCESS : EXIT_FAILURE;
  } else {
    S21_PRINT_ERROR("Usage: %s [OPTIONS] PATTERN [FILE...]\n", argv[0]);
    exit_code = EXIT_FAILURE;
  }
  s21_destroy_parse_result(&parse_result);
  return exit_code;
}
