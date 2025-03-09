#include "cub3d.h"

void	free_map(t_map *map)
{
	int	i;

	free(map->texture_no);
	free(map->texture_so);
	free(map->texture_we);
	free(map->texture_ea);
	i = 0;
	while (map->grid[i])
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	free(map);
}

int	parse_color(char *line, int *r, int *g, int *b)
{
	char	**components;
	int		i;

	components = ft_split(line, ',');
	if (!components || !components[0] || !components[1] || !components[2])
		return (1);
	*r = atoi(components[0]);
	*g = atoi(components[1]);
	*b = atoi(components[2]);
	i = 0;
	while (components[i])
	{
		free(components[i]);
		i++;
	}
	free(components);
	return (0);
}

void	parse_textures(char *line, t_map *map)
{
	if (ft_strncmp(line, "NO", 2) == 0)
		map->texture_no = ft_strdup(&line[3]);
	else if (ft_strncmp(line, "SO", 2) == 0)
		map->texture_so = ft_strdup(&line[3]);
	else if (ft_strncmp(line, "WE", 2) == 0)
		map->texture_we = ft_strdup(&line[3]);
	else if (ft_strncmp(line, "EA", 2) == 0)
		map->texture_ea = ft_strdup(&line[3]);
}

char	**parse_grid(int fd, char *line, t_game *game)
{
	char	**grid;
	char	*newline;
	int		i;
	int		j;

	i = 0;
	grid = malloc(sizeof(char *) * 1024);
	while (line)
	{
		newline = ft_strchr(line, '\n');
		if (newline)
			*newline = '\0';
		grid[i] = ft_strdup(line);
		j = 0;
		while (grid[i][j])
		{
			if (grid[i][j] == 'N' || grid[i][j] == 'S' || grid[i][j] == 'E'
				|| grid[i][j] == 'W')
			{
				// game->posx = j * SQUARE_SIZE + (SQUARE_SIZE / 2 - 10 / 2);
				// game->posy = i * SQUARE_SIZE + (SQUARE_SIZE / 2 - 10 / 2);
				//game->posx = j * SQUARE_SIZE;
				//game->posy = i * SQUARE_SIZE;
				game->posx = j * SQUARE_SIZE + (SQUARE_SIZE / 2);
				game->posy = i * SQUARE_SIZE + (SQUARE_SIZE / 2);
				//grid[i][j] = '0';
				if (grid[i][j] == 'N')
					game->angle = -PI/2;
				else if (grid[i][j] == 'S')
					game->angle = PI/2;
				else if (grid[i][j] == 'E')
					game->angle = 0;
				else if (grid[i][j] == 'W')
					game->angle = PI;
				grid[i][j] = '0';
				if (game->posx < 1 || game->posx >= game->map_l - 1
					|| game->posy < 1 || game->posy >= game->map_h - 1)
				{
					printf("ERROR: Player placed too close to the edge! posx = %d, posy = %d\n", game->posx, game->posy);
					exit(1);
				}
			}
			j++;
		}
		i++;
		line = get_next_line(fd);
	}
	grid[i] = NULL;
	return (grid);
}

int	allocate_map_memory(t_data *data, t_game *game)
{
	unsigned int i, j;
	if (game->map_h <= 0 || game->map_l <= 0)
	{
		printf("ERROR: Invalid map dimensions! map_h=%d, map_l=%d\n",
			game->map_h, game->map_l);
		return (1);
	}
	data->map_int = malloc(sizeof(int *) * game->map_h);
	if (!data->map_int)
		return (1);
	for (i = 0; i < game->map_h; i++)
	{
		data->map_int[i] = malloc(sizeof(int) * game->map_l);
		if (!data->map_int[i])
		{
			printf("ERROR: Memory allocation failed at row %d\n", i);
			// Free allocated rows before returning
			while (i > 0)
				free(data->map_int[--i]);
			free(data->map_int);
			return (1);
		}
		for (j = 0; j < game->map_l; j++)
			data->map_int[i][j] = -1;
	}
	return (0);
}

void	convert_map_to_int(t_data *data, t_game *game)
{
	unsigned int i, j;
	for (i = 0; i < game->map_h; i++)
	{
		for (j = 0; j < game->map_l && game->map_comp[i][j] != '\0'; j++)
		{
			if (game->map_comp[i][j] == '0' || game->map_comp[i][j] == '1')
				data->map_int[i][j] = game->map_comp[i][j] - '0';
			else
				data->map_int[i][j] = -1;
		}
	}
}

