/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:47:02 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:05:30 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	edit_pwd(t_msl *msl, char *path, char message)
{
	char	*tmp;

	if (message)
		unlink_message();
	tmp = ft_strjoin(msl->sys->pwd, "/");
	free(msl->sys->pwd);
	msl->sys->pwd = ft_strjoin(tmp, path);
	free(tmp);
	return (0);
}

int	go_parent(t_msl *msl, char *path)
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

int	go_root(t_msl *msl, char *pwd)
{
	if (chdir("/") == -1)
	{
		ft_cderrors(errno, "..", 0);
		free(pwd);
		return (1);
	}
	else
	{
		free(msl->sys->pwd);
		msl->sys->pwd = ft_strdup("/");
	}
	return (0);
}

int	go_firstparent(t_msl *msl, char *pwd)
{
	if (chdir(pwd) == -1)
	{
		ft_cderrors(errno, "..", 0);
		free(pwd);
		return (1);
	}
	else
	{
		free(msl->sys->pwd);
		msl->sys->pwd = ft_strdup(pwd);
	}
	return (0);
}

int	is_broken_pwd(t_msl *msl)
{
	int	i;

	i = 0;
	while (msl->sys->pwd[i] != '\0')
	{
		if (msl->sys->pwd[i] != '.' && msl->sys->pwd[i] != '/')
			return (0);
		i++;
	}
	return (1);
}
