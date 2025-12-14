/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_validate.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 08:30:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 12:23:02 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == 'N'
		|| c == 'S' || c == 'E' || c == 'W' || c == ' ');
}

int	is_map_line(char *line)
{
	while (*line == ' ' || *line == '\t')
		line++;
	return (*line == '1' || *line == '0' || *line == ' ');
}

static void	count_player(t_game *game, char c, int x, int y)
{
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		game->parse_state.player_count++;
		game->player_dir = c;
		game->player_start_x = x;
		game->player_start_y = y;
	}
}

static int	validate_map_chars(t_game *game)
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
