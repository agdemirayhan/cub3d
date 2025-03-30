/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 15:56:23 by msoklova          #+#    #+#             */
/*   Updated: 2025/03/30 17:10:29 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	flood_fill(t_flood params, int y, int x)
{
	if (y < 0 || y >= params.game->map_h || x < 0 || x >= params.game->map_l)
	{
		*(params.is_valid) = 0;
		return ;
	}
	if (params.map_copy[y][x] == ' ')
	{
		*(params.is_valid) = 0;
		return ;
	}
	if ((params.map_copy[y][x] == '0'
		|| ft_strchr("NSEW", params.map_copy[y][x])) &&
		params.map_copy[y][x] != 'V' && params.map_copy[y][x] != '1')
	{
		params.map_copy[y][x] = 'V';
		flood_fill(params, y - 1, x);
		flood_fill(params, y + 1, x);
		flood_fill(params, y, x - 1);
		flood_fill(params, y, x + 1);
	}
}

int	is_position_valid(t_game *game, int y, int x)
{
	if (y == 0 || y == game->map_h - 1 || x == 0 || x == game->map_l - 1)
		return (0);
	if (game->map_comp[y - 1][x] == ' ' || game->map_comp[y + 1][x] == ' ' ||
		game->map_comp[y][x - 1] == ' ' || game->map_comp[y][x + 1] == ' ')
		return (0);
	return (1);
}

int	process_map_cell(t_map_cell *map_cell, int i, int j)
{
	if (!ft_strchr(" 01NSEW", map_cell->game->map_comp[i][j]))
		return (printf("Error: Invalid character '%c' at position [%d,%d]\n",
				map_cell->game->map_comp[i][j], i, j), 0);
	if (ft_strchr("NSEW", map_cell->game->map_comp[i][j]))
	{
		(*map_cell->player_count)++;
		if (*map_cell->player_count > 1)
			return (printf("Error: Multiple players found\n"), 0);
		if (!is_position_valid(map_cell->game, i, j))
			return (printf("Error: Player not properly closed\n"), 0);
		my_fill_heroe_position(map_cell->game, i, j, map_cell->data);
	}
	if (map_cell->game->map_comp[i][j] == '0' &&
			!is_position_valid(map_cell->game, i, j))
		return (printf("Error: Open map boundary at [%d,%d]\n", i, j), 0);
	return (1);
}

int	scan_map_row(t_game *game, t_data *data, int i, int *player_count)
{
	int			j;
	t_map_cell	map_cell;

	map_cell.game = game;
	map_cell.data = data;
	map_cell.player_count = player_count;
	j = 0;
	while (j < game->map_l)
	{
		if (!process_map_cell(&map_cell, i, j))
			return (0);
		j++;
	}
	return (1);
}

int	validate_map_structure(t_game *game, t_data *data)
{
	int	i;
	int	player_count;

	i = 0;
	player_count = 0;
	while (i < game->map_h)
	{
		if (!scan_map_row(game, data, i, &player_count))
			return (0);
		i++;
	}
	if (player_count == 0)
		return (printf("Error: No player\n"), 0);
	return (1);
}
