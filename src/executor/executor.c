/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:49:44 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 18:06:05 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Executes shell tokens in the execution phase.
 * 
 * This function serves as the main executor for shell tokens in the execution
 * phase. It determines whether to execute a single built-in command or a
 * sequence of commands, manages the global signal state, and handles cleanup
 * after execution.
 * 
 * - If there is only one token and it is a built-in, it calls `only_builtin`.
 * - Otherwise, it calls `execute_orders` to handle multiple commands.
 * - Updates the global signal state (`g_signal`) to reflect the execution phase.
 * - Sets the last argument (`_` environment variable) using `set_last_arg`.
 * - Frees the token list and resets parsing errors after execution.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
void	executer(t_msl *msl)
{
	char	*last_arg;

	last_arg = NULL;
	if (msl && g_signal == S_INIT && msl->tocken != NULL)
	{
		g_signal = S_EXECUTION;
		last_arg = set_last_arg(msl);
		if (msl->total_tockens == 1 && is_builtin(msl->tocken))
			only_builtin(msl);
		else
			execute_orders(msl);
	}
	if (g_signal != S_EXECUTION_S)
		g_signal = S_INIT;
	create_last_arg(msl, last_arg);
	msl->pars_err = 0;
	free_tockens(msl);
}

/**
 * @brief Executes a single built-in command.
 * 
 * This function handles the execution of a single built-in command without
 * forking a child process. It manages input/output redirections and restores
 * the original file descriptors after execution.
 * 
 * - Saves the current `stdin` and `stdout` file descriptors.
 * - Handles input and output redirections using `fordward_in` and `fordward_out`.
 * - Executes the built-in command using `father_builtin`.
 * - Restores the original file descriptors after execution.
 * - Sets the shell's exit status based on the result of the built-in command.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
void	only_builtin(t_msl *msl)//
{
	t_tocken	*c_tocken;
	int			saved_stdin;
	int			saved_stdout;

	c_tocken = msl->tocken;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	check_create_redirs(c_tocken, c_tocken->files);
	fordward_in(c_tocken);
	fordward_out(c_tocken);
	if (c_tocken->error_file != 1)
		msl->exit_status = father_builtin(msl, c_tocken, is_builtin(c_tocken));
	else
		msl->exit_status = 1;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

/**
 * @brief Executes a sequence of commands.
 * 
 * This function iterates through the token list and executes each command
 * in a separate child process. It waits for all child processes to finish
 * after executing the commands.
 * 
 * - Calls `execute_childs` for each token to handle individual command execution.
 * - Waits for all child processes using `wait_childs3`.
 * 
 * @param msl Pointer to the main structure of the shell.
 */
void	execute_orders(t_msl *msl)
{
	t_tocken	*c_tocken;

	c_tocken = msl->tocken;
	while (c_tocken)
	{
		execute_childs(c_tocken, msl);
		c_tocken = c_tocken->next;
	}
	wait_childs3(msl);
}

/**
 * @brief Executes a single command in a child process.
 * 
 * This function forks a child process to execute a command. It handles
 * input/output redirections, pipes, and built-in commands. It also evaluates
 * errors in the token's commands before execution.
 * 
 * - The first three steps (checking redirections, evaluating command errors,
 *   and creating pipes) occur in the memory of the parent process.
 * - Checks and creates redirections using `check_create_redirs`.
 * - Evaluates command errors using `evaluate_tocken_cmds_errors` if the command
 *   is not a built-in.
 * - Creates pipes for inter-process communication if necessary.
 * - Forks a child process to execute the command.
 * - In the child process, calls `child_process` to handle execution.
 * - Closes file descriptors for pipes in the parent process.
 * - Stores the child process ID in the token structure.
 * 
 * @param c_tocken Pointer to the current token being executed.
 * @param msl Pointer to the main structure of the shell.
 */
void	execute_childs(t_tocken *c_tocken, t_msl *msl)
{
	pid_t	pid;
	int		s_builtin;

	s_builtin = is_builtin(c_tocken);
	check_create_redirs(c_tocken, c_tocken->files);
	if (!s_builtin)
		evaluate_tocken_cmds_errors(c_tocken, msl);
	if (msl->total_tockens - c_tocken->index != 0)
		create_pipes(c_tocken->next);
	pid = fork();
	if (pid < 0)
		ft_errerrno();
	if (pid == 0)
		child_process(s_builtin, c_tocken, msl);
	if (c_tocken->index != 1)
		close_fds(c_tocken->pipe_fds);
	c_tocken->pid = pid;
}

/**
 * @brief Executes a command or built-in in a child process.
 * 
 * This function handles the execution of a command or built-in in a child
 * process. It sets up input/output redirections, initializes signal handling,
 * and executes the command or built-in.
 * 
 * - Handles input and output redirections using `fordward_in` and `fordward_out`.
 * - Initializes signal handling for the child process using `signal_init_childs`.
 * - If there are file errors, the process exits with status `1`.
 * - If the command is a built-in, it calls `cmd_vs_builtin` to execute it.
 * - If no commands are present, the process exits with status `0`.
 * 
 * @param s_builtin Indicates if the command is a built-in (non-zero if true).
 * @param c_tocken Pointer to the current token being executed.
 * @param msl Pointer to the main structure of the shell.
 */
void	child_process(int s_builtin, t_tocken *c_tocken, t_msl *msl)
{
	fordward_in(c_tocken);
	fordward_out(c_tocken);
	signal_init_childs();
	if (c_tocken->error_file != 0)
		exit (1);
	if (c_tocken->pcmds)
		cmd_vs_builtin(msl, c_tocken, s_builtin);
	else
		exit(0);
}
