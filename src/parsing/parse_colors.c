/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:37:45 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_rgb_values(char *str, int *r, int *g, int *b)
{
	char	*end;

	str = skip_whitespace(str);
	*r = strtol(str, &end, 10);
	if (end == str || *r < 0 || *r > 255)
		return (0);
	str = skip_whitespace(end);
	if (*str != ',')
		return (0);
	str++;
	*g = strtol(str, &end, 10);
	if (end == str || *g < 0 || *g > 255)
		return (0);
	str = skip_whitespace(end);
	if (*str != ',')
		return (0);
	str++;
	*b = strtol(str, &end, 10);
	if (end == str || *b < 0 || *b > 255)
		return (0);
	str = skip_whitespace(end);
	if (*str != '\0' && *str != '\n')
		return (0);
	return (1);
}

int	parse_color_id(char *line, t_game *game, t_parse *parse)
{
	int	r;
	int	g;
	int	b;
	int	*target_color;
	int	*has_flag;

	line = skip_whitespace(line);
	if (line[0] == 'F')
	{
		target_color = &game->floor_color;
		has_flag = &parse->has_floor;
		line++;
	}
	else if (line[0] == 'C')
	{
		target_color = &game->ceiling_color;
		has_flag = &parse->has_ceiling;
		line++;
	}
	else
		return (-1);
	if (*has_flag)
	{
		write(2, "Error\nDuplicate color definition\n", 34);
		return (0);
	}
	if (!parse_rgb_values(line, &r, &g, &b))
	{
		write(2, "Error\nInvalid RGB color format\n", 32);
		return (0);
	}
	*target_color = create_color(r, g, b);
	*has_flag = 1;
	return (1);
}
