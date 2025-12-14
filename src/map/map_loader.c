/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 07:26:08 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_extension(const char *filename)
{
	int	len;

	len = 0;
	while (filename[len])
		len++;
	if (len < 4)
		return (0);
	if (filename[len - 4] != '.' || filename[len - 3] != 'c'
		|| filename[len - 2] != 'u' || filename[len - 1] != 'b')
		return (0);
	return (1);
}

int	is_config_line(char c)
{
	return (c == 'N' || c == 'S' || c == 'W'
		|| c == 'E' || c == 'F' || c == 'C');
}

int	should_skip_line(char *line, int map_started)
{
	if (line[0] == '\n' || line[0] == '\0')
		return (1);
	if (!map_started && is_config_line(line[0]))
		return (1);
	return (0);
}

int	get_line_length(char *line)
{
	int	l;

	l = 0;
	while (line[l])
		l++;
	return (l);
}
