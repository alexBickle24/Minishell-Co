/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:29:37 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 18:20:24 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Handles execution errors for commands or files.
 * 
 * This function is called when an error occurs while trying to execute a command
 * or access a file. It prints an error message to `stderr` and exits the process
 * with the appropriate error code:
 * - `127`: Command not found (`ENOENT`).
 * - `126`: Permission denied (`EACCES`) or if the file is a directory.
 * 
 * - If the file is a directory, it prints "Is a directory" and exits with code `126`.
 * - Otherwise, it prints the system error message using `strerror(errno)`.
 * 
 * @param file The name of the file or command that caused the error.
 * @param is_directory A flag indicating if the file is a directory (`1` if true, `0` otherwise).
 */
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

/**
 * @brief Handles errors for invalid or unknown commands.
 * 
 * This function is called when a command is not found or is invalid. It prints
 * an error message to `stderr` in the format:
 * 
 * `Minishell: <command>: command not found`
 * 
 * The process exits with error code `127`, which is used to indicate that the
 * command could not be found.
 * 
 * @param file The name of the command that caused the error.
 */
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

/**
 * @brief Handles generic errors without specifying a file or command.
 * 
 * This function is called when a generic error occurs. It prints the system
 * error message to `stderr` using `strerror(errno)` and exits the process
 * with error code `127`.
 * 
 * This is typically used for system-level errors where no specific file or
 * command is involved.
 */
void	ft_exterrno(void)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

/**
 * @brief Handles errors related to file redirections.
 * 
 * This function is called when an error occurs during input/output redirection.
 * It prints an error message to `stderr` based on the type of error:
 * - If `ambiguos == 0`, it prints the system error message using `strerror(errno)`.
 * - If `ambiguos == 1`, it prints "ambiguous redirect".
 * 
 * The error message is formatted as:
 * 
 * `Minishell: <file>: <error message>`
 * 
 * @param file The name of the file that caused the redirection error.
 * @param ambiguos A flag indicating if the error is due to an ambiguous redirect (`1` if true, `0` otherwise).
 */
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

/**
 * @brief Handles generic errors without exiting the process.
 * 
 * This function is called when a generic error occurs but the process should
 * not exit immediately. It prints the system error message to `stderr` using
 * `strerror(errno)`.
 * 
 * This is typically used for non-critical errors where the shell can continue
 * running.
 */
void	ft_errerrno(void)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}
