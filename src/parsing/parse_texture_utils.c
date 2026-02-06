/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 08:30:00 by afahs             #+#    #+#             */
/*   Updated: 2026/02/06 23:03:14 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*skip_whitespace_texture(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

void	trim_end(char *str)
{
	int	len;

	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t'
			|| str[len - 1] == '\n' || str[len - 1] == '\r'))
	{
		str[len - 1] = '\0';
		len--;
	}
}

int	check_texture_id(char *line)
{
	if ((line[0] == 'N' && line[1] == 'O')
		|| (line[0] == 'S' && line[1] == 'O')
		|| (line[0] == 'W' && line[1] == 'E')
		|| (line[0] == 'E' && line[1] == 'A'))
		return (1);
	return (0);
}

void	set_north_south(char *line, t_game *game,
	t_parse *parse, t_tex_parse *tex)
{
	if (line[0] == 'N' && line[1] == 'O')
	{
		tex->target = &game->north_path;
		tex->has_flag = &parse->has_north;
	}
	else if (line[0] == 'S' && line[1] == 'O')
	{
		tex->target = &game->south_path;
		tex->has_flag = &parse->has_south;
	}
}

void	set_west_east(char *line, t_game *game,
	t_parse *parse, t_tex_parse *tex)
{
	if (line[0] == 'W' && line[1] == 'E')
	{
		tex->target = &game->west_path;
		tex->has_flag = &parse->has_west;
	}
	else if (line[0] == 'E' && line[1] == 'A')
	{
		tex->target = &game->east_path;
		tex->has_flag = &parse->has_east;
	}
}
