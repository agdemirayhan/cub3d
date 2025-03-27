#include "cub3d.h"

void	draw_line(t_data *data, int x, int draw_start)
{
	int	y;

	y = 0;
	if (x < 0 || x >= WIN_WIDTH)
	{
		printf("ERROR: X coordinate out of bounds: x = %d\n", x);
		return ;
	}
	while (y < draw_start)
	{
		if (y >= 0 && y < WIN_HEIGHT)
			data->addr[y * WIN_WIDTH + x] = data->ceil_color;
		else
			printf("WARNING: Skipped out-of-bounds pixel at y = %d\n", y);
		y++;
	}
	while (y < WIN_HEIGHT)
	{
		if (y >= 0 && y < WIN_HEIGHT)
			data->addr[y * WIN_WIDTH + x] = data->floor_color;
		else
			printf("WARNING: Skipped out-of-bounds pixel at y = %d\n", y);
		y++;
	}
}

int	game_loop(void *param)
{
	t_data		*data;
	int			x;
	t_raycast	rc;
	t_dda		dda_;
	int			y;

	x = 0;
	data = (t_data *)param;
	move_player(data);
	while (x < WIN_WIDTH)
	{
		raycast_helper(data, &rc, &dda_, x);
		if (dda_.draw_start < 0)
			dda_.draw_start = 0;
		dda_.draw_end = (int)(dda_.line_height / 2.0) + (int)(WIN_HEIGHT / 2.0);
		if (dda_.draw_end >= WIN_HEIGHT)
			dda_.draw_end = WIN_HEIGHT - 1;
		draw_line(data, x, dda_.draw_start);
		texture_loop(data, &dda_, &rc, x);
		x++;
	}
	mlx_put_image_to_window(data->mlx.mlx_ptr, data->mlx.win_ptr, data->img, 0,
		0);
	return (0);
}

void	check_leaks(void)
{
	char	command[256];

	snprintf(command, sizeof(command), "leaks %d", getpid());
	system(command);
}

void	free_gamecomp(t_game *game)
{
	int	i;

	{
		i = 0;
		while (i < game->map_h)
		{
			free(game->map_comp[i]);
			i++;
		}
		free(game->map_comp);
	}
}

int	main(int argc, char **argv)
{
	t_game	game;
	t_data	data;

	if (LEAKS)
		atexit(check_leaks);
	if (argc != 2)
		return (printf("WRONG! use: ./cub3d <map_file>\n"), 1);
	ft_memset(&game, 0, sizeof(t_game));
	if (parsing(argv[1], &data, &game) != 0)
		return (printf("Error: Map parsing failed. Exiting.\n"), 1);
	if (init_window_and_map(&data, &game) != 0)
	{
		printf("Error: Failed to initialize window or map. Exiting.\n");
		if (game.map_comp)
			free_gamecomp(&game);
		return (1);
	}
	game_loop(&data);
	mlx_hook(data.mlx.win_ptr, 2, 0, key_press, &data);
	mlx_hook(data.mlx.win_ptr, 3, 0, key_release, &data);
	mlx_loop_hook(data.mlx.mlx_ptr, game_loop, &data);
	mlx_loop(data.mlx.mlx_ptr);
	return (0);
}
