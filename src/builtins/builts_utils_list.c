/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builts_utils_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicalons <vicalons@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 19:18:31 by vicalons          #+#    #+#             */
/*   Updated: 2025/10/12 19:18:32 by vicalons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*ft_lstnew_env(char *id, char *value, int alloc)
{
	t_env	*new;

	new = ft_calloc(1, sizeof(t_env));
	new->id = id;
	new->value = value;
	if (alloc)
	{
		new->id = ft_strdup(id);
		new->value = ft_strdup(value);
		if (!new->id || !new->value)
		{
			ft_freeptr(new->id);
			ft_freeptr(new->value);
			ft_freeptr(new);
			return (NULL);
		}
	}
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back_env(t_env **msl_env, t_env *new_env)
{
	t_env	*tmp_env;

	if (!*msl_env)
	{
		*msl_env = new_env;
		return ;
	}
	tmp_env = *msl_env;
	while (tmp_env->next)
		tmp_env = tmp_env->next;
	tmp_env->next = new_env;
}
