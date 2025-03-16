#include "cub3d.h"

int	key_press(int keycode, t_data *data)
{
	if (keycode == 53) // ESC key to exit
		exit(0);
	if (keycode == 13) // 'W'
		data->mapstate.keycode_fb = 'W';
	else if (keycode == 1) // 'S'
		data->mapstate.keycode_fb = 'S';
	else if (keycode == 2) // 'D'
		data->mapstate.keycode_lr = 'D';
	else if (keycode == 0) // 'A'
		data->mapstate.keycode_lr = 'A';
	else if (keycode == 124) // Right arrow → (or 'R')
		data->mapstate.keycode_r = 'R';
	else if (keycode == 123) // Left arrow ← (or 'L')
		data->mapstate.keycode_r = 'L';
	return (0);
}

int	key_release(int keycode, t_data *data)
{
	if (keycode == 13) // 'W'
		data->mapstate.keycode_fb = 0;
	else if (keycode == 1) // 'S'
		data->mapstate.keycode_fb = 0;
	else if (keycode == 2) // 'D'
		data->mapstate.keycode_lr = 0;
	else if (keycode == 0) // 'A'
		data->mapstate.keycode_lr = 0;
	else if (keycode == 124) // Right arrow
		data->mapstate.keycode_r = 0;
	else if (keycode == 123) // Left arrow
		data->mapstate.keycode_r = 0;
	return (0);
}

void	rotate(t_data *data, int keycode)
{
	double	olddirx;
	double	oldplanex;
	double	rotation_angle;

	if (keycode == 'R')
		data->mapstate.turn_direction = -1;
	else if (keycode == 'L')
		data->mapstate.turn_direction = 1;
	else
		return ;
	rotation_angle = data->mapstate.turn_direction * ROTSPEED;
	data->mapstate.angle += data->mapstate.turn_direction * (ROTSPEED * 180
			/ M_PI);
	olddirx = data->dir.x;
	data->dir.x = data->dir.x * cos(rotation_angle) - data->dir.y
		* sin(rotation_angle);
	data->dir.y = olddirx * sin(rotation_angle) + data->dir.y
		* cos(rotation_angle);
	oldplanex = data->plane.x;
	data->plane.x = data->plane.x * cos(rotation_angle) - data->plane.y
		* sin(rotation_angle);
	data->plane.y = oldplanex * sin(rotation_angle) + data->plane.y
		* cos(rotation_angle);
}

