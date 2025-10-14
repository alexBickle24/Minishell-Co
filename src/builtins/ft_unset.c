/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:31:56 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 17:59:47 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_unset(t_msl *msl, t_tocken *tocken)
{
	t_pcmds	*arg;
	t_env	*env_node;
	t_env	*prev_node;

	arg = tocken->pcmds->next;
	while (arg)
	{
		env_node = msl->own_env;
		prev_node = NULL;
		while (env_node)
		{
			if (!ft_strncmp(arg->cmd, env_node->id, ft_strlen(arg->cmd) + 1))
			{
				if (!ft_strncmp(arg->cmd, "OLDPWD\0", 7))
					msl->builts->unst_oldpwd = 1;
				remove_envnode(env_node, &(msl->own_env), prev_node);
				break ;
			}
			prev_node = env_node;
			env_node = env_node->next;
		}
		arg = arg->next;
	}
	return (0);
}

void	remove_envnode(t_env *node, t_env **begin, t_env *prev_node)
{
	if (!begin || !*begin)
		return ;
	if (*begin == node)
	{
		*begin = node->next;
		free_envnode(node);
	}
	else
	{
		prev_node->next = node->next;
		free_envnode(node);
	}
}

void	free_envnode(t_env *node)
{
	free(node->id);
	free(node->value);
	free(node);
}
