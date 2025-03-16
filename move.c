/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagdemir <aagdemir@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:38:04 by aagdemir          #+#    #+#             */
/*   Updated: 2025/03/16 18:38:07 by aagdemir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move_up(t_game *game)
{
	game->posx += cos(game->angle) * MOVE_SPEED;
	game->posy += sin(game->angle) * MOVE_SPEED;
}

void	move_down(t_game *game)
{
	game->posx -= cos(game->angle) * MOVE_SPEED;
	game->posy -= sin(game->angle) * MOVE_SPEED;
}

void	move_left(t_game *game)
{
	game->posx += cos(game->angle - P2) * MOVE_SPEED;
	game->posy += sin(game->angle - P2) * MOVE_SPEED;
}

void	move_right(t_game *game)
{
	game->posx += cos(game->angle + P2) * MOVE_SPEED;
	game->posy += sin(game->angle + P2) * MOVE_SPEED;
}
