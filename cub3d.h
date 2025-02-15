#ifndef CUB3D_H
# define CUB3D_H

# include "./get_next_line/get_next_line.h"
# include "./libft/libft.h"
# include <MLX42/MLX42.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# define PI 3.1415926535
# define DR (M_PI / 180)   // Convert degrees to radians
# define P2 (M_PI / 2)     // 90 degrees
# define P3 (3 * M_PI / 2) // 270 degrees

# ifndef SQUARE_SIZE
#  define SQUARE_SIZE 64
# endif

typedef struct s_map
{
	char			*texture_no;
	char			*texture_so;
	char			*texture_we;
	char			*texture_ea;
	int				floor_r;
	int				floor_g;
	int				floor_b;
	int				ceiling_r;
	int				ceiling_g;
	int				ceiling_b;
	char			**grid;
}					t_map;

typedef struct s_game
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_map			map;
	int				grid_width;
	int				grid_height;
	int				window_width;
	int				window_height;
	int				posx;
	int				posy;
	double			angle;
	mlx_key_data_t	last_key_data;
}					t_game;

void				draw_grid(mlx_image_t *img, t_game *game);
t_map				*parse_map(const char *filename, t_game *game);
char				**parse_grid(int fd, char *line, t_game *game);
void				parse_textures(char *line, t_map *map);
int					parse_color(char *line, int *r, int *g, int *b);
void				free_map(t_map *map);
void				calculate_grid_size(t_map *map, int *grid_width,
						int *grid_height);
void				keyhook(mlx_key_data_t keydata, void *param);
void				put_image_in_map(t_game *game);
void	draw_3d_view(mlx_image_t *img, t_game *game);

#endif