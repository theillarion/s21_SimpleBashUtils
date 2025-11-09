BUILD_DIR	=	.
NAME		=	$(BUILD_DIR)/s21_grep

CC			=	gcc
CFLAGS		=	-Wall -Werror -Wextra -std=c11

SRCS 		= 	grep.c main.c option.c parser.c strings.c
HEADERS		= 	grep.h option.h parser.h strings.h
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
						@bash test/test_grep.sh

format				:
						@cp ../../materials/linters/.clang-format .
						@clang-format -i $(SRCS) $(HEADERS)
						@rm .clang-format

check 				:	$(SRCS)
						@cppcheck --enable=all $(SRCS) $(INCLUDES) --suppress=missingIncludeSystem --check-level=exhaustive

.PHONY				:	all clean rebuild test format check