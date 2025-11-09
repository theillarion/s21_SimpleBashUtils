BUILD_DIR	=	.
NAME		=	$(BUILD_DIR)/s21_cat

CC			=	gcc
CFLAGS		=	-Wall -Werror -Wextra -std=c11

SRCS		=	cat.c main.c option.c parser.c
HEADERS		=	cat.h option.h utility.h parser.h
OBJS		=	$(SRCS:%.c=$(BUILD_DIR)/%.o)

INCLUDES	=	-I .
RM			=	rm -rf

all					:	$(NAME)

$(BUILD_DIR)/%.o	:	%.c $(HEADERS)
						$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(NAME)				:	$(OBJS)
						$(CC) $^ -o $@

clean				:	
						$(RM) $(OBJS) $(OBJS_D)
						$(RM) $(NAME)

rebuild				:	clean all

test				:	all
						@bash test/test_cat.sh

format				:
						@cp ../../materials/linters/.clang-format .
						@clang-format -i $(SRCS) $(HEADERS)
						@rm .clang-format

check 				:	$(SRCS)
						@cppcheck --enable=all $(SRCS) $(INCLUDES) --suppress=missingIncludeSystem --check-level=exhaustive

.PHONY				:	all clean rebuild test format check