BUILD_DIR	=	.
NAME		=	$(BUILD_DIR)/s21_cat

CC			=	gcc
CFLAGS		=	-Wall -Werror -Wextra -std=c11

SRCS		=	main.c cat.c option.c
HEADERS		=	cat.h option.h
OBJS		=	$(SRCS:%.c=$(BUILD_DIR)/%.o)

INCLUDES	=	-I .
RM			=	rm -rf

all					:	$(NAME)

$(BUILD_DIR)/%.o	:	%.c $(HEADERS)
						$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.PHONY				:	all clean rebuild run

$(NAME)				:	$(OBJS)
						$(CC) $^ -o $@

clean				:	
						$(RM) $(OBJS) $(OBJS_D)
						$(RM) $(NAME)

rebuild				:	clean all

run					:	$(NAME)
						@./$(NAME)

test				:	rebuild
						@bash test/test_cat.sh

format				:
						@cp ../../materials/linters/.clang-format .
						@clang-format -i $(SRCS) $(HEADERS)
						@rm .clang-format