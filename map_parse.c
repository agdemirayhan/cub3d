#include "cub3d.h"

int check_file_type(char *argv)
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

//int	which_texture(t_data *data, char *tmp, char *path, int *sign)
//{
//	if (tmp[0] == 'N' && tmp[1] == 'O')
//	{
//		*sign = 1;
//		if (!north_texture(data, path))
//			return (0);
//	}
//	else if (tmp[0] == 'S' && tmp[1] == 'O')
//	{
//		*sign = 0;
//		if (!south_texture(data, path))
//			return (0);
//	}
//	else if (tmp[0] == 'E' && tmp[1] == 'A')
//	{
//		*sign = 2;
//		if (!east_texture(data, path))
//			return (0);
//	}
//	else if (tmp[0] == 'W' && tmp[1] == 'E')
//	{
//		*sign = 3;
//		if (!west_texture(data, path))
//			return (0);
//	}
//	else
//		return (0);
//	return (1);
//}

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

int	load_map_file(char *argv, t_data *data, t_game *game)
{
	int		fd;
	char	*tmp;
	int		i;

	fd = open(argv, O_RDONLY);
	if (fd < 0)
		return (printf("Error: Failed to open map file\n"), 1);
	if (load_textures(fd, data))
	{
		close(fd);
		return (1);
	}
	while ((tmp = get_next_line(fd)) != NULL)
	{
		printf("tmp1: %s\n", tmp);
		if ((tmp[0] == 'F' || tmp[0] == 'C') && tmp[1] == ' ')
		{
			if (rgb_color(tmp, game))
			{
				free(tmp);
				close(fd);
				return (1);
			}
		}
		free(tmp);
	}
	close(fd);
	return (0);
}

