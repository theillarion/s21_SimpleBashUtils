#!/bin/bash

S21_GREP=../grep/s21_grep
GREP=grep
TESTDIR=test

flags=(
    # Одиночные флаги
    "-e Hello"
    "-e World"
    "-i -e hello"
    "-v -e Test"
    "-c -e Hello"
    "-l -e Tab"
    "-n -e World"
    "-h -e Hello"
    "-s -e Hello"
    "-f $TESTDIR/patterns.txt"
    "-o -e Hello"
    "-e Line"
    "-e spaces"
    "-e Tabbed"
    "-e 1234567890"
    "-e !@#"
    "-e End"
    "-i -e line"
    "-v -e Line"
    "-c -e Line"
    "-n -e Line"
    "-o -e Line"
    "-e Line -e End"
    "-e Line -v -e End"
    "-e Line -c -e End"
    "-e Line -n -e End"
    "-e Line -o -e End"
    # Комбинации флагов
    "-e Hello -e World"
    "-e Hello -e World -i"
    "-e Hello -e World -v"
    "-e Hello -e World -c"
    "-e Hello -e World -l"
    "-e Hello -e World -n"
    "-e Hello -e World -h"
    "-e Hello -e World -s"
    "-e Hello -e World -o"
    "-e Hello -f $TESTDIR/patterns.txt"
    "-e Hello -f $TESTDIR/pattern_variants.txt"
    "-e Hello -f $TESTDIR/patterns.txt -i"
    "-e Hello -f $TESTDIR/patterns.txt -v"
    "-e Hello -f $TESTDIR/patterns.txt -c"
    "-e Hello -f $TESTDIR/patterns.txt -l"
    "-e Hello -f $TESTDIR/patterns.txt -n"
    "-e Hello -f $TESTDIR/patterns.txt -h"
    "-e Hello -f $TESTDIR/patterns.txt -s"
    "-e Hello -f $TESTDIR/patterns.txt -o"
    "-f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt"
    "-f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt -i"
    "-f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt -v"
    "-f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt -c"
    "-f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt -l"
    "-f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt -n"
    "-f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt -h"
    "-f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt -s"
    "-f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt -o"
    # Краевые случаи
    "-e ''"
    "-e Hello -e ''"
    "-e Hello -e World -f $TESTDIR/patterns.txt -f $TESTDIR/patterns2.txt -i -v -c -n -h -s -o"
    "-lche -f $TESTDIR/pattern_space.txt"
    "-isnhco -e Hello $TESTDIR/file3.txt"
    # Пары и тройки
    "-iv -e Hello"
    "-in -e Hello"
    "-cv -e Hello"
    "-cl -e Hello"
    "-cn -e Hello"
    "-lv -e Hello"
    "-ln -e Hello"
    "-hn -e Hello"
    "-sn -e Hello"
    "-on -e Hello"
    # Несколько файлов
    "-e Hello $TESTDIR/file1.txt $TESTDIR/file2.txt"
)

files=(
    "$TESTDIR/file1.txt"
    "$TESTDIR/file2.txt"
    "$TESTDIR/file3.txt"
    "$TESTDIR/emptylines.txt"
    "$TESTDIR/case_mixed.txt"
    "$TESTDIR/specials.txt"
)

fail=0
total=0

test_num=1

# Обычные тесты
for flag in "${flags[@]}"; do
  for file in "${files[@]}"; do
    total=$((total+1))
    echo "[${test_num}/${#flags[@]}x${#files[@]}] Testing: flag=[$flag] file=[$file]"
    $S21_GREP $flag "$file" > "$TESTDIR/s21_grep_out.txt" 2> "$TESTDIR/s21_grep_err.txt"
    $GREP $flag "$file" > "$TESTDIR/grep_out.txt" 2> "$TESTDIR/grep_err.txt"
    diff -q "$TESTDIR/s21_grep_out.txt" "$TESTDIR/grep_out.txt" > /dev/null && diff -q "$TESTDIR/s21_grep_err.txt" "$TESTDIR/grep_err.txt" > /dev/null
    if [ $? -ne 0 ]; then
      echo -e "\033[0;31mFAILED\033[0m"
      echo -e "\033[0;31mFAIL: flag=[$flag] file=[$file]\033[0m"
      fail=$((fail+1))
    else
      echo -e "\033[0;32mPASS\033[0m"
    fi
    rm "$TESTDIR/s21_grep_out.txt" "$TESTDIR/grep_out.txt" "$TESTDIR/s21_grep_err.txt" "$TESTDIR/grep_err.txt"
    test_num=$((test_num+1))
  done
done

if [ $fail -eq 0 ]; then
  echo -e "\033[0;32mSUCCESS: $total tests passed!\033[0m"
else
  echo -e "\033[0;31mFAILED: $fail/$total tests failed\033[0m"
fi
