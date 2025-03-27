SRCS_LIST = cub3d.c move.c map_utils.c map_check.c hooks.c map_colors.c \
			map_parse.c map_textures.c map_which_textures.c map_loading.c \
			map_parse_utils.c

SRCS = $(SRCS_LIST)
OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS_LIST:.c=.o))

NAME = cube3d

# ------------------------------ COMPILATION ------------------------------
CC = cc
CFLAGS = -fsanitize=address
# FLAGS = -Wall -Wextra -Werror
RM = rm -rf

# ------------------------------ LIBRARIES ------------------------------
LIBFT = libft/libft.a
MLX_DIR = minilibx
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_INCLUDE = -I$(MLX_DIR)

GNL_DIR = libft
GNL_LIB = $(GNL_DIR)/get_next_line.a
GNL_INCLUDE = -I$(GNL_DIR)

ifeq ($(shell uname), Darwin)  # macOS
	MLX_FLAGS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
else  # Linux
	MLX_FLAGS = -L$(MLX_DIR) -lmlx -lX11 -lXext -lm -lbsd
endif

# ------------------------------ BUILD RULES ------------------------------
all: submodule $(NAME)

$(NAME): $(OBJS) | $(LIBFT) $(MLX_LIB) $(GNL_LIB)
	$(CC) $(CFLAGS) $(LIBFT) $(GNL_LIB) $(MLX_FLAGS) $(OBJS) -o $(NAME)

$(LIBFT):
	@make -C libft

$(MLX_LIB):
	@make -C $(MLX_DIR)

$(GNL_LIB):
	@make -C $(GNL_DIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(MLX_INCLUDE) $(GNL_INCLUDE) -c $< -o $@

# ------------------------------ CLEAN RULES ------------------------------
clean:
	@make clean -C libft
	@make clean -C $(MLX_DIR)
	@make clean -C $(GNL_DIR)
	$(RM) $(OBJDIR)

fclean: clean
	@make fclean -C libft
	@make clean -C $(MLX_DIR)
	@make fclean -C $(GNL_DIR)
	$(RM) $(NAME)

re: fclean all

leaks: $(MAKE) clean_objs
	$(MAKE) CFLAGS="$(CFLAGS) -g -DLEAKS" all

clean_objs:
	$(RM) $(OBJS)

submodule:
	@if [ ! -f "./libft/.git" ] && [ ! -d "./libft/.git" ]; then \
		echo "libft submodule not found. Initializing and updating libft submodule..."; \
		git submodule update --init --recursive libft; \
	else \
		echo "libft submodule already initialized. Updating libft submodule..."; \
		git submodule update --remote libft; \
	fi

.PHONY: all clean fclean re
