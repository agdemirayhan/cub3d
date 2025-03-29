/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 18:40:35 by aagdemir          #+#    #+#             */
/*   Updated: 2025/03/29 18:41:43 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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
	if (rc->ray.x == 0)
		dda_->delta_dist.x = 1e30;
	else
		dda_->delta_dist.x = fabs(1.0 / rc->ray.x);
	if (rc->ray.y == 0)
		dda_->delta_dist.y = 1e30;
	else
		dda_->delta_dist.y = fabs(1.0 / rc->ray.y);
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