void	print_map_int(t_data *data, t_game *game)
{
	unsigned int i, j;
	printf("Map Integer Representation:\n");
	for (i = 0; i < game->map_h; i++)
	{
		for (j = 0; j < game->map_l; j++)
		{
			printf("%d ", data->map_int[i][j]);
		}
		printf("\n");
	}
}

int	init_window_and_map(t_data *data, t_game *game)
{
	data->mlx.win_ptr = mlx_new_window(data->mlx.mlx_ptr, WIN_WIDTH, WIN_HEIGHT,
			"cub3d");
	if (!data->mlx.win_ptr)
		return (1);
	data->img = mlx_new_image(data->mlx.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!data->img)
	{
		printf("ERROR: mlx_new_image failed!\n");
		return (1);
	}
	data->addr = (int *)mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	if (!data->addr)
	{
		printf("ERROR: mlx_get_data_addr failed!\n");
		return (1);
	}
	// Allocate and convert the map
	if (allocate_map_memory(data, game))
		return (1);
	convert_map_to_int(data, game);
	print_map_int(data, game);
	return (0);
}

// TEMP printer
void	print_map_comp(t_game *game)
{
	int	i;

	if (!game || !game->map_comp)
	{
		printf("Map components not initialized.\n");
		return ;
	}
	printf("Printing map components (total %d lines):\n", game->map_h);
	for (i = 0; i < game->map_h; i++)
	{
		printf("%s\n", game->map_comp[i]);
	}
}

void	my_fill_heroe_position_1(t_data *data, char *direction)
{
	if (direction[0] == 'S')
	{
		data->dir.x = 1;
		data->dir.y = 0;
		data->plane.x = 0;
		data->plane.y = -0.66;
		data->mapstate.rot = 0;
	}
	if (direction[0] == 'E')
	{
		data->dir.x = 0;
		data->dir.y = 1;
		data->plane.x = 0.66;
		data->plane.y = 0;
		data->mapstate.rot = 90;
	}
	if (direction[0] == 'W')
	{
		data->dir.x = 0;
		data->dir.y = -1;
		data->plane.x = -0.66;
		data->plane.y = 0;
		data->mapstate.rot = 270;
	}
}

void	my_fill_heroe_position(t_game *game, int y, int x, t_data *data)
{
	char	*directions;
	char	*direction;

	directions = "ESWN";
	data->pos.x = y + 0.25;
	data->pos.y = x + 0.25;
	direction = ft_strchr(directions, game->map_comp[y][x]);
	game->map_comp[y][x] = '0';
	if (direction[0] == 'N')
	{
		data->dir.x = -1;
		data->dir.y = 0;
		data->plane.x = 0;
		data->plane.y = 0.66;
		data->mapstate.rot = 180;
	}
	my_fill_heroe_position_1(data, direction);
}

static int	my_check_chars_and_heroes(t_game *game, t_data *data)
{
	unsigned int	i;
	int				j;
	int				qty_heroes;

	qty_heroes = 0;
	i = -1;
	while (++i < game->map_h)
	{
		j = -1;
		while (game->map_comp[i][++j] != '\0')
		{
			if (!ft_strchr(" 01NSEW", game->map_comp[i][j]))
				return (printf("Error: Invalid character in map\n"), 1);
			if (ft_strchr("NSEW", game->map_comp[i][j]))
			{
				if (qty_heroes > 0)
					return (printf("Error: More than one hero in map\n"), 1);
				qty_heroes = 1;
				my_fill_heroe_position(game, i, j, data);
			}
		}
	}
	return (0);
}

int	check_texture(int sign, t_data *data)
{
	void	*textures[] = {data->cnv_img1, data->cnv_img2, data->cnv_img3,
			data->cnv_img4};

	if (sign >= 0 && sign < 4 && textures[sign] != NULL)
		return (sign + 1);
	return (0);
}

int extract_number(char **line) {
     int value = 0;
 
     while (**line == ' ') (*line)++; // Skip spaces
 
     if (!ft_isdigit(**line)) return -1; // Ensure it's a digit
 
     while (ft_isdigit(**line)) { // Convert string to int
         value = value * 10 + (**line - '0');
         (*line)++;
     }
 
     if (value > 255) return -1; // Ensure valid RGB range
 
     while (**line == ' ') (*line)++; // Skip spaces
 
     return value;
 }

