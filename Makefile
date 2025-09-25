CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./minilibx-linux
SRC = main.c map.c game.c render.c input.c gnl/get_next_line.c gnl/get_next_line_utils.c
OBJDIR = obj
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
NAME = cub3D
LIBS = -Lminilibx-linux -lmlx -lXext -lX11 -lm

all: minilibx $(OBJDIR) $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

minilibx:
	$(MAKE) -C minilibx-linux

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LIBS)

clean:
	$(MAKE) -C minilibx-linux clean
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all