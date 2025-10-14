/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_ps1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:08:20 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:30:04 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*set_ps1_hostuser(t_system *sys)
{
	char	*tmp;
	char	*host_user;

	if (!sys || !sys->host || !sys->user)
		return (NULL);
	tmp = ft_strjoin(sys->user, "@");
	if (!tmp)
		return (NULL);
	host_user = ft_strjoin(tmp, sys->host);
	free(tmp);
	if (!host_user)
		return (NULL);
	tmp = ft_strjoin(host_user, ":");
	free(host_user);
	return (tmp);
}

char	*set_ps1_path(t_msl *msl)
{
	char	*cwd;
	char	*display_path;

	cwd = msl->sys->pwd;
	if (!cwd)
		return (NULL);
	display_path = get_display_path(msl, cwd);
	return (display_path);
}

char	*get_display_path(t_msl *msl, char *cwd)
{
	t_env	*home_node;
	char	*find;
	char	*display_path;

	home_node = search_id_node(msl, "HOME");
	if (home_node)
	{
		find = ft_strnstr(cwd, home_node->value, ft_strlen(home_node->value));
		if (find)
		{
			display_path = ft_strjoin("~", find + ft_strlen(home_node->value));
			if (!display_path)
				return (free(cwd), NULL);
		}
		else
			display_path = ft_strdup(cwd);
	}
	else
		display_path = ft_strdup(cwd);
	return (display_path);
}

void	set_ps1(t_msl *msl, t_system *sys)
{
	char	*tmp;
	char	*dollar;

	if (msl->sys->ps1_path)
		free(msl->sys->ps1_path);
	if (sys->ps1)
		free(sys->ps1);
	msl->sys->ps1_path = set_ps1_path(msl);
	if (!sys->ps1_hostuser || !sys->ps1_path)
		tmp = ft_strdup("minishell");
	else
		tmp = ft_strjoin(sys->ps1_hostuser, sys->ps1_path);
	dollar = ft_strjoin(tmp, "$ ");
	free(tmp);
	if (dollar)
		sys->ps1 = dollar;
	else
		sys->ps1 = ft_strdup("minishell$ ");
}
