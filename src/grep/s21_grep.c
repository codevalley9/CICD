#include "s21_grep.h"

int main(int argc, char* argv[]) {
  command_line arg = {0};
  char* pattern = calloc(SIZE, sizeof(char*));
  read_flag(&arg, pattern, argc, argv);
  int file_count = argc - optind;

  if (argc == 1) print_error();
  if (argc == 2) {
    if (arg.flag_count == 0) {
      strcpy(pattern, argv[1]);
      arg.stdin_flag = 1;
    }
    if (arg.flag_count != 0) print_error();
  }
  if (argc == 3) {
    if (argc > arg.flag_count + 1) {
      if (arg.flag_count != 0) {
        strcpy(pattern, argv[2]);
        arg.stdin_flag = 1;
      }
      if (arg.flag_count == 0) {
        strcpy(pattern, argv[1]);
        arg.stdin_flag = 0;
      }

    } else
      print_error();
  }
  while (optind < argc) {
    flag_working(&arg, pattern, file_count, argv);
    optind++;
  }

  free(pattern);
  return 0;
}

void read_flag(command_line* p_arg, char* pattern, int argc, char* argv[]) {
  int flag_opt = 0;
  int pattern_count = 0;
  const struct option long_options[] = {{NULL, 0, NULL, 0}};
  while ((flag_opt = getopt_long(argc, argv, "e:ivclnhsof:", long_options,
                                 NULL)) != -1) {
    switch (flag_opt) {
      case 'e':
        p_arg->e = 1;
        pattern_count++;
        if (pattern_count > 1) {
          snprintf(&pattern[strlen(pattern)], SIZE, "%s", "|");
        }
        snprintf(&pattern[strlen(pattern)], SIZE, "%s", optarg);
        break;
      case 'i':
        p_arg->i = 1;
        p_arg->flag_count++;
        break;
      case 'v':
        p_arg->v = 1;
        p_arg->flag_count++;
        break;
      case 'c':
        p_arg->c = 1;
        p_arg->flag_count++;
        break;
      case 'l':
        p_arg->l = 1;
        p_arg->flag_count++;
        break;
      case 'n':
        p_arg->n = 1;
        p_arg->flag_count++;
        break;
      case 'h':
        p_arg->h = 1;
        p_arg->flag_count++;
        break;
      case 'f':
        p_arg->f = 1;
        p_arg->flag_count++;
        flag_F(&pattern_count, pattern);
        break;
      case 's':
        p_arg->s = 1;
        p_arg->flag_count++;
        break;
      case 'o':
        p_arg->o = 1;
        p_arg->flag_count++;
        break;
      case '?':
        fprintf(stderr,
                "usage: ./s21_grep [-chilnosv][-e pattern][-f file][pattern] "
                "[file ...]\n");
        break;
    }
  }
  if (!p_arg->e && !p_arg->f)
    snprintf(&pattern[strlen(pattern)], SIZE, "%s", argv[optind++]);
}

void flag_F(int* pattern_count, char* pattern) {
  FILE* file_f = fopen(optarg, "r");
  char* buffer = calloc(SIZE, sizeof(char*));
  if (file_f) {
    while (fgets(buffer, SIZE, file_f) != NULL) {
      if (buffer[strlen(buffer) - 1] == '\n' && strlen(buffer) > 1)
        buffer[strlen(buffer) - 1] = '\0';
      if (*pattern_count > 0)
        snprintf(&pattern[strlen(pattern)], SIZE, "%s", "|");
      snprintf(&pattern[strlen(pattern)], SIZE, "%s", buffer);
      (*pattern_count)++;
    }
    fclose(file_f);
  } else {
    fprintf(stderr, "s21_grep: %s: No file or directory\n", optarg);
  }
  free(buffer);
}

void flag_working(command_line* arg, char* pattern, int file_count,
                  char** argv) {
  char* buffer = calloc(SIZE, sizeof(char*));
  int match = 0;
  regex_t regex;
  regmatch_t regmatch[1] = {0};
  int line_add = 1;

  FILE* fp = NULL;
  if ((fp = fopen(argv[optind], "r")) != NULL) {
    int line_number = 1;
    int line_matches = 0;
    check_flag_i(arg, pattern, &regex);
    while (!feof(fp)) {
      if (fgets(buffer, SIZE, (arg->stdin_flag ? stdin : fp))) {
        match = regexec(&regex, buffer, 1, regmatch, 0);
        check_flag_v(arg, &match);
        if (match != REG_NOMATCH) {
          if (!arg->c && !arg->l) {
            if (!arg->h && file_count > 1) printf("%s:", argv[optind]);
            if (arg->n) printf("%d:", line_number);
            check_flag_o(arg, &line_add, buffer, &match, regmatch, &regex);
            if (!arg->o) printf("%s", buffer);
            if (buffer[strlen(buffer) - 1] != '\n' && line_add) printf("\n");
          }
          line_matches++;
        }
        line_number++;
      }
    }
    check_flag_c(argv, arg, &file_count, &line_matches);
    if (arg->l && line_matches) printf("%s\n", argv[optind]);
    regfree(&regex);
    fclose(fp);
  } else if (!arg->s) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[optind]);
  }
  free(buffer);
}

void check_flag_i(command_line* arg, char* pattern, regex_t* regex) {
  if (arg->i) {
    regcomp(regex, pattern, REG_ICASE | REG_EXTENDED);
  } else {
    regcomp(regex, pattern, REG_EXTENDED | REG_NEWLINE);
  }
}

void check_flag_v(command_line* arg, int* match) {
  if (arg->v) {
    arg->o = 0;
    if ((*match) == 1) {
      (*match) = 0;
    } else {
      (*match) = 1;
    }
  }
}

void check_flag_c(char** argv, command_line* arg, int* file_count,
                  int* line_matches) {
  if (arg->c) {
    if (!arg->h && (*file_count) > 1) printf("%s:", argv[optind]);
    if (arg->l && (*line_matches))
      printf("1\n");
    else
      printf("%d\n", (*line_matches));
  }
}

void check_flag_o(command_line* arg, int* line_add, char* buffer, int* match,
                  regmatch_t* regmatch, regex_t* regex) {
  if (arg->o && !arg->v) {
    (*line_add) = 0;
    char* ptr = buffer;
    while (!(*match) && regmatch[0].rm_eo != regmatch[0].rm_so) {
      printf("%.*s\n", (int)(regmatch[0].rm_eo - regmatch[0].rm_so),
             ptr + regmatch[0].rm_so);
      ptr += regmatch[0].rm_eo;
      (*match) = regexec(regex, ptr, 1, regmatch, REG_NOTBOL);
    }
  }
}

void print_error() {
  printf(
      "Please use: ./s21_grep [-chilnosv][-e pattern][-f file][pattern] "
      "[file ...]\n");
}