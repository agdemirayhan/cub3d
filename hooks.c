#include "cub3d.h"

void	clear_image(mlx_image_t *img, uint32_t color)
{
	int	x;
	int	y;

	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			mlx_put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}

void	put_image_in_map(t_game *game)
{
	static int	first_render = 1;

	if (!game->img)
	{
		fprintf(stderr, "Error: game->img is NULL\n");
		return ;
	}
	clear_image(game->img, 0x000000FF);
	draw_3d_view(game->img, game);
	if (first_render)
	{
		mlx_image_to_window(game->mlx, game->img, 0, 0);
		first_render = 0;
	}
}

void	move_up(t_game *game)
{
	double	new_posx;
	double	new_posy;
	printf("Moving up | Angle: %f\n", game->angle);
	fflush(stdout);
	new_posx = game->posx + cos(game->angle) * MOVE_SPEED;
	new_posy = game->posy + sin(game->angle) * MOVE_SPEED;
	if (!is_wall(game, new_posx, new_posy))
	{
		game->posx = new_posx;
		game->posy = new_posy;
	}
}

void	move_down(t_game *game)
{
	double	new_posx;
	double	new_posy;
	printf("Moving down | Angle: %f\n", game->angle);
	fflush(stdout);
	new_posx = game->posx - cos(game->angle) * MOVE_SPEED;
	new_posy = game->posy - sin(game->angle) * MOVE_SPEED;
	if (!is_wall(game, new_posx, new_posy))
	{
		game->posx = new_posx;
		game->posy = new_posy;
	}
}

void	move_left(t_game *game)
{
	double	new_posx;
	double	new_posy;
	printf("Moving left | Angle: %f\n", game->angle);
	fflush(stdout);
	new_posx = game->posx + cos(game->angle - P2) * MOVE_SPEED;
	new_posy = game->posy + sin(game->angle - P2) * MOVE_SPEED;
	if (!is_wall(game, new_posx, new_posy))
	{
		game->posx = new_posx;
		game->posy = new_posy;
	}
}

void	move_right(t_game *game)
{
	double	new_posx;
	double	new_posy;
	printf("Moving right | Angle: %f\n", game->angle);
	fflush(stdout);
	new_posx = game->posx + cos(game->angle + P2) * MOVE_SPEED;
	new_posy = game->posy + sin(game->angle + P2) * MOVE_SPEED;
	if (!is_wall(game, new_posx, new_posy))
	{
		game->posx = new_posx;
		game->posy = new_posy;
	}
}

void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = param;
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
	{
		if (keydata.key == MLX_KEY_ESCAPE)
			mlx_close_window(game->mlx);
		if (keydata.key == MLX_KEY_W)
			game->is_moving_up = true;
		if (keydata.key == MLX_KEY_S)
			game->is_moving_down = true;
		if (keydata.key == MLX_KEY_A)
			game->is_moving_left = true;
		if (keydata.key == MLX_KEY_D)
			game->is_moving_right = true;
		if (keydata.key == MLX_KEY_LEFT)
			game->turning_left = true;
		if (keydata.key == MLX_KEY_RIGHT)
			game->turning_right = true;
	}
	if (keydata.action == MLX_RELEASE)
	{
		if (keydata.key == MLX_KEY_W)
			game->is_moving_up = false;
		if (keydata.key == MLX_KEY_S)
			game->is_moving_down = false;
		if (keydata.key == MLX_KEY_A)
			game->is_moving_left = false;
		if (keydata.key == MLX_KEY_D)
			game->is_moving_right = false;
		if (keydata.key == MLX_KEY_LEFT)
			game->turning_left = false;
		if (keydata.key == MLX_KEY_RIGHT)
			game->turning_right = false;
	}
}