int	parsing(char *argv, t_data *data, t_game *game)
{
	int		fd;
	int		i;
	int		j;
	char	*line;
	int		map_start;
	int		len;
	char	*tmp;
	int		line_len;
	int		sign;
	int		h_start;

	map_start = 0;
	len = ft_strlen(argv);
	if (len < 4 || ft_strncmp(argv + len - 4, ".cub", 4) != 0)
		return (printf("Error: wrong map type\n"), 1);
	fd = open(argv, O_RDONLY);
	if (fd < 0)
		return (printf("Error: Failed to open map file\n"), 1);
	data->mlx.mlx_ptr = mlx_init();

	//map height
	game->map_h = 0;
	game->ceil_color = 100000000;
	game->floor_color = 100000000;
	data->cnv_img1 = NULL;
	data->cnv_img2 = NULL;
	data->cnv_img3 = NULL;
	data->cnv_img4 = NULL;
	int r, g, b;
	h_start = 0;
	tmp = get_next_line(fd);
	int k = 0;
	while (tmp != NULL && k < 4 )
	{
		if (tmp[0] == 'N' && tmp[1] == 'O')
		{
			sign = 1;
			data->cnv_img2 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
				line, &data->tex_w2, &data->tex_h2);
			}
		else if (tmp[0] == 'S' && tmp[1] == 'O')
		{
			sign = 0;
			data->cnv_img1 = mlx_xpm_file_to_image(data->mlx.mlx_ptr, \
				line, &data->tex_w1, &data->tex_h1);
			}
		else if (tmp[0] == 'E' && tmp[1] == 'A')
			{
				sign = 2;
				data->cnv_img3 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
				line, &data->tex_w3, &data->tex_h3);
			}
		else if (tmp[0] == 'W' && tmp[1] == 'E')
		{
			sign = 3;
			data->cnv_img4 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
				line, &data->tex_w4, &data->tex_h4);
			}	
		else
			return (ft_putstr_fd("Error: Invalid texture identifier\n", 1),
				1);
		printf("sign: %d\n", sign);
		if (check_texture(sign, data) != 0)
			return (ft_putstr_fd("Error: Duplicate texture identifier\n", 1),
				1);
				tmp = tmp + 2;
				while (*tmp && *tmp == ' ')
				tmp++;
				// if (parsing_texture(tmp, sign, data) != 0)
				// 	return (NULL);
				// free(tmp);
				tmp = get_next_line(fd);
				k++;
	}
	close(fd);
	free(tmp);
	fd = open(argv, O_RDONLY);
	 if (fd < 0)
        return (printf("fd fails on second open\n"), NULL);
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
		{
			game->map_h++;
		}
		free(tmp);
	}
	close(fd);
	if (game->map_h == 0)
		return (printf("Error: No valid map found\n"), 1);
	printf("Map height: %d\n", game->map_h);
	game->map_comp = malloc(sizeof(char *) * (game->map_h + 1));
	if (!game->map_comp)
		return (1);
	fd = open(argv, O_RDONLY);
	if (fd < 0)
	{
		free(game->map_comp);
		return (1);
	}
	map_start = 0;
	i = 0;
	while ((line = get_next_line(fd)) != NULL && i < game->map_h)
	{
		if (line[0] == '\n' || line[0] == '\0') {
			free(line);
			continue ;
		}
		j = 0;
		while (line[j] && (line[j] == ' ' || line[j] == '\t'))
			j++;
		if (line[j] == '1')
		{
			map_start = 1;
			line_len = ft_strlen(line);
			game->map_comp[i] = malloc(line_len + 1);
			if (!game->map_comp[i])
			{
				while (--i >= 0)
					free(game->map_comp[i]);
				free(game->map_comp);
				free(line);
				close(fd);
				return (1);
			}
			ft_strlcpy(game->map_comp[i], line, line_len + 1);
			if (line_len > 0 && game->map_comp[i][line_len - 1] == '\n')
				game->map_comp[i][line_len - 1] = '\0';
			i++;
		}
		else if (map_start && line[j] != '\0')
		{
			line_len = ft_strlen(line);
			game->map_comp[i] = malloc(line_len + 1);
			if (!game->map_comp[i])
			{
				while (--i >= 0)
					free(game->map_comp[i]);
				free(game->map_comp);
				free(line);
				close(fd);
				return (1);
			}
			ft_strlcpy(game->map_comp[i], line, line_len + 1);
			if (line_len > 0 && game->map_comp[i][line_len - 1] == '\n')
				game->map_comp[i][line_len - 1] = '\0';
			i++;
		}
		free(line);
	}
	game->map_comp[i] = NULL;
	game->map_h = i;
	close(fd);
	game->map_l = 0;
	i = 0;
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
	if (!validate_map_structure(game, data))
	{
		i = 0;
		while (i < game->map_h)
		{
			free(game->map_comp[i]);
			i++;
		}
		free(game->map_comp);
		game->map_comp = NULL;
		return (1);
	}
	if (!validate_map_boundaries(game))
	{
		i = 0;
		while (i < game->map_h)
		{
			free(game->map_comp[i]);
			i++;
		}
		free(game->map_comp);
		game->map_comp = NULL;
		return (1);
	}
	return (0);
}

