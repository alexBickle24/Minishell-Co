/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_pids.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:45:28 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 18:25:19 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Redirects input or output through a pipe.
 * 
 * This function redirects the specified file descriptor (`fd`) to one end
 * of a pipe. It closes the unused end of the pipe and performs the redirection
 * using `dup2`.
 * 
 * - If `pipe_port` is `0`, it redirects the read end of the pipe to `fd`.
 * - If `pipe_port` is `1`, it redirects the write end of the pipe to `fd`.
 * - Closes the unused end of the pipe before performing the redirection.
 * - If `dup2` fails, it closes the pipe and returns `1`.
 * - On success, it closes the pipe and returns `0`.
 * 
 * @param pipe_reference Pointer to the pipe file descriptors.
 * @param pipe_port The end of the pipe to redirect (`0` for read, `1` for write).
 * @param fd The file descriptor to redirect the pipe to.
 * @return `0` on success, or `1` if the redirection fails.
 */
void	wait_childs1(t_msl *msl)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	while (pid != -1 && errno != EINTR)
		pid = waitpid(-1, &status, 0);
	msl->exit_status = WEXITSTATUS(status);
}

/**
 * @brief Retrieves the process ID (PID) of the current process.
 * 
 * This function reads the `/proc/self/stat` file to obtain the PID of the
 * current process. It is useful for debugging or managing process-specific
 * operations.
 * 
 * - Opens `/proc/self/stat` in read-only mode.
 * - Reads the PID from the file into a buffer.
 * - Converts the PID from a string to an integer using `ft_atoi`.
 * - Returns `-1` if the file cannot be opened or read.
 * 
 * @return The PID of the current process, or `-1` on failure.
 */
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

/**
 * @brief Waits for all child processes and updates the shell's exit status.
 * 
 * This function waits for all child processes to terminate and updates the
 * shell's exit status based on the last executed process. It also tracks the
 * PID of the last process to finish.
 * 
 * - Iterates through the token list to find the last token.
 * - Uses `waitpid` in a loop to wait for all child processes.
 * - Updates `msl->last_process` with the PID of the last terminated process.
 * - Sets `msl->exit_status` based on the exit status or termination signal
 *   of the last process.
 * - Handles interruptions (`EINTR`) gracefully.
 * - Updates the global signal (`g_signal`) if a process is terminated by a signal.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
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

/**
 * @brief Waits for a specific child process (e.g., heredoc) to finish.
 * 
 * This function waits for a specific child process, identified by its PID,
 * to terminate. It updates the shell's exit status and global signal based
 * on the process's exit status.
 * 
 * - Uses `waitpid` to wait for the specified child process (`pid_heredoc`).
 * - Updates `g_signal` with the exit status of the process.
 * - If the process exits with status `2`, adjusts `msl->exit_status` to reflect
 *   a signal-based termination.
 * - Handles interruptions (`EINTR`) gracefully.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param pid_heredoc The PID of the heredoc process to wait for.
 */
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

/**
 * @brief Waits for a specific child process and returns its success status.
 * 
 * This function waits for a specific child process, identified by its PID,
 * to terminate. It returns `0` if the process exits successfully, or `1`
 * if the process exits with a non-zero status.
 * 
 * - Uses `waitpid` to wait for the specified child process (`pid_child`).
 * - Checks the exit status using `WIFEXITED` and `WEXITSTATUS`.
 * - Returns `1` if the process exits with a non-zero status, or `0` otherwise.
 * - Handles interruptions (`EINTR`) gracefully.
 * 
 * @param pid_child The PID of the child process to wait for.
 * @return `0` if the process exits successfully, or `1` on failure.
 */
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
