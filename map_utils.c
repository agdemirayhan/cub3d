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
				game->posx = j * SQUARE_SIZE;
				game->posy = i * SQUARE_SIZE;
				grid[i][j] = '0';
			}
			j++;
		}
		i++;
		line = get_next_line(fd);
	}
	grid[i] = NULL;
	return (grid);
}

t_map	*parse_map(const char *filename, t_game *game)
{
	t_map	*map;
	char	*line;
	int		fd;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	ft_memset(map, 0, sizeof(t_map));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	line = get_next_line(fd);
	while (line)
	{
		if (ft_strncmp(line, "F ", 2) == 0)
			parse_color(&line[2], &map->floor_r, &map->floor_g, &map->floor_b);
		else if (ft_strncmp(line, "C ", 2) == 0)
			parse_color(&line[2], &map->ceiling_r, &map->ceiling_g,
				&map->ceiling_b);
		else if (ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO",
				2) == 0 || ft_strncmp(line, "WE", 2) == 0 || ft_strncmp(line,
				"EA", 2) == 0)
			parse_textures(line, map);
		else if (line[0] == ' ' || line[0] == '1')
		{
			map->grid = parse_grid(fd, line, game);
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (map);
}

void	draw_rectangle(mlx_image_t *img, int x, int y, int width, int height,
		uint32_t color)
{
	int	i;
	int	j;
	int	px;
	int	py;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			px = x + j;
			py = y + i;
			if (px >= 0 && px < (int)img->width && py >= 0
				&& py < (int)img->height)
			{
				mlx_put_pixel(img, px, py, color);
			}
			if ((px % SQUARE_SIZE == 0) || (py % SQUARE_SIZE == 0))
			{
				mlx_put_pixel(img, px, py, 0xFFFFFFFF);
			}
			j++;
		}
		i++;
	}
}

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

