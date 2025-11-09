#!/bin/bash

S21_CAT=../cat/s21_cat
CAT=cat
TESTDIR=test

flags=(
    ""
    "-b"
    "-e"
    "-E"
    "-n"
    "-s"
    "-t"
    "-T"
    "-v"
    "-be"
    "-bn"
    "-bs"
    "-bt"
    "-bT"
    "-bE"
    "-bET"
    "-ne"
    "-ns"
    "-nt"
    "-nT"
    "-nE"
    "-nET"
    "-st"
    "-sT"
    "-sE"
    "-sET"
    "-tE"
    "-vE"
    "-vT"
    "-ve"
    "-vet"
    "-vET"
)

files=(
    "$TESTDIR/file1.txt"
    "$TESTDIR/file2.txt"
    "$TESTDIR/emptylines.txt"
    "$TESTDIR/nonprint.bin"
    "$TESTDIR/all_ff.bin"
    "$TESTDIR/all_bytes.txt"
    "$TESTDIR/random.bin"
)

fail=0
total=0

test_num=1

# Обычные тесты
for flag in "${flags[@]}"; do
  for file in "${files[@]}"; do
    total=$((total+1))
    echo "[${test_num}/${#flags[@]}x${#files[@]}] Testing: flag=[$flag] file=[$file]"
    $S21_CAT $flag "$file" > "$TESTDIR/s21_cat_out.txt" 2> "$TESTDIR/s21_cat_err.txt"
    $CAT $flag "$file" > "$TESTDIR/cat_out.txt" 2> "$TESTDIR/cat_err.txt"
    diff -q "$TESTDIR/s21_cat_out.txt" "$TESTDIR/cat_out.txt" > /dev/null && diff -q "$TESTDIR/s21_cat_err.txt" "$TESTDIR/cat_err.txt" > /dev/null
    if [ $? -ne 0 ]; then
      echo -e "\033[0;31mFAILED\033[0m"
      echo -e "\033[0;31mFAIL: flag=[$flag] file=[$file]\033[0m"
      fail=$((fail+1))
    else
      echo -e "\033[0;32mPASS\033[0m"
    fi
    rm "$TESTDIR/s21_cat_out.txt" "$TESTDIR/cat_out.txt" "$TESTDIR/s21_cat_err.txt" "$TESTDIR/cat_err.txt"
    test_num=$((test_num+1))
  done
done

if [ $fail -eq 0 ]; then
  echo -e "\033[0;32mSUCCESS: $total tests passed!\033[0m"
else
  echo -e "\033[0;31mFAILED: $fail/$total tests failed\033[0m"
fi
