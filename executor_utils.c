/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 22:50:00 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:05:02 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*set_last_arg(t_msl *msl)
{
	t_pcmds	*current;
	char	*target;

	target = NULL;
	if (msl && msl->total_tockens == 1)
	{
		current = msl->tocken->pcmds;
		while (current)
		{
			if (current->next == NULL)
			{
				target = ft_strdup(current->cmd);
				break ;
			}
			current = current->next;
		}
	}
	return (target);
}

void	create_last_arg(t_msl *msl, char *target)
{
	t_env	*node;

	if (target != NULL)
	{
		node = search_id_node(msl, "_");
		if (!node)
		{
			node = list_new_ownenv(ft_strdup("_"), target);
			list_addback_env(node, &(msl->own_env));
		}
		else
		{
			free(node->value);
			node->value = target;
		}
	}
}
