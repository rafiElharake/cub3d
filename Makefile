NAME		= cub3D

# Directories
SRC_DIR		= src
INC_DIR		= includes
GNL_DIR		= gnl
OBJ_DIR		= obj
MLX_DIR		= minilibx-linux
MLX_LIB		= $(MLX_DIR)/libmlx.a

# Source files
SRCS		= $(SRC_DIR)/main.c \
			  $(SRC_DIR)/game.c \
			  $(SRC_DIR)/input.c \
			  $(SRC_DIR)/render.c \
			  $(SRC_DIR)/map.c \
			  $(SRC_DIR)/parsing.c \
			  $(SRC_DIR)/validation.c \
			  $(GNL_DIR)/get_next_line.c \
			  $(GNL_DIR)/get_next_line_utils.c

# Object files - mirror source structure in obj/
OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS		:= $(OBJS:$(GNL_DIR)/%.c=$(OBJ_DIR)/gnl/%.o)

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I$(INC_DIR) -I$(GNL_DIR) -I$(MLX_DIR)
LDFLAGS		= -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

# Colors for pretty output
GREEN		= \033[0;32m
BLUE		= \033[0;34m
YELLOW		= \033[0;33m
RED			= \033[0;31m
RESET		= \033[0m

# Rules
all: mlx $(NAME)

# Compile MinilibX
mlx:
	@if [ ! -f $(MLX_LIB) ]; then \
		echo "$(YELLOW)Compiling MinilibX...$(RESET)"; \
		make -C $(MLX_DIR) > /dev/null 2>&1; \
		echo "$(GREEN)✓ MinilibX compiled!$(RESET)"; \
	fi

$(NAME): $(OBJS) $(MLX_LIB)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) created successfully!$(RESET)"

# Create object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create object files from GNL
$(OBJ_DIR)/gnl/%.o: $(GNL_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/gnl
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create obj directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/gnl

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@make -C $(MLX_DIR) clean > /dev/null 2>&1
	@echo "$(GREEN)✓ Object files cleaned!$(RESET)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)✓ $(NAME) removed!$(RESET)"

re: fclean all

# Bonus rules (if needed later)
bonus: all

# Print variables for debugging
debug:
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"

.PHONY: all clean fclean re bonus debug