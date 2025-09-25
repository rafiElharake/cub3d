#include "cub3d.h"

int is_wall(t_game *game, int x, int y) {
    if (!game->map || x < 0 || x >= game->map_width || y < 0 || y >= game->map_height)
        return 1;
    return game->map[y][x] == '1';
}

void draw_minimap(t_game *game) {
    //int minimap_offset_x = 20;
    //int minimap_offset_y = 20;
   // int minimap_cell = 12;
    int my = 0;
    while (my < game->map_height) {
        int mx = 0;
        while (mx < game->map_width) {
            int color = (game->map[my][mx] == '1') ? create_color(80, 80, 80) : create_color(200, 200, 200);
            int py = 0;
            while (py < 12) {
                int px = 0;
                while (px < 12) {
                    put_pixel(game, 20 + mx * 12 + px, 20 + my * 12 + py, color);
                    px++;
                }
                py++;
            }
            mx++;
        }
        my++;
    }
    int player_map_x = 20 + (int)(game->player_x / CELL_SIZE * 12);
    int player_map_y = 20 + (int)(game->player_y / CELL_SIZE * 12);
    int py = -2;
    while (py <= 2) {
        int px = -2;
        while (px <= 2) {
            if (px*px + py*py <= 4)
                put_pixel(game, player_map_x + px, player_map_y + py, create_color(255, 0, 0));
            px++;
        }
        py++;
    }
}

void draw_simple_scene(t_game *game)
{
    int x, y;
    double fov = 60.0 * (3.1415926535 / 180.0);
    double half_fov = fov / 2.0;
    double ray_angle, ray_x, ray_y, ray_dx, ray_dy, dist;
    int wall_height;
    x = 0;
    while (x < game->window_width) {
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
        y = 0;
        while (y < game->window_height) {
            if (y < wall_start)
                put_pixel(game, x, y, create_color(50, 50, 100));
            else if (y < wall_end)
                put_pixel(game, x, y, create_color(180, 180, 180));
            else
                put_pixel(game, x, y, create_color(100, 50, 0));
            y++;
        }
        x++;
    }
    draw_minimap(game);
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