int	count_map_lines(char *argv, t_game *game)
{
	int		fd;
	char	*tmp;
	int		i;
	int		map_start;

	map_start = 0;
	fd = open(argv, O_RDONLY);
	if (fd < 0)
		return (printf("fd fails on second open\n"), 1);
	while ((tmp = get_next_line(fd)) != NULL)
	{
		if (tmp[0] == '\n' || tmp[0] == '\0')
		{
			free(tmp);
			continue ;
		}
		i = 0;
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t'))
			i++;

		if (tmp[i] == '1')
		{
			map_start = 1;
			game->map_h++;
		}
		else if (map_start && tmp[i] != '\0')
			game->map_h++;

		free(tmp);
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

int	load_map_data(char *argv, t_game *game)
{
	int		fd;
	char	*line;
	int		map_start;
	int		i;

	map_start = 0;
	i = 0;
	game->map_comp = malloc(sizeof(char *) * (game->map_h + 1));
	if (!game->map_comp)
		return (1);
	fd = open(argv, O_RDONLY);
	if (fd < 0)
	{
		free(game->map_comp);
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL && i < game->map_h)
	{
		if (process_map_line(game, line, &i, &map_start))
		{
			free_map_data(game, i, line, fd);
			return (1);
		}
		free(line);
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

//int	parsing(char *argv, t_data *data, t_game *game)
//{
//	int		fd;
//	int		i;
//	int		j;
//	char	*line;
//	int		map_start;
//	int		len;
//	char	*tmp;
//	int		line_len;
//	int		sign;
//	int		h_start;

//	map_start = 0;
//	if (check_file_type(argv) != 0)
//		return (1);

//	init_game_data(data, game);
//	fd = open(argv, O_RDONLY);
//	if (fd < 0)
//		return (printf("Error: Failed to open map file\n"), 1);
//	int r, g, b;
//	h_start = 0;
//	if (load_textures(fd, data))
//		return (1);
//	//tmp = get_next_line(fd);
//	//int k = 0;
//	//while (tmp != NULL && k < 4 )
//	//{
//	//	line = tmp;
//	//	while(*line!='.')
//	//		line++;
//	//	len = ft_strlen(line);
//	//	line[len - 1] = '\0';
//	//	printf("line: %s\n", line);
//	//	if (tmp[0] == 'N' && tmp[1] == 'O')
//	//	{
//	//		sign = 1;
//	//		data->cnv_img2 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
//	//			line, &data->tex_w2, &data->tex_h2);
//	//		data->cnv_addr2 = (int *)mlx_get_data_addr(data->cnv_img2, \
//	//			&data->cnv_bpp2, &data->cnv_ll2, &data->cnv_en2);
//	//		}
//	//	else if (tmp[0] == 'S' && tmp[1] == 'O')
//	//	{
//	//		sign = 0;
//	//		data->cnv_img1 = mlx_xpm_file_to_image(data->mlx.mlx_ptr, \
//	//			line, &data->tex_w1, &data->tex_h1);
//	//			data->cnv_addr1 = (int *)mlx_get_data_addr(data->cnv_img1, \
//	//			&data->cnv_bpp1, &data->cnv_ll1, &data->cnv_en1);
//	//		}
//	//	else if (tmp[0] == 'E' && tmp[1] == 'A')
//	//		{
//	//			sign = 2;
//	//			data->cnv_img3 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
//	//			line, &data->tex_w3, &data->tex_h3);
//	//			data->cnv_addr3 = (int *)mlx_get_data_addr(data->cnv_img3, \
//	//			&data->cnv_bpp3, &data->cnv_ll3, &data->cnv_en3);
//	//		}
//	//	else if (tmp[0] == 'W' && tmp[1] == 'E')
//	//	{
//	//		sign = 3;
//	//		data->cnv_img4 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
//	//			line, &data->tex_w4, &data->tex_h4);
//	//		data->cnv_addr4 = (int *)mlx_get_data_addr(data->cnv_img4, \
//	//			&data->cnv_bpp4, &data->cnv_ll4, &data->cnv_en4);
//	//		}
//	//	else
//	//		return (ft_putstr_fd("Error: Invalid texture identifier\n", 1),
//	//			1);
//	//	printf("sign: %d\n", sign);
//	//	// if (check_texture(sign, data) != 0)
//	//	// 	return (ft_putstr_fd("Error: Duplicate texture identifier\n", 1),
//	//	// 		1);
//	//			tmp = tmp + 2;
//	//			while (*tmp && *tmp == ' ')
//	//			tmp++;
//	//			// if (parsing_texture(tmp, sign, data) != 0)
//	//			// 	return (NULL);
//	//			// free(tmp);
//	//			tmp = get_next_line(fd);
//	//			k++;
//	//}
//	// close(fd);
//	// free(tmp);
//	// tmp = NULL;
//	// fd = open(argv, O_RDONLY);
//	while ((tmp = get_next_line(fd)) != NULL)
//	{
//		printf("tmp1: %s\n", tmp);
//		//tmp = get_next_line(fd);
//		printf("tmp1: %s\n", tmp);
//		if ((tmp[0] == 'F' || tmp[0] == 'C') && tmp[1] == ' ')
//		{
//			if (rgb_color(tmp, game))
//			{
//				free(tmp);
//				return (1);
//			}
//		}
//		// free(tmp); // Free the original pointer
//		i = 0;
//		while (tmp[i] == ' ' || tmp[i] == '\t')
//			i++;
//	}
//	close(fd);
//	free(tmp);
//	fd = open(argv, O_RDONLY);
//	 if (fd < 0)
//		return (printf("fd fails on second open\n"), 1);
//	while ((tmp = get_next_line(fd)) != NULL)
//	{
//		if (tmp[0] == '\n' || tmp[0] == '\0')
//		{
//			free(tmp);
//			continue ;
//		}
//		i = 0;
//		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t'))
//			i++;

//		if (tmp[i] == '1')
//		{
//			map_start = 1;
//			game->map_h++;
//		}
//		else if (map_start && tmp[i] != '\0')
//		{
//			game->map_h++;
//		}
//		free(tmp);
//	}
//	close(fd);
//	if (game->map_h == 0)
//		return (printf("Error: No valid map found\n"), 1);
//	printf("Map height: %d\n", game->map_h);
//	game->map_comp = malloc(sizeof(char *) * (game->map_h + 1));
//	if (!game->map_comp)
//		return (1);
//	fd = open(argv, O_RDONLY);
//	if (fd < 0)
//	{
//		free(game->map_comp);
//		return (1);
//	}
//	map_start = 0;
//	i = 0;
//	while ((line = get_next_line(fd)) != NULL && i < game->map_h)
//	{
//		if (line[0] == '\n' || line[0] == '\0') {
//			free(line);
//			continue ;
//		}
//		j = 0;
//		while (line[j] && (line[j] == ' ' || line[j] == '\t'))
//			j++;
//		if (line[j] == '1')
//		{
//			map_start = 1;
//			line_len = ft_strlen(line);
//			game->map_comp[i] = malloc(line_len + 1);
//			if (!game->map_comp[i])
//			{
//				while (--i >= 0)
//					free(game->map_comp[i]);
//				free(game->map_comp);
//				free(line);
//				close(fd);
//				return (1);
//			}
//			ft_strlcpy(game->map_comp[i], line, line_len + 1);
//			if (line_len > 0 && game->map_comp[i][line_len - 1] == '\n')
//				game->map_comp[i][line_len - 1] = '\0';
//			i++;
//		}
//		else if (map_start && line[j] != '\0')
//		{
//			line_len = ft_strlen(line);
//			game->map_comp[i] = malloc(line_len + 1);
//			if (!game->map_comp[i])
//			{
//				while (--i >= 0)
//					free(game->map_comp[i]);
//				free(game->map_comp);
//				free(line);
//				close(fd);
//				return (1);
//			}
//			ft_strlcpy(game->map_comp[i], line, line_len + 1);
//			if (line_len > 0 && game->map_comp[i][line_len - 1] == '\n')
//				game->map_comp[i][line_len - 1] = '\0';
//			i++;
//		}
//		free(line);
//	}
//	game->map_comp[i] = NULL;
//	game->map_h = i;
//	close(fd);
//	game->map_l = 0;
//	i = 0;
//	while (i < game->map_h)
//	{
//		len = ft_strlen(game->map_comp[i]);
//		if (len > game->map_l)
//			game->map_l = len;
//		i++;
//	}
//	expand_map_lines(game);
//	print_map_comp(game);
//	printf("Map width: %d\n", game->map_l);
//	if (!validate_map_structure(game, data))
//	{
//		i = 0;
//		while (i < game->map_h)
//		{
//			free(game->map_comp[i]);
//			i++;
//		}
//		free(game->map_comp);
//		game->map_comp = NULL;
//		return (1);
//	}
//	if (!validate_map_boundaries(game))
//	{
//		i = 0;
//		while (i < game->map_h)
//		{
//			free(game->map_comp[i]);
//			i++;
//		}
//		free(game->map_comp);
//		game->map_comp = NULL;
//		return (1);
//	}
//	return (0);
//}
