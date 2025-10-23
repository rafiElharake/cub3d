#include "cub3d.h"

void move_player(t_game *game, double dx, double dy) {
    double new_x = game->player_x + dx;
    double new_y = game->player_y + dy;
    int map_x = (int)(new_x / CELL_SIZE);
    int map_y = (int)(new_y / CELL_SIZE);
    int current_map_x = (int)(game->player_x / CELL_SIZE);
    int current_map_y = (int)(game->player_y / CELL_SIZE);
    
    if (!is_wall(game, map_x, map_y) &&
        !is_wall(game, current_map_x, map_y) &&
        !is_wall(game, map_x, current_map_y)) 
        { 
        game->player_x = new_x;
        game->player_y = new_y;
    }
}
int handle_keypress(int keycode, t_game *game)
{
    printf("Key pressed: %d\n", keycode);
    if (keycode == KEY_ESC)
    {
        mlx_destroy_image(game->mlx, game->image);
        mlx_destroy_window(game->mlx, game->window);
        mlx_destroy_display(game->mlx);
        free(game->mlx);
        exit(0);
    }
    if (keycode == 'w' || keycode == 'W') {
        move_player(game, MOVE_STEP * cos(game->player_angle), MOVE_STEP * sin(game->player_angle));
    }
    if (keycode == 's' || keycode == 'S') {
        move_player(game, -MOVE_STEP * cos(game->player_angle), -MOVE_STEP * sin(game->player_angle));
    }
    if (keycode == KEY_LEFT)
        game->player_angle -= 0.1;
    if (keycode == KEY_RIGHT)
        game->player_angle += 0.1;

    return 0;
}

int handle_mouse(int x, int y, t_game *game)
{
    static int last_x = -1;
    int win_center_x = game->window_width / 2;
    if (last_x == -1)
        last_x = win_center_x;
    int dx = x - win_center_x;
    if (dx != 0)
    {
        game->player_angle += dx * 0.002;
        // Warp mouse back to center for infinite rotation
        mlx_mouse_move(game->mlx, game->window, win_center_x, game->window_height / 2);
    }
    (void)y;
    last_x = win_center_x;
    return 0;
}
