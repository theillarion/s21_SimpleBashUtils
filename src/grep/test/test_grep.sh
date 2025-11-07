#!/bin/bash

S21_GREP=../grep/s21_grep
GREP=grep
TESTDIR=test

flags=(
    "-e Hello"
    "-e World"
    "-i -e hello"
    "-v -e Test"
    "-c -e Hello"
    "-l -e Tab"
    "-n -e World"
    "-h -e Hello"
    "-e Tab -n"
    "-e Test -c"
    "-e Hello -v"
    "-e Hello -i"
    "-e Hello -l"
    "-e Hello -h"
    "-e Hello -n"
)

files=(
    "$TESTDIR/file1.txt"
    "$TESTDIR/file2.txt"
    "$TESTDIR/emptylines.txt"
    "$TESTDIR/all_bytes.txt"
)

fail=0
total=0

for flag in "${flags[@]}"; do
  for file in "${files[@]}"; do
    total=$((total+1))
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
