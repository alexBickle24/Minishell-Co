/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builts_utils_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 19:18:37 by vicalons          #+#    #+#             */
/*   Updated: 2025/10/14 03:12:29 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_check_id(t_msl *msl, char *id, char *value)
{
	t_env	*tmp_env;

	tmp_env = msl->own_env;
	while (tmp_env)
	{
		if (!ft_strncmp(tmp_env->id, id, ft_strlen(id) + 1))
		{
			ft_freeptr(tmp_env->id);
			tmp_env->id = ft_strdup(id);
			ft_freeptr(tmp_env->value);
			tmp_env->value = ft_strdup(value);
			return (1);
		}
		tmp_env = tmp_env->next;
	}
	return (0);
}

int	ft_check_isalnum_id(char *cmd, char *id)
{
	int	i;

	i = 0;
	while (id[i])
	{
		if (ft_isalnum(id[i]) || id[i] == '_')
			i++;
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (0);
		}
	}
	return (1);
}

int	ft_check_export(char *cmd)
{
	char	*id;
	int		i;

	i = 0;
	id = ft_get_env_id(cmd);
	if (!ft_isalpha(id[i]) && id[i] != '_')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		ft_freeptr(id);
		return (1);
	}
	if (ft_check_isalnum_id(cmd, id) == 0)
	{
		ft_freeptr(id);
		return (1);
	}
	ft_freeptr(id);
	return (0);
}
