/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoklova <msoklova@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 20:09:37 by aagdemir          #+#    #+#             */
/*   Updated: 2024/12/14 16:52:07 by msoklova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	next_line_organizer(t_nod **list)
{
	char	*buffer;
	t_nod	*newline_node;
	t_nod	*last_node;
	int		i;
	int		j;

	buffer = malloc(BUFFER_SIZE + 1);
	newline_node = malloc(sizeof(t_nod));
	if (buffer == NULL || newline_node == NULL)
		return (free(buffer));
	last_node = find_last_node(*list);
	i = 0;
	j = 0;
	while (last_node->string[i] && last_node->string[i] != '\n')
		i++;
	while (last_node->string[i] && last_node->string[i++])
		buffer[j++] = last_node->string[i];
	delete_list(list, newline_node, buffer, j);
	if (newline_node->string[0])
		*list = newline_node;
	else
	{
		free(newline_node);
		free(buffer);
	}
}

void	appendtolist(t_nod **list, char *buffer)
{
	t_nod	*new_node;
	t_nod	*last_node;

	new_node = malloc(sizeof(t_nod));
	if (new_node == NULL)
	{
		free(new_node);
		free(buffer);
		return ;
	}
	last_node = find_last_node(*list);
	if (last_node == NULL)
		*list = new_node;
	else
		last_node->next = new_node;
	new_node->string = buffer;
	new_node->next = NULL;
}

void	listhandler(int fd, t_nod **list)
{
	int		read_block;
	char	*buffer;

	buffer = NULL;
	while (42)
	{
		if (newline_check(*list))
			return ;
		else
		{
			buffer = malloc(BUFFER_SIZE + 1);
			if (buffer == NULL)
				return ;
			read_block = read(fd, buffer, BUFFER_SIZE);
			if (!read_block)
				return (free(buffer));
			buffer[read_block] = '\0';
			appendtolist(list, buffer);
		}
	}
}

char	*fill(t_nod *list)
{
	int		str_len;
	char	*next_str;

	if (list == NULL)
		return (NULL);
	str_len = characters_to_newline(list);
	next_str = malloc(str_len + 1);
	if (next_str == NULL)
		return (NULL);
	copy_str(list, next_str);
	if (next_str == NULL)
		return (NULL);
	return (next_str);
}

char	*get_next_line(int fd)
{
	static t_nod	*list;
	char			*textarr;
	t_nod			*temp;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &textarr, 0) < 0)
	{
		while (list)
		{
			temp = list;
			list = list->next;
			free(temp->string);
			free(temp);
		}
		return (NULL);
	}
	listhandler(fd, &list);
	if (list == NULL)
		return (NULL);
	textarr = fill(list);
	if (textarr == NULL)
		return (NULL);
	next_line_organizer(&list);
	return (textarr);
}

// cc -g -Wall -Werror -Wextra *.c *.h && ./a.out | cat -e

// #include <time.h>

// int	main(void)
// {
// 	int fd;
// 	char *line;
// 	int tic;
// 	int toc;

// 	fd = open("1char.txt", O_RDONLY);
// 	tic = clock();
// 	line = get_next_line(fd);
// 	while (line != NULL)
// 	{
// 		printf("%s", line);
// 		free(line);
// 		line = get_next_line(fd);
// 	}
// 	toc = clock();
// 	printf("Elapsed time: %f\n", (double)(toc - tic) / CLOCKS_PER_SEC);
// 	close(fd);
// 	return (0);
// }