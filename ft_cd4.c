/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:48:54 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:05:39 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unlink_message(void)
{
	ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
	ft_putstr_fd("cannot access parent directories: ", 2);
	ft_putstr_fd("No such file or directory\n", 2);
}

void	ft_cderrors(int value, char *path, char flag)
{
	if (value == ENOTDIR)
	{
		ft_putstr_fd("minishell: cd: ", 2),
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Not a directory\n", 2);
	}
	if (value == ENOENT)
	{
		if (flag)
			unlink_message();
		else
		{
			ft_putstr_fd("minishell: cd: ", 2),
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
	}
	if (value == EACCES)
	{
		ft_putstr_fd("minishell: cd: ", 2),
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permision denied\n", 2);
	}
}
