/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 23:23:13 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:43:46 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles `SIGINT` (Ctrl+C) signals in the main shell process.
 * 
 * This function updates the global signal state (`g_signal`) and performs
 * different actions based on the current state:
 * - If `g_signal` is `S_INIT` or `S_SIGINT`, it resets the input line,
 *   displays a new prompt, and redisplays the shell.
 * - If `g_signal` is `S_HEREDOC`, it interrupts heredoc input and displays
 *   a `^C` message.
 * 
 * @param signal The signal number (unused in this function).
 */
void	sig_handler(int signal)
{
	(void)signal;
	if (g_signal == S_INIT || g_signal == S_SIGINT)
	{
		g_signal = S_SIGINT;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (g_signal == S_HEREDOC)
	{
		write(1, "^C", 2);
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

/**
 * @brief Handles signals in child processes.
 * 
 * This function resets the signal handling behavior for child processes
 * to the default (`SIG_DFL`). This ensures that child processes do not
 * inherit the custom signal handling behavior of the main shell process.
 * 
 * @param signals The signal number to reset.
 */
void	sig_child_handler(int signals)
{
	signal(signals, SIG_DFL);
}

/**
 * @brief Handles `SIGINT` (Ctrl+C) signals during heredoc input.
 * 
 * This function closes all open file descriptors and exits the process
 * with the signal number. It ensures that heredoc input is interrupted
 * cleanly and does not leave open resources.
 * 
 * @param signals The signal number that triggered the handler.
 */
void	sig_heredoc_handler(int signals)
{
	int	i;

	i = -1;
	while (close(++i) == 0)
		close (i);
	exit (signals);
}
