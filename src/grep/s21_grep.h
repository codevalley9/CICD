#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 3000

// enum { SUCCESS, NO_ARGUMENT_FOR_FLAG_F, NO_SUCH_FLAG };

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int stdin_flag;
  int flag_count;
} command_line;

void read_flag(command_line* arg, char* pattern, int argc, char* argv[]);
void flag_F(int* str_count, char* pattern);
void flag_working(command_line* arg, char* pattern, int file_count,
                  char** argv);
void check_flag_i(command_line* arg, char* pattern, regex_t* regex);
void check_flag_v(command_line* arg, int* match);
void check_flag_c(char** argv, command_line* arg, int* file_count,
                  int* line_matches);
void check_flag_o(command_line* arg, int* line_add, char* buffer, int* match,
                  regmatch_t* regmatch, regex_t* regex);
void print_error();

#endif  // GREP_S21_GREP_H //зачем это?