/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 08:30:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 12:13:23 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_empty_line_parse(char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t'
			&& *line != '\n' && *line != '\r')
			return (0);
		line++;
	}
	return (1);
}

int	handle_config_line_parse(char *line, t_game *game, int *result, int fd)
{
	*result = parse_texture_id(line, game, &game->parse_state);
	if (*result == 0)
	{
		free(line);
		close(fd);
		return (0);
	}
	if (*result == -1)
	{
		*result = parse_color_id(line, game, &game->parse_state);
		if (*result == 0)
		{
			free(line);
			close(fd);
			return (0);
		}
		if (*result == -1)
		{
			write(2, "Error\nInvalid configuration line\n", 34);
			free(line);
			close(fd);
			return (0);
		}
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
