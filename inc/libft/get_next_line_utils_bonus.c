/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:22:43 by alex              #+#    #+#             */
/*   Updated: 2024/12/11 04:24:00 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	**create_new_branch(void **table)
{
	ssize_t	i;
	ssize_t	line;

	i = -1;
	line = 0;
	if (!table)
	{
		table = (void **)malloc(sizeof(void *) * 5);
		if (!table)
			return (NULL);
		while (++i < 5)
			table[i] = NULL;
		while (line <= 2)
		{
			table[line] = (void *)malloc(sizeof(ssize_t) * 2);
			if (!table[line])
				return (NULL);
			((ssize_t *)table[line])[0] = 0;
			((ssize_t *)table[line])[1] = 0;
			line++;
		}
	}
	return (table);
}

void	**ft_redim_fill_table(void **table, ssize_t new_lines, ssize_t lines)
{
	void	**aux_fill_table;
	int		i;

	if (lines >= new_lines)
		return (table);
	new_lines = new_lines + 1;
	aux_fill_table = (void **)malloc(sizeof(void *) * (new_lines + 4));
	if (!aux_fill_table)
		return (NULL);
	aux_fill_table[new_lines + 4 - 1] = NULL;
	table[0] = ft_realloc_fill_ssline(table, new_lines, lines, 0);
	if (!table[0])
		return (free(aux_fill_table), NULL);
	((ssize_t *)table[2])[0] = new_lines - 1;
	i = -1;
	while (++i <= (lines + 4))
	{
		aux_fill_table[i] = table[i];
	}
	free(table);
	table = aux_fill_table;
	return (table);
}

void	*ft_realloc_fill_ssline(void **table, ssize_t new_columns,
		ssize_t columns, ssize_t target)
{
	void	*aux_fill_size_l;
	ssize_t	i;

	i = 0;
	aux_fill_size_l = (void *)malloc(sizeof(ssize_t) * (new_columns + 1));
	if (!aux_fill_size_l)
		return (NULL);
	((ssize_t *)aux_fill_size_l)[new_columns] = 0;
	while (i <= (columns + 1))
	{
		((ssize_t *)aux_fill_size_l)[i] = ((ssize_t *)table[0])[i];
		i++;
	}
	free(table[target]);
	table[target] = aux_fill_size_l;
	return (table[target]);
}

void	**free_all(void **table, char *buffer)
{
	int	i;

	i = 0;
	if (table)
	{
		while (table[i] != NULL)
		{
			free(table[i]);
			i++;
		}
	}
	if (buffer)
		free(buffer);
	return (free(table), NULL);
}
