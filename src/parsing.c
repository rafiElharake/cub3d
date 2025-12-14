/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 02:28:10 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 08:24:49 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_parse_state(t_parse *parse)
{
	parse->has_north = 0;
	parse->has_south = 0;
	parse->has_east = 0;
	parse->has_west = 0;
	parse->has_floor = 0;
	parse->has_ceiling = 0;
	parse->player_count = 0;
	parse->map_started = 0;
	parse->map_line_count = 0;
}

int	validate_parse_complete(t_parse *parse)
{
	if (!parse->has_north || !parse->has_south ||
		!parse->has_east || !parse->has_west)
	{
		write(2, "Error\nMissing texture definition\n", 34);
		return (0);
	}
	if (!parse->has_floor || !parse->has_ceiling)
	{
		write(2, "Error\nMissing floor or ceiling color\n", 38);
		return (0);
	}
	if (parse->player_count != 1)
	{
		write(2, "Error\nMap must have exactly one player\n", 40);
		return (0);
	}
	return (1);
}

static char	*skip_whitespace(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static void	trim_end(char *str)
{
	int	len;

	len = strlen(str);
	while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' ||
		str[len - 1] == '\n' || str[len - 1] == '\r'))
	{
		str[len - 1] = '\0';
		len--;
	}
}

static int	parse_texture_id(char *line, t_game *game, t_parse *parse)
{
	char	*path;
	char	**target;
	int		*has_flag;

	line = skip_whitespace(line);
	if (line[0] == 'N' && line[1] == 'O')
	{
		target = &game->north_path;
		has_flag = &parse->has_north;
		line += 2;
	}
	else if (line[0] == 'S' && line[1] == 'O')
	{
		target = &game->south_path;
		has_flag = &parse->has_south;
		line += 2;
	}
	else if (line[0] == 'W' && line[1] == 'E')
	{
		target = &game->west_path;
		has_flag = &parse->has_west;
		line += 2;
	}
	else if (line[0] == 'E' && line[1] == 'A')
	{
		target = &game->east_path;
		has_flag = &parse->has_east;
		line += 2;
	}
	else
		return (-1);
	if (*has_flag)
	{
		write(2, "Error\nDuplicate texture definition\n", 36);
		return (0);
	}
	line = skip_whitespace(line);
	if (*line == '\0' || *line == '\n')
	{
		write(2, "Error\nTexture path missing\n", 28);
		return (0);
	}
	path = strdup(line);
	if (!path)
		return (0);
	trim_end(path);
	*target = path;
	*has_flag = 1;
	return (1);
}

static int	parse_rgb_values(char *str, int *r, int *g, int *b)
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

static int	parse_color_id(char *line, t_game *game, t_parse *parse)
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

static int	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || 
		c == 'S' || c == 'E' || c == 'W' || c == ' ');
}

static int	is_map_line(char *line)
{
	line = skip_whitespace(line);
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
	int	y;
	int	x;
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

static int	flood_fill_check(t_game *game, int x, int y, char **visited)
{
	if (x < 0 || x >= game->map_width || y < 0 || y >= game->map_height)
		return (0);
	if (game->map[y][x] == ' ')
		return (0);
	if (game->map[y][x] == '1' || visited[y][x])
		return (1);
	visited[y][x] = 1;
	if (!flood_fill_check(game, x + 1, y, visited))
		return (0);
	if (!flood_fill_check(game, x - 1, y, visited))
		return (0);
	if (!flood_fill_check(game, x, y + 1, visited))
		return (0);
	if (!flood_fill_check(game, x, y - 1, visited))
		return (0);
	return (1);
}

static int	validate_map_closed(t_game *game)
{
	char	**visited;
	int		i;
	int		result;

	visited = malloc(sizeof(char *) * game->map_height);
	if (!visited)
		return (0);
	i = 0;
	while (i < game->map_height)
	{
		visited[i] = calloc(game->map_width, sizeof(char));
		if (!visited[i])
		{
			while (--i >= 0)
				free(visited[i]);
			free(visited);
			return (0);
		}
		i++;
	}
	result = flood_fill_check(game, game->player_start_x, 
		game->player_start_y, visited);
	i = 0;
	while (i < game->map_height)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
	if (!result)
	{
		write(2, "Error\nMap is not closed by walls\n", 34);
		return (0);
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

static int	is_empty_line(char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t' && *line != '\n' && *line != '\r')
			return (0);
		line++;
	}
	return (1);
}

int	parse_file(const char *filename, t_game *game)
{
	int		fd;
	char	*line;
	int		result;

	init_parse_state(&game->parse_state);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error\nFailed to open file");
		return (0);
	}
	while ((line = get_next_line(fd)))
	{
		if (is_empty_line(line) && !game->parse_state.map_started)
		{
			free(line);
			continue ;
		}
		if (is_map_line(line))
			game->parse_state.map_started = 1;
		if (!game->parse_state.map_started)
		{
			result = parse_texture_id(line, game, &game->parse_state);
			if (result == 0)
			{
				free(line);
				close(fd);
				return (0);
			}
			if (result == -1)
			{
				result = parse_color_id(line, game, &game->parse_state);
				if (result == 0)
				{
					free(line);
					close(fd);
					return (0);
				}
				if (result == -1)
				{
					write(2, "Error\nInvalid configuration line\n", 34);
					free(line);
					close(fd);
					return (0);
				}
			}
			free(line);
		}
		else
		{
			free(line);
			break ;
		}
	}
	close(fd);
	if (!load_map(filename, game))
		return (0);
	if (!validate_map(game))
		return (0);
	return (1);
}
