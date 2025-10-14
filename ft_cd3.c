/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:30:08 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:05:35 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_parent(t_msl *msl)
{
	char		*bar;
	struct stat	st;

	bar = ft_strrchr(msl->sys->pwd, '/');
	if (bar == NULL)
		return (3);
	if (bar == msl->sys->pwd && bar[1] == 0)
		return (0);
	else
	{
		if (bar != msl->sys->pwd)
			*bar = '\0';
		if (stat(msl->sys->pwd, &st) == -1)
		{
			*bar = '/';
			return (1);
		}
		else
		{
			*bar = '/';
			return (0);
		}
	}
}

int	search_first_parent(t_msl *msl)
{
	struct stat	st;
	char		*tmp1;
	char		*pwd;

	pwd = ft_strdup(msl->sys->pwd);
	while (1)
	{
		tmp1 = ft_strrchr(pwd, '/');
		if (tmp1 != pwd)
			*tmp1 = '\0';
		else if (tmp1 == pwd)
		{
			if (go_root(msl, pwd) == 1)
				return (1);
			break ;
		}
		if (stat(pwd, &st) != -1)
		{
			if (go_firstparent(msl, pwd) == 1)
				return (1);
			break ;
		}
	}
	free(pwd);
	return (0);
}

void	set_olpwd(t_msl *msl, char *oldpath)
{
	t_env	*old_pwd;

	old_pwd = search_id_node(msl, "OLDPWD");
	if (!old_pwd)
	{
		if (msl->builts->unst_oldpwd == 0)
		{
			old_pwd = list_new_ownenv(ft_strdup("OLDPWD"), oldpath);
			if (!old_pwd)
			{
				free(oldpath);
				return ;
			}
			list_addback_env(old_pwd, &(msl->own_env));
		}
		else
			free(oldpath);
	}
	else
	{
		free(old_pwd->value);
		old_pwd->value = oldpath;
	}
}

void	set_pwd_oldpwd(t_msl *msl, char *oldpath)
{
	t_env	*pwd;
	t_env	*old_pwd;

	pwd = search_id_node(msl, "PWD");
	if (!pwd)
	{
		free(oldpath);
		old_pwd = search_id_node(msl, "OLDPWD");
		if (old_pwd)
		{
			free(old_pwd->value);
			old_pwd->value = (char *)ft_calloc(sizeof(char), 1);
		}
		return ;
	}
	else
	{
		free(pwd->value);
		pwd->value = ft_strdup(msl->sys->pwd);
		set_olpwd(msl, oldpath);
	}
}

int	go_oldpwd(t_msl *msl)
{
	t_env	*oldpwd;

	oldpwd = search_id_node(msl, "OLDPWD");
	if (oldpwd != NULL && *(oldpwd->value))
	{
		if (chdir(oldpwd->value) == -1)
		{
			ft_cderrors(errno, oldpwd->value, 0);
			return (1);
		}
		else
		{
			free(msl->sys->pwd);
			msl->sys->pwd = ft_strdup(oldpwd->value);
			ft_putendl_fd(msl->sys->pwd, 1);
			return (0);
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
}
