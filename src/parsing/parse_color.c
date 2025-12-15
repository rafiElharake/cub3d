/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 08:30:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 12:14:34 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static char	*skip_whitespace(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static int	parse_rgb_values(char *str, int *r, int *g, int *b)
{
	char	*end;

	str = skip_whitespace(str);
	*r = strtol(str, &end, 10);//not allowed
	if (end == str || *r < 0 || *r > 255)
		return (0);
	str = skip_whitespace(end);
	if (*str != ',')
		return (0);
	str++;
	*g = strtol(str, &end, 10);//not allowed
	if (end == str || *g < 0 || *g > 255)
		return (0);
	str = skip_whitespace(end);
	if (*str != ',')
		return (0);
	str++;
	*b = strtol(str, &end, 10);//not allowed
	if (end == str || *b < 0 || *b > 255)
		return (0);
	str = skip_whitespace(end);
	if (*str != '\0' && *str != '\n')
		return (0);
	return (1);
}

static int	set_color_target(char *line, t_game *game,
	t_parse *parse, t_color_parse *col)
{
	line = skip_whitespace(line);
	if (line[0] == 'F')
	{
		col->target_color = &game->floor_color;
		col->has_flag = &parse->has_floor;
		return (1);
	}
	else if (line[0] == 'C')
	{
		col->target_color = &game->ceiling_color;
		col->has_flag = &parse->has_ceiling;
		return (1);
	}
	return (-1);
}

int	parse_color_id(char *line, t_game *game, t_parse *parse)
{
	int				r;
	int				g;
	int				b;
	t_color_parse	col;
	int				skip;

	skip = set_color_target(line, game, parse, &col);
	if (skip == -1)
		return (-1);
	if (*col.has_flag)
	{
		write(2, "Error\nDuplicate color definition\n", 34);
		return (0);
	}
	line += skip;
	if (!parse_rgb_values(line, &r, &g, &b))
	{
		write(2, "Error\nInvalid RGB color format\n", 32);
		return (0);
	}
	*col.target_color = create_color(r, g, b);
	*col.has_flag = 1;
	return (1);
}
