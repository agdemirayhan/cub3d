/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:19:22 by aagdemir          #+#    #+#             */
/*   Updated: 2025/03/16 22:26:07 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	key_press(int keycode, t_data *data)
{
	if (keycode == 53)
		exit(0);
	if (keycode == 13)
		data->mapstate.keycode_fb = 'W';
	else if (keycode == 1)
		data->mapstate.keycode_fb = 'S';
	else if (keycode == 2)
		data->mapstate.keycode_lr = 'D';
	else if (keycode == 0)
		data->mapstate.keycode_lr = 'A';
	else if (keycode == 124)
		data->mapstate.keycode_r = 'R';
	else if (keycode == 123)
		data->mapstate.keycode_r = 'L';
	return (0);
}

int	key_release(int keycode, t_data *data)
{
	if (keycode == 13)
		data->mapstate.keycode_fb = 0;
	else if (keycode == 1)
		data->mapstate.keycode_fb = 0;
	else if (keycode == 2)
		data->mapstate.keycode_lr = 0;
	else if (keycode == 0)
		data->mapstate.keycode_lr = 0;
	else if (keycode == 124)
		data->mapstate.keycode_r = 0;
	else if (keycode == 123)
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

void	move_functions(t_data *data, double move_dir, double move_side)
{
	if (data->mapstate.keycode_fb == 'W' || data->mapstate.keycode_fb == 'S')
	{
		if (data->mapstate.keycode_fb == 'W')
			move_dir = 1;
		else
			move_dir = -1;
		if (data->map_int[(int)(data->pos.x + move_dir * data->dir.x
				* MOVE_SPEED)][(int)data->pos.y] == 0)
			data->pos.x += move_dir * data->dir.x * MOVE_SPEED;
		if (data->map_int[(int)data->pos.x][(int)(data->pos.y + move_dir
				* data->dir.y * MOVE_SPEED)] == 0)
			data->pos.y += move_dir * data->dir.y * MOVE_SPEED;
	}
	if (data->mapstate.keycode_lr == 'A' || data->mapstate.keycode_lr == 'D')
	{
		if (data->mapstate.keycode_lr == 'D')
			move_side = 1;
		else
			move_side = -1;
		if (data->map_int[(int)(data->pos.x + move_side * data->plane.x
				* MOVE_SPEED)][(int)data->pos.y] == 0)
			data->pos.x += move_side * data->plane.x * MOVE_SPEED;
		if (data->map_int[(int)data->pos.x][(int)(data->pos.y + move_side
				* data->plane.y * MOVE_SPEED)] == 0)
			data->pos.y += move_side * data->plane.y * MOVE_SPEED;
	}
}

void	move_player(t_data *data)
{
	double	move_dir;
	double	move_side;

	if (data->mapstate.keycode_fb == 'W' || data->mapstate.keycode_fb == 'S'
		|| data->mapstate.keycode_lr == 'A' || data->mapstate.keycode_lr == 'D')
		move_functions(data, move_dir, move_side);
	if (data->mapstate.keycode_r == 'L' || data->mapstate.keycode_r == 'R')
		rotate(data, data->mapstate.keycode_r);
}
