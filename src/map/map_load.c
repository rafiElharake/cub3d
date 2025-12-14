/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 07:31:26 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	process_all_lines(t_game *game, char **lines)
{
	int	i;

	i = 0;
	while (i < game->map_height)
	{
		if (!process_map_line(game, lines, i))
			return (0);
		i++;
	}
	return (1);
}

static int	open_and_read(const char *filename, t_game *game,
				char ***lines, int *maxw)
{
	int	fd;

	if (!check_extension(filename))
	{
		write(2, "Error: scene file must have .cub extension\n", 44);
		return (0);
	}
	fd = open(filename, 0);
	if (fd < 0)
	{
		perror("Error opening map file");
		return (0);
	}
	if (!read_map_lines(fd, lines, maxw, game))
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

static int	validate_and_allocate(t_game *game, char **lines, int maxw)
{
	if (game->map_height == 0)
	{
		write(2, "Error: empty map file\n", 22);
		free(lines);
		return (0);
	}
	game->map_width = maxw;
	game->map = malloc(sizeof(char *) * game->map_height);
	if (!game->map)
	{
		free_lines(lines, game->map_height);
		return (0);
	}
	return (1);
}

int	load_map(const char *filename, t_game *game)
{
	char	**lines;
	int		maxw;

	game->map_height = 0;
	game->map_width = 0;
	maxw = 0;
	if (!open_and_read(filename, game, &lines, &maxw))
		return (0);
	if (!validate_and_allocate(game, lines, maxw))
		return (0);
	if (!process_all_lines(game, lines))
		return (0);
	free(lines);
	return (1);
}