//void	*parsing(char *argv, t_data *data, t_game *game)
//{
//	int		fd;
//	int		i;
//	int		j;
//	int		h_start;
//	char	*tmp;
//	char	*line;

//	fd = open(argv, O_RDONLY);
//	printf("here: %s\n", argv);
//	if (fd < 0)
//		return (printf("fd fails\n"), NULL);
//	data->mlx.mlx_ptr = mlx_init();
//	game->map_h = 0;
//	game->ceil_color = 100000000;
//	game->floor_color = 100000000;
//	game->map_h_tmp = 0;
//	data->cnv_img1 = NULL;
//	data->cnv_img2 = NULL;
//	data->cnv_img3 = NULL;
//	data->cnv_img4 = NULL;
//	h_start = 0;
//	tmp = get_next_line(fd);
//	while (tmp != NULL)
//	{
//		i = 0;
//		while (tmp[i] == ' ' || tmp[i] == '\t')
//			i++;
//		if (tmp[i] != '\0')
//		{
//			if (tmp[i] == '1')
//			{
//				j = ft_strlen(tmp) - 1;
//				while (j >= 0 && (tmp[j] == ' ' || tmp[j] == '\t'
//						|| tmp[j] == '\n'))
//					j--;
//				if (j >= 0 && tmp[j] == '1')
//					h_start = 1;
//			}
//		}
//		if (h_start)
//			game->map_h++;
//		free(tmp);
//		tmp = get_next_line(fd);
//	}
//	close(fd);
//	printf("game->map_h: %d\n", game->map_h);
//	fd = open(argv, O_RDONLY);
//	if (fd < 0)
//		return (printf("fd fails on second open\n"), NULL);
//	while ((line = get_next_line(fd)) != NULL)
//	{
//		i = 0;
//		while (line[i] == ' ' || line[i] == '\t')
//			i++;
//		if (line[i] != '\0' && line[i] == '1')
//		{
//			j = ft_strlen(line) - 1;
//			while (j >= 0 && (line[j] == ' ' || line[j] == '\t'
//					|| line[j] == '\n'))
//				j--;
//			if (j >= 0 && line[j] == '1')
//				break ;
//		}
//		free(line);
//	}
//	game->map_comp = (char **)malloc(sizeof(char *) * game->map_h);
//	if (!game->map_comp)
//	{
//		close(fd);
//		return (printf("malloc map_comp fails\n"), NULL);
//	}
//	i = 0;
//	while (i < game->map_h && line != NULL)
//	{
//		game->map_comp[i] = malloc(1000);
//		if (!game->map_comp[i])
//			return (printf("malloc map_comp[%d] fails\n", i), NULL);
//		ft_memcpy(game->map_comp[i], line, ft_strlen(line) + 1);
//		if (game->map_comp[i][ft_strlen(line) - 1] == '\n')
//			game->map_comp[i][ft_strlen(line) - 1] = '\0';
//		free(line);
//		i++;
//		line = get_next_line(fd);
//	}
//	close(fd);
//	game->map_l = ft_strlen(game->map_comp[0]);
//	i = 0;
//	while (i < game->map_h)
//	{
//		if (game->map_l < ft_strlen(game->map_comp[i]))
//			game->map_l = ft_strlen(game->map_comp[i]);
//		i++;
//	}
//	print_map_comp(game);
//	printf("map_l:%d\n", game->map_l);
//	if (my_check_chars_and_heroes(game, data) != 0)
//	{
//		printf("Error: Invalid map configuration\n");
//		return (NULL);
//	}
//	i = -1;
//	while (++i < game->map_h)
//	{
//		printf("i:%d\n",i);
//		j = game->map_l;
//		if (j < ft_strlen(game->map_comp[i]))
//		{
//			game->map_comp[i][j] = '\0';
//		}
//		while (j > 0 && game->map_comp[i][--j] != '1')
//			game->map_comp[i][j] = ' ';
//	}
//	return (NULL);
//}

