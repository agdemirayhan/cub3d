/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_dim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 18:27:12 by msoklova          #+#    #+#             */
/*   Updated: 2025/03/30 18:27:14 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	validate_map_dimensions(t_game *game)
{
	if (game->map_h <= 0 || game->map_l <= 0)
	{
		printf("ERROR: Invalid map dimensions! map_h=%d, map_l=%d\n",
			game->map_h, game->map_l);
		return (1);
	}
	return (0);
}

static void	initialize_row(int *row, int length)
{
	int	j;

	j = 0;
	while (j < length)
	{
		row[j] = -1;
		j++;
	}
}

static int	allocate_rows(t_data *data, t_game *game)
{
	int	i;

	i = 0;
	while (i < game->map_h)
	{
		data->map_int[i] = malloc(sizeof(int) * game->map_l);
		if (!data->map_int[i])
		{
			printf("ERROR: Memory allocation failed at row %d\n", i);
			while (i > 0)
				free(data->map_int[--i]);
			free(data->map_int);
			return (1);
		}
		initialize_row(data->map_int[i], game->map_l);
		i++;
	}
	return (0);
}

int	allocate_map_memory(t_data *data, t_game *game)
{
	if (validate_map_dimensions(game))
		return (1);
	data->map_int = malloc(sizeof(int *) * game->map_h);
	if (!data->map_int)
		return (1);
	if (allocate_rows(data, game))
		return (1);
	return (0);
}
