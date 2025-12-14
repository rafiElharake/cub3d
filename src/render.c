#include "cub3d.h"

int is_wall(t_game *game, int x, int y)
{
	if (!game->map || x < 0 || x >= game->map_width || y < 0 || y >= game->map_height)
		return 1;
	return game->map[y][x] == '1';
}

void draw_minimap(t_game *game)
{
	int my = 0;
	while (my < game->map_height)
	{
		int mx = 0;
		while (mx < game->map_width)
		{
			int color = (game->map[my][mx] == '1') ? create_color(80, 80, 80) : create_color(200, 200, 200);
			int py = 0;
			while (py < 12)
			{
				int px = 0;
				while (px < 12)
				{
					put_pixel(game, 20 + mx * 12 + px, 20 + my * 12 + py, color);
					px++;
				}
				py++;
			}
			mx++;
		}
		my++;
	}
	int player_map_x = 20 + (int)(game->player_x / CELL_SIZE * 12);
	int player_map_y = 20 + (int)(game->player_y / CELL_SIZE * 12);
	int py = -2;
	while (py <= 2)
	{
		int px = -2;
		while (px <= 2)
		{
			if (px*px + py*py <= 4)
				put_pixel(game, player_map_x + px, player_map_y + py, create_color(255, 0, 0));
			px++;
		}
		py++;
	}
}

void draw_simple_scene(t_game *game)
{
	int x, y;
	double fov = 60.0 * (M_PI / 180.0);
	double half_fov = fov / 2.0;
	double ray_angle, ray_dx, ray_dy;
	int wall_height;
	
	x = 0;
	while (x < game->window_width)
	{
		ray_angle = game->player_angle - half_fov + fov * x / (double)game->window_width;
		ray_dx = cos(ray_angle);
		ray_dy = sin(ray_angle);
		
		int map_x = (int)(game->player_x / CELL_SIZE);
		int map_y = (int)(game->player_y / CELL_SIZE);
		
		double delta_dist_x = fabs(1.0 / ray_dx);
		double delta_dist_y = fabs(1.0 / ray_dy);
		
		double side_dist_x, side_dist_y;
		int step_x, step_y;
		int hit = 0;
		int side;
		
		if (ray_dx < 0)
		{
			step_x = -1;
			side_dist_x = ((game->player_x / CELL_SIZE) - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - (game->player_x / CELL_SIZE)) * delta_dist_x;
		}
		if (ray_dy < 0)
		{
			step_y = -1;
			side_dist_y = ((game->player_y / CELL_SIZE) - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - (game->player_y / CELL_SIZE)) * delta_dist_y;
		}
		
		while (hit == 0)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			if (is_wall(game, map_x, map_y))
				hit = 1;
		}
		
		double perp_wall_dist;
		if (side == 0)
			perp_wall_dist = (map_x - (game->player_x / CELL_SIZE) + (1 - step_x) / 2) / ray_dx;
		else
			perp_wall_dist = (map_y - (game->player_y / CELL_SIZE) + (1 - step_y) / 2) / ray_dy;
		
		if (perp_wall_dist > 0.0)
			wall_height = (int)(game->window_height / perp_wall_dist);
		else
			wall_height = game->window_height;
		
		int wall_start = (game->window_height - wall_height) / 2;
		int wall_end = wall_start + wall_height;
		
		t_texture *current_texture;
		if (side == 0)
		{
			if (ray_dx > 0)
				current_texture = &game->east_texture;
			else
				current_texture = &game->west_texture;
		}
		else
		{
			if (ray_dy > 0)
				current_texture = &game->south_texture;
			else
				current_texture = &game->north_texture;
		}
		
		double wall_x;
		if (side == 0)
			wall_x = game->player_y / CELL_SIZE + perp_wall_dist * ray_dy;
		else
			wall_x = game->player_x / CELL_SIZE + perp_wall_dist * ray_dx;
		wall_x -= floor(wall_x);
		
		int tex_x = (int)(wall_x * (double)current_texture->width);
		if (side == 0 && ray_dx > 0)
			tex_x = current_texture->width - tex_x - 1;
		if (side == 1 && ray_dy < 0)
			tex_x = current_texture->width - tex_x - 1;
		
		y = 0;
		while (y < game->window_height)
		{
			if (y < wall_start)
				put_pixel(game, x, y, game->ceiling_color);
			else if (y < wall_end)
			{
				int tex_y = ((y - wall_start) * current_texture->height) / wall_height;
				if (tex_y >= current_texture->height)
					tex_y = current_texture->height - 1;
				
				int color = get_texture_pixel(current_texture, tex_x, tex_y);
				
				if (side == 1)
				{
					int r = ((color >> 16) & 0xFF) / 2;
					int g = ((color >> 8) & 0xFF) / 2;
					int b = (color & 0xFF) / 2;
					color = create_color(r, g, b);
				}
				
				put_pixel(game, x, y, color);
			}
			else
				put_pixel(game, x, y, game->floor_color);
			y++;
		}
		x++;
	}
	if (game->show_minimap)
		draw_minimap(game);
}

void put_pixel(t_game *game, int x, int y, int color)
{
	char *dst;
	if (x >= 0 && x < game->window_width && y >= 0 && y < game->window_height)
	{
		dst = game->image_data + (y * game->line_length + x * (game->bits_per_pixel / 8));
		*(unsigned int*)dst = color;
	}
}

int create_color(int r, int g, int b)
{
	return (r << 16) | (g << 8) | b;
}

int render_frame(t_game *game)
{
	process_input(game);
	draw_simple_scene(game);
	mlx_put_image_to_window(game->mlx, game->window, game->image, 0, 0);
	return 0;
}