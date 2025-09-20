/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_formats.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 18:47:10 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/20 19:22:24 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**ft_env_to_table(t_env *env)
{
	t_env	*current;
	int		lines;
	char	*tmp;
	char	**table;

	table = create_table((void *)env, 2);
	if (table == NULL)
		return (NULL);
	current = env;
	lines = 0;
	while (current)
	{
		tmp = ft_strjoin(current->id, "=");
		if (!tmp)
			return (ft_free_table(table), NULL);
		table[lines] = ft_strjoin(tmp, current->value);
		if (!table[lines])
			return (NULL);
		free(tmp);
		current = current->next;
		lines++;
	}
	table[lines] = NULL;
	return (table);
}

char	**ft_pcmds_to_table(t_pcmds *pcmds)
{
	t_pcmds	*current;
	int		lines;
	char	**table;

	table = create_table((void *)pcmds, 1);
	if (!table)
		return (NULL);
	current = pcmds;
	lines = 0;
	while (current)
	{
		table[lines] = ft_strdup(current->cmd);
		if (!table[lines])
			return (ft_free_table(table), NULL);
		current = current->next;
		lines++;
	}
	table[lines] = NULL;
	return (table);
}

char	**create_table(void *list, int type)
{
	t_pcmds	*current;
	t_env	*current1;
	int		lines;
	char	**table;

	if (list == NULL)
		return (NULL);
	if (type == 1)
		current = (t_pcmds *)list;
	else if (type == 2)
		current1 = (t_env *)list;
	lines = 0;
	while (current)
	{
		current = current->next;
		lines++;
	}
	table = (char **)malloc(sizeof(char *) * (lines + 1));
	if (!table)
		return (NULL);
	return (table);
}
