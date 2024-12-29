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
	int	x;
	int	y;
	int	px;
	int	py;

	printf("game->window_width:%d\n", game->window_width);
	printf("game->window_height:%d\n", game->window_height);
	printf("game->posx:%d\n", game->posx);
	printf("game->posy:%d\n", game->posy);
	fflush(stdout);
	if (!game->img)
	{
		fprintf(stderr, "Error: game->img is NULL\n");
		return ;
	}
	y = 0;
	while (y < game->window_height)
	{
		x = 0;
		while (x < game->window_width)
		{
			px = game->posx + x;
			py = game->posy + y;
			if (px >= 0 && px < game->window_width && py >= 0
				&& py < game->window_height)
			{
				printf("MOVE");
				fflush(stdout);
				mlx_put_pixel(game->img, px, py, 0xFF0000FF);
			}
			x++;
		}
		y++;
	}
}

void	move(t_game *game, int dy, int dx)
{
	int	new_posy;
	int	new_posx;

	new_posy = game->posy + dy;
	new_posx = game->posx + dx;
	game->posy = new_posy;
	game->posx = new_posx;
}

void	move_up(t_game *game)
{
	move(game, -1, 0);
	printf("UP");
	fflush(stdout);
}

void	move_down(t_game *game)
{
	move(game, 1, 0);
}

void	move_right(t_game *game)
{
	move(game, 0, 1);
}

void	move_left(t_game *game)
{
	move(game, 0, -1);
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
		if (keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
			move_up(game);
		if (keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
			move_down(game);
		if (keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
			move_left(game);
		if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
			move_right(game);
		// if (game->status == END)
		// {
		// 	ft_printf("Congratulations!\n");
		// 	mlx_close_window(game->mlx);
		// }
	}
	put_image_in_map(game);
}