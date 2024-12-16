/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 21:59:34 by aagdemir          #+#    #+#             */
/*   Updated: 2024/12/14 16:52:32 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

struct					s_node
{
	char				*string;
	struct s_node		*next;
};
typedef struct s_node	t_nod;

char					*get_next_line(int fd);
void					delete_list(t_nod **list, t_nod *newline_node,
							char *buffer, int j);
int						characters_to_newline(t_nod *list);
t_nod					*find_last_node(t_nod *list);
int						newline_check(t_nod *list);
void					copy_str(t_nod *list, char *str);

#endif