/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 08:30:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 12:33:37 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*skip_whitespace(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static void	trim_end(char *str)
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

static int	set_texture_target(char *line, t_game *game,
	t_parse *parse, t_tex_parse *tex)
{
	line = skip_whitespace(line);
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
	else if (line[0] == 'W' && line[1] == 'E')
	{
		tex->target = &game->west_path;
		tex->has_flag = &parse->has_west;
	}
	else if (line[0] == 'E' && line[1] == 'A')
	{
		tex->target = &game->east_path;
		tex->has_flag = &parse->has_east;
	}
	else
		return (-1);
	return (2);
}

static void	parse_texture_id_helper(t_tex_parse tex, char *path)
{
	trim_end(path);
	*tex.target = path;
	*tex.has_flag = 1;
}

int	parse_texture_id(char *line, t_game *game, t_parse *parse)
{
	char		*path;
	t_tex_parse	tex;
	int			skip;

	skip = set_texture_target(line, game, parse, &tex);
	if (skip == -1)
		return (-1);
	if (*tex.has_flag)
	{
		write(2, "Error\nDuplicate texture definition\n", 36);
		return (0);
	}
	line += skip;
	line = skip_whitespace(line);
	if (*line == '\0' || *line == '\n')
	{
		write(2, "Error\nTexture path missing\n", 28);
		return (0);
	}
	path = ft_strdup(line);
	if (!path)
		return (0);
	parse_texture_id_helper(tex, path);
	return (1);
}
