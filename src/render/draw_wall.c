/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:01:50 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_texture	*select_texture(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->ray_dx > 0)
			return (&game->east_texture);
		else
			return (&game->west_texture);
	}
	else
	{
		if (ray->ray_dy > 0)
			return (&game->south_texture);
		else
			return (&game->north_texture);
	}
}

int	calculate_tex_x(t_ray *ray, t_texture *tex, double wall_x)
{
	int	tex_x;

	tex_x = (int)(wall_x * (double)tex->width);
	if (ray->side == 0 && ray->ray_dx > 0)
		tex_x = tex->width - tex_x - 1;
	if (ray->side == 1 && ray->ray_dy < 0)
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

double	calculate_wall_x(t_ray *ray, double perp_wall_dist)
{
	double	wall_x;

	if (ray->side == 0)
		wall_x = ray->player_pos_y / CELL_SIZE + perp_wall_dist * ray->ray_dy;
	else
		wall_x = ray->player_pos_x / CELL_SIZE + perp_wall_dist * ray->ray_dx;
	wall_x -= floor(wall_x);
	return (wall_x);
}

int	apply_shading(int color, int side)
{
	int	r;
	int	g;
	int	b;

	if (side == 1)
	{
		r = ((color >> 16) & 0xFF) / 2;
		g = ((color >> 8) & 0xFF) / 2;
		b = (color & 0xFF) / 2;
		return (create_color(r, g, b));
	}
	return (color);
}
