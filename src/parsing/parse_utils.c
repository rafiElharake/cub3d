/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 08:30:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 12:32:41 by afahs            ###   ########.fr       */
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

int	validate_map_closed(t_game *game)
{
	char	**visited;
	int		result;

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

static int	handle_line(char *line, t_game *game, int *result, int fd)
{
	if (is_map_line(line))
		game->parse_state.map_started = 1;
	if (! game->parse_state.map_started)
	{
		if (!handle_config_line_parse(line, game, result, fd))
			return (0);
	}
	return (1);
}

int	process_config_lines(int fd, t_game *game, int *result)
{
	char	*line;
	char	*temp;

	line = get_next_line(fd);
	while (line)
	{
		if (is_empty_line_parse(line) && !game->parse_state.map_started)
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if (! handle_line(line, game, result, fd))
		{
			free(line);
			while ((temp = get_next_line(fd)))
				free(temp);
			close(fd);
			free_texture_paths(game);
			return (0);
		}
		if (game->parse_state.map_started)
			break ;
		free(line);
		line = get_next_line(fd);
	}
	return (free(line), close(fd), 1);
}
