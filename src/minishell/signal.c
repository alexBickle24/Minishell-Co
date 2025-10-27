/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:34:47 by alejandro         #+#    #+#             */
/*   Updated: 2025/10/27 17:43:09 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Initializes signal handling for the main shell process.
 * 
 * This function sets up signal handlers for the main shell process:
 * - `SIGINT` (Ctrl+C): Calls `sig_handler` to handle interruptions.
 * - `SIGQUIT` (Ctrl+\): Ignores the signal.
 * 
 * The global signal state (`g_signal`) is initialized to `S_INIT`.
 * 
 * @note This ensures that the shell can handle user interruptions gracefully
 * while ignoring quit signals.
 */
void	signal_init(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	g_signal = S_INIT;
	sa_int.sa_handler = sig_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * @brief Initializes signal handling for child processes.
 * 
 * This function sets up signal handlers for child processes:
 * - `SIGINT` (Ctrl+C): Calls `sig_child_handler` to handle interruptions.
 * - `SIGQUIT` (Ctrl+\): Calls `sig_child_handler` to handle quit signals.
 * 
 * @note This ensures that child processes handle signals independently
 * and can reset their behavior to the default signal handling.
 */
void	signal_init_childs(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sig_child_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sig_child_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * @brief Initializes signal handling for heredoc input.
 * 
 * This function sets up signal handlers for heredoc processing:
 * - `SIGINT` (Ctrl+C): Calls `sig_heredoc_handler` to handle interruptions.
 * - `SIGQUIT` (Ctrl+\): Ignores the signal.
 * 
 * @note This ensures that heredoc input can be interrupted gracefully
 * without affecting the main shell process.
 */
void	signal_init_heredoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sig_heredoc_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
