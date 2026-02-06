/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 08:30:00 by afahs             #+#    #+#             */
/*   Updated: 2026/02/06 23:03:04 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	set_texture_target(char *line, t_game *game,
	t_parse *parse, t_tex_parse *tex)
{
	char	*start;

	start = line;
	line = skip_whitespace_texture(line);
	if (!check_texture_id(line))
		return (-1);
	set_north_south(line, game, parse, tex);
	set_west_east(line, game, parse, tex);
	return ((line - start) + 2);
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
	line = skip_whitespace_texture(line);
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
