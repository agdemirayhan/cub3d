#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_map	*map;
	mlx_t	*mlx;
	int		grid_width;
	int		grid_height;
	int		window_width;
	int		window_height;

	if (argc != 2)
	{
		printf("WRONG! use: ./cub3d <map_file>\n");
		return (1);
	}
	map = parse_map(argv[1]);
	if (!map)
	{
		printf("Error parsing map file.\n");
		return (1);
	}
	calculate_grid_size(map, &grid_width, &grid_height);
	window_width = grid_width * SQUARE_SIZE;
	window_height = grid_height * SQUARE_SIZE;
	mlx = mlx_init(window_width, window_height, "Cub3D", false);
	if (!mlx)
		return (1);
	mlx_image_t *img = mlx_new_image(mlx, window_width, window_height);
	if (!img)
		return (1);
	draw_grid(img, map);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_loop(mlx);
	free_map(map);
	return (0);
}

