/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 08:30:00 by afahs             #+#    #+#             */
/*   Updated: 2025/12/14 12:38:09 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	validate_file_extension(const char *filename)
{
	int	len;

	len = strlen(filename);
	if (len < 5)
	{
		write(2, "Error\nInvalid filename\n", 23);
		return (0);
	}
	if (strcmp(filename + len - 4, ".cub") != 0)
	{
		write(2, "Error\nFile must have .cub extension\n", 37);
		return (0);
	}
	return (1);
}

int	validate_file_access(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error\nCannot open file");
		return (0);
	}
	close(fd);
	return (1);
}

static int	check_xpm_extension(const char *path)
{
	int	len;

	len = strlen(path);
	if (len < 5 || strcmp(path + len - 4, ".xpm") != 0)
	{
		write(2, "Error\nTexture must be .xpm file: ", 34);
		write(2, path, strlen(path));
		write(2, "\n", 1);
		return (0);
	}
	return (1);
}

int	validate_texture_file(const char *path)
{
	int	fd;

	if (!path)
	{
		write(2, "Error\nTexture path is NULL\n", 28);
		return (0);
	}
	if (!check_xpm_extension(path))
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		write(2, "Error\nCannot open texture file: ", 33);
		write(2, path, strlen(path));
		write(2, "\n", 1);
		return (0);
	}
	close(fd);
	return (1);
}

int	validate_all_textures(t_game *game)
{
	if (!validate_texture_file(game->north_path))
		return (0);
	if (!validate_texture_file(game->south_path))
		return (0);
	if (!validate_texture_file(game->east_path))
		return (0);
	if (!validate_texture_file(game->west_path))
		return (0);
	return (1);
}
