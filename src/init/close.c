/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:20:28 by afahs             #+#    #+#             */
/*   Updated: 2026/02/06 21:35:43 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	cleanup_all(t_game *game)
{
	if (game->north_texture.img)
		mlx_destroy_image(game->mlx, game->north_texture.img);
	if (game->south_texture.img)
		mlx_destroy_image(game->mlx, game->south_texture.img);
	if (game->east_texture.img)
		mlx_destroy_image(game->mlx, game->east_texture.img);
	if (game->west_texture.img)
		mlx_destroy_image(game->mlx, game->west_texture.img);
	if (game->image)
		mlx_destroy_image(game->mlx, game->image);
	if (game->window)
		mlx_destroy_window(game->mlx, game->window);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	if (game->map)
		free_map(game->map, game->map_height);
	free_texture_paths(game);
}

int	handle_close(t_game *game)
{
	cleanup_all(game);
	exit(0);
	return (0);
}
