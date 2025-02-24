#include "cub3d.h"
#include <time.h>

void	game_loop(void *param)
{
	t_game			*game;
	static clock_t	last_time = 0;
	static int		frame_count = 0;
	static double	last_fps_update = 0;
	clock_t			current_time;
	double			delta_time;
	double			move_speed;

	// t_game_ray		*param;
	param = ft_calloc(1, sizeof(t_game));
	if (!game)
		return ;
	game = param;
	current_time = clock();
	delta_time = (double)(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;
	frame_count++;
	if ((double)(current_time) / CLOCKS_PER_SEC - last_fps_update >= 0.1)
	{
		printf("FPS: %d\n", frame_count * 10);
		fflush(stdout);
		frame_count = 0;
		last_fps_update = (double)(current_time) / CLOCKS_PER_SEC;
	}
	move_speed = MOVE_SPEED;
	if (game->is_moving_up)
	{
		game->posx += cos(game->angle) * move_speed;
		game->posy += sin(game->angle) * move_speed;
	}
	if (game->is_moving_down)
	{
		game->posx -= cos(game->angle) * move_speed;
		game->posy -= sin(game->angle) * move_speed;
	}
	if (game->is_moving_left)
	{
		game->posx += cos(game->angle - P2) * move_speed;
		game->posy += sin(game->angle - P2) * move_speed;
	}
	if (game->is_moving_right)
	{
		game->posx += cos(game->angle + P2) * move_speed;
		game->posy += sin(game->angle + P2) * move_speed;
	}
	if (game->turning_left)
	{
		game->angle -= PI / 64;
		if (game->angle < 0)
			game->angle += 2 * PI;
	}
	if (game->turning_right)
	{
		game->angle += PI / 64;
		if (game->angle > 2 * PI)
			game->angle -= 2 * PI;
	}
	// put_image_in_map(game);
	// free(param);
}

int	main(int argc, char **argv)
{
	t_game	game;
	t_data	*data;

	if (argc != 2)
	{
		printf("WRONG! use: ./cub3d <map_file>\n");
		return (1);
	}
	data = malloc(sizeof(t_data));
	if (!data)
	{
		printf("Memory allocation failed!\n");
		return (1);
	}
	parsing(argv[1], data, &game);
	init_window_and_map(data, &game);
	free(data);
	return (0);
}
