#include "cub3d.h"

int main(int argc, char **argv)
{
	t_game game;
	
	if (argc != 2)
	{
		write(2, "Usage: ./cub3D <mapfile.cub>\n", 29);
		return 1;
	}
	
	if (!validate_file_extension(argv[1]))
		return 1;
	if (!validate_file_access(argv[1]))
		return 1;
	
	memset(&game, 0, sizeof(t_game));
	
	if (!parse_file(argv[1], &game))
		return 1;
	
	if (!validate_all_textures(&game))
		return 1;
	
	print_config_data(&game);
	print_map_debug(&game);
	
	init_game(&game);
	
	if (!init_mlx(&game))
		return 1;
	
	mlx_mouse_hide(game.mlx, game.window);
	mlx_hook(game.window, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.window, 3, 1L << 1, handle_keyrelease, &game);
	mlx_hook(game.window, 17, 1L << 17, handle_close, &game);
	mlx_hook(game.window, 6, 1L << 6, handle_mouse, &game);
	mlx_loop_hook(game.mlx, render_frame, &game);
	
	mlx_loop(game.mlx);
	
	free_map(game.map, game.map_height);
	return 0;
}