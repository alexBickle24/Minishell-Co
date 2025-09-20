/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:29:37 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/20 19:30:57 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//FUNCoines de erro y codigo de salida para capturar con waitstatus ejecucion
//error cmd = 1
void	ft_exterror_exes(char *file, char is_directory)
{
	int	error_code;

	if (!file)
		return ;
	if (errno == ENOENT)
		error_code = 127;
	else if (errno == EACCES)
		error_code = 126;
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	if (is_directory)
	{
		ft_putstr_fd("Is a directory\n", 2);
		exit (126);
	}
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	exit(error_code);
}

//error cmd == 2
void	ft_exterror_cmd(char *file)
{
	if (!file)
		return ;
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("command not found", 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

//error sin salida
void	ft_exterrno(void)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

void	ft_error_redirs(char *file, char ambiguos)
{
	if (!file)
		return ;
	if (ambiguos == 0)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	else
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("ambiguous redirect", 2);
		ft_putstr_fd("\n", 2);
	}
}

//error generico con salida
void	ft_errerrno(void)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}
