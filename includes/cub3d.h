/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 12:46:44 by afahs             #+#    #+#             */
/*   Updated: 2026/01/14 16:36:06 by afahs            ###   ########.fr       */
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
# include "../minilibx-linux/mlx.h"
# include "../gnl/get_next_line.h"
# include <stddef.h>
# include <stdint.h>

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

typedef struct s_read_data
{
	int		map_started;
	int		*maxw;
	t_game	*game;
}	t_read_data;

typedef struct s_ray
{
	double	player_pos_x;
	double	player_pos_y;
	double	ray_dx;
	double	ray_dy;
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
}	t_ray;

typedef struct s_wall
{
	int			wall_height;
	int			wall_start;
	int			wall_end;
	t_texture	*texture;
	int			tex_x;
	int			side;
}	t_wall;

typedef struct s_tex_parse
{
	char	**target;
	int		*has_flag;
}	t_tex_parse;

typedef struct s_color_parse
{
	int		*target_color;
	int		*has_flag;
}	t_color_parse;

int			parse_file(const char *filename, t_game *game);
int			load_map(const char *filename, t_game *game);
int			check_extension(const char *filename);
void		parse_config_data(t_game *game, const char *filename);
char		*extract_path(char *line, int start);
int			parse_rgb_component(char *line, int *i, int *value);
int			parse_rgb(char *line, int start);
int			is_config_line(char c);
int			should_skip_line(char *line, int map_started);
int			get_line_length(char *line);
char		**resize_lines(char **lines, int old_cap, int new_cap, int height);
int			is_valid_char(char c);
int			validate_char(char c, t_game *game, char **lines, int i);
char		*create_padded_line(char *src, int width);
int			process_map_line(t_game *game, char **lines, int i);
int			read_map_lines(int fd, char ***lines, int *maxw, t_game *game);
void		parse_texture_line(t_game *game, char *line);
void		parse_color_line(t_game *game, char *line);
int			process_config_line(t_game *game, char *line);
void		free_lines(char **lines, int height);
int			init_mlx(t_game *game);
int			load_texture(t_game *game, t_texture *texture, char *path);
int			load_textures(t_game *game);
int			is_wall(t_game *game, int x, int y);
int			render_frame(t_game *game);
void		draw_simple_scene(t_game *game);
void		draw_minimap(t_game *game);
void		put_pixel(t_game *game, int x, int y, int color);
int			get_texture_pixel(t_texture *texture, int x, int y);
int			create_color(int r, int g, int b);
int			handle_keypress(int keycode, t_game *game);
int			handle_keyrelease(int keycode, t_game *game);
int			handle_close(t_game *game);
void		process_input(t_game *game);
int			handle_mouse(int x, int y, t_game *game);
void		move_player(t_game *game, double dx, double dy);
void		free_map(char **map, int height);
void		init_game(t_game *game);
void		init_player(t_game *game);
void		init_parse_state(t_parse *parse);
int			validate_parse_complete(t_parse *parse);
int			validate_file_extension(const char *filename);
int			validate_file_access(const char *filename);
int			validate_texture_file(const char *path);
int			validate_all_textures(t_game *game);
int			validate_map(t_game *game);
void		init_ray_dir(t_ray *ray, double angle);
void		init_step_x(t_ray *ray);
void		init_step_y(t_ray *ray);
void		perform_dda(t_game *game, t_ray *ray);
double		calculate_wall_dist(t_ray *ray);
int			calculate_wall_height(double perp_wall_dist, int window_height);
t_texture	*select_texture(t_game *game, t_ray *ray);
int			calculate_tex_x(t_ray *ray, t_texture *tex, double wall_x);
double		calculate_wall_x(t_ray *ray, double perp_wall_dist);
int			apply_shading(int color, int side);
int			parse_texture_id(char *line, t_game *game, t_parse *parse);
int			parse_color_id(char *line, t_game *game, t_parse *parse);
int			is_map_line(char *line);
int			validate_map(t_game *game);
int			validate_map_closed(t_game *game);
int			process_config_lines(int fd, t_game *game, int *result);
int			is_empty_line_parse(char *line);
int			flood_fill_check(t_game *game, int x, int y, char **visited);
int			handle_config_line_parse(char *line, t_game *game,
				int *res, int fd);
void		*ft_calloc(size_t nmemb, size_t size);
int			ft_strcmp(const char *s1, const char *s2);
void		free_texture_paths(t_game *game);
void		cleanup_all(t_game *game);

#endif