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
    "-e Hello $TESTDIR/file1.txt $TESTDIR/emptylines.txt"
    "-e Hello $TESTDIR/file2.txt $TESTDIR/emptylines.txt"
    # Пустой файл
    "-e Hello $TESTDIR/emptylines.txt"
)

files=(
    "$TESTDIR/file1.txt"
    "$TESTDIR/file2.txt"
    "$TESTDIR/file3.txt"
    "$TESTDIR/emptylines.txt"
    "$TESTDIR/patterns.txt"
    "$TESTDIR/patterns2.txt"
    "$TESTDIR/invalid_pattern.txt"
)

fail=0
total=0

for flag in "${flags[@]}"; do
  for file in "${files[@]}"; do
    total=$((total+1))
    echo $flag $file
    $S21_GREP $flag "$file" > "$TESTDIR/s21_grep_out.txt" 2> "$TESTDIR/s21_grep_err.txt"
    $GREP $flag "$file" > "$TESTDIR/grep_out.txt" 2> "$TESTDIR/grep_err.txt"
    diff -q "$TESTDIR/s21_grep_out.txt" "$TESTDIR/grep_out.txt" > /dev/null && diff -q "$TESTDIR/s21_grep_err.txt" "$TESTDIR/grep_err.txt" > /dev/null
    if [ $? -ne 0 ]; then
      echo -e "\033[0;31mFAIL: flag=[$flag] file=[$file]\033[0m"
      fail=$((fail+1))
    fi
    rm "$TESTDIR/s21_grep_out.txt" "$TESTDIR/grep_out.txt" "$TESTDIR/s21_grep_err.txt" "$TESTDIR/grep_err.txt"
  done
done

if [ $fail -eq 0 ]; then
  echo -e "\033[0;32mSUCCESS: $total tests passed!\033[0m"
else
  echo -e "\033[0;31mFAILED: $fail/$total tests failed\033[0m"
fi
