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

void	rotate(t_data *data, int keycode)
{
	double	olddirx;
	double	oldplanex;
	double	rotation_angle;

	if (keycode == 124) // Right rotation
	{
		data->mapstate.turn_direction = -1;
		printf("Rotating right...\n");
	}
	else if (keycode == 123) // Left rotation
	{
		data->mapstate.turn_direction = 1;
		printf("Rotating left...\n");
	}
	else
		return ; // Invalid keycode, exit function
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

	// Moving forward ('W') or backward ('S')
	if (data->mapstate.keycode_fb == 'W' || data->mapstate.keycode_fb == 'S')
	{
		move_dir = (data->mapstate.keycode_fb == 'W') ? 1 : -1;
		printf("Moving %s...\n", (move_dir == 1) ? "forward" : "backward");
		if (data->map_int[(int)(data->pos.x + move_dir * data->dir.x
				* MOVE_SPEED)][(int)data->pos.y] == 0)
			data->pos.x += move_dir * data->dir.x * MOVE_SPEED;
		if (data->map_int[(int)data->pos.x][(int)(data->pos.y + move_dir
				* data->dir.y * MOVE_SPEED)] == 0)
			data->pos.y += move_dir * data->dir.y * MOVE_SPEED;
	}
	// Moving left ('A') or right ('D')
	if (data->mapstate.keycode_lr == 'A' || data->mapstate.keycode_lr == 'D')
	{
		move_side = (data->mapstate.keycode_lr == 'D') ? 1 : -1;
		printf("Strafing %s...\n", (move_side == 1) ? "right" : "left");
		if (data->map_int[(int)(data->pos.x + move_side * data->plane.x
				* MOVE_SPEED)][(int)data->pos.y] == 0)
			data->pos.x += move_side * data->plane.x * MOVE_SPEED;
		if (data->map_int[(int)data->pos.x][(int)(data->pos.y + move_side
				* data->plane.y * MOVE_SPEED)] == 0)
			data->pos.y += move_side * data->plane.y * MOVE_SPEED;
	}
	// Rotating left ('L') or right ('R')
	if (data->mapstate.keycode_r == 'L' || data->mapstate.keycode_r == 'R')
	{
		printf("Rotating %s...\n",
			(data->mapstate.keycode_r == 'L') ? "left" : "right");
		rotate(data, data->mapstate.keycode_r);
	}
}

int	game_loop(void *param)
{
	t_data		*data;
	int			x;
	t_raycast	raycast;
	t_dda		dda;
	int			y;

	x = 0;
	data = (t_data *)param;
	move_player(data);
	while (x < WIN_WIDTH)
	{
		raycast.cam_offset = 2.0 * x / (double)WIN_WIDTH - 1;
		raycast.ray.x = data->dir.x + data->plane.x * raycast.cam_offset;
		raycast.ray.y = data->dir.y + data->plane.y * raycast.cam_offset;
		dda.map_x = (int)data->pos.x;
		dda.map_y = (int)data->pos.y;
		dda.delta_dist.x = 1e30;
		if (dda.delta_dist.x != 0)
			dda.delta_dist.x = fabs(1.0 / raycast.ray.x);
		dda.delta_dist.y = 1e30;
		if (dda.delta_dist.y != 0)
			dda.delta_dist.y = fabs(1.0 / raycast.ray.y);
		if (raycast.ray.x < 0)
		{
			dda.step_x = -1;
			dda.side_dist.x = (data->pos.x - dda.map_x) * dda.delta_dist.x;
		}
		else
		{
			dda.step_x = 1;
			dda.side_dist.x = (dda.map_x + 1.0 - data->pos.x)
				* dda.delta_dist.x;
		}
		if (raycast.ray.y < 0)
		{
			dda.step_y = -1;
			dda.side_dist.y = (data->pos.y - dda.map_y) * dda.delta_dist.y;
		}
		else
		{
			dda.step_y = 1;
			dda.side_dist.y = (dda.map_y + 1.0 - data->pos.y)
				* dda.delta_dist.y;
		}
		dda.hit = 0;
		while (!dda.hit)
		{
			if (dda.side_dist.x < dda.side_dist.y)
			{
				dda.side_dist.x += dda.delta_dist.x;
				dda.map_x += dda.step_x;
				dda.side = 0;
			}
			else
			{
				dda.side_dist.y += dda.delta_dist.y;
				dda.map_y += dda.step_y;
				dda.side = 1;
			}
			if (data->map_int[dda.map_x][dda.map_y] == 1)
				dda.hit = 1;
			else
				dda.hit = 0;
		}
		if (dda.side == 0)
			dda.perp_wall_dist = (dda.side_dist.x - dda.delta_dist.x);
		else
			dda.perp_wall_dist = (dda.side_dist.y - dda.delta_dist.y);
		dda.line_height = (int)(WIN_HEIGHT / dda.perp_wall_dist);
		dda.draw_start = (int)(-dda.line_height / 2.0) + (int)(WIN_HEIGHT
				/ 2.0);
		if (dda.draw_start < 0)
			dda.draw_start = 0;
		dda.draw_end = (int)(dda.line_height / 2.0) + (int)(WIN_HEIGHT / 2.0);
		if (dda.draw_end >= WIN_HEIGHT)
			dda.draw_end = WIN_HEIGHT - 1;
		y = 0;
		while (y < dda.draw_start)
		{
			data->addr[y * WIN_WIDTH + x] = data->ceil_color;
			y++;
		}
		while (y < WIN_HEIGHT)
		{
			data->addr[y * WIN_WIDTH + x] = data->floor_color;
			y++;
		}
	}
	mlx_put_image_to_window(data->mlx.mlx_ptr, data->mlx.win_ptr, data->img, 0,
		0);
	return (0);
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
	game_loop(data);
	mlx_hook(data->mlx.win_ptr, 2, 0, key_press, data);
	mlx_loop_hook(data->mlx.mlx_ptr, game_loop, data);
	mlx_loop(data->mlx.mlx_ptr);
	free(data);
	return (0);
}

