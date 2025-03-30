/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 16:24:06 by msoklova          #+#    #+#             */
/*   Updated: 2025/03/30 16:26:20 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

char	*create_expanded_line(char *original, int current_len, int new_len)
{
	int		j;
	char	*expanded;

	expanded = malloc(new_len + 1);
	if (!expanded)
		return (NULL);
	j = 0;
	while (j < current_len)
	{
		expanded[j] = original[j];
		j++;
	}
	while (j < new_len)
		expanded[j++] = ' ';
	expanded[new_len] = '\0';
	return (expanded);
}

void	expand_map_lines(t_game *game)
{
	int		i;
	int		current_len;
	char	*expanded;

	i = 0;
	while (i < game->map_h)
	{
		current_len = ft_strlen(game->map_comp[i]);
		if (current_len >= game->map_l)
		{
			i++;
			continue ;
		}
		expanded = create_expanded_line(game->map_comp[i],
				current_len, game->map_l);
		if (!expanded)
			return ;
		free(game->map_comp[i]);
		game->map_comp[i] = expanded;
		i++;
	}
}
