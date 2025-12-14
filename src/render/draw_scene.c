/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:38:51 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_wall_column(t_game *game, t_wall *wall, int x, int y)
{
	int	tex_y;
	int	color;

	if (y < wall->wall_start)
		put_pixel(game, x, y, game->ceiling_color);
	else if (y < wall->wall_end)
	{
		tex_y = ((y - wall->wall_start) * wall->texture->height)
			/ wall->wall_height;
		if (tex_y >= wall->texture->height)
			tex_y = wall->texture->height - 1;
		color = get_texture_pixel(wall->texture, wall->tex_x, tex_y);
		color = apply_shading(color, wall->side);
		put_pixel(game, x, y, color);
	}
	else
		put_pixel(game, x, y, game->floor_color);
}

static void	draw_column(t_game *game, t_wall *wall, int x)
{
	int	y;

	y = 0;
	while (y < game->window_height)
	{
		draw_wall_column(game, wall, x, y);
		y++;
	}
}

static void	prepare_wall(t_game *game, t_ray *ray, t_wall *wall, double dist)
{
	double	wall_x;

	wall->wall_height = calculate_wall_height(dist, game->window_height);
	wall->wall_start = (game->window_height - wall->wall_height) / 2;
	wall->wall_end = wall->wall_start + wall->wall_height;
	wall->texture = select_texture(game, ray);
	wall_x = calculate_wall_x(ray, dist);
	wall->tex_x = calculate_tex_x(ray, wall->texture, wall_x);
	wall->side = ray->side;
}

static void	cast_ray(t_game *game, t_ray *ray, double angle, int x)
{
	double	perp_wall_dist;
	t_wall	wall;

	init_ray_dir(ray, angle);
	init_step_x(ray);
	init_step_y(ray);
	perform_dda(game, ray);
	perp_wall_dist = calculate_wall_dist(ray);
	prepare_wall(game, ray, &wall, perp_wall_dist);
	draw_column(game, &wall, x);
}

void	draw_simple_scene(t_game *game)
{
	int		x;
	double	fov;
	double	half_fov;
	double	ray_angle;
	t_ray	ray;

	fov = 60.0 * (M_PI / 180.0);
	half_fov = fov / 2.0;
	ray.player_pos_x = game->player_x;
	ray.player_pos_y = game->player_y;
	x = 0;
	while (x < game->window_width)
	{
		ray_angle = game->player_angle - half_fov
			+ fov * x / (double)game->window_width;
		cast_ray(game, &ray, ray_angle, x);
		x++;
	}
	draw_minimap(game);
}
