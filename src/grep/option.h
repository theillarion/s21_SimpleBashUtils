#ifndef S21_GREP_OPTION_H
#define S21_GREP_OPTION_H

#include <stdbool.h>

typedef enum s21_option {
  OPTION_NONE = 0,              // Опции не выбраны
  OPTION_PATTERN = 1 << 0,      // -e: Шаблон
  OPTION_IGNORE_CASE = 1 << 1,  // -i: Игнорировать регистр
  OPTION_INVERT = 1 << 2,  // -v: Инвертировать совпадения
  OPTION_COUNT = 1 << 3,  // -c: Только количество совпавших строк
  OPTION_FILES_WITH_MATCH = 1 << 4,  // -l: Только имена файлов с совпадениями
  OPTION_LINE_NUMBER = 1 << 5,  // -n: Печатать номер строки
  OPTION_NO_FILENAME = 1 << 6,  // -h: Не печатать имя файла
  OPTION_NO_MESSAGES = 1 << 7,  // -s: Подавлять сообщения об ошибках
  OPTION_FILE_PATTERN = 1 << 8,  // -f: Шаблоны из файла
  OPTION_ONLY_MATCHING = 1 << 9,  // -o: Только совпадающие части
} t_s21_option;

t_s21_option add_option(t_s21_option options, t_s21_option option);
bool exists_option(t_s21_option options, t_s21_option option);

#endif  // S21_GREP_OPTION_H
