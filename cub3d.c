#include "cub3d.h"

void	sidedist_step(t_data *data, t_raycast *rc, t_dda *dda_)
{
	if (rc->ray.x < 0)
	{
		dda_->step_x = -1;
		dda_->side_dist.x = (data->pos.x - dda_->map_x) * dda_->delta_dist.x;
	}
	else
	{
		dda_->step_x = 1;
		dda_->side_dist.x = (dda_->map_x + 1.0 - data->pos.x)
			* dda_->delta_dist.x;
	}
	if (rc->ray.y < 0)
	{
		dda_->step_y = -1;
		dda_->side_dist.y = (data->pos.y - dda_->map_y) * dda_->delta_dist.y;
	}
	else
	{
		dda_->step_y = 1;
		dda_->side_dist.y = (dda_->map_y + 1.0 - data->pos.y)
			* dda_->delta_dist.y;
	}
}
static void	prepare_dda(t_data *data, t_raycast *rc, t_dda *dda_)
{
	dda_->map_x = (int)data->pos.x;
	dda_->map_y = (int)data->pos.y;
	dda_->delta_dist.x = (rc->ray.x == 0) ? 1e30 : fabs(1.0 / rc->ray.x);
	dda_->delta_dist.y = (rc->ray.y == 0) ? 1e30 : fabs(1.0 / rc->ray.y);
	sidedist_step(data, rc, dda_);
}

void	dda(t_data *data, t_raycast *rc, t_dda *dda_)
{
	prepare_dda(data, rc, dda_);
	dda_->hit = 0;
	while (!dda_->hit)
	{
		if (dda_->side_dist.x < dda_->side_dist.y)
		{
			dda_->side_dist.x += dda_->delta_dist.x;
			dda_->map_x += dda_->step_x;
			dda_->side = X;
		}
		else
		{
			dda_->side_dist.y += dda_->delta_dist.y;
			dda_->map_y += dda_->step_y;
			dda_->side = Y;
		}
		if (data->map_int[dda_->map_x][dda_->map_y] == 1)
			dda_->hit = 1;
		else
			dda_->hit = 0;
	}
	if (dda_->side == X)
		dda_->perp_wall_dist = (dda_->side_dist.x - dda_->delta_dist.x);
	else
		dda_->perp_wall_dist = (dda_->side_dist.y - dda_->delta_dist.y);
}

void	raycast_helper(t_data *data, t_raycast *rc, t_dda *dda_, int x)
{
	(*rc).camera_x = 2.0 * x / (double)WIN_WIDTH - 1;
	(*rc).ray.x = data->dir.x + data->plane.x * (*rc).camera_x;
	(*rc).ray.y = data->dir.y + data->plane.y * (*rc).camera_x;
	dda(data, rc, dda_);
	(*dda_).line_height = (int)(WIN_HEIGHT / (*dda_).perp_wall_dist);
	(*dda_).draw_start = (int)(-(*dda_).line_height / 2.0) + (int)(WIN_HEIGHT
			/ 2.0);
}

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

int	get_color(t_data *data, t_dda *dda_, t_raycast *rc)
{
	int	color;

	color = 0;
	if (data->texx < 0 || data->texx >= data->tex_w || data->texy < 0
		|| data->texy >= data->tex_h)
	{
		printf("ERRORRR!\n");
		return (0xFFFFFF);
	}
	if (!data->cnv_addr1 || !data->cnv_addr2 || !data->cnv_addr3
		|| !data->cnv_addr4)
	{
		printf("ERROR: Texture addresses are NULL!\n");
		return (0xFFFFFF);
	}
	if (dda_->side == 0 && rc->ray.x > 0)
		color = *(data->cnv_addr1 + (data->tex_h * data->texy + data->texx));
	else if (dda_->side == 0 && rc->ray.x < 0)
		color = *(data->cnv_addr2 + (data->tex_h * data->texy + data->texx));
	else if (dda_->side == 1 && rc->ray.y > 0)
		color = *(data->cnv_addr3 + (data->tex_h * data->texy + data->texx));
	else if (dda_->side == 1 && rc->ray.y < 0)
		color = *(data->cnv_addr4 + (data->tex_h * data->texy + data->texx));
	return (color);
}

