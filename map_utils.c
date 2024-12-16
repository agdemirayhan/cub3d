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

char	**parse_grid(int fd, char *line)
{
	char **grid;
	int i;

	i = 0;
	grid = malloc(sizeof(char *) * 1024);
	while (line)
	{
		if (line[0] == ' ' || line[0] == '1')
		{
			grid[i++] = ft_strdup(line);
		}
		free(line);
		line = get_next_line(fd);
	}
	grid[i] = NULL;
	return grid;
}

t_map	*parse_map(const char *filename)
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
			parse_color(&line[2], &map->ceiling_r, &map->ceiling_g, &map->ceiling_b);
		else if (ft_strncmp(line, "NO", 2) == 0 || ft_strncmp(line, "SO", 2) == 0 ||
				ft_strncmp(line, "WE", 2) == 0 || ft_strncmp(line, "EA", 2) == 0)
			parse_textures(line, map);
		else if (line[0] == ' ' || line[0] == '1')
		{
			map->grid = parse_grid(fd, line);
			break;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (map);
}

void draw_rectangle(mlx_image_t *img, int x, int y, int width, int height, uint32_t color)
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
			if (px >= 0 && px < (int)img->width && py >= 0 && py < (int)img->height)
			{
				mlx_put_pixel(img, px, py, color);
			}
			j++;
		}
		i++;
	}
}

void draw_grid(mlx_image_t *img, t_map *map)
{
	int	square_size;
	int	x;
	int	y;

	square_size = 20;
	y = 0;
	while (map->grid[y])
	{
		x = 0;
		while (map->grid[y][x])
		{
			if (map->grid[y][x] == '1')
			{
				draw_rectangle(img, x * square_size, y * square_size, square_size, square_size, 0xFFFFFF);
			}
			else if (map->grid[y][x] == '0')
			{
				draw_rectangle(img, x * square_size, y * square_size, square_size, square_size, 0x000000);
			}
			x++;
		}
		y++;
	}
}





