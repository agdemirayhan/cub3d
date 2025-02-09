#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_game game;

	if (argc != 2)
	{
		printf("WRONG! use: ./cub3d <map_file>\n");
		return (1);
	}
	game.map = *parse_map(argv[1], &game);
	if (!game.map.grid)
	{
		printf("Error parsing map file.\n");
		return (1);
	}
	calculate_grid_size(&game.map, &game.grid_width, &game.grid_height);
	game.window_width = game.grid_width * SQUARE_SIZE;
	game.window_height = game.grid_height * SQUARE_SIZE;
	game.mlx = mlx_init(game.window_width, game.window_height, "Cub3D", false);
	if (!game.mlx)
	{
		free_map(&game.map);
		return (1);
	}
	game.img = mlx_new_image(game.mlx, game.window_width, game.window_height);
	if (!game.img)
	{
		mlx_terminate(game.mlx);
		free_map(&game.map);
		return (1);
	}
	game.angle = 0;
	draw_grid(game.img, &game);
	mlx_image_to_window(game.mlx, game.img, 0, 0);
	mlx_key_hook(game.mlx, &keyhook, &game);
	mlx_loop(game.mlx);
	mlx_terminate(game.mlx);
	free_map(&game.map);
	return (0);
}