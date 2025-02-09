#include "cub3d.h"

// void	player_to_window(t_game *game, int x, int y)
// {
// 	mlx_image_to_window(game->mlx, game->assets.terrain, x * TILESIZE, y
// 			* TILESIZE);
// 	if (game->map[y][x] == '1')
// 		mlx_image_to_window(game->mlx, game->assets.wall, x * TILESIZE, y
// 				* TILESIZE);
// 	else if (game->map[y][x] == 'E')
// 		mlx_image_to_window(game->mlx, game->assets.hut, x * TILESIZE, y
// 				* TILESIZE);
// 	else if (game->map[y][x] == 'P')
// 		mlx_image_to_window(game->mlx, game->assets.lumberjack, x * TILESIZE, y
// 				* TILESIZE);
// 	else if (game->map[y][x] == 'X')
// 	{
// 		mlx_image_to_window(game->mlx, game->assets.hut, x * TILESIZE, y
// 				* TILESIZE);
// 		mlx_image_to_window(game->mlx, game->assets.lumberjack, x * TILESIZE, y
// 				* TILESIZE);
// 	}
// 	else if (game->map[y][x] == 'C')
// 		mlx_image_to_window(game->mlx, game->assets.tree, x * TILESIZE, y
// 				* TILESIZE);
// }

void	put_image_in_map(t_game *game)
{
	// int x;
	// int y;
	if (!game->img)
	{
		fprintf(stderr, "Error: game->img is NULL\n");
		return ;
	}
	draw_grid(game->img, game);
	mlx_image_to_window(game->mlx, game->img, 0, 0);
}

void	move(t_game *game, int dy, int dx)
{
	int	new_posy;
	int	new_posx;

	new_posy = game->posy + dy;
	new_posx = game->posx + dx;
	if (game->map.grid[new_posy][new_posx] == '0')
	{
		game->posy = new_posy;
		game->posx = new_posx;
	}
}

void	move_up(t_game *game)
{
	move(game, -1, 0);
	printf("UP\n");
	fflush(stdout);
}

void	move_down(t_game *game)
{
	move(game, 1, 0);
	printf("DOWN\n");
	fflush(stdout);
}

void	move_right(t_game *game)
{
	move(game, 0, 1);
	printf("RIGHT\n");
	fflush(stdout);
}

void	move_left(t_game *game)
{
	move(game, 0, -1);
	printf("LEFT\n");
	fflush(stdout);
}

void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_game *game;

	game = param;
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
	{
		if (keydata.key == MLX_KEY_ESCAPE)
		{
			mlx_close_window(game->mlx);
		}
		if (keydata.key == MLX_KEY_W)
			move_up(game);
		if (keydata.key == MLX_KEY_S)
			move_down(game);
		if (keydata.key == MLX_KEY_A)
			move_left(game);
		if (keydata.key == MLX_KEY_D)
			move_right(game);
		if (keydata.key == MLX_KEY_LEFT)
			game->angle -= 0.1;
		if (keydata.key == MLX_KEY_RIGHT)
			game->angle += 0.1;
		// if (game->status == END)
		// {
		// 	ft_printf("Congratulations!\n");
		// 	mlx_close_window(game->mlx);
		// }
	}
	put_image_in_map(game);
}