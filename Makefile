NAME = cub3D

SRC_DIR = src
MAP_DIR = src/map
RENDER_DIR = src/render
PRC_DIR = src/parsing
INC_DIR = includes
GNL_DIR = gnl
OBJ_DIR = obj
MLX_DIR = minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.a

SRCS = $(SRC_DIR)/main.c \
	   $(SRC_DIR)/init/game.c \
	   $(SRC_DIR)/init/init_game.c \
	   $(SRC_DIR)/init/close.c \
	   $(SRC_DIR)/input/input.c \
	   $(SRC_DIR)/input/keys.c \
	   $(RENDER_DIR)/render.c \
	   $(RENDER_DIR)/minimap.c \
	   $(RENDER_DIR)/raycast.c \
	   $(RENDER_DIR)/draw_wall.c \
	   $(RENDER_DIR)/draw_scene.c \
	   $(MAP_DIR)/map.c \
	   $(MAP_DIR)/map_loader.c \
	   $(MAP_DIR)/map_config.c \
	   $(MAP_DIR)/map_parse.c \
	   $(MAP_DIR)/map_utils.c \
	   $(MAP_DIR)/map_read.c \
	   $(MAP_DIR)/map_load.c \
	   $(PRC_DIR)/parsing.c \
	   $(PRC_DIR)/parse_texture.c \
	   $(PRC_DIR)/parse_texture_utils.c \
	   $(PRC_DIR)/parse_color.c \
	   $(PRC_DIR)/parse_map_validate.c \
	   $(PRC_DIR)/parse_utils.c \
	   $(PRC_DIR)/parse_utils_2.c \
	   $(PRC_DIR)/parse_helpers.c \
	   $(PRC_DIR)/validation.c \
	   $(GNL_DIR)/get_next_line.c \
	   $(GNL_DIR)/get_next_line_utils.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS := $(OBJS:$(MAP_DIR)/%.c=$(OBJ_DIR)/map/%.o)
OBJS := $(OBJS:$(RENDER_DIR)/%.c=$(OBJ_DIR)/render/%.o)
OBJS := $(OBJS:$(GNL_DIR)/%.c=$(OBJ_DIR)/gnl/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I$(INC_DIR) -I$(GNL_DIR) -I$(MLX_DIR)
LDFLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

all: mlx $(NAME)

mlx:
	@if [ ! -f $(MLX_LIB) ]; then \
		echo "Compiling MinilibX..."; \
		make -C $(MLX_DIR) > /dev/null 2>&1; \
		echo "✓ MinilibX compiled!"; \
	fi

$(NAME): $(OBJS) $(MLX_LIB)
	@echo "Linking $(NAME)..."
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "✓ $(NAME) created successfully!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/map/%.o: $(MAP_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/map
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/render/%.o: $(RENDER_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/render
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/gnl/%.o: $(GNL_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/gnl
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/map
	@mkdir -p $(OBJ_DIR)/render
	@mkdir -p $(OBJ_DIR)/gnl

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@make -C $(MLX_DIR) clean > /dev/null 2>&1
	@echo "✓ Object files cleaned!"

fclean: clean
	@echo "Removing $(NAME)..."
	@rm -f $(NAME)
	@echo "✓ $(NAME) removed!"

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus