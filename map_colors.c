#include "cub3d.h"

int	extract_number(char **line)
{
	int value = 0;

	while (**line == ' ')
		(*line)++;
	if (!ft_isdigit(**line))
		return -1;
	while (ft_isdigit(**line))
	{
		value = value * 10 + (**line - '0');
		(*line)++;
		if (value > 255)
			return -1;
	}
	while (**line == ' ')
	(*line)++;
	return value;
}

int	rgb_values(char *line_ptr, t_rgb *rgb, char *tmp)
{
	rgb->r = extract_number(&line_ptr);
	if (rgb->r < 0 || *line_ptr != ',')
	{
		free(tmp);
		return (printf("Error: Invalid red color value\n"), 1);
	}
	line_ptr++;
	while (*line_ptr == ' ')
		line_ptr++;
	rgb->g = extract_number(&line_ptr);
	if (rgb->g < 0 || *line_ptr != ',')
	{
		free(tmp);
		return (printf("Error: Invalid green color value\n"), 1);
	}
	line_ptr++;
	while (*line_ptr == ' ')
		line_ptr++;
	rgb->b = extract_number(&line_ptr);
	if (rgb->b < 0 || (*line_ptr != '\0' && *line_ptr != '\n'))
	{
		free(tmp);
		return (printf("Error: Invalid blue color value\n"), 1);
	}
	return (0);
}

int	rgb_color(char *tmp, t_game *game)
{
	char	identifier;
	char	*line_ptr;
	t_rgb	rgb;

	identifier = tmp[0];
	line_ptr = tmp + 2;
	while (*line_ptr == ' ')
		line_ptr++;
	if (rgb_values(line_ptr, &rgb, tmp))
		return (1);
	if (identifier == 'C')
		game->ceil_color = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
	else if (identifier == 'F')
		game->floor_color = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
	printf("Parsed %c color: R=%d, G=%d, B=%d\n", identifier, rgb.r, rgb.g, rgb.b);
	return (0);
}