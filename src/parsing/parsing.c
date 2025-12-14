/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 02:28:10 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:44:16 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_parse_state(t_parse *parse)
{
	parse->has_north = 0;
	parse->has_south = 0;
	parse->has_east = 0;
	parse->has_west = 0;
	parse->has_floor = 0;
	parse->has_ceiling = 0;
	parse->player_count = 0;
	parse->map_started = 0;
	parse->map_line_count = 0;
}

int	validate_parse_complete(t_parse *parse)
{
	if (!parse->has_north || !parse->has_south
		|| !parse->has_east || !parse->has_west)
	{
		write(2, "Error\nMissing texture definition\n", 34);
		return (0);
	}
	if (!parse->has_floor || !parse->has_ceiling)
	{
		write(2, "Error\nMissing floor or ceiling color\n", 38);
		return (0);
	}
	if (parse->player_count != 1)
	{
		write(2, "Error\nMap must have exactly one player\n", 40);
		return (0);
	}
	return (1);
}

int	validate_map(t_game *game)
{
	if (!validate_map_chars(game))
		return (0);
	if (!validate_parse_complete(&game->parse_state))
		return (0);
	if (!validate_map_closed(game))
		return (0);
	return (1);
}

int	parse_file(const char *filename, t_game *game)
{
	int		fd;
	char	*line;

	init_parse_state(&game->parse_state);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error\nFailed to open file");
		return (0);
	}
	line = get_next_line(fd);
	while (line)
	{
		if (!process_parse_line(game, line, fd))
		{
			close(fd);
			return (0);
		}
		line = get_next_line(fd);
	}
	close(fd);
	if (!load_map(filename, game) || !validate_map(game))
		return (0);
	return (1);
}
