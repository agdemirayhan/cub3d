#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_map	*map;
	mlx_t	*mlx;

	if (argc != 2)
	{
		printf("WRONG! use: ./cub3d <map_file>\n");
		return (1);
	}
	map = parse_map(argv[1]);
	if (!map) {
		printf("Error parsing map file.\n");
		return (1);
	}
	mlx = mlx_init(WIDTH, HEIGHT, "Cub3D", false);
	if (!mlx)
		return (1);
	mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
		return (1);
	draw_grid(img, map);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_loop(mlx);
	free_map(map);
	return (0);
}
