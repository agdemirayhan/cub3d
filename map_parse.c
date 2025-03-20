#include "cub3d.h"

int	setup_map_data(char *argv, t_game *game, int *fd)
{
	game->map_comp = malloc(sizeof(char *) * (game->map_h + 1));
	if (!game->map_comp)
		return (1);
	*fd = open(argv, O_RDONLY);
	if (*fd < 0)
	{
		free(game->map_comp);
		return (1);
	}
	return (0);
}

int	load_map_data(char *argv, t_game *game)
{
	int		fd;
	char	*line;
	int		map_start;
	int		i;

	map_start = 0;
	i = 0;
	if (setup_map_data(argv, game, &fd))
		return (1);
	line = get_next_line(fd);
	while (line != NULL && i < game->map_h)
	{
		if (process_map_line(game, line, &i, &map_start))
		{
			free_map_data(game, i, line, fd);
			return (1);
		}
		free(line);
		line = get_next_line(fd);
	}
	game->map_comp[i] = NULL;
	game->map_h = i;
	close(fd);
	return (0);
}

void	find_max_width_and_expand(t_game *game)
{
	int	i;
	int	len;

	i = 0;
	game->map_l = 0;
	while (i < game->map_h)
	{
		len = ft_strlen(game->map_comp[i]);
		if (len > game->map_l)
			game->map_l = len;
		i++;
	}
	expand_map_lines(game);
	print_map_comp(game);
	printf("Map width: %d\n", game->map_l);
}

int	validate_map(t_game *game, t_data *data)
{
	if (!validate_map_structure(game, data))
	{
		free_map_array(game);
		return (0);
	}
	if (!validate_map_boundaries(game))
	{
		free_map_array(game);
		return (0);
	}
	return (1);
}

int	parsing(char *argv, t_data *data, t_game *game)
{
	if (check_file_type(argv) != 0)
		return (1);
	init_game_data(data, game);
	if (load_map_file(argv, data, game))
		return (1);
	if (count_map_lines(argv, game))
		return (1);
	if (load_map_data(argv, game))
		return (1);
	find_max_width_and_expand(game);
	if (!validate_map(game, data))
		return (1);
	return (0);
}
