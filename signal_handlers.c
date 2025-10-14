/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 23:23:13 by alejandro         #+#    #+#             */
/*   Updated: 2025/09/26 23:24:40 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	sig_child_handler(int signals)
{
	signal(signals, SIG_DFL);
}

void	sig_heredoc_handler(int signals)
{
	int	i;

	i = -1;
	while (close(++i) == 0)
		close (i);
	exit (signals);
}
