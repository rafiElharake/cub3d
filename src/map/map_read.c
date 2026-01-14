/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2026/01/14 16:35:59 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	process_map_line(t_game *game, char **lines, int i)
{
	char	*src;
	int		l;
	int		j;

	src = lines[i];
	l = get_line_length(src);
	if (l > 0 && src[l - 1] == '\n')
		l--;
	j = 0;
	while (j < l)
	{
		if (!validate_char(src[j], game, lines, i))
			return (0);
		j++;
	}
	game->map[i] = create_padded_line(src, game->map_width);
	if (!game->map[i])
		return (0);
	free(src);
	lines[i] = NULL;
	return (1);
}

static int	handle_line(char ***lines, int *cap, t_game *game, char *line)
{
	if (game->map_height + 1 > *cap)
	{
		*cap *= 2;
		*lines = resize_lines(*lines, *cap / 2, *cap, game->map_height);
		if (!*lines)
			return (0);
	}
	(*lines)[game->map_height++] = line;
	return (1);
}

static int	process_read_line(char *line, int *map_started, int *maxw)
{
	int	len;

	len = get_line_length(line);
	if (should_skip_line(line, *map_started))
		return (0);
	*map_started = 1;
	if (len > *maxw)
		*maxw = len;
	return (1);
}

static int	read_loop(int fd, char ***lines, int *cap,
				t_read_data *data)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		if (!process_read_line(line, &data->map_started, data->maxw))
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if (!handle_line(lines, cap, data->game, line))
		{
			free(line);
			return (0);
		}
		line = get_next_line(fd);
	}
	return (1);
}

int	read_map_lines(int fd, char ***lines, int *maxw, t_game *game)
{
	int				cap;
	t_read_data		data;

	cap = 16;
	*lines = malloc(sizeof(char *) * cap);
	if (!*lines)
		return (0);
	data.map_started = 0;
	data.maxw = maxw;
	data.game = game;
	return (read_loop(fd, lines, &cap, &data));
}
