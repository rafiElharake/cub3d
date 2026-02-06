/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 06:50:05 by afahs             #+#    #+#             */
/*   Updated: 2026/02/06 21:45:11 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	validate_and_parse(char *filename, t_game *game)
{
	if (!validate_file_extension(filename))
		return (0);
	if (!validate_file_access(filename))
		return (0);
	if (!parse_file(filename, game))
		return (0);
	if (!validate_all_textures(game))
		return (0);
	return (1);
}

static void	setup_hooks(t_game *game)
{
//	mlx_mouse_hide(game->mlx, game->window);
	mlx_hook(game->window, 2, 1L << 0, handle_keypress, game);
	mlx_hook(game->window, 3, 1L << 1, handle_keyrelease, game);
	mlx_hook(game->window, 17, 1L << 17, handle_close, game);
	mlx_hook(game->window, 6, 1L << 6, handle_mouse, game);
	mlx_loop_hook(game->mlx, render_frame, game);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		write(2, "Usage: ./cub3D <mapfile.cub>\n", 29);
		return (1);
	}
	memset(&game, 0, sizeof(t_game));
	if (!validate_and_parse(argv[1], &game))
	{
		free_map(game.map, game.map_height);
		return (1);
	}
	init_game(&game);
	if (!init_mlx(&game))
		return (1);
	setup_hooks(&game);
	mlx_loop(game.mlx);
	free_map(game.map, game.map_height);
	return (0);
}
