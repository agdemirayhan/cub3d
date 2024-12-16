# Variables
NAME = get_next_line.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = get_next_line.c get_next_line_utils.c
OBJS = $(SRCS:.c=.o)
INCDIR = ../include
RM = rm -f

# Default rule
all: $(NAME)

# Create static library
$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "Built $(NAME)"

# Object files rule
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Clean object files
clean:
	$(RM) $(OBJS)
	@echo "Cleaned object files"

# Clean all generated files
fclean: clean
	$(RM) $(NAME)
	@echo "Cleaned all generated files"

# Recompile everything
re: fclean all

# Declare phony targets
.PHONY: all clean fclean re