void	calculate_grid_size(t_map *map, int *grid_width, int *grid_height)
{
	int	width;
	int	height;
	int	row_width;

	width = 0;
	height = 0;
	while (map->grid[height])
	{
		row_width = ft_strlen(map->grid[height]);
		if (row_width > width)
			width = row_width;
		height++;
	}
	*grid_width = width;
	*grid_height = height;
}

void draw_grid(void *mlx_ptr, void *win_ptr, t_game *game)
{
	if (!game || !game->map_comp)
	{
		printf("Error: Invalid game or map structure\n");
		return ;
	}

	// Draw map walls
	int y = 0;
	while (y < game->map_h)
	{
		int x = 0;
		while (x < game->map_l)
		{
			if (game->map_comp[y] && game->map_comp[y][x] == '1')
			{
				// Draw wall square
				int start_x = x * SQUARE_SIZE;
				int start_y = y * SQUARE_SIZE;

				int i = 0;
				while (i < SQUARE_SIZE)
				{
					int j = 0;
					while (j < SQUARE_SIZE)
					{
						mlx_pixel_put(mlx_ptr, win_ptr,
							start_x + i,
							start_y + j,
							0xFFFFFF);
						j++;
					}
					i++;
				}
			}
			x++;
		}
		y++;
	}

	// Verify player position
	if (game->posx >= 0 && game->posx < game->window_width &&
		game->posy >= 0 && game->posy < game->window_height)
	{
		// Draw player
		int player_size = 8;
		int i = -player_size / 2;
		while (i < player_size / 2)
		{
			int j = -player_size / 2;
			while (j < player_size / 2)
			{
				int px = game->posx + i;
				int py = game->posy + j;

				if (px >= 0 && px < game->window_width &&
					py >= 0 && py < game->window_height)
				{
					mlx_pixel_put(mlx_ptr, win_ptr, px, py, 0xFF00FF);
				}
				j++;
			}
			i++;
		}
	}
	else
	{
		printf("Error: wrong position: x=%d, y=%d\n",
			game->posx, game->posy);
	}
}

//void	draw_grid(void *mlx_ptr, void *win_ptr, t_game *game)
//{
//	int	x;
//	int	y;

//	y = 0;
//	while (y < game->map_h)
//	{
//		x = 0;
//		while (x < game->map_l)
//		{
//			if (game->map.grid[y][x] == '1')
//			{
//				int i, j;
//				for (i = 0; i < SQUARE_SIZE; i++)
//				{
//					for (j = 0; j < SQUARE_SIZE; j++)
//					{
//						mlx_pixel_put(mlx_ptr, win_ptr, x * SQUARE_SIZE + i, y * SQUARE_SIZE + j, 0xFFFFFF);
//					}
//				}
//			}
//			x++;
//		}
//		y++;
//	}
//	int player_size = 8; // Make player more visible
//	int px = game->posx - player_size/2;
//	int py = game->posy - player_size/2;

//	for (int i = 0; i < player_size; i++) {
//		for (int j = 0; j < player_size; j++) {
//			mlx_pixel_put(mlx_ptr, win_ptr, px + i, py + j, 0xFF0000);
//		}
//	}

//	// Draw direction line to show where player is facing
//	int line_length = 20;
//	int end_x = game->posx + cos(game->angle) * line_length;
//	int end_y = game->posy + sin(game->angle) * line_length;
//	for (int i = 0; i < line_length; i++) {
//		int x = game->posx + cos(game->angle) * i;
//		int y = game->posy + sin(game->angle) * i;
//		mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0xFF0000);
//	}
//}
