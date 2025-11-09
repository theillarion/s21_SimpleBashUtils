#include <getopt.h>
#include <stdlib.h>

#include "cat.h"
#include "parser.h"
#include "utility.h"

int main(int argc, char** argv) {
  int exit_code = EXIT_SUCCESS;
  bool ok = true;
  t_s21_option options = s21_parse_arguments(argc, argv, &ok);

  if (ok) {
    exit_code = (s21_cat(options, &argv[optind])) ? EXIT_SUCCESS : EXIT_FAILURE;
  } else {
    exit_code = EXIT_FAILURE;
    S21_PRINT_ERROR("Try '%s --help' for more information.\n", argv[0]);
  }

  return exit_code;
}
