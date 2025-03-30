/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_boundry.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:11:53 by msoklova          #+#    #+#             */
/*   Updated: 2025/03/30 16:36:30 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

char	**create_map_copy(t_game *game)
{
	char	**map_copy;
	int		i;

	map_copy = malloc(sizeof(char *) * (game->map_h + 1));
	if (!map_copy)
		return (NULL);
	i = 0;
	while (i < game->map_h)
	{
		map_copy[i] = ft_strdup(game->map_comp[i]);
		if (!map_copy[i])
		{
			while (--i >= 0)
				free(map_copy[i]);
			free(map_copy);
			return (NULL);
		}
		i++;
	}
	map_copy[game->map_h] = NULL;
	return (map_copy);
}

int	find_start_pos(t_game *game, char **map_copy, int *start_x, int *start_y)
{
	int	i;
	int	j;

	*start_x = -1;
	*start_y = -1;
	i = 0;
	while (i < game->map_h && *start_x == -1)
	{
		j = 0;
		while (j < game->map_l)
		{
			if (ft_strchr("NSEW0", map_copy[i][j]))
			{
				*start_y = i;
				*start_x = j;
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

void	free_map_copy(char **map_copy, int height)
{
	int	i;

	i = 0;
	while (i < height)
		free(map_copy[i++]);
	free(map_copy);
}

t_flood	init_flood(char **map_copy, int *is_valid, t_game *game)
{
	t_flood	params;

	params.map_copy = map_copy;
	params.is_valid = is_valid;
	params.game = game;
	return (params);
}

int	validate_map_boundaries(t_game *game)
{
	int				is_valid;
	char			**map_copy;
	int				start_x;
	int				start_y;
	t_flood			params;

	is_valid = 1;
	map_copy = create_map_copy(game);
	if (!map_copy)
		return (0);
	if (!find_start_pos(game, map_copy, &start_x, &start_y))
	{
		printf("Error: No valid starting position found\n");
		free_map_copy(map_copy, game->map_h);
		return (0);
	}
	params = init_flood(map_copy, &is_valid, game);
	flood_fill(params, start_y, start_x);
	free_map_copy(map_copy, game->map_h);
	if (!is_valid)
		printf("Error: Map is not properly enclosed by walls\n");
	return (is_valid);
}