void	tex_onwhich_side(t_data *data, t_dda *dda_, t_raycast *rc)
{
	if (dda_->side == 0 && rc->ray.x > 0)
	{
		data->tex_w = data->tex_w1;
		data->tex_h = data->tex_h1;
	}
	else if (dda_->side == 0 && rc->ray.x < 0)
	{
		data->tex_w = data->tex_w2;
		data->tex_h = data->tex_h2;
	}
	else if (dda_->side == 1 && rc->ray.y > 0)
	{
		data->tex_w = data->tex_w3;
		data->tex_h = data->tex_h3;
	}
	else if (dda_->side == 1 && rc->ray.y < 0)
	{
		data->tex_w = data->tex_w4;
		data->tex_h = data->tex_h4;
	}
}

void	texture_prep(t_data *data, t_dda *dda_, t_raycast *rc)
{
	double	wallx;

	if (dda_->side == 0)
		wallx = data->pos.y + dda_->perp_wall_dist * rc->ray.y;
	else
		wallx = data->pos.x + dda_->perp_wall_dist * rc->ray.x;
	wallx -= floor(wallx);
	tex_onwhich_side(data, dda_, rc);
	data->texx = (int)(wallx * (double)data->tex_w);
	if (dda_->side == 0 && rc->ray.x > 0)
		data->texx = data->tex_w - data->texx - 1;
	if (dda_->side == 1 && rc->ray.y < 0)
		data->texx = data->tex_w - data->texx - 1;
}
void	texture_loop(t_data *data, t_dda *dda_, t_raycast *rc, int x)
{
	double	step;
	double	texpos;
	int		y;
	int		color;

	texture_prep(data, dda_, rc);
	step = 1.0 * data->tex_h / (double)dda_->line_height;
	texpos = (dda_->draw_start - WIN_HEIGHT / 2.0 + dda_->line_height / 2.0)
		* step;
	y = dda_->draw_start;
	while (y <= dda_->draw_end)
	{
		data->texy = ((int)texpos) % data->tex_h;
		if (data->texy < 0)
			data->texy += data->tex_h;
		texpos += step;
		color = get_color(data, dda_, rc);
		// printf("color: %d\n", color);
		if (dda_->side == 1)
			color = (color >> 1) & 8355711;
		data->addr[y * WIN_WIDTH + x] = color;
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

int	main(int argc, char **argv)
{
	t_game	game;
	t_data	data;
	int		i;

	// atexit(check_leaks);
	if (argc != 2)
	{
		printf("WRONG! use: ./cub3d <map_file>\n");
		return (1);
	}
	// data = malloc(sizeof(t_data));
	// if (!data)
	// {
	// 	printf("Memory allocation failed!\n");
	// 	return (1);
	// }
	ft_memset(&game, 0, sizeof(t_game));
	if (parsing(argv[1], &data, &game) != 0)
	{
		printf("Error: Map parsing failed. Exiting.\n");
		//free(&data);
		return (1);
	}
	if (init_window_and_map(&data, &game) != 0)
	{
		printf("Error: Failed to initialize window or map. Exiting.\n");
		if (game.map_comp)
		{
			i = 0;
			while (i < game.map_h)
			{
				free(game.map_comp[i]);
				i++;
			}
			free(game.map_comp);
		}
		//free(&data);
		return (1);
	}
	game_loop(&data);
	mlx_hook(data.mlx.win_ptr, 2, 0, key_press, &data);
	mlx_hook(data.mlx.win_ptr, 3, 0, key_release, &data);
	mlx_loop_hook(data.mlx.mlx_ptr, game_loop, &data);
	mlx_loop(data.mlx.mlx_ptr);
	//free(&data);
	return (0);
}

// int	main(int argc, char **argv)
//{
//	t_game	game;
//	t_data	*data;

//	if (argc != 2)
//	{
//		printf("WRONG! use: ./cub3d <map_file>\n");
//		return (1);
//	}
//	data = malloc(sizeof(t_data));
//	if (!data)
//	{
//		printf("Memory allocation failed!\n");
//		return (1);
//	}
//	parsing(argv[1], data, &game);
//	init_window_and_map(data, &game);
//	game_loop(data);
//	mlx_hook(data->mlx.win_ptr, 2, 0, key_press, data);
//	mlx_loop_hook(data->mlx.mlx_ptr, game_loop, data);
//	mlx_loop(data->mlx.mlx_ptr);
//	free(data);
//	return (0);
//}