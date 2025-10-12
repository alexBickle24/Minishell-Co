/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicalons <vicalons@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 19:18:13 by vicalons          #+#    #+#             */
/*   Updated: 2025/10/12 19:21:15 by vicalons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*ft_sort_env(t_env *own_env)
{
	t_env	*tmp;
	t_env	*swap;

	tmp = own_env;
	swap = ft_lstnew_env("", "", 0);
	while (own_env->next != NULL)
	{
		if (own_env->next && ft_strcmp(own_env->id, own_env->next->id) > 0)
		{
			swap->id = own_env->id;
			swap->value = own_env->value;
			own_env->id = own_env->next->id;
			own_env->value = own_env->next->value;
			own_env->next->id = swap->id;
			own_env->next->value = swap->value;
			own_env = tmp;
		}
		else
			own_env = own_env->next;
	}
	own_env = tmp;
	ft_freeptr(swap);
	return (tmp);
}

void	ft_print_env(t_env *own_env)
{
	t_env	*tmp;

	tmp = own_env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->id, "_"))
			ft_putendl_fd("\r", 1);
		else if (tmp->value[0])
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(tmp->id, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putendl_fd("\"", 1);
		}
		else
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putendl_fd(tmp->id, 1);
		}
		tmp = tmp->next;
	}
}

char	*ft_get_one_env_value(char *env, char *id)
{
	int		i;
	int		j;
	char	*value;

	if (!env || !id)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		if (env[i] == '=')
			break ;
	}
	value = malloc(sizeof(char) * (ft_strlen(env) - i + 1));
	if (!value)
		return (NULL);
	j = 0;
	while (env[++i])
		value[j++] = env[i];
	value[j] = '\0';
	return (value);
}

void	ft_add_env(t_msl *msl, char *cmd)
{
	char	*id;
	char	*value;
	t_env	*new_env;

	if (!ft_strrchr(cmd, '='))
		return ;
	id = ft_get_env_id(cmd);
	value = ft_get_one_env_value(cmd, id);
	if (!ft_check_id(msl, id, value))
	{
		new_env = ft_lstnew_env(id, value, 1);
		ft_lstadd_back_env(&msl->own_env, new_env);
		ft_freeptr(id);
		ft_freeptr(value);
		return ;
	}
	ft_freeptr(id);
	ft_freeptr(value);
	return ;
}

void	ft_export(t_msl *msl, t_pcmds *pcmds)
{
	t_pcmds	*tmp;

	if (ft_argscounter(pcmds) == 1
		&& !ft_strcmp(pcmds->cmd, "export"))
	{
		ft_print_env(ft_sort_env(msl->own_env));
	}
	else
	{
		tmp = pcmds->next;
		while (tmp)
		{
			if (ft_check_export(tmp->cmd))
				ft_add_env(msl, tmp->cmd);
			else
				return ;
			tmp = tmp->next;
		}
	}
}
