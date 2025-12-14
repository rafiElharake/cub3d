/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 05:32:50 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move_player(t_game *game, double dx, double dy)
{
	double	new_x;
	double	new_y;
	int		map_x;
	int		map_y;

	new_x = game->player_x + dx;
	new_y = game->player_y + dy;
	map_x = (int)(new_x / CELL_SIZE);
	map_y = (int)(new_y / CELL_SIZE);
	if (!is_wall(game, map_x, map_y)
		&& !is_wall(game, (int)(game->player_x / CELL_SIZE), map_y)
		&& !is_wall(game, map_x, (int)(game->player_y / CELL_SIZE)))
	{
		game->player_x = new_x;
		game->player_y = new_y;
	}
}

static void	process_wasd(t_game *game)
{
	double	move_speed;
	double	angle;

	move_speed = MOVE_SPEED;
	if (game->input.w)
		move_player(game, move_speed * cos(game->player_angle),
			move_speed * sin(game->player_angle));
	if (game->input.s)
		move_player(game, -move_speed * cos(game->player_angle),
			-move_speed * sin(game->player_angle));
	if (game->input.a)
	{
		angle = game->player_angle - M_PI / 2;
		move_player(game, move_speed * cos(angle),
			move_speed * sin(angle));
	}
	if (game->input.d)
	{
		angle = game->player_angle + M_PI / 2;
		move_player(game, move_speed * cos(angle),
			move_speed * sin(angle));
	}
}

void	process_input(t_game *game)
{
	double	rotation_speed;

	rotation_speed = ROTATION_SPEED;
	process_wasd(game);
	if (game->input.left)
		game->player_angle -= rotation_speed;
	if (game->input.right)
		game->player_angle += rotation_speed;
}
