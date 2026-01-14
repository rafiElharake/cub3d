/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:28:03 by afahs             #+#    #+#             */
/*   Updated: 2026/01/14 16:36:28 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

static int	cleanup_on_error(int fd, char *line, t_game *game)
{
	char	*temp;

	free(line);
	temp = get_next_line(fd);
	while (temp)
	{
		free(temp);
		temp = get_next_line(fd);
	}
	close(fd);
	free_texture_paths(game);
	return (0);
}

int	process_config_lines(int fd, t_game *game, int *result)
{
	char	*line;

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
			return (cleanup_on_error(fd, line, game));
		if (game->parse_state.map_started)
			break ;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (1);
}