// void	rotate(t_data *data, int keycode)
// {
// 	double	olddirx;
// 	double	oldplanex;
// 	double	rotation_angle;

// 	if (keycode == 124) // Right rotation
// 		data->mapstate.turn_direction = -1;
// 	else if (keycode == 123) // Left rotation
// 		data->mapstate.turn_direction = 1;
// 	else
// 		return ;         // Invalid keycode, exit function
// 	if (keycode == 124) // Right rotation
// 		data->mapstate.turn_direction = -1;
// 	else if (keycode == 123) // Left rotation
// 		data->mapstate.turn_direction = 1;
// 	else
// 		return ; // Invalid keycode, exit function
// 	rotation_angle = data->mapstate.turn_direction * ROTSPEED;
// 	data->mapstate.angle += data->mapstate.turn_direction * (ROTSPEED * 180
// 			/ M_PI);
// 	olddirx = data->dir.x;
// 	data->dir.x = data->dir.x * cos(rotation_angle) - data->dir.y
// 		* sin(rotation_angle);
// 	data->dir.y = olddirx * sin(rotation_angle) + data->dir.y
// 		* cos(rotation_angle);
// 	oldplanex = data->plane.x;
// 	data->plane.x = data->plane.x * cos(rotation_angle) - data->plane.y
// 		* sin(rotation_angle);
// 	data->plane.y = oldplanex * sin(rotation_angle) + data->plane.y
// 		* cos(rotation_angle);
// }

// void	move_player(t_data *data)
// {
// 	double	move_dir;
// 	double	move_side;

// 	// Moving forward ('W') or backward ('S')
// 	if (data->mapstate.keycode_fb == 'W' || data->mapstate.keycode_fb == 'S')
// 	{
// 		move_dir = (data->mapstate.keycode_fb == 'W') ? 1 : -1;
// 		if (data->map_int[(int)(data->pos.x + move_dir * data->dir.x
// 				* MOVE_SPEED)][(int)data->pos.y] == 0)
// 			data->pos.x += move_dir * data->dir.x * MOVE_SPEED;
// 		if (data->map_int[(int)data->pos.x][(int)(data->pos.y + move_dir
// 				* data->dir.y * MOVE_SPEED)] == 0)
// 			data->pos.y += move_dir * data->dir.y * MOVE_SPEED;
// 	}
// 	// Moving left ('A') or right ('D')
// 	if (data->mapstate.keycode_lr == 'A' || data->mapstate.keycode_lr == 'D')
// 	{
// 		move_side = (data->mapstate.keycode_lr == 'D') ? 1 : -1;
// 		if (data->map_int[(int)(data->pos.x + move_side * data->plane.x
// 				* MOVE_SPEED)][(int)data->pos.y] == 0)
// 			data->pos.x += move_side * data->plane.x * MOVE_SPEED;
// 		if (data->map_int[(int)data->pos.x][(int)(data->pos.y + move_side
// 				* data->plane.y * MOVE_SPEED)] == 0)
// 			data->pos.y += move_side * data->plane.y * MOVE_SPEED;
// 	}
// 	// Rotating left ('L') or right ('R')
// 	if (data->mapstate.keycode_r == 'L' || data->mapstate.keycode_r == 'R')
// 		rotate(data, data->mapstate.keycode_r);
// }