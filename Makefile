CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./minilibx-linux
SRC = main.c
OBJ = $(SRC:.c=.o)
NAME = cub3D
LIBS = -Lminilibx-linux -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LIBS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all