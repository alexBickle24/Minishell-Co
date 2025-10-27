/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 22:50:00 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 18:07:35 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Retrieves the last argument of the first command in the token list.
 * 
 * This function iterates through the `pcmds` list of the first token to find
 * the last argument of the command. It duplicates the last argument string
 * and returns it.
 * 
 * - If there is only one token in the shell (`msl->total_tockens == 1`), it
 *   processes the `pcmds` list of the first token.
 * - Iterates through the `pcmds` list to find the last node.
 * - Duplicates the `cmd` field of the last node and returns it.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @return A dynamically allocated string containing the last argument, or `NULL`
 *         if no arguments are found.
 */
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

/**
 * @brief Updates or creates the `_` environment variable with the last argument.
 * 
 * This function sets the `_` environment variable to the value of the last
 * argument of the first command. If the `_` variable does not exist, it is
 * created. If it exists, its value is updated.
 * 
 * - Searches for the `_` variable in the environment list using `search_id_node`.
 * - If the variable is not found, it creates a new environment node with `_`
 *   as the key and the last argument as the value.
 * - If the variable exists, it updates its value to the last argument.
 * - Frees the previous value of the `_` variable to avoid memory leaks.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param target The last argument string to set as the value of `_`.
 */
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