// PRINTING THE MOVES
void	move_player(t_data *data)
{
	double	move_dir;
	double	move_side;

	if (data->mapstate.keycode_fb == 'W' || data->mapstate.keycode_fb == 'S' )
	{
		move_dir = (data->mapstate.keycode_fb == 'W') ? 1 : -1;
		if (data->map_int[(int)(data->pos.x + move_dir * data->dir.x
				* MOVE_SPEED)][(int)data->pos.y] == 0)
			data->pos.x += move_dir * data->dir.x * MOVE_SPEED;
		if (data->map_int[(int)data->pos.x][(int)(data->pos.y + move_dir
				* data->dir.y * MOVE_SPEED)] == 0)
			data->pos.y += move_dir * data->dir.y * MOVE_SPEED;
	}
	if (data->mapstate.keycode_lr == 'A' || data->mapstate.keycode_lr == 'D')
	{
		move_side = (data->mapstate.keycode_lr == 'D') ? 1 : -1;
		if (data->map_int[(int)(data->pos.x + move_side * data->plane.x
				* MOVE_SPEED)][(int)data->pos.y] == 0)
			data->pos.x += move_side * data->plane.x * MOVE_SPEED;
		if (data->map_int[(int)data->pos.x][(int)(data->pos.y + move_side
				* data->plane.y * MOVE_SPEED)] == 0)
			data->pos.y += move_side * data->plane.y * MOVE_SPEED;
	}
	if (data->mapstate.keycode_r == 'L' || data->mapstate.keycode_r == 'R')
		rotate(data, data->mapstate.keycode_r);
}

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
	// printf("pos.x = %f, pos.y = %f\n", data->pos.x, data->pos.y);
	// Ensure delta_dist is never zero to avoid division errors
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
		// **Check Bounds Before Accessing map_int**
		// printf("data->mapstate.map_width: %d\n", data->mapstate.map_width);
		// Check if we hit a wall
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
	// Check if x is within bounds
	if (x < 0 || x >= WIN_WIDTH)
	{
		printf("ERROR: X coordinate out of bounds: x = %d\n", x);
		return ;
	}
	// Drawing ceiling
	while (y < draw_start)
	{
		if (y >= 0 && y < WIN_HEIGHT)
			data->addr[y * WIN_WIDTH + x] = data->ceil_color;
		else
			printf("WARNING: Skipped out-of-bounds pixel at y = %d\n", y);
		y++;
	}
	// Drawing floor
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
	// Ensure texture coordinates are within bounds
	if (data->texx < 0 || data->texx >= data->tex_w || data->texy < 0
		|| data->texy >= data->tex_h)
			return (0xFFFFFF); // Return white color as a fallback
	// Ensure texture pointers are not NULL
	if (!data->cnv_addr1 || !data->cnv_addr2 || !data->cnv_addr3
		|| !data->cnv_addr4)
	{
		printf("ERROR: Texture addresses are NULL!\n");
		return (0xFFFFFF); // Return white color as a fallback
	}
	// Select the correct texture and fetch the color
	// printf("data->tex_h: %d\n", data->tex_h);
	// printf("data->texy: %d\n", data->texy);
	// printf("data->texx: %d\n", data->texx);
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
	// 	printf("dda_->side: %d\n", dda_->side);
	// printf("rc->ray.x: %f\n", rc->ray.x);
	// printf("data->tex_w1: %d\n", data->tex_w1);
	// printf("data->tex_h1: %d\n", data->tex_h1);
	if (dda_->side == 0 && rc->ray.x > 0)
	{
		// printf("test1\n");
		data->tex_w = data->tex_w1;
		data->tex_h = data->tex_h1;
	}
	else if (dda_->side == 0 && rc->ray.x < 0)
	{
		// printf("test2\n");
		data->tex_w = data->tex_w2;
		data->tex_h = data->tex_h2;
	}
	else if (dda_->side == 1 && rc->ray.y > 0)
	{
		// printf("test3\n");
		data->tex_w = data->tex_w3;
		data->tex_h = data->tex_h3;
	}
	else if (dda_->side == 1 && rc->ray.y < 0)
	{
		// printf("test4\n");
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
	// printf("wallx: %f\n", wallx);
	// printf("data->tex_w: %d\n", data->tex_w);
	// printf("data->texx: %d\n", data->texx);
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
		if (dda_->side == 1)
			color = (color >> 1) & 8355711;
		data->addr[y * WIN_WIDTH + x] = color;
		y++;
	}
}

// void	texture_loop(t_data *data, t_dda *dda_, t_raycast *rc, int x)
//{
//	double	step;
//	double	texpos;
//	int		y;
//	int		color;

//	color = 0;
//	texture_prep(data, dda_, rc);
//	step = 1.0 * data->tex_h / dda_->line_height;
//	texpos = (dda_->draw_start - WIN_HEIGHT / 2 + dda_->line_height / 2) * step;
//	y = dda_->draw_start;
//	while (y < dda_->draw_end + 1)
//	{
//		data->texy = (int)texpos & (data->tex_h - 1);
//		texpos += step;
//		color = get_color(data, dda_, rc);
//		if (dda_->side == 1)
//			color = (color >> 1) & 8355711;
//		data->addr[y * WIN_WIDTH + x] = color;
//		y++;
//	}
//}

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
		free(&data);
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
		free(&data);
		return (1);
	}
	game_loop(&data);
	mlx_hook(data.mlx.win_ptr, 2, 0, key_press, &data);
	mlx_hook(data.mlx.win_ptr, 3, 0, key_release, &data);
	mlx_loop_hook(data.mlx.mlx_ptr, game_loop, &data);
	mlx_loop(data.mlx.mlx_ptr);
	free(&data);
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