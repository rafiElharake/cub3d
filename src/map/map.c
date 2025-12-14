/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 07:22:38 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_map(char **map, int height)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (i < height)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

static void	print_floor_color(t_game *game)
{
	if (game->floor_color >= 0)
		printf("Floor: %d,%d,%d\n", (game->floor_color >> 16) & 0xFF,
			(game->floor_color >> 8) & 0xFF, game->floor_color & 0xFF);
	else
		printf("Floor: NULL\n");
}

static void	print_ceiling_color(t_game *game)
{
	if (game->ceiling_color >= 0)
		printf("Ceiling: %d,%d,%d\n", (game->ceiling_color >> 16) & 0xFF,
			(game->ceiling_color >> 8) & 0xFF, game->ceiling_color & 0xFF);
	else
		printf("Ceiling: NULL\n");
}

void	print_config_data(t_game *game)
{
	printf("North: %s\n", game->north_path);
	printf("South: %s\n", game->south_path);
	printf("West: %s\n", game->west_path);
	printf("East: %s\n", game->east_path);
	print_floor_color(game);
	print_ceiling_color(game);
}
