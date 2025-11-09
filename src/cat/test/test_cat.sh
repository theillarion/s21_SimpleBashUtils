#!/bin/bash

S21_CAT=../cat/s21_cat
CAT=cat
TESTDIR=test
USE_VALGRIND=false

if [[ "$1" == "--valgrind" ]]; then
  USE_VALGRIND=true
  echo "=== Valgrind mode enabled ==="
fi

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

###################################
#     VALGRIND TESTS (optional)
###################################
if $USE_VALGRIND; then
  echo -e "\n\n=== VALGRIND TESTS ==="

  val_fail=0
  val_total=0
  val_test_num=1

  for flag in "${flags[@]}"; do
    for file in "${files[@]}"; do
      val_total=$((val_total+1))
      echo "[${val_test_num}/${#flags[@]}x${#files[@]}] Memory testing: flag=[$flag] file=[$file]"
      valgrind_output="$TESTDIR/valgrind_out.txt"
      
      valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes $S21_CAT $flag $file > /dev/null 2> "$valgrind_output"

      leaks_ok=$(grep -c "All heap blocks were freed -- no leaks are possible" "$valgrind_output")
      errors_ok=$(grep -c "ERROR SUMMARY: 0 errors from 0 contexts" "$valgrind_output")

      if [ $leaks_ok -eq 1 ] && [ $errors_ok -eq 1 ]; then
        echo -e "\033[0;32mVALGRIND PASS\033[0m\n"
      else
        echo -e "\033[0;31mVALGRIND FAILED\033[0m"
        grep -E "HEAP SUMMARY|in use at exit|total heap usage|ERROR SUMMARY" "$valgrind_output"
        echo -e ""
        val_fail=$((val_fail+1))
      fi

      rm "$valgrind_output"
      val_test_num=$((val_test_num+1))
    done
  done

  if [ $val_fail -eq 0 ]; then
    echo -e "\n\033[0;32mVALGRIND SUCCESS: $val_total tests passed!\033[0m"
  else
    echo -e "\n\033[0;31mVALGRIND FAILED: $val_fail/$val_total tests failed\033[0m"
  fi
fi
