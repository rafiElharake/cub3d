/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_config.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 07:55:40 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	copy_path_chars(char *path, char *line, int start, int len)
{
	int	j;

	j = 0;
	while (j < len)
	{
		path[j] = line[start + j];
		j++;
	}
	path[len] = '\0';
}

char	*extract_path(char *line, int start)
{
	int		i;
	int		len;
	char	*path;

	i = start;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	len = 0;
	while (line[i + len] && line[i + len] != '\n')
		len++;
	if (len <= 0)
		return (NULL);
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	copy_path_chars(path, line, i, len);
	while (len > 0 && (path[len - 1] == ' ' || path[len - 1] == '\t'))
		path[--len] = '\0';
	return (path);
}

int	parse_rgb_component(char *line, int *i, int *value)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
	*value = 0;
	while (line[*i] >= '0' && line[*i] <= '9')
	{
		*value = *value * 10 + (line[*i] - '0');
		(*i)++;
	}
	if (*value > 255)
		return (0);
	return (1);
}

int	parse_rgb(char *line, int start)
{
	int	r;
	int	g;
	int	b;
	int	i;

	i = start;
	if (!parse_rgb_component(line, &i, &r))
		return (-1);
	if (line[i++] != ',')
		return (-1);
	if (!parse_rgb_component(line, &i, &g))
		return (-1);
	if (line[i++] != ',')
		return (-1);
	if (!parse_rgb_component(line, &i, &b))
		return (-1);
	return ((r << 16) | (g << 8) | b);
}

void	free_lines(char **lines, int height)
{
	int	i;

	i = 0;
	while (i < height)
		free(lines[i++]);
	free(lines);
}
