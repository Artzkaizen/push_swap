# Project name
NAME            := push_swap

# Directories
INC_DIR         := includes
SRC_DIR         := src
OBJ_DIR         := obj
TEST_DIR        := test
TEST_OBJ_DIR    := test/obj

# Source files
SRCS            := push_swap.c \
				   utils.c \
				   ft_split.c \
				   ft_printf.c \
				   ft_atoi.c \
				   stack.c \
				   ft_printf_utils.c \
				   test.c \

SRCS		:= $(SRCS:%=$(SRC_DIR)/%)
OBJS		:= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

BONUS		:= push_swap.c

# Compiler and flags
CC              := cc
CFLAGS          := -Wall -Wextra -Werror -I $(INC_DIR)
DEBUG           := -g3


# Clean command
RM              := rm -f

all: $(NAME)

algo: $(NAME)
	./$(NAME) $(ARGS)
# Run tests
test: $(TARGET)
	./$(TARGET)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# bonus
# bonus:	$(OBJS) $(BONUS_OBJS)
# 	ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)

# Clean rule (remove object files)
clean:
	$(RM) *.o

# Full clean (remove object files and static library)
fclean: clean
	$(RM) $(NAME)

# Rebuild the project
re: fclean all

# Phony targets
.PHONY: clean fclean re
