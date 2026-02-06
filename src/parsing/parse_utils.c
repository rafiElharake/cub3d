/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 08:30:00 by afahs             #+#    #+#             */
/*   Updated: 2026/02/06 22:27:09 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	**alloc_visited(t_game *game)
{
	char	**visited;
	int		i;

	visited = malloc(sizeof(char *) * game->map_height);
	if (!visited)
		return (NULL);
	i = 0;
	while (i < game->map_height)
	{
		visited[i] = ft_calloc(game->map_width, sizeof(char));
		if (!visited[i])
		{
			while (--i >= 0)
				free(visited[i]);
			free(visited);
			return (NULL);
		}
		i++;
	}
	return (visited);
}

static void	free_visited(char **visited, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
}

static int	check_horizontal_edges(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->map_width)
	{
		if (game->map[0][i] != '1' && game->map[0][i] != ' ')
			return (0);
		if (game->map[game->map_height - 1][i] != '1'
			&& game->map[game->map_height - 1][i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

static int	check_vertical_edges(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->map_height)
	{
		if (game->map[i][0] != '1' && game->map[i][0] != ' ')
			return (0);
		if (game->map[i][game->map_width - 1] != '1'
			&& game->map[i][game->map_width - 1] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	validate_map_closed(t_game *game)
{
	char	**visited;
	int		result;

	if (!check_horizontal_edges(game) || !check_vertical_edges(game))
	{
		write(2, "Error\nMap is not closed by walls\n", 34);
		return (0);
	}
	visited = alloc_visited(game);
	if (!visited)
		return (0);
	result = flood_fill_check(game, game->player_start_x,
			game->player_start_y, visited);
	free_visited(visited, game->map_height);
	if (!result)
	{
		write(2, "Error\nMap is not closed by walls\n", 34);
		return (0);
	}
	return (1);
}
