#include "cub3d.h"

char	*texture_path(char *tmp)
{
	char	*line;
	int		len;

	line = tmp;
	while (*line != '.')
		line++;
	len = ft_strlen(line);
	line[len - 1] = '\0';
	printf("line: %s\n", line);
	return (line);
}

int	north_texture(t_data *data, char *path)
{
	data->cnv_img2 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
			path, &data->tex_w2, &data->tex_h2);
	if (!data->cnv_img2)
		return (0);
	data->cnv_addr2 = (int *)mlx_get_data_addr(data->cnv_img2,
			&data->cnv_bpp2, &data->cnv_ll2, &data->cnv_en2);
	return (1);
}

int	south_texture(t_data *data, char *path)
{
	data->cnv_img1 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
			path, &data->tex_w1, &data->tex_h1);
	if (!data->cnv_img1)
		return (0);
	data->cnv_addr1 = (int *)mlx_get_data_addr(data->cnv_img1,
			&data->cnv_bpp1, &data->cnv_ll1, &data->cnv_en1);
	return (1);
}

int	east_texture(t_data *data, char *path)
{
	data->cnv_img3 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
			path, &data->tex_w3, &data->tex_h3);
	if (!data->cnv_img3)
		return (0);
	data->cnv_addr3 = (int *)mlx_get_data_addr(data->cnv_img3,
			&data->cnv_bpp3, &data->cnv_ll3, &data->cnv_en3);
	return (1);
}

int	west_texture(t_data *data, char *path)
{
	data->cnv_img4 = mlx_xpm_file_to_image(data->mlx.mlx_ptr,
			path, &data->tex_w4, &data->tex_h4);
	if (!data->cnv_img4)
		return (0);
	data->cnv_addr4 = (int *)mlx_get_data_addr(data->cnv_img4,
			&data->cnv_bpp4, &data->cnv_ll4, &data->cnv_en4);
	return (1);
}
