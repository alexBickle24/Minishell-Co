/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_pids.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:45:28 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/14 20:07:56 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_childs1(t_msl *msl)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	while (pid != -1 && errno != EINTR)
		pid = waitpid(-1, &status, 0);
	msl->exit_status = WEXITSTATUS(status);
}

int	ft_getpid(void)
{
	int		fd;
	char	buffer[MLEN_INT];

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (-1);
	if (read(fd, buffer, MLEN_INT) == -1)
		return (-1);
	close (fd);
	return (ft_atoi(buffer));
}

void	wait_childs3(t_msl *msl)
{
	t_tocken	*c_tocken;
	pid_t		pid;
	int			status;

	c_tocken = msl->tocken;
	while (c_tocken->next)
		c_tocken = c_tocken->next;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid > 0)
			msl->last_process = pid;
		if (pid == c_tocken->pid)
		{
			if (WIFEXITED(status))
				msl->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				msl->exit_status = WTERMSIG(status) + 128;
		}
		if (pid < 0 && errno != EINTR)
			break ;
	}
	if (WIFSIGNALED(status))
		g_signal = S_EXECUTION_S;
}

void	wait_childs4(t_msl *msl, pid_t pid_heredoc)
{
	pid_t	pid;
	int		status;

	while (1)
	{
		pid = waitpid(pid_heredoc, &status, 0);
		if (pid == pid_heredoc)
		{
			if (WIFEXITED(status))
			{
				g_signal = WEXITSTATUS(status);
				if (WEXITSTATUS(status) == 2)
					msl->exit_status = WEXITSTATUS(status) + 128;
			}
		}
		if (pid < 0 && errno != EINTR)
			break ;
	}
}

int	wait_childs5(pid_t pid_child)
{
	pid_t	pid;
	int		status;
	int		ret;

	ret = 0;
	while (1)
	{
		pid = waitpid(pid_child, &status, 0);
		if (pid == pid_child)
		{
			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) != 0)
					ret = 1;
				else
					ret = 0;
			}
		}
		if (pid < 0 && errno != EINTR)
			break ;
	}
	return (ret);
}
