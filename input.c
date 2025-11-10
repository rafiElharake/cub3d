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

void process_input(t_game *game)
{
    double move_speed = MOVE_SPEED;
    double rotation_speed = ROTATION_SPEED;

    
    if (game->input.w) {
        move_player(game, move_speed * cos(game->player_angle), move_speed * sin(game->player_angle));
    }
    if (game->input.s) {
        move_player(game, -move_speed * cos(game->player_angle), -move_speed * sin(game->player_angle));
    }
    if (game->input.a) {
        // Strafe left (90 degrees counter-clockwise from facing direction)
        move_player(game, move_speed * cos(game->player_angle - M_PI/2), move_speed * sin(game->player_angle - M_PI/2));
    }
    if (game->input.d) {
        // Strafe right (90 degrees clockwise from facing direction)
        move_player(game, move_speed * cos(game->player_angle + M_PI/2), move_speed * sin(game->player_angle + M_PI/2));
    }
    if (game->input.left)
        game->player_angle -= rotation_speed;
    if (game->input.right)
        game->player_angle += rotation_speed;
}

int handle_keypress(int keycode, t_game *game)
{
    if (keycode == KEY_ESC)
    {
        mlx_destroy_image(game->mlx, game->image);
        mlx_destroy_window(game->mlx, game->window);
        mlx_destroy_display(game->mlx);
        free(game->mlx);
        exit(0);
    }
    if (keycode == 'w' || keycode == 'W') game->input.w = 1;
    if (keycode == 's' || keycode == 'S') game->input.s = 1;
    if (keycode == 'a' || keycode == 'A') game->input.a = 1;
    if (keycode == 'd' || keycode == 'D') game->input.d = 1;
    if (keycode == KEY_LEFT) game->input.left = 1;
    if (keycode == KEY_RIGHT) game->input.right = 1;
    return 0;
}

int handle_keyrelease(int keycode, t_game *game)
{
    if (keycode == 'w' || keycode == 'W') game->input.w = 0;
    if (keycode == 's' || keycode == 'S') game->input.s = 0;
    if (keycode == 'a' || keycode == 'A') game->input.a = 0;
    if (keycode == 'd' || keycode == 'D') game->input.d = 0;
    if (keycode == KEY_LEFT) game->input.left = 0;
    if (keycode == KEY_RIGHT) game->input.right = 0;
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
        mlx_mouse_move(game->mlx, game->window, win_center_x, game->window_height / 2);
    }
    (void)y;
    last_x = win_center_x;
    return 0;
}
