#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <string.h>
# include <errno.h>
# include "../minilibx-linux/mlx.h"
# include "../gnl/get_next_line.h"

# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080
# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

# define MOVE_SPEED 0.2
# define ROTATION_SPEED 0.001
# define CELL_SIZE 4

typedef struct s_texture
{
	void	*img;
	char	*data;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}	t_texture;

typedef struct s_input
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_input;

typedef struct s_parse
{
	int	has_north;
	int	has_south;
	int	has_east;
	int	has_west;
	int	has_floor;
	int	has_ceiling;
	int	player_count;
	int	map_started;
	int	map_line_count;
}	t_parse;

typedef struct s_game
{
	void		*mlx;
	void		*window;
	void		*image;
	char		*image_data;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	char		**map;
	int			map_width;
	int			map_height;
	t_input		input;
	char		*north_path;
	char		*south_path;
	char		*east_path;
	char		*west_path;
	t_texture	north_texture;
	t_texture	south_texture;
	t_texture	east_texture;
	t_texture	west_texture;
	int			floor_color;
	int			ceiling_color;
	int			window_width;
	int			window_height;
	t_parse		parse_state;
	char		player_dir;
	int			player_start_x;
	int			player_start_y;
	double		player_x;
	double		player_y;
	double		player_angle;
	int			show_minimap;
}	t_game;

int		parse_file(const char *filename, t_game *game);
int		load_map(const char *filename, t_game *game);
void	parse_config_data(t_game *game, const char *filename);
void	print_config_data(t_game *game);
int		init_mlx(t_game *game);
int		load_texture(t_game *game, t_texture *texture, char *path);
int		load_textures(t_game *game);
int		is_wall(t_game *game, int x, int y);
int		render_frame(t_game *game);
void	draw_simple_scene(t_game *game);
void	draw_minimap(t_game *game);
void	put_pixel(t_game *game, int x, int y, int color);
int		get_texture_pixel(t_texture *texture, int x, int y);
int		create_color(int r, int g, int b);
int		handle_keypress(int keycode, t_game *game);
int		handle_keyrelease(int keycode, t_game *game);
int		handle_close(t_game *game);
void	process_input(t_game *game);
int		handle_mouse(int x, int y, t_game *game);
void	move_player(t_game *game, double dx, double dy);
void	free_map(char **map, int height);
void	init_game(t_game *game);
void	init_parse_state(t_parse *parse);
int		validate_parse_complete(t_parse *parse);
int		validate_file_extension(const char *filename);
int		validate_file_access(const char *filename);
int		validate_texture_file(const char *path);
int		validate_all_textures(t_game *game);
void	print_map_debug(t_game *game);
int		validate_map(t_game *game);

#endif