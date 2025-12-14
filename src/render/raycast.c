/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:20:50 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_ray_dir(t_ray *ray, double angle)
{
	ray->ray_dx = cos(angle);
	ray->ray_dy = sin(angle);
	ray->map_x = (int)(ray->player_pos_x / CELL_SIZE);
	ray->map_y = (int)(ray->player_pos_y / CELL_SIZE);
	ray->delta_dist_x = fabs(1.0 / ray->ray_dx);
	ray->delta_dist_y = fabs(1.0 / ray->ray_dy);
}

void	init_step_x(t_ray *ray)
{
	if (ray->ray_dx < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = ((ray->player_pos_x / CELL_SIZE) - ray->map_x)
			* ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0
				- (ray->player_pos_x / CELL_SIZE)) * ray->delta_dist_x;
	}
}

void	init_step_y(t_ray *ray)
{
	if (ray->ray_dy < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = ((ray->player_pos_y / CELL_SIZE) - ray->map_y)
			* ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0
				- (ray->player_pos_y / CELL_SIZE)) * ray->delta_dist_y;
	}
}

double	calculate_wall_dist(t_ray *ray)
{
	double	perp_wall_dist;

	if (ray->side == 0)
		perp_wall_dist = (ray->map_x - (ray->player_pos_x / CELL_SIZE)
				+ (1 - ray->step_x) / 2) / ray->ray_dx;
	else
		perp_wall_dist = (ray->map_y - (ray->player_pos_y / CELL_SIZE)
				+ (1 - ray->step_y) / 2) / ray->ray_dy;
	return (perp_wall_dist);
}

int	calculate_wall_height(double perp_wall_dist, int window_height)
{
	int	wall_height;

	if (perp_wall_dist > 0.0)
		wall_height = (int)(window_height / perp_wall_dist);
	else
		wall_height = window_height;
	return (wall_height);
}
