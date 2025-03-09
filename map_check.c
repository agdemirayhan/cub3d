#include "cub3d.h"

void	flood_fill(char **map_copy, int y, int x, int *is_valid, t_game *game)
{
	if (y < 0 || y >= game->map_h || x < 0 || x >= game->map_l)
	{
		*is_valid = 0;
		return ;
	}
	if (map_copy[y][x] == ' ')
	{
		*is_valid = 0;
		return ;
	}
	if ((map_copy[y][x] == '0' || ft_strchr("NSEW", map_copy[y][x])) &&
		map_copy[y][x] != 'V' && map_copy[y][x] != '1')
	{
		map_copy[y][x] = 'V';
		flood_fill(map_copy, y - 1, x, is_valid, game);
		flood_fill(map_copy, y + 1, x, is_valid, game);
		flood_fill(map_copy, y, x - 1, is_valid, game);
		flood_fill(map_copy, y, x + 1, is_valid, game);
	}
}

int	validate_map_boundaries(t_game *game)
{
	int		is_valid;
	char	**map_copy;
	int		i;
	int		j;
	int		start_x;
	int		start_y;

	is_valid = 1;
	map_copy = malloc(sizeof(char *) * (game->map_h + 1));
	if (!map_copy)
		return (0);
	i = 0;
	while (i < game->map_h)
	{
		map_copy[i] = ft_strdup(game->map_comp[i]);
		if (!map_copy[i])
		{
			while (--i >= 0)
				free(map_copy[i]);
			free(map_copy);
			return (0);
		}
		i++;
	}
	map_copy[game->map_h] = NULL;
	start_x = -1;
	start_y = -1;
	i = 0;
	while (i < game->map_h && start_x == -1)
	{
		j = 0;
		while (j < game->map_l)
		{
			if (ft_strchr("NSEW0", map_copy[i][j]))
			{
				start_y = i;
				start_x = j;
				break;
			}
			j++;
		}
		i++;
	}
	if (start_x == -1)
	{
		printf("Error: No valid starting position found\n");
		i = 0;
		while (i < game->map_h)
			free(map_copy[i++]);
		free(map_copy);
		return 0;
	}
	flood_fill(map_copy, start_y, start_x, &is_valid, game);
	i = 0;
	while (i < game->map_h)
		free(map_copy[i++]);
	free(map_copy);
	if (!is_valid)
		printf("Error: Map is not properly enclosed by walls\n");
	return is_valid;
}

void	expand_map_lines(t_game *game)
{
	int	i;
	int	j;
	int	current_len;
	char *expanded;

	i = 0;
	while (i < game->map_h)
	{
		current_len = ft_strlen(game->map_comp[i]);
		if (current_len >= game->map_l)
		{
			i++;
			continue;
		}
		expanded = malloc(game->map_l + 1);
		if (!expanded)
			return ;
		j = 0;
		while (j < current_len)
		{
			expanded[j] = game->map_comp[i][j];
			j++;
		}
		while (j < game->map_l)
			expanded[j++] = ' ';
		expanded[game->map_l] = '\0';
		free(game->map_comp[i]);
		game->map_comp[i] = expanded;
		i++;
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

int	validate_map_structure(t_game *game, t_data *data)
{
	int	i;
	int	j;
	int	player_count;

	i = 0;
	player_count = 0;
	while (i < game->map_h)
	{
		j = 0;
		while (j < game->map_l)
		{
			if (!ft_strchr(" 01NSEW", game->map_comp[i][j]))
				return (printf("Error: Invalid character '%c' at position [%d,%d]\n",
							game->map_comp[i][j], i, j), 0);
			if (ft_strchr("NSEW", game->map_comp[i][j]))
			{
				player_count++;
				if (player_count > 1)
					return (printf("Error: Multiple players found\n"), 0);
				if (!is_position_valid(game, i, j))
					return (printf("Error: Player position not properly enclosed by walls\n"), 0);
				my_fill_heroe_position(game, i, j, data);
			}
			if (game->map_comp[i][j] == '0' && !is_position_valid(game, i, j))
				return (printf("Error: Open map boundary at [%d,%d]\n", i, j), 0);
			j++;
		}
		i++;
	}
	if (player_count == 0)
		return (printf("Error: No player\n"), 0);
	return (1);
}
