/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 18:42:02 by aagdemir          #+#    #+#             */
/*   Updated: 2025/03/29 18:51:31 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	get_color(t_data *data, t_dda *dda_, t_raycast *rc)
{
	int	color;

	color = 0;
	if (data->texx < 0 || data->texx >= data->tex_w || data->texy < 0
		|| data->texy >= data->tex_h)
		return (0xFFFFFF);
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
		if (dda_->side == 1)
			color = (color >> 1) & 8355711;
		data->addr[y * WIN_WIDTH + x] = color;
		y++;
	}
}
