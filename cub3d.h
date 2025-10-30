/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_name <your_email@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by your_name        #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by your_name       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <string.h>
# include <errno.h>
# include <sys/time.h>
# include <stddef.h>
# include "minilibx-linux/mlx.h"
# include "gnl/get_next_line.h"

/* Window settings */
# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080
# define FOV 60.0
# define PI 3.14159265359
# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

/* Key codes (may vary by system) */
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_UP 65362
# define KEY_DOWN 65364

/* Movement settings */
# define MOVE_SPEED 0.2
# define ROTATION_SPEED 0.001
# define MOVE_STEP (0.25 * CELL_SIZE)
# define MOUSE_SENSITIVITY 0.002

/* Map settings */
# define MAP_WIDTH 10
# define MAP_HEIGHT 8
# define CELL_SIZE 4

/* Map characters */
# define WALL '1'
# define EMPTY '0'
# define NORTH 'N'
# define SOUTH 'S'
# define EAST 'E'
# define WEST 'W'

/* Wall directions */
# define WALL_NORTH 0
# define WALL_SOUTH 1
# define WALL_EAST 2
# define WALL_WEST 3

/* Error messages */
# define ERR_MALLOC "Memory allocation failed"
# define ERR_FILE "File error"
# define ERR_FORMAT "Invalid file format"
# define ERR_MAP "Invalid map"
# define ERR_TEXTURE "Texture error"
# define ERR_COLOR "Invalid color"
# define ERR_MLX "MLX initialization failed"

/* Texture structure */
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

/* Ray structure */
typedef struct s_ray
{
	double	ray_angle;
	double	distance;
	int		hit_vertical;
	double	hit_x;
	double	hit_y;
	int		wall_direction;
	int		texture_x;
}	t_ray;

/* Player structure */
typedef struct s_player
{
	double	x;
	double	y;
	double	angle;
	double	move_speed;
	double	rotation_speed;
}	t_player;

/* Input structure for key states */
typedef struct s_input
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_input;

/* Main game structure */
typedef struct s_game
{
	/* MLX variables */
	void		*mlx;
	void		*window;
	void		*image;
	char		*image_data;
	int			bits_per_pixel;
	int			line_length;
	int			endian;

	/* Map data */
	char		**map;
	int			map_width;
	int			map_height;

	/* Player */
	t_player	player;

	/* Input */
	t_input		input;

	/* Textures */
	t_texture	wall_texture;

	/* Colors */
	int			floor_color;
	int			ceiling_color;

	/* Window settings */
	int			window_width;
	int			window_height;

	/* Player position and angle */
	double		player_x;
	double		player_y;
	double		player_angle;
}	t_game;

/* Function prototypes */

/* Parsing */
int		parse_cub_file(const char *filename, t_game *game);
int		parse_line(char *line, t_game *game);
int		parse_texture(char *line, t_game *game);
int		parse_color(char *line, t_game *game);
int		parse_map(int fd, t_game *game);
int		validate_map(t_game *game);
int		find_player(t_game *game);
int		load_map(const char *filename, t_game *game);

/* Graphics initialization */
int		init_mlx(t_game *game);
int		load_texture(t_game *game, t_texture *texture, char *path);
void	init_textures(t_game *game);
int		load_textures(t_game *game);
void	init_player(t_game *game, int x, int y, char direction);

/* Raycasting */
void	cast_rays(t_game *game);
void	cast_single_ray(t_game *game, t_ray *ray, double ray_angle);
double	calculate_distance(double x1, double y1, double x2, double y2);
int		is_wall(t_game *game, int x, int y);

/* Rendering */
int		render_frame(t_game *game);
void	draw_wall(t_game *game, int x, t_ray *ray);
void	draw_floor_ceiling(t_game *game);
void	put_pixel(t_game *game, int x, int y, int color);
int		get_texture_pixel(t_texture *texture, int x, int y);
void	draw_simple_scene(t_game *game);
void	draw_minimap(t_game *game);

/* Input handling */
int		handle_keypress(int keycode, t_game *game);
int		handle_keyrelease(int keycode, t_game *game);
int		handle_close(t_game *game);
void	process_input(t_game *game);
int		handle_mouse(int x, int y, t_game *game);

/* Movement */
void	move_player(t_game *game, double dx, double dy);
int		check_collision(t_game *game, double x, double y);
void	rotate_player(t_game *game, double angle);

/* Utilities */
int		create_color(int r, int g, int b);
double	normalize_angle(double angle);
double	deg_to_rad(double degrees);
double	rad_to_deg(double radians);

/* Error handling */
void	error_exit(const char *msg);
void	free_map(char **map, int height);

/* Memory management */
void	safe_free(void **ptr);

/* Game initialization */
void	init_game(t_game *game);

#endif