/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 19:18:13 by vicalons          #+#    #+#             */
/*   Updated: 2025/10/14 02:55:46 by alejandro        ###   ########.fr       */
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
		if (own_env->next && ft_strncmp(own_env->id, own_env->next->id,
				ft_strlen(own_env->id) + 1) > 0)
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
		if (!ft_strncmp(tmp->id, "_\0", 2))
			ft_putstr_fd("\r", 1);
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

int	ft_add_env(t_msl *msl, char *cmd)
{
	char	*id;
	char	*value;
	char	*trim_value;
	t_env	*new_env;

	if (!ft_strrchr(cmd, '='))
		return (2);
	id = ft_get_env_id(cmd);
	value = ft_get_one_env_value(cmd, id);
	if (!ft_check_id(msl, id, value))
	{
		trim_value = ft_strtrim(value, " \t\n\v\f\r");
		new_env = ft_lstnew_env(id, trim_value, 1);
		list_addback_env(new_env, &(msl->own_env));
		ft_freeptr(id);
		ft_freeptr(value);
		ft_freeptr(trim_value);
		return (0);
	}
	ft_freeptr(id);
	ft_freeptr(value);
	return (1);
}

int	ft_export(t_msl *msl, t_pcmds *pcmds)
{
	t_pcmds	*tmp;
	int		ret;

	ret = 0;
	if (ft_argscounter(pcmds) == 1
		&& !ft_strncmp(pcmds->cmd, "export\0", 8))
	{
		ft_print_env(ft_sort_env(msl->own_env));
	}
	else
	{
		tmp = pcmds->next;
		while (tmp)
		{
			ret = ft_check_export(tmp->cmd);
			if (ret == 0)
				ft_add_env(msl, tmp->cmd);
			tmp = tmp->next;
		}
	}
	return (ret);
}
