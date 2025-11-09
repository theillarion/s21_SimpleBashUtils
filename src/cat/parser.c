#include "parser.h"

#include <getopt.h>
#include <stdlib.h>

#include "option.h"

static const char* short_opt_str = "bnsvETet";
static const struct option long_opt_str[] = {{"number-nonblank", 0, NULL, 'b'},
                                             {"number", 0, NULL, 'n'},
                                             {"squeeze-blank", 0, NULL, 's'}};

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