void	draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1,
		uint32_t color)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	err = dx - dy;
	if (x0 < x1)
		sx = 1;
	else
		sx = -1;
	if (y0 < y1)
		sy = 1;
	else
		sy = -1;
	while (1)
	{
		if (x0 >= 0 && x0 < (int)img->width && y0 >= 0 && y0 < (int)img->height)
			mlx_put_pixel(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = err * 2;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void	draw_3d_view(mlx_image_t *img, t_game *game)
{
	int		x;
	float	ray_angle;
	float	ray_x;
	float	ray_y;
	float	distance;
	int		wall_height;

	int start_y, end_y;
	// Loop through every column (x) in the viewport
	for (x = 0; x < game->window_width; x++)
	{
		// Calculate the angle for the current ray
		ray_angle = game->angle + atan((x - game->window_width / 2)
				/ (float)(game->window_width / 2));
		// Initialize ray position to the player's position
		ray_x = game->posx;
		ray_y = game->posy;
		distance = 0;
		// Cast the ray until it hits a wall ('1')
		while (game->map.grid[(int)(ray_y / SQUARE_SIZE)][(int)(ray_x
				/ SQUARE_SIZE)] == '0')
		{
			ray_x += cos(ray_angle) * 1; // Move the ray forward
			ray_y += sin(ray_angle) * 1;
			distance++;
		}
		// Correct for fisheye distortion
		distance *= cos(ray_angle - game->angle);
		// Calculate the height of the wall based on distance
		wall_height = (SQUARE_SIZE * game->window_height) / (distance * 2);
		// Determine the starting and ending Y positions of the wall slice
		start_y = (game->window_height / 2) - (wall_height / 2);
		end_y = start_y + wall_height;
		// Clamp values within the screen bounds
		if (start_y < 0)
			start_y = 0;
		if (end_y > game->window_height)
			end_y = game->window_height;
		// Draw the vertical line for the current ray
		if (x >= 0 && x < (int)img->width)
			draw_line(img, x, start_y, x, end_y, 0xFFFFFF);
	}
}

void	draw_grid(mlx_image_t *img, t_game *game)
{
	int x;
	int y;
	int line_length;
	int end_x;
	int end_y;

	y = 0;
	while (game->map.grid[y])
	{
		x = 0;
		while (game->map.grid[y][x])
		{
			if (game->map.grid[y][x] == '1')
			{
				draw_rectangle(img, x * SQUARE_SIZE, y * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, 0xFFFFFFFF);
			}
			else if (game->map.grid[y][x] == '0')
			{
				draw_rectangle(img, x * SQUARE_SIZE, y * SQUARE_SIZE,
					SQUARE_SIZE, SQUARE_SIZE, 0x000000FF);
			}
			x++;
		}
		y++;
	}
	draw_rectangle(img, game->posx, game->posy, 16, 16, 0xFF0000FF);

	double temp_angle = game->angle - PI / 6;

	while (temp_angle <= game->angle + PI / 6)
	{
		line_length = 0;
		end_x = game->posx + 16 / 2 + line_length * cos(temp_angle);
		end_y = game->posy + 16 / 2 + line_length * sin(temp_angle);
		while (game->map.grid[(end_y) / SQUARE_SIZE][(end_x)
			/ SQUARE_SIZE] == '0')
		{
			// printf("line_length:%d\n", line_length);
			line_length++;
			end_x = game->posx + 16 / 2 + line_length * cos(temp_angle);
			end_y = game->posy + 16 / 2 + line_length * sin(temp_angle);
		}
		draw_line(img, game->posx + 16 / 2, game->posy + 16 / 2, end_x, end_y,
			0x00F0F0FF);
		// draw_line(img, game->posx + 16 / 2, game->posy + 16 / 2, end_x, end_y,
		// 	0x00F0F0FF);

		temp_angle = temp_angle + PI / 256;
	}
	draw_3d_view(img, game);

	// printf("end_x:%d\n", end_x);
	// printf("end_y:%d\n", end_y);
	// printf("game->map.grid[end_y][end_x]:%c\n", game->map.grid[end_y
	// / SQUARE_SIZE][end_x / SQUARE_SIZE]);

	// CALISAN KOD
	// while (game->map.grid[(end_y) / SQUARE_SIZE][(end_x)
	// / SQUARE_SIZE] == '0')
	// {
	// 	// printf("line_length:%d\n", line_length);
	// 	line_length++;
	// 	end_x = game->posx + 16 / 2 + line_length * cos(game->angle);
	// 	end_y = game->posy + 16 / 2 + line_length * sin(game->angle);
	// }
	// draw_line(img, game->posx + 16 / 2, game->posy + 16 / 2, end_x, end_y,
	// 	0x00F0F0FF);
	// double temp_angle = game->angle - PI / 6;
	// line_length = 0;
	// end_x = game->posx + 16 / 2 + line_length * cos(temp_angle);
	// end_y = game->posy + 16 / 2 + line_length * sin(temp_angle);
	// while (game->map.grid[(end_y) / SQUARE_SIZE][(end_x)
	// / SQUARE_SIZE] == '0')
	// {
	// 	// printf("line_length:%d\n", line_length);
	// 	line_length++;
	// 	end_x = game->posx + 16 / 2 + line_length * cos(temp_angle);
	// 	end_y = game->posy + 16 / 2 + line_length * sin(temp_angle);
	// }
	// draw_line(img, game->posx + 16 / 2, game->posy + 16 / 2, end_x, end_y,
	// 	0x00F0F0FF);
}

int	is_wall(t_game *game, double x, double y)
{
	int	grid_x;
	int	grid_y;

	grid_x = (int)(x / SQUARE_SIZE);
	grid_y = (int)(y / SQUARE_SIZE);
	if (grid_x < 0 || grid_x >= game->grid_width || grid_y < 0 || grid_y >= game->grid_height)
		return (1);
	return (game->map.grid[grid_y][grid_x] == '1');
}