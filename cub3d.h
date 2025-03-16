#ifndef CUB3D_H
# define CUB3D_H

# include "./get_next_line/get_next_line.h"
# include "./libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include <mlx.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# define PI 3.1415926535
# define DR (M_PI / 180)
# define P2 (M_PI / 2)
# define P3 (3 * M_PI / 2)
# define MOVE_SPEED 0.1
# define ROTSPEED 0.05
# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080
# define X 0
# define Y 1

# ifndef SQUARE_SIZE
#  define SQUARE_SIZE 64
# endif

# ifndef QUEUE_SIZE
#  define QUEUE_SIZE 10000
# endif

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_mlx
{
	void			*mlx_ptr;
	void			*win_ptr;
}					t_mlx;

typedef struct s_vec
{
	double			x;
	double			y;
}					t_vec;

typedef struct s_mapstate
{
	unsigned int	map_width;
	unsigned int	map_height;
	int				keycode_fb;
	int				keycode_r;
	int				keycode_lr;
	double			rot;
	int				prev_keycode_fb;
	int				prev_keycode_r;
	int				prev_keycode_lr;
	int				turn_direction;
	int				angle;
}					t_mapstate;

// typedef struct s_mapstate
// {
// 	unsigned int	map_width;
// 	unsigned int	map_height;
// 	int				keycode_fb;
// 	int				keycode_r;
// 	int				keycode_lr;
// 	int				turn_direction;
// 	int				angle;
// }					t_mapstate;

typedef struct s_data
{
	t_mlx			mlx;
	t_vec			pos;
	t_vec			dir;
	t_vec			plane;
	void			*img;
	int				*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	t_mapstate		mapstate;
	int				**map_int;
	unsigned int	ceil_color;
	unsigned int	floor_color;
	int				tex_w;
	int				tex_h;
	int				tex_w1;
	int				tex_h1;
	int				tex_w2;
	int				tex_h2;
	int				tex_w3;
	int				tex_h3;
	int				tex_w4;
	int				tex_h4;
	int				cnv_bpp1;
	int				cnv_ll1;
	int				cnv_en1;
	void			*cnv_img2;
	int				*cnv_addr2;
	int				cnv_bpp2;
	int				cnv_ll2;
	int				cnv_en2;
	void			*cnv_img3;
	int				*cnv_addr3;
	int				cnv_bpp3;
	int				cnv_ll3;
	int				cnv_en3;
	void			*cnv_img4;
	int				*cnv_addr4;
	int				cnv_bpp4;
	int				cnv_ll4;
	int				cnv_en4;
	int				texx;
	int				texy;
	void			*cnv_img1;
	int				*cnv_addr1;
}					t_data;

typedef struct s_game_ray
{
	int				x;
	double			screen;
	double			pos_x;
	double			pos_y;
	double			ray_x;
	double			ray_y;
	int				step_x;
	int				step_y;
	double			box_dist_x;
	double			box_dist_y;
	double			delta_x;
	double			delta_y;
	double			wall_pos;
	int				side;
	double			wall_dist;
	char			**map;
}					t_game_ray;

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

typedef struct s_dda
{
	int				map_x;
	int				map_y;
	t_vec			side_dist;
	t_vec			delta_dist;
	int				step_x;
	int				step_y;
	int				side;
	double			perp_wall_dist;
	int				hit;
	int				line_height;
	int				draw_start;
	int				draw_end;

}					t_dda;

typedef struct s_raycast
{
	double			camera_x;
	t_vec			ray;
}					t_raycast;

typedef struct s_rgb
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;
}	t_rgb;

typedef struct s_game
{
	// mlx_t			*mlx;
	// mlx_image_t		*img;
	t_map			map;
	// mlx_image_t		*ray;
	int				map_h;
	int				map_l;
	char			**map_comp;
	int				grid_width;
	int				grid_height;
	int				window_width;
	int				window_height;
	t_rgb			rgb;
	int				posx;
	int				posy;
	double			angle;
	unsigned int	ceil_color;
	unsigned int	floor_color;
	unsigned int	map_h_tmp;
	// mlx_key_data_t	last_key_data;

	bool			is_moving_up;
	bool			is_moving_down;
	bool			is_moving_left;
	bool			is_moving_right;
	bool			turning_left;
	bool			turning_right;
}					t_game;

// void				draw_grid(mlx_image_t *img, t_game *game);
//void				*parsing(char *argv, t_data *data, t_game *game);
int					parsing(char *argv, t_data *data, t_game *game);
int					init_window_and_map(t_data *data, t_game *game);
char				**parse_grid(int fd, char *line, t_game *game);
void				parse_textures(char *line, t_map *map);
int					parse_color(char *line, int *r, int *g, int *b);
void				free_map(t_map *map);
void				calculate_grid_size(t_map *map, int *grid_width,
						int *grid_height);
// void				keyhook(mlx_key_data_t keydata, void *param);
void				put_image_in_map(t_game *game);
// void				draw_3d_view(mlx_image_t *img, t_game *game);
void				move_up(t_game *game);
void				move_down(t_game *game);
void				move_left(t_game *game);
void				move_right(t_game *game);
void	flood_fill(char **map_copy, int y, int x, int *is_valid, t_game *game);
int	validate_map_boundaries(t_game *game);
void	expand_map_lines(t_game *game);
int	is_position_valid(t_game *game, int y, int x);
int	validate_map_structure(t_game *game, t_data *data);
void	my_fill_heroe_position_1(t_data *data, char *direction);
void	my_fill_heroe_position(t_game *game, int y, int x, t_data *data);

#endif