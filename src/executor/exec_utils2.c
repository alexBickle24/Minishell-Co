/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:48:27 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 18:17:18 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Searches for an executable file in a list of directories.
 * 
 * This function iterates through a list of possible directories and checks
 * if the command exists and is accessible in any of them. If found, it returns
 * the full path to the executable.
 * 
 * - Combines each directory with the command name to form a full path.
 * - Checks if the file exists and is accessible using `access`.
 * - Frees intermediate memory allocations for each path.
 * 
 * @param posible_paths Array of directories to search in.
 * @param x_file The command to search for.
 * @return The full path to the executable, or `NULL` if not found.
 */
void	exec_cmd(t_tocken *c_tocken)
{
	int		error;
	t_pcmds	*pcmds;

	pcmds = c_tocken->pcmds;
	error = c_tocken->error_cmd;
	if (error == 1)
		ft_exterror_exes(c_tocken->cmd_tb[0], 0);
	else if (error == 2)
		ft_exterror_cmd(c_tocken->cmd_tb[0]);
	else if (error == 3)
		ft_exterror_exes(c_tocken->cmd_tb[0], 1);
	else if (error == 0)
	{
		if (execve(pcmds->cmd, c_tocken->cmd_tb, c_tocken->env_tb) == -1)
			ft_exterrno();
		else
			exit(0);
	}
}

/**
 * @brief Checks if a command is a built-in shell command.
 * 
 * This function compares the first command in the token's `pcmds` list with
 * the names of built-in shell commands (e.g., `echo`, `cd`, `pwd`, etc.).
 * If the command matches a built-in, it returns a corresponding integer code.
 * 
 * - Built-in commands and their codes:
 *   - `echo`: 1
 *   - `cd`: 2
 *   - `pwd`: 3
 *   - `export`: 4
 *   - `unset`: 5
 *   - `env`: 6
 *   - `exit`: 7
 * 
 * @param tocken Pointer to the token containing the command to check.
 * @return An integer code representing the built-in command, or `0` if not a built-in.
 */
int	is_builtin(t_tocken *tocken)
{
	if (!tocken->pcmds)
		return (0);
	if (!ft_strncmp(tocken->pcmds->cmd, "echo\0", 5))
		return (1);
	if (!ft_strncmp(tocken->pcmds->cmd, "cd\0", 3))
		return (2);
	if (!ft_strncmp(tocken->pcmds->cmd, "pwd\0", 4))
		return (3);
	if (!ft_strncmp(tocken->pcmds->cmd, "export\0", 7))
		return (4);
	if (!ft_strncmp(tocken->pcmds->cmd, "unset\0", 6))
		return (5);
	if (!ft_strncmp(tocken->pcmds->cmd, "env\0", 4))
		return (6);
	if (!ft_strncmp(tocken->pcmds->cmd, "exit\0", 5))
		return (7);
	return (0);
}

/**
 * @brief Executes a built-in command in the parent process.
 * 
 * This function handles the execution of built-in commands without forking
 * a child process. It calls the corresponding function for each built-in
 * command and returns the result.
 * 
 * - Built-in commands:
 *   - `echo`: Calls `ft_echo`.
 *   - `cd`: Calls `ft_cd`.
 *   - `pwd`: Calls `ft_pwd`.
 *   - `export`: Calls `ft_export`.
 *   - `unset`: Calls `ft_unset`.
 *   - `env`: Calls `ft_env`.
 *   - `exit`: Calls `ft_exit` and terminates the shell.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param c_tocken Pointer to the current token containing the command.
 * @param builtin The integer code representing the built-in command.
 * @return The result of the built-in command execution.
 */
int	father_builtin(t_msl *msl, t_tocken *c_tocken, int builtin)
{
	if (builtin == 1)
		return (ft_echo(c_tocken));
	else if (builtin == 2)
		return (ft_cd(msl, c_tocken->pcmds));
	else if (builtin == 3)
		return (ft_pwd(msl));
	else if (builtin == 4)
		return (ft_export(msl, c_tocken->pcmds));
	else if (builtin == 5)
		return (ft_unset(msl, c_tocken));
	else if (builtin == 6)
		return (ft_env(msl, c_tocken->pcmds));
	else if (builtin == 7)//
		ft_exit(msl, c_tocken, 1);
	return (0);
}

/**
 * @brief Creates a pipe for inter-process communication.
 * 
 * This function creates a pipe and stores its file descriptors in the
 * `pipe_fds` field of the current token. If the pipe creation fails, it
 * calls `ft_errerrno` to handle the error.
 * 
 * @param c_tocken Pointer to the current token for which the pipe is created.
 */
void	create_pipes(t_tocken *c_tocken)
{
	if (pipe(c_tocken->pipe_fds) == -1)
		ft_errerrno();
}

/**
 * @brief Executes a built-in command or an external command in a child process.
 * 
 * This function determines whether the command is a built-in or an external
 * command. If it is a built-in, it executes the corresponding function and
 * exits the child process with the result. If it is not a built-in, it calls
 * `exec_cmd` to execute the external command.
 * 
 * - Built-in commands:
 *   - `echo`: Calls `ft_echo`.
 *   - `cd`: Calls `ft_cd`.
 *   - `pwd`: Calls `ft_pwd`.
 *   - `export`: Calls `ft_export`.
 *   - `unset`: Calls `ft_unset`.
 *   - `env`: Calls `ft_env`.
 *   - `exit`: Calls `ft_exit` and terminates the child process.
 * - If the command is not a built-in, it calls `exec_cmd`, which uses `execve`
 *   to execute the binary. `execve` replaces the current child process with
 *   the new process specified by the command.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param c_tocken Pointer to the current token containing the command.
 * @param builtin The integer code representing the built-in command.
 */
void	cmd_vs_builtin(t_msl *msl, t_tocken *c_tocken, int builtin)
{
	if (builtin == 1)
		exit (ft_echo(c_tocken));
	else if (builtin == 2)
		exit (ft_cd(msl, c_tocken->pcmds));
	else if (builtin == 3)
		exit (ft_pwd(msl));
	else if (builtin == 4)
		exit(ft_export(msl, c_tocken->pcmds));
	else if (builtin == 5)
		exit (ft_unset(msl, c_tocken));
	else if (builtin == 6)
		exit (ft_env(msl, c_tocken->pcmds));
	else if (builtin == 7)
		ft_exit (msl, c_tocken, 0);
	else
		exec_cmd(c_tocken);
}
