/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 00:00:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 07:55:46 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	**resize_lines(char **lines, int old_cap, int new_cap, int height)
{
	char	**tmp;
	int		i;

	(void)old_cap;
	tmp = malloc(sizeof(char *) * new_cap);
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < height)
	{
		tmp[i] = lines[i];
		i++;
	}
	free(lines);
	return (tmp);
}

int	is_valid_char(char c)
{
	return (c == '0' || c == '1' || c == 'N'
		|| c == 'S' || c == 'E' || c == 'W' || c == ' ');
}

static void	free_partial_map(t_game *game, char **lines, int i)
{
	int	k;

	k = 0;
	while (k < i)
		free(game->map[k++]);
	free(game->map);
	k = 0;
	while (k < game->map_height)
		free(lines[k++]);
	free(lines);
}

int	validate_char(char c, t_game *game, char **lines, int i)
{
	if (!is_valid_char(c))
	{
		write(2, "Error: invalid character '", 26);
		write(2, &c, 1);
		write(2, "' in map\n", 9);
		free_partial_map(game, lines, i);
		return (0);
	}
	return (1);
}

char	*create_padded_line(char *src, int width)
{
	char	*dst;
	int		l;
	int		j;

	l = get_line_length(src);
	if (l > 0 && src[l - 1] == '\n')
		l--;
	dst = malloc(width + 1);
	if (!dst)
		return (NULL);
	j = 0;
	while (j < width)
		dst[j++] = '1';
	j = 0;
	while (j < l)
	{
		dst[j] = src[j];
		j++;
	}
	dst[width] = '\0';
	return (dst);
}
