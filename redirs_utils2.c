/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 22:14:44 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:07:27 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tunel_in_file(char *file)
{
	int	fd_trgt;

	if (!file)
		return (2);
	if (access(file, F_OK | R_OK) == -1)
		return (1);
	fd_trgt = open(file, O_RDONLY, 0664);
	if (fd_trgt < 0)
		return (1);
	if (dup2(fd_trgt, STDIN_FILENO) == -1)
	{
		close(fd_trgt);
		return (1);
	}
	close(fd_trgt);
	return (0);
}

int	tunel_out_file(char *file, char append)
{
	int	fd_trgt;

	if (!file)
		return (2);
	if (append)
		fd_trgt = open(file, O_WRONLY | O_CREAT | O_APPEND, 0664);
	else
		fd_trgt = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_trgt < 0)
		return (1);
	if (dup2(fd_trgt, STDOUT_FILENO) == -1)
	{
		close(fd_trgt);
		return (1);
	}
	close(fd_trgt);
	return (0);
}

int	pipe_forward(int *pipe_reference, int pipe_port, int fd)
{
	int		other_fd;

	if (pipe_port == 0)
		other_fd = 1;
	else
		other_fd = 0;
	close(pipe_reference[other_fd]);
	if (dup2(pipe_reference[pipe_port], fd) == -1)
		return (close(pipe_reference[pipe_port]), 1);
	return (close(pipe_reference[pipe_port]), 0);
}
