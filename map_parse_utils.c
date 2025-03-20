#include "cub3d.h"

int	check_file_type(char *argv)
{
	int	len;

	len = ft_strlen(argv);
	if (len < 4 || ft_strncmp(argv + len - 4, ".cub", 4) != 0)
		return (printf("Error: wrong map type\n"), 1);
	return (0);
}

void	init_game_data(t_data *data, t_game *game)
{
	data->mlx.mlx_ptr = mlx_init();
	game->map_h = 0;
	game->ceil_color = 100000000;
	game->floor_color = 100000000;
	data->cnv_img1 = NULL;
	data->cnv_img2 = NULL;
	data->cnv_img3 = NULL;
	data->cnv_img4 = NULL;
}

int	load_textures(int fd, t_data *data)
{
	char	*tmp;
	char	*path;
	int		sign;
	int		k;

	k = 0;
	tmp = get_next_line(fd);
	while (tmp != NULL && k < 4)
	{
		path = texture_path(tmp);
		if (!which_texture(data, tmp, path, &sign))
		{
			free(tmp);
			return (ft_putstr_fd("Error: Failed to load texture\n", 1), 1);
		}
		printf("sign: %d\n", sign);
		free(tmp);
		tmp = get_next_line(fd);
		k++;
	}
	return (0);
}

void	free_map_data(t_game *game, int i, char *line, int fd)
{
	while (--i >= 0)
		free(game->map_comp[i]);
	free(game->map_comp);
	free(line);
	close(fd);
}

void	free_map_array(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->map_h)
	{
		free(game->map_comp[i]);
		i++;
	}
	free(game->map_comp);
	game->map_comp = NULL;
}
