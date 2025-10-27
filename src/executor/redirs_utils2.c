/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 22:14:44 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 18:23:19 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Redirects input from a file to the standard input (STDIN).
 * 
 * This function opens the specified file in read-only mode and redirects
 * its content to the standard input (`STDIN_FILENO`). It checks for file
 * existence and read permissions before proceeding.
 * 
 * - If the file does not exist or cannot be read, it returns `1`.
 * - If the file is `NULL`, it returns `2`.
 * - If the redirection (`dup2`) fails, it closes the file descriptor and returns `1`.
 * - On success, it closes the file descriptor and returns `0`.
 * 
 * @param file The name of the file to be used for input redirection.
 * @return `0` on success, `1` on failure, or `2` if the file is `NULL`.
 */
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

/**
 * @brief Redirects output to a file from the standard output (STDOUT).
 * 
 * This function opens the specified file in write mode and redirects the
 * standard output (`STDOUT_FILENO`) to the file. It supports two modes:
 * - Append mode (`O_APPEND`): Appends output to the file if it exists.
 * - Truncate mode (`O_TRUNC`): Overwrites the file if it exists.
 * 
 * - If the file is `NULL`, it returns `2`.
 * - If the file cannot be opened or created, it returns `1`.
 * - If the redirection (`dup2`) fails, it closes the file descriptor and returns `1`.
 * - On success, it closes the file descriptor and returns `0`.
 * 
 * @param file The name of the file to be used for output redirection.
 * @param append A flag indicating whether to append (`1`) or overwrite (`0`) the file.
 * @return `0` on success, `1` on failure, or `2` if the file is `NULL`.
 */
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
