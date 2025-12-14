/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:21:51 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_wall(t_game *game, int x, int y)
{
	if (!game->map || x < 0 || x >= game->map_width
		|| y < 0 || y >= game->map_height)
		return (1);
	return (game->map[y][x] == '1');
}

void	put_pixel(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < game->window_width
		&& y >= 0 && y < game->window_height)
	{
		dst = game->image_data + (y * game->line_length
				+ x * (game->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

int	create_color(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

void	perform_dda(t_game *game, t_ray *ray)
{
	ray->hit = 0;
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (is_wall(game, ray->map_x, ray->map_y))
			ray->hit = 1;
	}
}

int	render_frame(t_game *game)
{
	process_input(game);
	draw_simple_scene(game);
	mlx_put_image_to_window(game->mlx, game->window, game->image, 0, 0);
	return (0);
}
