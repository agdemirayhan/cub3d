#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include <fcntl.h>
# include "./get_next_line/get_next_line.h"
# include "./libft/libft.h"
# include <MLX42/MLX42.h>

# ifndef SQUARE_SIZE
#  define SQUARE_SIZE 42
# endif

typedef struct	s_map
{
	char	*texture_no;
	char	*texture_so;
	char	*texture_we;
	char	*texture_ea;
	int		floor_r;
	int		floor_g;
	int		floor_b;
	int		ceiling_r;
	int		ceiling_g;
	int		ceiling_b;
	char	**grid;
}			t_map;


void	draw_grid(mlx_image_t *img, t_map *map);
t_map	*parse_map(const char *filename);
char	**parse_grid(int fd, char *line);
void	parse_textures(char *line, t_map *map);
int		parse_color(char *line, int *r, int *g, int *b);
void	free_map(t_map *map);
void	calculate_grid_size(t_map *map, int *grid_width, int *grid_height);

#endif