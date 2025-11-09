#include "option.h"
t_s21_option add_option(t_s21_option options, t_s21_option option) {
  return options | option;
}

bool exists_option(t_s21_option options, t_s21_option option) {
  return (options & option) == option;
}
