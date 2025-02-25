#include "cub3d.h"

/* PRINTING THE MOVES
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
*/

void	rotate(t_data *data, int keycode)
{
	double	olddirx;
	double	oldplanex;
	double	rotation_angle;

	if (keycode == 124) // Right rotation
		data->mapstate.turn_direction = -1;
	else if (keycode == 123) // Left rotation
		data->mapstate.turn_direction = 1;
	else
		return ;         // Invalid keycode, exit function
	if (keycode == 124) // Right rotation
		data->mapstate.turn_direction = -1;
	else if (keycode == 123) // Left rotation
		data->mapstate.turn_direction = 1;
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

void	move_player(t_data *data)
{
	double	move_dir;
	double	move_side;

	// Moving forward ('W') or backward ('S')
	if (data->mapstate.keycode_fb == 'W' || data->mapstate.keycode_fb == 'S')
	{
		move_dir = (data->mapstate.keycode_fb == 'W') ? 1 : -1;
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
		if (data->map_int[(int)(data->pos.x + move_side * data->plane.x
				* MOVE_SPEED)][(int)data->pos.y] == 0)
			data->pos.x += move_side * data->plane.x * MOVE_SPEED;
		if (data->map_int[(int)data->pos.x][(int)(data->pos.y + move_side
				* data->plane.y * MOVE_SPEED)] == 0)
			data->pos.y += move_side * data->plane.y * MOVE_SPEED;
	}
	// Rotating left ('L') or right ('R')
	if (data->mapstate.keycode_r == 'L' || data->mapstate.keycode_r == 'R')
		rotate(data, data->mapstate.keycode_r);
}

void	game_loop(void *param)
{
	int		x;
	t_data	*data;
	double	move_dir;
	double	move_side;

	x = 0;
	data = (t_data *)param;
	move_player(data);
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
	game_loop(&data);
	mlx_loop(data->mlx.mlx_ptr);
	free(data);
	return (0);
}
