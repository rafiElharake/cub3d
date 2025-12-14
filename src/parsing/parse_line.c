/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:43:43 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_empty_line(char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t' && *line != '\n' && *line != '\r')
			return (0);
		line++;
	}
	return (1);
}

static int	handle_config_line(t_game *game, char *line)
{
	int	result;

	result = parse_texture_id(line, game, &game->parse_state);
	if (result == 0)
		return (0);
	if (result == -1)
	{
		result = parse_color_id(line, game, &game->parse_state);
		if (result == 0)
			return (0);
		if (result == -1)
		{
			write(2, "Error\nInvalid configuration line\n", 34);
			return (0);
		}
	}
	return (1);
}

int	process_parse_line(t_game *game, char *line, int fd)
{
	(void)fd;
	if (is_empty_line(line) && !game->parse_state.map_started)
	{
		free(line);
		return (1);
	}
	if (is_map_line(line))
	{
		game->parse_state.map_started = 1;
		free(line);
		return (0);
	}
	if (!game->parse_state.map_started)
	{
		if (!handle_config_line(game, line))
		{
			free(line);
			return (0);
		}
	}
	free(line);
	return (1);
}
