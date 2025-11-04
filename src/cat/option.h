#ifndef S21_OPTIONS_H
#define S21_OPTIONS_H

#include <stdbool.h>

typedef enum s21_option {
  OPTION_NONE = 0,                  // Опции не выбраны
  OPTION_NUMBER_NONBLANK = 1 << 0,  // -b: Нумерует только непустые строки
  OPTION_SHOW_ENDS = 1 << 1,        // -E: Отображает символы конца строки как $
  OPTION_NUMBER_ALL = 1 << 2,       // -n: Нумерует все выходные строки
  OPTION_SQUEEZE_BLANKS = 1 << 3,  // -s: Сжимает несколько смежных пустых строк
  OPTION_SHOW_TABS = 1 << 4,       // -T: Отображает табы как ^I
  OPTION_SHOW_NONPRINTING =
      1 << 5,  // -v: Отображает все непечаемые символы, кроме $ и ^I
} t_s21_option;

t_s21_option add_option(t_s21_option options, t_s21_option option);
t_s21_option remove_option(t_s21_option options, t_s21_option option);
bool exists_option(t_s21_option options, t_s21_option option);

#endif  // S21_OPTIONS_H
