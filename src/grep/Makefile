NAME = s21_grep
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
SRCS = grep.c main.c option.c parser.c strings.c
OBJS = $(SRCS:.c=.o)
INCLUDES = -I .
RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(INCLUDES) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(NAME)

rebuild: clean all

run: $(NAME)
	@./$(NAME)

test: rebuild
	@bash test/test_grep.sh