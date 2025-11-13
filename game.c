#include "cub3d.h"

void init_game(t_game *game)
{
    game->mlx = NULL;
    game->window = NULL;
    game->image = NULL;
    game->image_data = NULL;
    game->window_width = WINDOW_WIDTH;
    game->window_height = WINDOW_HEIGHT;

    
    game->input.w = 0;
    game->input.a = 0;
    game->input.s = 0;
    game->input.d = 0;
    game->input.left = 0;
    game->input.right = 0;
    
    int found = 0;
    int y = 0;
    while (y < game->map_height && !found) {
        int x = 0;
        while (x < game->map_width && !found) {
            char c = game->map[y][x];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W') {
                game->player_x = (x + 0.5) * CELL_SIZE;
                game->player_y = (y + 0.5) * CELL_SIZE;
                switch (c) {
                    case 'N': game->player_angle = -M_PI/2; break;
                    case 'S': game->player_angle = M_PI/2; break;
                    case 'E': game->player_angle = 0.0; break;
                    case 'W': game->player_angle = M_PI; break;
                }
                found = 1;
            }
            x++;
        }
        y++;
    }
    if (!found) {
        game->player_x = 1.5 * CELL_SIZE;
        game->player_y = 1.5 * CELL_SIZE;
        game->player_angle = 0.0;
    }
}

int init_mlx(t_game *game)
{
    game->mlx = mlx_init();
    if (!game->mlx)
        return 0;
    game->window = mlx_new_window(game->mlx, game->window_width, game->window_height, "cub3D - Minimal Version");
    if (!game->window)
        return 0;
    game->image = mlx_new_image(game->mlx, game->window_width, game->window_height);
    if (!game->image)
        return 0;
    game->image_data = mlx_get_data_addr(game->image, &game->bits_per_pixel, 
                                        &game->line_length, &game->endian);
    if (!game->image_data)
        return 0;
    if (!load_textures(game))
        return 0;
    
    return 1;
}

int load_texture(t_game *game, t_texture *texture, char *path)
{
    texture->img = mlx_xpm_file_to_image(game->mlx, path, &texture->width, &texture->height);
    if (!texture->img)
        return 0;
    texture->data = mlx_get_data_addr(texture->img, &texture->bpp, &texture->line_len, &texture->endian);
    if (!texture->data)
        return 0;
    return 1;
}

int load_textures(t_game *game)
{
    if (!load_texture(game, &game->north_texture, game->north_path))
        return 0;
    if (!load_texture(game, &game->south_texture, game->south_path))
        return 0;
    if (!load_texture(game, &game->east_texture, game->east_path))
        return 0;
    if (!load_texture(game, &game->west_texture, game->west_path))
        return 0;
    return 1;
}

int get_texture_pixel(t_texture *texture, int x, int y)
{
    char *pixel;
    
    if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
        return 0;
    
    pixel = texture->data + (y * texture->line_len + x * (texture->bpp / 8));
    return *(int*)pixel;
}

int handle_close(t_game *game)
{
    mlx_destroy_image(game->mlx, game->image);
    mlx_destroy_window(game->mlx, game->window);
    mlx_destroy_display(game->mlx);
    free(game->mlx);
    exit(0);
    return 0;
}
