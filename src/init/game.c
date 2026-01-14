/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afahs <afahs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 05:06:32 by afahs             #+#    #+#             */
/*   Updated: 2026/01/14 16:21:43 by afahs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (0);
	game->window = mlx_new_window(game->mlx, game->window_width,
			game->window_height, "cub3D");
	if (!game->window)
		return (0);
	game->image = mlx_new_image(game->mlx, game->window_width,
			game->window_height);
	if (!game->image)
		return (0);
	game->image_data = mlx_get_data_addr(game->image, &game->bits_per_pixel,
			&game->line_length, &game->endian);
	if (!game->image_data)
		return (0);
	if (!load_textures(game))
		return (0);
	return (1);
}

int	load_texture(t_game *game, t_texture *texture, char *path)
{
	texture->img = mlx_xpm_file_to_image(game->mlx, path, &texture->width,
			&texture->height);
	if (!texture->img)
		return (0);
	texture->data = mlx_get_data_addr(texture->img, &texture->bpp,
			&texture->line_len, &texture->endian);
	if (!texture->data)
		return (0);
	return (1);
}

int	load_textures(t_game *game)
{
	if (!load_texture(game, &game->north_texture, game->north_path))
		return (0);
	if (!load_texture(game, &game->south_texture, game->south_path))
		return (0);
	if (!load_texture(game, &game->east_texture, game->east_path))
		return (0);
	if (!load_texture(game, &game->west_texture, game->west_path))
		return (0);
	return (1);
}

int	get_texture_pixel(t_texture *texture, int x, int y)
{
	char	*pixel;

	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (0);
	pixel = texture->data + (y * texture->line_len + x * (texture->bpp / 8));
	return (*(int *) pixel);
}
