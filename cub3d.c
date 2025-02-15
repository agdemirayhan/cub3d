#include "cub3d.h"

void	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	// Process stored key press
	if (game->last_key_data.action == MLX_PRESS
		|| game->last_key_data.action == MLX_REPEAT)
	{
		keyhook(game->last_key_data, game);
	}
	// Refresh screen
	// draw_3d_view(game->img, game);
	// put_image_in_map(game);
	// Debug print
	// printf("Game loop running...\n");
	fflush(stdout);
}

int	main(int argc, char **argv)
{
	t_game	game;

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
	game.last_key_data.key = 0; // Initialize last key data
	draw_grid(game.img, &game);
	mlx_image_to_window(game.mlx, game.img, 0, 0);
	// Set up key hook and loop hook
	mlx_key_hook(game.mlx, &keyhook, &game);
	mlx_loop_hook(game.mlx, &game_loop, &game);
	mlx_loop(game.mlx);
	mlx_terminate(game.mlx);
	free_map(&game.map);
	return (0);
}
