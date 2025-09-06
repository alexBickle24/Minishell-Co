/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_ps1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 22:08:20 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/05 22:09:01 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*set_ps1_hostuser(t_system *sys)
{
	char	*tmp;
	char	*host_user;
	char	*final;

	if (!sys || !sys->host || !sys->user)
		return (NULL);
	tmp = ft_strjoin(sys->user, "@");
	if (!tmp)
		return (NULL);
	host_user = ft_strjoin(tmp, sys->host);
	free(tmp);
	if (!host_user)
		return (NULL);
	tmp = ft_strjoin(C_GREEN, host_user);
	free(host_user);
	if (!tmp)
		return (NULL);
	final = ft_strjoin(tmp, "\033[0m:");
	free(tmp);
	return (final);
}

char	*set_ps1_path(t_msl *msl)
{
	char	*cwd;
	char	*display_path;
	char	*color_path;
	t_env	*pwd_node;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	pwd_node = search_id_node(msl, "PWD");
	if (!pwd_node || !ft_strncmp(pwd_node->value, cwd, ft_strlen(cwd) + 1))
		display_path = get_display_path(msl, cwd);
	else
		display_path = ft_strdup(pwd_node->value);
	free(cwd);
	if (!display_path)
		return (NULL);
	color_path = ft_strjoin(C_BLUE, display_path);
	free(display_path);
	if (!color_path)
		return (NULL);
	display_path = ft_strjoin(color_path, C_RESET);
	free(color_path);
	return (display_path);
}

char	*get_display_path(t_msl *msl, char *cwd)
{
	t_env	*home_node;
	char	control;
	char	*find;
	char	*display_path;

	home_node = search_id_node(msl, "HOME");
	control = 0;
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
			control = 1;
	}
	if (!home_node || control == 1)
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
	dollar = ft_strjoin(tmp, BOOSTER);
	free(tmp);
	if (dollar)
		sys->ps1 = dollar;
	else
		sys->ps1 = ft_strdup("\001\033[0;32m\002minishell\001\033[0m\002$");
}
