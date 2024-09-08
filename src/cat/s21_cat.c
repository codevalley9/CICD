#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;
  int n;
  int s;
  int T;
  int E;
  int v;
} Flag;

int parser(int argc, char *argv[], Flag *flags);
void cat(int argc, char *argv[], Flag *flags);

int main(int argc, char *argv[]) {
  Flag flags = {0};

  if (!(parser(argc, argv, &flags))) {
    cat(argc, argv, &flags);
  } else {
    fprintf(stderr, "Mistake\n");
  }
  return 0;
}

int parser(int argc, char *argv[], Flag *flags) {
  int current_flag;
  opterr = 0;
  int Error = 0;

  const struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                        {"number", 0, NULL, 'n'},
                                        {"squeeze-blank", 0, NULL, 's'},
                                        {NULL, 0, NULL, 0}};
  while ((current_flag =
              getopt_long(argc, argv, "+benstvET", long_options, NULL)) != -1) {
    switch (current_flag) {
      case 'b':
        flags->b = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->T = 1;
        flags->v = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'e':
        flags->E = 1;
        flags->v = 1;
        break;
      case 'T':
        flags->T = 1;
        break;
      case 'E':
        flags->E = 1;
        break;
      default:
        Error = 1;
        break;
    }
  }
  return Error;
}

void cat(int argc, char *argv[], Flag *flags) {
  FILE *file;
  for (; optind < argc; optind++) {
    file = fopen(argv[optind], "r");
    if (file == NULL) {
      fprintf(stderr, "NO SUCH FILE\n");
    } else {
      char str = '\n';
      int symbol, number_line = 0, current_position = 0, current_line = 0,
                  next_line = 0;
      while ((symbol = fgetc(file)) != EOF) {
        if (flags->s) {
          current_line = 0;
          if (symbol == '\n' && current_position == 0) {
            current_line = 1;
          }
          if (symbol == '\n' && current_position != 0) {
            current_position = 0;
          }
          if (symbol != '\n' && current_position == 0) {
            current_position = 1;
          }
          if (current_line && next_line) continue;
          next_line = current_line;
        }
        if (((flags->b || (flags->b && flags->n)) && symbol != '\n' &&
             str == '\n') ||
            (flags->n && !(flags->b) && str == '\n'))
          printf("%6d\t", ++number_line);
        if (flags->E && symbol == '\n') putchar('$');
        if (flags->T && symbol == '\t') {
          printf("^");
          symbol = 'I';
        }
        if (flags->v) {
          if (symbol < 9 || (symbol > 10 && symbol < 32)) {
            printf("^");
            symbol += 64;
          }
          if (symbol == 127) {
            printf("^");
            symbol -= 64;
          }
          if (symbol > 127) {
            printf("M-^");
            symbol -= 64;
          }
        }
        str = symbol;
        putchar(str);
      }
    }
    fclose(file);
  }
}
