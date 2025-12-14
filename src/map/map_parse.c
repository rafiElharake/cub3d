/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 07:55:06 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_paths(t_game *game)
{
	game->north_path = NULL;
	game->south_path = NULL;
	game->east_path = NULL;
	game->west_path = NULL;
}

int	process_config_line(t_game *game, char *line)
{
	if (line[0] == '\n' || line[0] == '\0')
		return (0);
	if (line[0] == '0' || line[0] == '1' || line[0] == ' ')
		return (1);
	parse_texture_line(game, line);
	parse_color_line(game, line);
	return (0);
}

void	parse_config_data(t_game *game, const char *filename)
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return ;
	init_paths(game);
	line = get_next_line(fd);
	while (line)
	{
		if (process_config_line(game, line))
		{
			free(line);
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

void	parse_texture_line(t_game *game, char *line)
{
	if (line[0] == 'N' && line[1] == 'O')
		game->north_path = extract_path(line, 2);
	else if (line[0] == 'S' && line[1] == 'O')
		game->south_path = extract_path(line, 2);
	else if (line[0] == 'W' && line[1] == 'E')
		game->west_path = extract_path(line, 2);
	else if (line[0] == 'E' && line[1] == 'A')
		game->east_path = extract_path(line, 2);
}

void	parse_color_line(t_game *game, char *line)
{
	if (line[0] == 'F')
		game->floor_color = parse_rgb(line, 1);
	else if (line[0] == 'C')
		game->ceiling_color = parse_rgb(line, 1);
}
