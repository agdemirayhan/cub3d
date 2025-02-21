NAME := cub3D

OBJ_DIR := objs
MLX_DIR := ./MLX42
LIBFT := ./libft
GNL := ./get_next_line

HEADERS := -I ./include -I $(MLX_DIR)/include -I $(LIBFT)/include -I $(GNL)/include
LIBS := $(MLX_DIR)/build/libmlx42.a $(LIBFT)/libft.a $(GNL)/get_next_line.a -ldl -lglfw -pthread -lm

SRCS := cub3d.c map_utils.c hooks.c
OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)

RM := rm -rf
CC := cc
MAKE := make
MKDIR := mkdir -p

# CFLAGS := -Wall -Wextra -Werror -fsanitize=address
CFLAGS := -fsanitize=address
LIB_FLAGS := -framework Cocoa -framework OpenGL -framework IOKit $(LIBS) -g

# Detect Homebrew GLFW installation
BREW_GLFW := $(shell brew --prefix glfw 2>/dev/null)
ifeq ($(BREW_GLFW),)
    $(error GLFW not found. Install it with 'brew install glfw')
endif

export LDFLAGS := -L$(BREW_GLFW)/lib
export CPPFLAGS := -I$(BREW_GLFW)/include
export PKG_CONFIG_PATH := $(BREW_GLFW)/lib/pkgconfig

all: libmlx libft get_next_line $(NAME)

libmlx:
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "MLX42 not found. Cloning..."; \
		git clone https://github.com/codam-coding-college/MLX42.git $(MLX_DIR); \
	fi
	@echo "Start compiling MLX..."
	@cd $(MLX_DIR) && cmake -B build && $(MAKE) -C build -j4
	@echo "Done compiling MLX"

libft:
	@$(MAKE) -C $(LIBFT)

get_next_line:
	@$(MAKE) -C $(GNL)

$(NAME): $(OBJ_DIR) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(HEADERS) -o $(NAME) $(LIB_FLAGS) `pkg-config --cflags --libs glfw3`
	@echo "Successful build!"

clean:
	@$(RM) $(OBJ_DIR)
	@$(RM) $(MLX_DIR)/build
	@$(MAKE) -C $(LIBFT) clean
	@$(MAKE) -C $(GNL) clean
	@echo "Bins successfully cleaned!"

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT) fclean
	@$(MAKE) -C $(GNL) fclean
	@$(RM) $(MLX_DIR)
	@echo "Everything successfully cleaned!"

re: fclean all

$(OBJ_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

$(OBJ_DIR):
	@echo "Creating object directory..."
	$(MKDIR) $(OBJ_DIR)

.PHONY: all clean fclean re libmlx libft get_next_line