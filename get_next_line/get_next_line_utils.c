/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 20:10:05 by aagdemir          #+#    #+#             */
/*   Updated: 2024/12/14 16:52:22 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	delete_list(t_nod **list, t_nod *newline_node, char *buffer, int j)
{
	t_nod	*tmp;

	buffer[j] = '\0';
	newline_node->string = buffer;
	newline_node->next = NULL;
	if (*list == NULL)
		return (free(list));
	while (*list)
	{
		tmp = (*list)->next;
		free((*list)->string);
		free(*list);
		*list = tmp;
	}
}

int	characters_to_newline(t_nod *list)
{
	int	i;
	int	len;

	if (list == NULL)
		return (0);
	len = 0;
	while (list)
	{
		i = 0;
		while (list->string[i])
		{
			if (list->string[i] == '\n')
			{
				len++;
				return (len);
			}
			i++;
			len++;
		}
		list = list->next;
	}
	return (len);
}

t_nod	*find_last_node(t_nod *list)
{
	if (list == NULL)
		return (NULL);
	while (list->next)
		list = list->next;
	return (list);
}

int	newline_check(t_nod *list)
{
	int	i;

	if (list == NULL)
		return (0);
	while (list)
	{
		i = 0;
		while (list->string[i])
		{
			if (list->string[i] == '\n')
				return (1);
			++i;
		}
		list = list->next;
	}
	return (0);
}

void	copy_str(t_nod *list, char *str)
{
	int	i;
	int	j;

	if (list == NULL)
		return ;
	j = 0;
	while (list)
	{
		i = 0;
		while (list->string[i])
		{
			if (list->string[i] == '\n')
			{
				str[j] = '\n';
				j++;
				str[j] = '\0';
				return ;
			}
			str[j++] = list->string[i++];
		}
		list = list->next;
	}
	str[j] = '\0';
}
