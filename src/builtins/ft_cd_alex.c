/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_alex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicalons <vicalons@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 00:54:28 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/12 19:33:31 by vicalons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_cd(t_msl *msl, t_pcmds *pcmds)
{
	int		nbr_tockens;
	char	succes;
	char	*old_pwd;

	succes = 0;
	old_pwd = ft_strdup(msl->sys->pwd);
	nbr_tockens = ft_argscounter(pcmds);
	if (nbr_tockens > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (nbr_tockens == 1)
		succes = go_home(msl);
	else if (nbr_tockens == 2)
		succes = one_arg_cases(msl, pcmds);
	if (succes == 0)
		set_pwd_oldpwd(msl, old_pwd);
	else
		free(old_pwd);
	if (succes == 2)
		return (0);
	return (succes);
}

int	one_arg_cases(t_msl *msl, t_pcmds *pcmds)
{
	char	*path;

	path = pcmds->next->cmd;
	if (ft_strncmp(path, ".\0", 2) == 0)
		return (stay_case(path, msl));
	else if (ft_strncmp(path, "..\0", 3) == 0)
		return (parent_case(path, msl));
	else if (ft_strncmp(path, "-\0", 2) == 0)
		return (go_oldpwd(msl));
	else
	{
		if (chdir(path) == -1)
		{
			ft_cderrors(errno, path, 0);
			return (1);
		}
		else
		{
			free(msl->sys->pwd);
			msl->sys->pwd = getcwd(NULL, 0);
			return (0);
		}
	}
}

int	go_home(t_msl *msl)
{
	t_env	*home_node;

	home_node = search_id_node(msl, "HOME");
	if (home_node == NULL)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	else
	{
		if (home_node->value[0] == '\0')
			return (1);
		else
		{
			if (chdir(home_node->value) == -1)
			{
				ft_cderrors(errno, home_node->value, 0);
				return (1);
			}
			else
			{
				free(msl->sys->pwd);
				msl->sys->pwd = ft_strdup(home_node->value);
				return (0);
			}
		}
	}
}

int	stay_case(char *path, t_msl *msl)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL && is_broken_pwd(msl))
		return (1);
	if (cwd == NULL && errno == ENOENT)
		return (edit_pwd(msl, path, 1));
	else
	{
		if (chdir(path) == -1)
		{
			ft_cderrors(errno, path, 0);
			free(cwd);
			return (1);
		}
		else
		{
			free(msl->sys->pwd);
			msl->sys->pwd = cwd;
			return (0);
		}
	}
}

int	parent_case(char *path, t_msl *msl)
{
	int	flag;

	if (is_broken_pwd(msl))
	{
		if (ft_strlen(msl->sys->pwd) >= 2)
			return (edit_pwd(msl, path, 0));
		if (ft_strlen(msl->sys->pwd) == 1 && *(msl->sys->pwd) != '/')
		{
			free(msl->sys->pwd);
			msl->sys->pwd = ft_strdup("..");
			return (0);
		}
	}
	if (msl->sys->pwd[ft_strlen(msl->sys->pwd) - 1] == '.')
		return (search_first_parent(msl));
	flag = check_parent(msl);
	if (flag == 1)
		return (edit_pwd(msl, path, 1));
	else if (flag == 0)
		go_parent(msl, path);
	else if (flag == 2)
		return (2);
	return (0);
}
