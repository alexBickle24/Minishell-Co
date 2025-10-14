/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:09:19 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/27 15:48:07 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env_table_to_list(t_msl *msl, char **env_tb)
{
	t_env	*tmp;
	t_env	*last_node;

	if (!msl || !env_tb)
		return ;
	last_node = msl->own_env;
	if (msl->own_env == NULL)
	{
		while (*env_tb)
		{
			tmp = search_creat_envnode(msl, env_tb);
			if (!tmp)
				return ;
			if (!(msl->own_env))
				msl->own_env = tmp;
			else
				last_node->next = tmp;
			last_node = tmp;
			env_tb++;
		}
	}
}

t_env	*search_creat_envnode(t_msl *msl, char **env_tb)
{
	char	*id;
	char	*value;
	t_env	*tmp;

	id = ft_get_env_id(*env_tb);
	value = ft_get_env_value(env_tb, id);
	if (!value)
	{
		free(id);
		free(value);
		free_own_env(msl);
		return (NULL);
	}
	tmp = list_new_ownenv(id, value);
	if (!tmp)
	{
		free(id);
		free(value);
		free_own_env(msl);
		return (NULL);
	}
	return (tmp);
}

void	set_shlvl(t_msl *msl)
{
	t_env	*shlvl_node;
	int		shlvl_num;

	if (!msl)
		return ;
	shlvl_node = search_id_node(msl, "SHLVL");
	if (shlvl_node == NULL)
	{
		shlvl_node = list_new_ownenv(ft_strdup("SHLVL"), ft_itoa(1));
		list_addback_env(shlvl_node, &(msl->own_env));
		return ;
	}
	shlvl_num = ft_atoi(shlvl_node->value);
	free(shlvl_node->value);
	if (shlvl_num >= 999)
	{
		ft_shlvl_warning(++shlvl_num);
		shlvl_node->value = ft_strdup("1");
	}
	else if (shlvl_num < 0)
		shlvl_node->value = ft_strdup("0");
	else
		shlvl_node->value = ft_itoa(++shlvl_num);
}

void	check_home_message(t_msl *msl)
{
	t_env	*node;

	if (msl->mode == 1)
		return ;
	node = search_id_node(msl, "HOME");
	if (node == NULL)
	{
		ft_error_home();
		return ;
	}
	if (ft_strncmp(node->value, msl->sys->home, ft_strlen(node->value) + 1))
	{
		ft_error_home();
		return ;
	}
}

void	set_pwd(t_msl *msl)
{
	t_env	*env_node;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		pwd = ft_strdup(".");
	env_node = search_id_node(msl, "PWD");
	if (env_node)
	{
		if (env_node->value)
		{
			free(env_node->value);
			env_node->value = pwd;
		}
	}
	else
	{
		env_node = list_new_ownenv(ft_strdup("PWD"), pwd);
		if (!env_node)
		{
			free(pwd);
			return ;
		}
		list_addback_env(env_node, &(msl->own_env));
	}
}
