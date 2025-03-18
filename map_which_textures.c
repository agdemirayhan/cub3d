#include "cub3d.h"

static int	check_north_texture(t_data *data, char *path, int *sign)
{
	*sign = 1;
	return (north_texture(data, path));
}

static int	check_south_texture(t_data *data, char *path, int *sign)
{
	*sign = 0;
	return (south_texture(data, path));
}

static int	check_east_texture(t_data *data, char *path, int *sign)
{
	*sign = 2;
	return (east_texture(data, path));
}

static int	check_west_texture(t_data *data, char *path, int *sign)
{
	*sign = 3;
	return (west_texture(data, path));
}

int	which_texture(t_data *data, char *tmp, char *path, int *sign)
{
	if (tmp[0] == 'N' && tmp[1] == 'O')
		return (check_north_texture(data, path, sign));
	else if (tmp[0] == 'S' && tmp[1] == 'O')
		return (check_south_texture(data, path, sign));
	else if (tmp[0] == 'E' && tmp[1] == 'A')
		return (check_east_texture(data, path, sign));
	else if (tmp[0] == 'W' && tmp[1] == 'E')
		return (check_west_texture(data, path, sign));
	else
		return (0);
}
