/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 07:30:12 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 12:35:12 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_player_angle(t_game *game, char direction)
{
	if (direction == 'N')
		game->player_angle = -M_PI / 2;
	else if (direction == 'S')
		game->player_angle = M_PI / 2;
	else if (direction == 'E')
		game->player_angle = 0.0;
	else if (direction == 'W')
		game->player_angle = M_PI;
}

static int	find_player_position(t_game *game, int x, int y)
{
	char	c;

	c = game->map[y][x];
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		game->player_x = (x + 0.5) * CELL_SIZE;
		game->player_y = (y + 0.5) * CELL_SIZE;
		set_player_angle(game, c);
		return (1);
	}
	return (0);
}

void	init_player(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			if (find_player_position(game, x, y))
				return ;
			x++;
		}
		y++;
	}
	game->player_x = 1.5 * CELL_SIZE;
	game->player_y = 1.5 * CELL_SIZE;
	game->player_angle = 0.0;
}

static void	init_game_values(t_game *game)
{
	game->mlx = NULL;
	game->window = NULL;
	game->image = NULL;
	game->image_data = NULL;
	game->window_width = WINDOW_WIDTH;
	game->window_height = WINDOW_HEIGHT;
	game->input.w = 0;
	game->input.a = 0;
	game->input.s = 0;
	game->input.d = 0;
	game->input.left = 0;
	game->input.right = 0;
	game->show_minimap = 1;
}

void	init_game(t_game *game)
{
	init_game_values(game);
	init_player(game);
}
