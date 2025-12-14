/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:02:10 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_map_cell(t_game *game, int mx, int my)
{
	int	color;
	int	py;
	int	px;

	if (game->map[my][mx] == '1')
		color = create_color(80, 80, 80);
	else
		color = create_color(200, 200, 200);
	py = 0;
	while (py < 12)
	{
		px = 0;
		while (px < 12)
		{
			put_pixel(game, 20 + mx * 12 + px, 20 + my * 12 + py, color);
			px++;
		}
		py++;
	}
}

static void	draw_player_dot(t_game *game, int x, int y)
{
	int	py;
	int	px;

	py = -2;
	while (py <= 2)
	{
		px = -2;
		while (px <= 2)
		{
			if (px * px + py * py <= 4)
				put_pixel(game, x + px, y + py, create_color(255, 0, 0));
			px++;
		}
		py++;
	}
}

void	draw_minimap(t_game *game)
{
	int	my;
	int	mx;
	int	player_map_x;
	int	player_map_y;

	if (!game->show_minimap)
		return ;
	my = 0;
	while (my < game->map_height)
	{
		mx = 0;
		while (mx < game->map_width)
		{
			draw_map_cell(game, mx, my);
			mx++;
		}
		my++;
	}
	player_map_x = 20 + (int)(game->player_x / CELL_SIZE * 12);
	player_map_y = 20 + (int)(game->player_y / CELL_SIZE * 12);
	draw_player_dot(game, player_map_x, player_map_y);
}
