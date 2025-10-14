/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 23:19:24 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/26 23:20:55 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_lessopen(t_msl *msl)
{
	t_env	*l_open;
	char	*l_open_val;

	if (!msl)
		return ;
	l_open = search_id_node(msl, "LESSOPEN");
	if (l_open == NULL)
	{
		l_open_val = ft_strdup("| /usr/bin/lesspipe %s");
		l_open = list_new_ownenv(ft_strdup("LESSOPEN"), l_open_val);
		if (!l_open)
		{
			free(l_open_val);
			return ;
		}
		list_addback_env(l_open, &(msl->own_env));
	}
}

void	set_lessclose(t_msl *msl)
{
	t_env	*l_close;
	char	*l_close_val;

	if (!msl)
		return ;
	l_close = search_id_node(msl, "LESSCLOSE");
	if (l_close == NULL)
	{
		l_close_val = ft_strdup("/usr/bin/lesspipe %s %s");
		l_close = list_new_ownenv(ft_strdup("LESSCLOSE"), l_close_val);
		if (!l_close)
		{
			free(l_close_val);
			return ;
		}
		list_addback_env(l_close, &(msl->own_env));
	}
}

void	set_last_arg_init(t_msl *msl)
{
	t_env	*last;
	t_env	*path;

	path = search_id_node(msl, "PATH");
	if (!path)
		return ;
	last = search_id_node(msl, "_");
	if (last)
	{
		free(last->value);
		last->value = ft_strdup(path->value);
	}
	else
	{
		last = list_new_ownenv(ft_strdup("_"), ft_strdup(path->value));
		if (!last)
			return ;
		list_addback_env(last, &(msl->own_env));
	}
}
