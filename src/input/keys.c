/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 05:31:01 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 05:31:04 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	handle_keypress(int keycode, t_game *game)
{
	if (keycode == KEY_ESC)
	{
		mlx_destroy_image(game->mlx, game->image);
		mlx_destroy_window(game->mlx, game->window);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		exit(0);
	}
	if (keycode == 'w' || keycode == 'W')
		game->input.w = 1;
	if (keycode == 's' || keycode == 'S')
		game->input.s = 1;
	if (keycode == 'a' || keycode == 'A')
		game->input.a = 1;
	if (keycode == 'd' || keycode == 'D')
		game->input.d = 1;
	if (keycode == KEY_LEFT)
		game->input.left = 1;
	if (keycode == KEY_RIGHT)
		game->input.right = 1;
	if (keycode == 'm' || keycode == 'M')
		game->show_minimap = !game->show_minimap;
	return (0);
}

int	handle_keyrelease(int keycode, t_game *game)
{
	if (keycode == 'w' || keycode == 'W')
		game->input.w = 0;
	if (keycode == 's' || keycode == 'S')
		game->input.s = 0;
	if (keycode == 'a' || keycode == 'A')
		game->input.a = 0;
	if (keycode == 'd' || keycode == 'D')
		game->input.d = 0;
	if (keycode == KEY_LEFT)
		game->input.left = 0;
	if (keycode == KEY_RIGHT)
		game->input.right = 0;
	return (0);
}

int	handle_mouse(int x, int y, t_game *game)
{
	static int	last_x = -1;
	int			win_center_x;
	int			dx;

	(void)y;
	win_center_x = game->window_width / 2;
	if (last_x == -1)
		last_x = win_center_x;
	dx = x - win_center_x;
	if (dx != 0)
	{
		game->player_angle += dx * 0.002;
		mlx_mouse_move(game->mlx, game->window, win_center_x,
			game->window_height / 2);
	}
	last_x = win_center_x;
	return (0);
}
