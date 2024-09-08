CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11 -D_GNU_SOURCE
SOURCES=s21_grep.c
COMM=-o s21_grep

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) $(SOURCES) $(COMM)

clean: 
	rm -rf s21_grep
	rm -rf *.o s21_grep a.out
	rm -rf *.o *_exe *.dSYM *.out

test: s21_grep
	sh test_func_grep.sh

test_linux: s21_grep
	bash test_func_grep.sh

rebuild: clean all

leak: s21_grep
	leaks --atExit -- ./s21_grep -e int s21_grep.c test_file/all_flag_tests/*

test_leak: s21_grep
	sh test_leak_grep.sh

check: s21_grep
	cp ../../materials/linters/.clang-format ../../src/.clang-format
	clang-format -n s21_grep.c s21_grep.h
	rm ../.clang-format

test_valgrind:
	sh ~/C3_SimpleBashUtils-0/materials/build/run.sh

.PHONY: s21_grep rebuild clean all
