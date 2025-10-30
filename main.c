#include "cub3d.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        write(2, "Usage: ./cub3D <mapfile>\n", 25);
        return 1;
    }
    t_game game;
    if (!load_map(argv[1], &game)) {
        write(2, "Failed to load map file\n", 24);
        return 1;
    }
    init_game(&game);
    if (!init_mlx(&game))
        return 1;
    mlx_mouse_hide(game.mlx, game.window);
    mlx_hook(game.window, 2, 1L << 0, handle_keypress, &game);
    mlx_hook(game.window, 3, 1L << 1, handle_keyrelease, &game);
    mlx_hook(game.window, 17, 1L << 17, handle_close, &game);
    mlx_hook(game.window, 6, 1L << 6, handle_mouse, &game); // Mouse motion
    mlx_loop_hook(game.mlx, render_frame, &game);
    mlx_loop(game.mlx);
    int i = 0;
    while (i < game.map_height) {
        free(game.map[i]);
        i++;
    }
    free(game.map);
    return 0;
}