#include "cub3d.h"

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
	// draw_3d_view(game->img, game);
	mlx_image_to_window(game->mlx, game->img, 0, 0);
}

void	move(t_game *game, int dy, int dx)
{
	int	new_posy;
	int	new_posx;

	new_posy = game->posy + (dy * 5);
	new_posx = game->posx + (dx * 5);
	game->posy = new_posy;
	game->posx = new_posx;
	printf("Moved to: (%d, %d)\n", game->posx, game->posy);
	return ; // Exit after successful move
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
		{
			game->angle -= PI / 32;
			if (game->angle < 0)
				game->angle += 2 * PI;
			printf("game->angle: %f\n", game->angle);
			fflush(stdout);
		}
		if (keydata.key == MLX_KEY_RIGHT)
		{
			game->angle += PI / 32;
			if (game->angle > 2 * PI)
				game->angle -= 2 * PI;
			printf("game->angle: %f\n", game->angle);
			fflush(stdout);
		}
		// if (game->status == END)
		// {
		// 	ft_printf("Congratulations!\n");
		// 	mlx_close_window(game->mlx);
		// }
	}
	put_image_in_map(game);
}