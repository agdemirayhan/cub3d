#include "cub3d.h"

static int	process_color(int fd, t_game *game)
{
	char	*tmp;

	tmp = get_next_line(fd);
	while (tmp != NULL)
	{
		printf("tmp1: %s\n", tmp);
		if ((tmp[0] == 'F' || tmp[0] == 'C') && tmp[1] == ' ')
		{
			if (rgb_color(tmp, game))
			{
				free(tmp);
				return (1);
			}
		}
		free(tmp);
		tmp = get_next_line(fd);
	}
	return (0);
}

int	load_map_file(char *argv, t_data *data, t_game *game)
{
	int	fd;

	fd = open(argv, O_RDONLY);
	if (fd < 0)
		return (printf("Error: Failed to open map file\n"), 1);
	if (load_textures(fd, data))
	{
		close(fd);
		return (1);
	}
	if (process_color(fd, game))
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	line_for_counting(char *line, int *map_start, int *map_height)
{
	int	i;

	if (line[0] == '\n' || line[0] == '\0')
		return (0);
	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '1')
	{
		*map_start = 1;
		(*map_height)++;
		return (1);
	}
	else if (*map_start && line[i] != '\0')
	{
		(*map_height)++;
		return (1);
	}
	return (0);
}

int	count_map_lines(char *argv, t_game *game)
{
	int		fd;
	char	*tmp;
	int		map_start;

	map_start = 0;
	game->map_h = 0;
	fd = open(argv, O_RDONLY);
	if (fd < 0)
		return (printf("fd fails on second open\n"), 1);
	tmp = get_next_line(fd);
	while (tmp != NULL)
	{
		line_for_counting(tmp, &map_start, &game->map_h);
		free(tmp);
		tmp = get_next_line(fd);
	}
	close(fd);
	if (game->map_h == 0)
		return (printf("Error: No valid map found\n"), 1);
	printf("Map height: %d\n", game->map_h);
	return (0);
}

int	process_map_line(t_game *game, char *line, int *i, int *map_start)
{
	int	j;
	int	line_len;

	j = 0;
	if (line[0] == '\n' || line[0] == '\0')
		return (0);
	while (line[j] && (line[j] == ' ' || line[j] == '\t'))
		j++;
	if (line[j] == '1')
		*map_start = 1;
	else if (!(*map_start) || line[j] == '\0')
		return (0);
	line_len = ft_strlen(line);
	game->map_comp[*i] = malloc(line_len + 1);
	if (!game->map_comp[*i])
		return (1);
	ft_strlcpy(game->map_comp[*i], line, line_len + 1);
	if (line_len > 0 && game->map_comp[*i][line_len - 1] == '\n')
		game->map_comp[*i][line_len - 1] = '\0';
	(*i)++;
	return (0);
}
