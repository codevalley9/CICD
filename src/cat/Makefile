NAME=s21_cat
CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -g 

.PHONY: s21_grep test style clean rebuild

all: $(NAME)

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) $(NAME).c -o $(NAME)

clean:
	rm -rf $(NAME) *.log *.dSYM

docker_test:
	sh ../../materials/build/run.sh

test:
	sh test_func_cat.sh
test_linux:
	bash test_func_cat.sh
style:
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c
	rm -rf .clang-format
leak:
	leaks -atExit -- ./$(NAME)

rebuild:
	$(MAKE) clean
	$(MAKE) all
