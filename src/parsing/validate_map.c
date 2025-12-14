/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:28:20 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	validate_map_chars(t_game *game)
{
	int		y;
	int		x;
	char	c;

	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			c = game->map[y][x];
			if (!is_valid_map_char(c))
			{
				write(2, "Error\nInvalid character in map: '", 34);
				write(2, &c, 1);
				write(2, "'\n", 2);
				return (0);
			}
			count_player(game, c, x, y);
			x++;
		}
		y++;
	}
	return (1);
}

int	flood_fill_check(t_game *game, int x, int y, char **visited)
{
	if (x < 0 || x >= game->map_width || y < 0 || y >= game->map_height)
		return (0);
	if (game->map[y][x] == ' ')
		return (0);
	if (game->map[y][x] == '1' || visited[y][x])
		return (1);
	visited[y][x] = 1;
	if (!flood_fill_check(game, x + 1, y, visited))
		return (0);
	if (!flood_fill_check(game, x - 1, y, visited))
		return (0);
	if (!flood_fill_check(game, x, y + 1, visited))
		return (0);
	if (!flood_fill_check(game, x, y - 1, visited))
		return (0);
	return (1);
}

static int	alloc_visited(t_game *game, char ***visited)
{
	int	i;

	*visited = malloc(sizeof(char *) * game->map_height);
	if (!*visited)
		return (0);
	i = 0;
	while (i < game->map_height)
	{
		(*visited)[i] = calloc(game->map_width, sizeof(char));
		if (!(*visited)[i])
		{
			while (--i >= 0)
				free((*visited)[i]);
			free(*visited);
			return (0);
		}
		i++;
	}
	return (1);
}

int	validate_map_closed(t_game *game)
{
	char	**visited;
	int		i;
	int		result;

	if (!alloc_visited(game, &visited))
		return (0);
	result = flood_fill_check(game, game->player_start_x,
			game->player_start_y, visited);
	i = 0;
	while (i < game->map_height)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
	if (!result)
	{
		write(2, "Error\nMap is not closed by walls\n", 34);
		return (0);
	}
	return (1);
}
