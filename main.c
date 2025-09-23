/* ************************************************************************** */
/*                                                                            */
/*   minimal_cub3d.c - A minimal working version that opens a window         */
/*   This is a simplified version to test MLX functionality and understand   */
/*   the basic graphics pipeline before implementing full raycasting         */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <mlx.h>
#include <math.h>

// Simple 2D map: 1 = wall, 0 = empty
const char *g_map[MAP_HEIGHT] = {
    "1111111111",
    "1000000001",
    "1011111101",
    "1000000101",
    "1011100101",
    "1000100101",
    "1000000001",
    "1111111111"
};

// Update is_wall to match cub3d.h prototype
int is_wall(t_game *game, int x, int y) {
    (void)game; // Suppress unused parameter warning
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return 1;
    return g_map[y][x] == '1';
}

// Update draw_simple_scene to use is_wall(game, x, y)
void draw_simple_scene(t_game *game)
{
    int x, y;
    double fov = 60.0 * (3.1415926535 / 180.0);
    double half_fov = fov / 2.0;
    double ray_angle, ray_x, ray_y, ray_dx, ray_dy, dist;
    int wall_height;
    for (x = 0; x < game->window_width; x++) {
        ray_angle = game->player_angle - half_fov + fov * x / (double)game->window_width;
        ray_dx = cos(ray_angle);
        ray_dy = sin(ray_angle);
        dist = 0.0;
        while (1) {
            ray_x = game->player_x + ray_dx * dist;
            ray_y = game->player_y + ray_dy * dist;
            int map_x = (int)(ray_x / CELL_SIZE);
            int map_y = (int)(ray_y / CELL_SIZE);
            if (is_wall(game, map_x, map_y) || dist > 1000.0)
                break;
            dist += 0.1;
        }
        if (dist > 0.0)
            wall_height = (int)(game->window_height / dist);
        else
            wall_height = game->window_height;
        int wall_start = (game->window_height - wall_height) / 2;
        int wall_end = wall_start + wall_height;
        for (y = 0; y < game->window_height; y++) {
            if (y < wall_start)
                put_pixel(game, x, y, create_color(50, 50, 100));
            else if (y < wall_end)
                put_pixel(game, x, y, create_color(180, 180, 180));
            else
                put_pixel(game, x, y, create_color(100, 50, 0));
        }
    }
    // Draw minimap grid (top-left corner)
    int minimap_offset_x = 20;
    int minimap_offset_y = 20;
    int minimap_cell = 12;
    for (int my = 0; my < MAP_HEIGHT; my++) {
        for (int mx = 0; mx < MAP_WIDTH; mx++) {
            int color = (g_map[my][mx] == '1') ? create_color(80, 80, 80) : create_color(200, 200, 200);
            for (int py = 0; py < minimap_cell; py++) {
                for (int px = 0; px < minimap_cell; px++) {
                    put_pixel(game, minimap_offset_x + mx * minimap_cell + px, minimap_offset_y + my * minimap_cell + py, color);
                }
            }
        }
    }
    // Draw player on minimap (as a small circle)
    int player_map_x = minimap_offset_x + (int)(game->player_x / CELL_SIZE * minimap_cell);
    int player_map_y = minimap_offset_y + (int)(game->player_y / CELL_SIZE * minimap_cell);
    for (int py = -2; py <= 2; py++) {
        for (int px = -2; px <= 2; px++) {
            if (px*px + py*py <= 4)
                put_pixel(game, player_map_x + px, player_map_y + py, create_color(255, 0, 0));
        }
    }
}

void put_pixel(t_game *game, int x, int y, int color)
{
    char *dst;

    if (x >= 0 && x < game->window_width && y >= 0 && y < game->window_height)
    {
        dst = game->image_data + (y * game->line_length + x * (game->bits_per_pixel / 8));
        *(unsigned int*)dst = color;
        
    }
}

int create_color(int r, int g, int b)
{
    return (r << 16) | (g << 8) | b;
}

int render_frame(t_game *game)
{
    draw_simple_scene(game);
    mlx_put_image_to_window(game->mlx, game->window, game->image, 0, 0);
    return 0; 
}

int handle_keypress(int keycode, t_game *game)
{
    double move_step = 10.0;
    double new_x = game->player_x;
    double new_y = game->player_y;
    printf("Key pressed: %d\n", keycode);
    if (keycode == KEY_ESC)
    {
        mlx_destroy_image(game->mlx, game->image);
        mlx_destroy_window(game->mlx, game->window);
        mlx_destroy_display(game->mlx);
        free(game->mlx);
        exit(0);
    }
    // Forward/back movement (W/S) in the direction the player is facing
    if (keycode == 'w' || keycode == 'W') {
        new_x += cos(game->player_angle) * move_step;
        new_y += sin(game->player_angle) * move_step;
    }
    if (keycode == 's' || keycode == 'S') {
        new_x -= cos(game->player_angle) * move_step;
        new_y -= sin(game->player_angle) * move_step;
    }
    // Left/right arrow rotates the player
    if (keycode == KEY_LEFT)
        game->player_angle -= 0.1;
    if (keycode == KEY_RIGHT)
        game->player_angle += 0.1;
    // Only update if not colliding with wall
    int map_x = (int)(new_x / CELL_SIZE);
    int map_y = (int)(new_y / CELL_SIZE);
    if (!is_wall(game, map_x, map_y)) {
        game->player_x = new_x;
        game->player_y = new_y;
    }
    return 0;
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
    return 1;
}

void init_game(t_game *game)
{
    game->mlx = NULL;
    game->window = NULL;
    game->image = NULL;
    game->image_data = NULL;

    game->window_width = WINDOW_WIDTH;
    game->window_height = WINDOW_HEIGHT;

    // Start player in an empty cell inside the minimap grid
    game->player_x = 1.5 * CELL_SIZE;
    game->player_y = 1.5 * CELL_SIZE;
    game->player_angle = 0.0;
}

int main()
{
    t_game game;
    init_game(&game);
    if (!init_mlx(&game))
        return 1;
    mlx_hook(game.window, 2, 1L << 0, handle_keypress, &game);
    mlx_hook(game.window, 17, 1L << 17, handle_close, &game);
    mlx_loop_hook(game.mlx, render_frame, &game);
    mlx_loop(game.mlx);
    return 0;
}