/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:19:22 by aagdemir          #+#    #+#             */
/*   Updated: 2025/03/29 18:28:23 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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
