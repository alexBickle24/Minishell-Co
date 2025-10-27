/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:45:08 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:56:16 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Executes a command and retrieves its output into a target string.
 * 
 * This function runs a command with optional arguments in a child process,
 * captures its output through a pipe, and stores the result in the `target`
 * string. It is useful for retrieving system information or command results.
 * 
 * - Creates a pipe for inter-process communication.
 * - Forks a child process to execute the command using `execute_cmd`.
 * - Waits for the child process to finish using `wait_childs5`.
 * - Reads the command's output from the pipe and stores it in `target`.
 * - Removes the trailing newline character from the output.
 * - Handles errors such as pipe creation or fork failure.
 * 
 * @note This function may need to be fixed to handle potential buffer overflows
 * if `target_len` is `0`.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param target Pointer to the string where the command output will be stored.
 * @param command The command to be executed.
 * @param args Optional arguments for the command.
 * @return `0` on success, or a non-zero value on failure.
 */
int	get_infcmds(t_msl *msl, char **target, char *command, char *args)
{
	pid_t	pid;
	int		pipe_fds[2];
	int		ret;

	if (pipe(pipe_fds) == -1)
		ft_errerrno();
	pid = fork();
	if (pid < 0)
		ft_errerrno();
	if (pid == 0)
		execute_cmd(command, args, pipe_fds, ft_env_to_table(msl->own_env));
	ret = wait_childs5(pid);
	if (ret == 0)
	{
		close(pipe_fds[1]);
		*target = get_next_line(pipe_fds[0]);
		close(pipe_fds[0]);
		if (*target == NULL)
			return (1);
		(*target)[ft_strlen(*target) - 1] = '\0';
		return (ret);
	}
	close_fds(pipe_fds);
	return (ret);
}

/**
 * @brief Finds the path of a command using the `which` utility.
 * 
 * This function runs the `which` command in a child process to locate the
 * full path of a given command. The result is returned as a string.
 * 
 * - Creates a pipe for inter-process communication.
 * - Forks a child process to execute the `which` command using `execute_cmd`.
 * - Waits for the child process to finish using `wait_childs5`.
 * - Reads the command's path from the pipe and returns it as a string.
 * - Removes the trailing newline character from the output.
 * - Handles errors such as pipe creation or fork failure.
 * 
 * @note This function may need to be fixed to handle potential buffer overflows
 * if `target_len` is `0`.
 * 
 * @param msl Pointer to the main structure of the shell.
 * @param command The command to locate.
 * @param args Optional arguments for the `which` command.
 * @return A string containing the full path of the command, or `NULL` if not found.
 */
char	*which_cmd(t_msl *msl, char *command, char *args)
{
	pid_t	pid;
	int		pipe_fds[2];
	char	*where;

	if (pipe(pipe_fds) == -1)
		ft_errerrno();
	pid = fork();
	if (pid < 0)
		ft_errerrno();
	if (pid == 0)
		execute_cmd(command, args, pipe_fds, ft_env_to_table(msl->own_env));
	if (wait_childs5(pid) == 0)
	{
		close(pipe_fds[1]);
		where = get_next_line(pipe_fds[0]);
		close(pipe_fds[0]);
		if (where == NULL)
			return (NULL);
		where[ft_strlen(where) - 1] = '\0';
		return (where);
	}
	close_fds(pipe_fds);
	return (NULL);
}

/**
 * @brief Executes a command with arguments and redirects its output to a pipe.
 * 
 * This function is called in a child process to execute a command using `execve`.
 * It redirects the standard output to a pipe for inter-process communication.
 * 
 * - Creates an argument array using `create_args`.
 * - Redirects the pipe's write end to `STDOUT_FILENO` using `pipe_forward`.
 * - Executes the command with `execve` using the provided arguments and environment.
 * - Frees allocated memory for arguments and environment variables on failure.
 * - Exits the child process with a non-zero status if `execve` fails.
 * 
 * @param command The command to be executed.
 * @param option Optional arguments for the command.
 * @param pipes Array of file descriptors for the pipe.
 * @param env Array of environment variables for the command.
 */
void	execute_cmd(char *command, char *option, int *pipes, char **env)
{
	char	**args;

	args = create_args(command, option);
	if (args == NULL)
		return ;
	pipe_forward(pipes, 1, STDOUT_FILENO);
	if (execve(command, args, env) == -1)
	{
		ft_free_table(env);
		ft_free_table(args);
		exit(1);
	}
}

/**
 * @brief Creates an argument array for a command.
 * 
 * This function allocates memory for an array of strings representing the
 * command and its optional arguments. The array is null-terminated.
 * 
 * - If no arguments are provided, the array contains only the command.
 * - If an argument is provided, the array contains the command and the argument.
 * - Allocates memory for the array and duplicates the strings using `ft_strdup`.
 * - Returns `NULL` if memory allocation fails.
 * 
 * @param command The command to be executed.
 * @param option Optional argument for the command.
 * @return A null-terminated array of strings representing the command and its arguments.
 */
char	**create_args(char *command, char *option)
{
	char	**args;

	if (option == NULL)
	{
		args = (char **)malloc(sizeof (char *) * 2);
		if (args == NULL)
			return (NULL);
		args[0] = ft_strdup(command);
		args[1] = NULL;
	}
	else
	{
		args = (char **)malloc(sizeof (char *) * 3);
		if (args == NULL)
			return (NULL);
		args[0] = ft_strdup(command);
		args[1] = ft_strdup(option);
		args[2] = NULL;
	}
	return (args);
}
